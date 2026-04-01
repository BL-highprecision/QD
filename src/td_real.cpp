/*
 * src/td_real.cpp
 *
 * Triple-double precision arithmetic.
 */
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>

#include "config.h"
#include <qd/td_real.h>
#include "util.h"

#include <qd/bits.h>
#include <qd/inline.h>

using std::cerr;
using std::endl;
using std::ios_base;
using std::istream;
using std::ostream;
using std::string;

namespace {

inline bool td_real_div_needs_rescale(double a_hi) {
  return std::fabs(a_hi) > 0x1p+969;
}

inline bool td_real_sqrt_needs_rescale(double a_hi) {
  return std::fabs(a_hi) > 0x1p+969;
}

inline void renorm(double &c0, double &c1, double &c2) {
  double s0;
  double s1;
  double s2 = 0.0;

  if (QD_ISINF(c0)) {
    return;
  }

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

  if (QD_ISINF(c0)) {
    return;
  }

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

  if (QD_ISINF(c0)) {
    return;
  }

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
  double t1;
  double t2;
  double t3;

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

  if (za && zb) {
    return s;
  }

  if (!zb) {
    b = a;
    a = s;
  } else {
    a = s;
  }

  return 0.0;
}

inline void round_string_td(char *s, int precision, int *offset) {
  int i;

  if (precision > 0 && s[precision] >= '5') {
    s[precision - 1]++;

    i = precision - 1;
    while (i > 0 && s[i] > '9') {
      s[i] -= 10;
      s[--i]++;
    }
  }

  if (s[0] > '9') {
    for (i = precision; i >= 1; i--) {
      s[i + 1] = s[i];
    }
    s[0] = '1';
    s[1] = '0';
    (*offset)++;
    precision++;
  }

  s[precision] = 0;
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

inline td_real signed_zero(double sign_source) {
  return td_real(std::copysign(0.0, sign_source));
}

inline td_real td_nint(const td_real &a) {
  double x0 = qd::nint(a[0]);
  double x1 = 0.0;
  double x2 = 0.0;

  if (x0 == a[0]) {
    x1 = qd::nint(a[1]);
    if (x1 == a[1]) {
      x2 = qd::nint(a[2]);
    } else if (std::abs(x1 - a[1]) == 0.5 && a[2] < 0.0) {
      x1 -= 1.0;
    }
  } else if (std::abs(x0 - a[0]) == 0.5 && a[1] < 0.0) {
    x0 -= 1.0;
  }

  renorm(x0, x1, x2);
  return td_real(x0, x1, x2);
}

static const int td_exp_squares = 12;
static const double td_exp_k = 4096.0;
static const double td_exp_inv_k = 1.0 / td_exp_k;
static const td_real td_pi16(1.963495408493620697e-01,
                             7.654042494670957545e-18,
                             -1.871731131073962291e-34);

static const double td_sin_table[4][3] = {
  {1.950903220161282758e-01, -7.991079068461731263e-18,  6.184627002422071324e-34},
  {3.826834323650897818e-01, -1.005077269646158761e-17, -2.060531630280669539e-34},
  {5.555702330196021776e-01,  4.709410940561676821e-17, -2.064052038368292118e-33},
  {7.071067811865475727e-01, -4.833646656726456726e-17,  2.069337654349706787e-33}
};

static const double td_cos_table[4][3] = {
  {9.807852804032304306e-01, 1.854693999782500573e-17, -1.069656444553075966e-33},
  {9.238795325112867385e-01, 1.764504708433667706e-17, -5.044253732158682026e-34},
  {8.314696123025452357e-01, 1.407385698472802389e-18,  4.695131538398083001e-35},
  {7.071067811865475727e-01, -4.833646656726456726e-17,  2.069337654349706787e-33}
};

/* exp(r) - 1 for a tiny reduced argument r. */
inline td_real expm1_small(const td_real &a) {
  td_real term = a;
  td_real sum = a;
  double n = 1.0;
  const td_real thresh = td_real(td_real::_eps * td_exp_inv_k);

  do {
    n += 1.0;
    term *= a;
    term /= n;
    sum += term;
  } while (abs(term) > thresh);

  return sum;
}

inline td_real sin_taylor(const td_real &a) {
  if (a.is_zero()) {
    return signed_zero(a[0]);
  }

  const td_real thresh = abs(a) * td_real(td_real::_eps * 0.5);
  td_real x = -sqr(a);
  td_real term = a;
  td_real sum = a;
  double n = 1.0;

  do {
    term *= x;
    term /= (n + 1.0) * (n + 2.0);
    sum += term;
    n += 2.0;
  } while (abs(term) > thresh);

  return sum;
}

inline td_real cos_taylor(const td_real &a) {
  if (a.is_zero()) {
    return 1.0;
  }

  const td_real thresh = td_real(td_real::_eps * 0.5);
  td_real x = -sqr(a);
  td_real term = 1.0;
  td_real sum = 1.0;
  double n = 0.0;

  do {
    term *= x;
    term /= (n + 1.0) * (n + 2.0);
    sum += term;
    n += 2.0;
  } while (abs(term) > thresh);

  return sum;
}

inline void sincos_taylor(const td_real &a, td_real &sin_a, td_real &cos_a) {
  if (a.is_zero()) {
    sin_a = signed_zero(a[0]);
    cos_a = 1.0;
    return;
  }

  sin_a = sin_taylor(a);
  cos_a = sqrt(1.0 - sqr(sin_a));
}

/* Reduce to t with |t| <= pi/32 while tracking pi/2 and pi/16 sectors. */
inline void reduce_trig_arg(const td_real &a, td_real &t, int &j, int &k) {
  td_real z = td_nint(a / td_real::_2pi);
  td_real r = a - td_real::_2pi * z;

  double q = std::floor(r[0] / td_real::_pi2[0] + 0.5);
  t = r - td_real::_pi2 * q;
  j = static_cast<int>(q);
  while (j > 2) j -= 4;
  while (j < -2) j += 4;

  q = std::floor(t[0] / td_pi16[0] + 0.5);
  t -= td_pi16 * q;
  k = static_cast<int>(q);
}

}  // namespace

const td_real td_real::_nan = td_real(qd::_d_nan, qd::_d_nan, qd::_d_nan);
const td_real td_real::_inf = td_real(qd::_d_inf, qd::_d_inf, qd::_d_inf);
const td_real td_real::_2pi = td_real(
    6.283185307179586232e+00, 2.449293598294706414e-16, -5.989539619436679332e-33);
const td_real td_real::_pi = td_real(
    3.141592653589793116e+00, 1.224646799147353207e-16, -2.994769809718339666e-33);
const td_real td_real::_3pi4 = td_real(
    2.356194490192344837e+00, 9.1848509936051484375e-17, 3.9168984647504003225e-33);
const td_real td_real::_pi2 = td_real(
    1.570796326794896558e+00, 6.123233995736766036e-17, -1.497384904859169833e-33);
const td_real td_real::_pi4 = td_real(
    7.853981633974482790e-01, 3.061616997868383018e-17, -7.486924524295849165e-34);
const td_real td_real::_e = td_real(
    2.718281828459045091e+00, 1.445646891729250158e-16, -2.127717108038176765e-33);
const td_real td_real::_log2 = td_real(
    6.931471805599452862e-01, 2.319046813846299558e-17, 5.707708438416212066e-34);
const td_real td_real::_log10 = td_real(
    2.302585092994045901e+00, -2.170756223382249351e-16, -9.984262454465776570e-33);
const double td_real::_eps = 5.47382212626881668e-48; /* 2^-157 */
const double td_real::_min_normalized = 1.80519437586482958e-276; /* 2^(-1022 + 2*53) */
const td_real td_real::_max = td_real(
    1.79769313486231570815e+308,
    9.97920154767359795037e+291,
    5.53956966280111259858e+275);
const td_real td_real::_safe_max = td_real(
    1.7976931080746007281e+308,
    9.97920154767359795037e+291,
    5.53956966280111259858e+275);
const int td_real::_ndigits = 47;

void td_real::error(const char *msg) {
  if (td_suppress_error_messages) {
    return;
  }
  if (msg) {
    cerr << "ERROR " << msg << endl;
  }
}

td_real::td_real(const dd_real &dd) {
  x[0] = dd._hi();
  x[1] = dd._lo();
  x[2] = 0.0;
}

td_real::td_real(const qd_real &qd) {
  *this = from_qd_truncate(qd);
}

td_real::td_real(const char *s) {
  if (td_real::read(s, *this)) {
    td_real::error("(td_real::td_real): INPUT ERROR.");
    *this = td_real::_nan;
  }
}

td_real &td_real::operator=(double a) {
  x[0] = a;
  x[1] = 0.0;
  x[2] = 0.0;
  return *this;
}

td_real &td_real::operator=(const dd_real &a) {
  x[0] = a._hi();
  x[1] = a._lo();
  x[2] = 0.0;
  return *this;
}

td_real &td_real::operator=(const qd_real &a) {
  *this = from_qd_truncate(a);
  return *this;
}

td_real &td_real::operator=(const char *s) {
  if (td_real::read(s, *this)) {
    td_real::error("(td_real::operator=): INPUT ERROR.");
    *this = td_real::_nan;
  }
  return *this;
}

bool td_real::is_zero() const {
  return x[0] == 0.0 && x[1] == 0.0 && x[2] == 0.0;
}

bool td_real::is_one() const {
  return x[0] == 1.0 && x[1] == 0.0 && x[2] == 0.0;
}

bool td_real::is_positive() const {
  return x[0] > 0.0;
}

bool td_real::is_negative() const {
  return x[0] < 0.0;
}

td_real td_real::ieee_add(const td_real &a, const td_real &b) {
  int i = 0;
  int j = 0;
  int k = 0;
  double s;
  double t;
  double u;
  double v;
  double x[4] = {0.0, 0.0, 0.0, 0.0};

  if (std::abs(a[i]) > std::abs(b[j])) {
    u = a[i++];
  } else {
    u = b[j++];
  }

  if (i >= 3) {
    v = b[j++];
  } else if (j >= 3) {
    v = a[i++];
  } else if (std::abs(a[i]) > std::abs(b[j])) {
    v = a[i++];
  } else {
    v = b[j++];
  }

  u = qd::quick_two_sum(u, v, v);

  while (k < 3) {
    if (i >= 3 && j >= 3) {
      x[k] = u;
      if (k < 2) {
        x[++k] = v;
      } else {
        x[3] += v;
      }
      break;
    }

    if (i >= 3) {
      t = b[j++];
    } else if (j >= 3) {
      t = a[i++];
    } else if (std::abs(a[i]) > std::abs(b[j])) {
      t = a[i++];
    } else {
      t = b[j++];
    }

    s = quick_three_accum(u, v, t);
    if (s != 0.0) {
      x[k++] = s;
    }
  }

  for (k = i; k < 3; k++) {
    x[3] += a[k];
  }
  for (k = j; k < 3; k++) {
    x[3] += b[k];
  }

  renorm(x[0], x[1], x[2], x[3]);
  return td_real(x[0], x[1], x[2]);
}

td_real operator+(const td_real &a, double b) {
  double c0;
  double c1;
  double c2;
  double e;

  c0 = qd::two_sum(a[0], b, e);
  c1 = qd::two_sum(a[1], e, e);
  c2 = qd::two_sum(a[2], e, e);
  renorm(c0, c1, c2, e);
  return td_real(c0, c1, c2);
}

td_real operator+(double a, const td_real &b) {
  return b + a;
}

td_real operator+(const td_real &a, const dd_real &b) {
  return a + td_real(b);
}

td_real operator+(const dd_real &a, const td_real &b) {
  return td_real(a) + b;
}

td_real operator+(const td_real &a, const td_real &b) {
  return td_real::ieee_add(a, b);
}

td_real &td_real::operator+=(double a) {
  *this = *this + a;
  return *this;
}

td_real &td_real::operator+=(const dd_real &a) {
  *this = *this + a;
  return *this;
}

td_real &td_real::operator+=(const qd_real &a) {
  *this = *this + a;
  return *this;
}

td_real &td_real::operator+=(const td_real &a) {
  *this = *this + a;
  return *this;
}

td_real operator-(const td_real &a, double b) {
  double c0;
  double c1;
  double c2;
  double e;

  c0 = qd::two_diff(a[0], b, e);
  c1 = qd::two_sum(a[1], e, e);
  c2 = qd::two_sum(a[2], e, e);
  renorm(c0, c1, c2, e);
  return td_real(c0, c1, c2);
}

td_real operator-(double a, const td_real &b) {
  return td_real(a) - b;
}

td_real operator-(const td_real &a, const dd_real &b) {
  return a - td_real(b);
}

td_real operator-(const dd_real &a, const td_real &b) {
  return td_real(a) - b;
}

td_real operator-(const td_real &a, const td_real &b) {
  double s0;
  double s1;
  double s2;
  double t0;
  double t1;
  double t2;

  s0 = qd::two_diff(a[0], b[0], t0);
  s1 = qd::two_diff(a[1], b[1], t1);
  s2 = qd::two_diff(a[2], b[2], t2);

  s1 = qd::two_sum(s1, t0, t0);
  three_sum(s2, t0, t1);
  t0 += t2;

  renorm(s0, s1, s2, t0);
  return td_real(s0, s1, s2);
}

td_real &td_real::operator-=(double a) {
  *this = *this - a;
  return *this;
}

td_real &td_real::operator-=(const dd_real &a) {
  *this = *this - a;
  return *this;
}

td_real &td_real::operator-=(const qd_real &a) {
  *this = *this - a;
  return *this;
}

td_real &td_real::operator-=(const td_real &a) {
  *this = *this - a;
  return *this;
}

td_real td_real::operator-() const {
  return td_real(-x[0], -x[1], -x[2]);
}

td_real operator*(const td_real &a, double b) {
  double p0;
  double p1;
  double p2;
  double q0;
  double q1;
  double q2;

  p0 = qd::two_prod(a[0], b, q0);
  p1 = qd::two_prod(a[1], b, q1);
  p2 = qd::two_prod(a[2], b, q2);

  p1 = qd::two_sum(p1, q0, q0);
  p2 = qd::two_sum(p2, q1, q1);
  q0 += q1 + q2;

  renorm(p0, p1, p2, q0);
  return td_real(p0, p1, p2);
}

td_real operator*(double a, const td_real &b) {
  return b * a;
}

td_real operator*(const td_real &a, const dd_real &b) {
  return a * td_real(b);
}

td_real operator*(const dd_real &a, const td_real &b) {
  return td_real(a) * b;
}

td_real operator*(const td_real &a, const td_real &b) {
  double p0;
  double p1;
  double p2;
  double p3;
  double p4;
  double p5;
  double q0;
  double q1;
  double q2;
  double q3;
  double q4;
  double q5;
  double t0;
  double t1;
  double s0;
  double s1;
  double s2;

  p0 = qd::two_prod(a[0], b[0], q0);
  p1 = qd::two_prod(a[0], b[1], q1);
  p2 = qd::two_prod(a[1], b[0], q2);
  p3 = qd::two_prod(a[0], b[2], q3);
  p4 = qd::two_prod(a[1], b[1], q4);
  p5 = qd::two_prod(a[2], b[0], q5);

  three_sum(p1, p2, q0);
  three_sum(p2, q1, q2);
  three_sum(p3, p4, p5);

  s0 = qd::two_sum(p2, p3, t0);
  s1 = qd::two_sum(q1, p4, t1);
  s2 = q2 + p5;
  s1 = qd::two_sum(s1, t0, t0);
  s2 += t0 + t1;

  s1 += a[1] * b[2] + a[2] * b[1] + q0 + q3 + q4 + q5;
  renorm(p0, p1, s0, s1, s2);
  return td_real(p0, p1, s0);
}

td_real &td_real::operator*=(double a) {
  *this = *this * a;
  return *this;
}

td_real &td_real::operator*=(const dd_real &a) {
  *this = *this * a;
  return *this;
}

td_real &td_real::operator*=(const qd_real &a) {
  *this = *this * a;
  return *this;
}

td_real &td_real::operator*=(const td_real &a) {
  *this = *this * a;
  return *this;
}

td_real operator/(const td_real &a, double b) {
  double t0;
  double t1;
  double q0;
  double q1;
  double q2;
  td_real r;

  if (b == 0.0) {
    td_real::error("(td_real::operator/): Division by zero.");
    return td_real::_nan;
  }

  const bool rescale = td_real_div_needs_rescale(a[0]);
  const td_real aa = rescale ? mul_pwr2(a, 0x1p-53) : a;

  q0 = aa[0] / b;
  t0 = qd::two_prod(q0, b, t1);
  r = aa - td_real(t0, t1, 0.0);

  q1 = r[0] / b;
  t0 = qd::two_prod(q1, b, t1);
  r -= td_real(t0, t1, 0.0);

  q2 = r[0] / b;

  renorm(q0, q1, q2);
  td_real result(q0, q1, q2);
  return rescale ? mul_pwr2(result, 0x1p+53) : result;
}

td_real operator/(double a, const td_real &b) {
  return td_real(a) / b;
}

td_real operator/(const td_real &a, const dd_real &b) {
  return a / td_real(b);
}

td_real operator/(const dd_real &a, const td_real &b) {
  return td_real(a) / b;
}

td_real operator/(const td_real &a, const td_real &b) {
  double q0;
  double q1;
  double q2;
  td_real r;

  if (b.is_zero()) {
    td_real::error("(td_real::operator/): Division by zero.");
    return td_real::_nan;
  }

  const bool rescale = td_real_div_needs_rescale(a[0]);
  const td_real aa = rescale ? mul_pwr2(a, 0x1p-53) : a;

  q0 = aa[0] / b[0];
  r = aa - b * q0;

  q1 = r[0] / b[0];
  r -= b * q1;

  q2 = r[0] / b[0];

  renorm(q0, q1, q2);
  td_real result(q0, q1, q2);
  return rescale ? mul_pwr2(result, 0x1p+53) : result;
}

td_real &td_real::operator/=(double a) {
  *this = *this / a;
  return *this;
}

td_real &td_real::operator/=(const dd_real &a) {
  *this = *this / a;
  return *this;
}

td_real &td_real::operator/=(const qd_real &a) {
  *this = *this / a;
  return *this;
}

td_real &td_real::operator/=(const td_real &a) {
  *this = *this / a;
  return *this;
}

dd_real &dd_real::operator+=(const td_real &a) {
  *this += to_dd_real(a);
  return *this;
}

dd_real &dd_real::operator-=(const td_real &a) {
  *this -= to_dd_real(a);
  return *this;
}

dd_real &dd_real::operator*=(const td_real &a) {
  *this *= to_dd_real(a);
  return *this;
}

dd_real &dd_real::operator/=(const td_real &a) {
  *this /= to_dd_real(a);
  return *this;
}

qd_real &qd_real::operator+=(const td_real &a) {
  *this += to_qd_real(a);
  return *this;
}

qd_real &qd_real::operator-=(const td_real &a) {
  *this -= to_qd_real(a);
  return *this;
}

qd_real &qd_real::operator*=(const td_real &a) {
  *this *= to_qd_real(a);
  return *this;
}

qd_real &qd_real::operator/=(const td_real &a) {
  *this /= to_qd_real(a);
  return *this;
}

qd_real operator+(const td_real &a, const qd_real &b) {
  return to_qd_real(a) + b;
}

qd_real operator+(const qd_real &a, const td_real &b) {
  return a + to_qd_real(b);
}

qd_real operator-(const td_real &a, const qd_real &b) {
  return to_qd_real(a) - b;
}

qd_real operator-(const qd_real &a, const td_real &b) {
  return a - to_qd_real(b);
}

qd_real operator*(const td_real &a, const qd_real &b) {
  return to_qd_real(a) * b;
}

qd_real operator*(const qd_real &a, const td_real &b) {
  return a * to_qd_real(b);
}

qd_real operator/(const td_real &a, const qd_real &b) {
  return to_qd_real(a) / b;
}

qd_real operator/(const qd_real &a, const td_real &b) {
  return a / to_qd_real(b);
}

td_real td_real::operator^(int n) const {
  return npwr(*this, n);
}

td_real td_real::sqr(double d) {
  return ::sqr(td_real(d));
}

td_real sqr(const td_real &a) {
  return a * a;
}

td_real sqrt(const td_real &a) {
  if (a.is_zero()) {
    return 0.0;
  }

  if (a.is_negative()) {
    td_real::error("(td_real::sqrt): Negative argument.");
    return td_real::_nan;
  }

  if (td_real_sqrt_needs_rescale(a[0])) {
    return mul_pwr2(sqrt(mul_pwr2(a, 0.25)), 2.0);
  }

  td_real x(std::sqrt(a[0]));
  x += (a - sqr(x)) / (2.0 * x);
  x += (a - sqr(x)) / (2.0 * x);
  return x;
}

td_real td_real::sqrt(double d) {
  return ::sqrt(td_real(d));
}

td_real nroot(const td_real &a, int n) {
  /* Strategy: use Newton iteration to solve

        1 / (x^n) - a = 0

     for x = a^(-1/n), then return 1 / x.
  */
  if (n <= 0) {
    td_real::error("(td_real::nroot): N must be positive.");
    return td_real::_nan;
  }

  if (n % 2 == 0 && a.is_negative()) {
    td_real::error("(td_real::nroot): Negative argument.");
    return td_real::_nan;
  }

  if (n == 1) {
    return a;
  }
  if (n == 2) {
    return sqrt(a);
  }
  if (a.is_zero()) {
    return td_real(0.0);
  }

  td_real r = abs(a);
  td_real x = std::exp(-std::log(r[0]) / n);
  double dbl_n = static_cast<double>(n);

  x += x * (1.0 - r * npwr(x, n)) / dbl_n;
  x += x * (1.0 - r * npwr(x, n)) / dbl_n;
  x += x * (1.0 - r * npwr(x, n)) / dbl_n;

  if (a[0] < 0.0) {
    x = -x;
  }

  return 1.0 / x;
}

td_real npwr(const td_real &a, int n) {
  if (n == 0) {
    if (a.is_zero()) {
      td_real::error("(td_real::npwr): Invalid argument.");
      return td_real::_nan;
    }
    return 1.0;
  }

  td_real r = a;
  td_real s = 1.0;
  int N = std::abs(n);

  if (N > 1) {
    while (N > 0) {
      if (N % 2 == 1) {
        s *= r;
      }
      N /= 2;
      if (N > 0) {
        r = sqr(r);
      }
    }
  } else {
    s = r;
  }

  if (n < 0) {
    return 1.0 / s;
  }
  return s;
}

td_real pow(const td_real &a, int n) {
  return npwr(a, n);
}

td_real pow(const td_real &a, const td_real &b) {
  return exp(b * log(a));
}

td_real abs(const td_real &a) {
  return (a[0] < 0.0) ? -a : a;
}

td_real fabs(const td_real &a) {
  return abs(a);
}

td_real ldexp(const td_real &a, int n) {
  return td_real(std::ldexp(a[0], n), std::ldexp(a[1], n), std::ldexp(a[2], n));
}

td_real mul_pwr2(const td_real &a, double d) {
  return td_real(a[0] * d, a[1] * d, a[2] * d);
}

bool operator==(const td_real &a, double b) {
  return a[0] == b && a[1] == 0.0 && a[2] == 0.0;
}

bool operator==(double a, const td_real &b) {
  return b == a;
}

bool operator==(const td_real &a, const dd_real &b) {
  return a == td_real(b);
}

bool operator==(const dd_real &a, const td_real &b) {
  return td_real(a) == b;
}

bool operator==(const td_real &a, const td_real &b) {
  return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}

bool operator==(const td_real &a, const qd_real &b) {
  return to_qd_real(a) == b;
}

bool operator==(const qd_real &a, const td_real &b) {
  return a == to_qd_real(b);
}

bool operator!=(const td_real &a, double b) {
  return !(a == b);
}

bool operator!=(double a, const td_real &b) {
  return !(a == b);
}

bool operator!=(const td_real &a, const dd_real &b) {
  return !(a == b);
}

bool operator!=(const dd_real &a, const td_real &b) {
  return !(a == b);
}

bool operator!=(const td_real &a, const td_real &b) {
  return !(a == b);
}

bool operator!=(const td_real &a, const qd_real &b) {
  return !(a == b);
}

bool operator!=(const qd_real &a, const td_real &b) {
  return !(a == b);
}

bool operator<(const td_real &a, double b) {
  return a < td_real(b);
}

bool operator<(double a, const td_real &b) {
  return td_real(a) < b;
}

bool operator<(const td_real &a, const dd_real &b) {
  return a < td_real(b);
}

bool operator<(const dd_real &a, const td_real &b) {
  return td_real(a) < b;
}

bool operator<(const td_real &a, const td_real &b) {
  if (a[0] != b[0]) {
    return a[0] < b[0];
  }
  if (a[1] != b[1]) {
    return a[1] < b[1];
  }
  return a[2] < b[2];
}

bool operator<(const td_real &a, const qd_real &b) {
  return to_qd_real(a) < b;
}

bool operator<(const qd_real &a, const td_real &b) {
  return a < to_qd_real(b);
}

bool operator>(const td_real &a, double b) {
  return td_real(b) < a;
}

bool operator>(double a, const td_real &b) {
  return b < td_real(a);
}

bool operator>(const td_real &a, const dd_real &b) {
  return td_real(b) < a;
}

bool operator>(const dd_real &a, const td_real &b) {
  return b < td_real(a);
}

bool operator>(const td_real &a, const td_real &b) {
  return b < a;
}

bool operator>(const td_real &a, const qd_real &b) {
  return b < a;
}

bool operator>(const qd_real &a, const td_real &b) {
  return b < a;
}

bool operator<=(const td_real &a, double b) {
  return !(a > b);
}

bool operator<=(double a, const td_real &b) {
  return !(a > b);
}

bool operator<=(const td_real &a, const dd_real &b) {
  return !(a > b);
}

bool operator<=(const dd_real &a, const td_real &b) {
  return !(a > b);
}

bool operator<=(const td_real &a, const td_real &b) {
  return !(a > b);
}

bool operator<=(const td_real &a, const qd_real &b) {
  return !(a > b);
}

bool operator<=(const qd_real &a, const td_real &b) {
  return !(a > b);
}

bool operator>=(const td_real &a, double b) {
  return !(a < b);
}

bool operator>=(double a, const td_real &b) {
  return !(a < b);
}

bool operator>=(const td_real &a, const dd_real &b) {
  return !(a < b);
}

bool operator>=(const dd_real &a, const td_real &b) {
  return !(a < b);
}

bool operator>=(const td_real &a, const td_real &b) {
  return !(a < b);
}

bool operator>=(const td_real &a, const qd_real &b) {
  return !(a < b);
}

bool operator>=(const qd_real &a, const td_real &b) {
  return !(a < b);
}

dd_real to_dd_real(const td_real &a) {
  dd_real result(a[0], a[1]);
  result += a[2];
  return result;
}

td_real to_td_real(const dd_real &a) {
  return td_real(a);
}

td_real to_td_real(const qd_real &a) {
  return from_qd_truncate(a);
}

qd_real to_qd_real(const td_real &a) {
  return to_qd_conversion(a);
}

double to_double(const td_real &a) {
  return a[0];
}

int to_int(const td_real &a) {
  return static_cast<int>(a[0]);
}

td_real exp(const td_real &a) {
  if (a.isnan()) {
    return td_real::_nan;
  }
  if (a.is_zero()) {
    return 1.0;
  }
  if (a.isinf()) {
    return a.is_positive() ? td_real::_inf : 0.0;
  }
  if (a[0] <= -709.0) {
    return 0.0;
  }
  if (a[0] >= 709.0) {
    return td_real::_inf;
  }
  if (a.is_one()) {
    return td_real::_e;
  }

  double m = std::floor(a[0] / td_real::_log2[0] + 0.5);
  td_real r = mul_pwr2(a - td_real::_log2 * m, td_exp_inv_k);
  td_real s = expm1_small(r);

  for (int i = 0; i < td_exp_squares; i++) {
    s = mul_pwr2(s, 2.0) + sqr(s);
  }

  return ldexp(s + 1.0, static_cast<int>(m));
}

td_real log(const td_real &a) {
  if (a.isnan()) {
    return td_real::_nan;
  }
  if (a.is_zero()) {
    return -td_real::_inf;
  }
  if (a.is_negative()) {
    td_real::error("(td_real::log): Non-positive argument.");
    return td_real::_nan;
  }
  if (a.isinf()) {
    return td_real::_inf;
  }
  if (a.is_one()) {
    return 0.0;
  }

  int e;
  double m = std::frexp(a[0], &e);
  td_real x = td_real(std::log(m)) + td_real::_log2 * static_cast<double>(e);

  x = x + a * exp(-x) - 1.0;
  x = x + a * exp(-x) - 1.0;
  x = x + a * exp(-x) - 1.0;

  return x;
}

td_real log10(const td_real &a) {
  return log(a) / td_real::_log10;
}

td_real sin(const td_real &a) {
  td_real s, c;
  sincos(a, s, c);
  return s;
}

td_real cos(const td_real &a) {
  td_real s, c;
  sincos(a, s, c);
  return c;
}

td_real tan(const td_real &a) {
  td_real s, c;
  sincos(a, s, c);
  return s / c;
}

void sincos(const td_real &a, td_real &s, td_real &c) {
  if (a.isnan()) {
    s = c = td_real::_nan;
    return;
  }
  if (a.isinf()) {
    td_real::error("(td_real::sincos): Infinite argument.");
    s = c = td_real::_nan;
    return;
  }
  if (a.is_zero()) {
    s = signed_zero(a[0]);
    c = 1.0;
    return;
  }

  td_real t;
  int j, k;
  reduce_trig_arg(a, t, j, k);
  int abs_j = std::abs(j);
  int abs_k = std::abs(k);

  if (abs_k > 4) {
    td_real::error("(td_real::sincos): Cannot reduce modulo pi/16.");
    s = c = td_real::_nan;
    return;
  }

  td_real sin_t;
  td_real cos_t;
  td_real ss;
  td_real cc;
  sincos_taylor(t, sin_t, cos_t);

  if (abs_k == 0) {
    ss = sin_t;
    cc = cos_t;
  } else {
    td_real u(td_cos_table[abs_k - 1]);
    td_real v(td_sin_table[abs_k - 1]);

    if (k > 0) {
      ss = u * sin_t + v * cos_t;
      cc = u * cos_t - v * sin_t;
    } else {
      ss = u * sin_t - v * cos_t;
      cc = u * cos_t + v * sin_t;
    }
  }

  if (abs_j == 0) {
    s = ss;
    c = cc;
  } else if (j == 1) {
    s = cc;
    c = -ss;
  } else if (j == -1) {
    s = -cc;
    c = ss;
  } else {
    s = -ss;
    c = -cc;
  }
}

td_real asin(const td_real &a) {
  td_real abs_a = abs(a);

  if (a.isnan()) {
    return td_real::_nan;
  }
  if (abs_a > 1.0) {
    td_real::error("(td_real::asin): Argument out of domain.");
    return td_real::_nan;
  }
  if (abs_a.is_one()) {
    return a.is_positive() ? td_real::_pi2 : -td_real::_pi2;
  }

  return atan2(a, sqrt(1.0 - sqr(a)));
}

td_real acos(const td_real &a) {
  td_real abs_a = abs(a);

  if (a.isnan()) {
    return td_real::_nan;
  }
  if (abs_a > 1.0) {
    td_real::error("(td_real::acos): Argument out of domain.");
    return td_real::_nan;
  }
  if (abs_a.is_one()) {
    return a.is_positive() ? td_real(0.0) : td_real::_pi;
  }

  return atan2(sqrt(1.0 - sqr(a)), a);
}

td_real atan(const td_real &a) {
  return atan2(a, td_real(1.0));
}

td_real atan2(const td_real &y, const td_real &x) {
  if (x.isnan() || y.isnan()) {
    return td_real::_nan;
  }

  if (x.is_zero()) {
    if (y.is_zero()) {
      td_real::error("(td_real::atan2): Both arguments zero.");
      return td_real::_nan;
    }

    return std::signbit(y[0]) ? -td_real::_pi2 : td_real::_pi2;
  }

  if (y.is_zero()) {
    if (x.is_positive()) {
      return signed_zero(y[0]);
    }
    return std::signbit(y[0]) ? -td_real::_pi : td_real::_pi;
  }

  if (x == y) {
    return y.is_positive() ? td_real::_pi4 : -td_real::_3pi4;
  }
  if (x == -y) {
    return y.is_positive() ? td_real::_3pi4 : -td_real::_pi4;
  }

  td_real r = sqrt(sqr(x) + sqr(y));
  td_real xx = x / r;
  td_real yy = y / r;
  td_real z(std::atan2(to_double(y), to_double(x)));
  td_real sin_z;
  td_real cos_z;

  if (std::abs(xx[0]) > std::abs(yy[0])) {
    for (int i = 0; i < 3; i++) {
      sincos(z, sin_z, cos_z);
      z += (yy - sin_z) / cos_z;
    }
  } else {
    for (int i = 0; i < 3; i++) {
      sincos(z, sin_z, cos_z);
      z -= (xx - cos_z) / sin_z;
    }
  }

  return z;
}

td_real sinh(const td_real &a) {
  if (a.isnan()) {
    return td_real::_nan;
  }
  if (a.is_zero()) {
    return signed_zero(a[0]);
  }
  if (a.isinf()) {
    return a;
  }

  if (abs(a) > 0.05) {
    td_real ea = exp(a);
    return mul_pwr2(ea - (1.0 / ea), 0.5);
  }

  td_real s = a;
  td_real t = a;
  td_real r = sqr(a);
  double m = 1.0;
  td_real thresh = abs(a) * td_real(td_real::_eps);

  do {
    m += 2.0;
    t *= r;
    t /= (m - 1.0) * m;
    s += t;
  } while (abs(t) > thresh);

  return s;
}

td_real cosh(const td_real &a) {
  if (a.isnan()) {
    return td_real::_nan;
  }
  if (a.is_zero()) {
    return 1.0;
  }
  if (a.isinf()) {
    return td_real::_inf;
  }

  td_real ea = exp(a);
  return mul_pwr2(ea + (1.0 / ea), 0.5);
}

td_real tanh(const td_real &a) {
  if (a.isnan()) {
    return td_real::_nan;
  }
  if (a.is_zero()) {
    return signed_zero(a[0]);
  }
  if (a.isinf()) {
    return a.is_positive() ? td_real(1.0) : td_real(-1.0);
  }

  if (abs(a) > 0.05) {
    td_real ea = exp(a);
    td_real inv_ea = 1.0 / ea;
    return (ea - inv_ea) / (ea + inv_ea);
  }

  td_real s = sinh(a);
  td_real c = sqrt(1.0 + sqr(s));
  return s / c;
}

void sincosh(const td_real &a, td_real &s, td_real &c) {
  if (std::abs(to_double(a)) <= 0.05) {
    s = sinh(a);
    c = sqrt(1.0 + sqr(s));
  } else {
    td_real ea = exp(a);
    td_real inv_ea = 1.0 / ea;
    s = mul_pwr2(ea - inv_ea, 0.5);
    c = mul_pwr2(ea + inv_ea, 0.5);
  }
}

td_real asinh(const td_real &a) {
  if (a.isnan()) {
    return td_real::_nan;
  }
  if (a.isinf()) {
    return a;
  }
  return log(a + sqrt(sqr(a) + 1.0));
}

td_real acosh(const td_real &a) {
  if (a.isnan()) {
    return td_real::_nan;
  }
  if (a < 1.0) {
    td_real::error("(td_real::acosh): Argument out of domain.");
    return td_real::_nan;
  }
  if (a.isinf()) {
    return td_real::_inf;
  }
  return log(a + sqrt(sqr(a) - 1.0));
}

td_real atanh(const td_real &a) {
  if (a.isnan()) {
    return td_real::_nan;
  }
  if (abs(a) >= 1.0) {
    td_real::error("(td_real::atanh): Argument out of domain.");
    return td_real::_nan;
  }
  return mul_pwr2(log((1.0 + a) / (1.0 - a)), 0.5);
}

td_real nint(const td_real &a) {
  return td_nint(a);
}

ostream &operator<<(ostream &os, const td_real &td) {
  bool showpos = (os.flags() & ios_base::showpos) != 0;
  bool uppercase = (os.flags() & ios_base::uppercase) != 0;
  return os << td.to_string(os.precision(), os.width(), os.flags(),
      showpos, uppercase, os.fill());
}

istream &operator>>(istream &s, td_real &a) {
  char str[255];
  s >> str;
  a = td_real(str);
  return s;
}

int td_real::read(const char *s, td_real &a) {
  const char *p = s;
  char ch;
  int sign = 0;
  int point = -1;
  int nd = 0;
  int e = 0;
  bool done = false;
  td_real r = 0.0;

  while (*p == ' ') {
    p++;
  }

  if (std::strcmp(p, "nan") == 0 || std::strcmp(p, "NAN") == 0) {
    a = td_real::_nan;
    return 0;
  }
  if (std::strcmp(p, "inf") == 0 || std::strcmp(p, "INF") == 0) {
    a = td_real::_inf;
    return 0;
  }
  if (std::strcmp(p, "+inf") == 0 || std::strcmp(p, "+INF") == 0) {
    a = td_real::_inf;
    return 0;
  }
  if (std::strcmp(p, "-inf") == 0 || std::strcmp(p, "-INF") == 0) {
    a = -td_real::_inf;
    return 0;
  }

  while (!done && (ch = *p) != '\0') {
    if (ch >= '0' && ch <= '9') {
      int d = ch - '0';
      r *= 10.0;
      r += static_cast<double>(d);
      nd++;
    } else {
      switch (ch) {
      case '.':
        if (point >= 0) {
          return -1;
        }
        point = nd;
        break;
      case '-':
      case '+':
        if (sign != 0 || nd > 0) {
          return -1;
        }
        sign = (ch == '-') ? -1 : 1;
        break;
      case 'E':
      case 'e':
        if (std::sscanf(p + 1, "%d", &e) != 1) {
          return -1;
        }
        done = true;
        break;
      case ' ':
        done = true;
        break;
      default:
        return -1;
      }
    }
    p++;
  }

  if (point >= 0) {
    e -= (nd - point);
  }

  if (e != 0) {
    r *= (td_real(10.0) ^ e);
  }

  a = (sign < 0) ? -r : r;
  return 0;
}

void td_real::to_digits(char *s, int &expn, int precision) const {
  int D = precision + 1;
  td_real r = abs(*this);
  int e;
  int i;
  int d;

  if (x[0] == 0.0) {
    expn = 0;
    for (i = 0; i < precision; i++) {
      s[i] = '0';
    }
    return;
  }

  e = static_cast<int>(std::floor(std::log10(std::abs(x[0]))));

  if (e < -300) {
    r *= td_real(10.0) ^ 300;
    r /= td_real(10.0) ^ (e + 300);
  } else if (e > 300) {
    r = ldexp(r, -53);
    r /= td_real(10.0) ^ e;
    r = ldexp(r, 53);
  } else {
    r /= td_real(10.0) ^ e;
  }

  if (r >= 10.0) {
    r /= 10.0;
    e++;
  } else if (r < 1.0) {
    r *= 10.0;
    e--;
  }

  if (r >= 10.0 || r < 1.0) {
    td_real::error("(td_real::to_digits): can't compute exponent.");
    return;
  }

  for (i = 0; i < D; i++) {
    d = static_cast<int>(r[0]);
    r -= d;
    r *= 10.0;
    s[i] = static_cast<char>(d + '0');
  }

  for (i = D - 1; i > 0; i--) {
    if (s[i] < '0') {
      s[i - 1]--;
      s[i] += 10;
    } else if (s[i] > '9') {
      s[i - 1]++;
      s[i] -= 10;
    }
  }

  if (s[0] <= '0') {
    td_real::error("(td_real::to_digits): non-positive leading digit.");
    return;
  }

  if (s[D - 1] >= '5') {
    s[D - 2]++;
    i = D - 2;
    while (i > 0 && s[i] > '9') {
      s[i] -= 10;
      s[--i]++;
    }
  }

  if (s[0] > '9') {
    e++;
    for (i = precision; i >= 2; i--) {
      s[i] = s[i - 1];
    }
    s[0] = '1';
    s[1] = '0';
  }

  s[precision] = 0;
  expn = e;
}

void td_real::write(char *s, int len, int precision, bool showpos, bool uppercase) const {
  string str = to_string(precision, 0, ios_base::scientific, showpos, uppercase);
  std::strncpy(s, str.c_str(), len - 1);
  s[len - 1] = 0;
}

string td_real::to_string(int precision, int width, ios_base::fmtflags fmt,
    bool showpos, bool uppercase, char fill) const {
  string s;
  bool fixed = (fmt & ios_base::fixed) != 0;
  bool sgn = true;
  int i;
  int e = 0;

  if (isinf()) {
    if (*this < 0.0) {
      s += '-';
    } else if (showpos) {
      s += '+';
    } else {
      sgn = false;
    }
    s += uppercase ? "INF" : "inf";
  } else if (isnan()) {
    s = uppercase ? "NAN" : "nan";
    sgn = false;
  } else {
    if (*this < 0.0) {
      s += '-';
    } else if (showpos) {
      s += '+';
    } else {
      sgn = false;
    }

    if (*this == 0.0) {
      s += '0';
      if (precision > 0) {
        s += '.';
        s.append(precision, '0');
      }
    } else {
      int off = fixed ? (1 + static_cast<int>(std::floor(std::log10(std::abs(x[0]))))) : 1;
      int d = precision + off;
      int d_with_extra = fixed ? std::max(96, d) : d;

      if (fixed && precision == 0 && abs(*this) < 1.0) {
        s += (abs(*this) >= 0.5) ? '1' : '0';
        return s;
      }

      if (fixed && d <= 0) {
        s += '0';
        if (precision > 0) {
          s += '.';
          s.append(precision, '0');
        }
      } else {
        char *t;
        int j;

        if (fixed) {
          t = new char[d_with_extra + 1];
          to_digits(t, e, d_with_extra);
        } else {
          t = new char[d + 1];
          to_digits(t, e, d);
        }

        off = e + 1;

        if (fixed) {
          round_string_td(t, d, &off);
          if (off > 0) {
            for (i = 0; i < off; i++) {
              s += t[i];
            }
            if (precision > 0) {
              s += '.';
              for (j = 0; j < precision; j++, i++) {
                s += t[i];
              }
            }
          } else {
            s += "0.";
            if (off < 0) {
              s.append(-off, '0');
            }
            for (i = 0; i < d; i++) {
              s += t[i];
            }
          }
        } else {
          s += t[0];
          if (precision > 0) {
            s += '.';
          }
          for (i = 1; i <= precision; i++) {
            s += t[i];
          }
        }

        delete[] t;
      }
    }

    if (!fixed && !isinf()) {
      s += uppercase ? 'E' : 'e';
      append_expn(s, e);
    }
  }

  int len = s.length();
  if (len < width) {
    int delta = width - len;
    if (fmt & ios_base::internal) {
      if (sgn) {
        s.insert(static_cast<string::size_type>(1), delta, fill);
      } else {
        s.insert(static_cast<string::size_type>(0), delta, fill);
      }
    } else if (fmt & ios_base::left) {
      s.append(delta, fill);
    } else {
      s.insert(static_cast<string::size_type>(0), delta, fill);
    }
  }

  return s;
}

bool td_suppress_error_messages = false;
