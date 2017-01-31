//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: run_Init_State.cpp
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 28-Jan-2017 01:24:11
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
// Arguments    : const float RES_VH[2]
//                float NBUFFER
//                const MatlabStruct_laneFilter *laneFilter
//                const MatlabStruct_vpFilter *vpFilter
//                double *msg
//                MatlabStruct_likelihoods *likelihoods
//                MatlabStruct_templates *templates
//                MatlabStruct_vanishingPt *vanishingPt
// Return Type  : void
//
void run_Init_State(const float RES_VH[2], float NBUFFER, const
                    MatlabStruct_laneFilter *, const MatlabStruct_vpFilter *,
                    double *msg, MatlabStruct_likelihoods *likelihoods,
                    MatlabStruct_templates *templates, MatlabStruct_vanishingPt *
                    vanishingPt)
{
  emxArray_real_T *ROOT_DIR_TEMPLATE;
  emxArray_real_T *ROOT_PROB_TEMPLATE;
  float absx;
  int i0;
  int loop_ub;
  int i1;
  int unnamed_idx_0;
  int n;
  emxArray_real_T *ROOT_IDEPTH_TEMPLATE;
  float step;
  float angle;
  float x;
  signed char b_n;
  emxInit_real_T(&ROOT_DIR_TEMPLATE, 2);
  emxInit_real_T(&ROOT_PROB_TEMPLATE, 2);

  // 'run_Init_State:3' coder.cstructname(laneFilter, 'MatlabStruct_laneFilter'); 
  // 'run_Init_State:4' coder.cstructname(vpFilter, 'MatlabStruct_vpFilter');
  // %%%%%%%%%%%%%%%%%%%%%%%%%%
  //     %% (re)start the filters %%
  // %%%%%%%%%%%%%%%%%%%%%%%%%%
  // 'run_Init_State:9' laneFilter.mFilter = laneFilter.mPrior;
  // 'run_Init_State:10' vpFilter.mFilter    = vpFilter.mPrior;
  // 'run_Init_State:17' TOT_P_ALL      = zeros( RES_VH(1), RES_VH(2), NBUFFER); 
  // 'run_Init_State:18' DIR_ALL        = zeros( RES_VH(1), RES_VH(2), NBUFFER); 
  // 'run_Init_State:19' TOT_P_ALL_BACK_UP = zeros( RES_VH(1), RES_VH(2), NBUFFER); 
  // 'run_Init_State:20' DIR_ALL_BACK_UP   = zeros( RES_VH(1), RES_VH(2), NBUFFER); 
  // 'run_Init_State:21' MASK_FOC_TOT_P = ones(  RES_VH(1), RES_VH(2));
  // 'run_Init_State:22' FOC_TOT_P      = zeros( RES_VH(1), RES_VH(2));
  // 'run_Init_State:23' TOT_P          = zeros( RES_VH(1), RES_VH(2));
  // 'run_Init_State:24' AVG_DIR_TOT_P  = zeros( RES_VH(1), RES_VH(2));
  // 'run_Init_State:25' IDX_LANE_PIX   = [];
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //     %% create the gradient template %%
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // 'run_Init_State:30' ROOT_DIR_TEMPLATE = createTemplate(RES_VH(1),RES_VH(2)); 
  createTemplate(RES_VH[0], RES_VH[1], ROOT_DIR_TEMPLATE);

  // 'run_Init_State:31' SEGMENT           = zeros(RES_VH(1),RES_VH(2));
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //     %% create the prob template               %%
  //     %% masks out parts above horizon + margin %%
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // 'run_Init_State:37' Margin          = 100;
  //  Note: Should be fitted using sweeps
  // 'run_Init_State:38' ROOT_PROB_TEMPLATE = zeros(2*RES_VH(1)+1,RES_VH(2));
  absx = 2.0F * RES_VH[0];
  i0 = ROOT_PROB_TEMPLATE->size[0] * ROOT_PROB_TEMPLATE->size[1];
  ROOT_PROB_TEMPLATE->size[0] = (int)(absx + 1.0F);
  ROOT_PROB_TEMPLATE->size[1] = (int)RES_VH[1];
  emxEnsureCapacity((emxArray__common *)ROOT_PROB_TEMPLATE, i0, (int)sizeof
                    (double));
  loop_ub = (int)(absx + 1.0F) * (int)RES_VH[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    ROOT_PROB_TEMPLATE->data[i0] = 0.0;
  }

  // 'run_Init_State:39' ROOT_PROB_TEMPLATE(RES_VH(1)+Margin:end,:) = 1;
  if ((double)(RES_VH[0] + 100.0F) > (int)(absx + 1.0F)) {
    i0 = 1;
    i1 = 1;
  } else {
    i0 = (int)(RES_VH[0] + 100.0F);
    i1 = (int)(absx + 1.0F) + 1;
  }

  unnamed_idx_0 = i1 - i0;
  loop_ub = (int)RES_VH[1];
  for (i1 = 0; i1 < loop_ub; i1++) {
    for (n = 0; n < unnamed_idx_0; n++) {
      ROOT_PROB_TEMPLATE->data[((i0 + n) + ROOT_PROB_TEMPLATE->size[0] * i1) - 1]
        = 1.0;
    }
  }

  // 'run_Init_State:40' for n = 481:RES_VH(1)+Margin
  for (n = 0; n < (int)((RES_VH[0] + 100.0F) + -480.0F); n++) {
    // 'run_Init_State:41' ROOT_PROB_TEMPLATE(n,:) = ((n-RES_VH(1))/Margin)^3;
    absx = rt_powf_snf(((481.0F + (float)n) - RES_VH[0]) / 100.0F, 3.0F);
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
  // 'run_Init_State:47' ROOT_IDEPTH_TEMPLATE = zeros(2*RES_VH(1)+1,RES_VH(2));
  absx = 2.0F * RES_VH[0];
  i0 = ROOT_IDEPTH_TEMPLATE->size[0] * ROOT_IDEPTH_TEMPLATE->size[1];
  ROOT_IDEPTH_TEMPLATE->size[0] = (int)(absx + 1.0F);
  ROOT_IDEPTH_TEMPLATE->size[1] = (int)RES_VH[1];
  emxEnsureCapacity((emxArray__common *)ROOT_IDEPTH_TEMPLATE, i0, (int)sizeof
                    (double));
  loop_ub = (int)(absx + 1.0F) * (int)RES_VH[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    ROOT_IDEPTH_TEMPLATE->data[i0] = 0.0;
  }

  // 'run_Init_State:48' step  = 45/RES_VH(1);
  step = 45.0F / RES_VH[0];

  // 'run_Init_State:49' angle = 90-step;
  angle = 90.0F - step;

  // 'run_Init_State:50' for n = RES_VH(1)+1:2*RES_VH(1)+1
  i0 = (int)((2.0F * RES_VH[0] + 1.0F) + (1.0F - (RES_VH[0] + 1.0F)));
  for (n = 0; n < i0; n++) {
    // 'run_Init_State:51' ROOT_IDEPTH_TEMPLATE(n,:) = 1.75*tand(angle);
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
    unnamed_idx_0 = (int)((RES_VH[0] + 1.0F) + (float)n);
    for (i1 = 0; i1 < loop_ub; i1++) {
      ROOT_IDEPTH_TEMPLATE->data[(unnamed_idx_0 + ROOT_IDEPTH_TEMPLATE->size[0] *
        i1) - 1] = absx;
    }

    // 'run_Init_State:52' angle                     = angle-step;
    angle -= step;
  }

  // 'run_Init_State:54' ROOT_IDEPTH_TEMPLATE( 100 < ROOT_IDEPTH_TEMPLATE ) = 100; 
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
  // 'run_Init_State:69' templates = struct;
  // 'run_Init_State:70' templates.ROOT_DIR_TEMPLATE = ROOT_DIR_TEMPLATE;
  i0 = templates->ROOT_DIR_TEMPLATE->size[0] * templates->
    ROOT_DIR_TEMPLATE->size[1];
  templates->ROOT_DIR_TEMPLATE->size[0] = ROOT_DIR_TEMPLATE->size[0];
  templates->ROOT_DIR_TEMPLATE->size[1] = ROOT_DIR_TEMPLATE->size[1];
  emxEnsureCapacity((emxArray__common *)templates->ROOT_DIR_TEMPLATE, i0, (int)
                    sizeof(double));
  loop_ub = ROOT_DIR_TEMPLATE->size[0] * ROOT_DIR_TEMPLATE->size[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    templates->ROOT_DIR_TEMPLATE->data[i0] = ROOT_DIR_TEMPLATE->data[i0];
  }

  emxFree_real_T(&ROOT_DIR_TEMPLATE);

  // 'run_Init_State:71' templates.ROOT_PROB_TEMPLATE = ROOT_PROB_TEMPLATE;
  // 'run_Init_State:72' templates.ROOT_PROB_TEMPLATE = ROOT_PROB_TEMPLATE;
  i0 = templates->ROOT_PROB_TEMPLATE->size[0] * templates->
    ROOT_PROB_TEMPLATE->size[1];
  templates->ROOT_PROB_TEMPLATE->size[0] = ROOT_PROB_TEMPLATE->size[0];
  templates->ROOT_PROB_TEMPLATE->size[1] = ROOT_PROB_TEMPLATE->size[1];
  emxEnsureCapacity((emxArray__common *)templates->ROOT_PROB_TEMPLATE, i0, (int)
                    sizeof(double));
  loop_ub = ROOT_PROB_TEMPLATE->size[0] * ROOT_PROB_TEMPLATE->size[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    templates->ROOT_PROB_TEMPLATE->data[i0] = ROOT_PROB_TEMPLATE->data[i0];
  }

  emxFree_real_T(&ROOT_PROB_TEMPLATE);

  // 'run_Init_State:73' templates.ROOT_DEPTH_TEMPLATE = ROOT_IDEPTH_TEMPLATE;
  i0 = templates->ROOT_DEPTH_TEMPLATE->size[0] * templates->
    ROOT_DEPTH_TEMPLATE->size[1];
  templates->ROOT_DEPTH_TEMPLATE->size[0] = ROOT_IDEPTH_TEMPLATE->size[0];
  templates->ROOT_DEPTH_TEMPLATE->size[1] = ROOT_IDEPTH_TEMPLATE->size[1];
  emxEnsureCapacity((emxArray__common *)templates->ROOT_DEPTH_TEMPLATE, i0, (int)
                    sizeof(double));
  loop_ub = ROOT_IDEPTH_TEMPLATE->size[0] * ROOT_IDEPTH_TEMPLATE->size[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    templates->ROOT_DEPTH_TEMPLATE->data[i0] = ROOT_IDEPTH_TEMPLATE->data[i0];
  }

  emxFree_real_T(&ROOT_IDEPTH_TEMPLATE);

  // 'run_Init_State:74' templates.SEGMENT = SEGMENT;
  i0 = templates->SEGMENT->size[0] * templates->SEGMENT->size[1];
  templates->SEGMENT->size[0] = (int)RES_VH[0];
  templates->SEGMENT->size[1] = (int)RES_VH[1];
  emxEnsureCapacity((emxArray__common *)templates->SEGMENT, i0, (int)sizeof
                    (double));
  loop_ub = (int)RES_VH[0] * (int)RES_VH[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    templates->SEGMENT->data[i0] = 0.0;
  }

  // 'run_Init_State:77' vanishingPt = struct;
  // 'run_Init_State:78' vanishingPt.V = 0;
  vanishingPt->V = 0.0;

  // 'run_Init_State:79' vanishingPt.H = 0;
  vanishingPt->H = 0.0;

  // 'run_Init_State:80' vanishingPt.V_prev = 0;
  vanishingPt->V_prev = 0.0;

  // 'run_Init_State:81' vanishingPt.H_prev = 0;
  vanishingPt->H_prev = 0.0;

  // 'run_Init_State:84' likelihoods =struct;
  // 'run_Init_State:85' likelihoods.TOT_P_ALL         = TOT_P_ALL;
  i0 = likelihoods->TOT_P_ALL->size[0] * likelihoods->TOT_P_ALL->size[1] *
    likelihoods->TOT_P_ALL->size[2];
  likelihoods->TOT_P_ALL->size[0] = (int)RES_VH[0];
  likelihoods->TOT_P_ALL->size[1] = (int)RES_VH[1];
  likelihoods->TOT_P_ALL->size[2] = (int)NBUFFER;
  emxEnsureCapacity((emxArray__common *)likelihoods->TOT_P_ALL, i0, (int)sizeof
                    (double));
  loop_ub = (int)RES_VH[0] * (int)RES_VH[1] * (int)NBUFFER;
  for (i0 = 0; i0 < loop_ub; i0++) {
    likelihoods->TOT_P_ALL->data[i0] = 0.0;
  }

  // 'run_Init_State:86' likelihoods.DIR_ALL           = DIR_ALL;
  i0 = likelihoods->DIR_ALL->size[0] * likelihoods->DIR_ALL->size[1] *
    likelihoods->DIR_ALL->size[2];
  likelihoods->DIR_ALL->size[0] = (int)RES_VH[0];
  likelihoods->DIR_ALL->size[1] = (int)RES_VH[1];
  likelihoods->DIR_ALL->size[2] = (int)NBUFFER;
  emxEnsureCapacity((emxArray__common *)likelihoods->DIR_ALL, i0, (int)sizeof
                    (double));
  loop_ub = (int)RES_VH[0] * (int)RES_VH[1] * (int)NBUFFER;
  for (i0 = 0; i0 < loop_ub; i0++) {
    likelihoods->DIR_ALL->data[i0] = 0.0;
  }

  // 'run_Init_State:87' likelihoods.MASK_FOC_TOT_P    = MASK_FOC_TOT_P;
  i0 = likelihoods->MASK_FOC_TOT_P->size[0] * likelihoods->MASK_FOC_TOT_P->size
    [1];
  likelihoods->MASK_FOC_TOT_P->size[0] = (int)RES_VH[0];
  likelihoods->MASK_FOC_TOT_P->size[1] = (int)RES_VH[1];
  emxEnsureCapacity((emxArray__common *)likelihoods->MASK_FOC_TOT_P, i0, (int)
                    sizeof(double));
  loop_ub = (int)RES_VH[0] * (int)RES_VH[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    likelihoods->MASK_FOC_TOT_P->data[i0] = 1.0;
  }

  // 'run_Init_State:88' likelihoods.TOT_P             = TOT_P;
  i0 = likelihoods->TOT_P->size[0] * likelihoods->TOT_P->size[1];
  likelihoods->TOT_P->size[0] = (int)RES_VH[0];
  likelihoods->TOT_P->size[1] = (int)RES_VH[1];
  emxEnsureCapacity((emxArray__common *)likelihoods->TOT_P, i0, (int)sizeof
                    (double));
  loop_ub = (int)RES_VH[0] * (int)RES_VH[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    likelihoods->TOT_P->data[i0] = 0.0;
  }

  // 'run_Init_State:89' likelihoods.FOC_TOT_P         = FOC_TOT_P;
  i0 = likelihoods->FOC_TOT_P->size[0] * likelihoods->FOC_TOT_P->size[1];
  likelihoods->FOC_TOT_P->size[0] = (int)RES_VH[0];
  likelihoods->FOC_TOT_P->size[1] = (int)RES_VH[1];
  emxEnsureCapacity((emxArray__common *)likelihoods->FOC_TOT_P, i0, (int)sizeof
                    (double));
  loop_ub = (int)RES_VH[0] * (int)RES_VH[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    likelihoods->FOC_TOT_P->data[i0] = 0.0;
  }

  // 'run_Init_State:90' likelihoods.AVG_DIR_TOT_P     = AVG_DIR_TOT_P;
  i0 = likelihoods->AVG_DIR_TOT_P->size[0] * likelihoods->AVG_DIR_TOT_P->size[1];
  likelihoods->AVG_DIR_TOT_P->size[0] = (int)RES_VH[0];
  likelihoods->AVG_DIR_TOT_P->size[1] = (int)RES_VH[1];
  emxEnsureCapacity((emxArray__common *)likelihoods->AVG_DIR_TOT_P, i0, (int)
                    sizeof(double));
  loop_ub = (int)RES_VH[0] * (int)RES_VH[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    likelihoods->AVG_DIR_TOT_P->data[i0] = 0.0;
  }

  // 'run_Init_State:91' likelihoods.TOT_P_ALL_BACK_UP = TOT_P_ALL_BACK_UP;
  i0 = likelihoods->TOT_P_ALL_BACK_UP->size[0] * likelihoods->
    TOT_P_ALL_BACK_UP->size[1] * likelihoods->TOT_P_ALL_BACK_UP->size[2];
  likelihoods->TOT_P_ALL_BACK_UP->size[0] = (int)RES_VH[0];
  likelihoods->TOT_P_ALL_BACK_UP->size[1] = (int)RES_VH[1];
  likelihoods->TOT_P_ALL_BACK_UP->size[2] = (int)NBUFFER;
  emxEnsureCapacity((emxArray__common *)likelihoods->TOT_P_ALL_BACK_UP, i0, (int)
                    sizeof(double));
  loop_ub = (int)RES_VH[0] * (int)RES_VH[1] * (int)NBUFFER;
  for (i0 = 0; i0 < loop_ub; i0++) {
    likelihoods->TOT_P_ALL_BACK_UP->data[i0] = 0.0;
  }

  // 'run_Init_State:92' likelihoods.DIR_ALL_BACK_UP   = DIR_ALL_BACK_UP;
  i0 = likelihoods->DIR_ALL_BACK_UP->size[0] * likelihoods->
    DIR_ALL_BACK_UP->size[1] * likelihoods->DIR_ALL_BACK_UP->size[2];
  likelihoods->DIR_ALL_BACK_UP->size[0] = (int)RES_VH[0];
  likelihoods->DIR_ALL_BACK_UP->size[1] = (int)RES_VH[1];
  likelihoods->DIR_ALL_BACK_UP->size[2] = (int)NBUFFER;
  emxEnsureCapacity((emxArray__common *)likelihoods->DIR_ALL_BACK_UP, i0, (int)
                    sizeof(double));
  loop_ub = (int)RES_VH[0] * (int)RES_VH[1] * (int)NBUFFER;
  for (i0 = 0; i0 < loop_ub; i0++) {
    likelihoods->DIR_ALL_BACK_UP->data[i0] = 0.0;
  }

  // 'run_Init_State:93' likelihoods.IDX_LANE_PIX      = IDX_LANE_PIX;
  // 'run_Init_State:96' coder.cstructname(templates, 'MatlabStruct_templates'); 
  // 'run_Init_State:97' coder.cstructname(vanishingPt, 'MatlabStruct_vanishingPt'); 
  // 'run_Init_State:98' coder.cstructname(likelihoods, 'MatlabStruct_likelihoods'); 
  // 'run_Init_State:102' msg = 0;
  *msg = 0.0;
}

//
// File trailer for run_Init_State.cpp
//
// [EOF]
//
