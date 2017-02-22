//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: round.cpp
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 21-Feb-2017 22:59:58
//

// Include Files
#include "rt_nonfinite.h"
#include "createTemplate.h"
#include "round.h"

// Function Declarations
static float rt_roundf_snf(float u);

// Function Definitions

//
// Arguments    : float u
// Return Type  : float
//
static float rt_roundf_snf(float u)
{
  float y;
  if (std::abs(u) < 8.388608E+6F) {
    if (u >= 0.5F) {
      y = std::floor(u + 0.5F);
    } else if (u > -0.5F) {
      y = u * 0.0F;
    } else {
      y = std::ceil(u - 0.5F);
    }
  } else {
    y = u;
  }

  return y;
}

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
