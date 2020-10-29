/*
 * vtb27.c
 *
 * Code generation for function 'vtb27'
 *
 */

/* Include files */
#include "vtb27.h"
#include "vitdeccore.h"

/* Function Definitions */
void vtb27(const unsigned char encoded[16384], unsigned char decodedout[8192])
{
  static const double dv0[128] = { 0.0, 3.0, 1.0, 2.0, 0.0, 3.0, 1.0, 2.0, 3.0,
    0.0, 2.0, 1.0, 3.0, 0.0, 2.0, 1.0, 3.0, 0.0, 2.0, 1.0, 3.0, 0.0, 2.0, 1.0,
    0.0, 3.0, 1.0, 2.0, 0.0, 3.0, 1.0, 2.0, 2.0, 1.0, 3.0, 0.0, 2.0, 1.0, 3.0,
    0.0, 1.0, 2.0, 0.0, 3.0, 1.0, 2.0, 0.0, 3.0, 1.0, 2.0, 0.0, 3.0, 1.0, 2.0,
    0.0, 3.0, 2.0, 1.0, 3.0, 0.0, 2.0, 1.0, 3.0, 0.0, 3.0, 0.0, 2.0, 1.0, 3.0,
    0.0, 2.0, 1.0, 0.0, 3.0, 1.0, 2.0, 0.0, 3.0, 1.0, 2.0, 0.0, 3.0, 1.0, 2.0,
    0.0, 3.0, 1.0, 2.0, 3.0, 0.0, 2.0, 1.0, 3.0, 0.0, 2.0, 1.0, 1.0, 2.0, 0.0,
    3.0, 1.0, 2.0, 0.0, 3.0, 2.0, 1.0, 3.0, 0.0, 2.0, 1.0, 3.0, 0.0, 2.0, 1.0,
    3.0, 0.0, 2.0, 1.0, 3.0, 0.0, 1.0, 2.0, 0.0, 3.0, 1.0, 2.0, 0.0, 3.0 };

  static const double dv1[128] = { 0.0, 0.0, 1.0, 1.0, 2.0, 2.0, 3.0, 3.0, 4.0,
    4.0, 5.0, 5.0, 6.0, 6.0, 7.0, 7.0, 8.0, 8.0, 9.0, 9.0, 10.0, 10.0, 11.0,
    11.0, 12.0, 12.0, 13.0, 13.0, 14.0, 14.0, 15.0, 15.0, 16.0, 16.0, 17.0, 17.0,
    18.0, 18.0, 19.0, 19.0, 20.0, 20.0, 21.0, 21.0, 22.0, 22.0, 23.0, 23.0, 24.0,
    24.0, 25.0, 25.0, 26.0, 26.0, 27.0, 27.0, 28.0, 28.0, 29.0, 29.0, 30.0, 30.0,
    31.0, 31.0, 32.0, 32.0, 33.0, 33.0, 34.0, 34.0, 35.0, 35.0, 36.0, 36.0, 37.0,
    37.0, 38.0, 38.0, 39.0, 39.0, 40.0, 40.0, 41.0, 41.0, 42.0, 42.0, 43.0, 43.0,
    44.0, 44.0, 45.0, 45.0, 46.0, 46.0, 47.0, 47.0, 48.0, 48.0, 49.0, 49.0, 50.0,
    50.0, 51.0, 51.0, 52.0, 52.0, 53.0, 53.0, 54.0, 54.0, 55.0, 55.0, 56.0, 56.0,
    57.0, 57.0, 58.0, 58.0, 59.0, 59.0, 60.0, 60.0, 61.0, 61.0, 62.0, 62.0, 63.0,
    63.0 };

  double dv2[64];
  double dv3[2048];
  double dv4[2048];
  vitdeccore(encoded, dv0, dv1, decodedout, dv2, dv3, dv4);

  /*  function decodedout = vtb27(encoded) %#codegen */
  /*  trellis = poly2trellis(7, [171, 133]); */
  /*  decodedout = uint8(vitdec(encoded, trellis, 32, 'trunc', 'soft', 8)); */
  /*  end */
}

/* End of code generation (vtb27.c) */
