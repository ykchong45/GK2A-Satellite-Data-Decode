/*
 * SystemCore.c
 *
 * Code generation for function 'SystemCore'
 *
 */

/* Include files */
#include "rs255223.h"
#include "SystemCore.h"
#include "RSDecoder.h"

/* Function Definitions */
void SystemCore_step(comm_RSDecoder *obj, const unsigned char varargin_1[255],
                     unsigned char varargout_1[223], signed char *varargout_2)
{
  int i;
  static const unsigned char uv0[255] = { 2U, 4U, 8U, 16U, 32U, 64U, 128U, 29U,
    58U, 116U, 232U, 205U, 135U, 19U, 38U, 76U, 152U, 45U, 90U, 180U, 117U, 234U,
    201U, 143U, 3U, 6U, 12U, 24U, 48U, 96U, 192U, 157U, 39U, 78U, 156U, 37U, 74U,
    148U, 53U, 106U, 212U, 181U, 119U, 238U, 193U, 159U, 35U, 70U, 140U, 5U, 10U,
    20U, 40U, 80U, 160U, 93U, 186U, 105U, 210U, 185U, 111U, 222U, 161U, 95U,
    190U, 97U, 194U, 153U, 47U, 94U, 188U, 101U, 202U, 137U, 15U, 30U, 60U, 120U,
    240U, 253U, 231U, 211U, 187U, 107U, 214U, 177U, 127U, 254U, 225U, 223U, 163U,
    91U, 182U, 113U, 226U, 217U, 175U, 67U, 134U, 17U, 34U, 68U, 136U, 13U, 26U,
    52U, 104U, 208U, 189U, 103U, 206U, 129U, 31U, 62U, 124U, 248U, 237U, 199U,
    147U, 59U, 118U, 236U, 197U, 151U, 51U, 102U, 204U, 133U, 23U, 46U, 92U,
    184U, 109U, 218U, 169U, 79U, 158U, 33U, 66U, 132U, 21U, 42U, 84U, 168U, 77U,
    154U, 41U, 82U, 164U, 85U, 170U, 73U, 146U, 57U, 114U, 228U, 213U, 183U,
    115U, 230U, 209U, 191U, 99U, 198U, 145U, 63U, 126U, 252U, 229U, 215U, 179U,
    123U, 246U, 241U, MAX_uint8_T, 227U, 219U, 171U, 75U, 150U, 49U, 98U, 196U,
    149U, 55U, 110U, 220U, 165U, 87U, 174U, 65U, 130U, 25U, 50U, 100U, 200U,
    141U, 7U, 14U, 28U, 56U, 112U, 224U, 221U, 167U, 83U, 166U, 81U, 162U, 89U,
    178U, 121U, 242U, 249U, 239U, 195U, 155U, 43U, 86U, 172U, 69U, 138U, 9U, 18U,
    36U, 72U, 144U, 61U, 122U, 244U, 245U, 247U, 243U, 251U, 235U, 203U, 139U,
    11U, 22U, 44U, 88U, 176U, 125U, 250U, 233U, 207U, 131U, 27U, 54U, 108U, 216U,
    173U, 71U, 142U, 1U };

  static const unsigned char uv1[255] = { 0U, 1U, 25U, 2U, 50U, 26U, 198U, 3U,
    223U, 51U, 238U, 27U, 104U, 199U, 75U, 4U, 100U, 224U, 14U, 52U, 141U, 239U,
    129U, 28U, 193U, 105U, 248U, 200U, 8U, 76U, 113U, 5U, 138U, 101U, 47U, 225U,
    36U, 15U, 33U, 53U, 147U, 142U, 218U, 240U, 18U, 130U, 69U, 29U, 181U, 194U,
    125U, 106U, 39U, 249U, 185U, 201U, 154U, 9U, 120U, 77U, 228U, 114U, 166U, 6U,
    191U, 139U, 98U, 102U, 221U, 48U, 253U, 226U, 152U, 37U, 179U, 16U, 145U,
    34U, 136U, 54U, 208U, 148U, 206U, 143U, 150U, 219U, 189U, 241U, 210U, 19U,
    92U, 131U, 56U, 70U, 64U, 30U, 66U, 182U, 163U, 195U, 72U, 126U, 110U, 107U,
    58U, 40U, 84U, 250U, 133U, 186U, 61U, 202U, 94U, 155U, 159U, 10U, 21U, 121U,
    43U, 78U, 212U, 229U, 172U, 115U, 243U, 167U, 87U, 7U, 112U, 192U, 247U,
    140U, 128U, 99U, 13U, 103U, 74U, 222U, 237U, 49U, 197U, 254U, 24U, 227U,
    165U, 153U, 119U, 38U, 184U, 180U, 124U, 17U, 68U, 146U, 217U, 35U, 32U,
    137U, 46U, 55U, 63U, 209U, 91U, 149U, 188U, 207U, 205U, 144U, 135U, 151U,
    178U, 220U, 252U, 190U, 97U, 242U, 86U, 211U, 171U, 20U, 42U, 93U, 158U,
    132U, 60U, 57U, 83U, 71U, 109U, 65U, 162U, 31U, 45U, 67U, 216U, 183U, 123U,
    164U, 118U, 196U, 23U, 73U, 236U, 127U, 12U, 111U, 246U, 108U, 161U, 59U,
    82U, 41U, 157U, 85U, 170U, 251U, 96U, 134U, 177U, 187U, 204U, 62U, 90U, 203U,
    89U, 95U, 176U, 156U, 169U, 160U, 81U, 11U, 245U, 22U, 235U, 122U, 117U, 44U,
    215U, 79U, 174U, 213U, 233U, 230U, 231U, 173U, 232U, 116U, 214U, 244U, 234U,
    168U, 80U, 88U, 175U };

  if (obj->isInitialized != 1) {
    obj->isSetupComplete = false;
    obj->isInitialized = 1;

    /* System object Constructor function: comm.internal.IntegerRSDecoder */
    obj->cIntRSDec.S0_isInitialized = 0;
    for (i = 0; i < 255; i++) {
      obj->cIntRSDec.P0_table1[i] = uv0[i];
    }

    for (i = 0; i < 255; i++) {
      obj->cIntRSDec.P1_table2[i] = uv1[i];
    }

    obj->isSetupComplete = true;
  }

  RSDecoder_stepImpl(obj, varargin_1, varargout_1, varargout_2);
}

/* End of code generation (SystemCore.c) */
