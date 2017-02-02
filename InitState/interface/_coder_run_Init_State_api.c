/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_run_Init_State_api.c
 *
 * MATLAB Coder version            : 3.2
 * C/C++ source code generated on  : 01-Feb-2017 19:25:15
 */

/* Include Files */
#include "tmwtypes.h"
#include "_coder_run_Init_State_api.h"
#include "_coder_run_Init_State_mex.h"

/* Type Definitions */
#ifndef struct_emxArray__common
#define struct_emxArray__common

struct emxArray__common
{
  void *data;
  int32_T *size;
  int32_T allocatedSize;
  int32_T numDimensions;
  boolean_T canFreeData;
};

#endif                                 /*struct_emxArray__common*/

#ifndef typedef_emxArray__common
#define typedef_emxArray__common

typedef struct emxArray__common emxArray__common;

#endif                                 /*typedef_emxArray__common*/

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;
emlrtContext emlrtContextGlobal = { true,/* bFirstTime */
  false,                               /* bInitialized */
  131435U,                             /* fVersionInfo */
  NULL,                                /* fErrorFunction */
  "run_Init_State",                    /* fFunctionName */
  NULL,                                /* fRTCallStack */
  false,                               /* bDebugMode */
  { 2045744189U, 2170104910U, 2743257031U, 4284093946U },/* fSigWrd */
  NULL                                 /* fSigMem */
};

/* Function Declarations */
static int32_T (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
  const emlrtMsgIdentifier *parentId))[2];
static const mxArray *b_emlrt_marshallOut(const emxArray_real_T *u);
static int32_T c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *NBUFFER,
  const char_T *identifier);
static const mxArray *c_emlrt_marshallOut(const emxArray_real_T *u);
static void c_emxFreeStruct_MatlabStruct_fo(MatlabStruct_focusMask *pStruct);
static void c_emxFreeStruct_MatlabStruct_la(MatlabStruct_laneFilter *pStruct);
static void c_emxFreeStruct_MatlabStruct_li(MatlabStruct_likelihoods *pStruct);
static void c_emxFreeStruct_MatlabStruct_te(MatlabStruct_templates *pStruct);
static void c_emxFreeStruct_MatlabStruct_vp(MatlabStruct_vpFilter *pStruct);
static void c_emxInitStruct_MatlabStruct_fo(const emlrtStack *sp,
  MatlabStruct_focusMask *pStruct, boolean_T doPush);
static void c_emxInitStruct_MatlabStruct_la(const emlrtStack *sp,
  MatlabStruct_laneFilter *pStruct, boolean_T doPush);
static void c_emxInitStruct_MatlabStruct_li(const emlrtStack *sp,
  MatlabStruct_likelihoods *pStruct, boolean_T doPush);
static void c_emxInitStruct_MatlabStruct_te(const emlrtStack *sp,
  MatlabStruct_templates *pStruct, boolean_T doPush);
static void c_emxInitStruct_MatlabStruct_vp(const emlrtStack *sp,
  MatlabStruct_vpFilter *pStruct, boolean_T doPush);
static int32_T d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId);
static const mxArray *d_emlrt_marshallOut(const MatlabStruct_templates u);
static void e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *laneFilter,
  const char_T *identifier, MatlabStruct_laneFilter *y);
static const mxArray *e_emlrt_marshallOut(const MatlabStruct_vanishingPt u);
static int32_T (*emlrt_marshallIn(const emlrtStack *sp, const mxArray *RES_VH,
  const char_T *identifier))[2];
static const mxArray *emlrt_marshallOut(const MatlabStruct_likelihoods u);
static void emxEnsureCapacity(emxArray__common *emxArray, int32_T oldNumel,
  int32_T elementSize);
static void emxFree_real32_T(emxArray_real32_T **pEmxArray);
static void emxFree_real_T(emxArray_real_T **pEmxArray);
static void emxInit_real32_T(const emlrtStack *sp, emxArray_real32_T **pEmxArray,
  int32_T numDimensions, boolean_T doPush);
static void emxInit_real_T(const emlrtStack *sp, emxArray_real_T **pEmxArray,
  int32_T numDimensions, boolean_T doPush);
static void emxInit_real_T1(const emlrtStack *sp, emxArray_real_T **pEmxArray,
  int32_T numDimensions, boolean_T doPush);
static void f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, MatlabStruct_laneFilter *y);
static const mxArray *f_emlrt_marshallOut(const MatlabStruct_focusMask u);
static void g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, emxArray_real32_T *y);
static void h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *vpFilter,
  const char_T *identifier, MatlabStruct_vpFilter *y);
static void i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, MatlabStruct_vpFilter *y);
static int32_T (*j_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[2];
static int32_T k_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId);
static void l_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, emxArray_real32_T *ret);

/* Function Definitions */

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : int32_T (*)[2]
 */
static int32_T (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
  const emlrtMsgIdentifier *parentId))[2]
{
  int32_T (*y)[2];
  y = j_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}
/*
 * Arguments    : const emxArray_real_T *u
 * Return Type  : const mxArray *
 */
  static const mxArray *b_emlrt_marshallOut(const emxArray_real_T *u)
{
  const mxArray *y;
  const mxArray *m0;
  real_T *pData;
  int32_T i0;
  int32_T i;
  int32_T b_i;
  int32_T c_i;
  y = NULL;
  m0 = emlrtCreateNumericArray(3, *(int32_T (*)[3])u->size, mxDOUBLE_CLASS,
    mxREAL);
  pData = (real_T *)mxGetPr(m0);
  i0 = 0;
  for (i = 0; i < u->size[2]; i++) {
    for (b_i = 0; b_i < u->size[1]; b_i++) {
      for (c_i = 0; c_i < u->size[0]; c_i++) {
        pData[i0] = u->data[(c_i + u->size[0] * b_i) + u->size[0] * u->size[1] *
          i];
        i0++;
      }
    }
  }

  emlrtAssign(&y, m0);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *NBUFFER
 *                const char_T *identifier
 * Return Type  : int32_T
 */
static int32_T c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *NBUFFER,
  const char_T *identifier)
{
  int32_T y;
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = d_emlrt_marshallIn(sp, emlrtAlias(NBUFFER), &thisId);
  emlrtDestroyArray(&NBUFFER);
  return y;
}

/*
 * Arguments    : const emxArray_real_T *u
 * Return Type  : const mxArray *
 */
static const mxArray *c_emlrt_marshallOut(const emxArray_real_T *u)
{
  const mxArray *y;
  const mxArray *m1;
  real_T *pData;
  int32_T i1;
  int32_T i;
  int32_T b_i;
  y = NULL;
  m1 = emlrtCreateNumericArray(2, *(int32_T (*)[3])u->size, mxDOUBLE_CLASS,
    mxREAL);
  pData = (real_T *)mxGetPr(m1);
  i1 = 0;
  for (i = 0; i < u->size[1]; i++) {
    for (b_i = 0; b_i < u->size[0]; b_i++) {
      pData[i1] = u->data[b_i + u->size[0] * i];
      i1++;
    }
  }

  emlrtAssign(&y, m1);
  return y;
}

/*
 * Arguments    : MatlabStruct_focusMask *pStruct
 * Return Type  : void
 */
static void c_emxFreeStruct_MatlabStruct_fo(MatlabStruct_focusMask *pStruct)
{
  emxFree_real_T(&pStruct->FOCUS);
}

/*
 * Arguments    : MatlabStruct_laneFilter *pStruct
 * Return Type  : void
 */
static void c_emxFreeStruct_MatlabStruct_la(MatlabStruct_laneFilter *pStruct)
{
  emxFree_real32_T(&pStruct->mFilter);
  emxFree_real32_T(&pStruct->mPrior);
}

/*
 * Arguments    : MatlabStruct_likelihoods *pStruct
 * Return Type  : void
 */
static void c_emxFreeStruct_MatlabStruct_li(MatlabStruct_likelihoods *pStruct)
{
  emxFree_real_T(&pStruct->TOT_ALL);
  emxFree_real_T(&pStruct->TOT_MAX);
  emxFree_real_T(&pStruct->GRADIENT_DIR_ALL);
  emxFree_real_T(&pStruct->GRADIENT_DIR_AVG);
  emxFree_real_T(&pStruct->TOT_ALL_BACK_UP);
  emxFree_real_T(&pStruct->GRADIENT_DIR_ALL_BACK_UP);
  emxFree_real_T(&pStruct->TOT_FOCUSED);
}

/*
 * Arguments    : MatlabStruct_templates *pStruct
 * Return Type  : void
 */
static void c_emxFreeStruct_MatlabStruct_te(MatlabStruct_templates *pStruct)
{
  emxFree_real_T(&pStruct->GRADIENT_DIR_ROOT);
  emxFree_real_T(&pStruct->PROB_ROOT);
  emxFree_real_T(&pStruct->DEPTH_ROOT);
}

/*
 * Arguments    : MatlabStruct_vpFilter *pStruct
 * Return Type  : void
 */
static void c_emxFreeStruct_MatlabStruct_vp(MatlabStruct_vpFilter *pStruct)
{
  emxFree_real32_T(&pStruct->mFilter);
  emxFree_real32_T(&pStruct->mPrior);
}

/*
 * Arguments    : const emlrtStack *sp
 *                MatlabStruct_focusMask *pStruct
 *                boolean_T doPush
 * Return Type  : void
 */
static void c_emxInitStruct_MatlabStruct_fo(const emlrtStack *sp,
  MatlabStruct_focusMask *pStruct, boolean_T doPush)
{
  emxInit_real_T1(sp, &pStruct->FOCUS, 2, doPush);
}

/*
 * Arguments    : const emlrtStack *sp
 *                MatlabStruct_laneFilter *pStruct
 *                boolean_T doPush
 * Return Type  : void
 */
static void c_emxInitStruct_MatlabStruct_la(const emlrtStack *sp,
  MatlabStruct_laneFilter *pStruct, boolean_T doPush)
{
  emxInit_real32_T(sp, &pStruct->mFilter, 2, doPush);
  emxInit_real32_T(sp, &pStruct->mPrior, 2, doPush);
}

/*
 * Arguments    : const emlrtStack *sp
 *                MatlabStruct_likelihoods *pStruct
 *                boolean_T doPush
 * Return Type  : void
 */
static void c_emxInitStruct_MatlabStruct_li(const emlrtStack *sp,
  MatlabStruct_likelihoods *pStruct, boolean_T doPush)
{
  emxInit_real_T(sp, &pStruct->TOT_ALL, 3, doPush);
  emxInit_real_T1(sp, &pStruct->TOT_MAX, 2, doPush);
  emxInit_real_T(sp, &pStruct->GRADIENT_DIR_ALL, 3, doPush);
  emxInit_real_T1(sp, &pStruct->GRADIENT_DIR_AVG, 2, doPush);
  emxInit_real_T(sp, &pStruct->TOT_ALL_BACK_UP, 3, doPush);
  emxInit_real_T(sp, &pStruct->GRADIENT_DIR_ALL_BACK_UP, 3, doPush);
  emxInit_real_T1(sp, &pStruct->TOT_FOCUSED, 2, doPush);
}

/*
 * Arguments    : const emlrtStack *sp
 *                MatlabStruct_templates *pStruct
 *                boolean_T doPush
 * Return Type  : void
 */
static void c_emxInitStruct_MatlabStruct_te(const emlrtStack *sp,
  MatlabStruct_templates *pStruct, boolean_T doPush)
{
  emxInit_real_T1(sp, &pStruct->GRADIENT_DIR_ROOT, 2, doPush);
  emxInit_real_T1(sp, &pStruct->PROB_ROOT, 2, doPush);
  emxInit_real_T1(sp, &pStruct->DEPTH_ROOT, 2, doPush);
}

/*
 * Arguments    : const emlrtStack *sp
 *                MatlabStruct_vpFilter *pStruct
 *                boolean_T doPush
 * Return Type  : void
 */
static void c_emxInitStruct_MatlabStruct_vp(const emlrtStack *sp,
  MatlabStruct_vpFilter *pStruct, boolean_T doPush)
{
  emxInit_real32_T(sp, &pStruct->mFilter, 2, doPush);
  emxInit_real32_T(sp, &pStruct->mPrior, 2, doPush);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : int32_T
 */
static int32_T d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId)
{
  int32_T y;
  y = k_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const MatlabStruct_templates u
 * Return Type  : const mxArray *
 */
static const mxArray *d_emlrt_marshallOut(const MatlabStruct_templates u)
{
  const mxArray *y;
  y = NULL;
  emlrtAssign(&y, emlrtCreateStructMatrix(1, 1, 0, NULL));
  emlrtAddField(y, c_emlrt_marshallOut(u.GRADIENT_DIR_ROOT), "GRADIENT_DIR_ROOT",
                0);
  emlrtAddField(y, c_emlrt_marshallOut(u.PROB_ROOT), "PROB_ROOT", 0);
  emlrtAddField(y, c_emlrt_marshallOut(u.DEPTH_ROOT), "DEPTH_ROOT", 0);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *laneFilter
 *                const char_T *identifier
 *                MatlabStruct_laneFilter *y
 * Return Type  : void
 */
static void e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *laneFilter,
  const char_T *identifier, MatlabStruct_laneFilter *y)
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  f_emlrt_marshallIn(sp, emlrtAlias(laneFilter), &thisId, y);
  emlrtDestroyArray(&laneFilter);
}

/*
 * Arguments    : const MatlabStruct_vanishingPt u
 * Return Type  : const mxArray *
 */
static const mxArray *e_emlrt_marshallOut(const MatlabStruct_vanishingPt u)
{
  const mxArray *y;
  const mxArray *b_y;
  const mxArray *m2;
  const mxArray *c_y;
  const mxArray *d_y;
  const mxArray *e_y;
  y = NULL;
  emlrtAssign(&y, emlrtCreateStructMatrix(1, 1, 0, NULL));
  b_y = NULL;
  m2 = emlrtCreateDoubleScalar(u.V);
  emlrtAssign(&b_y, m2);
  emlrtAddField(y, b_y, "V", 0);
  c_y = NULL;
  m2 = emlrtCreateDoubleScalar(u.H);
  emlrtAssign(&c_y, m2);
  emlrtAddField(y, c_y, "H", 0);
  d_y = NULL;
  m2 = emlrtCreateDoubleScalar(u.V_prev);
  emlrtAssign(&d_y, m2);
  emlrtAddField(y, d_y, "V_prev", 0);
  e_y = NULL;
  m2 = emlrtCreateDoubleScalar(u.H_prev);
  emlrtAssign(&e_y, m2);
  emlrtAddField(y, e_y, "H_prev", 0);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *RES_VH
 *                const char_T *identifier
 * Return Type  : int32_T (*)[2]
 */
static int32_T (*emlrt_marshallIn(const emlrtStack *sp, const mxArray *RES_VH,
  const char_T *identifier))[2]
{
  int32_T (*y)[2];
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = b_emlrt_marshallIn(sp, emlrtAlias(RES_VH), &thisId);
  emlrtDestroyArray(&RES_VH);
  return y;
}
/*
 * Arguments    : const MatlabStruct_likelihoods u
 * Return Type  : const mxArray *
 */
  static const mxArray *emlrt_marshallOut(const MatlabStruct_likelihoods u)
{
  const mxArray *y;
  y = NULL;
  emlrtAssign(&y, emlrtCreateStructMatrix(1, 1, 0, NULL));
  emlrtAddField(y, b_emlrt_marshallOut(u.TOT_ALL), "TOT_ALL", 0);
  emlrtAddField(y, c_emlrt_marshallOut(u.TOT_MAX), "TOT_MAX", 0);
  emlrtAddField(y, b_emlrt_marshallOut(u.GRADIENT_DIR_ALL), "GRADIENT_DIR_ALL",
                0);
  emlrtAddField(y, c_emlrt_marshallOut(u.GRADIENT_DIR_AVG), "GRADIENT_DIR_AVG",
                0);
  emlrtAddField(y, b_emlrt_marshallOut(u.TOT_ALL_BACK_UP), "TOT_ALL_BACK_UP", 0);
  emlrtAddField(y, b_emlrt_marshallOut(u.GRADIENT_DIR_ALL_BACK_UP),
                "GRADIENT_DIR_ALL_BACK_UP", 0);
  emlrtAddField(y, c_emlrt_marshallOut(u.TOT_FOCUSED), "TOT_FOCUSED", 0);
  return y;
}

/*
 * Arguments    : emxArray__common *emxArray
 *                int32_T oldNumel
 *                int32_T elementSize
 * Return Type  : void
 */
static void emxEnsureCapacity(emxArray__common *emxArray, int32_T oldNumel,
  int32_T elementSize)
{
  int32_T newNumel;
  int32_T i;
  void *newData;
  if (oldNumel < 0) {
    oldNumel = 0;
  }

  newNumel = 1;
  for (i = 0; i < emxArray->numDimensions; i++) {
    newNumel *= emxArray->size[i];
  }

  if (newNumel > emxArray->allocatedSize) {
    i = emxArray->allocatedSize;
    if (i < 16) {
      i = 16;
    }

    while (i < newNumel) {
      if (i > 1073741823) {
        i = MAX_int32_T;
      } else {
        i <<= 1;
      }
    }

    newData = emlrtCallocMex((uint32_T)i, (uint32_T)elementSize);
    if (emxArray->data != NULL) {
      memcpy(newData, emxArray->data, (uint32_T)(elementSize * oldNumel));
      if (emxArray->canFreeData) {
        emlrtFreeMex(emxArray->data);
      }
    }

    emxArray->data = newData;
    emxArray->allocatedSize = i;
    emxArray->canFreeData = true;
  }
}

/*
 * Arguments    : emxArray_real32_T **pEmxArray
 * Return Type  : void
 */
static void emxFree_real32_T(emxArray_real32_T **pEmxArray)
{
  if (*pEmxArray != (emxArray_real32_T *)NULL) {
    if (((*pEmxArray)->data != (real32_T *)NULL) && (*pEmxArray)->canFreeData) {
      emlrtFreeMex((void *)(*pEmxArray)->data);
    }

    emlrtFreeMex((void *)(*pEmxArray)->size);
    emlrtFreeMex((void *)*pEmxArray);
    *pEmxArray = (emxArray_real32_T *)NULL;
  }
}

/*
 * Arguments    : emxArray_real_T **pEmxArray
 * Return Type  : void
 */
static void emxFree_real_T(emxArray_real_T **pEmxArray)
{
  if (*pEmxArray != (emxArray_real_T *)NULL) {
    if (((*pEmxArray)->data != (real_T *)NULL) && (*pEmxArray)->canFreeData) {
      emlrtFreeMex((void *)(*pEmxArray)->data);
    }

    emlrtFreeMex((void *)(*pEmxArray)->size);
    emlrtFreeMex((void *)*pEmxArray);
    *pEmxArray = (emxArray_real_T *)NULL;
  }
}

/*
 * Arguments    : const emlrtStack *sp
 *                emxArray_real32_T **pEmxArray
 *                int32_T numDimensions
 *                boolean_T doPush
 * Return Type  : void
 */
static void emxInit_real32_T(const emlrtStack *sp, emxArray_real32_T **pEmxArray,
  int32_T numDimensions, boolean_T doPush)
{
  emxArray_real32_T *emxArray;
  int32_T i;
  *pEmxArray = (emxArray_real32_T *)emlrtMallocMex(sizeof(emxArray_real32_T));
  if (doPush) {
    emlrtPushHeapReferenceStackR2012b(sp, (void *)pEmxArray, (void (*)(void *))
      emxFree_real32_T);
  }

  emxArray = *pEmxArray;
  emxArray->data = (real32_T *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int32_T *)emlrtMallocMex((uint32_T)(sizeof(int32_T)
    * numDimensions));
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (i = 0; i < numDimensions; i++) {
    emxArray->size[i] = 0;
  }
}

/*
 * Arguments    : const emlrtStack *sp
 *                emxArray_real_T **pEmxArray
 *                int32_T numDimensions
 *                boolean_T doPush
 * Return Type  : void
 */
static void emxInit_real_T(const emlrtStack *sp, emxArray_real_T **pEmxArray,
  int32_T numDimensions, boolean_T doPush)
{
  emxArray_real_T *emxArray;
  int32_T i;
  *pEmxArray = (emxArray_real_T *)emlrtMallocMex(sizeof(emxArray_real_T));
  if (doPush) {
    emlrtPushHeapReferenceStackR2012b(sp, (void *)pEmxArray, (void (*)(void *))
      emxFree_real_T);
  }

  emxArray = *pEmxArray;
  emxArray->data = (real_T *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int32_T *)emlrtMallocMex((uint32_T)(sizeof(int32_T)
    * numDimensions));
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (i = 0; i < numDimensions; i++) {
    emxArray->size[i] = 0;
  }
}

/*
 * Arguments    : const emlrtStack *sp
 *                emxArray_real_T **pEmxArray
 *                int32_T numDimensions
 *                boolean_T doPush
 * Return Type  : void
 */
static void emxInit_real_T1(const emlrtStack *sp, emxArray_real_T **pEmxArray,
  int32_T numDimensions, boolean_T doPush)
{
  emxArray_real_T *emxArray;
  int32_T i;
  *pEmxArray = (emxArray_real_T *)emlrtMallocMex(sizeof(emxArray_real_T));
  if (doPush) {
    emlrtPushHeapReferenceStackR2012b(sp, (void *)pEmxArray, (void (*)(void *))
      emxFree_real_T);
  }

  emxArray = *pEmxArray;
  emxArray->data = (real_T *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int32_T *)emlrtMallocMex((uint32_T)(sizeof(int32_T)
    * numDimensions));
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (i = 0; i < numDimensions; i++) {
    emxArray->size[i] = 0;
  }
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 *                MatlabStruct_laneFilter *y
 * Return Type  : void
 */
static void f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, MatlabStruct_laneFilter *y)
{
  emlrtMsgIdentifier thisId;
  static const char * fieldNames[2] = { "mFilter", "mPrior" };

  static const int32_T dims = 0;
  thisId.fParent = parentId;
  thisId.bParentIsCell = false;
  emlrtCheckStructR2012b(sp, parentId, u, 2, fieldNames, 0U, &dims);
  thisId.fIdentifier = "mFilter";
  g_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0, "mFilter")),
                     &thisId, y->mFilter);
  thisId.fIdentifier = "mPrior";
  g_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0, "mPrior")),
                     &thisId, y->mPrior);
  emlrtDestroyArray(&u);
}

/*
 * Arguments    : const MatlabStruct_focusMask u
 * Return Type  : const mxArray *
 */
static const mxArray *f_emlrt_marshallOut(const MatlabStruct_focusMask u)
{
  const mxArray *y;
  y = NULL;
  emlrtAssign(&y, emlrtCreateStructMatrix(1, 1, 0, NULL));
  emlrtAddField(y, c_emlrt_marshallOut(u.FOCUS), "FOCUS", 0);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 *                emxArray_real32_T *y
 * Return Type  : void
 */
static void g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, emxArray_real32_T *y)
{
  l_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *vpFilter
 *                const char_T *identifier
 *                MatlabStruct_vpFilter *y
 * Return Type  : void
 */
static void h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *vpFilter,
  const char_T *identifier, MatlabStruct_vpFilter *y)
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  i_emlrt_marshallIn(sp, emlrtAlias(vpFilter), &thisId, y);
  emlrtDestroyArray(&vpFilter);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 *                MatlabStruct_vpFilter *y
 * Return Type  : void
 */
static void i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, MatlabStruct_vpFilter *y)
{
  emlrtMsgIdentifier thisId;
  static const char * fieldNames[2] = { "mFilter", "mPrior" };

  static const int32_T dims = 0;
  thisId.fParent = parentId;
  thisId.bParentIsCell = false;
  emlrtCheckStructR2012b(sp, parentId, u, 2, fieldNames, 0U, &dims);
  thisId.fIdentifier = "mFilter";
  g_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0, "mFilter")),
                     &thisId, y->mFilter);
  thisId.fIdentifier = "mPrior";
  g_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0, "mPrior")),
                     &thisId, y->mPrior);
  emlrtDestroyArray(&u);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : int32_T (*)[2]
 */
static int32_T (*j_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[2]
{
  int32_T (*ret)[2];
  static const int32_T dims[2] = { 1, 2 };

  emlrtCheckBuiltInR2012b(sp, msgId, src, "int32", false, 2U, dims);
  ret = (int32_T (*)[2])mxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}
/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : int32_T
 */
  static int32_T k_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId)
{
  int32_T ret;
  static const int32_T dims = 0;
  emlrtCheckBuiltInR2012b(sp, msgId, src, "int32", false, 0U, &dims);
  ret = *(int32_T *)mxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 *                emxArray_real32_T *ret
 * Return Type  : void
 */
static void l_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, emxArray_real32_T *ret)
{
  static const int32_T dims[2] = { -1, -1 };

  boolean_T bv0[2] = { true, true };

  int32_T iv0[2];
  int32_T i2;
  emlrtCheckVsBuiltInR2012b(sp, msgId, src, "single", false, 2U, dims, &bv0[0],
    iv0);
  i2 = ret->size[0] * ret->size[1];
  ret->size[0] = iv0[0];
  ret->size[1] = iv0[1];
  emxEnsureCapacity((emxArray__common *)ret, i2, (int32_T)sizeof(real32_T));
  emlrtImportArrayR2015b(sp, src, ret->data, 4, false);
  emlrtDestroyArray(&src);
}

/*
 * Arguments    : const mxArray *prhs[4]
 *                const mxArray *plhs[4]
 * Return Type  : void
 */
void run_Init_State_api(const mxArray *prhs[4], const mxArray *plhs[4])
{
  MatlabStruct_laneFilter laneFilter;
  MatlabStruct_vpFilter vpFilter;
  MatlabStruct_likelihoods likelihoods;
  MatlabStruct_templates templates;
  MatlabStruct_focusMask masks;
  int32_T (*RES_VH)[2];
  int32_T NBUFFER;
  MatlabStruct_vanishingPt vanishingPt;
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  st.tls = emlrtRootTLSGlobal;
  emlrtHeapReferenceStackEnterFcnR2012b(&st);
  c_emxInitStruct_MatlabStruct_la(&st, &laneFilter, true);
  c_emxInitStruct_MatlabStruct_vp(&st, &vpFilter, true);
  c_emxInitStruct_MatlabStruct_li(&st, &likelihoods, true);
  c_emxInitStruct_MatlabStruct_te(&st, &templates, true);
  c_emxInitStruct_MatlabStruct_fo(&st, &masks, true);
  prhs[0] = emlrtProtectR2012b(prhs[0], 0, false, -1);

  /* Marshall function inputs */
  RES_VH = emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "RES_VH");
  NBUFFER = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[1]), "NBUFFER");
  e_emlrt_marshallIn(&st, emlrtAliasP(prhs[2]), "laneFilter", &laneFilter);
  h_emlrt_marshallIn(&st, emlrtAliasP(prhs[3]), "vpFilter", &vpFilter);

  /* Invoke the target function */
  run_Init_State(*RES_VH, NBUFFER, &laneFilter, &vpFilter, &likelihoods,
                 &templates, &vanishingPt, &masks);

  /* Marshall function outputs */
  plhs[0] = emlrt_marshallOut(likelihoods);
  plhs[1] = d_emlrt_marshallOut(templates);
  plhs[2] = e_emlrt_marshallOut(vanishingPt);
  plhs[3] = f_emlrt_marshallOut(masks);
  c_emxFreeStruct_MatlabStruct_fo(&masks);
  c_emxFreeStruct_MatlabStruct_te(&templates);
  c_emxFreeStruct_MatlabStruct_li(&likelihoods);
  c_emxFreeStruct_MatlabStruct_vp(&vpFilter);
  c_emxFreeStruct_MatlabStruct_la(&laneFilter);
  emlrtHeapReferenceStackLeaveFcnR2012b(&st);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void run_Init_State_atexit(void)
{
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  run_Init_State_xil_terminate();
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void run_Init_State_initialize(void)
{
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtClearAllocCountR2012b(&st, false, 0U, 0);
  emlrtEnterRtStackR2012b(&st);
  emlrtFirstTimeR2012b(emlrtRootTLSGlobal);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void run_Init_State_terminate(void)
{
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  st.tls = emlrtRootTLSGlobal;
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/*
 * File trailer for _coder_run_Init_State_api.c
 *
 * [EOF]
 */
