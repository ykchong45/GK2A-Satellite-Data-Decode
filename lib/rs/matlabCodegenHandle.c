/*
 * matlabCodegenHandle.c
 *
 * Code generation for function 'matlabCodegenHandle'
 *
 */

/* Include files */
#include "rs255223.h"
#include "matlabCodegenHandle.h"

/* Function Definitions */
void c_matlabCodegenHandle_matlabCod(comm_RSDecoder *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    if (obj->isInitialized == 1) {
      obj->isInitialized = 2;
    }
  }
}

/* End of code generation (matlabCodegenHandle.c) */
