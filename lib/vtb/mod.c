/*
 * mod.c
 *
 * Code generation for function 'mod'
 *
 */

/* Include files */
#include "vtb27.h"
#include "mod.h"

/* Function Definitions */
int b_mod(int x)
{
  return x - ((x >> 13) << 13);
}

/* End of code generation (mod.c) */
