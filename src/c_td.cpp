/*
 * src/c_td.cpp
 *
 * C wrapper functions for triple-double precision arithmetic.
 */
#include <cstring>
#include <iostream>

#include "config.h"
#include <qd/td_real.h>
#include <qd/c_td.h>

#define TO_DOUBLE_PTR(a, ptr) \
  ptr[0] = (a)[0]; \
  ptr[1] = (a)[1]; \
  ptr[2] = (a)[2];

extern "C" {

void c_td_add(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(td_real(a) + td_real(b), c);
}

void c_td_add_dd_td(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(dd_real(a) + td_real(b), c);
}

void c_td_add_td_dd(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(td_real(a) + dd_real(b), c);
}

void c_td_add_qd_td(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(to_td_real(qd_real(a)) + td_real(b), c);
}

void c_td_add_td_qd(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(td_real(a) + to_td_real(qd_real(b)), c);
}

void c_td_add_d_td(double a, const double *b, double *c) {
  TO_DOUBLE_PTR(a + td_real(b), c);
}

void c_td_add_td_d(const double *a, double b, double *c) {
  TO_DOUBLE_PTR(td_real(a) + b, c);
}

void c_td_selfadd(const double *a, double *b) {
  td_real bb(b);
  bb += td_real(a);
  TO_DOUBLE_PTR(bb, b);
}

void c_td_selfadd_dd(const double *a, double *b) {
  td_real bb(b);
  bb += dd_real(a);
  TO_DOUBLE_PTR(bb, b);
}

void c_td_selfadd_qd(const double *a, double *b) {
  td_real bb(b);
  bb += to_td_real(qd_real(a));
  TO_DOUBLE_PTR(bb, b);
}

void c_td_selfadd_d(double a, double *b) {
  td_real bb(b);
  bb += a;
  TO_DOUBLE_PTR(bb, b);
}

void c_td_sub(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(td_real(a) - td_real(b), c);
}

void c_td_sub_dd_td(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(dd_real(a) - td_real(b), c);
}

void c_td_sub_td_dd(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(td_real(a) - dd_real(b), c);
}

void c_td_sub_qd_td(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(to_td_real(qd_real(a)) - td_real(b), c);
}

void c_td_sub_td_qd(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(td_real(a) - to_td_real(qd_real(b)), c);
}

void c_td_sub_d_td(double a, const double *b, double *c) {
  TO_DOUBLE_PTR(a - td_real(b), c);
}

void c_td_sub_td_d(const double *a, double b, double *c) {
  TO_DOUBLE_PTR(td_real(a) - b, c);
}

void c_td_selfsub(const double *a, double *b) {
  td_real bb(b);
  bb -= td_real(a);
  TO_DOUBLE_PTR(bb, b);
}

void c_td_selfsub_dd(const double *a, double *b) {
  td_real bb(b);
  bb -= dd_real(a);
  TO_DOUBLE_PTR(bb, b);
}

void c_td_selfsub_qd(const double *a, double *b) {
  td_real bb(b);
  bb -= to_td_real(qd_real(a));
  TO_DOUBLE_PTR(bb, b);
}

void c_td_selfsub_d(double a, double *b) {
  td_real bb(b);
  bb -= a;
  TO_DOUBLE_PTR(bb, b);
}

void c_td_mul(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(td_real(a) * td_real(b), c);
}

void c_td_mul_dd_td(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(dd_real(a) * td_real(b), c);
}

void c_td_mul_td_dd(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(td_real(a) * dd_real(b), c);
}

void c_td_mul_qd_td(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(to_td_real(qd_real(a)) * td_real(b), c);
}

void c_td_mul_td_qd(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(td_real(a) * to_td_real(qd_real(b)), c);
}

void c_td_mul_d_td(double a, const double *b, double *c) {
  TO_DOUBLE_PTR(a * td_real(b), c);
}

void c_td_mul_td_d(const double *a, double b, double *c) {
  TO_DOUBLE_PTR(td_real(a) * b, c);
}

void c_td_selfmul(const double *a, double *b) {
  td_real bb(b);
  bb *= td_real(a);
  TO_DOUBLE_PTR(bb, b);
}

void c_td_selfmul_dd(const double *a, double *b) {
  td_real bb(b);
  bb *= dd_real(a);
  TO_DOUBLE_PTR(bb, b);
}

void c_td_selfmul_qd(const double *a, double *b) {
  td_real bb(b);
  bb *= to_td_real(qd_real(a));
  TO_DOUBLE_PTR(bb, b);
}

void c_td_selfmul_d(double a, double *b) {
  td_real bb(b);
  bb *= a;
  TO_DOUBLE_PTR(bb, b);
}

void c_td_div(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(td_real(a) / td_real(b), c);
}

void c_td_div_dd_td(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(dd_real(a) / td_real(b), c);
}

void c_td_div_td_dd(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(td_real(a) / dd_real(b), c);
}

void c_td_div_qd_td(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(to_td_real(qd_real(a)) / td_real(b), c);
}

void c_td_div_td_qd(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(td_real(a) / to_td_real(qd_real(b)), c);
}

void c_td_div_d_td(double a, const double *b, double *c) {
  TO_DOUBLE_PTR(a / td_real(b), c);
}

void c_td_div_td_d(const double *a, double b, double *c) {
  TO_DOUBLE_PTR(td_real(a) / b, c);
}

void c_td_selfdiv(const double *a, double *b) {
  td_real bb(b);
  bb /= td_real(a);
  TO_DOUBLE_PTR(bb, b);
}

void c_td_selfdiv_dd(const double *a, double *b) {
  td_real bb(b);
  bb /= dd_real(a);
  TO_DOUBLE_PTR(bb, b);
}

void c_td_selfdiv_qd(const double *a, double *b) {
  td_real bb(b);
  bb /= to_td_real(qd_real(a));
  TO_DOUBLE_PTR(bb, b);
}

void c_td_selfdiv_d(double a, double *b) {
  td_real bb(b);
  bb /= a;
  TO_DOUBLE_PTR(bb, b);
}

void c_td_copy(const double *a, double *b) {
  b[0] = a[0];
  b[1] = a[1];
  b[2] = a[2];
}

void c_td_copy_dd(const double *a, double *b) {
  b[0] = a[0];
  b[1] = a[1];
  b[2] = 0.0;
}

void c_td_copy_qd(const double *a, double *b) {
  TO_DOUBLE_PTR(to_td_real(qd_real(a)), b);
}

void c_td_copy_d(double a, double *b) {
  b[0] = a;
  b[1] = 0.0;
  b[2] = 0.0;
}

void c_td_sqrt(const double *a, double *b) {
  TO_DOUBLE_PTR(sqrt(td_real(a)), b);
}

void c_td_sqr(const double *a, double *b) {
  TO_DOUBLE_PTR(sqr(td_real(a)), b);
}

void c_td_abs(const double *a, double *b) {
  TO_DOUBLE_PTR(abs(td_real(a)), b);
}

void c_td_npwr(const double *a, int n, double *b) {
  TO_DOUBLE_PTR(npwr(td_real(a), n), b);
}

void c_td_exp(const double *a, double *b) {
  TO_DOUBLE_PTR(exp(td_real(a)), b);
}

void c_td_log(const double *a, double *b) {
  TO_DOUBLE_PTR(log(td_real(a)), b);
}

void c_td_log10(const double *a, double *b) {
  TO_DOUBLE_PTR(log10(td_real(a)), b);
}

void c_td_sin(const double *a, double *b) {
  TO_DOUBLE_PTR(sin(td_real(a)), b);
}

void c_td_cos(const double *a, double *b) {
  TO_DOUBLE_PTR(cos(td_real(a)), b);
}

void c_td_tan(const double *a, double *b) {
  TO_DOUBLE_PTR(tan(td_real(a)), b);
}

void c_td_sincos(const double *a, double *s, double *c) {
  td_real ss;
  td_real cc;
  sincos(td_real(a), ss, cc);
  TO_DOUBLE_PTR(ss, s);
  TO_DOUBLE_PTR(cc, c);
}

void c_td_asin(const double *a, double *b) {
  TO_DOUBLE_PTR(asin(td_real(a)), b);
}

void c_td_acos(const double *a, double *b) {
  TO_DOUBLE_PTR(acos(td_real(a)), b);
}

void c_td_atan(const double *a, double *b) {
  TO_DOUBLE_PTR(atan(td_real(a)), b);
}

void c_td_atan2(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(atan2(td_real(a), td_real(b)), c);
}

void c_td_sinh(const double *a, double *b) {
  TO_DOUBLE_PTR(sinh(td_real(a)), b);
}

void c_td_cosh(const double *a, double *b) {
  TO_DOUBLE_PTR(cosh(td_real(a)), b);
}

void c_td_tanh(const double *a, double *b) {
  TO_DOUBLE_PTR(tanh(td_real(a)), b);
}

void c_td_sincosh(const double *a, double *s, double *c) {
  td_real ss;
  td_real cc;
  sincosh(td_real(a), ss, cc);
  TO_DOUBLE_PTR(ss, s);
  TO_DOUBLE_PTR(cc, c);
}

void c_td_asinh(const double *a, double *b) {
  TO_DOUBLE_PTR(asinh(td_real(a)), b);
}

void c_td_acosh(const double *a, double *b) {
  TO_DOUBLE_PTR(acosh(td_real(a)), b);
}

void c_td_atanh(const double *a, double *b) {
  TO_DOUBLE_PTR(atanh(td_real(a)), b);
}

void c_td_read(const char *s, double *a) {
  TO_DOUBLE_PTR(td_real(s), a);
}

void c_td_swrite(const double *a, int precision, char *s, int len) {
  td_real(a).write(s, len, precision);
}

void c_td_write(const double *a) {
  std::cout << td_real(a).to_string(td_real::_ndigits) << std::endl;
}

void c_td_neg(const double *a, double *b) {
  b[0] = -a[0];
  b[1] = -a[1];
  b[2] = -a[2];
}

void c_td_comp(const double *a, const double *b, int *result) {
  td_real aa(a);
  td_real bb(b);
  if (aa < bb) {
    *result = -1;
  } else if (aa > bb) {
    *result = 1;
  } else {
    *result = 0;
  }
}

void c_td_comp_td_d(const double *a, double b, int *result) {
  td_real aa(a);
  if (aa < b) {
    *result = -1;
  } else if (aa > b) {
    *result = 1;
  } else {
    *result = 0;
  }
}

void c_td_comp_d_td(double a, const double *b, int *result) {
  td_real bb(b);
  if (a < bb) {
    *result = -1;
  } else if (a > bb) {
    *result = 1;
  } else {
    *result = 0;
  }
}

void c_td_comp_td_qd(const double *a, const double *b, int *result) {
  td_real aa(a);
  td_real bb = to_td_real(qd_real(b));
  if (aa < bb) {
    *result = -1;
  } else if (aa > bb) {
    *result = 1;
  } else {
    *result = 0;
  }
}

void c_td_comp_qd_td(const double *a, const double *b, int *result) {
  td_real aa = to_td_real(qd_real(a));
  td_real bb(b);
  if (aa < bb) {
    *result = -1;
  } else if (aa > bb) {
    *result = 1;
  } else {
    *result = 0;
  }
}

void c_td_pi(double *a) {
  TO_DOUBLE_PTR(td_real::_pi, a);
}

void c_td_2pi(double *a) {
  TO_DOUBLE_PTR(td_real::_2pi, a);
}

double c_td_epsilon(void) {
  return std::numeric_limits<td_real>::epsilon();
}

}  // extern "C"
