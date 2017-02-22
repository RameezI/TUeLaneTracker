//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: morphopAlgo.cpp
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 21-Feb-2017 22:59:58
//

// Include Files
#include "rt_nonfinite.h"
#include "createTemplate.h"
#include "morphopAlgo.h"
#include "isrow.h"
#include "createTemplate_emxutil.h"

// Type Definitions
typedef struct {
  boolean_T Neighborhood[9];
  int ImageSize[2];
  double InteriorStart[2];
  int InteriorEnd[2];
  int ImageNeighborLinearOffsets[9];
  double Padding;
  double PadValue;
  boolean_T ProcessBorder;
  double NeighborhoodCenter;
  int NeighborLinearIndices[9];
  int NeighborSubscriptOffsets[18];
} c_images_internal_coder_Neighbo;

// Function Declarations
static int div_s32(int numerator, int denominator);
static double rt_remd_snf(double u0, double u1);
static double rt_roundd_snf(double u);

// Function Definitions

//
// Arguments    : int numerator
//                int denominator
// Return Type  : int
//
static int div_s32(int numerator, int denominator)
{
  int quotient;
  unsigned int absNumerator;
  unsigned int absDenominator;
  boolean_T quotientNeedsNegation;
  if (denominator == 0) {
    if (numerator >= 0) {
      quotient = MAX_int32_T;
    } else {
      quotient = MIN_int32_T;
    }
  } else {
    if (numerator < 0) {
      absNumerator = ~(unsigned int)numerator + 1U;
    } else {
      absNumerator = (unsigned int)numerator;
    }

    if (denominator < 0) {
      absDenominator = ~(unsigned int)denominator + 1U;
    } else {
      absDenominator = (unsigned int)denominator;
    }

    quotientNeedsNegation = ((numerator < 0) != (denominator < 0));
    absNumerator /= absDenominator;
    if (quotientNeedsNegation) {
      quotient = -(int)absNumerator;
    } else {
      quotient = (int)absNumerator;
    }
  }

  return quotient;
}

//
// Arguments    : double u0
//                double u1
// Return Type  : double
//
static double rt_remd_snf(double u0, double u1)
{
  double y;
  double b_u1;
  double tr;
  if (!((!rtIsNaN(u0)) && (!rtIsInf(u0)) && ((!rtIsNaN(u1)) && (!rtIsInf(u1)))))
  {
    y = rtNaN;
  } else {
    if (u1 < 0.0) {
      b_u1 = std::ceil(u1);
    } else {
      b_u1 = std::floor(u1);
    }

    if ((u1 != 0.0) && (u1 != b_u1)) {
      tr = u0 / u1;
      if (std::abs(tr - rt_roundd_snf(tr)) <= DBL_EPSILON * std::abs(tr)) {
        y = 0.0;
      } else {
        y = std::fmod(u0, u1);
      }
    } else {
      y = std::fmod(u0, u1);
    }
  }

  return y;
}

//
// Arguments    : double u
// Return Type  : double
//
static double rt_roundd_snf(double u)
{
  double y;
  if (std::abs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = std::floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = std::ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

//
// Arguments    : const emxArray_real32_T *A
//                emxArray_real32_T *B
// Return Type  : void
//
void morphopAlgo(const emxArray_real32_T *A, emxArray_real32_T *B)
{
  emxArray_real32_T *out_;
  emxArray_real32_T *b_B;
  int i1;
  c_images_internal_coder_Neighbo np;
  int k[2];
  boolean_T nhConn[9];
  int nhInds[9];
  int i;
  int imageNeighborLinearOffsets[9];
  int imnhSubs[18];
  int pixelsPerImPage[2];
  double centerLocationInImage;
  int indx;
  int pind;
  int firstIndRange[2];
  int r;
  signed char subs[2];
  signed char b_subs[2];
  int a[18];
  emxArray_real32_T *in;
  c_images_internal_coder_Neighbo *nhoodObj;
  int ak;
  int ck;
  signed char c_subs[2];
  int centerPixelSub[2];
  int secondInd;
  double b_centerPixelSub[2];
  float padValue;
  float imnh_data[81];
  int imnh_size[2];
  int minval;
  int imnhInds[9];
  int b_minval;
  int b_pind;
  int b_secondInd;
  int firstInd;
  int b_i;
  int b_firstInd;
  int u0;
  int pixelSub[2];
  int varargin_1_size_idx_0;
  float varargin_1_data[82];
  float mtmp;
  boolean_T exitg13;
  int b_pixelSub[2];
  boolean_T p;
  int imnhInds_data[9];
  boolean_T exitg12;
  int tmp_data[9];
  boolean_T exitg11;
  float b_imnh_data[81];
  int c_pixelSub[2];
  float b_varargin_1_data[82];
  float b_mtmp;
  int trueCount;
  int b_imnhInds_data[9];
  boolean_T exitg10;
  boolean_T exitg9;
  boolean_T exitg8;
  int b_trueCount;
  int d_pixelSub[2];
  int c_imnhInds_data[9];
  boolean_T exitg6;
  boolean_T exitg5;
  boolean_T exitg7;
  int c_trueCount;
  int d_imnhInds_data[9];
  boolean_T exitg4;
  boolean_T exitg3;
  boolean_T exitg2;
  boolean_T exitg1;
  emxInit_real32_T1(&out_, 1);
  emxInit_real32_T1(&b_B, 1);
  if (!((A->size[0] == 0) || (A->size[1] == 0))) {
    for (i1 = 0; i1 < 2; i1++) {
      np.ImageSize[i1] = A->size[i1];
    }

    for (i1 = 0; i1 < 9; i1++) {
      np.Neighborhood[i1] = true;
    }

    np.Padding = 1.0;
    np.ProcessBorder = true;
    np.NeighborhoodCenter = 1.0;
    np.PadValue = 0.0;
    for (i1 = 0; i1 < 2; i1++) {
      k[i1] = np.ImageSize[i1];
    }

    for (i1 = 0; i1 < 9; i1++) {
      nhConn[i1] = np.Neighborhood[i1];
    }

    for (i = 0; i < 9; i++) {
      nhInds[i] = np.ImageNeighborLinearOffsets[i];
    }

    for (i = 0; i < 9; i++) {
      imageNeighborLinearOffsets[i] = np.NeighborLinearIndices[i];
    }

    memcpy(&imnhSubs[0], &np.NeighborSubscriptOffsets[0], 18U * sizeof(int));

    //  Process pixels with full neighborhood
    //  Process pixels with partial neighborhood
    //  Process pixels with full neighborhood
    //  Process pixels with partial neighborhood
    pixelsPerImPage[0] = 1;
    pixelsPerImPage[1] = k[0];
    for (i1 = 0; i1 < 2; i1++) {
      k[i1]--;
    }

    centerLocationInImage = nhConn[0];
    for (indx = 0; indx < 8; indx++) {
      centerLocationInImage += (double)nhConn[indx + 1];
    }

    if (!(centerLocationInImage == 0.0)) {
      indx = 0;
      for (pind = 0; pind < 9; pind++) {
        if (nhConn[pind]) {
          r = (int)rt_remd_snf((1.0 + (double)pind) - 1.0, 3.0) + 1;
          subs[1] = (signed char)((int)(((double)(pind - r) + 1.0) / 3.0) + 1);
          subs[0] = (signed char)r;
          b_subs[0] = (signed char)r;
          b_subs[1] = (signed char)(subs[1] - 1);
          for (i1 = 0; i1 < 2; i1++) {
            imnhSubs[indx + 9 * i1] = subs[i1];
            firstIndRange[i1] = b_subs[i1] * (1 + (i1 << 1));
          }

          imageNeighborLinearOffsets[indx] = firstIndRange[0] + firstIndRange[1];
          c_subs[0] = (signed char)r;
          c_subs[1] = (signed char)(subs[1] - 1);
          for (i1 = 0; i1 < 2; i1++) {
            firstIndRange[i1] = c_subs[i1] * pixelsPerImPage[i1];
          }

          nhInds[indx] = (int)((double)firstIndRange[0] + (double)firstIndRange
                               [1]);
          indx++;
        }
      }

      for (i1 = 0; i1 < 2; i1++) {
        firstIndRange[i1] = (2 - i1) * pixelsPerImPage[i1];
      }

      centerLocationInImage = (double)firstIndRange[0] + (double)firstIndRange[1];
      for (i1 = 0; i1 < 9; i1++) {
        nhInds[i1] -= (int)centerLocationInImage;
      }

      memcpy(&a[0], &imnhSubs[0], 18U * sizeof(int));
      ak = 0;
      for (ck = 0; ck <= 10; ck += 9) {
        for (indx = 0; indx < 9; indx++) {
          imnhSubs[ck + indx] = a[ak + indx] - 2;
        }

        ak += 9;
      }
    }

    for (i = 0; i < 9; i++) {
      np.ImageNeighborLinearOffsets[i] = nhInds[i];
    }

    for (i = 0; i < 9; i++) {
      np.NeighborLinearIndices[i] = imageNeighborLinearOffsets[i];
    }

    memcpy(&np.NeighborSubscriptOffsets[0], &imnhSubs[0], 18U * sizeof(int));
    for (i1 = 0; i1 < 2; i1++) {
      np.InteriorStart[i1] = 2.0;
    }

    for (i1 = 0; i1 < 2; i1++) {
      np.InteriorEnd[i1] = k[i1];
    }

    emxInit_real32_T(&in, 2);
    nhoodObj = &np;
    i1 = in->size[0] * in->size[1];
    in->size[0] = A->size[0];
    in->size[1] = A->size[1];
    emxEnsureCapacity((emxArray__common *)in, i1, (int)sizeof(float));
    indx = A->size[0] * A->size[1];
    for (i1 = 0; i1 < indx; i1++) {
      in->data[i1] = A->data[i1];
    }

    centerPixelSub[0] = (int)np.InteriorStart[1];
    centerPixelSub[1] = np.InteriorEnd[1];
    firstIndRange[0] = (int)np.InteriorStart[0];
    firstIndRange[1] = np.InteriorEnd[0];
    for (i = 0; i < 9; i++) {
      imageNeighborLinearOffsets[i] = np.ImageNeighborLinearOffsets[i];
    }

    indx = np.ImageSize[0];
    ak = centerPixelSub[1];
    ck = centerPixelSub[0];

#pragma omp parallel \
 num_threads(omp_get_max_threads()) \
 private(b_B,out_,imnh_data,imnh_size,imnhInds,b_pind,b_i,b_firstInd,varargin_1_size_idx_0,mtmp,exitg13) \
 firstprivate(varargin_1_data)

    {
      emxInit_real32_T1(&b_B, 1);
      emxInit_real32_T1(&out_, 1);

#pragma omp for nowait

      for (secondInd = ck; secondInd <= ak; secondInd++) {
        b_pind = B->size[0];
        b_i = b_B->size[0];
        b_B->size[0] = b_pind;
        emxEnsureCapacity((emxArray__common *)b_B, b_i, (int)sizeof(float));
        for (b_i = 0; b_i < b_pind; b_i++) {
          b_B->data[b_i] = B->data[b_i + B->size[0] * (secondInd - 1)];
        }

        b_i = out_->size[0];
        out_->size[0] = b_B->size[0];
        emxEnsureCapacity((emxArray__common *)out_, b_i, (int)sizeof(float));
        for (b_firstInd = firstIndRange[0]; b_firstInd <= firstIndRange[1];
             b_firstInd++) {
          b_pind = (secondInd - 1) * indx + b_firstInd;
          for (b_i = 0; b_i < 9; b_i++) {
            imnhInds[b_i] = imageNeighborLinearOffsets[b_i] + b_pind;
          }

          if (isrow(in)) {
            imnh_size[0] = 1;
            imnh_size[1] = 9;
            for (b_i = 0; b_i < 9; b_i++) {
              imnh_data[b_i] = in->data[imnhInds[b_i] - 1];
            }
          } else {
            imnh_size[0] = 9;
            imnh_size[1] = 1;
            for (b_i = 0; b_i < 9; b_i++) {
              imnh_data[b_i] = in->data[imnhInds[b_i] - 1];
            }
          }

          varargin_1_size_idx_0 = imnh_size[0] * imnh_size[1] + 1;
          b_pind = imnh_size[0] * imnh_size[1];
          for (b_i = 0; b_i < b_pind; b_i++) {
            varargin_1_data[b_i] = imnh_data[b_i];
          }

          varargin_1_data[imnh_size[0] * imnh_size[1]] = ((real32_T)rtMinusInf);
          b_pind = 1;
          mtmp = varargin_1_data[0];
          if (rtIsNaNF(varargin_1_data[0])) {
            b_i = 2;
            exitg13 = false;
            while ((!exitg13) && (b_i <= varargin_1_size_idx_0)) {
              b_pind = b_i;
              if (!rtIsNaNF(varargin_1_data[b_i - 1])) {
                mtmp = varargin_1_data[b_i - 1];
                exitg13 = true;
              } else {
                b_i++;
              }
            }
          }

          if (b_pind < varargin_1_size_idx_0) {
            while (b_pind + 1 <= varargin_1_size_idx_0) {
              if (varargin_1_data[b_pind] > mtmp) {
                mtmp = varargin_1_data[b_pind];
              }

              b_pind++;
            }
          }

          out_->data[b_firstInd - 1] = mtmp;
        }

        b_pind = out_->size[0];
        for (b_i = 0; b_i < b_pind; b_i++) {
          B->data[b_i + B->size[0] * (secondInd - 1)] = out_->data[b_i];
        }
      }

      emxFree_real32_T(&out_);
      emxFree_real32_T(&b_B);
    }

    if (nhoodObj->ProcessBorder) {
      firstIndRange[1] = nhoodObj->ImageSize[0];
      b_centerPixelSub[1] = nhoodObj->InteriorStart[1] - 1.0;
      padValue = (float)nhoodObj->PadValue;
      indx = nhoodObj->ImageSize[0];
      if (firstIndRange[1] <= indx) {
        minval = firstIndRange[1];
      } else {
        minval = indx;
      }

      indx = nhoodObj->ImageSize[1];
      if (indx > b_centerPixelSub[1]) {
        b_minval = (int)b_centerPixelSub[1];
      } else {
        b_minval = indx;
      }

      for (b_secondInd = 0; b_secondInd < b_minval; b_secondInd++) {
        for (firstInd = 1; firstInd <= minval; firstInd++) {
          indx = nhoodObj->ImageSize[0];
          pind = b_secondInd * indx + firstInd;
          for (i = 0; i < 9; i++) {
            indx = nhoodObj->ImageNeighborLinearOffsets[i];
            indx += pind;
            imageNeighborLinearOffsets[i] = indx;
          }

          for (i1 = 0; i1 < 2; i1++) {
            firstIndRange[i1] = nhoodObj->ImageSize[i1];
          }

          if (firstIndRange[0] == 0) {
            r = 1;
          } else {
            r = pind - firstIndRange[0] * div_s32(pind - 1, firstIndRange[0]);
          }

          ak = pind - r;
          if (firstIndRange[0] == 0) {
            if (ak == 0) {
              i = 0;
            } else if (ak < 0) {
              i = MIN_int32_T;
            } else {
              i = MAX_int32_T;
            }
          } else if (firstIndRange[0] == 1) {
            i = ak;
          } else if (firstIndRange[0] == -1) {
            i = -ak;
          } else {
            if (ak >= 0) {
              indx = ak;
            } else if (ak == MIN_int32_T) {
              indx = MAX_int32_T;
            } else {
              indx = -ak;
            }

            if (firstIndRange[0] >= 0) {
              ck = firstIndRange[0];
            } else if (firstIndRange[0] == MIN_int32_T) {
              ck = MAX_int32_T;
            } else {
              ck = -firstIndRange[0];
            }

            i = div_s32(indx, ck);
            indx -= i * ck;
            if ((indx > 0) && (indx >= (ck >> 1) + (ck & 1))) {
              i++;
            }

            if ((ak < 0) != (firstIndRange[0] < 0)) {
              i = -i;
            }
          }

          pixelSub[1] = i + 1;
          pixelSub[0] = r;
          for (i1 = 0; i1 < 2; i1++) {
            centerPixelSub[i1] = pixelSub[i1];
          }

          for (i1 = 0; i1 < 18; i1++) {
            a[i1] = nhoodObj->NeighborSubscriptOffsets[i1];
          }

          ak = 0;
          i = 0;
          for (ck = 0; ck <= 10; ck += 9) {
            for (indx = 0; indx < 9; indx++) {
              imnhSubs[ck + indx] = a[ak + indx] + centerPixelSub[i];
            }

            ak += 9;
            i++;
          }

          for (i = 0; i < 9; i++) {
            nhConn[i] = true;
          }

          switch ((int)nhoodObj->Padding) {
           case 1:
            u0 = 0;
            for (r = 0; r < 9; r++) {
              ak = 0;
              exitg12 = false;
              while ((!exitg12) && (ak < 2)) {
                if ((imnhSubs[r + 9 * ak] < 1) || (imnhSubs[r + 9 * ak] >
                     nhoodObj->ImageSize[ak])) {
                  nhConn[r] = false;
                  exitg12 = true;
                } else {
                  ak++;
                }
              }

              if (nhConn[r]) {
                u0++;
              }
            }

            indx = 0;
            for (i = 0; i < 9; i++) {
              if (nhConn[i]) {
                imnhInds_data[indx] = imageNeighborLinearOffsets[i];
                indx++;
              }
            }
            break;

           case 2:
            u0 = 9;
            for (r = 0; r < 9; r++) {
              imnhInds_data[r] = imageNeighborLinearOffsets[r];
              ak = 0;
              exitg11 = false;
              while ((!exitg11) && (ak < 2)) {
                if ((imnhSubs[r + 9 * ak] < 1) || (imnhSubs[r + 9 * ak] >
                     nhoodObj->ImageSize[ak])) {
                  nhConn[r] = false;
                  imnhInds_data[r] = 1;
                  exitg11 = true;
                } else {
                  ak++;
                }
              }
            }
            break;

           case 3:
            u0 = 9;
            for (r = 0; r < 9; r++) {
              imnhInds_data[r] = imageNeighborLinearOffsets[r];
              for (ak = 0; ak < 2; ak++) {
                ck = imnhSubs[r + 9 * ak];
                if (imnhSubs[r + 9 * ak] < 1) {
                  nhConn[r] = false;
                  ck = 1;
                }

                if (ck > nhoodObj->ImageSize[ak]) {
                  nhConn[r] = false;
                  ck = nhoodObj->ImageSize[ak];
                }

                centerPixelSub[ak] = ck;
              }

              if (!nhConn[r]) {
                for (i1 = 0; i1 < 2; i1++) {
                  firstIndRange[i1] = nhoodObj->ImageSize[i1];
                }

                k[0] = 1;
                k[1] = firstIndRange[0];
                indx = 1;
                for (i = 0; i < 2; i++) {
                  indx += (centerPixelSub[i] - 1) * k[i];
                }

                imnhInds_data[r] = indx;
              }
            }
            break;

           case 4:
            u0 = 9;
            for (r = 0; r < 9; r++) {
              imnhInds_data[r] = imageNeighborLinearOffsets[r];
              for (ak = 0; ak < 2; ak++) {
                ck = imnhSubs[r + 9 * ak];
                if (imnhSubs[r + 9 * ak] < 1) {
                  nhConn[r] = false;
                  indx = nhoodObj->ImageSize[ak];
                  ck = imnhSubs[r + 9 * ak] + (indx << 1);
                }

                if (ck > nhoodObj->ImageSize[ak]) {
                  nhConn[r] = false;
                  indx = nhoodObj->ImageSize[ak];
                  ck = ((indx << 1) - ck) + 1;
                }

                centerPixelSub[ak] = ck;
              }

              if (!nhConn[r]) {
                for (i1 = 0; i1 < 2; i1++) {
                  firstIndRange[i1] = nhoodObj->ImageSize[i1];
                }

                k[0] = 1;
                k[1] = firstIndRange[0];
                indx = 1;
                for (i = 0; i < 2; i++) {
                  indx += (centerPixelSub[i] - 1) * k[i];
                }

                imnhInds_data[r] = indx;
              }
            }
            break;
          }

          p = (in->size[0] == 1);
          if (p) {
            for (i1 = 0; i1 < u0; i1++) {
              tmp_data[i1] = imnhInds_data[i1];
            }

            ak = 1;
            ck = u0;
            for (i1 = 0; i1 < u0; i1++) {
              b_imnh_data[i1] = in->data[tmp_data[i1] - 1];
            }
          } else {
            ak = u0;
            ck = 1;
            for (i1 = 0; i1 < u0; i1++) {
              b_imnh_data[i1] = in->data[imnhInds_data[i1] - 1];
            }
          }

          if (nhoodObj->Padding == 2.0) {
            for (i = 0; i < 9; i++) {
              if (!nhConn[i]) {
                b_imnh_data[i] = padValue;
              }
            }
          }

          i = ak * ck + 1;
          indx = ak * ck;
          for (i1 = 0; i1 < indx; i1++) {
            b_varargin_1_data[i1] = b_imnh_data[i1];
          }

          b_varargin_1_data[ak * ck] = ((real32_T)rtMinusInf);
          indx = 1;
          b_mtmp = b_varargin_1_data[0];
          if (i > 1) {
            if (rtIsNaNF(b_varargin_1_data[0])) {
              ak = 2;
              exitg10 = false;
              while ((!exitg10) && (ak <= i)) {
                indx = ak;
                if (!rtIsNaNF(b_varargin_1_data[ak - 1])) {
                  b_mtmp = b_varargin_1_data[ak - 1];
                  exitg10 = true;
                } else {
                  ak++;
                }
              }
            }

            if (indx < i) {
              while (indx + 1 <= i) {
                if (b_varargin_1_data[indx] > b_mtmp) {
                  b_mtmp = b_varargin_1_data[indx];
                }

                indx++;
              }
            }
          }

          B->data[pind - 1] = b_mtmp;
        }
      }

      firstIndRange[1] = nhoodObj->ImageSize[0];
      centerPixelSub[0] = nhoodObj->InteriorEnd[1] + 1;
      centerPixelSub[1] = nhoodObj->ImageSize[1];
      padValue = (float)nhoodObj->PadValue;
      indx = nhoodObj->ImageSize[0];
      if (firstIndRange[1] <= indx) {
        minval = firstIndRange[1];
      } else {
        minval = indx;
      }

      indx = nhoodObj->ImageSize[1];
      if (centerPixelSub[1] <= indx) {
        b_minval = centerPixelSub[1];
      } else {
        b_minval = indx;
      }

      u0 = centerPixelSub[0];
      if (!(u0 >= 1)) {
        u0 = 1;
      }

      while (u0 <= b_minval) {
        for (firstInd = 1; firstInd <= minval; firstInd++) {
          indx = nhoodObj->ImageSize[0];
          pind = (u0 - 1) * indx + firstInd;
          for (i = 0; i < 9; i++) {
            indx = nhoodObj->ImageNeighborLinearOffsets[i];
            indx += pind;
            imageNeighborLinearOffsets[i] = indx;
          }

          for (i1 = 0; i1 < 2; i1++) {
            firstIndRange[i1] = nhoodObj->ImageSize[i1];
          }

          if (firstIndRange[0] == 0) {
            r = 1;
          } else {
            r = pind - firstIndRange[0] * div_s32(pind - 1, firstIndRange[0]);
          }

          ak = pind - r;
          if (firstIndRange[0] == 0) {
            if (ak == 0) {
              i = 0;
            } else if (ak < 0) {
              i = MIN_int32_T;
            } else {
              i = MAX_int32_T;
            }
          } else if (firstIndRange[0] == 1) {
            i = ak;
          } else if (firstIndRange[0] == -1) {
            i = -ak;
          } else {
            if (ak >= 0) {
              indx = ak;
            } else if (ak == MIN_int32_T) {
              indx = MAX_int32_T;
            } else {
              indx = -ak;
            }

            if (firstIndRange[0] >= 0) {
              ck = firstIndRange[0];
            } else if (firstIndRange[0] == MIN_int32_T) {
              ck = MAX_int32_T;
            } else {
              ck = -firstIndRange[0];
            }

            i = div_s32(indx, ck);
            indx -= i * ck;
            if ((indx > 0) && (indx >= (ck >> 1) + (ck & 1))) {
              i++;
            }

            if ((ak < 0) != (firstIndRange[0] < 0)) {
              i = -i;
            }
          }

          b_pixelSub[1] = i + 1;
          b_pixelSub[0] = r;
          for (i1 = 0; i1 < 2; i1++) {
            centerPixelSub[i1] = b_pixelSub[i1];
          }

          for (i1 = 0; i1 < 18; i1++) {
            a[i1] = nhoodObj->NeighborSubscriptOffsets[i1];
          }

          ak = 0;
          i = 0;
          for (ck = 0; ck <= 10; ck += 9) {
            for (indx = 0; indx < 9; indx++) {
              imnhSubs[ck + indx] = a[ak + indx] + centerPixelSub[i];
            }

            ak += 9;
            i++;
          }

          for (i = 0; i < 9; i++) {
            nhConn[i] = true;
          }

          switch ((int)nhoodObj->Padding) {
           case 1:
            trueCount = 0;
            for (r = 0; r < 9; r++) {
              ak = 0;
              exitg9 = false;
              while ((!exitg9) && (ak < 2)) {
                if ((imnhSubs[r + 9 * ak] < 1) || (imnhSubs[r + 9 * ak] >
                     nhoodObj->ImageSize[ak])) {
                  nhConn[r] = false;
                  exitg9 = true;
                } else {
                  ak++;
                }
              }

              if (nhConn[r]) {
                trueCount++;
              }
            }

            indx = 0;
            for (i = 0; i < 9; i++) {
              if (nhConn[i]) {
                b_imnhInds_data[indx] = imageNeighborLinearOffsets[i];
                indx++;
              }
            }
            break;

           case 2:
            trueCount = 9;
            for (r = 0; r < 9; r++) {
              b_imnhInds_data[r] = imageNeighborLinearOffsets[r];
              ak = 0;
              exitg8 = false;
              while ((!exitg8) && (ak < 2)) {
                if ((imnhSubs[r + 9 * ak] < 1) || (imnhSubs[r + 9 * ak] >
                     nhoodObj->ImageSize[ak])) {
                  nhConn[r] = false;
                  b_imnhInds_data[r] = 1;
                  exitg8 = true;
                } else {
                  ak++;
                }
              }
            }
            break;

           case 3:
            trueCount = 9;
            for (r = 0; r < 9; r++) {
              b_imnhInds_data[r] = imageNeighborLinearOffsets[r];
              for (ak = 0; ak < 2; ak++) {
                ck = imnhSubs[r + 9 * ak];
                if (imnhSubs[r + 9 * ak] < 1) {
                  nhConn[r] = false;
                  ck = 1;
                }

                if (ck > nhoodObj->ImageSize[ak]) {
                  nhConn[r] = false;
                  ck = nhoodObj->ImageSize[ak];
                }

                centerPixelSub[ak] = ck;
              }

              if (!nhConn[r]) {
                for (i1 = 0; i1 < 2; i1++) {
                  firstIndRange[i1] = nhoodObj->ImageSize[i1];
                }

                k[0] = 1;
                k[1] = firstIndRange[0];
                indx = 1;
                for (i = 0; i < 2; i++) {
                  indx += (centerPixelSub[i] - 1) * k[i];
                }

                b_imnhInds_data[r] = indx;
              }
            }
            break;

           case 4:
            trueCount = 9;
            for (r = 0; r < 9; r++) {
              b_imnhInds_data[r] = imageNeighborLinearOffsets[r];
              for (ak = 0; ak < 2; ak++) {
                ck = imnhSubs[r + 9 * ak];
                if (imnhSubs[r + 9 * ak] < 1) {
                  nhConn[r] = false;
                  indx = nhoodObj->ImageSize[ak];
                  ck = imnhSubs[r + 9 * ak] + (indx << 1);
                }

                if (ck > nhoodObj->ImageSize[ak]) {
                  nhConn[r] = false;
                  indx = nhoodObj->ImageSize[ak];
                  ck = ((indx << 1) - ck) + 1;
                }

                centerPixelSub[ak] = ck;
              }

              if (!nhConn[r]) {
                for (i1 = 0; i1 < 2; i1++) {
                  firstIndRange[i1] = nhoodObj->ImageSize[i1];
                }

                k[0] = 1;
                k[1] = firstIndRange[0];
                indx = 1;
                for (i = 0; i < 2; i++) {
                  indx += (centerPixelSub[i] - 1) * k[i];
                }

                b_imnhInds_data[r] = indx;
              }
            }
            break;
          }

          p = (in->size[0] == 1);
          if (p) {
            for (i1 = 0; i1 < trueCount; i1++) {
              tmp_data[i1] = b_imnhInds_data[i1];
            }

            ak = 1;
            ck = trueCount;
            for (i1 = 0; i1 < trueCount; i1++) {
              b_imnh_data[i1] = in->data[tmp_data[i1] - 1];
            }
          } else {
            ak = trueCount;
            ck = 1;
            for (i1 = 0; i1 < trueCount; i1++) {
              b_imnh_data[i1] = in->data[b_imnhInds_data[i1] - 1];
            }
          }

          if (nhoodObj->Padding == 2.0) {
            for (i = 0; i < 9; i++) {
              if (!nhConn[i]) {
                b_imnh_data[i] = padValue;
              }
            }
          }

          i = ak * ck + 1;
          indx = ak * ck;
          for (i1 = 0; i1 < indx; i1++) {
            b_varargin_1_data[i1] = b_imnh_data[i1];
          }

          b_varargin_1_data[ak * ck] = ((real32_T)rtMinusInf);
          indx = 1;
          b_mtmp = b_varargin_1_data[0];
          if (i > 1) {
            if (rtIsNaNF(b_varargin_1_data[0])) {
              ak = 2;
              exitg7 = false;
              while ((!exitg7) && (ak <= i)) {
                indx = ak;
                if (!rtIsNaNF(b_varargin_1_data[ak - 1])) {
                  b_mtmp = b_varargin_1_data[ak - 1];
                  exitg7 = true;
                } else {
                  ak++;
                }
              }
            }

            if (indx < i) {
              while (indx + 1 <= i) {
                if (b_varargin_1_data[indx] > b_mtmp) {
                  b_mtmp = b_varargin_1_data[indx];
                }

                indx++;
              }
            }
          }

          B->data[pind - 1] = b_mtmp;
        }

        u0++;
      }

      b_centerPixelSub[1] = nhoodObj->InteriorStart[0] - 1.0;
      centerPixelSub[1] = nhoodObj->ImageSize[1];
      padValue = (float)nhoodObj->PadValue;
      indx = nhoodObj->ImageSize[0];
      if (indx > b_centerPixelSub[1]) {
        minval = (int)b_centerPixelSub[1];
      } else {
        minval = indx;
      }

      indx = nhoodObj->ImageSize[1];
      if (centerPixelSub[1] <= indx) {
        b_minval = centerPixelSub[1];
      } else {
        b_minval = indx;
      }

      for (b_secondInd = 1; b_secondInd <= b_minval; b_secondInd++) {
        for (firstInd = 0; firstInd < minval; firstInd++) {
          indx = nhoodObj->ImageSize[0];
          pind = (b_secondInd - 1) * indx + firstInd;
          for (i = 0; i < 9; i++) {
            indx = nhoodObj->ImageNeighborLinearOffsets[i];
            indx = (indx + pind) + 1;
            imageNeighborLinearOffsets[i] = indx;
          }

          for (i1 = 0; i1 < 2; i1++) {
            firstIndRange[i1] = nhoodObj->ImageSize[i1];
          }

          if (firstIndRange[0] == 0) {
            r = 0;
          } else {
            r = pind - firstIndRange[0] * div_s32(pind, firstIndRange[0]);
          }

          ak = pind - r;
          if (firstIndRange[0] == 0) {
            if (ak == 0) {
              i = 0;
            } else if (ak < 0) {
              i = MIN_int32_T;
            } else {
              i = MAX_int32_T;
            }
          } else if (firstIndRange[0] == 1) {
            i = ak;
          } else if (firstIndRange[0] == -1) {
            i = -ak;
          } else {
            if (ak >= 0) {
              indx = ak;
            } else if (ak == MIN_int32_T) {
              indx = MAX_int32_T;
            } else {
              indx = -ak;
            }

            if (firstIndRange[0] >= 0) {
              ck = firstIndRange[0];
            } else if (firstIndRange[0] == MIN_int32_T) {
              ck = MAX_int32_T;
            } else {
              ck = -firstIndRange[0];
            }

            i = div_s32(indx, ck);
            indx -= i * ck;
            if ((indx > 0) && (indx >= (ck >> 1) + (ck & 1))) {
              i++;
            }

            if ((ak < 0) != (firstIndRange[0] < 0)) {
              i = -i;
            }
          }

          c_pixelSub[1] = i + 1;
          c_pixelSub[0] = r + 1;
          for (i1 = 0; i1 < 2; i1++) {
            centerPixelSub[i1] = c_pixelSub[i1];
          }

          for (i1 = 0; i1 < 18; i1++) {
            a[i1] = nhoodObj->NeighborSubscriptOffsets[i1];
          }

          ak = 0;
          i = 0;
          for (ck = 0; ck <= 10; ck += 9) {
            for (indx = 0; indx < 9; indx++) {
              imnhSubs[ck + indx] = a[ak + indx] + centerPixelSub[i];
            }

            ak += 9;
            i++;
          }

          for (i = 0; i < 9; i++) {
            nhConn[i] = true;
          }

          switch ((int)nhoodObj->Padding) {
           case 1:
            b_trueCount = 0;
            for (r = 0; r < 9; r++) {
              ak = 0;
              exitg6 = false;
              while ((!exitg6) && (ak < 2)) {
                if ((imnhSubs[r + 9 * ak] < 1) || (imnhSubs[r + 9 * ak] >
                     nhoodObj->ImageSize[ak])) {
                  nhConn[r] = false;
                  exitg6 = true;
                } else {
                  ak++;
                }
              }

              if (nhConn[r]) {
                b_trueCount++;
              }
            }

            indx = 0;
            for (i = 0; i < 9; i++) {
              if (nhConn[i]) {
                c_imnhInds_data[indx] = imageNeighborLinearOffsets[i];
                indx++;
              }
            }
            break;

           case 2:
            b_trueCount = 9;
            for (r = 0; r < 9; r++) {
              c_imnhInds_data[r] = imageNeighborLinearOffsets[r];
              ak = 0;
              exitg5 = false;
              while ((!exitg5) && (ak < 2)) {
                if ((imnhSubs[r + 9 * ak] < 1) || (imnhSubs[r + 9 * ak] >
                     nhoodObj->ImageSize[ak])) {
                  nhConn[r] = false;
                  c_imnhInds_data[r] = 1;
                  exitg5 = true;
                } else {
                  ak++;
                }
              }
            }
            break;

           case 3:
            b_trueCount = 9;
            for (r = 0; r < 9; r++) {
              c_imnhInds_data[r] = imageNeighborLinearOffsets[r];
              for (ak = 0; ak < 2; ak++) {
                ck = imnhSubs[r + 9 * ak];
                if (imnhSubs[r + 9 * ak] < 1) {
                  nhConn[r] = false;
                  ck = 1;
                }

                if (ck > nhoodObj->ImageSize[ak]) {
                  nhConn[r] = false;
                  ck = nhoodObj->ImageSize[ak];
                }

                centerPixelSub[ak] = ck;
              }

              if (!nhConn[r]) {
                for (i1 = 0; i1 < 2; i1++) {
                  firstIndRange[i1] = nhoodObj->ImageSize[i1];
                }

                k[0] = 1;
                k[1] = firstIndRange[0];
                indx = 1;
                for (i = 0; i < 2; i++) {
                  indx += (centerPixelSub[i] - 1) * k[i];
                }

                c_imnhInds_data[r] = indx;
              }
            }
            break;

           case 4:
            b_trueCount = 9;
            for (r = 0; r < 9; r++) {
              c_imnhInds_data[r] = imageNeighborLinearOffsets[r];
              for (ak = 0; ak < 2; ak++) {
                ck = imnhSubs[r + 9 * ak];
                if (imnhSubs[r + 9 * ak] < 1) {
                  nhConn[r] = false;
                  indx = nhoodObj->ImageSize[ak];
                  ck = imnhSubs[r + 9 * ak] + (indx << 1);
                }

                if (ck > nhoodObj->ImageSize[ak]) {
                  nhConn[r] = false;
                  indx = nhoodObj->ImageSize[ak];
                  ck = ((indx << 1) - ck) + 1;
                }

                centerPixelSub[ak] = ck;
              }

              if (!nhConn[r]) {
                for (i1 = 0; i1 < 2; i1++) {
                  firstIndRange[i1] = nhoodObj->ImageSize[i1];
                }

                k[0] = 1;
                k[1] = firstIndRange[0];
                indx = 1;
                for (i = 0; i < 2; i++) {
                  indx += (centerPixelSub[i] - 1) * k[i];
                }

                c_imnhInds_data[r] = indx;
              }
            }
            break;
          }

          p = (in->size[0] == 1);
          if (p) {
            for (i1 = 0; i1 < b_trueCount; i1++) {
              tmp_data[i1] = c_imnhInds_data[i1];
            }

            ak = 1;
            ck = b_trueCount;
            for (i1 = 0; i1 < b_trueCount; i1++) {
              b_imnh_data[i1] = in->data[tmp_data[i1] - 1];
            }
          } else {
            ak = b_trueCount;
            ck = 1;
            for (i1 = 0; i1 < b_trueCount; i1++) {
              b_imnh_data[i1] = in->data[c_imnhInds_data[i1] - 1];
            }
          }

          if (nhoodObj->Padding == 2.0) {
            for (i = 0; i < 9; i++) {
              if (!nhConn[i]) {
                b_imnh_data[i] = padValue;
              }
            }
          }

          i = ak * ck + 1;
          indx = ak * ck;
          for (i1 = 0; i1 < indx; i1++) {
            b_varargin_1_data[i1] = b_imnh_data[i1];
          }

          b_varargin_1_data[ak * ck] = ((real32_T)rtMinusInf);
          indx = 1;
          b_mtmp = b_varargin_1_data[0];
          if (i > 1) {
            if (rtIsNaNF(b_varargin_1_data[0])) {
              ak = 2;
              exitg4 = false;
              while ((!exitg4) && (ak <= i)) {
                indx = ak;
                if (!rtIsNaNF(b_varargin_1_data[ak - 1])) {
                  b_mtmp = b_varargin_1_data[ak - 1];
                  exitg4 = true;
                } else {
                  ak++;
                }
              }
            }

            if (indx < i) {
              while (indx + 1 <= i) {
                if (b_varargin_1_data[indx] > b_mtmp) {
                  b_mtmp = b_varargin_1_data[indx];
                }

                indx++;
              }
            }
          }

          B->data[pind] = b_mtmp;
        }
      }

      firstIndRange[0] = nhoodObj->InteriorEnd[0] + 1;
      firstIndRange[1] = nhoodObj->ImageSize[0];
      centerPixelSub[1] = nhoodObj->ImageSize[1];
      padValue = (float)nhoodObj->PadValue;
      u0 = firstIndRange[0];
      if (!(u0 >= 1)) {
        u0 = 1;
      }

      indx = nhoodObj->ImageSize[0];
      if (firstIndRange[1] <= indx) {
        minval = firstIndRange[1];
      } else {
        minval = indx;
      }

      indx = nhoodObj->ImageSize[1];
      if (centerPixelSub[1] <= indx) {
        b_minval = centerPixelSub[1];
      } else {
        b_minval = indx;
      }

      for (b_secondInd = 1; b_secondInd <= b_minval; b_secondInd++) {
        for (firstInd = u0; firstInd <= minval; firstInd++) {
          indx = nhoodObj->ImageSize[0];
          pind = (b_secondInd - 1) * indx + firstInd;
          for (i = 0; i < 9; i++) {
            indx = nhoodObj->ImageNeighborLinearOffsets[i];
            indx += pind;
            imageNeighborLinearOffsets[i] = indx;
          }

          for (i1 = 0; i1 < 2; i1++) {
            firstIndRange[i1] = nhoodObj->ImageSize[i1];
          }

          if (firstIndRange[0] == 0) {
            r = 1;
          } else {
            r = pind - firstIndRange[0] * div_s32(pind - 1, firstIndRange[0]);
          }

          ak = pind - r;
          if (firstIndRange[0] == 0) {
            if (ak == 0) {
              i = 0;
            } else if (ak < 0) {
              i = MIN_int32_T;
            } else {
              i = MAX_int32_T;
            }
          } else if (firstIndRange[0] == 1) {
            i = ak;
          } else if (firstIndRange[0] == -1) {
            i = -ak;
          } else {
            if (ak >= 0) {
              indx = ak;
            } else if (ak == MIN_int32_T) {
              indx = MAX_int32_T;
            } else {
              indx = -ak;
            }

            if (firstIndRange[0] >= 0) {
              ck = firstIndRange[0];
            } else if (firstIndRange[0] == MIN_int32_T) {
              ck = MAX_int32_T;
            } else {
              ck = -firstIndRange[0];
            }

            i = div_s32(indx, ck);
            indx -= i * ck;
            if ((indx > 0) && (indx >= (ck >> 1) + (ck & 1))) {
              i++;
            }

            if ((ak < 0) != (firstIndRange[0] < 0)) {
              i = -i;
            }
          }

          d_pixelSub[1] = i + 1;
          d_pixelSub[0] = r;
          for (i1 = 0; i1 < 2; i1++) {
            centerPixelSub[i1] = d_pixelSub[i1];
          }

          for (i1 = 0; i1 < 18; i1++) {
            a[i1] = nhoodObj->NeighborSubscriptOffsets[i1];
          }

          ak = 0;
          i = 0;
          for (ck = 0; ck <= 10; ck += 9) {
            for (indx = 0; indx < 9; indx++) {
              imnhSubs[ck + indx] = a[ak + indx] + centerPixelSub[i];
            }

            ak += 9;
            i++;
          }

          for (i = 0; i < 9; i++) {
            nhConn[i] = true;
          }

          switch ((int)nhoodObj->Padding) {
           case 1:
            c_trueCount = 0;
            for (r = 0; r < 9; r++) {
              ak = 0;
              exitg3 = false;
              while ((!exitg3) && (ak < 2)) {
                if ((imnhSubs[r + 9 * ak] < 1) || (imnhSubs[r + 9 * ak] >
                     nhoodObj->ImageSize[ak])) {
                  nhConn[r] = false;
                  exitg3 = true;
                } else {
                  ak++;
                }
              }

              if (nhConn[r]) {
                c_trueCount++;
              }
            }

            indx = 0;
            for (i = 0; i < 9; i++) {
              if (nhConn[i]) {
                d_imnhInds_data[indx] = imageNeighborLinearOffsets[i];
                indx++;
              }
            }
            break;

           case 2:
            c_trueCount = 9;
            for (r = 0; r < 9; r++) {
              d_imnhInds_data[r] = imageNeighborLinearOffsets[r];
              ak = 0;
              exitg2 = false;
              while ((!exitg2) && (ak < 2)) {
                if ((imnhSubs[r + 9 * ak] < 1) || (imnhSubs[r + 9 * ak] >
                     nhoodObj->ImageSize[ak])) {
                  nhConn[r] = false;
                  d_imnhInds_data[r] = 1;
                  exitg2 = true;
                } else {
                  ak++;
                }
              }
            }
            break;

           case 3:
            c_trueCount = 9;
            for (r = 0; r < 9; r++) {
              d_imnhInds_data[r] = imageNeighborLinearOffsets[r];
              for (ak = 0; ak < 2; ak++) {
                ck = imnhSubs[r + 9 * ak];
                if (imnhSubs[r + 9 * ak] < 1) {
                  nhConn[r] = false;
                  ck = 1;
                }

                if (ck > nhoodObj->ImageSize[ak]) {
                  nhConn[r] = false;
                  ck = nhoodObj->ImageSize[ak];
                }

                centerPixelSub[ak] = ck;
              }

              if (!nhConn[r]) {
                for (i1 = 0; i1 < 2; i1++) {
                  firstIndRange[i1] = nhoodObj->ImageSize[i1];
                }

                k[0] = 1;
                k[1] = firstIndRange[0];
                indx = 1;
                for (i = 0; i < 2; i++) {
                  indx += (centerPixelSub[i] - 1) * k[i];
                }

                d_imnhInds_data[r] = indx;
              }
            }
            break;

           case 4:
            c_trueCount = 9;
            for (r = 0; r < 9; r++) {
              d_imnhInds_data[r] = imageNeighborLinearOffsets[r];
              for (ak = 0; ak < 2; ak++) {
                ck = imnhSubs[r + 9 * ak];
                if (imnhSubs[r + 9 * ak] < 1) {
                  nhConn[r] = false;
                  indx = nhoodObj->ImageSize[ak];
                  ck = imnhSubs[r + 9 * ak] + (indx << 1);
                }

                if (ck > nhoodObj->ImageSize[ak]) {
                  nhConn[r] = false;
                  indx = nhoodObj->ImageSize[ak];
                  ck = ((indx << 1) - ck) + 1;
                }

                centerPixelSub[ak] = ck;
              }

              if (!nhConn[r]) {
                for (i1 = 0; i1 < 2; i1++) {
                  firstIndRange[i1] = nhoodObj->ImageSize[i1];
                }

                k[0] = 1;
                k[1] = firstIndRange[0];
                indx = 1;
                for (i = 0; i < 2; i++) {
                  indx += (centerPixelSub[i] - 1) * k[i];
                }

                d_imnhInds_data[r] = indx;
              }
            }
            break;
          }

          p = (in->size[0] == 1);
          if (p) {
            for (i1 = 0; i1 < c_trueCount; i1++) {
              tmp_data[i1] = d_imnhInds_data[i1];
            }

            ak = 1;
            ck = c_trueCount;
            for (i1 = 0; i1 < c_trueCount; i1++) {
              b_imnh_data[i1] = in->data[tmp_data[i1] - 1];
            }
          } else {
            ak = c_trueCount;
            ck = 1;
            for (i1 = 0; i1 < c_trueCount; i1++) {
              b_imnh_data[i1] = in->data[d_imnhInds_data[i1] - 1];
            }
          }

          if (nhoodObj->Padding == 2.0) {
            for (i = 0; i < 9; i++) {
              if (!nhConn[i]) {
                b_imnh_data[i] = padValue;
              }
            }
          }

          i = ak * ck + 1;
          indx = ak * ck;
          for (i1 = 0; i1 < indx; i1++) {
            b_varargin_1_data[i1] = b_imnh_data[i1];
          }

          b_varargin_1_data[ak * ck] = ((real32_T)rtMinusInf);
          indx = 1;
          b_mtmp = b_varargin_1_data[0];
          if (i > 1) {
            if (rtIsNaNF(b_varargin_1_data[0])) {
              ak = 2;
              exitg1 = false;
              while ((!exitg1) && (ak <= i)) {
                indx = ak;
                if (!rtIsNaNF(b_varargin_1_data[ak - 1])) {
                  b_mtmp = b_varargin_1_data[ak - 1];
                  exitg1 = true;
                } else {
                  ak++;
                }
              }
            }

            if (indx < i) {
              while (indx + 1 <= i) {
                if (b_varargin_1_data[indx] > b_mtmp) {
                  b_mtmp = b_varargin_1_data[indx];
                }

                indx++;
              }
            }
          }

          B->data[pind - 1] = b_mtmp;
        }
      }
    }

    emxFree_real32_T(&in);
  }

  emxFree_real32_T(&b_B);
  emxFree_real32_T(&out_);
}

//
// File trailer for morphopAlgo.cpp
//
// [EOF]
//
