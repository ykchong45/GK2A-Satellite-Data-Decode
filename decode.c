#include <stdio.h>
#include <stdint.h>
#include <io.h>
#include <fcntl.h>
#include <memory.h> // memset
#include <stdlib.h> // exit
#include "fec.h"

// Derandomization PN Lookup Table
#include "pn.h"

#define FRAMESIZE 1024
#define SYNCWORDSIZE 32
#define MINCORRELATIONBITS 46
#define FRAMEBITS (FRAMESIZE * 8)
#define RSBLOCKS 4 // RS interleaving depth;
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

int viterbiPolynomial[2] = {VITPOLYA, VITPOLYB};

void initUW()
{
    // printf("Converting Sync Words to Soft Data\n");
    for (int i = 0; i < 64; i++)
    {
        UW0b[i] = (UW0 >> (64 - i - 1)) & 1 ? 0xFF : 0x00;
        UW2b[i] = (UW2 >> (64 - i - 1)) & 1 ? 0xFF : 0x00;
    }
}

uint32_t hardCorrelate(uint8_t dataByte, uint8_t wordByte)
{
    // since the input of dataByte from demodulation are 8-byte soft-decision code,
    // actual 0 signal varies from -128 to -1, actual 1 signal varies from 0 to 127
    // with Two's complement representation, 0 -128 to -1 has 1 on its MSB, making them as >127 unsigned number.

    // return 1 if (a > 127 and b == 255) or (a < 127 and b == 0) else 0
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

int readDataForChunk(uint8_t *data, uint32_t size, FILE *sockfd)
{
    return fread(data, size, 1, sockfd);
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

    // interleave parity
    for (int i = 0; i < 32; i++)
    {
        outbuff[PARITY_OFFSET + i * RSBLOCKS + currentRSFrameNum] = indata[i + 223];
    }
}

uint32_t swapEndianess(uint32_t num)
{
    return ((num << 24) & 0xff000000) | ((num << 8) & 0xff0000) | ((num >> 8) & 0xff00) | ((num >> 24) & 0xff);
}

void writeOutputToFile(uint8_t *data, int sizeToWrite, uint8_t vcid, uint32_t counter)
{
    // Output filename is in format "vcid_counter".
    char filename[20];
    sprintf(filename, "./vcdu/%d_%d", vcid, counter);
    printf("Creating file %s ...\n", filename);

    FILE *fout = fopen(filename, "wb");
    fwrite(data, sizeToWrite, 1, fout);
    fclose(fout);
}

int main()
{
    initUW();

    void *viterbi;
    set_viterbi27_polynomial(viterbiPolynomial);
    if ((viterbi = create_viterbi27(FRAMEBITS)) == NULL)
    {
        printf("create_viterbi27 failed\n");
        exit(1);
    }

    uint8_t codedData[CODEDFRAMESIZE];
    uint8_t decodedData[FRAMESIZE];
    uint8_t rsWorkBuffer[255];
    uint8_t rsCorrectedData[FRAMESIZE];

    int dropCount = 0;

    // open() does not support O_BINARY, and would stop upon EOF.
    FILE *newsockfd = fopen("./data/rec_128", "rb");
    if (newsockfd == NULL)
    {
        printf("Error opening local rec data.\n");
        exit(1);
    }

    int cnt = 0;
    while (cnt++ < 5)
    {
        printf("loop %d\n", cnt);
        // printf("local data reading pointer at %d\n", ftell(newsockfd));

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
            // printf("*mc = %d, p = %d\n", mc, p);

            // Shift data with its syncword at the beginning
            if (p != 0)
            {
                // Shift position
                uint8_t *shiftedPosition = codedData + p;
                memcpy(codedData, shiftedPosition, CODEDFRAMESIZE - p); // Copy from p to chunk size to start of codedData

                // readsize += chunkSize; // Add what we processed to readsize.

                chunkSize = p; // Read needed bytes to fill a frame.
                readDataForChunk(codedData + CODEDFRAMESIZE - p, chunkSize, newsockfd);
            }

            // Correct Frame Phase
            fixPacket(codedData, CODEDFRAMESIZE, n);

            // Viterbi Decoding
            init_viterbi27(viterbi, 0);
            update_viterbi27_blk(viterbi, codedData, FRAMEBITS + 6);
            chainback_viterbi27(viterbi, decodedData, FRAMEBITS, 0);

            // // Test Viterbi Output
            // printf("Viterbi output: ");
            // for (int i = 0; i < 10; i++)
            // {
            //     printf("%02hhX ", decodedData[i]);
            // }
            // printf("\n");

            // // Derandomization
            uint8_t skipSyncWord = SYNCWORDSIZE / 8;
            memcpy(decodedData, decodedData + skipSyncWord, FRAMESIZE - skipSyncWord);
            for (int i = 0; i < FRAMESIZE - skipSyncWord; i++)
            {
                decodedData[i] ^= pn[i % 255];
            }

            // Reed-Solomon, 1 RSFrame at a time
            int derrlocs[255];
            int derrors[4] = {0, 0, 0, 0};
            for (int i = 0; i < RSBLOCKS; i++)
            {
                deinterleaveRS(decodedData, rsWorkBuffer, i);
                derrors[i] = decode_rs_ccsds(rsWorkBuffer, derrlocs, 0, 0);
                interleaveRS(rsWorkBuffer, rsCorrectedData, i);
            }

            printf("RS correction: ");
            for (int i = 0; i < RSBLOCKS; i++)
            {
                printf("%d ", derrors[i]);
            }
            printf("\n");

            // drop frame if RS cannot correct all.
            if (derrors[0] == -1 && derrors[1] == -1 && derrors[2] == -1 && derrors[3] == -1)
            {
                dropCount++;
            }
            else
            {
                uint8_t versionNumber = *rsCorrectedData & 0xC0 >> 6;
                uint8_t scid = (*rsCorrectedData & 0x3F << 2) | (*(rsCorrectedData + 1) & 0xC0 >> 6);
                uint8_t vcid = *(rsCorrectedData + 1) & 0x3F;
                printf("VCID: %d, ", vcid);

                // Packet Counter from Packet
                // counter field spans 3-byte, and we make it a 4-byte pointer, its sequence is changed:
                // i.e. {0x01, 0x02, 0x03, 0x04, 0x05, ...} => {0x04030201, ...}
                // so we swapEndians and mask it to get the correct value.
                uint32_t counter = *((uint32_t *)(rsCorrectedData + 2));
                counter = swapEndianess(counter);
                counter &= 0xFFFFFF00;
                counter >>= 8;
                printf("COUNTER: %d\n", counter);

                writeOutputToFile(rsCorrectedData, 892, vcid, counter);
            }
        }
        printf("\n");
    }

    return 0;
}