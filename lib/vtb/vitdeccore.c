/*
 * vitdeccore.c
 *
 * Code generation for function 'vitdeccore'
 *
 */

/* Include files */
#include <math.h>
#include <string.h>
#include "vtb27.h"
#include "vitdeccore.h"
#include "mod.h"

/* Function Declarations */
static unsigned int ACS(const int branchMetric[4], int stateMetric[64], unsigned
  int tbInput[2112], unsigned int tbState[2112], unsigned int tbIdx, const
  unsigned int output[128], const unsigned int nextState[128]);
static void QnonPuncBMC(const int codeWord[2], int branchMetric[4]);
static double rt_roundd_snf(double u);
static void vitdecUnQ(const unsigned char code[16384], const unsigned int
                      output[128], const unsigned int nextState[128], int
                      stateMetric[64], unsigned int tbInput[2112], unsigned int
                      tbState[2112], unsigned int tbIdx, unsigned char decoded
                      [8192], unsigned int *tbWorkStore);

/* Function Definitions */
static unsigned int ACS(const int branchMetric[4], int stateMetric[64], unsigned
  int tbInput[2112], unsigned int tbState[2112], unsigned int tbIdx, const
  unsigned int output[128], const unsigned int nextState[128])
{
  unsigned int minState;
  int renorm;
  int i;
  int stateIdx;
  int tempMetric[64];
  unsigned int qY;
  int q1;
  unsigned int q0;
  unsigned int b_qY;
  renorm = 32767;
  for (i = 0; i < 64; i++) {
    tempMetric[i] = 32767;
  }

  for (stateIdx = 0; stateIdx < 64; stateIdx++) {
    i = stateMetric[stateIdx];
    qY = output[stateIdx] + 1U;
    if (qY < output[stateIdx]) {
      qY = MAX_uint32_T;
    }

    q1 = branchMetric[(int)qY - 1];
    if ((i < 0) && (q1 < MIN_int32_T - i)) {
      i = MIN_int32_T;
    } else if ((i > 0) && (q1 > MAX_int32_T - i)) {
      i = MAX_int32_T;
    } else {
      i += q1;
    }

    qY = nextState[stateIdx] + 1U;
    if (qY < nextState[stateIdx]) {
      qY = MAX_uint32_T;
    }

    if (i < tempMetric[(int)qY - 1]) {
      qY = nextState[stateIdx] + 1U;
      if (qY < nextState[stateIdx]) {
        qY = MAX_uint32_T;
      }

      tempMetric[(int)qY - 1] = i;
      if (i < renorm) {
        renorm = i;
      }

      qY = nextState[stateIdx] + 1U;
      if (qY < nextState[stateIdx]) {
        qY = MAX_uint32_T;
      }

      b_qY = tbIdx + 1U;
      if (b_qY < tbIdx) {
        b_qY = MAX_uint32_T;
      }

      tbState[((int)qY + (((int)b_qY - 1) << 6)) - 1] = (unsigned int)stateIdx;
      qY = nextState[stateIdx] + 1U;
      if (qY < nextState[stateIdx]) {
        qY = MAX_uint32_T;
      }

      b_qY = tbIdx + 1U;
      if (b_qY < tbIdx) {
        b_qY = MAX_uint32_T;
      }

      tbInput[((int)qY + (((int)b_qY - 1) << 6)) - 1] = 0U;
    }

    i = stateMetric[stateIdx];
    q0 = output[stateIdx + 64];
    qY = q0 + 1U;
    if (qY < q0) {
      qY = MAX_uint32_T;
    }

    q1 = branchMetric[(int)qY - 1];
    if ((i < 0) && (q1 < MIN_int32_T - i)) {
      i = MIN_int32_T;
    } else if ((i > 0) && (q1 > MAX_int32_T - i)) {
      i = MAX_int32_T;
    } else {
      i += q1;
    }

    q0 = nextState[stateIdx + 64];
    qY = q0 + 1U;
    if (qY < q0) {
      qY = MAX_uint32_T;
    }

    if (i < tempMetric[(int)qY - 1]) {
      qY = q0 + 1U;
      if (qY < q0) {
        qY = MAX_uint32_T;
      }

      tempMetric[(int)qY - 1] = i;
      if (i < renorm) {
        renorm = i;
      }

      qY = q0 + 1U;
      if (qY < q0) {
        qY = MAX_uint32_T;
      }

      b_qY = tbIdx + 1U;
      if (b_qY < tbIdx) {
        b_qY = MAX_uint32_T;
      }

      tbState[((int)qY + (((int)b_qY - 1) << 6)) - 1] = (unsigned int)stateIdx;
      qY = q0 + 1U;
      if (qY < q0) {
        qY = MAX_uint32_T;
      }

      b_qY = tbIdx + 1U;
      if (b_qY < tbIdx) {
        b_qY = MAX_uint32_T;
      }

      tbInput[((int)qY + (((int)b_qY - 1) << 6)) - 1] = 1U;
    }
  }

  minState = 0U;
  for (stateIdx = 0; stateIdx < 64; stateIdx++) {
    if ((tempMetric[stateIdx] >= 0) && (renorm < tempMetric[stateIdx] -
         MAX_int32_T)) {
      i = MAX_int32_T;
    } else if ((tempMetric[stateIdx] < 0) && (renorm > tempMetric[stateIdx] -
                MIN_int32_T)) {
      i = MIN_int32_T;
    } else {
      i = tempMetric[stateIdx] - renorm;
    }

    stateMetric[stateIdx] = i;
    if (i == 0) {
      minState = (unsigned int)stateIdx;
    }
  }

  return minState;
}

static void QnonPuncBMC(const int codeWord[2], int branchMetric[4])
{
  int p;
  int qY;
  int b_qY;
  branchMetric[0] = 0;
  branchMetric[1] = 0;
  branchMetric[2] = 0;
  branchMetric[3] = 0;
  for (p = 0; p < 4; p++) {
    if (codeWord[1] < -2147483392) {
      qY = MAX_int32_T;
    } else {
      qY = 255 - codeWord[1];
    }

    if ((branchMetric[p] < 0) && (qY < MIN_int32_T - branchMetric[p])) {
      qY = MIN_int32_T;
    } else if ((branchMetric[p] > 0) && (qY > MAX_int32_T - branchMetric[p])) {
      qY = MAX_int32_T;
    } else {
      qY += branchMetric[p];
    }

    if ((branchMetric[p] < 0) && (codeWord[1] < MIN_int32_T - branchMetric[p]))
    {
      b_qY = MIN_int32_T;
    } else if ((branchMetric[p] > 0) && (codeWord[1] > MAX_int32_T
                - branchMetric[p])) {
      b_qY = MAX_int32_T;
    } else {
      b_qY = branchMetric[p] + codeWord[1];
    }

    if ((p & 1) != 0) {
      branchMetric[p] = qY;
    } else {
      branchMetric[p] = b_qY;
    }

    if (codeWord[0] < -2147483392) {
      qY = MAX_int32_T;
    } else {
      qY = 255 - codeWord[0];
    }

    if ((branchMetric[p] < 0) && (qY < MIN_int32_T - branchMetric[p])) {
      qY = MIN_int32_T;
    } else if ((branchMetric[p] > 0) && (qY > MAX_int32_T - branchMetric[p])) {
      qY = MAX_int32_T;
    } else {
      qY += branchMetric[p];
    }

    if ((branchMetric[p] < 0) && (codeWord[0] < MIN_int32_T - branchMetric[p]))
    {
      b_qY = MIN_int32_T;
    } else if ((branchMetric[p] > 0) && (codeWord[0] > MAX_int32_T
                - branchMetric[p])) {
      b_qY = MAX_int32_T;
    } else {
      b_qY = branchMetric[p] + codeWord[0];
    }

    if (p >> 1 != 0) {
      branchMetric[p] = qY;
    } else {
      branchMetric[p] = b_qY;
    }
  }
}

static double rt_roundd_snf(double u)
{
  double y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

static void vitdecUnQ(const unsigned char code[16384], const unsigned int
                      output[128], const unsigned int nextState[128], int
                      stateMetric[64], unsigned int tbInput[2112], unsigned int
                      tbState[2112], unsigned int tbIdx, unsigned char decoded
                      [8192], unsigned int *tbWorkStore)
{
  unsigned char preDecodVec[16384];
  unsigned int tbWorkLastTB;
  unsigned int minStateLastTB;
  unsigned int input;
  int ib;
  int inputIdxOffset;
  int b_preDecodVec[2];
  unsigned int qY;
  int iv0[4];
  unsigned int minState;
  unsigned int b_qY;
  unsigned int tbWork;
  memcpy(&preDecodVec[0], &code[0], sizeof(unsigned char) << 14);
  memset(&decoded[0], 0, sizeof(unsigned char) << 13);
  *tbWorkStore = 0U;
  tbWorkLastTB = 0U;
  minStateLastTB = 0U;
  input = 0U;
  for (ib = 0; ib < 8192; ib++) {
    inputIdxOffset = ib << 1;
    b_preDecodVec[0] = preDecodVec[inputIdxOffset];
    b_preDecodVec[1] = preDecodVec[1 + inputIdxOffset];
    QnonPuncBMC(b_preDecodVec, iv0);
    minState = ACS(iv0, stateMetric, tbInput, tbState, tbIdx, output, nextState);
    tbWork = tbIdx;
    *tbWorkStore = tbIdx;
    if (ib == 8191) {
      minStateLastTB = minState;
      tbWorkLastTB = tbIdx;
    }

    if (ib >= 32) {
      for (inputIdxOffset = 0; inputIdxOffset < 33; inputIdxOffset++) {
        qY = minState + 1U;
        if (qY < minState) {
          qY = MAX_uint32_T;
        }

        b_qY = tbWork + 1U;
        if (b_qY < tbWork) {
          b_qY = MAX_uint32_T;
        }

        input = tbInput[((int)qY + (((int)b_qY - 1) << 6)) - 1];
        qY = minState + 1U;
        if (qY < minState) {
          qY = MAX_uint32_T;
        }

        b_qY = tbWork + 1U;
        if (b_qY < tbWork) {
          b_qY = MAX_uint32_T;
        }

        minState = tbState[((int)qY + (((int)b_qY - 1) << 6)) - 1];
        if (tbWork > 0U) {
          tbWork--;
        } else {
          tbWork = 32U;
        }
      }

      inputIdxOffset = b_mod(ib - 32);
      if (inputIdxOffset > 2147483646) {
        inputIdxOffset = MAX_int32_T;
      } else {
        inputIdxOffset++;
      }

      decoded[inputIdxOffset - 1] = (unsigned char)(input & 1U);
    }

    if (tbIdx < 32U) {
      tbIdx++;
    } else {
      tbIdx = 0U;
    }
  }

  for (inputIdxOffset = 0; inputIdxOffset < 32; inputIdxOffset++) {
    qY = minStateLastTB + 1U;
    if (qY < minStateLastTB) {
      qY = MAX_uint32_T;
    }

    b_qY = tbWorkLastTB + 1U;
    if (b_qY < tbWorkLastTB) {
      b_qY = MAX_uint32_T;
    }

    decoded[8191 - inputIdxOffset] = (unsigned char)(tbInput[((int)qY + (((int)
      b_qY - 1) << 6)) - 1] & 1U);
    qY = minStateLastTB + 1U;
    if (qY < minStateLastTB) {
      qY = MAX_uint32_T;
    }

    b_qY = tbWorkLastTB + 1U;
    if (b_qY < tbWorkLastTB) {
      b_qY = MAX_uint32_T;
    }

    minStateLastTB = tbState[((int)qY + (((int)b_qY - 1) << 6)) - 1];
    if (tbWorkLastTB > 0U) {
      tbWorkLastTB--;
    } else {
      tbWorkLastTB = 32U;
    }
  }
}

void vitdeccore(const unsigned char code[16384], const double outputs[128],
                const double nextStates[128], unsigned char decoded[8192],
                double storeMetric[64], double storeState[2048], double
                storeInput[2048])
{
  int i0;
  unsigned int tbInput[2112];
  int stateMetric[64];
  static const unsigned short uv0[64] = { 0U, 32767U, 32767U, 32767U, 32767U,
    32767U, 32767U, 32767U, 32767U, 32767U, 32767U, 32767U, 32767U, 32767U,
    32767U, 32767U, 32767U, 32767U, 32767U, 32767U, 32767U, 32767U, 32767U,
    32767U, 32767U, 32767U, 32767U, 32767U, 32767U, 32767U, 32767U, 32767U,
    32767U, 32767U, 32767U, 32767U, 32767U, 32767U, 32767U, 32767U, 32767U,
    32767U, 32767U, 32767U, 32767U, 32767U, 32767U, 32767U, 32767U, 32767U,
    32767U, 32767U, 32767U, 32767U, 32767U, 32767U, 32767U, 32767U, 32767U,
    32767U, 32767U, 32767U, 32767U, 32767U };

  unsigned int tbState[2112];
  unsigned int b_outputs[128];
  unsigned int b_nextStates[128];
  unsigned int tbWorkStore;
  double d0;
  int indx1;
  unsigned int qY;
  int b_tbWorkStore;
  int c_tbWorkStore;
  int storeInput_tmp;
  for (i0 = 0; i0 < 64; i0++) {
    stateMetric[i0] = uv0[i0];
  }

  memset(&tbInput[0], 0, 2112U * sizeof(unsigned int));
  memset(&tbState[0], 0, 2112U * sizeof(unsigned int));
  for (i0 = 0; i0 < 128; i0++) {
    d0 = rt_roundd_snf(outputs[i0]);
    if (d0 < 4.294967296E+9) {
      if (d0 >= 0.0) {
        tbWorkStore = (unsigned int)d0;
      } else {
        tbWorkStore = 0U;
      }
    } else if (d0 >= 4.294967296E+9) {
      tbWorkStore = MAX_uint32_T;
    } else {
      tbWorkStore = 0U;
    }

    b_outputs[i0] = tbWorkStore;
    d0 = rt_roundd_snf(nextStates[i0]);
    if (d0 < 4.294967296E+9) {
      if (d0 >= 0.0) {
        tbWorkStore = (unsigned int)d0;
      } else {
        tbWorkStore = 0U;
      }
    } else if (d0 >= 4.294967296E+9) {
      tbWorkStore = MAX_uint32_T;
    } else {
      tbWorkStore = 0U;
    }

    b_nextStates[i0] = tbWorkStore;
  }

  vitdecUnQ(code, b_outputs, b_nextStates, stateMetric, tbInput, tbState, 0U,
            decoded, &tbWorkStore);
  for (i0 = 0; i0 < 64; i0++) {
    storeMetric[i0] = stateMetric[i0];
  }

  for (indx1 = 0; indx1 < 32; indx1++) {
    qY = tbWorkStore + 1U;
    if (qY < tbWorkStore) {
      qY = MAX_uint32_T;
    }

    b_tbWorkStore = (int)qY;
    qY = tbWorkStore + 1U;
    if (qY < tbWorkStore) {
      qY = MAX_uint32_T;
    }

    c_tbWorkStore = (int)qY;
    for (i0 = 0; i0 < 64; i0++) {
      storeInput_tmp = i0 + ((31 - indx1) << 6);
      storeInput[storeInput_tmp] = tbInput[i0 + ((b_tbWorkStore - 1) << 6)];
      storeState[storeInput_tmp] = tbState[i0 + ((c_tbWorkStore - 1) << 6)];
    }

    if (tbWorkStore > 0U) {
      tbWorkStore--;
    } else {
      tbWorkStore = 32U;
    }
  }
}

/* End of code generation (vitdeccore.c) */
