//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: round.cpp
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 01-Feb-2017 19:25:15
//

// Include Files
#include "rt_nonfinite.h"
#include "run_Init_State.h"
#include "round.h"
#include "run_Init_State_rtwutil.h"

// Function Definitions

//
// Arguments    : emxArray_real32_T *x
// Return Type  : void
//
void b_round(emxArray_real32_T *x)
{
  int nx;
  int k;
  nx = x->size[1] << 1;
  for (k = 0; k + 1 <= nx; k++) {
    x->data[k] = rt_roundf_snf(x->data[k]);
  }
}

//
// File trailer for round.cpp
//
// [EOF]
//
