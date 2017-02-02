//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: run_Init_State.cpp
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 01-Feb-2017 19:25:15
//

// Include Files
#include "rt_nonfinite.h"
#include "run_Init_State.h"
#include "run_Init_State_emxutil.h"
#include "createTemplate.h"
#include "run_Init_State_rtwutil.h"

// Function Declarations
static float rt_powf_snf(float u0, float u1);
static float rt_remf_snf(float u0, float u1);

// Function Definitions

//
// Arguments    : float u0
//                float u1
// Return Type  : float
//
static float rt_powf_snf(float u0, float u1)
{
  float y;
  float f0;
  float f1;
  if (rtIsNaNF(u0) || rtIsNaNF(u1)) {
    y = ((real32_T)rtNaN);
  } else {
    f0 = std::abs(u0);
    f1 = std::abs(u1);
    if (rtIsInfF(u1)) {
      if (f0 == 1.0F) {
        y = ((real32_T)rtNaN);
      } else if (f0 > 1.0F) {
        if (u1 > 0.0F) {
          y = ((real32_T)rtInf);
        } else {
          y = 0.0F;
        }
      } else if (u1 > 0.0F) {
        y = 0.0F;
      } else {
        y = ((real32_T)rtInf);
      }
    } else if (f1 == 0.0F) {
      y = 1.0F;
    } else if (f1 == 1.0F) {
      if (u1 > 0.0F) {
        y = u0;
      } else {
        y = 1.0F / u0;
      }
    } else if (u1 == 2.0F) {
      y = u0 * u0;
    } else if ((u1 == 0.5F) && (u0 >= 0.0F)) {
      y = std::sqrt(u0);
    } else if ((u0 < 0.0F) && (u1 > std::floor(u1))) {
      y = ((real32_T)rtNaN);
    } else {
      y = std::pow(u0, u1);
    }
  }

  return y;
}

//
// Arguments    : float u0
//                float u1
// Return Type  : float
//
static float rt_remf_snf(float u0, float u1)
{
  float y;
  float b_u1;
  float tr;
  if (!((!rtIsNaNF(u0)) && (!rtIsInfF(u0)) && ((!rtIsNaNF(u1)) && (!rtIsInfF(u1)))))
  {
    y = ((real32_T)rtNaN);
  } else {
    if (u1 < 0.0F) {
      b_u1 = std::ceil(u1);
    } else {
      b_u1 = std::floor(u1);
    }

    if ((u1 != 0.0F) && (u1 != b_u1)) {
      tr = u0 / u1;
      if (std::abs(tr - rt_roundf_snf(tr)) <= FLT_EPSILON * std::abs(tr)) {
        y = 0.0F;
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
// coder.cstructname(laneFilter, 'MatlabStruct_laneFilter');
// coder.cstructname(vpFilter, 'MatlabStruct_vpFilter');
// Arguments    : const int RES_VH[2]
//                int NBUFFER
//                const MatlabStruct_laneFilter *laneFilter
//                const MatlabStruct_vpFilter *vpFilter
//                MatlabStruct_likelihoods *likelihoods
//                MatlabStruct_templates *templates
//                MatlabStruct_vanishingPt *vanishingPt
//                MatlabStruct_focusMask *masks
// Return Type  : void
//
void run_Init_State(const int RES_VH[2], int NBUFFER, const
                    MatlabStruct_laneFilter *, const MatlabStruct_vpFilter *,
                    MatlabStruct_likelihoods *likelihoods,
                    MatlabStruct_templates *templates, MatlabStruct_vanishingPt *
                    vanishingPt, MatlabStruct_focusMask *masks)
{
  float b_RES_VH[2];
  int i0;
  emxArray_real_T *ROOT_DIR_TEMPLATE;
  emxArray_real_T *ROOT_PROB_TEMPLATE;
  float absx;
  int loop_ub;
  int i1;
  int unnamed_idx_0;
  int n;
  emxArray_real_T *ROOT_IDEPTH_TEMPLATE;
  float step;
  float angle;
  float x;
  signed char b_n;

  // %%%%%%%%%%%%%%%%%%%%%%%%%%
  //     %% (re)start the filters %%
  // %%%%%%%%%%%%%%%%%%%%%%%%%%
  //     IDX_LANE_PIX   = [];
  //
  //  Create Segment Mask %%
  //  %
  //  %     SEGMENT            = zeros( RES_VH(1),RES_VH(2));
  //  %     SEGMENT_TIGHT      = zeros( RES_VH(1),RES_VH(2) );
  //  %     SEGMENT_VERY_TIGHT = zeros( RES_VH(1),RES_VH(2) );
  //  %     SEGMENT_WIDE       = zeros( RES_VH(1),RES_VH(2) );
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //     %% create the gradient template %%
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  for (i0 = 0; i0 < 2; i0++) {
    b_RES_VH[i0] = (float)RES_VH[i0];
  }

  emxInit_real_T(&ROOT_DIR_TEMPLATE, 2);
  emxInit_real_T(&ROOT_PROB_TEMPLATE, 2);
  createTemplate(b_RES_VH[0], b_RES_VH[1], ROOT_DIR_TEMPLATE);

  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //     %% create the prob template               %%
  //     %% masks out parts above horizon + margin %%
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //  Note: Should be fitted using sweeps
  absx = 2.0F * b_RES_VH[0];
  i0 = ROOT_PROB_TEMPLATE->size[0] * ROOT_PROB_TEMPLATE->size[1];
  ROOT_PROB_TEMPLATE->size[0] = (int)(absx + 1.0F);
  ROOT_PROB_TEMPLATE->size[1] = (int)b_RES_VH[1];
  emxEnsureCapacity((emxArray__common *)ROOT_PROB_TEMPLATE, i0, (int)sizeof
                    (double));
  loop_ub = (int)(absx + 1.0F) * (int)b_RES_VH[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    ROOT_PROB_TEMPLATE->data[i0] = 0.0;
  }

  if ((double)(b_RES_VH[0] + 100.0F) > (int)(absx + 1.0F)) {
    i0 = 1;
    i1 = 1;
  } else {
    i0 = (int)(b_RES_VH[0] + 100.0F);
    i1 = (int)(absx + 1.0F) + 1;
  }

  unnamed_idx_0 = i1 - i0;
  loop_ub = (int)b_RES_VH[1];
  for (i1 = 0; i1 < loop_ub; i1++) {
    for (n = 0; n < unnamed_idx_0; n++) {
      ROOT_PROB_TEMPLATE->data[((i0 + n) + ROOT_PROB_TEMPLATE->size[0] * i1) - 1]
        = 1.0;
    }
  }

  for (n = 0; n < (int)((b_RES_VH[0] + 100.0F) + -480.0F); n++) {
    absx = rt_powf_snf(((481.0F + (float)n) - b_RES_VH[0]) / 100.0F, 3.0F);
    loop_ub = ROOT_PROB_TEMPLATE->size[1];
    for (i0 = 0; i0 < loop_ub; i0++) {
      ROOT_PROB_TEMPLATE->data[((int)(481.0F + (float)n) +
        ROOT_PROB_TEMPLATE->size[0] * i0) - 1] = absx;
    }
  }

  emxInit_real_T(&ROOT_IDEPTH_TEMPLATE, 2);

  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //     %% create the depth template              %%
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  absx = 2.0F * b_RES_VH[0];
  i0 = ROOT_IDEPTH_TEMPLATE->size[0] * ROOT_IDEPTH_TEMPLATE->size[1];
  ROOT_IDEPTH_TEMPLATE->size[0] = (int)(absx + 1.0F);
  ROOT_IDEPTH_TEMPLATE->size[1] = (int)b_RES_VH[1];
  emxEnsureCapacity((emxArray__common *)ROOT_IDEPTH_TEMPLATE, i0, (int)sizeof
                    (double));
  loop_ub = (int)(absx + 1.0F) * (int)b_RES_VH[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    ROOT_IDEPTH_TEMPLATE->data[i0] = 0.0;
  }

  step = 45.0F / b_RES_VH[0];
  angle = 90.0F - step;
  i0 = (int)((2.0F * b_RES_VH[0] + 1.0F) + (1.0F - (b_RES_VH[0] + 1.0F)));
  for (n = 0; n < i0; n++) {
    if (!((!rtIsInfF(angle)) && (!rtIsNaNF(angle)))) {
      x = ((real32_T)rtNaN);
    } else {
      x = rt_remf_snf(angle, 360.0F);
      absx = std::abs(x);
      if (absx > 180.0F) {
        if (x > 0.0F) {
          x -= 360.0F;
        } else {
          x += 360.0F;
        }

        absx = std::abs(x);
      }

      if (absx <= 45.0F) {
        x *= 0.0174532924F;
        b_n = 0;
      } else if (absx <= 135.0F) {
        if (x > 0.0F) {
          x = 0.0174532924F * (x - 90.0F);
          b_n = 1;
        } else {
          x = 0.0174532924F * (x + 90.0F);
          b_n = -1;
        }
      } else if (x > 0.0F) {
        x = 0.0174532924F * (x - 180.0F);
        b_n = 2;
      } else {
        x = 0.0174532924F * (x + 180.0F);
        b_n = -2;
      }

      x = std::tan(x);
      if ((b_n == 1) || (b_n == -1)) {
        absx = 1.0F / x;
        x = -(1.0F / x);
        if (rtIsInfF(x) && (b_n == 1)) {
          x = -(-absx);
        }
      }
    }

    absx = 1.75F * x;
    loop_ub = ROOT_IDEPTH_TEMPLATE->size[1];
    unnamed_idx_0 = (int)((b_RES_VH[0] + 1.0F) + (float)n);
    for (i1 = 0; i1 < loop_ub; i1++) {
      ROOT_IDEPTH_TEMPLATE->data[(unnamed_idx_0 + ROOT_IDEPTH_TEMPLATE->size[0] *
        i1) - 1] = absx;
    }

    angle -= step;
  }

  unnamed_idx_0 = ROOT_IDEPTH_TEMPLATE->size[0] * ROOT_IDEPTH_TEMPLATE->size[1];
  for (n = 0; n < unnamed_idx_0; n++) {
    if (100.0 < ROOT_IDEPTH_TEMPLATE->data[n]) {
      ROOT_IDEPTH_TEMPLATE->data[n] = 100.0;
    }
  }

  //      figure(234)
  //      imshow(ROOT_IDEPTH_TEMPLATE,[0 100])
  //      colormap jet
  //      drawnow
  //      pause
  // %%%%%%%%%%%%%%%%%%%%%%%
  //     %% this state is done %%
  // %%%%%%%%%%%%%%%%%%%%%%%
  i0 = templates->GRADIENT_DIR_ROOT->size[0] * templates->
    GRADIENT_DIR_ROOT->size[1];
  templates->GRADIENT_DIR_ROOT->size[0] = ROOT_DIR_TEMPLATE->size[0];
  templates->GRADIENT_DIR_ROOT->size[1] = ROOT_DIR_TEMPLATE->size[1];
  emxEnsureCapacity((emxArray__common *)templates->GRADIENT_DIR_ROOT, i0, (int)
                    sizeof(double));
  loop_ub = ROOT_DIR_TEMPLATE->size[0] * ROOT_DIR_TEMPLATE->size[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    templates->GRADIENT_DIR_ROOT->data[i0] = ROOT_DIR_TEMPLATE->data[i0];
  }

  emxFree_real_T(&ROOT_DIR_TEMPLATE);
  i0 = templates->PROB_ROOT->size[0] * templates->PROB_ROOT->size[1];
  templates->PROB_ROOT->size[0] = ROOT_PROB_TEMPLATE->size[0];
  templates->PROB_ROOT->size[1] = ROOT_PROB_TEMPLATE->size[1];
  emxEnsureCapacity((emxArray__common *)templates->PROB_ROOT, i0, (int)sizeof
                    (double));
  loop_ub = ROOT_PROB_TEMPLATE->size[0] * ROOT_PROB_TEMPLATE->size[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    templates->PROB_ROOT->data[i0] = ROOT_PROB_TEMPLATE->data[i0];
  }

  emxFree_real_T(&ROOT_PROB_TEMPLATE);
  i0 = templates->DEPTH_ROOT->size[0] * templates->DEPTH_ROOT->size[1];
  templates->DEPTH_ROOT->size[0] = ROOT_IDEPTH_TEMPLATE->size[0];
  templates->DEPTH_ROOT->size[1] = ROOT_IDEPTH_TEMPLATE->size[1];
  emxEnsureCapacity((emxArray__common *)templates->DEPTH_ROOT, i0, (int)sizeof
                    (double));
  loop_ub = ROOT_IDEPTH_TEMPLATE->size[0] * ROOT_IDEPTH_TEMPLATE->size[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    templates->DEPTH_ROOT->data[i0] = ROOT_IDEPTH_TEMPLATE->data[i0];
  }

  emxFree_real_T(&ROOT_IDEPTH_TEMPLATE);
  vanishingPt->V = 0.0;
  vanishingPt->H = 0.0;
  vanishingPt->V_prev = 0.0;
  vanishingPt->H_prev = 0.0;
  i0 = likelihoods->TOT_ALL->size[0] * likelihoods->TOT_ALL->size[1] *
    likelihoods->TOT_ALL->size[2];
  likelihoods->TOT_ALL->size[0] = RES_VH[0];
  likelihoods->TOT_ALL->size[1] = RES_VH[1];
  likelihoods->TOT_ALL->size[2] = NBUFFER;
  emxEnsureCapacity((emxArray__common *)likelihoods->TOT_ALL, i0, (int)sizeof
                    (double));
  loop_ub = RES_VH[0] * RES_VH[1] * NBUFFER;
  for (i0 = 0; i0 < loop_ub; i0++) {
    likelihoods->TOT_ALL->data[i0] = 0.0;
  }

  i0 = likelihoods->TOT_MAX->size[0] * likelihoods->TOT_MAX->size[1];
  likelihoods->TOT_MAX->size[0] = RES_VH[0];
  likelihoods->TOT_MAX->size[1] = RES_VH[1];
  emxEnsureCapacity((emxArray__common *)likelihoods->TOT_MAX, i0, (int)sizeof
                    (double));
  loop_ub = RES_VH[0] * RES_VH[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    likelihoods->TOT_MAX->data[i0] = 0.0;
  }

  i0 = likelihoods->GRADIENT_DIR_ALL->size[0] * likelihoods->
    GRADIENT_DIR_ALL->size[1] * likelihoods->GRADIENT_DIR_ALL->size[2];
  likelihoods->GRADIENT_DIR_ALL->size[0] = RES_VH[0];
  likelihoods->GRADIENT_DIR_ALL->size[1] = RES_VH[1];
  likelihoods->GRADIENT_DIR_ALL->size[2] = NBUFFER;
  emxEnsureCapacity((emxArray__common *)likelihoods->GRADIENT_DIR_ALL, i0, (int)
                    sizeof(double));
  loop_ub = RES_VH[0] * RES_VH[1] * NBUFFER;
  for (i0 = 0; i0 < loop_ub; i0++) {
    likelihoods->GRADIENT_DIR_ALL->data[i0] = 0.0;
  }

  i0 = likelihoods->GRADIENT_DIR_AVG->size[0] * likelihoods->
    GRADIENT_DIR_AVG->size[1];
  likelihoods->GRADIENT_DIR_AVG->size[0] = RES_VH[0];
  likelihoods->GRADIENT_DIR_AVG->size[1] = RES_VH[1];
  emxEnsureCapacity((emxArray__common *)likelihoods->GRADIENT_DIR_AVG, i0, (int)
                    sizeof(double));
  loop_ub = RES_VH[0] * RES_VH[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    likelihoods->GRADIENT_DIR_AVG->data[i0] = 0.0;
  }

  i0 = likelihoods->TOT_ALL_BACK_UP->size[0] * likelihoods->
    TOT_ALL_BACK_UP->size[1] * likelihoods->TOT_ALL_BACK_UP->size[2];
  likelihoods->TOT_ALL_BACK_UP->size[0] = RES_VH[0];
  likelihoods->TOT_ALL_BACK_UP->size[1] = RES_VH[1];
  likelihoods->TOT_ALL_BACK_UP->size[2] = NBUFFER;
  emxEnsureCapacity((emxArray__common *)likelihoods->TOT_ALL_BACK_UP, i0, (int)
                    sizeof(double));
  loop_ub = RES_VH[0] * RES_VH[1] * NBUFFER;
  for (i0 = 0; i0 < loop_ub; i0++) {
    likelihoods->TOT_ALL_BACK_UP->data[i0] = 0.0;
  }

  i0 = likelihoods->GRADIENT_DIR_ALL_BACK_UP->size[0] *
    likelihoods->GRADIENT_DIR_ALL_BACK_UP->size[1] *
    likelihoods->GRADIENT_DIR_ALL_BACK_UP->size[2];
  likelihoods->GRADIENT_DIR_ALL_BACK_UP->size[0] = RES_VH[0];
  likelihoods->GRADIENT_DIR_ALL_BACK_UP->size[1] = RES_VH[1];
  likelihoods->GRADIENT_DIR_ALL_BACK_UP->size[2] = NBUFFER;
  emxEnsureCapacity((emxArray__common *)likelihoods->GRADIENT_DIR_ALL_BACK_UP,
                    i0, (int)sizeof(double));
  loop_ub = RES_VH[0] * RES_VH[1] * NBUFFER;
  for (i0 = 0; i0 < loop_ub; i0++) {
    likelihoods->GRADIENT_DIR_ALL_BACK_UP->data[i0] = 0.0;
  }

  i0 = likelihoods->TOT_FOCUSED->size[0] * likelihoods->TOT_FOCUSED->size[1];
  likelihoods->TOT_FOCUSED->size[0] = RES_VH[0];
  likelihoods->TOT_FOCUSED->size[1] = RES_VH[1];
  emxEnsureCapacity((emxArray__common *)likelihoods->TOT_FOCUSED, i0, (int)
                    sizeof(double));
  loop_ub = RES_VH[0] * RES_VH[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    likelihoods->TOT_FOCUSED->data[i0] = 0.0;
  }

  i0 = masks->FOCUS->size[0] * masks->FOCUS->size[1];
  masks->FOCUS->size[0] = RES_VH[0];
  masks->FOCUS->size[1] = RES_VH[1];
  emxEnsureCapacity((emxArray__common *)masks->FOCUS, i0, (int)sizeof(double));
  loop_ub = RES_VH[0] * RES_VH[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    masks->FOCUS->data[i0] = 1.0;
  }

  // Redifinition in createEGOLaneMask()
  //  %                        segmentMask =struct;
  //  %                         SEGMENT            = zeros( 480, 640 );
  //  %                         SEGMENT_TIGHT      = zeros( 480, 640 );
  //  %                         SEGMENT_VERY_TIGHT = zeros( 480, 640 );
  //  %                         SEGMENT_WIDE       = zeros( 480, 640 );
  //  %
}

//
// File trailer for run_Init_State.cpp
//
// [EOF]
//
