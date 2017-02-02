//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: run_Init_State_emxAPI.h
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 01-Feb-2017 19:25:15
//
#ifndef RUN_INIT_STATE_EMXAPI_H
#define RUN_INIT_STATE_EMXAPI_H

// Include Files
#include <cmath>
#include <float.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "omp.h"
#include "run_Init_State_types.h"

// Function Declarations
extern emxArray_real32_T *emxCreateND_real32_T(int numDimensions, int *size);
extern emxArray_real_T *emxCreateND_real_T(int numDimensions, int *size);
extern emxArray_real32_T *emxCreateWrapperND_real32_T(float *data, int
  numDimensions, int *size);
extern emxArray_real_T *emxCreateWrapperND_real_T(double *data, int
  numDimensions, int *size);
extern emxArray_real32_T *emxCreateWrapper_real32_T(float *data, int rows, int
  cols);
extern emxArray_real_T *emxCreateWrapper_real_T(double *data, int rows, int cols);
extern emxArray_real32_T *emxCreate_real32_T(int rows, int cols);
extern emxArray_real_T *emxCreate_real_T(int rows, int cols);
extern void emxDestroyArray_real32_T(emxArray_real32_T *emxArray);
extern void emxDestroyArray_real_T(emxArray_real_T *emxArray);
extern void emxDestroy_MatlabStruct_focusMask(MatlabStruct_focusMask emxArray);
extern void emxDestroy_MatlabStruct_laneFilter(MatlabStruct_laneFilter emxArray);
extern void emxDestroy_MatlabStruct_likelihoods(MatlabStruct_likelihoods
  emxArray);
extern void emxDestroy_MatlabStruct_templates(MatlabStruct_templates emxArray);
extern void emxDestroy_MatlabStruct_vpFilter(MatlabStruct_vpFilter emxArray);
extern void emxInit_MatlabStruct_focusMask(MatlabStruct_focusMask *pStruct);
extern void emxInit_MatlabStruct_laneFilter(MatlabStruct_laneFilter *pStruct);
extern void emxInit_MatlabStruct_likelihoods(MatlabStruct_likelihoods *pStruct);
extern void emxInit_MatlabStruct_templates(MatlabStruct_templates *pStruct);
extern void emxInit_MatlabStruct_vpFilter(MatlabStruct_vpFilter *pStruct);

#endif

//
// File trailer for run_Init_State_emxAPI.h
//
// [EOF]
//
