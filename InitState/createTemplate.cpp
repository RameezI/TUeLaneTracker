//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: createTemplate.cpp
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 28-Jan-2017 01:24:11
//

// Include Files
#include "rt_nonfinite.h"
#include "run_Init_State.h"
#include "createTemplate.h"
#include "run_Init_State_emxutil.h"
#include "round.h"
#include "morphopAlgo.h"

// Function Definitions

//
// %% empty template
// Arguments    : float H
//                float W
//                emxArray_real_T *TEMPLATE
// Return Type  : void
//
void createTemplate(float H, float W, emxArray_real_T *TEMPLATE)
{
  float y;
  float an;
  int i2;
  int br;
  emxArray_real32_T *b_y;
  double ndbl;
  double apnd;
  double cdiff;
  double absa;
  emxArray_real32_T *pix;
  double absb;
  int n;
  int h;
  emxArray_real32_T *pixrot;
  emxArray_boolean_T *r0;
  emxArray_boolean_T *r1;
  int nm1d2;
  emxArray_int32_T *r2;
  emxArray_real32_T *varargin_2;
  emxArray_real32_T *b_pixrot;
  emxArray_real32_T *c_pixrot;
  emxArray_real32_T *d_pixrot;
  float cs;
  emxArray_real32_T *e_pixrot;
  float sn;
  emxArray_real32_T *f_pixrot;
  emxArray_real32_T *g_pixrot;
  float rot[4];
  unsigned int unnamed_idx_1;
  int ar;
  emxArray_real32_T *h_pixrot;
  emxArray_real32_T *i_pixrot;
  emxArray_real32_T *j_pixrot;
  int ic;
  int ib;
  int ia;
  emxArray_real32_T *k_pixrot;
  emxArray_real32_T *l_pixrot;
  emxArray_real32_T *m_pixrot;
  emxArray_real_T *B;

  //
  //
  //
  //  create edge template
  //
  //
  //
  // 'createTemplate:11' TEMPLATE = -180 * ones( 2*H+1, 2*W+1 );
  y = 2.0F * H;
  an = 2.0F * W;
  i2 = TEMPLATE->size[0] * TEMPLATE->size[1];
  TEMPLATE->size[0] = (int)(y + 1.0F);
  TEMPLATE->size[1] = (int)(an + 1.0F);
  emxEnsureCapacity((emxArray__common *)TEMPLATE, i2, (int)sizeof(double));
  br = (int)(y + 1.0F) * (int)(an + 1.0F);
  for (i2 = 0; i2 < br; i2++) {
    TEMPLATE->data[i2] = -180.0;
  }

  // 'createTemplate:12' pix      = [-2*W:2*W;zeros(1,4*W+1)];
  y = -2.0F * W;
  an = 2.0F * W;
  emxInit_real32_T(&b_y, 2);
  if (rtIsNaNF(y) || rtIsNaNF(an)) {
    i2 = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity((emxArray__common *)b_y, i2, (int)sizeof(float));
    b_y->data[0] = ((real32_T)rtNaN);
  } else if (an < y) {
    i2 = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 0;
    emxEnsureCapacity((emxArray__common *)b_y, i2, (int)sizeof(float));
  } else if ((rtIsInfF(y) || rtIsInfF(an)) && (y == an)) {
    i2 = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity((emxArray__common *)b_y, i2, (int)sizeof(float));
    b_y->data[0] = ((real32_T)rtNaN);
  } else if (std::floor(y) == y) {
    i2 = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = (int)std::floor(an - y) + 1;
    emxEnsureCapacity((emxArray__common *)b_y, i2, (int)sizeof(float));
    br = (int)std::floor(an - y);
    for (i2 = 0; i2 <= br; i2++) {
      b_y->data[b_y->size[0] * i2] = y + (float)i2;
    }
  } else {
    ndbl = std::floor(((double)an - y) + 0.5);
    apnd = y + ndbl;
    cdiff = apnd - an;
    absa = std::abs((double)y);
    absb = std::abs((double)an);
    if ((absa >= absb) || rtIsNaN(absb)) {
      absb = absa;
    }

    if (std::abs(cdiff) < 2.38418579E-7F * (float)absb) {
      ndbl++;
    } else if (cdiff > 0.0) {
      an = (float)(y + (ndbl - 1.0));
    } else {
      ndbl++;
      an = (float)apnd;
    }

    if (ndbl >= 0.0) {
      n = (int)ndbl;
    } else {
      n = 0;
    }

    i2 = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = n;
    emxEnsureCapacity((emxArray__common *)b_y, i2, (int)sizeof(float));
    if (n > 0) {
      b_y->data[0] = y;
      if (n > 1) {
        b_y->data[n - 1] = an;
        nm1d2 = (n - 1) / 2;
        for (br = 1; br < nm1d2; br++) {
          b_y->data[br] = y + (float)br;
          b_y->data[(n - br) - 1] = an - (float)br;
        }

        if (nm1d2 << 1 == n - 1) {
          b_y->data[nm1d2] = (y + an) / 2.0F;
        } else {
          b_y->data[nm1d2] = y + (float)nm1d2;
          b_y->data[nm1d2 + 1] = an - (float)nm1d2;
        }
      }
    }
  }

  emxInit_real32_T(&pix, 2);
  i2 = pix->size[0] * pix->size[1];
  pix->size[0] = 2;
  pix->size[1] = b_y->size[1];
  emxEnsureCapacity((emxArray__common *)pix, i2, (int)sizeof(float));
  br = b_y->size[1];
  for (i2 = 0; i2 < br; i2++) {
    pix->data[pix->size[0] * i2] = b_y->data[b_y->size[0] * i2];
  }

  br = (int)(4.0F * W + 1.0F);
  for (i2 = 0; i2 < br; i2++) {
    pix->data[1 + pix->size[0] * i2] = 0.0F;
  }

  //     %% left border
  // 'createTemplate:15' for h = H+1:2*H+1
  i2 = (int)((2.0F * H + 1.0F) + (1.0F - (H + 1.0F)));
  h = 0;
  emxInit_real32_T(&pixrot, 2);
  emxInit_boolean_T(&r0, 2);
  emxInit_boolean_T(&r1, 2);
  emxInit_int32_T(&r2, 2);
  emxInit_real32_T(&varargin_2, 2);
  emxInit_real32_T(&b_pixrot, 2);
  emxInit_real32_T(&c_pixrot, 2);
  emxInit_real32_T(&d_pixrot, 2);
  while (h <= i2 - 1) {
    //         %% angle
    // 'createTemplate:19' an = atan((h-(H+1)) / W );
    an = std::atan((((H + 1.0F) + (float)h) - (H + 1.0F)) / W);

    //         %% draw in template
    // 'createTemplate:22' cs     = cos(-an);
    cs = std::cos(-an);

    // 'createTemplate:23' sn     = sin(-an);
    sn = std::sin(-an);

    // 'createTemplate:24' rot    = [cs -sn; sn cs];
    rot[0] = cs;
    rot[2] = -sn;
    rot[1] = sn;
    rot[3] = cs;

    // 'createTemplate:25' pixrot = rot*pix;
    unnamed_idx_1 = (unsigned int)pix->size[1];
    ar = pixrot->size[0] * pixrot->size[1];
    pixrot->size[0] = 2;
    pixrot->size[1] = (int)unnamed_idx_1;
    pixrot->size[0] = 2;
    emxEnsureCapacity((emxArray__common *)pixrot, ar, (int)sizeof(float));
    br = pixrot->size[1];
    for (ar = 0; ar < br; ar++) {
      for (nm1d2 = 0; nm1d2 < 2; nm1d2++) {
        pixrot->data[nm1d2 + pixrot->size[0] * ar] = 0.0F;
      }
    }

    if (pix->size[1] != 0) {
      nm1d2 = (pix->size[1] - 1) << 1;
      for (n = 0; n <= nm1d2; n += 2) {
        for (ic = n + 1; ic <= n + 2; ic++) {
          pixrot->data[ic - 1] = 0.0F;
        }
      }

      br = 0;
      for (n = 0; n <= nm1d2; n += 2) {
        ar = -1;
        for (ib = br; ib + 1 <= br + 2; ib++) {
          if (pix->data[ib] != 0.0F) {
            ia = ar;
            for (ic = n; ic + 1 <= n + 2; ic++) {
              ia++;
              pixrot->data[ic] += pix->data[ib] * rot[ia];
            }
          }

          ar += 2;
        }

        br += 2;
      }
    }

    // 'createTemplate:26' pixrot(1,:) = pixrot(1,:) + W+1;
    nm1d2 = pixrot->size[1];
    ar = b_pixrot->size[0] * b_pixrot->size[1];
    b_pixrot->size[0] = 1;
    b_pixrot->size[1] = nm1d2;
    emxEnsureCapacity((emxArray__common *)b_pixrot, ar, (int)sizeof(float));
    for (ar = 0; ar < nm1d2; ar++) {
      b_pixrot->data[b_pixrot->size[0] * ar] = (pixrot->data[pixrot->size[0] *
        ar] + W) + 1.0F;
    }

    br = b_pixrot->size[1];
    for (ar = 0; ar < br; ar++) {
      pixrot->data[pixrot->size[0] * ar] = b_pixrot->data[b_pixrot->size[0] * ar];
    }

    // 'createTemplate:27' pixrot(2,:) = pixrot(2,:) + H+1;
    nm1d2 = pixrot->size[1];
    ar = c_pixrot->size[0] * c_pixrot->size[1];
    c_pixrot->size[0] = 1;
    c_pixrot->size[1] = nm1d2;
    emxEnsureCapacity((emxArray__common *)c_pixrot, ar, (int)sizeof(float));
    for (ar = 0; ar < nm1d2; ar++) {
      c_pixrot->data[c_pixrot->size[0] * ar] = (pixrot->data[1 + pixrot->size[0]
        * ar] + H) + 1.0F;
    }

    br = c_pixrot->size[1];
    for (ar = 0; ar < br; ar++) {
      pixrot->data[1 + pixrot->size[0] * ar] = c_pixrot->data[c_pixrot->size[0] *
        ar];
    }

    // 'createTemplate:28' pixrot      = pixrot(:, (1 <= pixrot(1,:)) & (pixrot(1,:) <= 2*W+1) & (1 <= pixrot(2,:)) & (pixrot(2,:) <= 2*H+1) ); 
    y = 2.0F * W;
    br = pixrot->size[1];
    ar = r0->size[0] * r0->size[1];
    r0->size[0] = 1;
    r0->size[1] = br;
    emxEnsureCapacity((emxArray__common *)r0, ar, (int)sizeof(boolean_T));
    for (ar = 0; ar < br; ar++) {
      r0->data[r0->size[0] * ar] = ((1.0F <= pixrot->data[pixrot->size[0] * ar])
        && (pixrot->data[pixrot->size[0] * ar] <= y + 1.0F) && (1.0F <=
        pixrot->data[1 + pixrot->size[0] * ar]));
    }

    y = 2.0F * H;
    br = pixrot->size[1];
    ar = r1->size[0] * r1->size[1];
    r1->size[0] = 1;
    r1->size[1] = br;
    emxEnsureCapacity((emxArray__common *)r1, ar, (int)sizeof(boolean_T));
    for (ar = 0; ar < br; ar++) {
      r1->data[r1->size[0] * ar] = (pixrot->data[1 + pixrot->size[0] * ar] <= y
        + 1.0F);
    }

    n = r0->size[1] - 1;
    nm1d2 = 0;
    for (br = 0; br <= n; br++) {
      if (r0->data[br] && r1->data[br]) {
        nm1d2++;
      }
    }

    ar = r2->size[0] * r2->size[1];
    r2->size[0] = 1;
    r2->size[1] = nm1d2;
    emxEnsureCapacity((emxArray__common *)r2, ar, (int)sizeof(int));
    nm1d2 = 0;
    for (br = 0; br <= n; br++) {
      if (r0->data[br] && r1->data[br]) {
        r2->data[nm1d2] = br + 1;
        nm1d2++;
      }
    }

    ar = d_pixrot->size[0] * d_pixrot->size[1];
    d_pixrot->size[0] = 2;
    d_pixrot->size[1] = r2->size[1];
    emxEnsureCapacity((emxArray__common *)d_pixrot, ar, (int)sizeof(float));
    br = r2->size[1];
    for (ar = 0; ar < br; ar++) {
      for (nm1d2 = 0; nm1d2 < 2; nm1d2++) {
        d_pixrot->data[nm1d2 + d_pixrot->size[0] * ar] = pixrot->data[nm1d2 +
          pixrot->size[0] * (r2->data[r2->size[0] * ar] - 1)];
      }
    }

    ar = pixrot->size[0] * pixrot->size[1];
    pixrot->size[0] = 2;
    pixrot->size[1] = d_pixrot->size[1];
    emxEnsureCapacity((emxArray__common *)pixrot, ar, (int)sizeof(float));
    br = d_pixrot->size[1];
    for (ar = 0; ar < br; ar++) {
      for (nm1d2 = 0; nm1d2 < 2; nm1d2++) {
        pixrot->data[nm1d2 + pixrot->size[0] * ar] = d_pixrot->data[nm1d2 +
          d_pixrot->size[0] * ar];
      }
    }

    // 'createTemplate:29' pixrot      = round(pixrot);
    b_round(pixrot);

    //         %% get indices of the vanashing line
    // 'createTemplate:32' index = sub2ind([2*H+1, 2*W+1], pixrot(2,:), pixrot(1,:) ); 
    br = pixrot->size[1];
    ar = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = br;
    emxEnsureCapacity((emxArray__common *)b_y, ar, (int)sizeof(float));
    for (ar = 0; ar < br; ar++) {
      b_y->data[b_y->size[0] * ar] = pixrot->data[1 + pixrot->size[0] * ar];
    }

    br = pixrot->size[1];
    ar = varargin_2->size[0] * varargin_2->size[1];
    varargin_2->size[0] = 1;
    varargin_2->size[1] = br;
    emxEnsureCapacity((emxArray__common *)varargin_2, ar, (int)sizeof(float));
    for (ar = 0; ar < br; ar++) {
      varargin_2->data[varargin_2->size[0] * ar] = pixrot->data[pixrot->size[0] *
        ar];
    }

    nm1d2 = (int)(2.0F * H + 1.0F);

    //         %% draw
    // 'createTemplate:35' TEMPLATE(index) = rad2deg(an);
    ar = r2->size[0] * r2->size[1];
    r2->size[0] = 1;
    r2->size[1] = b_y->size[1];
    emxEnsureCapacity((emxArray__common *)r2, ar, (int)sizeof(int));
    br = b_y->size[0] * b_y->size[1];
    for (ar = 0; ar < br; ar++) {
      r2->data[ar] = (int)b_y->data[ar] + nm1d2 * ((int)varargin_2->data[ar] - 1);
    }

    an *= 57.2957802F;
    br = r2->size[0] * r2->size[1];
    for (ar = 0; ar < br; ar++) {
      TEMPLATE->data[r2->data[ar] - 1] = an;
    }

    h++;
  }

  emxFree_real32_T(&d_pixrot);
  emxFree_real32_T(&c_pixrot);
  emxFree_real32_T(&b_pixrot);

  //     %% bottom border left part
  // 'createTemplate:39' for w = W:-1:0
  i2 = (int)((-1.0F - W) / -1.0F);
  h = 0;
  emxInit_real32_T(&e_pixrot, 2);
  emxInit_real32_T(&f_pixrot, 2);
  emxInit_real32_T(&g_pixrot, 2);
  while (h <= i2 - 1) {
    //         %% angle
    // 'createTemplate:42' an = pi/2-atan( w / H );
    y = (W + -(float)h) / H;
    an = 1.57079637F - std::atan(y);

    //         %% draw in template
    // 'createTemplate:45' cs     = cos(-an);
    cs = std::cos(-an);

    // 'createTemplate:46' sn     = sin(-an);
    sn = std::sin(-an);

    // 'createTemplate:47' rot    = [cs -sn; sn cs];
    rot[0] = cs;
    rot[2] = -sn;
    rot[1] = sn;
    rot[3] = cs;

    // 'createTemplate:48' pixrot = rot*pix;
    unnamed_idx_1 = (unsigned int)pix->size[1];
    ar = pixrot->size[0] * pixrot->size[1];
    pixrot->size[0] = 2;
    pixrot->size[1] = (int)unnamed_idx_1;
    pixrot->size[0] = 2;
    emxEnsureCapacity((emxArray__common *)pixrot, ar, (int)sizeof(float));
    br = pixrot->size[1];
    for (ar = 0; ar < br; ar++) {
      for (nm1d2 = 0; nm1d2 < 2; nm1d2++) {
        pixrot->data[nm1d2 + pixrot->size[0] * ar] = 0.0F;
      }
    }

    if (pix->size[1] != 0) {
      nm1d2 = (pix->size[1] - 1) << 1;
      for (n = 0; n <= nm1d2; n += 2) {
        for (ic = n + 1; ic <= n + 2; ic++) {
          pixrot->data[ic - 1] = 0.0F;
        }
      }

      br = 0;
      for (n = 0; n <= nm1d2; n += 2) {
        ar = -1;
        for (ib = br; ib + 1 <= br + 2; ib++) {
          if (pix->data[ib] != 0.0F) {
            ia = ar;
            for (ic = n; ic + 1 <= n + 2; ic++) {
              ia++;
              pixrot->data[ic] += pix->data[ib] * rot[ia];
            }
          }

          ar += 2;
        }

        br += 2;
      }
    }

    // 'createTemplate:49' pixrot(1,:) = pixrot(1,:) + W+1;
    nm1d2 = pixrot->size[1];
    ar = e_pixrot->size[0] * e_pixrot->size[1];
    e_pixrot->size[0] = 1;
    e_pixrot->size[1] = nm1d2;
    emxEnsureCapacity((emxArray__common *)e_pixrot, ar, (int)sizeof(float));
    for (ar = 0; ar < nm1d2; ar++) {
      e_pixrot->data[e_pixrot->size[0] * ar] = (pixrot->data[pixrot->size[0] *
        ar] + W) + 1.0F;
    }

    br = e_pixrot->size[1];
    for (ar = 0; ar < br; ar++) {
      pixrot->data[pixrot->size[0] * ar] = e_pixrot->data[e_pixrot->size[0] * ar];
    }

    // 'createTemplate:50' pixrot(2,:) = pixrot(2,:) + H+1;
    nm1d2 = pixrot->size[1];
    ar = f_pixrot->size[0] * f_pixrot->size[1];
    f_pixrot->size[0] = 1;
    f_pixrot->size[1] = nm1d2;
    emxEnsureCapacity((emxArray__common *)f_pixrot, ar, (int)sizeof(float));
    for (ar = 0; ar < nm1d2; ar++) {
      f_pixrot->data[f_pixrot->size[0] * ar] = (pixrot->data[1 + pixrot->size[0]
        * ar] + H) + 1.0F;
    }

    br = f_pixrot->size[1];
    for (ar = 0; ar < br; ar++) {
      pixrot->data[1 + pixrot->size[0] * ar] = f_pixrot->data[f_pixrot->size[0] *
        ar];
    }

    // 'createTemplate:51' pixrot      = pixrot(:, (1 <= pixrot(1,:)) & (pixrot(1,:) <= 2*W+1) & (1 <= pixrot(2,:)) & (pixrot(2,:) <= 2*H+1) ); 
    an = 2.0F * W;
    br = pixrot->size[1];
    ar = r0->size[0] * r0->size[1];
    r0->size[0] = 1;
    r0->size[1] = br;
    emxEnsureCapacity((emxArray__common *)r0, ar, (int)sizeof(boolean_T));
    for (ar = 0; ar < br; ar++) {
      r0->data[r0->size[0] * ar] = ((1.0F <= pixrot->data[pixrot->size[0] * ar])
        && (pixrot->data[pixrot->size[0] * ar] <= an + 1.0F) && (1.0F <=
        pixrot->data[1 + pixrot->size[0] * ar]));
    }

    an = 2.0F * H;
    br = pixrot->size[1];
    ar = r1->size[0] * r1->size[1];
    r1->size[0] = 1;
    r1->size[1] = br;
    emxEnsureCapacity((emxArray__common *)r1, ar, (int)sizeof(boolean_T));
    for (ar = 0; ar < br; ar++) {
      r1->data[r1->size[0] * ar] = (pixrot->data[1 + pixrot->size[0] * ar] <= an
        + 1.0F);
    }

    n = r0->size[1] - 1;
    nm1d2 = 0;
    for (br = 0; br <= n; br++) {
      if (r0->data[br] && r1->data[br]) {
        nm1d2++;
      }
    }

    ar = r2->size[0] * r2->size[1];
    r2->size[0] = 1;
    r2->size[1] = nm1d2;
    emxEnsureCapacity((emxArray__common *)r2, ar, (int)sizeof(int));
    nm1d2 = 0;
    for (br = 0; br <= n; br++) {
      if (r0->data[br] && r1->data[br]) {
        r2->data[nm1d2] = br + 1;
        nm1d2++;
      }
    }

    ar = g_pixrot->size[0] * g_pixrot->size[1];
    g_pixrot->size[0] = 2;
    g_pixrot->size[1] = r2->size[1];
    emxEnsureCapacity((emxArray__common *)g_pixrot, ar, (int)sizeof(float));
    br = r2->size[1];
    for (ar = 0; ar < br; ar++) {
      for (nm1d2 = 0; nm1d2 < 2; nm1d2++) {
        g_pixrot->data[nm1d2 + g_pixrot->size[0] * ar] = pixrot->data[nm1d2 +
          pixrot->size[0] * (r2->data[r2->size[0] * ar] - 1)];
      }
    }

    ar = pixrot->size[0] * pixrot->size[1];
    pixrot->size[0] = 2;
    pixrot->size[1] = g_pixrot->size[1];
    emxEnsureCapacity((emxArray__common *)pixrot, ar, (int)sizeof(float));
    br = g_pixrot->size[1];
    for (ar = 0; ar < br; ar++) {
      for (nm1d2 = 0; nm1d2 < 2; nm1d2++) {
        pixrot->data[nm1d2 + pixrot->size[0] * ar] = g_pixrot->data[nm1d2 +
          g_pixrot->size[0] * ar];
      }
    }

    // 'createTemplate:52' pixrot      = round(pixrot);
    b_round(pixrot);

    //         %% get indices of the vanashing line
    // 'createTemplate:55' index = sub2ind([2*H+1, 2*W+1], pixrot(2,:), pixrot(1,:) ); 
    br = pixrot->size[1];
    ar = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = br;
    emxEnsureCapacity((emxArray__common *)b_y, ar, (int)sizeof(float));
    for (ar = 0; ar < br; ar++) {
      b_y->data[b_y->size[0] * ar] = pixrot->data[1 + pixrot->size[0] * ar];
    }

    br = pixrot->size[1];
    ar = varargin_2->size[0] * varargin_2->size[1];
    varargin_2->size[0] = 1;
    varargin_2->size[1] = br;
    emxEnsureCapacity((emxArray__common *)varargin_2, ar, (int)sizeof(float));
    for (ar = 0; ar < br; ar++) {
      varargin_2->data[varargin_2->size[0] * ar] = pixrot->data[pixrot->size[0] *
        ar];
    }

    nm1d2 = (int)(2.0F * H + 1.0F);

    //         %% draw
    // 'createTemplate:58' TEMPLATE(index) = rad2deg(an);
    ar = r2->size[0] * r2->size[1];
    r2->size[0] = 1;
    r2->size[1] = b_y->size[1];
    emxEnsureCapacity((emxArray__common *)r2, ar, (int)sizeof(int));
    br = b_y->size[0] * b_y->size[1];
    for (ar = 0; ar < br; ar++) {
      r2->data[ar] = (int)b_y->data[ar] + nm1d2 * ((int)varargin_2->data[ar] - 1);
    }

    an = 57.2957802F * (1.57079637F - std::atan(y));
    br = r2->size[0] * r2->size[1];
    for (ar = 0; ar < br; ar++) {
      TEMPLATE->data[r2->data[ar] - 1] = an;
    }

    h++;
  }

  emxFree_real32_T(&g_pixrot);
  emxFree_real32_T(&f_pixrot);
  emxFree_real32_T(&e_pixrot);

  //     %% bottom border rithg part
  // 'createTemplate:62' for w = W:-1:0
  i2 = (int)((-1.0F - W) / -1.0F);
  h = 0;
  emxInit_real32_T(&h_pixrot, 2);
  emxInit_real32_T(&i_pixrot, 2);
  emxInit_real32_T(&j_pixrot, 2);
  while (h <= i2 - 1) {
    //         %% angle
    // 'createTemplate:65' an = pi/2+atan( w / H );
    y = (W + -(float)h) / H;
    an = 1.57079637F + std::atan(y);

    //         %% draw in template
    // 'createTemplate:68' cs     = cos(-an);
    cs = std::cos(-an);

    // 'createTemplate:69' sn     = sin(-an);
    sn = std::sin(-an);

    // 'createTemplate:70' rot    = [cs -sn; sn cs];
    rot[0] = cs;
    rot[2] = -sn;
    rot[1] = sn;
    rot[3] = cs;

    // 'createTemplate:71' pixrot = rot*pix;
    unnamed_idx_1 = (unsigned int)pix->size[1];
    ar = pixrot->size[0] * pixrot->size[1];
    pixrot->size[0] = 2;
    pixrot->size[1] = (int)unnamed_idx_1;
    pixrot->size[0] = 2;
    emxEnsureCapacity((emxArray__common *)pixrot, ar, (int)sizeof(float));
    br = pixrot->size[1];
    for (ar = 0; ar < br; ar++) {
      for (nm1d2 = 0; nm1d2 < 2; nm1d2++) {
        pixrot->data[nm1d2 + pixrot->size[0] * ar] = 0.0F;
      }
    }

    if (pix->size[1] != 0) {
      nm1d2 = (pix->size[1] - 1) << 1;
      for (n = 0; n <= nm1d2; n += 2) {
        for (ic = n + 1; ic <= n + 2; ic++) {
          pixrot->data[ic - 1] = 0.0F;
        }
      }

      br = 0;
      for (n = 0; n <= nm1d2; n += 2) {
        ar = -1;
        for (ib = br; ib + 1 <= br + 2; ib++) {
          if (pix->data[ib] != 0.0F) {
            ia = ar;
            for (ic = n; ic + 1 <= n + 2; ic++) {
              ia++;
              pixrot->data[ic] += pix->data[ib] * rot[ia];
            }
          }

          ar += 2;
        }

        br += 2;
      }
    }

    // 'createTemplate:72' pixrot(1,:) = pixrot(1,:) + W+1;
    nm1d2 = pixrot->size[1];
    ar = h_pixrot->size[0] * h_pixrot->size[1];
    h_pixrot->size[0] = 1;
    h_pixrot->size[1] = nm1d2;
    emxEnsureCapacity((emxArray__common *)h_pixrot, ar, (int)sizeof(float));
    for (ar = 0; ar < nm1d2; ar++) {
      h_pixrot->data[h_pixrot->size[0] * ar] = (pixrot->data[pixrot->size[0] *
        ar] + W) + 1.0F;
    }

    br = h_pixrot->size[1];
    for (ar = 0; ar < br; ar++) {
      pixrot->data[pixrot->size[0] * ar] = h_pixrot->data[h_pixrot->size[0] * ar];
    }

    // 'createTemplate:73' pixrot(2,:) = pixrot(2,:) + H+1;
    nm1d2 = pixrot->size[1];
    ar = i_pixrot->size[0] * i_pixrot->size[1];
    i_pixrot->size[0] = 1;
    i_pixrot->size[1] = nm1d2;
    emxEnsureCapacity((emxArray__common *)i_pixrot, ar, (int)sizeof(float));
    for (ar = 0; ar < nm1d2; ar++) {
      i_pixrot->data[i_pixrot->size[0] * ar] = (pixrot->data[1 + pixrot->size[0]
        * ar] + H) + 1.0F;
    }

    br = i_pixrot->size[1];
    for (ar = 0; ar < br; ar++) {
      pixrot->data[1 + pixrot->size[0] * ar] = i_pixrot->data[i_pixrot->size[0] *
        ar];
    }

    // 'createTemplate:74' pixrot      = pixrot(:, (1 <= pixrot(1,:)) & (pixrot(1,:) <= 2*W+1) & (1 <= pixrot(2,:)) & (pixrot(2,:) <= 2*H+1) ); 
    an = 2.0F * W;
    br = pixrot->size[1];
    ar = r0->size[0] * r0->size[1];
    r0->size[0] = 1;
    r0->size[1] = br;
    emxEnsureCapacity((emxArray__common *)r0, ar, (int)sizeof(boolean_T));
    for (ar = 0; ar < br; ar++) {
      r0->data[r0->size[0] * ar] = ((1.0F <= pixrot->data[pixrot->size[0] * ar])
        && (pixrot->data[pixrot->size[0] * ar] <= an + 1.0F) && (1.0F <=
        pixrot->data[1 + pixrot->size[0] * ar]));
    }

    an = 2.0F * H;
    br = pixrot->size[1];
    ar = r1->size[0] * r1->size[1];
    r1->size[0] = 1;
    r1->size[1] = br;
    emxEnsureCapacity((emxArray__common *)r1, ar, (int)sizeof(boolean_T));
    for (ar = 0; ar < br; ar++) {
      r1->data[r1->size[0] * ar] = (pixrot->data[1 + pixrot->size[0] * ar] <= an
        + 1.0F);
    }

    n = r0->size[1] - 1;
    nm1d2 = 0;
    for (br = 0; br <= n; br++) {
      if (r0->data[br] && r1->data[br]) {
        nm1d2++;
      }
    }

    ar = r2->size[0] * r2->size[1];
    r2->size[0] = 1;
    r2->size[1] = nm1d2;
    emxEnsureCapacity((emxArray__common *)r2, ar, (int)sizeof(int));
    nm1d2 = 0;
    for (br = 0; br <= n; br++) {
      if (r0->data[br] && r1->data[br]) {
        r2->data[nm1d2] = br + 1;
        nm1d2++;
      }
    }

    ar = j_pixrot->size[0] * j_pixrot->size[1];
    j_pixrot->size[0] = 2;
    j_pixrot->size[1] = r2->size[1];
    emxEnsureCapacity((emxArray__common *)j_pixrot, ar, (int)sizeof(float));
    br = r2->size[1];
    for (ar = 0; ar < br; ar++) {
      for (nm1d2 = 0; nm1d2 < 2; nm1d2++) {
        j_pixrot->data[nm1d2 + j_pixrot->size[0] * ar] = pixrot->data[nm1d2 +
          pixrot->size[0] * (r2->data[r2->size[0] * ar] - 1)];
      }
    }

    ar = pixrot->size[0] * pixrot->size[1];
    pixrot->size[0] = 2;
    pixrot->size[1] = j_pixrot->size[1];
    emxEnsureCapacity((emxArray__common *)pixrot, ar, (int)sizeof(float));
    br = j_pixrot->size[1];
    for (ar = 0; ar < br; ar++) {
      for (nm1d2 = 0; nm1d2 < 2; nm1d2++) {
        pixrot->data[nm1d2 + pixrot->size[0] * ar] = j_pixrot->data[nm1d2 +
          j_pixrot->size[0] * ar];
      }
    }

    // 'createTemplate:75' pixrot      = round(pixrot);
    b_round(pixrot);

    //         %% get indices of the vanashing line
    // 'createTemplate:78' index = sub2ind([2*H+1, 2*W+1], pixrot(2,:), pixrot(1,:) ); 
    br = pixrot->size[1];
    ar = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = br;
    emxEnsureCapacity((emxArray__common *)b_y, ar, (int)sizeof(float));
    for (ar = 0; ar < br; ar++) {
      b_y->data[b_y->size[0] * ar] = pixrot->data[1 + pixrot->size[0] * ar];
    }

    br = pixrot->size[1];
    ar = varargin_2->size[0] * varargin_2->size[1];
    varargin_2->size[0] = 1;
    varargin_2->size[1] = br;
    emxEnsureCapacity((emxArray__common *)varargin_2, ar, (int)sizeof(float));
    for (ar = 0; ar < br; ar++) {
      varargin_2->data[varargin_2->size[0] * ar] = pixrot->data[pixrot->size[0] *
        ar];
    }

    nm1d2 = (int)(2.0F * H + 1.0F);

    //         %% draw
    // 'createTemplate:81' TEMPLATE(index) = rad2deg(an);
    ar = r2->size[0] * r2->size[1];
    r2->size[0] = 1;
    r2->size[1] = b_y->size[1];
    emxEnsureCapacity((emxArray__common *)r2, ar, (int)sizeof(int));
    br = b_y->size[0] * b_y->size[1];
    for (ar = 0; ar < br; ar++) {
      r2->data[ar] = (int)b_y->data[ar] + nm1d2 * ((int)varargin_2->data[ar] - 1);
    }

    an = 57.2957802F * (1.57079637F + std::atan(y));
    br = r2->size[0] * r2->size[1];
    for (ar = 0; ar < br; ar++) {
      TEMPLATE->data[r2->data[ar] - 1] = an;
    }

    h++;
  }

  emxFree_real32_T(&j_pixrot);
  emxFree_real32_T(&i_pixrot);
  emxFree_real32_T(&h_pixrot);

  //     %% right border
  // 'createTemplate:85' pix = [-2*W:2*W;zeros(1,4*W+1)];
  y = -2.0F * W;
  an = 2.0F * W;
  if (rtIsNaNF(y) || rtIsNaNF(an)) {
    i2 = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity((emxArray__common *)b_y, i2, (int)sizeof(float));
    b_y->data[0] = ((real32_T)rtNaN);
  } else if (an < y) {
    i2 = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 0;
    emxEnsureCapacity((emxArray__common *)b_y, i2, (int)sizeof(float));
  } else if ((rtIsInfF(y) || rtIsInfF(an)) && (y == an)) {
    i2 = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity((emxArray__common *)b_y, i2, (int)sizeof(float));
    b_y->data[0] = ((real32_T)rtNaN);
  } else if (std::floor(y) == y) {
    i2 = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = (int)std::floor(an - y) + 1;
    emxEnsureCapacity((emxArray__common *)b_y, i2, (int)sizeof(float));
    br = (int)std::floor(an - y);
    for (i2 = 0; i2 <= br; i2++) {
      b_y->data[b_y->size[0] * i2] = y + (float)i2;
    }
  } else {
    ndbl = std::floor(((double)an - y) + 0.5);
    apnd = y + ndbl;
    cdiff = apnd - an;
    absa = std::abs((double)y);
    absb = std::abs((double)an);
    if ((absa >= absb) || rtIsNaN(absb)) {
      absb = absa;
    }

    if (std::abs(cdiff) < 2.38418579E-7F * (float)absb) {
      ndbl++;
    } else if (cdiff > 0.0) {
      an = (float)(y + (ndbl - 1.0));
    } else {
      ndbl++;
      an = (float)apnd;
    }

    if (ndbl >= 0.0) {
      n = (int)ndbl;
    } else {
      n = 0;
    }

    i2 = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = n;
    emxEnsureCapacity((emxArray__common *)b_y, i2, (int)sizeof(float));
    if (n > 0) {
      b_y->data[0] = y;
      if (n > 1) {
        b_y->data[n - 1] = an;
        nm1d2 = (n - 1) / 2;
        for (br = 1; br < nm1d2; br++) {
          b_y->data[br] = y + (float)br;
          b_y->data[(n - br) - 1] = an - (float)br;
        }

        if (nm1d2 << 1 == n - 1) {
          b_y->data[nm1d2] = (y + an) / 2.0F;
        } else {
          b_y->data[nm1d2] = y + (float)nm1d2;
          b_y->data[nm1d2 + 1] = an - (float)nm1d2;
        }
      }
    }
  }

  i2 = pix->size[0] * pix->size[1];
  pix->size[0] = 2;
  pix->size[1] = b_y->size[1];
  emxEnsureCapacity((emxArray__common *)pix, i2, (int)sizeof(float));
  br = b_y->size[1];
  for (i2 = 0; i2 < br; i2++) {
    pix->data[pix->size[0] * i2] = b_y->data[b_y->size[0] * i2];
  }

  br = (int)(4.0F * W + 1.0F);
  for (i2 = 0; i2 < br; i2++) {
    pix->data[1 + pix->size[0] * i2] = 0.0F;
  }

  // 'createTemplate:86' for h = H+1:2*H+1
  i2 = (int)((2.0F * H + 1.0F) + (1.0F - (H + 1.0F)));
  h = 0;
  emxInit_real32_T(&k_pixrot, 2);
  emxInit_real32_T(&l_pixrot, 2);
  emxInit_real32_T(&m_pixrot, 2);
  while (h <= i2 - 1) {
    //         %% angle
    // 'createTemplate:89' an = pi-atan( (h-(H+1)) / W );
    y = (((H + 1.0F) + (float)h) - (H + 1.0F)) / W;
    an = 3.14159274F - std::atan(y);

    //         %% draw in template
    // 'createTemplate:92' cs     = cos(-an);
    cs = std::cos(-an);

    // 'createTemplate:93' sn     = sin(-an);
    sn = std::sin(-an);

    // 'createTemplate:94' rot    = [cs -sn; sn cs];
    rot[0] = cs;
    rot[2] = -sn;
    rot[1] = sn;
    rot[3] = cs;

    // 'createTemplate:95' pixrot = rot*pix;
    unnamed_idx_1 = (unsigned int)pix->size[1];
    ar = pixrot->size[0] * pixrot->size[1];
    pixrot->size[0] = 2;
    pixrot->size[1] = (int)unnamed_idx_1;
    pixrot->size[0] = 2;
    emxEnsureCapacity((emxArray__common *)pixrot, ar, (int)sizeof(float));
    br = pixrot->size[1];
    for (ar = 0; ar < br; ar++) {
      for (nm1d2 = 0; nm1d2 < 2; nm1d2++) {
        pixrot->data[nm1d2 + pixrot->size[0] * ar] = 0.0F;
      }
    }

    if (pix->size[1] != 0) {
      nm1d2 = (pix->size[1] - 1) << 1;
      for (n = 0; n <= nm1d2; n += 2) {
        for (ic = n + 1; ic <= n + 2; ic++) {
          pixrot->data[ic - 1] = 0.0F;
        }
      }

      br = 0;
      for (n = 0; n <= nm1d2; n += 2) {
        ar = -1;
        for (ib = br; ib + 1 <= br + 2; ib++) {
          if (pix->data[ib] != 0.0F) {
            ia = ar;
            for (ic = n; ic + 1 <= n + 2; ic++) {
              ia++;
              pixrot->data[ic] += pix->data[ib] * rot[ia];
            }
          }

          ar += 2;
        }

        br += 2;
      }
    }

    // 'createTemplate:96' pixrot(1,:) = pixrot(1,:) + W+1;
    nm1d2 = pixrot->size[1];
    ar = k_pixrot->size[0] * k_pixrot->size[1];
    k_pixrot->size[0] = 1;
    k_pixrot->size[1] = nm1d2;
    emxEnsureCapacity((emxArray__common *)k_pixrot, ar, (int)sizeof(float));
    for (ar = 0; ar < nm1d2; ar++) {
      k_pixrot->data[k_pixrot->size[0] * ar] = (pixrot->data[pixrot->size[0] *
        ar] + W) + 1.0F;
    }

    br = k_pixrot->size[1];
    for (ar = 0; ar < br; ar++) {
      pixrot->data[pixrot->size[0] * ar] = k_pixrot->data[k_pixrot->size[0] * ar];
    }

    // 'createTemplate:97' pixrot(2,:) = pixrot(2,:) + H+1;
    nm1d2 = pixrot->size[1];
    ar = l_pixrot->size[0] * l_pixrot->size[1];
    l_pixrot->size[0] = 1;
    l_pixrot->size[1] = nm1d2;
    emxEnsureCapacity((emxArray__common *)l_pixrot, ar, (int)sizeof(float));
    for (ar = 0; ar < nm1d2; ar++) {
      l_pixrot->data[l_pixrot->size[0] * ar] = (pixrot->data[1 + pixrot->size[0]
        * ar] + H) + 1.0F;
    }

    br = l_pixrot->size[1];
    for (ar = 0; ar < br; ar++) {
      pixrot->data[1 + pixrot->size[0] * ar] = l_pixrot->data[l_pixrot->size[0] *
        ar];
    }

    // 'createTemplate:98' pixrot      = pixrot(:, (1 <= pixrot(1,:)) & (pixrot(1,:) <= 2*W+1) & (1 <= pixrot(2,:)) & (pixrot(2,:) <= 2*H+1) ); 
    an = 2.0F * W;
    br = pixrot->size[1];
    ar = r0->size[0] * r0->size[1];
    r0->size[0] = 1;
    r0->size[1] = br;
    emxEnsureCapacity((emxArray__common *)r0, ar, (int)sizeof(boolean_T));
    for (ar = 0; ar < br; ar++) {
      r0->data[r0->size[0] * ar] = ((1.0F <= pixrot->data[pixrot->size[0] * ar])
        && (pixrot->data[pixrot->size[0] * ar] <= an + 1.0F) && (1.0F <=
        pixrot->data[1 + pixrot->size[0] * ar]));
    }

    an = 2.0F * H;
    br = pixrot->size[1];
    ar = r1->size[0] * r1->size[1];
    r1->size[0] = 1;
    r1->size[1] = br;
    emxEnsureCapacity((emxArray__common *)r1, ar, (int)sizeof(boolean_T));
    for (ar = 0; ar < br; ar++) {
      r1->data[r1->size[0] * ar] = (pixrot->data[1 + pixrot->size[0] * ar] <= an
        + 1.0F);
    }

    n = r0->size[1] - 1;
    nm1d2 = 0;
    for (br = 0; br <= n; br++) {
      if (r0->data[br] && r1->data[br]) {
        nm1d2++;
      }
    }

    ar = r2->size[0] * r2->size[1];
    r2->size[0] = 1;
    r2->size[1] = nm1d2;
    emxEnsureCapacity((emxArray__common *)r2, ar, (int)sizeof(int));
    nm1d2 = 0;
    for (br = 0; br <= n; br++) {
      if (r0->data[br] && r1->data[br]) {
        r2->data[nm1d2] = br + 1;
        nm1d2++;
      }
    }

    ar = m_pixrot->size[0] * m_pixrot->size[1];
    m_pixrot->size[0] = 2;
    m_pixrot->size[1] = r2->size[1];
    emxEnsureCapacity((emxArray__common *)m_pixrot, ar, (int)sizeof(float));
    br = r2->size[1];
    for (ar = 0; ar < br; ar++) {
      for (nm1d2 = 0; nm1d2 < 2; nm1d2++) {
        m_pixrot->data[nm1d2 + m_pixrot->size[0] * ar] = pixrot->data[nm1d2 +
          pixrot->size[0] * (r2->data[r2->size[0] * ar] - 1)];
      }
    }

    ar = pixrot->size[0] * pixrot->size[1];
    pixrot->size[0] = 2;
    pixrot->size[1] = m_pixrot->size[1];
    emxEnsureCapacity((emxArray__common *)pixrot, ar, (int)sizeof(float));
    br = m_pixrot->size[1];
    for (ar = 0; ar < br; ar++) {
      for (nm1d2 = 0; nm1d2 < 2; nm1d2++) {
        pixrot->data[nm1d2 + pixrot->size[0] * ar] = m_pixrot->data[nm1d2 +
          m_pixrot->size[0] * ar];
      }
    }

    // 'createTemplate:99' pixrot      = round(pixrot);
    b_round(pixrot);

    //         %% get indices of the vanashing line
    // 'createTemplate:102' index = sub2ind([2*H+1, 2*W+1], pixrot(2,:), pixrot(1,:) ); 
    br = pixrot->size[1];
    ar = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = br;
    emxEnsureCapacity((emxArray__common *)b_y, ar, (int)sizeof(float));
    for (ar = 0; ar < br; ar++) {
      b_y->data[b_y->size[0] * ar] = pixrot->data[1 + pixrot->size[0] * ar];
    }

    br = pixrot->size[1];
    ar = varargin_2->size[0] * varargin_2->size[1];
    varargin_2->size[0] = 1;
    varargin_2->size[1] = br;
    emxEnsureCapacity((emxArray__common *)varargin_2, ar, (int)sizeof(float));
    for (ar = 0; ar < br; ar++) {
      varargin_2->data[varargin_2->size[0] * ar] = pixrot->data[pixrot->size[0] *
        ar];
    }

    nm1d2 = (int)(2.0F * H + 1.0F);

    //         %% draw
    // 'createTemplate:105' TEMPLATE(index) = rad2deg(an);
    ar = r2->size[0] * r2->size[1];
    r2->size[0] = 1;
    r2->size[1] = b_y->size[1];
    emxEnsureCapacity((emxArray__common *)r2, ar, (int)sizeof(int));
    br = b_y->size[0] * b_y->size[1];
    for (ar = 0; ar < br; ar++) {
      r2->data[ar] = (int)b_y->data[ar] + nm1d2 * ((int)varargin_2->data[ar] - 1);
    }

    an = 57.2957802F * (3.14159274F - std::atan(y));
    br = r2->size[0] * r2->size[1];
    for (ar = 0; ar < br; ar++) {
      TEMPLATE->data[r2->data[ar] - 1] = an;
    }

    h++;
  }

  emxFree_real32_T(&m_pixrot);
  emxFree_real32_T(&l_pixrot);
  emxFree_real32_T(&k_pixrot);
  emxFree_real32_T(&varargin_2);
  emxFree_real32_T(&b_y);
  emxFree_int32_T(&r2);
  emxFree_boolean_T(&r1);
  emxFree_boolean_T(&r0);
  emxFree_real32_T(&pixrot);
  emxFree_real32_T(&pix);
  emxInit_real_T(&B, 2);

  //     %% fill in holes
  // 'createTemplate:109' TEMPLATE = imdilate( TEMPLATE, ones(3,3) );
  for (i2 = 0; i2 < 2; i2++) {
    ar = B->size[0] * B->size[1];
    B->size[i2] = TEMPLATE->size[i2];
    emxEnsureCapacity((emxArray__common *)B, ar, (int)sizeof(double));
  }

  morphopAlgo(TEMPLATE, B);
  i2 = TEMPLATE->size[0] * TEMPLATE->size[1];
  TEMPLATE->size[0] = B->size[0];
  TEMPLATE->size[1] = B->size[1];
  emxEnsureCapacity((emxArray__common *)TEMPLATE, i2, (int)sizeof(double));
  br = B->size[0] * B->size[1];
  for (i2 = 0; i2 < br; i2++) {
    TEMPLATE->data[i2] = B->data[i2];
  }

  // 'createTemplate:110' TEMPLATE(1:H+2,:) = -180;
  br = B->size[1];
  if (1.0F > H + 2.0F) {
    nm1d2 = 0;
  } else {
    nm1d2 = (int)(H + 2.0F);
  }

  emxFree_real_T(&B);
  for (i2 = 0; i2 < br; i2++) {
    for (ar = 0; ar < nm1d2; ar++) {
      TEMPLATE->data[ar + TEMPLATE->size[0] * i2] = -180.0;
    }
  }
}

//
// File trailer for createTemplate.cpp
//
// [EOF]
//
