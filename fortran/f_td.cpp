/*
 * fortran/f_td.cpp
 *
 * C++ wrapper functions for triple-double precision arithmetic.
 * This can be used from Fortran code.
 */
#include "config.h"
#ifdef HAVE_FORTRAN

#include <cstring>
#include <iostream>
#include <cstdlib>

#include <qd/td_real.h>
#include <qd/inline.h>

#define f_td_add          FC_FUNC_(f_td_add, F_TD_ADD)
#define f_td_add_td_d     FC_FUNC_(f_td_add_td_d, F_TD_ADD_TD_D)

#define f_td_sub          FC_FUNC_(f_td_sub, F_TD_SUB)
#define f_td_sub_td_d     FC_FUNC_(f_td_sub_td_d, F_TD_SUB_TD_D)
#define f_td_sub_d_td     FC_FUNC_(f_td_sub_d_td, F_TD_SUB_D_TD)

#define f_td_mul          FC_FUNC_(f_td_mul, F_TD_MUL)
#define f_td_mul_td_d     FC_FUNC_(f_td_mul_td_d, F_TD_MUL_TD_D)

#define f_td_div          FC_FUNC_(f_td_div, F_TD_DIV)
#define f_td_div_td_d     FC_FUNC_(f_td_div_td_d, F_TD_DIV_TD_D)
#define f_td_div_d_td     FC_FUNC_(f_td_div_d_td, F_TD_DIV_D_TD)

#define f_td_sqrt         FC_FUNC_(f_td_sqrt, F_TD_SQRT)
#define f_td_sqr          FC_FUNC_(f_td_sqr, F_TD_SQR)
#define f_td_abs          FC_FUNC_(f_td_abs, F_TD_ABS)

#define f_td_npwr         FC_FUNC_(f_td_npwr, F_TD_NPWR)
#define f_td_nroot        FC_FUNC_(f_td_nroot, F_TD_NROOT)
#define f_td_nint         FC_FUNC_(f_td_nint, F_TD_NINT)
#define f_td_aint         FC_FUNC_(f_td_aint, F_TD_AINT)
#define f_td_floor        FC_FUNC_(f_td_floor, F_TD_FLOOR)
#define f_td_ceil         FC_FUNC_(f_td_ceil, F_TD_CEIL)

#define f_td_exp          FC_FUNC_(f_td_exp, F_TD_EXP)
#define f_td_log          FC_FUNC_(f_td_log, F_TD_LOG)
#define f_td_log10        FC_FUNC_(f_td_log10, F_TD_LOG10)

#define f_td_sin          FC_FUNC_(f_td_sin, F_TD_SIN)
#define f_td_cos          FC_FUNC_(f_td_cos, F_TD_COS)
#define f_td_tan          FC_FUNC_(f_td_tan, F_TD_TAN)
#define f_td_sincos       FC_FUNC_(f_td_sincos, F_TD_SINCOS)

#define f_td_asin         FC_FUNC_(f_td_asin, F_TD_ASIN)
#define f_td_acos         FC_FUNC_(f_td_acos, F_TD_ACOS)
#define f_td_atan         FC_FUNC_(f_td_atan, F_TD_ATAN)
#define f_td_atan2        FC_FUNC_(f_td_atan2, F_TD_ATAN2)

#define f_td_sinh         FC_FUNC_(f_td_sinh, F_TD_SINH)
#define f_td_cosh         FC_FUNC_(f_td_cosh, F_TD_COSH)
#define f_td_tanh         FC_FUNC_(f_td_tanh, F_TD_TANH)
#define f_td_sincosh      FC_FUNC_(f_td_sincosh, F_TD_SINCOSH)

#define f_td_asinh        FC_FUNC_(f_td_asinh, F_TD_ASINH)
#define f_td_acosh        FC_FUNC_(f_td_acosh, F_TD_ACOSH)
#define f_td_atanh        FC_FUNC_(f_td_atanh, F_TD_ATANH)

#define f_td_swrite       FC_FUNC_(f_td_swrite, F_TD_SWRITE)
#define f_td_write        FC_FUNC_(f_td_write, F_TD_WRITE)
#define f_td_neg          FC_FUNC_(f_td_neg, F_TD_NEG)
#define f_td_rand         FC_FUNC_(f_td_rand, F_TD_RAND)
#define f_td_comp         FC_FUNC_(f_td_comp, F_TD_COMP)
#define f_td_comp_td_d    FC_FUNC_(f_td_comp_td_d, F_TD_COMP_TD_D)
#define f_td_comp_d_td    FC_FUNC_(f_td_comp_d_td, F_TD_COMP_D_TD)
#define f_td_pi           FC_FUNC_(f_td_pi, F_TD_PI)
#define f_td_nan          FC_FUNC_(f_td_nan, F_TD_NAN)

#define TO_DOUBLE_PTR(a, ptr) \
  ptr[0] = (a)[0]; \
  ptr[1] = (a)[1]; \
  ptr[2] = (a)[2];

extern "C" {

static td_real td_floor_local(const td_real &a) {
  double x0 = std::floor(a[0]);
  double x1 = 0.0;
  double x2 = 0.0;

  if (x0 == a[0]) {
    x1 = std::floor(a[1]);
    if (x1 == a[1]) {
      x2 = std::floor(a[2]);
    }
    td::renorm(x0, x1, x2);
  }

  return td_real(x0, x1, x2);
}

static td_real td_ceil_local(const td_real &a) {
  double x0 = std::ceil(a[0]);
  double x1 = 0.0;
  double x2 = 0.0;

  if (x0 == a[0]) {
    x1 = std::ceil(a[1]);
    if (x1 == a[1]) {
      x2 = std::ceil(a[2]);
    }
    td::renorm(x0, x1, x2);
  }

  return td_real(x0, x1, x2);
}

static td_real td_aint_local(const td_real &a) {
  return (a[0] >= 0.0) ? td_floor_local(a) : td_ceil_local(a);
}

static td_real tdrand_local() {
  static const double m_const = 4.6566128730773926e-10;
  double m = m_const;
  td_real r = 0.0;

  for (int i = 0; i < 6; i++, m *= m_const) {
    r += std::rand() * m;
  }

  return r;
}

void f_td_add(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(td_real(a) + td_real(b), c);
}

void f_td_add_td_d(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(td_real(a) + *b, c);
}

void f_td_sub(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(td_real(a) - td_real(b), c);
}

void f_td_sub_td_d(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(td_real(a) - *b, c);
}

void f_td_sub_d_td(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(*a - td_real(b), c);
}

void f_td_mul(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(td_real(a) * td_real(b), c);
}

void f_td_mul_td_d(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(td_real(a) * *b, c);
}

void f_td_div(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(td_real(a) / td_real(b), c);
}

void f_td_div_td_d(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(td_real(a) / *b, c);
}

void f_td_div_d_td(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(*a / td_real(b), c);
}

void f_td_sqrt(const double *a, double *b) {
  TO_DOUBLE_PTR(sqrt(td_real(a)), b);
}

void f_td_sqr(const double *a, double *b) {
  TO_DOUBLE_PTR(sqr(td_real(a)), b);
}

void f_td_abs(const double *a, double *b) {
  TO_DOUBLE_PTR(abs(td_real(a)), b);
}

void f_td_npwr(const double *a, const int *n, double *b) {
  TO_DOUBLE_PTR(npwr(td_real(a), *n), b);
}

void f_td_nroot(const double *a, const int *n, double *b) {
  TO_DOUBLE_PTR(nroot(td_real(a), *n), b);
}

void f_td_nint(const double *a, double *b) {
  TO_DOUBLE_PTR(nint(td_real(a)), b);
}

void f_td_aint(const double *a, double *b) {
  TO_DOUBLE_PTR(td_aint_local(td_real(a)), b);
}

void f_td_floor(const double *a, double *b) {
  TO_DOUBLE_PTR(td_floor_local(td_real(a)), b);
}

void f_td_ceil(const double *a, double *b) {
  TO_DOUBLE_PTR(td_ceil_local(td_real(a)), b);
}

void f_td_exp(const double *a, double *b) {
  TO_DOUBLE_PTR(exp(td_real(a)), b);
}

void f_td_log(const double *a, double *b) {
  TO_DOUBLE_PTR(log(td_real(a)), b);
}

void f_td_log10(const double *a, double *b) {
  TO_DOUBLE_PTR(log10(td_real(a)), b);
}

void f_td_sin(const double *a, double *b) {
  TO_DOUBLE_PTR(sin(td_real(a)), b);
}

void f_td_cos(const double *a, double *b) {
  TO_DOUBLE_PTR(cos(td_real(a)), b);
}

void f_td_tan(const double *a, double *b) {
  TO_DOUBLE_PTR(tan(td_real(a)), b);
}

void f_td_sincos(const double *a, double *s, double *c) {
  td_real ss, cc;
  sincos(td_real(a), ss, cc);
  TO_DOUBLE_PTR(ss, s);
  TO_DOUBLE_PTR(cc, c);
}

void f_td_asin(const double *a, double *b) {
  TO_DOUBLE_PTR(asin(td_real(a)), b);
}

void f_td_acos(const double *a, double *b) {
  TO_DOUBLE_PTR(acos(td_real(a)), b);
}

void f_td_atan(const double *a, double *b) {
  TO_DOUBLE_PTR(atan(td_real(a)), b);
}

void f_td_atan2(const double *a, const double *b, double *c) {
  TO_DOUBLE_PTR(atan2(td_real(a), td_real(b)), c);
}

void f_td_sinh(const double *a, double *b) {
  TO_DOUBLE_PTR(sinh(td_real(a)), b);
}

void f_td_cosh(const double *a, double *b) {
  TO_DOUBLE_PTR(cosh(td_real(a)), b);
}

void f_td_tanh(const double *a, double *b) {
  TO_DOUBLE_PTR(tanh(td_real(a)), b);
}

void f_td_sincosh(const double *a, double *s, double *c) {
  td_real ss, cc;
  sincosh(td_real(a), ss, cc);
  TO_DOUBLE_PTR(ss, s);
  TO_DOUBLE_PTR(cc, c);
}

void f_td_asinh(const double *a, double *b) {
  TO_DOUBLE_PTR(asinh(td_real(a)), b);
}

void f_td_acosh(const double *a, double *b) {
  TO_DOUBLE_PTR(acosh(td_real(a)), b);
}

void f_td_atanh(const double *a, double *b) {
  TO_DOUBLE_PTR(atanh(td_real(a)), b);
}

void f_td_swrite(const double *a, int *precision, char *s, int *maxlen) {
  int prec = *precision;
  if (prec <= 0 || prec > td_real::_ndigits) prec = td_real::_ndigits;
  std::ios_base::fmtflags fmt = static_cast<std::ios_base::fmtflags>(0);
  std::string str = td_real(a).to_string(prec, 0, fmt, false, true);

  int len = 0;
  if (a[0] < 0.0) {
    strncpy(&s[len], str.c_str(), *maxlen - len);
  } else {
    s[len++] = ' ';
    strncpy(&s[len], str.c_str(), *maxlen - len);
  }

  len += str.length();
  for (int i = len; i < *maxlen; i++) s[i] = ' ';
}

void f_td_write(const double *a) {
  std::cout << td_real(a) << std::endl;
}

void f_td_neg(const double *a, double *b) {
  b[0] = -a[0];
  b[1] = -a[1];
  b[2] = -a[2];
}

void f_td_rand(double *a) {
  TO_DOUBLE_PTR(tdrand_local(), a);
}

void f_td_comp(const double *a, const double *b, int *result) {
  td_real aa(a), bb(b);
  if (aa < bb) {
    *result = -1;
  } else if (aa > bb) {
    *result = 1;
  } else {
    *result = 0;
  }
}

void f_td_comp_td_d(const double *a, const double *b, int *result) {
  td_real aa(a);
  if (aa < *b) {
    *result = -1;
  } else if (aa > *b) {
    *result = 1;
  } else {
    *result = 0;
  }
}

void f_td_comp_d_td(const double *a, const double *b, int *result) {
  td_real bb(b);
  if (*a < bb) {
    *result = -1;
  } else if (*a > bb) {
    *result = 1;
  } else {
    *result = 0;
  }
}

void f_td_pi(double *a) {
  TO_DOUBLE_PTR(td_real::_pi, a);
}

void f_td_nan(double *a) {
  TO_DOUBLE_PTR(td_real::_nan, a);
}

}

#endif /* HAVE_FORTRAN */
