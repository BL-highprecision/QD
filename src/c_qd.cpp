/*
 * src/c_qd.cc
 *
 * This work was supported by the Director, Office of Science, Division
 * of Mathematical, Information, and Computational Sciences of the
 * U.S. Department of Energy under contract number DE-AC03-76SF00098.
 *
 * Copyright (c) 2000-2001
 *
 * Contains C wrapper function for quad-double precision arithmetic.
 * This can be used from fortran code.
 */
#include <cstring>

#include "config.h"
#include <qd/td_real.h>
#include <qd/qd_real.h>
#include <qd/c_qd.h>

#define TO_DOUBLE_PTR(a, ptr) ptr[0] = a.x[0]; ptr[1] = a.x[1]; \
                              ptr[2] = a.x[2]; ptr[3] = a.x[3];

extern "C" {



/* add */
void c_qd_add(const double *a, const double *b, double *c) {
  qd_real cc;
  cc = qd_real(a) + qd_real(b);
  TO_DOUBLE_PTR(cc, c);
}
void c_qd_add_qd_dd(const double *a, const double *b, double *c) {
  qd_real cc;
  cc = qd_real(a) + dd_real(b);
  TO_DOUBLE_PTR(cc, c);
}
void c_qd_add_dd_qd(const double *a, const double *b, double *c) {
  qd_real cc;
  cc = dd_real(a) + qd_real(b);
  TO_DOUBLE_PTR(cc, c);
}
void c_qd_add_qd_d(const double *a, double b, double *c) {
  qd_real cc;
  cc = qd_real(a) + b;
  TO_DOUBLE_PTR(cc, c);
}
void c_qd_add_d_qd(double a, const double *b, double *c) {
  qd_real cc;
  cc = a + qd_real(b);
  TO_DOUBLE_PTR(cc, c);
}
void c_qd_add_td_qd(const double *a, const double *b, double *c) {
  qd_real cc;
  cc = to_qd_real(td_real(a)) + qd_real(b);
  TO_DOUBLE_PTR(cc, c);
}
void c_qd_add_qd_td(const double *a, const double *b, double *c) {
  qd_real cc;
  cc = qd_real(a) + to_qd_real(td_real(b));
  TO_DOUBLE_PTR(cc, c);
}



/* sub */
void c_qd_sub(const double *a, const double *b, double *c) {
  qd_real cc;
  cc = qd_real(a) - qd_real(b);
  TO_DOUBLE_PTR(cc, c);
}
void c_qd_sub_qd_dd(const double *a, const double *b, double *c) {
  qd_real cc;
  cc = qd_real(a) - dd_real(b);
  TO_DOUBLE_PTR(cc, c);
}
void c_qd_sub_dd_qd(const double *a, const double *b, double *c) {
  qd_real cc;
  cc = dd_real(a) - qd_real(b);
  TO_DOUBLE_PTR(cc, c);
}
void c_qd_sub_qd_d(const double *a, double b, double *c) {
  qd_real cc;
  cc = qd_real(a) - b;
  TO_DOUBLE_PTR(cc, c);
}
void c_qd_sub_d_qd(double a, const double *b, double *c) {
  qd_real cc;
  cc = a - qd_real(b);
  TO_DOUBLE_PTR(cc, c);
}
void c_qd_sub_td_qd(const double *a, const double *b, double *c) {
  qd_real cc;
  cc = to_qd_real(td_real(a)) - qd_real(b);
  TO_DOUBLE_PTR(cc, c);
}
void c_qd_sub_qd_td(const double *a, const double *b, double *c) {
  qd_real cc;
  cc = qd_real(a) - to_qd_real(td_real(b));
  TO_DOUBLE_PTR(cc, c);
}



/* mul */
void c_qd_mul(const double *a, const double *b, double *c) {
  qd_real cc;
  cc = qd_real(a) * qd_real(b);
  TO_DOUBLE_PTR(cc, c);
}
void c_qd_mul_qd_dd(const double *a, const double *b, double *c) {
  qd_real cc;
  cc = qd_real(a) * dd_real(b);
  TO_DOUBLE_PTR(cc, c);
}
void c_qd_mul_dd_qd(const double *a, const double *b, double *c) {
  qd_real cc;
  cc = dd_real(a) * qd_real(b);
  TO_DOUBLE_PTR(cc, c);
}
void c_qd_mul_qd_d(const double *a, double b, double *c) {
  qd_real cc;
  cc = qd_real(a) * b;
  TO_DOUBLE_PTR(cc, c);
}
void c_qd_mul_d_qd(double a, const double *b, double *c) {
  qd_real cc;
  cc = a * qd_real(b);
  TO_DOUBLE_PTR(cc, c);
}
void c_qd_mul_td_qd(const double *a, const double *b, double *c) {
  qd_real cc;
  cc = to_qd_real(td_real(a)) * qd_real(b);
  TO_DOUBLE_PTR(cc, c);
}
void c_qd_mul_qd_td(const double *a, const double *b, double *c) {
  qd_real cc;
  cc = qd_real(a) * to_qd_real(td_real(b));
  TO_DOUBLE_PTR(cc, c);
}



/* div */
void c_qd_div(const double *a, const double *b, double *c) {
  qd_real cc;
  cc = qd_real(a) / qd_real(b);
  TO_DOUBLE_PTR(cc, c);
}
void c_qd_div_qd_dd(const double *a, const double *b, double *c) {
  qd_real cc;
  cc = qd_real(a) / dd_real(b);
  TO_DOUBLE_PTR(cc, c);
}
void c_qd_div_dd_qd(const double *a, const double *b, double *c) {
  qd_real cc;
  cc = dd_real(a) / qd_real(b);
  TO_DOUBLE_PTR(cc, c);
}
void c_qd_div_qd_d(const double *a, double b, double *c) {
  qd_real cc;
  cc = qd_real(a) / b;
  TO_DOUBLE_PTR(cc, c);
}
void c_qd_div_d_qd(double a, const double *b, double *c) {
  qd_real cc;
  cc = a / qd_real(b);
  TO_DOUBLE_PTR(cc, c);
}
void c_qd_div_td_qd(const double *a, const double *b, double *c) {
  qd_real cc;
  cc = to_qd_real(td_real(a)) / qd_real(b);
  TO_DOUBLE_PTR(cc, c);
}
void c_qd_div_qd_td(const double *a, const double *b, double *c) {
  qd_real cc;
  cc = qd_real(a) / to_qd_real(td_real(b));
  TO_DOUBLE_PTR(cc, c);
}




/* selfadd */
void c_qd_selfadd(const double *a, double *b) {
  qd_real bb(b);
  bb += qd_real(a);
  TO_DOUBLE_PTR(bb, b);
}
void c_qd_selfadd_dd(const double *a, double *b) {
  qd_real bb(b);
  bb += dd_real(a);
  TO_DOUBLE_PTR(bb, b);
}
void c_qd_selfadd_td(const double *a, double *b) {
  qd_real bb(b);
  bb += to_qd_real(td_real(a));
  TO_DOUBLE_PTR(bb, b);
}
void c_qd_selfadd_d(double a, double *b) {
  qd_real bb(b);
  bb += a;
  TO_DOUBLE_PTR(bb, b);
}



/* selfsub */
void c_qd_selfsub(const double *a, double *b) {
  qd_real bb(b);
  bb -= qd_real(a);
  TO_DOUBLE_PTR(bb, b);
}
void c_qd_selfsub_dd(const double *a, double *b) {
  qd_real bb(b);
  bb -= dd_real(a);
  TO_DOUBLE_PTR(bb, b);
}
void c_qd_selfsub_td(const double *a, double *b) {
  qd_real bb(b);
  bb -= to_qd_real(td_real(a));
  TO_DOUBLE_PTR(bb, b);
}
void c_qd_selfsub_d(double a, double *b) {
  qd_real bb(b);
  bb -= a;
  TO_DOUBLE_PTR(bb, b);
}



/* selfmul */
void c_qd_selfmul(const double *a, double *b) {
  qd_real bb(b);
  bb *= qd_real(a);
  TO_DOUBLE_PTR(bb, b);
}
void c_qd_selfmul_dd(const double *a, double *b) {
  qd_real bb(b);
  bb *= dd_real(a);
  TO_DOUBLE_PTR(bb, b);
}
void c_qd_selfmul_td(const double *a, double *b) {
  qd_real bb(b);
  bb *= to_qd_real(td_real(a));
  TO_DOUBLE_PTR(bb, b);
}
void c_qd_selfmul_d(double a, double *b) {
  qd_real bb(b);
  bb *= a;
  TO_DOUBLE_PTR(bb, b);
}



/* selfdiv */
void c_qd_selfdiv(const double *a, double *b) {
  qd_real bb(b);
  bb /= qd_real(a);
  TO_DOUBLE_PTR(bb, b);
}
void c_qd_selfdiv_dd(const double *a, double *b) {
  qd_real bb(b);
  bb /= dd_real(a);
  TO_DOUBLE_PTR(bb, b);
}
void c_qd_selfdiv_td(const double *a, double *b) {
  qd_real bb(b);
  bb /= to_qd_real(td_real(a));
  TO_DOUBLE_PTR(bb, b);
}
void c_qd_selfdiv_d(double a, double *b) {
  qd_real bb(b);
  bb /= a;
  TO_DOUBLE_PTR(bb, b);
}



/* copy */
void c_qd_copy(const double *a, double *b) {
  b[0] = a[0];
  b[1] = a[1];
  b[2] = a[2];
  b[3] = a[3];
}
void c_qd_copy_dd(const double *a, double *b) {
  b[0] = a[0];
  b[1] = a[1];
  b[2] = 0.0;
  b[3] = 0.0;
}
void c_qd_copy_td(const double *a, double *b) {
  qd_real bb = to_qd_real(td_real(a));
  TO_DOUBLE_PTR(bb, b);
}
void c_qd_copy_d(double a, double *b) {
  b[0] = a;
  b[1] = 0.0;
  b[2] = 0.0;
  b[3] = 0.0;
}


int c_qd_sqrt(const double *a, double *b) {
  int flag;
  qd_real bb;
  bb = fsqrt(qd_real(a), flag);
  TO_DOUBLE_PTR(bb, b);
  return flag;
}

void c_qd_sqr(const double *a, double *b) {
  qd_real bb;
  bb = sqr(qd_real(a));
  TO_DOUBLE_PTR(bb, b);
}

void c_qd_abs(const double *a, double *b) {
  qd_real bb;
  bb = abs(qd_real(a));
  TO_DOUBLE_PTR(bb, b);
}

void c_qd_npwr(const double *a, int n, double *b) {
  qd_real bb;
  bb = npwr(qd_real(a), n);
  TO_DOUBLE_PTR(bb, b);
}

void c_qd_nroot(const double *a, int n, double *b) {
  qd_real bb;
  bb = nroot(qd_real(a), n);
  TO_DOUBLE_PTR(bb, b);
}

void c_qd_nint(const double *a, double *b) {
  qd_real bb;
  bb = nint(qd_real(a));
  TO_DOUBLE_PTR(bb, b);
}
void c_qd_aint(const double *a, double *b) {
  qd_real bb;
  bb = aint(qd_real(a));
  TO_DOUBLE_PTR(bb, b);
}
void c_qd_floor(const double *a, double *b) {
  qd_real bb;
  bb = floor(qd_real(a));
  TO_DOUBLE_PTR(bb, b);
}
void c_qd_ceil(const double *a, double *b) {
  qd_real bb;
  bb = ceil(qd_real(a));
  TO_DOUBLE_PTR(bb, b);
}

void c_qd_log(const double *a, double *b) {
  qd_real bb;
  bb = log(qd_real(a));
  TO_DOUBLE_PTR(bb, b);
}
void c_qd_log10(const double *a, double *b) {
  qd_real bb;
  bb = log10(qd_real(a));
  TO_DOUBLE_PTR(bb, b);
}
void c_qd_exp(const double *a, double *b) {
  qd_real bb;
  bb = exp(qd_real(a));
  TO_DOUBLE_PTR(bb, b);
}

void c_qd_sin(const double *a, double *b) {
  qd_real bb;
  bb = sin(qd_real(a));
  TO_DOUBLE_PTR(bb, b);
}
void c_qd_cos(const double *a, double *b) {
  qd_real bb;
  bb = cos(qd_real(a));
  TO_DOUBLE_PTR(bb, b);
}
void c_qd_tan(const double *a, double *b) {
  qd_real bb;
  bb = tan(qd_real(a));
  TO_DOUBLE_PTR(bb, b);
}

void c_qd_asin(const double *a, double *b) {
  qd_real bb;
  bb = asin(qd_real(a));
  TO_DOUBLE_PTR(bb, b);
}
void c_qd_acos(const double *a, double *b) {
  qd_real bb;
  bb = acos(qd_real(a));
  TO_DOUBLE_PTR(bb, b);
}
void c_qd_atan(const double *a, double *b) {
  qd_real bb;
  bb = atan(qd_real(a));
  TO_DOUBLE_PTR(bb, b);
}

void c_qd_atan2(const double *a, const double *b, double *c) {
  qd_real cc;
  cc = atan2(qd_real(a), qd_real(b));
  TO_DOUBLE_PTR(cc, c);
}

void c_qd_sinh(const double *a, double *b) {
  qd_real bb;
  bb = sinh(qd_real(a));
  TO_DOUBLE_PTR(bb, b);
}
void c_qd_cosh(const double *a, double *b) {
  qd_real bb;
  bb = cosh(qd_real(a));
  TO_DOUBLE_PTR(bb, b);
}
void c_qd_tanh(const double *a, double *b) {
  qd_real bb;
  bb = tanh(qd_real(a));
  TO_DOUBLE_PTR(bb, b);
}

void c_qd_asinh(const double *a, double *b) {
  qd_real bb;
  bb = asinh(qd_real(a));
  TO_DOUBLE_PTR(bb, b);
}
void c_qd_acosh(const double *a, double *b) {
  qd_real bb;
  bb = acosh(qd_real(a));
  TO_DOUBLE_PTR(bb, b);
}
void c_qd_atanh(const double *a, double *b) {
  qd_real bb;
  bb = atanh(qd_real(a));
  TO_DOUBLE_PTR(bb, b);
}

void c_qd_sincos(const double *a, double *s, double *c) {
  qd_real ss, cc;
  sincos(qd_real(a), ss, cc);
  TO_DOUBLE_PTR(cc, c);
  TO_DOUBLE_PTR(ss, s);
}

void c_qd_sincosh(const double *a, double *s, double *c) {
  qd_real ss, cc;
  sincosh(qd_real(a), ss, cc);
  TO_DOUBLE_PTR(cc, c);
  TO_DOUBLE_PTR(ss, s);
}

void c_qd_read(const char *s, double *a) {
  qd_real aa(s);
  TO_DOUBLE_PTR(aa, a);
}

void c_qd_swrite(const double *a, int precision, char *s, int len) {
  qd_real(a).write(s, len, precision);
}

void c_qd_write(const double *a) {
  std::cout << qd_real(a).to_string(qd_real::_ndigits) << std::endl;
}

void c_qd_neg(const double *a, double *b) {
  b[0] = -a[0];
  b[1] = -a[1];
  b[2] = -a[2];
  b[3] = -a[3];
}

void c_qd_rand(double *a) {
  qd_real aa;
  aa = qdrand();
  TO_DOUBLE_PTR(aa, a);
}

void c_qd_comp(const double *a, const double *b, int *result) {
  qd_real aa(a), bb(b);
  if (aa < bb)
    *result = -1;
  else if (aa > bb)
    *result = 1;
  else 
    *result = 0;
}

void c_qd_comp_qd_d(const double *a, double b, int *result) {
  qd_real aa(a);
  if (aa < b)
    *result = -1;
  else if (aa > b)
    *result = 1;
  else 
    *result = 0;
}

void c_qd_comp_d_qd(double a, const double *b, int *result) {
  qd_real bb(b);
  if (a < bb)
    *result = -1;
  else if (a > bb)
    *result = 1;
  else 
    *result = 0;
}

void c_qd_comp_qd_td(const double *a, const double *b, int *result) {
  qd_real aa(a), bb = to_qd_real(td_real(b));
  if (aa < bb)
    *result = -1;
  else if (aa > bb)
    *result = 1;
  else
    *result = 0;
}

void c_qd_comp_td_qd(const double *a, const double *b, int *result) {
  qd_real aa = to_qd_real(td_real(a)), bb(b);
  if (aa < bb)
    *result = -1;
  else if (aa > bb)
    *result = 1;
  else
    *result = 0;
}

void c_qd_pi(double *a) {
  TO_DOUBLE_PTR(qd_real::_pi, a);
}

void c_qd_2pi(double *a) {
  TO_DOUBLE_PTR(qd_real::_2pi, a);
}

double c_qd_epsilon(void) {
    return (double) std::numeric_limits<qd_real>::epsilon();
}

void c_qd_pi2(double *a) {
  TO_DOUBLE_PTR(qd_real::_pi2, a);
}

void c_qd_pi4(double *a) {
  TO_DOUBLE_PTR(qd_real::_pi4, a);
}

void c_qd_3pi4(double *a) {
  TO_DOUBLE_PTR(qd_real::_3pi4, a);
}

void c_qd_e(double *a) {
  TO_DOUBLE_PTR(qd_real::_e, a);
}

void c_qd_ln2(double *a) {
  TO_DOUBLE_PTR(qd_real::_log2, a);
}

void c_qd_ln10(double *a) {
  TO_DOUBLE_PTR(qd_real::_log10, a);
}

void c_qd_nan(double *a) {
  TO_DOUBLE_PTR(qd_real::_nan, a);
}

void c_qd_inf(double *a) {
  TO_DOUBLE_PTR(qd_real::_inf, a);
}

void c_qd_pow(const double *a, const double *b, double *c) {
  qd_real cc;
  cc = pow(qd_real(a), qd_real(b));
  TO_DOUBLE_PTR(cc, c);
}

void c_qd_log2(const double *a, double *b) {
  qd_real bb;
  bb = log(qd_real(a)) / qd_real::_log2;
  TO_DOUBLE_PTR(bb, b);
}

void c_qd_fmod(const double *a, const double *b, double *c) {
  qd_real cc;
  cc = fmod(qd_real(a), qd_real(b));
  TO_DOUBLE_PTR(cc, c);
}

void c_qd_inv(const double *a, double *b) {
  qd_real bb;
  bb = inv(qd_real(a));
  TO_DOUBLE_PTR(bb, b);
}

int c_qd_isfinite(const double *a) {
  return isfinite(qd_real(a));
}

int c_qd_isinf(const double *a) {
  return isinf(qd_real(a));
}

int c_qd_isnan(const double *a) {
  return isnan(qd_real(a));
}

void c_qd_fmax(const double *a, const double *b, double *c) {
  qd_real aa(a), bb(b);
  if (isnan(aa)) {
    TO_DOUBLE_PTR(bb, c);
  } else if (isnan(bb)) {
    TO_DOUBLE_PTR(aa, c);
  } else if (aa > bb) {
    TO_DOUBLE_PTR(aa, c);
  } else {
    TO_DOUBLE_PTR(bb, c);
  }
}

void c_qd_fmin(const double *a, const double *b, double *c) {
  qd_real aa(a), bb(b);
  if (isnan(aa)) {
    TO_DOUBLE_PTR(bb, c);
  } else if (isnan(bb)) {
    TO_DOUBLE_PTR(aa, c);
  } else if (aa < bb) {
    TO_DOUBLE_PTR(aa, c);
  } else {
    TO_DOUBLE_PTR(bb, c);
  }
}

void c_qd_max(const double *a, const double *b, double *c) {
  qd_real aa(a), bb(b);
  qd_real cc;
  cc = (aa > bb) ? aa : bb;
  TO_DOUBLE_PTR(cc, c);
}

void c_qd_min(const double *a, const double *b, double *c) {
  qd_real aa(a), bb(b);
  qd_real cc;
  cc = (aa < bb) ? aa : bb;
  TO_DOUBLE_PTR(cc, c);
}

void c_qd_divrem(const double *a, const double *b, double *q, double *r) {
  qd_real rr;
  qd_real qq = divrem(qd_real(a), qd_real(b), rr);
  TO_DOUBLE_PTR(qq, q);
  TO_DOUBLE_PTR(rr, r);
}

void c_qd_drem(const double *a, const double *b, double *c) {
  qd_real cc;
  cc = drem(qd_real(a), qd_real(b));
  TO_DOUBLE_PTR(cc, c);
}

}
