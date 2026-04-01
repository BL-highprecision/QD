/*
 * include/c_td.h
 *
 * C wrapper function prototypes for triple-double precision arithmetic.
 */
#ifndef _QD_C_TD_H
#define _QD_C_TD_H

#include <qd/c_dd.h>
#include <qd/qd_config.h>

#ifdef __cplusplus
extern "C" {
#endif

void c_td_add(const double *a, const double *b, double *c);
void c_td_add_dd_td(const double *a, const double *b, double *c);
void c_td_add_td_dd(const double *a, const double *b, double *c);
void c_td_add_d_td(double a, const double *b, double *c);
void c_td_add_td_d(const double *a, double b, double *c);
void c_td_selfadd(const double *a, double *b);
void c_td_selfadd_dd(const double *a, double *b);
void c_td_selfadd_d(double a, double *b);

void c_td_sub(const double *a, const double *b, double *c);
void c_td_sub_dd_td(const double *a, const double *b, double *c);
void c_td_sub_td_dd(const double *a, const double *b, double *c);
void c_td_sub_d_td(double a, const double *b, double *c);
void c_td_sub_td_d(const double *a, double b, double *c);
void c_td_selfsub(const double *a, double *b);
void c_td_selfsub_dd(const double *a, double *b);
void c_td_selfsub_d(double a, double *b);

void c_td_mul(const double *a, const double *b, double *c);
void c_td_mul_dd_td(const double *a, const double *b, double *c);
void c_td_mul_td_dd(const double *a, const double *b, double *c);
void c_td_mul_d_td(double a, const double *b, double *c);
void c_td_mul_td_d(const double *a, double b, double *c);
void c_td_selfmul(const double *a, double *b);
void c_td_selfmul_dd(const double *a, double *b);
void c_td_selfmul_d(double a, double *b);

void c_td_div(const double *a, const double *b, double *c);
void c_td_div_dd_td(const double *a, const double *b, double *c);
void c_td_div_td_dd(const double *a, const double *b, double *c);
void c_td_div_d_td(double a, const double *b, double *c);
void c_td_div_td_d(const double *a, double b, double *c);
void c_td_selfdiv(const double *a, double *b);
void c_td_selfdiv_dd(const double *a, double *b);
void c_td_selfdiv_d(double a, double *b);

void c_td_copy(const double *a, double *b);
void c_td_copy_dd(const double *a, double *b);
void c_td_copy_d(double a, double *b);

void c_td_sqrt(const double *a, double *b);
void c_td_sqr(const double *a, double *b);
void c_td_abs(const double *a, double *b);
void c_td_npwr(const double *a, int n, double *b);

void c_td_exp(const double *a, double *b);
void c_td_log(const double *a, double *b);
void c_td_log10(const double *a, double *b);

void c_td_sin(const double *a, double *b);
void c_td_cos(const double *a, double *b);
void c_td_tan(const double *a, double *b);
void c_td_sincos(const double *a, double *s, double *c);

void c_td_asin(const double *a, double *b);
void c_td_acos(const double *a, double *b);
void c_td_atan(const double *a, double *b);
void c_td_atan2(const double *a, const double *b, double *c);

void c_td_sinh(const double *a, double *b);
void c_td_cosh(const double *a, double *b);
void c_td_tanh(const double *a, double *b);
void c_td_sincosh(const double *a, double *s, double *c);

void c_td_asinh(const double *a, double *b);
void c_td_acosh(const double *a, double *b);
void c_td_atanh(const double *a, double *b);

void c_td_read(const char *s, double *a);
void c_td_swrite(const double *a, int precision, char *s, int len);
void c_td_write(const double *a);
void c_td_neg(const double *a, double *b);
void c_td_comp(const double *a, const double *b, int *result);
void c_td_comp_td_d(const double *a, double b, int *result);
void c_td_comp_d_td(double a, const double *b, int *result);
void c_td_pi(double *a);
void c_td_2pi(double *a);
double c_td_epsilon(void);

#ifdef __cplusplus
}
#endif

#endif /* _QD_C_TD_H */
