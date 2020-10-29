/*
 * rs255223_types.h
 *
 * Code generation for function 'rs255223'
 *
 */

#ifndef RS255223_TYPES_H
#define RS255223_TYPES_H

/* Include files */
#include "rtwtypes.h"

/* Type Definitions */
#ifndef struct_c_comm_internal_IntegerRSDecode
#define struct_c_comm_internal_IntegerRSDecode

struct c_comm_internal_IntegerRSDecode
{
  int S0_isInitialized;
  boolean_T W0_ErasPuncVec[255];
  int W1_Syndrome[32];
  int W2_GammaZ[33];
  int W3_GammaZTemp[2];
  int W4_PsiZ[33];
  int W5_PsiZStar[33];
  int W6_Dz[34];
  int W7_Errloc[32];
  int W8_OmegaZ[66];
  int W9_OmegaZActual[32];
  int W10_TempVec2t1[33];
  int W11_CCode[255];
  int W12_PsiZDeriv[32];
  int W13_d[33];
  int W14_tmp[33];
  int W15_tmpQuotient[33];
  boolean_T W16_erasures[255];
  int W17_outCorrCode[255];
  int W18_input[255];
  int W19_outMsg[223];
  int W20_outCNumErr;
  int P0_table1[255];
  int P1_table2[255];
};

#endif                                 /*struct_c_comm_internal_IntegerRSDecode*/

#ifndef typedef_c_comm_internal_IntegerRSDecode
#define typedef_c_comm_internal_IntegerRSDecode

typedef struct c_comm_internal_IntegerRSDecode c_comm_internal_IntegerRSDecode;

#endif                                 /*typedef_c_comm_internal_IntegerRSDecode*/

#ifndef struct_comm_BitToInteger_1
#define struct_comm_BitToInteger_1

struct comm_BitToInteger_1
{
  boolean_T SystemObjectStructType;
};

#endif                                 /*struct_comm_BitToInteger_1*/

#ifndef typedef_comm_BitToInteger_1
#define typedef_comm_BitToInteger_1

typedef struct comm_BitToInteger_1 comm_BitToInteger_1;

#endif                                 /*typedef_comm_BitToInteger_1*/

#ifndef typedef_comm_RSDecoder
#define typedef_comm_RSDecoder

typedef struct {
  boolean_T matlabCodegenIsDeleted;
  int isInitialized;
  boolean_T isSetupComplete;
  c_comm_internal_IntegerRSDecode cIntRSDec;
  comm_BitToInteger_1 cBIC;
} comm_RSDecoder;

#endif                                 /*typedef_comm_RSDecoder*/
#endif

/* End of code generation (rs255223_types.h) */
