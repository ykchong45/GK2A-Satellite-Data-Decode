/*
 * rs255223.c
 *
 * Code generation for function 'rs255223'
 *
 */

/* Include files */
#include "rs255223.h"
#include "matlabCodegenHandle.h"
#include "SystemCore.h"

/* Function Definitions */
void rs255223(const unsigned char datain[255], unsigned char dataout[223],
              signed char *err)
{
  c_comm_internal_IntegerRSDecode *obj;
  comm_RSDecoder dec;
  obj = &dec.cIntRSDec;
  obj->S0_isInitialized = 0;
  dec.matlabCodegenIsDeleted = true;

  /*  more about RS: https://www.cs.cmu.edu/~guyb/realworld/reedsolomon/reed_solomon_codes.html */
  dec.isInitialized = 0;
  dec.matlabCodegenIsDeleted = false;
  SystemCore_step(&dec, datain, dataout, err);
  c_matlabCodegenHandle_matlabCod(&dec);
}

/* End of code generation (rs255223.c) */
