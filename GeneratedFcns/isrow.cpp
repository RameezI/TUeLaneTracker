//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: isrow.cpp
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 21-Feb-2017 22:59:58
//

// Include Files
#include "rt_nonfinite.h"
#include "createTemplate.h"
#include "isrow.h"

// Function Definitions

//
// Arguments    : const emxArray_real32_T *x
// Return Type  : boolean_T
//
boolean_T isrow(const emxArray_real32_T *x)
{
  return x->size[0] == 1;
}

//
// File trailer for isrow.cpp
//
// [EOF]
//
