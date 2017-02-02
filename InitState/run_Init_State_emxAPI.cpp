//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: run_Init_State_emxAPI.cpp
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 01-Feb-2017 19:25:15
//

// Include Files
#include "rt_nonfinite.h"
#include "run_Init_State.h"
#include "run_Init_State_emxAPI.h"
#include "run_Init_State_emxutil.h"

// Function Definitions

//
// Arguments    : int numDimensions
//                int *size
// Return Type  : emxArray_real32_T *
//
emxArray_real32_T *emxCreateND_real32_T(int numDimensions, int *size)
{
  emxArray_real32_T *emx;
  int numEl;
  int i;
  emxInit_real32_T(&emx, numDimensions);
  numEl = 1;
  for (i = 0; i < numDimensions; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = (float *)calloc((unsigned int)numEl, sizeof(float));
  emx->numDimensions = numDimensions;
  emx->allocatedSize = numEl;
  return emx;
}

//
// Arguments    : int numDimensions
//                int *size
// Return Type  : emxArray_real_T *
//
emxArray_real_T *emxCreateND_real_T(int numDimensions, int *size)
{
  emxArray_real_T *emx;
  int numEl;
  int i;
  emxInit_real_T1(&emx, numDimensions);
  numEl = 1;
  for (i = 0; i < numDimensions; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = (double *)calloc((unsigned int)numEl, sizeof(double));
  emx->numDimensions = numDimensions;
  emx->allocatedSize = numEl;
  return emx;
}

//
// Arguments    : float *data
//                int numDimensions
//                int *size
// Return Type  : emxArray_real32_T *
//
emxArray_real32_T *emxCreateWrapperND_real32_T(float *data, int numDimensions,
  int *size)
{
  emxArray_real32_T *emx;
  int numEl;
  int i;
  emxInit_real32_T(&emx, numDimensions);
  numEl = 1;
  for (i = 0; i < numDimensions; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = data;
  emx->numDimensions = numDimensions;
  emx->allocatedSize = numEl;
  emx->canFreeData = false;
  return emx;
}

//
// Arguments    : double *data
//                int numDimensions
//                int *size
// Return Type  : emxArray_real_T *
//
emxArray_real_T *emxCreateWrapperND_real_T(double *data, int numDimensions, int *
  size)
{
  emxArray_real_T *emx;
  int numEl;
  int i;
  emxInit_real_T1(&emx, numDimensions);
  numEl = 1;
  for (i = 0; i < numDimensions; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = data;
  emx->numDimensions = numDimensions;
  emx->allocatedSize = numEl;
  emx->canFreeData = false;
  return emx;
}

//
// Arguments    : float *data
//                int rows
//                int cols
// Return Type  : emxArray_real32_T *
//
emxArray_real32_T *emxCreateWrapper_real32_T(float *data, int rows, int cols)
{
  emxArray_real32_T *emx;
  int size[2];
  int numEl;
  int i;
  size[0] = rows;
  size[1] = cols;
  emxInit_real32_T(&emx, 2);
  numEl = 1;
  for (i = 0; i < 2; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = data;
  emx->numDimensions = 2;
  emx->allocatedSize = numEl;
  emx->canFreeData = false;
  return emx;
}

//
// Arguments    : double *data
//                int rows
//                int cols
// Return Type  : emxArray_real_T *
//
emxArray_real_T *emxCreateWrapper_real_T(double *data, int rows, int cols)
{
  emxArray_real_T *emx;
  int size[2];
  int numEl;
  int i;
  size[0] = rows;
  size[1] = cols;
  emxInit_real_T1(&emx, 2);
  numEl = 1;
  for (i = 0; i < 2; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = data;
  emx->numDimensions = 2;
  emx->allocatedSize = numEl;
  emx->canFreeData = false;
  return emx;
}

//
// Arguments    : int rows
//                int cols
// Return Type  : emxArray_real32_T *
//
emxArray_real32_T *emxCreate_real32_T(int rows, int cols)
{
  emxArray_real32_T *emx;
  int size[2];
  int numEl;
  int i;
  size[0] = rows;
  size[1] = cols;
  emxInit_real32_T(&emx, 2);
  numEl = 1;
  for (i = 0; i < 2; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = (float *)calloc((unsigned int)numEl, sizeof(float));
  emx->numDimensions = 2;
  emx->allocatedSize = numEl;
  return emx;
}

//
// Arguments    : int rows
//                int cols
// Return Type  : emxArray_real_T *
//
emxArray_real_T *emxCreate_real_T(int rows, int cols)
{
  emxArray_real_T *emx;
  int size[2];
  int numEl;
  int i;
  size[0] = rows;
  size[1] = cols;
  emxInit_real_T1(&emx, 2);
  numEl = 1;
  for (i = 0; i < 2; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = (double *)calloc((unsigned int)numEl, sizeof(double));
  emx->numDimensions = 2;
  emx->allocatedSize = numEl;
  return emx;
}

//
// Arguments    : emxArray_real32_T *emxArray
// Return Type  : void
//
void emxDestroyArray_real32_T(emxArray_real32_T *emxArray)
{
  emxFree_real32_T(&emxArray);
}

//
// Arguments    : emxArray_real_T *emxArray
// Return Type  : void
//
void emxDestroyArray_real_T(emxArray_real_T *emxArray)
{
  emxFree_real_T(&emxArray);
}

//
// Arguments    : MatlabStruct_focusMask emxArray
// Return Type  : void
//
void emxDestroy_MatlabStruct_focusMask(MatlabStruct_focusMask emxArray)
{
  c_emxFreeStruct_MatlabStruct_fo(&emxArray);
}

//
// Arguments    : MatlabStruct_laneFilter emxArray
// Return Type  : void
//
void emxDestroy_MatlabStruct_laneFilter(MatlabStruct_laneFilter emxArray)
{
  c_emxFreeStruct_MatlabStruct_la(&emxArray);
}

//
// Arguments    : MatlabStruct_likelihoods emxArray
// Return Type  : void
//
void emxDestroy_MatlabStruct_likelihoods(MatlabStruct_likelihoods emxArray)
{
  c_emxFreeStruct_MatlabStruct_li(&emxArray);
}

//
// Arguments    : MatlabStruct_templates emxArray
// Return Type  : void
//
void emxDestroy_MatlabStruct_templates(MatlabStruct_templates emxArray)
{
  c_emxFreeStruct_MatlabStruct_te(&emxArray);
}

//
// Arguments    : MatlabStruct_vpFilter emxArray
// Return Type  : void
//
void emxDestroy_MatlabStruct_vpFilter(MatlabStruct_vpFilter emxArray)
{
  c_emxFreeStruct_MatlabStruct_vp(&emxArray);
}

//
// Arguments    : MatlabStruct_focusMask *pStruct
// Return Type  : void
//
void emxInit_MatlabStruct_focusMask(MatlabStruct_focusMask *pStruct)
{
  c_emxInitStruct_MatlabStruct_fo(pStruct);
}

//
// Arguments    : MatlabStruct_laneFilter *pStruct
// Return Type  : void
//
void emxInit_MatlabStruct_laneFilter(MatlabStruct_laneFilter *pStruct)
{
  c_emxInitStruct_MatlabStruct_la(pStruct);
}

//
// Arguments    : MatlabStruct_likelihoods *pStruct
// Return Type  : void
//
void emxInit_MatlabStruct_likelihoods(MatlabStruct_likelihoods *pStruct)
{
  c_emxInitStruct_MatlabStruct_li(pStruct);
}

//
// Arguments    : MatlabStruct_templates *pStruct
// Return Type  : void
//
void emxInit_MatlabStruct_templates(MatlabStruct_templates *pStruct)
{
  c_emxInitStruct_MatlabStruct_te(pStruct);
}

//
// Arguments    : MatlabStruct_vpFilter *pStruct
// Return Type  : void
//
void emxInit_MatlabStruct_vpFilter(MatlabStruct_vpFilter *pStruct)
{
  c_emxInitStruct_MatlabStruct_vp(pStruct);
}

//
// File trailer for run_Init_State_emxAPI.cpp
//
// [EOF]
//
