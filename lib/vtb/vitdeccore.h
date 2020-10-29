/*
 * vitdeccore.h
 *
 * Code generation for function 'vitdeccore'
 *
 */

#ifndef VITDECCORE_H
#define VITDECCORE_H

/* Include files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "vtb27_types.h"

/* Function Declarations */
#ifdef __cplusplus

extern "C" {

#endif

  extern void vitdeccore(const unsigned char code[16384], const double outputs
    [128], const double nextStates[128], unsigned char decoded[8192], double
    storeMetric[64], double storeState[2048], double storeInput[2048]);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (vitdeccore.h) */
