#include <stdio.h>
#include <stdint.h>
#include <io.h>
#include <fcntl.h>
#include <memory.h> // memset

// MATLAB code dll load
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"

// Viterbi algo
#include "vtb27.h"
#include "vtb27_types.h"

// Derandomization PN Lookup Table
#include "pn.h"

// Reed-Solomon algo
#include "rs255223.h"
#include "rs255223_types.h"

#define FRAMESIZE 1024
#define SYNCWORDSIZE 32
#define MINCORRELATIONBITS 46
#define FRAMEBITS (FRAMESIZE * 8)
#define RSBLOCKS 4
#define PARITY_OFFSET 892

#define VITPOLYA 0x4F
#define VITPOLYB 0x6D

#define CODEDFRAMESIZE (FRAMEBITS * 2)

typedef struct
{
    uint32_t uw0mc;
    uint32_t uw0p;
    uint32_t uw2mc;
    uint32_t uw2p;
} correlation_t;

// bear in mind that uw2 is actually the encoding result of 1ACFFC1D
const uint64_t UW0 = 0xfca2b63db00d9794; // 0 degrees inverted phase shift
const uint64_t UW2 = 0x035d49c24ff2686b; // 180 degrees inverted phase shift

uint8_t UW0b[64]; // The Encoded UW0
uint8_t UW2b[64]; // The Encoded UW2

void initUW()
{
    printf("Converting Sync Words to Soft Data\n");
    for (int i = 0; i < 64; i++)
    {
        UW0b[i] = (UW0 >> (64 - i - 1)) & 1 ? 0xFF : 0x00;
        UW2b[i] = (UW2 >> (64 - i - 1)) & 1 ? 0xFF : 0x00;
    }
}

uint32_t hardCorrelate(uint8_t dataByte, uint8_t wordByte)
{
    //1 if (a > 127 and b == 255) or (a < 127 and b == 0) else 0
    return (dataByte >= 127 & wordByte == 0) | (dataByte < 127 & wordByte == 255);
}

void resetCorrelation(correlation_t *corr)
{
    memset(corr, 0x00, sizeof(correlation_t));
}

uint32_t maxCorrelation(uint32_t a, uint32_t b)
{
    return (a > b ? a : b);
}

void checkCorrelation(uint8_t *buffer, int buffLength, correlation_t *corr)
{
    initUW();
    resetCorrelation(corr);

    for (int i = 0; i < buffLength - 64; i++)
    {
        uint32_t uw0c = 0;
        uint32_t uw2c = 0;

        for (int k = 0; k < 64; k++)
        {
            uw0c += hardCorrelate(buffer[i + k], UW0b[k]);
            uw2c += hardCorrelate(buffer[i + k], UW2b[k]);
        }

        corr->uw0p = uw0c > corr->uw0mc ? i : corr->uw0p;
        corr->uw2p = uw2c > corr->uw2mc ? i : corr->uw2p;

        corr->uw0mc = uw0c > corr->uw0mc ? uw0c : corr->uw0mc;
        corr->uw2mc = uw2c > corr->uw2mc ? uw2c : corr->uw2mc;
    }
}

#define CSIZE 16

int readDataForChunk(uint8_t *data, uint32_t size, int sockfd)
{
    int readBytes = 0;
    int chunkSize;
    int n;
    int retry = 0;
    while (readBytes < size)
    {
        chunkSize = (size - readBytes) > CSIZE ? CSIZE : size - readBytes;
        n = _read(sockfd, data + readBytes, chunkSize);
        if (n > 0)
        {
            readBytes += n;
        }
        else
        {
            return -1;
        }
    }
    return readBytes;
}

void fixPacket(uint8_t *buffer, int buffLength, uint8_t n)
{
    if (n == 4)
    {
        for (int i = 0; i < buffLength; i += 2)
        {
            // Process 180 phase shift, aka inverted bits
            buffer[i] ^= 0xFF;
            buffer[i + 1] ^= 0xFF;
        }
    }
}

void vtb27_main(uint8_t codedData[], uint8_t *decodedData)
{
    uint8_t temp[FRAMESIZE * 8];
    // return data is 1 uint8 represent 1 bit.
    vtb27(codedData, temp);
    // concatenate the temp[FRAMESIZE*8] to decodedData[FRAMESIZE].
    for (int i = 0; i < FRAMESIZE; i++)
    {
        uint8_t t = 0;
        for (int j = 0; j < 8; j++)
        {
            t <<= 1;
            t |= temp[8 * i + j];
        }
        decodedData[i] = t;
    }
}

void deinterleaveRS(uint8_t *data, uint8_t *rsbuff, uint8_t currentRSFrameNum)
{
    // LRIT uses (255/223, 4) as parameters. Each of 255 bytes where 223 of them are data, the rest are parity.
    // TotalRSFrameNum = 4, and currentRSFrameNum is looping from 0~3.
    // Copy data byte
    for (int i = 0; i < 223; i++)
    {
        rsbuff[i] = data[i * RSBLOCKS + currentRSFrameNum];
    }
    // Copy parity byte
    for (int i = 0; i < 32; i++)
    {
        rsbuff[i + 223] = data[PARITY_OFFSET + i * RSBLOCKS + currentRSFrameNum];
    }
}

void interleaveRS(uint8_t *indata, uint8_t *outbuff, uint8_t currentRSFrameNum)
{
    // interleave data
    for (int i = 0; i < 233; i++)
    {
        outbuff[i * RSBLOCKS + currentRSFrameNum] = indata[i];
    }

    // NO interleave parity, because MATLAB function only returns the corrected data, without parity.
    // for (int i = 0; i < 32; i++)
    // {
    //     outbuff[PARITY_OFFSET + i * RSBLOCKS + currentRSFrameNum] = indata[i + 223];
    // }
}

int main()
{
    uint8_t codedData[CODEDFRAMESIZE];
    uint8_t decodedData[FRAMESIZE];
    int newsockfd = _open("../Demod data/rec_128", _O_BINARY, O_RDONLY);

    int cnt = 0;
    while (cnt++ < 6)
    {
        printf("loop %d\n", cnt);
        printf("sockfd = %d\n", _tell(newsockfd));
        // Read Data
        uint32_t chunkSize = CODEDFRAMESIZE;
        memset(codedData, 0x00, chunkSize);
        int dataRead = readDataForChunk(codedData, chunkSize, newsockfd);
        if (dataRead == -1)
        {
            printf("Client dropped connection!\n");
            return -1;
        }

        correlation_t corr;
        // Check Correlation
        checkCorrelation(codedData, chunkSize, &corr);
        // Get Max Correlation
        uint32_t maxCorr = maxCorrelation(corr.uw0mc, corr.uw2mc);

        if (maxCorr < MINCORRELATIONBITS)
        {
            printf("  Skipping read. Correlation %d less than required %d.\n", maxCorr, MINCORRELATIONBITS);
        }
        else
        {
            // Check Phase Shifting and Position
            uint8_t n;
            uint32_t p;
            uint32_t mc;

            if (maxCorr == corr.uw0mc)
            {
                n = 0;
                p = corr.uw0p;
                mc = corr.uw0mc;
            }
            else if (maxCorr == corr.uw2mc)
            {
                n = 4;
                p = corr.uw2p;
                mc = corr.uw2mc;
            }
            printf("p = %d\n\n", p);

            // Shift data with its syncword at the beginning
            if (p != 0)
            {
                // Shift position
                char *shiftedPosition = codedData + p;
                memcpy(codedData, shiftedPosition, CODEDFRAMESIZE - p); // Copy from p to chunk size to start of codedData

                // readsize += chunkSize; // Add what we processed to readsize.

                uint32_t oldChunkSize = chunkSize;
                chunkSize = p; // Read needed bytes to fill a frame.
                readDataForChunk(codedData + CODEDFRAMESIZE - p, chunkSize, newsockfd);
            }

            // Correct Frame Phase
            //printf("  Fixing packet.\n");
            fixPacket(codedData, CODEDFRAMESIZE, n);

            // Viterbi Decoding
            vtb27_main(codedData, decodedData);

            // Test Viterbi Output
            for (int i = 0; i < 10; i++)
            {
                printf("%02hhX ", decodedData[i]);
            }
            printf("\n");

            // Derandomization
            uint8_t skipSyncWord = SYNCWORDSIZE / 8;
            memcpy(decodedData, decodedData + skipSyncWord, FRAMESIZE - skipSyncWord);
            for (int i = 0; i < FRAMESIZE - skipSyncWord; i++)
            {
                decodedData[i] ^= pn[i % 255];
            }

            // Test output

            // Reed-Solomon, 1 RSFrame at a time
            uint8_t RSbuff[255];
            uint8_t RScorrected[223];
            int8_t err;
            for (int i = 0; i < RSBLOCKS; i++)
            {
                deinterleaveRS(decodedData, RSbuff, i);
                rs255223(RSbuff, RScorrected, &err);
                interleaveRS(RScorrected, decodedData, i);
            }
        }
    }

    return 0;
}