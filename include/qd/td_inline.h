/*
 * include/td_inline.h
 *
 * Contains small functions (suitable for inlining) in the triple-double
 * arithmetic package.
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
#ifndef _QD_TD_INLINE_H
#define _QD_TD_INLINE_H

#include <cmath>
#include <qd/inline.h>

#ifndef QD_INLINE
#define inline
#endif

namespace td {

inline bool div_needs_rescale(double a_hi) {
  return std::fabs(a_hi) > 0x1p+969;
}

inline void renorm(double &c0, double &c1, double &c2) {
  double s0;
  double s1;
  double s2 = 0.0;

  if (QD_ISINF(c0)) return;

  c0 = qd::quick_two_sum(c0, c1, c1);
  s0 = c0;
  s1 = c1;

  if (s1 != 0.0) {
    s1 = qd::quick_two_sum(s1, c2, s2);
  } else {
    s0 = qd::quick_two_sum(s0, c2, s1);
  }

  c0 = s0;
  c1 = s1;
  c2 = s2;
}

inline void renorm(double &c0, double &c1, double &c2, double &c3) {
  double s0;
  double s1;
  double s2 = 0.0;

  if (QD_ISINF(c0)) return;

  s0 = qd::quick_two_sum(c2, c3, c3);
  s0 = qd::quick_two_sum(c1, s0, c2);
  c0 = qd::quick_two_sum(c0, s0, c1);

  s0 = c0;
  s1 = c1;
  if (s1 != 0.0) {
    s1 = qd::quick_two_sum(s1, c2, s2);
    if (s2 != 0.0) {
      s2 += c3;
    } else {
      s1 = qd::quick_two_sum(s1, c3, s2);
    }
  } else {
    s0 = qd::quick_two_sum(s0, c2, s1);
    if (s1 != 0.0) {
      s1 = qd::quick_two_sum(s1, c3, s2);
    } else {
      s0 = qd::quick_two_sum(s0, c3, s1);
    }
  }

  c0 = s0;
  c1 = s1;
  c2 = s2;
}

inline void renorm(double &c0, double &c1, double &c2, double &c3, double &c4) {
  double s0;
  double s1;
  double s2 = 0.0;
  double s3 = 0.0;

  if (QD_ISINF(c0)) return;

  s0 = qd::quick_two_sum(c3, c4, c4);
  s0 = qd::quick_two_sum(c2, s0, c3);
  s0 = qd::quick_two_sum(c1, s0, c2);
  c0 = qd::quick_two_sum(c0, s0, c1);

  s0 = c0;
  s1 = c1;

  if (s1 != 0.0) {
    s1 = qd::quick_two_sum(s1, c2, s2);
    if (s2 != 0.0) {
      s2 = qd::quick_two_sum(s2, c3, s3);
      if (s3 != 0.0) {
        s3 += c4;
      } else {
        s2 = qd::quick_two_sum(s2, c4, s3);
      }
    } else {
      s1 = qd::quick_two_sum(s1, c3, s2);
      if (s2 != 0.0) {
        s2 = qd::quick_two_sum(s2, c4, s3);
      } else {
        s1 = qd::quick_two_sum(s1, c4, s2);
      }
    }
  } else {
    s0 = qd::quick_two_sum(s0, c2, s1);
    if (s1 != 0.0) {
      s1 = qd::quick_two_sum(s1, c3, s2);
      if (s2 != 0.0) {
        s2 = qd::quick_two_sum(s2, c4, s3);
      } else {
        s1 = qd::quick_two_sum(s1, c4, s2);
      }
    } else {
      s0 = qd::quick_two_sum(s0, c3, s1);
      if (s1 != 0.0) {
        s1 = qd::quick_two_sum(s1, c4, s2);
      } else {
        s0 = qd::quick_two_sum(s0, c4, s1);
      }
    }
  }

  c0 = s0;
  c1 = s1;
  c2 = s2;
}

inline void three_sum(double &a, double &b, double &c) {
  double t1, t2, t3;
  t1 = qd::two_sum(a, b, t2);
  a = qd::two_sum(c, t1, t3);
  b = qd::two_sum(t2, t3, c);
}

inline double quick_three_accum(double &a, double &b, double c) {
  double s;
  bool za;
  bool zb;

  s = qd::two_sum(b, c, b);
  s = qd::two_sum(a, s, a);

  za = (a != 0.0);
  zb = (b != 0.0);

  if (za && zb) return s;

  if (!zb) {
    b = a;
    a = s;
  } else {
    a = s;
  }

  return 0.0;
}

inline td_real from_qd_truncate(const qd_real &a) {
  double x0 = a[0];
  double x1 = a[1];
  double x2 = a[2];
  double x3 = a[3];
  renorm(x0, x1, x2, x3);
  return td_real(x0, x1, x2);
}

inline qd_real to_qd_conversion(const td_real &a) {
  return qd_real(a[0], a[1], a[2], 0.0);
}

}  // namespace td

inline td_real::td_real(const dd_real &dd) {
  x[0] = dd._hi();
  x[1] = dd._lo();
  x[2] = 0.0;
}

inline td_real::td_real(const qd_real &qd) {
  *this = td::from_qd_truncate(qd);
}

inline td_real &td_real::operator=(double a) {
  x[0] = a;
  x[1] = 0.0;
  x[2] = 0.0;
  return *this;
}

inline td_real &td_real::operator=(const dd_real &a) {
  x[0] = a._hi();
  x[1] = a._lo();
  x[2] = 0.0;
  return *this;
}

inline td_real &td_real::operator=(const qd_real &a) {
  *this = td::from_qd_truncate(a);
  return *this;
}

inline bool td_real::is_zero() const {
  return x[0] == 0.0 && x[1] == 0.0 && x[2] == 0.0;
}

inline bool td_real::is_one() const {
  return x[0] == 1.0 && x[1] == 0.0 && x[2] == 0.0;
}

inline bool td_real::is_positive() const {
  return x[0] > 0.0;
}

inline bool td_real::is_negative() const {
  return x[0] < 0.0;
}

/* Sloppy addition: deterministic, component-wise.
   Satisfies Cray-style error bound.
   Cost: 4 TwoSum + 2 add + renorm4 = 41 flops. */
inline td_real td_real::sloppy_add(const td_real &a, const td_real &b) {
  double s0, s1, s2, t0, t1, t2;

  s0 = qd::two_sum(a[0], b[0], t0);
  s1 = qd::two_sum(a[1], b[1], t1);
  s2 = qd::two_sum(a[2], b[2], t2);

  s1 = qd::two_sum(s1, t0, t0);
  s2 += (t0 + t1);
  t0 = t2;

  td::renorm(s0, s1, s2, t0);
  return td_real(s0, s1, s2);
}

/* IEEE addition: merge-based, data-dependent.
   Satisfies IEEE-style error bound.
   Cost: 55--68 flops (data-dependent). */
inline td_real td_real::ieee_add(const td_real &a, const td_real &b) {
  int i = 0;
  int j = 0;
  int k = 0;
  double s;
  double t;
  double u;
  double v;
  double x[4] = {0.0, 0.0, 0.0, 0.0};

  if (std::abs(a[i]) > std::abs(b[j])) u = a[i++];
  else u = b[j++];

  if (i >= 3) v = b[j++];
  else if (j >= 3) v = a[i++];
  else if (std::abs(a[i]) > std::abs(b[j])) v = a[i++];
  else v = b[j++];

  u = qd::quick_two_sum(u, v, v);

  while (k < 3) {
    if (i >= 3 && j >= 3) {
      x[k] = u;
      if (k < 2) x[++k] = v;
      else x[3] += v;
      break;
    }

    if (i >= 3) t = b[j++];
    else if (j >= 3) t = a[i++];
    else if (std::abs(a[i]) > std::abs(b[j])) t = a[i++];
    else t = b[j++];

    s = td::quick_three_accum(u, v, t);
    if (s != 0.0) x[k++] = s;
  }

  for (k = i; k < 3; k++) x[3] += a[k];
  for (k = j; k < 3; k++) x[3] += b[k];

  td::renorm(x[0], x[1], x[2], x[3]);
  return td_real(x[0], x[1], x[2]);
}

inline td_real operator+(const td_real &a, double b) {
  double c0, c1, c2, e;
  c0 = qd::two_sum(a[0], b, e);
  c1 = qd::two_sum(a[1], e, e);
  c2 = qd::two_sum(a[2], e, e);
  td::renorm(c0, c1, c2, e);
  return td_real(c0, c1, c2);
}

inline td_real operator+(double a, const td_real &b) {
  return b + a;
}

inline td_real operator+(const td_real &a, const dd_real &b) {
  return a + td_real(b);
}

inline td_real operator+(const dd_real &a, const td_real &b) {
  return td_real(a) + b;
}

inline qd_real operator+(const td_real &a, const qd_real &b) {
  return td::to_qd_conversion(a) + b;
}

inline qd_real operator+(const qd_real &a, const td_real &b) {
  return a + td::to_qd_conversion(b);
}

inline td_real operator+(const td_real &a, const td_real &b) {
#ifndef QD_IEEE_ADD
  return td_real::sloppy_add(a, b);
#else
  return td_real::ieee_add(a, b);
#endif
}

inline td_real &td_real::operator+=(double a) {
  *this = *this + a;
  return *this;
}

inline td_real &td_real::operator+=(const dd_real &a) {
  *this = *this + a;
  return *this;
}

inline td_real &td_real::operator+=(const qd_real &a) {
  *this = td::from_qd_truncate(td::to_qd_conversion(*this) + a);
  return *this;
}

inline td_real &td_real::operator+=(const td_real &a) {
  *this = *this + a;
  return *this;
}

inline td_real td_real::operator-() const {
  return td_real(-x[0], -x[1], -x[2]);
}

inline td_real operator-(const td_real &a, double b) {
  double c0, c1, c2, e;
  c0 = qd::two_diff(a[0], b, e);
  c1 = qd::two_sum(a[1], e, e);
  c2 = qd::two_sum(a[2], e, e);
  td::renorm(c0, c1, c2, e);
  return td_real(c0, c1, c2);
}

inline td_real operator-(double a, const td_real &b) {
  return td_real(a) - b;
}

inline td_real operator-(const td_real &a, const dd_real &b) {
  return a - td_real(b);
}

inline td_real operator-(const dd_real &a, const td_real &b) {
  return td_real(a) - b;
}

inline qd_real operator-(const td_real &a, const qd_real &b) {
  return td::to_qd_conversion(a) - b;
}

inline qd_real operator-(const qd_real &a, const td_real &b) {
  return a - td::to_qd_conversion(b);
}

inline td_real operator-(const td_real &a, const td_real &b) {
#ifndef QD_IEEE_ADD
  /* Sloppy subtraction: deterministic, component-wise.
     Cost: 3 TwoDiff + 1 TwoSum + 2 add + renorm4 = 41 flops. */
  double s0, s1, s2, t0, t1, t2;
  s0 = qd::two_diff(a[0], b[0], t0);
  s1 = qd::two_diff(a[1], b[1], t1);
  s2 = qd::two_diff(a[2], b[2], t2);
  s1 = qd::two_sum(s1, t0, t0);
  s2 += (t0 + t1);
  t0 = t2;
  td::renorm(s0, s1, s2, t0);
  return td_real(s0, s1, s2);
#else
  /* Accurate subtraction: uses ThreeSum for full error tracking.
     Cost: 3 TwoDiff + 1 TwoSum + 1 ThreeSum + 1 add + renorm4 = 58 flops. */
  double s0, s1, s2, t0, t1, t2;
  s0 = qd::two_diff(a[0], b[0], t0);
  s1 = qd::two_diff(a[1], b[1], t1);
  s2 = qd::two_diff(a[2], b[2], t2);
  s1 = qd::two_sum(s1, t0, t0);
  td::three_sum(s2, t0, t1);
  t0 += t2;
  td::renorm(s0, s1, s2, t0);
  return td_real(s0, s1, s2);
#endif
}

inline td_real &td_real::operator-=(double a) {
  *this = *this - a;
  return *this;
}

inline td_real &td_real::operator-=(const dd_real &a) {
  *this = *this - a;
  return *this;
}

inline td_real &td_real::operator-=(const qd_real &a) {
  *this = td::from_qd_truncate(td::to_qd_conversion(*this) - a);
  return *this;
}

inline td_real &td_real::operator-=(const td_real &a) {
  *this = *this - a;
  return *this;
}

inline td_real mul_pwr2(const td_real &a, double d) {
  return td_real(a[0] * d, a[1] * d, a[2] * d);
}

inline td_real operator*(const td_real &a, double b) {
  double p0, p1, p2, q0, q1, q2;
  p0 = qd::two_prod(a[0], b, q0);
  p1 = qd::two_prod(a[1], b, q1);
  p2 = qd::two_prod(a[2], b, q2);
  p1 = qd::two_sum(p1, q0, q0);
  p2 = qd::two_sum(p2, q1, q1);
  q0 += q1 + q2;
  td::renorm(p0, p1, p2, q0);
  return td_real(p0, p1, p2);
}

inline td_real operator*(double a, const td_real &b) {
  return b * a;
}

inline td_real operator*(const td_real &a, const dd_real &b) {
  return a * td_real(b);
}

inline td_real operator*(const dd_real &a, const td_real &b) {
  return td_real(a) * b;
}

inline qd_real operator*(const td_real &a, const qd_real &b) {
  return td::to_qd_conversion(a) * b;
}

inline qd_real operator*(const qd_real &a, const td_real &b) {
  return a * td::to_qd_conversion(b);
}

inline td_real operator*(const td_real &a, const td_real &b) {
  double p0, p1, p2, p3, p4, p5;
  double q0, q1, q2, q3, q4, q5;
  double t0, t1, s0, s1, s2;

  p0 = qd::two_prod(a[0], b[0], q0);
  p1 = qd::two_prod(a[0], b[1], q1);
  p2 = qd::two_prod(a[1], b[0], q2);
  p3 = qd::two_prod(a[0], b[2], q3);
  p4 = qd::two_prod(a[1], b[1], q4);
  p5 = qd::two_prod(a[2], b[0], q5);

  td::three_sum(p1, p2, q0);
  td::three_sum(p2, q1, q2);
  td::three_sum(p3, p4, p5);

  s0 = qd::two_sum(p2, p3, t0);
  s1 = qd::two_sum(q1, p4, t1);
  s2 = q2 + p5;
  s1 = qd::two_sum(s1, t0, t0);
  s2 += t0 + t1;

  s1 += a[1] * b[2] + a[2] * b[1] + q0 + q3 + q4 + q5;
  td::renorm(p0, p1, s0, s1, s2);
  return td_real(p0, p1, s0);
}

inline td_real sqr(const td_real &a) {
  return a * a;
}

inline td_real &td_real::operator*=(double a) {
  *this = *this * a;
  return *this;
}

inline td_real &td_real::operator*=(const dd_real &a) {
  *this = *this * a;
  return *this;
}

inline td_real &td_real::operator*=(const qd_real &a) {
  *this = td::from_qd_truncate(td::to_qd_conversion(*this) * a);
  return *this;
}

inline td_real &td_real::operator*=(const td_real &a) {
  *this = *this * a;
  return *this;
}

inline td_real operator/(const td_real &a, double b) {
  double t0, t1, q0, q1, q2;
  td_real r;

  if (b == 0.0) {
    td_real::error("(td_real::operator/): Division by zero.");
    return td_real::_nan;
  }

  const bool rescale = td::div_needs_rescale(a[0]);
  const td_real aa = rescale ? mul_pwr2(a, 0x1p-53) : a;

  q0 = aa[0] / b;
  t0 = qd::two_prod(q0, b, t1);
  r = aa - td_real(t0, t1, 0.0);

  q1 = r[0] / b;
  t0 = qd::two_prod(q1, b, t1);
  r -= td_real(t0, t1, 0.0);

  q2 = r[0] / b;

  td::renorm(q0, q1, q2);
  td_real result(q0, q1, q2);
  return rescale ? mul_pwr2(result, 0x1p+53) : result;
}

inline td_real operator/(double a, const td_real &b) {
  return td_real(a) / b;
}

inline td_real operator/(const td_real &a, const dd_real &b) {
  return a / td_real(b);
}

inline td_real operator/(const dd_real &a, const td_real &b) {
  return td_real(a) / b;
}

inline qd_real operator/(const td_real &a, const qd_real &b) {
  return td::to_qd_conversion(a) / b;
}

inline qd_real operator/(const qd_real &a, const td_real &b) {
  return a / td::to_qd_conversion(b);
}

/* Accurate division: uses full TD*double and TD-TD subtraction for residuals.
   Cost: 3 div + 2 x (TD*double + TD-TD sub) + renorm3
       = 3 div + 6 TP + 2*(29 + sub) + 6,
   where sub = 41 (sloppy, default) or 58 (accurate, QD_IEEE_ADD).
   Default: 3 div + 6TP + 146.
   FMA: 3 div + 158.  No FMA: 3 div + 248. */
inline td_real td_real::accurate_div(const td_real &a, const td_real &b) {
  double q0, q1, q2;
  td_real r;

  if (b.is_zero()) {
    td_real::error("(td_real::operator/): Division by zero.");
    return td_real::_nan;
  }

  const bool rescale = td::div_needs_rescale(a[0]);
  const td_real aa = rescale ? mul_pwr2(a, 0x1p-53) : a;

  q0 = aa[0] / b[0];
  r = aa - b * q0;

  q1 = r[0] / b[0];
  r -= b * q1;

  q2 = r[0] / b[0];

  td::renorm(q0, q1, q2);
  td_real result(q0, q1, q2);
  return rescale ? mul_pwr2(result, 0x1p+53) : result;
}

/* Sloppy division: uses cheap scalar residual instead of full TD-TD sub.
   Cost: 3 div + 2 x TD*double + 2 x (TwoDiff + ~4 scalar) + 1 add + renorm3
       = 3 div + 6 TP + 58 + 10 + 9 + 1 + 6 = 3 div + 6 TP + 84.
   FMA: 3 div + 96.  No FMA: 3 div + 186. */
inline td_real td_real::sloppy_div(const td_real &a, const td_real &b) {
  double q0, q1, q2;
  double s0, s1, t0;

  if (b.is_zero()) {
    td_real::error("(td_real::operator/): Division by zero.");
    return td_real::_nan;
  }

  const bool rescale = td::div_needs_rescale(a[0]);
  const td_real aa = rescale ? mul_pwr2(a, 0x1p-53) : a;

  q0 = aa[0] / b[0];

  /* Residual r = aa - b*q0 (sloppy: TwoDiff for leading term,
     plain subtraction for lower terms). */
  td_real p = b * q0;
  s0 = qd::two_diff(aa[0], p[0], t0);
  s1 = (aa[1] - p[1]) + (aa[2] - p[2]) + t0;

  q1 = s0 / b[0];

  /* Second residual r -= b*q1 (sloppy). */
  p = b * q1;
  s0 = qd::two_diff(s0, p[0], t0);
  s1 += t0 - p[1] - p[2];

  q2 = (s0 + s1) / b[0];

  td::renorm(q0, q1, q2);
  td_real result(q0, q1, q2);
  return rescale ? mul_pwr2(result, 0x1p+53) : result;
}

inline td_real operator/(const td_real &a, const td_real &b) {
#ifdef QD_SLOPPY_DIV
  return td_real::sloppy_div(a, b);
#else
  return td_real::accurate_div(a, b);
#endif
}

inline td_real &td_real::operator/=(double a) {
  *this = *this / a;
  return *this;
}

inline td_real &td_real::operator/=(const dd_real &a) {
  *this = *this / a;
  return *this;
}

inline td_real &td_real::operator/=(const qd_real &a) {
  *this = td::from_qd_truncate(td::to_qd_conversion(*this) / a);
  return *this;
}

inline td_real &td_real::operator/=(const td_real &a) {
  *this = *this / a;
  return *this;
}

inline dd_real &dd_real::operator+=(const td_real &a) {
  *this += to_dd_real(a);
  return *this;
}

inline dd_real &dd_real::operator-=(const td_real &a) {
  *this -= to_dd_real(a);
  return *this;
}

inline dd_real &dd_real::operator*=(const td_real &a) {
  *this *= to_dd_real(a);
  return *this;
}

inline dd_real &dd_real::operator/=(const td_real &a) {
  *this /= to_dd_real(a);
  return *this;
}

inline qd_real &qd_real::operator+=(const td_real &a) {
  *this += td::to_qd_conversion(a);
  return *this;
}

inline qd_real &qd_real::operator-=(const td_real &a) {
  *this -= td::to_qd_conversion(a);
  return *this;
}

inline qd_real &qd_real::operator*=(const td_real &a) {
  *this *= td::to_qd_conversion(a);
  return *this;
}

inline qd_real &qd_real::operator/=(const td_real &a) {
  *this /= td::to_qd_conversion(a);
  return *this;
}

inline td_real td_real::sqr(double d) {
  return ::sqr(td_real(d));
}

inline td_real td_real::sqrt(double d) {
  return ::sqrt(td_real(d));
}

inline td_real td_real::operator^(int n) const {
  return npwr(*this, n);
}

inline td_real pow(const td_real &a, int n) {
  return npwr(a, n);
}

inline td_real abs(const td_real &a) {
  return (a[0] < 0.0) ? -a : a;
}

inline td_real fabs(const td_real &a) {
  return abs(a);
}

inline td_real ldexp(const td_real &a, int n) {
  return td_real(std::ldexp(a[0], n), std::ldexp(a[1], n), std::ldexp(a[2], n));
}

inline bool operator==(const td_real &a, double b) {
  return a[0] == b && a[1] == 0.0 && a[2] == 0.0;
}

inline bool operator==(double a, const td_real &b) {
  return b == a;
}

inline bool operator==(const td_real &a, const dd_real &b) {
  return a == td_real(b);
}

inline bool operator==(const dd_real &a, const td_real &b) {
  return td_real(a) == b;
}

inline bool operator==(const td_real &a, const qd_real &b) {
  return td::to_qd_conversion(a) == b;
}

inline bool operator==(const qd_real &a, const td_real &b) {
  return a == td::to_qd_conversion(b);
}

inline bool operator==(const td_real &a, const td_real &b) {
  return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}

inline bool operator<(const td_real &a, double b) {
  return a < td_real(b);
}

inline bool operator<(double a, const td_real &b) {
  return td_real(a) < b;
}

inline bool operator<(const td_real &a, const dd_real &b) {
  return a < td_real(b);
}

inline bool operator<(const dd_real &a, const td_real &b) {
  return td_real(a) < b;
}

inline bool operator<(const td_real &a, const qd_real &b) {
  return td::to_qd_conversion(a) < b;
}

inline bool operator<(const qd_real &a, const td_real &b) {
  return a < td::to_qd_conversion(b);
}

inline bool operator<(const td_real &a, const td_real &b) {
  if (a[0] != b[0]) return a[0] < b[0];
  if (a[1] != b[1]) return a[1] < b[1];
  return a[2] < b[2];
}

inline bool operator>(const td_real &a, double b) {
  return td_real(b) < a;
}

inline bool operator>(double a, const td_real &b) {
  return b < td_real(a);
}

inline bool operator>(const td_real &a, const dd_real &b) {
  return td_real(b) < a;
}

inline bool operator>(const dd_real &a, const td_real &b) {
  return b < td_real(a);
}

inline bool operator>(const td_real &a, const qd_real &b) {
  return b < a;
}

inline bool operator>(const qd_real &a, const td_real &b) {
  return b < a;
}

inline bool operator>(const td_real &a, const td_real &b) {
  return b < a;
}

inline bool operator<=(const td_real &a, double b) {
  return !(a > b);
}

inline bool operator<=(double a, const td_real &b) {
  return !(a > b);
}

inline bool operator<=(const td_real &a, const dd_real &b) {
  return !(a > b);
}

inline bool operator<=(const dd_real &a, const td_real &b) {
  return !(a > b);
}

inline bool operator<=(const td_real &a, const qd_real &b) {
  return !(a > b);
}

inline bool operator<=(const qd_real &a, const td_real &b) {
  return !(a > b);
}

inline bool operator<=(const td_real &a, const td_real &b) {
  return !(a > b);
}

inline bool operator>=(const td_real &a, double b) {
  return !(a < b);
}

inline bool operator>=(double a, const td_real &b) {
  return !(a < b);
}

inline bool operator>=(const td_real &a, const dd_real &b) {
  return !(a < b);
}

inline bool operator>=(const dd_real &a, const td_real &b) {
  return !(a < b);
}

inline bool operator>=(const td_real &a, const qd_real &b) {
  return !(a < b);
}

inline bool operator>=(const qd_real &a, const td_real &b) {
  return !(a < b);
}

inline bool operator>=(const td_real &a, const td_real &b) {
  return !(a < b);
}

inline bool operator!=(const td_real &a, double b) {
  return !(a == b);
}

inline bool operator!=(double a, const td_real &b) {
  return !(a == b);
}

inline bool operator!=(const td_real &a, const dd_real &b) {
  return !(a == b);
}

inline bool operator!=(const dd_real &a, const td_real &b) {
  return !(a == b);
}

inline bool operator!=(const td_real &a, const qd_real &b) {
  return !(a == b);
}

inline bool operator!=(const qd_real &a, const td_real &b) {
  return !(a == b);
}

inline bool operator!=(const td_real &a, const td_real &b) {
  return !(a == b);
}

inline dd_real to_dd_real(const td_real &a) {
  dd_real result(a[0], a[1]);
  result += a[2];
  return result;
}

inline td_real to_td_real(const dd_real &a) {
  return td_real(a);
}

inline td_real to_td_real(const qd_real &a) {
  return td::from_qd_truncate(a);
}

inline qd_real to_qd_real(const td_real &a) {
  return td::to_qd_conversion(a);
}

inline double to_double(const td_real &a) {
  return a[0];
}

inline int to_int(const td_real &a) {
  return static_cast<int>(a[0]);
}

#endif /* _QD_TD_INLINE_H */
