//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: run_Init_State_terminate.cpp
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 01-Feb-2017 19:25:15
//

// Include Files
#include "rt_nonfinite.h"
#include "run_Init_State.h"
#include "run_Init_State_terminate.h"
#include "run_Init_State_data.h"

// Function Definitions

//
// Arguments    : void
// Return Type  : void
//
void run_Init_State_terminate()
{
  omp_destroy_nest_lock(&emlrtNestLockGlobal);
}

//
// File trailer for run_Init_State_terminate.cpp
//
// [EOF]
//
