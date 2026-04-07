/*
 * include/c_td.h
 *
 * C wrapper function prototypes for triple-double precision arithmetic.
 *
 * Copyright (c) 2026, Nakata Maho
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
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
void c_td_add_qd_td(const double *a, const double *b, double *c);
void c_td_add_td_qd(const double *a, const double *b, double *c);
void c_td_add_d_td(double a, const double *b, double *c);
void c_td_add_td_d(const double *a, double b, double *c);
void c_td_selfadd(const double *a, double *b);
void c_td_selfadd_dd(const double *a, double *b);
void c_td_selfadd_qd(const double *a, double *b);
void c_td_selfadd_d(double a, double *b);

void c_td_sub(const double *a, const double *b, double *c);
void c_td_sub_dd_td(const double *a, const double *b, double *c);
void c_td_sub_td_dd(const double *a, const double *b, double *c);
void c_td_sub_qd_td(const double *a, const double *b, double *c);
void c_td_sub_td_qd(const double *a, const double *b, double *c);
void c_td_sub_d_td(double a, const double *b, double *c);
void c_td_sub_td_d(const double *a, double b, double *c);
void c_td_selfsub(const double *a, double *b);
void c_td_selfsub_dd(const double *a, double *b);
void c_td_selfsub_qd(const double *a, double *b);
void c_td_selfsub_d(double a, double *b);

void c_td_mul(const double *a, const double *b, double *c);
void c_td_mul_dd_td(const double *a, const double *b, double *c);
void c_td_mul_td_dd(const double *a, const double *b, double *c);
void c_td_mul_qd_td(const double *a, const double *b, double *c);
void c_td_mul_td_qd(const double *a, const double *b, double *c);
void c_td_mul_d_td(double a, const double *b, double *c);
void c_td_mul_td_d(const double *a, double b, double *c);
void c_td_selfmul(const double *a, double *b);
void c_td_selfmul_dd(const double *a, double *b);
void c_td_selfmul_qd(const double *a, double *b);
void c_td_selfmul_d(double a, double *b);

void c_td_div(const double *a, const double *b, double *c);
void c_td_div_dd_td(const double *a, const double *b, double *c);
void c_td_div_td_dd(const double *a, const double *b, double *c);
void c_td_div_qd_td(const double *a, const double *b, double *c);
void c_td_div_td_qd(const double *a, const double *b, double *c);
void c_td_div_d_td(double a, const double *b, double *c);
void c_td_div_td_d(const double *a, double b, double *c);
void c_td_selfdiv(const double *a, double *b);
void c_td_selfdiv_dd(const double *a, double *b);
void c_td_selfdiv_qd(const double *a, double *b);
void c_td_selfdiv_d(double a, double *b);

void c_td_copy(const double *a, double *b);
void c_td_copy_dd(const double *a, double *b);
void c_td_copy_qd(const double *a, double *b);
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
void c_td_comp_td_qd(const double *a, const double *b, int *result);
void c_td_comp_qd_td(const double *a, const double *b, int *result);
void c_td_pi(double *a);
void c_td_2pi(double *a);
double c_td_epsilon(void);

#ifdef __cplusplus
}
#endif

#endif /* _QD_C_TD_H */
