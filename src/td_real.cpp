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

}  // namespace

const td_real td_real::_nan = td_real(qd::_d_nan, qd::_d_nan, qd::_d_nan);
const td_real td_real::_inf = td_real(qd::_d_inf, qd::_d_inf, qd::_d_inf);
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

td_real operator+(const td_real &a, const td_real &b) {
  return td_real::ieee_add(a, b);
}

td_real &td_real::operator+=(double a) {
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

td_real &td_real::operator/=(const td_real &a) {
  *this = *this / a;
  return *this;
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

bool operator==(const td_real &a, const td_real &b) {
  return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}

bool operator!=(const td_real &a, double b) {
  return !(a == b);
}

bool operator!=(double a, const td_real &b) {
  return !(a == b);
}

bool operator!=(const td_real &a, const td_real &b) {
  return !(a == b);
}

bool operator<(const td_real &a, double b) {
  return a < td_real(b);
}

bool operator<(double a, const td_real &b) {
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

bool operator>(const td_real &a, double b) {
  return td_real(b) < a;
}

bool operator>(double a, const td_real &b) {
  return b < td_real(a);
}

bool operator>(const td_real &a, const td_real &b) {
  return b < a;
}

bool operator<=(const td_real &a, double b) {
  return !(a > b);
}

bool operator<=(double a, const td_real &b) {
  return !(a > b);
}

bool operator<=(const td_real &a, const td_real &b) {
  return !(a > b);
}

bool operator>=(const td_real &a, double b) {
  return !(a < b);
}

bool operator>=(double a, const td_real &b) {
  return !(a < b);
}

bool operator>=(const td_real &a, const td_real &b) {
  return !(a < b);
}

double to_double(const td_real &a) {
  return a[0];
}

int to_int(const td_real &a) {
  return static_cast<int>(a[0]);
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
