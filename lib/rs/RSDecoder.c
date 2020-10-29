/*
 * RSDecoder.c
 *
 * Code generation for function 'RSDecoder'
 *
 */

/* Include files */
#include "rs255223.h"
#include "RSDecoder.h"

/* Function Definitions */
void RSDecoder_stepImpl(comm_RSDecoder *obj, const unsigned char x[255],
  unsigned char y[223], signed char *numErr)
{
  c_comm_internal_IntegerRSDecode *b_obj;
  int noErrorStatus;
  int i;
  int inIdx;
  int temp;
  int Temp3;
  int j;
  int Temp2;
  int kCC;
  int b_i;
  int degPsiZ;
  boolean_T loopflag;
  int numRoot;
  b_obj = &obj->cIntRSDec;

  /* System object Outputs function: comm.internal.IntegerRSDecoder */
  /* start of COMM_DoBerlekamp()  */
  /* The function uses the algorithm  described */
  /* in Clark and Cain, Error-Correction Coding for Digital Communications, 1981 */
  /* PART I - ERROR LOCATOR POLYNOMIAL COMPUTATION */
  /* Compute syndrome series : length = 2*t */
  noErrorStatus = 1;
  for (i = 0; i < 255; i++) {
    b_obj->W0_ErasPuncVec[i] = false;
  }

  /*  start of assignInputs(): Populate the codeword and erasure vectors with the proper data  */
  /* Assign message symbols. */
  /* If there are erasures, insert zeros in the erased positions. */
  inIdx = 0;
  for (i = 0; i < 223; i++) {
    b_obj->W11_CCode[i] = x[inIdx];
    b_obj->W0_ErasPuncVec[i] = false;
    inIdx++;
  }

  /* Assign parity symbols, again accounting for erasures */
  /* no puncturing */
  inIdx = 223;
  for (i = 0; i < 32; i++) {
    b_obj->W11_CCode[i + 223] = x[inIdx];
    b_obj->W0_ErasPuncVec[i + 223] = false;
    inIdx++;
  }

  /*  end of assignInputs()  */
  /* Initialize Gamma(Z) = 1 : ASCENDING ORDER.  length = 2t+1 */
  obj->cIntRSDec.W2_GammaZ[0U] = 1;
  for (i = 0; i < 32; i++) {
    b_obj->W2_GammaZ[i + 1] = 0;
  }

  /* Calculate the erasure polynomial GammaZ.GammaZ is the set of coefficients */
  /* of the erasure polynomial in ASCENDING order, because the syndrome is */
  /* calculated in ascending order as well. */
  /*  start of calculateGammaZ() -- Calculate the erasure locator polynomial */
  inIdx = 1;
  obj->cIntRSDec.W3_GammaZTemp[0U] = 1;
  for (i = 254; i >= 0; i--) {
    if (b_obj->W0_ErasPuncVec[i]) {
      /* Raise alpha (2) to the erasure power, and put it in GammaZTemp[1] */
      /*  start of gf_pow():gf_pow raises x^yd  */
      temp = b_obj->P1_table2[1] * (254 - i) % 255;
      if (temp == 0) {
        temp = 255;
      }

      temp = b_obj->P0_table1[temp - 1];

      /*  end of gf_pow()  */
      b_obj->W3_GammaZTemp[1U] = temp;

      /* Convolve GammaZ with GammaZTemp */
      for (j = 0; j < 33; j++) {
        b_obj->W10_TempVec2t1[j] = b_obj->W2_GammaZ[j];
      }

      /*  start of gf_conv():gf_conv convolves A with B and stores the result in retValue  */
      for (b_i = 0; b_i < inIdx + 1; b_i++) {
        b_obj->W2_GammaZ[b_i] = 0;
      }

      for (j = 0; j < inIdx; j++) {
        if ((b_obj->W3_GammaZTemp[0] == 0) || (b_obj->W10_TempVec2t1[j] == 0)) {
          temp = 0;
        } else {
          temp = (b_obj->P1_table2[b_obj->W3_GammaZTemp[0] - 1] +
                  b_obj->P1_table2[b_obj->W10_TempVec2t1[j] - 1]) % 255;
          if (temp == 0) {
            temp = 255;
          }

          temp = b_obj->P0_table1[temp - 1];
        }

        b_obj->W2_GammaZ[j] ^= temp;
      }

      for (j = 0; j < inIdx; j++) {
        if ((b_obj->W3_GammaZTemp[1] == 0) || (b_obj->W10_TempVec2t1[j] == 0)) {
          temp = 0;
        } else {
          temp = (b_obj->P1_table2[b_obj->W3_GammaZTemp[1] - 1] +
                  b_obj->P1_table2[b_obj->W10_TempVec2t1[j] - 1]) % 255;
          if (temp == 0) {
            temp = 255;
          }

          temp = b_obj->P0_table1[temp - 1];
        }

        b_obj->W2_GammaZ[1 + j] ^= temp;
      }

      /*  end of gf_conv()  */
      inIdx++;
    }
  }

  /*  end of calculateGammaZ()  */
  /* Calculate the syndrome by evaluating the codeword at successive */
  /* powers of alpha.  The syndrome is in ASCENDING order. */
  for (i = 0; i < 32; i++) {
    Temp3 = 0;
    for (j = 254; j >= 0; j--) {
      /*  start of gf_pow():gf_pow raises x^yd  */
      temp = b_obj->P1_table2[1] * ((1 + i) * j) % 255;
      if (temp == 0) {
        temp = 255;
      }

      Temp2 = b_obj->P0_table1[temp - 1];

      /*  end of gf_pow()  */
      /* CCode[nfull-1-j] is the current input code symbol. */
      /* Multiply it by alpha, then get the sum so far. */
      /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
      if ((b_obj->W11_CCode[254 - j] == 0) || (Temp2 == 0)) {
        temp = 0;
      } else {
        temp = (b_obj->P1_table2[b_obj->W11_CCode[254 - j] - 1] +
                b_obj->P1_table2[Temp2 - 1]) % 255;
        if (temp == 0) {
          temp = 255;
        }

        temp = b_obj->P0_table1[temp - 1];
      }

      /* end of gf_mul() */
      Temp3 ^= temp;
    }

    b_obj->W1_Syndrome[i] = Temp3;
    if ((noErrorStatus != 0) && (b_obj->W1_Syndrome[i] != 0)) {
      noErrorStatus = 0;
    }
  }

  /* Stop if all syndromes == 0 (i.e. input word is already a valid BCH/RS codeword) */
  if (noErrorStatus == 1) {
    /*  start of assignOutputs():Populate output vectors with proper data */
    inIdx = 223;

    /* Corrected message.  If there is a decoding failure, return the input message. */
    for (i = 0; i < 223; i++) {
      y[i] = (unsigned char)b_obj->W11_CCode[i];
      b_obj->W17_outCorrCode[i] = b_obj->W11_CCode[i];
    }

    /* Optional output for # of errors corrected */
    *numErr = 0;

    /* Parity of corrected codeword. If it is punctured, remove the punctured symbols. */
    /* If there is a decoding failure, return the input parity. */
    for (i = 0; i < 32; i++) {
      b_obj->W17_outCorrCode[inIdx] = b_obj->W11_CCode[i + 223];
      inIdx++;
    }

    /*  end of assignOutputs()  */
  } else {
    /* Calculate the error/erasure locator polynomial PsiZ */
    /*  start of calculatePsiZ(): Calculate the error/erasure locator polynomial PsiZ */
    /* Use the diagram in Fig. 5-8 of Clark and Cain to implement the algorithm. */
    /* Box 1 -- Initializations */
    /* kCC is the location of the oldest symbol in the LFSR at the point */
    /* where the register fails. */
    kCC = -1;

    /* discrep is the convolution of PsiZ and the syndrome */
    /* L is the length of the linear feedback shift register (LFSR) */
    Temp2 = 0;

    /* Connection polynomial = 1.  ASCENDING order.  deg(PsiZ) = 2*t. */
    /* To account for erasures, PsiZ = LambdaZ * GammaZ, where LambdaZ is the */
    /* error locator polynomial, and GammaZ is the erasure locator polynomial. */
    /* NOTE:  PsiZ is the coefficients of the connection polynomial in order of */
    /*        ASCENDING powers rather than the conventional descending order. */
    /*        This is such that after the set of iterations, the inverse of */
    /*        roots of PsiZ in descending order can be obtained directly by */
    /*        finding the roots of PsiZ in ascending order.  PsiZ is */
    /*        initialized as GammaZ in case there are erasures.
     */
    /* Initialize Psi(Z) = Gamma(Z) : ASCENDING ORDER.  length = 2t+1 */
    for (i = 0; i < 33; i++) {
      b_obj->W4_PsiZ[i] = b_obj->W2_GammaZ[i];
      b_obj->W5_PsiZStar[i] = 0;
    }

    /* Initialize correction polynomial D(z) = z*GammaZ : ASCENDING ORDER. */
    /*   length = 2t+2  */
    obj->cIntRSDec.W6_Dz[0U] = 0;
    for (i = 0; i < 33; i++) {
      b_obj->W6_Dz[i + 1] = b_obj->W2_GammaZ[i];
    }

    /* 2*t-numErasPuncs iterations (Diamond 3) */
    for (b_i = 0; b_i < 32; b_i++) {
      /* Box 2 -- Calculate the discrepancy, which is the sum over i of */
      /*          PsiZ(i)*Syndrome(n-i) with i going from 0 to L  */
      Temp3 = 0;
      for (i = 0; i < Temp2 + 1; i++) {
        noErrorStatus = b_i - i;
        if (noErrorStatus >= 0) {
          /* Check that syndrome position is valid */
          /* Multiply the current Psi coefficient by the */
          /* (nCC-L)'th syndrome value.  Then update sum. */
          /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
          if ((b_obj->W4_PsiZ[i] == 0) || (b_obj->W1_Syndrome[noErrorStatus] ==
               0)) {
            temp = 0;
          } else {
            temp = (b_obj->P1_table2[b_obj->W4_PsiZ[i] - 1] + b_obj->
                    P1_table2[b_obj->W1_Syndrome[b_i - i] - 1]) % 255;
            if (temp == 0) {
              temp = 255;
            }

            temp = b_obj->P0_table1[temp - 1];
          }

          /* end of gf_mul() */
          Temp3 ^= temp;
        }
      }

      /* Diamond 1 -- Continue if the discrepancy is not equal to zero */
      if (Temp3 != 0) {
        /* Box 3 -- Connection polynomial */
        /*          PsiZ(n) = PsiZ(n-1) - discrep(n)*Dz */
        for (i = 0; i < 33; i++) {
          b_obj->W10_TempVec2t1[i] = Temp3;
        }

        for (i = 0; i < 32; i++) {
          /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
          if ((b_obj->W10_TempVec2t1[i] == 0) || (b_obj->W6_Dz[i] == 0)) {
            temp = 0;
          } else {
            temp = (b_obj->P1_table2[b_obj->W10_TempVec2t1[i] - 1] +
                    b_obj->P1_table2[b_obj->W6_Dz[i] - 1]) % 255;
            if (temp == 0) {
              temp = 255;
            }

            temp = b_obj->P0_table1[temp - 1];
          }

          /* end of gf_mul() */
          b_obj->W5_PsiZStar[i] = b_obj->W4_PsiZ[i] ^ temp;
        }

        /* Diamond 2  */
        inIdx = b_i - kCC;
        if (Temp2 < inIdx) {
          /* Boxes 4-7 -- Correction polynomial */
          /*              Dz = PsiZ(n-1) / discrep(n) */
          kCC = b_i - Temp2;
          for (i = 0; i < 33; i++) {
            /*  start of gf_div():gf_div divides the scalars x/b */
            if (b_obj->W10_TempVec2t1[i] == 0) {
              temp = b_obj->P0_table1[254U];
            } else {
              temp = b_obj->P0_table1[254 - b_obj->P1_table2
                [b_obj->W10_TempVec2t1[i] - 1]];
            }

            /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
            if ((temp == 0) || (b_obj->W4_PsiZ[i] == 0)) {
              temp = 0;
            } else {
              temp = (b_obj->P1_table2[temp - 1] + b_obj->P1_table2
                      [b_obj->W4_PsiZ[i] - 1]) % 255;
              if (temp == 0) {
                temp = 255;
              }

              temp = b_obj->P0_table1[temp - 1];
            }

            /* end of gf_mul() */
            /*  end of gf_div() */
            b_obj->W6_Dz[i] = temp;
          }

          Temp2 = inIdx;
        }

        /* Box 8 -- Reset the error/erasure locator polynomial */
        for (i = 0; i < 33; i++) {
          b_obj->W4_PsiZ[i] = b_obj->W5_PsiZStar[i];
        }
      }

      /* Box 9 -- Correction polynomial */
      /*           Dz = z * Dz */
      for (i = 31; i >= 0; i--) {
        b_obj->W6_Dz[i + 1] = b_obj->W6_Dz[i];
      }

      b_obj->W6_Dz[0U] = 0;
    }

    /*  end of calculatePsiZ  */
    /* FIND ERROR LOCATIONS */
    /* At this point, error/erasure locator polynomial has been found,  which is PsiZ */
    /* Find degree of Psi(Z) */
    degPsiZ = 0;
    loopflag = true;
    for (i = 32; i >= 0; i--) {
      if ((b_obj->W4_PsiZ[i] > 0) && loopflag) {
        degPsiZ = i;
        loopflag = false;
      }
    }

    /* Degree of Psi(Z) must be equal to L and larger than 0 */
    /* (i.e. cannot be a constant) */
    if ((degPsiZ != Temp2) || (degPsiZ < 1)) {
      /*  start of assignOutputs():Populate output vectors with proper data */
      inIdx = 223;

      /* Corrected message.  If there is a decoding failure, return the input message. */
      for (i = 0; i < 223; i++) {
        y[i] = x[i];
        b_obj->W17_outCorrCode[i] = b_obj->W11_CCode[i];
      }

      /* Optional output for # of errors corrected */
      *numErr = -1;

      /* Parity of corrected codeword. If it is punctured, remove the punctured symbols. */
      /* If there is a decoding failure, return the input parity. */
      for (i = 0; i < 32; i++) {
        b_obj->W17_outCorrCode[inIdx] = x[inIdx];
        inIdx++;
      }

      /*  end of assignOutputs()  */
    } else {
      /* Initialize contents at pointer Errloc */
      for (i = 0; i < 32; i++) {
        b_obj->W7_Errloc[i] = 0;
      }

      /*  start of gf_roots()  */
      /* This function takes in vector X, computes the roots and stores them in roots */
      /* it returns the number of roots */
      Temp3 = 33;
      numRoot = 0;
      for (i = 0; i < 33; i++) {
        b_obj->W14_tmp[i] = b_obj->W4_PsiZ[i];
      }

      i = 0;
      while (i < 256) {
        Temp2 = 0;
        for (inIdx = 0; inIdx < Temp3; inIdx++) {
          b_obj->W13_d[inIdx] = i;
        }

        for (inIdx = 0; inIdx < Temp3; inIdx++) {
          /*  start of gf_pow():gf_pow raises x^yd  */
          temp = b_obj->W13_d[inIdx];
          if (temp == 0) {
            if (inIdx == 0) {
              temp = 1;
            }
          } else {
            temp = b_obj->P1_table2[temp - 1] * inIdx % 255;
            if (temp == 0) {
              temp = 255;
            }

            temp = b_obj->P0_table1[temp - 1];
          }

          /*  end of gf_pow()  */
          b_obj->W13_d[inIdx] = temp;
        }

        for (inIdx = 0; inIdx < Temp3; inIdx++) {
          /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
          if ((b_obj->W13_d[inIdx] == 0) || (b_obj->W14_tmp[inIdx] == 0)) {
            temp = 0;
          } else {
            temp = (b_obj->P1_table2[b_obj->W13_d[inIdx] - 1] + b_obj->
                    P1_table2[b_obj->W14_tmp[inIdx] - 1]) % 255;
            if (temp == 0) {
              temp = 255;
            }

            temp = b_obj->P0_table1[temp - 1];
          }

          /* end of gf_mul() */
          Temp2 ^= temp;
        }

        if (Temp2 == 0) {
          b_obj->W7_Errloc[numRoot] = i;
          numRoot++;

          /*  start of gf_div():gf_div divides the scalars x/b */
          temp = b_obj->P0_table1[254 - b_obj->P1_table2[0]];

          /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
          if (temp == 0) {
            Temp2 = 0;
          } else {
            temp = (b_obj->P1_table2[temp - 1] + b_obj->P1_table2[0]) % 255;
            if (temp == 0) {
              temp = 255;
            }

            Temp2 = b_obj->P0_table1[temp - 1];
          }

          /* end of gf_mul() */
          /*  end of gf_div() */
          /*  start of gf_div():gf_div divides the scalars x/b */
          if (i == 0) {
            temp = b_obj->P0_table1[254U];
          } else {
            temp = b_obj->P0_table1[254 - b_obj->P1_table2[i - 1]];
          }

          /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
          if (temp == 0) {
            noErrorStatus = 0;
          } else {
            temp = (b_obj->P1_table2[temp - 1] + b_obj->P1_table2[0]) % 255;
            if (temp == 0) {
              temp = 255;
            }

            noErrorStatus = b_obj->P0_table1[temp - 1];
          }

          /* end of gf_mul() */
          /*  end of gf_div() */
          /*  start of gf_deconv --- de-convolves A from B  */
          kCC = 0;
          for (b_i = 0; b_i < Temp3; b_i++) {
            b_obj->W15_tmpQuotient[b_i] = 0;
          }

          for (b_i = 0; b_i < Temp3 - 1; b_i++) {
            /*  start of gf_div():gf_div divides the scalars x/b */
            if (Temp2 == 0) {
              temp = b_obj->P0_table1[254U];
            } else {
              temp = b_obj->P0_table1[254 - b_obj->P1_table2[Temp2 - 1]];
            }

            /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
            if ((temp == 0) || (b_obj->W14_tmp[kCC] == 0)) {
              inIdx = 0;
            } else {
              temp = (b_obj->P1_table2[temp - 1] + b_obj->P1_table2
                      [b_obj->W14_tmp[kCC] - 1]) % 255;
              if (temp == 0) {
                temp = 255;
              }

              inIdx = b_obj->P0_table1[temp - 1];
            }

            /* end of gf_mul() */
            /*  end of gf_div() */
            for (j = 1; j < Temp3; j++) {
              b_obj->W15_tmpQuotient[j - 1] = b_obj->W15_tmpQuotient[j];
            }

            b_obj->W15_tmpQuotient[Temp3 - 1] = inIdx;

            /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
            if ((inIdx == 0) || (Temp2 == 0)) {
              temp = 0;
            } else {
              temp = (b_obj->P1_table2[inIdx - 1] + b_obj->P1_table2[Temp2 - 1])
                % 255;
              if (temp == 0) {
                temp = 255;
              }

              temp = b_obj->P0_table1[temp - 1];
            }

            /* end of gf_mul() */
            b_obj->W14_tmp[kCC] ^= temp;

            /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
            if ((inIdx == 0) || (noErrorStatus == 0)) {
              temp = 0;
            } else {
              temp = (b_obj->P1_table2[inIdx - 1] + b_obj->
                      P1_table2[noErrorStatus - 1]) % 255;
              if (temp == 0) {
                temp = 255;
              }

              temp = b_obj->P0_table1[temp - 1];
            }

            /* end of gf_mul() */
            b_obj->W14_tmp[kCC + 1] ^= temp;
            kCC++;
          }

          for (b_i = 0; b_i < Temp3 - 1; b_i++) {
            b_obj->W14_tmp[b_i] = b_obj->W15_tmpQuotient[b_i + 1];
          }

          /*  end of gf_deconv  */
          Temp3--;
          i--;
        }

        i++;
      }

      for (inIdx = 0; inIdx < numRoot; inIdx++) {
        /*  start of gf_div():gf_div divides the scalars x/b */
        if (b_obj->W7_Errloc[inIdx] == 0) {
          temp = b_obj->P0_table1[254U];
        } else {
          temp = b_obj->P0_table1[254 - b_obj->P1_table2[b_obj->W7_Errloc[inIdx]
            - 1]];
        }

        /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
        if (temp == 0) {
          temp = 0;
        } else {
          temp = (b_obj->P1_table2[temp - 1] + b_obj->P1_table2[0]) % 255;
          if (temp == 0) {
            temp = 255;
          }

          temp = b_obj->P0_table1[temp - 1];
        }

        /* end of gf_mul() */
        /*  end of gf_div() */
        b_obj->W7_Errloc[inIdx] = temp;
      }

      /*  end of gf_roots() */
      /* Decoding failure if one of the following conditions is met: */
      /* (1) Psi(Z) has no roots in this field */
      /* (2) Number of roots not equal to degree of PsiZ */
      if (numRoot != degPsiZ) {
        /*  start of assignOutputs():Populate output vectors with proper data */
        inIdx = 223;

        /* Corrected message.  If there is a decoding failure, return the input message. */
        for (i = 0; i < 223; i++) {
          y[i] = x[i];
          b_obj->W17_outCorrCode[i] = b_obj->W11_CCode[i];
        }

        /* Optional output for # of errors corrected */
        *numErr = -1;

        /* Parity of corrected codeword. If it is punctured, remove the punctured symbols. */
        /* If there is a decoding failure, return the input parity. */
        for (i = 0; i < 32; i++) {
          b_obj->W17_outCorrCode[inIdx] = x[inIdx];
          inIdx++;
        }

        /*  end of assignOutputs()  */
      } else {
        /* Test if the error locations are unique
         */
        loopflag = true;
        i = 0;
        while ((i < numRoot - 1) && loopflag) {
          j = i + 1;
          while ((j < numRoot) && loopflag) {
            if (b_obj->W7_Errloc[i] == b_obj->W7_Errloc[j]) {
              loopflag = false;
            }

            j++;
          }

          i++;
        }

        if (!loopflag) {
          /*  start of assignOutputs():Populate output vectors with proper data */
          inIdx = 223;

          /* Corrected message.  If there is a decoding failure, return the input message. */
          for (i = 0; i < 223; i++) {
            y[i] = x[i];
            b_obj->W17_outCorrCode[i] = b_obj->W11_CCode[i];
          }

          /* Optional output for # of errors corrected */
          *numErr = -1;

          /* Parity of corrected codeword. If it is punctured, remove the punctured symbols. */
          /* If there is a decoding failure, return the input parity. */
          for (i = 0; i < 32; i++) {
            b_obj->W17_outCorrCode[inIdx] = x[inIdx];
            inIdx++;
          }

          /*  end of assignOutputs()  */
        } else {
          /* PART II - FIND ERROR MAGNITUDES AT EACH OF THE ERROR/ERASURE LOCATIONS, */
          /* AND CORRECT THEM */
          /*  start of correctErrors():Calculate the error magnitude in the current error position,  */
          /*                           and correct the errors. */
          for (i = 0; i < 66; i++) {
            b_obj->W8_OmegaZ[i] = 0;
          }

          /*  start of gf_conv():gf_conv convolves A with B and stores the result in retValue  */
          for (i = 0; i < 64; i++) {
            b_obj->W8_OmegaZ[i] = 0;
          }

          for (inIdx = 0; inIdx < 33; inIdx++) {
            for (j = 0; j < 32; j++) {
              if ((b_obj->W4_PsiZ[inIdx] == 0) || (b_obj->W1_Syndrome[j] == 0))
              {
                temp = 0;
              } else {
                temp = (b_obj->P1_table2[b_obj->W4_PsiZ[inIdx] - 1] +
                        b_obj->P1_table2[b_obj->W1_Syndrome[j] - 1]) % 255;
                if (temp == 0) {
                  temp = 255;
                }

                temp = b_obj->P0_table1[temp - 1];
              }

              noErrorStatus = inIdx + j;
              b_obj->W8_OmegaZ[noErrorStatus] ^= temp;
            }
          }

          /*  end of gf_conv()  */
          /* Disregard terms of x^(2t) and higher in Omega(Z) */
          /* because we have no knowledge of such terms in S(Z). */
          /* That is, retain terms up to x^(2t-1) */
          for (i = 0; i < 32; i++) {
            b_obj->W9_OmegaZActual[i] = b_obj->W8_OmegaZ[i];
          }

          /* Compute derivative of PsiZ */
          inIdx = 1;
          for (i = 0; i < 16; i++) {
            b_obj->W12_PsiZDeriv[inIdx - 1] = b_obj->W4_PsiZ[inIdx];
            b_obj->W12_PsiZDeriv[inIdx] = 0;
            inIdx += 2;
          }

          /* Find error magnitude at each error location.  Use the expression found on pg. 222 of Wicker. */
          for (j = 0; j < numRoot; j++) {
            /* Dot product for numerator term */
            Temp3 = 0;
            for (i = 0; i < 32; i++) {
              inIdx = -i;

              /*  start of gf_pow():gf_pow raises x^yd  */
              temp = b_obj->W7_Errloc[j];
              if (-i < 0) {
                /*  start of gf_div():gf_div divides the scalars x/b */
                if (b_obj->W7_Errloc[j] == 0) {
                  temp = b_obj->P0_table1[254U];
                } else {
                  temp = b_obj->P0_table1[254 - b_obj->P1_table2
                    [b_obj->W7_Errloc[j] - 1]];
                }

                /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
                if (temp == 0) {
                  temp = 0;
                } else {
                  temp = (b_obj->P1_table2[temp - 1] + b_obj->P1_table2[0]) %
                    255;
                  if (temp == 0) {
                    temp = 255;
                  }

                  temp = b_obj->P0_table1[temp - 1];
                }

                /* end of gf_mul() */
                /*  end of gf_div() */
                inIdx = i;
              }

              if (temp == 0) {
                inIdx = (inIdx == 0);
              } else {
                temp = b_obj->P1_table2[temp - 1] * inIdx % 255;
                if (temp == 0) {
                  temp = 255;
                }

                inIdx = b_obj->P0_table1[temp - 1];
              }

              /*  end of gf_pow()  */
              Temp2 = b_obj->W9_OmegaZActual[i];
              if (Temp2 > 0) {
                /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
                if (inIdx == 0) {
                  temp = 0;
                } else {
                  temp = (b_obj->P1_table2[Temp2 - 1] + b_obj->P1_table2[inIdx -
                          1]) % 255;
                  if (temp == 0) {
                    temp = 255;
                  }

                  temp = b_obj->P0_table1[temp - 1];
                }

                /* end of gf_mul() */
                Temp3 ^= temp;
              }
            }

            /* Dot product for denominator */
            noErrorStatus = 0;
            kCC = b_obj->W7_Errloc[j];
            for (i = 0; i < 32; i++) {
              inIdx = -i;

              /*  start of gf_pow():gf_pow raises x^yd  */
              temp = kCC;
              if (-i < 0) {
                /*  start of gf_div():gf_div divides the scalars x/b */
                if (kCC == 0) {
                  temp = b_obj->P0_table1[254U];
                } else {
                  temp = b_obj->P0_table1[254 - b_obj->P1_table2[kCC - 1]];
                }

                /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
                if (temp == 0) {
                  temp = 0;
                } else {
                  temp = (b_obj->P1_table2[temp - 1] + b_obj->P1_table2[0]) %
                    255;
                  if (temp == 0) {
                    temp = 255;
                  }

                  temp = b_obj->P0_table1[temp - 1];
                }

                /* end of gf_mul() */
                /*  end of gf_div() */
                inIdx = i;
              }

              if (temp == 0) {
                inIdx = (inIdx == 0);
              } else {
                temp = b_obj->P1_table2[temp - 1] * inIdx % 255;
                if (temp == 0) {
                  temp = 255;
                }

                inIdx = b_obj->P0_table1[temp - 1];
              }

              /*  end of gf_pow()  */
              Temp2 = b_obj->W12_PsiZDeriv[i];
              if (Temp2 > 0) {
                /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
                if (inIdx == 0) {
                  temp = 0;
                } else {
                  temp = (b_obj->P1_table2[Temp2 - 1] + b_obj->P1_table2[inIdx -
                          1]) % 255;
                  if (temp == 0) {
                    temp = 255;
                  }

                  temp = b_obj->P0_table1[temp - 1];
                }

                /* end of gf_mul() */
                noErrorStatus ^= temp;
              }
            }

            /* Re-use space in Temp1 */
            /*  start of gf_div():gf_div divides the scalars x/b */
            if (noErrorStatus == 0) {
              temp = b_obj->P0_table1[254U];
            } else {
              temp = b_obj->P0_table1[254 - b_obj->P1_table2[noErrorStatus - 1]];
            }

            /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
            if ((temp == 0) || (Temp3 == 0)) {
              inIdx = 0;
            } else {
              temp = (b_obj->P1_table2[temp - 1] + b_obj->P1_table2[Temp3 - 1]) %
                255;
              if (temp == 0) {
                temp = 255;
              }

              inIdx = b_obj->P0_table1[temp - 1];
            }

            /* end of gf_mul() */
            /*  end of gf_div() */
            Temp2 = b_obj->W7_Errloc[j];

            /* Find exponent representations of Errloc ==> get actual error locations */
            Temp2 = b_obj->P1_table2[Temp2 - 1];

            /* Correct the current error */
            b_obj->W11_CCode[254 - Temp2] ^= inIdx;
          }

          /*  end of correctErrors()  */
          /* Assign outputs.  Reduce cnumerr by the number of punctures and erasures. */
          /*  start of assignOutputs():Populate output vectors with proper data */
          inIdx = 223;

          /* Corrected message.  If there is a decoding failure, return the input message. */
          for (i = 0; i < 223; i++) {
            y[i] = (unsigned char)b_obj->W11_CCode[i];
            b_obj->W17_outCorrCode[i] = b_obj->W11_CCode[i];
          }

          /* Optional output for # of errors corrected */
          *numErr = (signed char)numRoot;

          /* Parity of corrected codeword. If it is punctured, remove the punctured symbols. */
          /* If there is a decoding failure, return the input parity. */
          for (i = 0; i < 32; i++) {
            b_obj->W17_outCorrCode[inIdx] = b_obj->W11_CCode[i + 223];
            inIdx++;
          }

          /*  end of assignOutputs()  */
        }
      }
    }
  }

  /*  end of COMM_DoBerlekamp()  */
}

/* End of code generation (RSDecoder.c) */
