/*
 * RSDecoder.h
 *
 * Code generation for function 'RSDecoder'
 *
 */

#ifndef RSDECODER_H
#define RSDECODER_H

/* Include files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "rs255223_types.h"

/* Function Declarations */
#ifdef __cplusplus

extern "C" {

#endif

  extern void RSDecoder_stepImpl(comm_RSDecoder *obj, const unsigned char x[255],
    unsigned char y[223], signed char *numErr);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (RSDecoder.h) */
