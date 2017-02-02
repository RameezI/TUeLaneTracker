//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: main.cpp
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 01-Feb-2017 19:25:15
//

//***********************************************************************
// This automatically generated example C main file shows how to call
// entry-point functions that MATLAB Coder generated. You must customize
// this file for your application. Do not modify this file directly.
// Instead, make a copy of this file, modify it, and integrate it into
// your development environment.
//
// This file initializes entry-point function arguments to a default
// size and value before calling the entry-point functions. It does
// not store or use any values returned from the entry-point functions.
// If necessary, it does pre-allocate memory for returned values.
// You can use this file as a starting point for a main function that
// you can deploy in your application.
//
// After you copy the file, and before you deploy it, you must make the
// following changes:
// * For variable-size function arguments, change the example sizes to
// the sizes that your application requires.
// * Change the example values of function arguments to the values that
// your application requires.
// * If the entry-point functions return values, store these values or
// otherwise use them as required by your application.
//
//***********************************************************************
// Include Files
#include "rt_nonfinite.h"
#include "run_Init_State.h"
#include "main.h"
#include "run_Init_State_terminate.h"
#include "run_Init_State_emxAPI.h"
#include "run_Init_State_initialize.h"

// Function Declarations
static void argInit_1x2_int32_T(int result[2]);
static MatlabStruct_laneFilter argInit_MatlabStruct_laneFilter();
static MatlabStruct_vpFilter argInit_MatlabStruct_vpFilter();
static int argInit_int32_T();
static float argInit_real32_T();
static emxArray_real32_T *c_argInit_UnboundedxUnbounded_r();
static void main_run_Init_State();

// Function Definitions

//
// Arguments    : int result[2]
// Return Type  : void
//
static void argInit_1x2_int32_T(int result[2])
{
  int idx1;

  // Loop over the array to initialize each element.
  for (idx1 = 0; idx1 < 2; idx1++) {
    // Set the value of the array element.
    // Change this value to the value that the application requires.
    result[idx1] = argInit_int32_T();
  }
}

//
// Arguments    : void
// Return Type  : MatlabStruct_laneFilter
//
static MatlabStruct_laneFilter argInit_MatlabStruct_laneFilter()
{
  MatlabStruct_laneFilter result;

  // Set the value of each structure field.
  // Change this value to the value that the application requires.
  result.mFilter = c_argInit_UnboundedxUnbounded_r();
  result.mPrior = c_argInit_UnboundedxUnbounded_r();
  return result;
}

//
// Arguments    : void
// Return Type  : MatlabStruct_vpFilter
//
static MatlabStruct_vpFilter argInit_MatlabStruct_vpFilter()
{
  MatlabStruct_vpFilter result;

  // Set the value of each structure field.
  // Change this value to the value that the application requires.
  result.mFilter = c_argInit_UnboundedxUnbounded_r();
  result.mPrior = c_argInit_UnboundedxUnbounded_r();
  return result;
}

//
// Arguments    : void
// Return Type  : int
//
static int argInit_int32_T()
{
  return 0;
}

//
// Arguments    : void
// Return Type  : float
//
static float argInit_real32_T()
{
  return 0.0F;
}

//
// Arguments    : void
// Return Type  : emxArray_real32_T *
//
static emxArray_real32_T *c_argInit_UnboundedxUnbounded_r()
{
  emxArray_real32_T *result;
  static int iv0[2] = { 2, 2 };

  int idx0;
  int idx1;

  // Set the size of the array.
  // Change this size to the value that the application requires.
  result = emxCreateND_real32_T(2, *(int (*)[2])&iv0[0]);

  // Loop over the array to initialize each element.
  for (idx0 = 0; idx0 < result->size[0U]; idx0++) {
    for (idx1 = 0; idx1 < result->size[1U]; idx1++) {
      // Set the value of the array element.
      // Change this value to the value that the application requires.
      result->data[idx0 + result->size[0] * idx1] = argInit_real32_T();
    }
  }

  return result;
}

//
// Arguments    : void
// Return Type  : void
//
static void main_run_Init_State()
{
  MatlabStruct_likelihoods likelihoods;
  MatlabStruct_templates templates;
  MatlabStruct_focusMask masks;
  int RES_VH[2];
  int NBUFFER;
  MatlabStruct_laneFilter laneFilter;
  MatlabStruct_vpFilter vpFilter;
  MatlabStruct_vanishingPt vanishingPt;
  emxInit_MatlabStruct_likelihoods(&likelihoods);
  emxInit_MatlabStruct_templates(&templates);
  emxInit_MatlabStruct_focusMask(&masks);

  // Initialize function 'run_Init_State' input arguments.
  // Initialize function input argument 'RES_VH'.
  argInit_1x2_int32_T(RES_VH);
  NBUFFER = argInit_int32_T();

  // Initialize function input argument 'laneFilter'.
  laneFilter = argInit_MatlabStruct_laneFilter();

  // Initialize function input argument 'vpFilter'.
  vpFilter = argInit_MatlabStruct_vpFilter();

  // Call the entry-point 'run_Init_State'.
  run_Init_State(RES_VH, NBUFFER, &laneFilter, &vpFilter, &likelihoods,
                 &templates, &vanishingPt, &masks);
  emxDestroy_MatlabStruct_focusMask(masks);
  emxDestroy_MatlabStruct_templates(templates);
  emxDestroy_MatlabStruct_likelihoods(likelihoods);
  emxDestroy_MatlabStruct_vpFilter(vpFilter);
  emxDestroy_MatlabStruct_laneFilter(laneFilter);
}

//
// Arguments    : int argc
//                const char * const argv[]
// Return Type  : int
//
int main(int, const char * const [])
{
  // Initialize the application.
  // You do not need to do this more than one time.
  run_Init_State_initialize();

  // Invoke the entry-point functions.
  // You can call entry-point functions multiple times.
  main_run_Init_State();

  // Terminate the application.
  // You do not need to do this more than one time.
  run_Init_State_terminate();
  return 0;
}

//
// File trailer for main.cpp
//
// [EOF]
//
