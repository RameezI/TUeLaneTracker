//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: createTemplate_initialize.cpp
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 21-Feb-2017 22:59:58
//

// Include Files
#include "rt_nonfinite.h"
#include "createTemplate.h"
#include "createTemplate_initialize.h"
#include "createTemplate_data.h"

// Function Definitions

//
// Arguments    : void
// Return Type  : void
//
void createTemplate_initialize()
{
  rt_InitInfAndNaN(8U);
  omp_init_nest_lock(&emlrtNestLockGlobal);
}

//
// File trailer for createTemplate_initialize.cpp
//
// [EOF]
//
