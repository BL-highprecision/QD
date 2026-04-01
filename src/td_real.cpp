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

#ifndef QD_INLINE
#include <qd/td_inline.h>
#endif

using std::cerr;
using std::endl;
using std::ios_base;
using std::istream;
using std::ostream;
using std::string;

namespace {

inline bool td_real_sqrt_needs_rescale(double a_hi) {
  return std::fabs(a_hi) > 0x1p+969;
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

  td::renorm(x0, x1, x2);
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

td_real &td_real::operator=(const char *s) {
  if (td_real::read(s, *this)) {
    td_real::error("(td_real::operator=): INPUT ERROR.");
    *this = td_real::_nan;
  }
  return *this;
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

td_real pow(const td_real &a, const td_real &b) {
  return exp(b * log(a));
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
