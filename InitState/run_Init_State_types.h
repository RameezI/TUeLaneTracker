//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: run_Init_State_types.h
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 01-Feb-2017 19:25:15
//
#ifndef RUN_INIT_STATE_TYPES_H
#define RUN_INIT_STATE_TYPES_H

// Include Files
#include "rtwtypes.h"

// Type Definitions
#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T

struct emxArray_real_T
{
  double *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 //struct_emxArray_real_T

typedef struct {
  emxArray_real_T *FOCUS;
} MatlabStruct_focusMask;

#ifndef struct_emxArray_real32_T
#define struct_emxArray_real32_T

struct emxArray_real32_T
{
  float *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 //struct_emxArray_real32_T

typedef struct {
  emxArray_real32_T *mFilter;
  emxArray_real32_T *mPrior;
} MatlabStruct_laneFilter;

typedef struct {
  emxArray_real_T *TOT_ALL;
  emxArray_real_T *TOT_MAX;
  emxArray_real_T *GRADIENT_DIR_ALL;
  emxArray_real_T *GRADIENT_DIR_AVG;
  emxArray_real_T *TOT_ALL_BACK_UP;
  emxArray_real_T *GRADIENT_DIR_ALL_BACK_UP;
  emxArray_real_T *TOT_FOCUSED;
} MatlabStruct_likelihoods;

typedef struct {
  emxArray_real_T *GRADIENT_DIR_ROOT;
  emxArray_real_T *PROB_ROOT;
  emxArray_real_T *DEPTH_ROOT;
} MatlabStruct_templates;

typedef struct {
  double V;
  double H;
  double V_prev;
  double H_prev;
} MatlabStruct_vanishingPt;

typedef struct {
  emxArray_real32_T *mFilter;
  emxArray_real32_T *mPrior;
} MatlabStruct_vpFilter;

#ifndef struct_emxArray__common
#define struct_emxArray__common

struct emxArray__common
{
  void *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 //struct_emxArray__common

#ifndef struct_emxArray_boolean_T
#define struct_emxArray_boolean_T

struct emxArray_boolean_T
{
  boolean_T *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 //struct_emxArray_boolean_T

#ifndef struct_emxArray_int32_T
#define struct_emxArray_int32_T

struct emxArray_int32_T
{
  int *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 //struct_emxArray_int32_T
#endif

//
// File trailer for run_Init_State_types.h
//
// [EOF]
//
