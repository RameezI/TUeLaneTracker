//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: run_Init_State_emxutil.h
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 01-Feb-2017 19:25:15
//
#ifndef RUN_INIT_STATE_EMXUTIL_H
#define RUN_INIT_STATE_EMXUTIL_H

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
extern void c_emxFreeStruct_MatlabStruct_fo(MatlabStruct_focusMask *pStruct);
extern void c_emxFreeStruct_MatlabStruct_la(MatlabStruct_laneFilter *pStruct);
extern void c_emxFreeStruct_MatlabStruct_li(MatlabStruct_likelihoods *pStruct);
extern void c_emxFreeStruct_MatlabStruct_te(MatlabStruct_templates *pStruct);
extern void c_emxFreeStruct_MatlabStruct_vp(MatlabStruct_vpFilter *pStruct);
extern void c_emxInitStruct_MatlabStruct_fo(MatlabStruct_focusMask *pStruct);
extern void c_emxInitStruct_MatlabStruct_la(MatlabStruct_laneFilter *pStruct);
extern void c_emxInitStruct_MatlabStruct_li(MatlabStruct_likelihoods *pStruct);
extern void c_emxInitStruct_MatlabStruct_te(MatlabStruct_templates *pStruct);
extern void c_emxInitStruct_MatlabStruct_vp(MatlabStruct_vpFilter *pStruct);
extern void emxEnsureCapacity(emxArray__common *emxArray, int oldNumel, int
  elementSize);
extern void emxFree_boolean_T(emxArray_boolean_T **pEmxArray);
extern void emxFree_int32_T(emxArray_int32_T **pEmxArray);
extern void emxFree_real32_T(emxArray_real32_T **pEmxArray);
extern void emxFree_real_T(emxArray_real_T **pEmxArray);
extern void emxInit_boolean_T(emxArray_boolean_T **pEmxArray, int numDimensions);
extern void emxInit_int32_T(emxArray_int32_T **pEmxArray, int numDimensions);
extern void emxInit_real32_T(emxArray_real32_T **pEmxArray, int numDimensions);
extern void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);
extern void emxInit_real_T1(emxArray_real_T **pEmxArray, int numDimensions);
extern void emxInit_real_T2(emxArray_real_T **pEmxArray, int numDimensions);

#endif

//
// File trailer for run_Init_State_emxutil.h
//
// [EOF]
//
