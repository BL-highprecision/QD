/*
 * include/td_real.h
 *
 * Triple-double precision (about 159 bits / 47 decimal digits)
 * floating point arithmetic package.
 */
#ifndef _QD_TD_REAL_H
#define _QD_TD_REAL_H

#include <cmath>
#include <iostream>
#include <limits>
#include <string>

#include <qd/fpu.h>
#include <qd/qd_config.h>
#include <qd/dd_real.h>
#include <qd/qd_real.h>

#ifdef isnan
#undef isnan
#endif

#ifdef isfinite
#undef isfinite
#endif

#ifdef isinf
#undef isinf
#endif

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

struct QD_API td_real {
  double x[3];

  td_real() {
    x[0] = 0.0;
    x[1] = 0.0;
    x[2] = 0.0;
  }

  td_real(double x0, double x1, double x2) {
    x[0] = x0;
    x[1] = x1;
    x[2] = x2;
  }

  explicit td_real(const double *xx) {
    x[0] = xx[0];
    x[1] = xx[1];
    x[2] = xx[2];
  }

  td_real(double d) {
    x[0] = d;
    x[1] = 0.0;
    x[2] = 0.0;
  }

  td_real(int i) {
    x[0] = static_cast<double>(i);
    x[1] = 0.0;
    x[2] = 0.0;
  }

  td_real(const dd_real &dd);
  explicit td_real(const qd_real &qd);
  td_real(const char *s);

  double operator[](int i) const { return x[i]; }
  double &operator[](int i) { return x[i]; }

  static void error(const char *msg);

  static const td_real _nan;
  static const td_real _inf;
  static const td_real _2pi;
  static const td_real _pi;
  static const td_real _3pi4;
  static const td_real _pi2;
  static const td_real _pi4;
  static const td_real _e;
  static const td_real _log2;
  static const td_real _log10;
  static const td_real _max;
  static const td_real _safe_max;

  static const double _eps;
  static const double _min_normalized;
  static const int _ndigits;

  bool isnan() const {
    return QD_ISNAN(x[0]) || QD_ISNAN(x[1]) || QD_ISNAN(x[2]);
  }
  bool isfinite() const { return QD_ISFINITE(x[0]); }
  bool isinf() const { return QD_ISINF(x[0]); }

  static td_real ieee_add(const td_real &a, const td_real &b);

  td_real &operator+=(double a);
  td_real &operator+=(const td_real &a);

  td_real &operator-=(double a);
  td_real &operator-=(const td_real &a);

  td_real operator-() const;

  td_real &operator*=(double a);
  td_real &operator*=(const td_real &a);

  td_real &operator/=(double a);
  td_real &operator/=(const td_real &a);

  td_real &operator=(double a);
  td_real &operator=(const dd_real &a);
  td_real &operator=(const qd_real &a);
  td_real &operator=(const char *s);

  td_real operator^(int n) const;

  static td_real sqr(double d);
  static td_real sqrt(double d);

  bool is_zero() const;
  bool is_one() const;
  bool is_positive() const;
  bool is_negative() const;

  void to_digits(char *s, int &expn, int precision = _ndigits) const;
  void write(char *s, int len, int precision = _ndigits,
      bool showpos = false, bool uppercase = false) const;
  std::string to_string(int precision = _ndigits, int width = 0,
      std::ios_base::fmtflags fmt = static_cast<std::ios_base::fmtflags>(0),
      bool showpos = false, bool uppercase = false, char fill = ' ') const;
  int read(const char *s, td_real &a);
};

namespace std {
  template <>
  class numeric_limits<td_real> : public numeric_limits<double> {
  public:
    inline static double epsilon() { return td_real::_eps; }
    inline static td_real max() { return td_real::_max; }
    inline static td_real safe_max() { return td_real::_safe_max; }
    inline static double min() { return td_real::_min_normalized; }
    static const int digits = 157;
    static const int digits10 = 47;
  };
}

QD_API inline bool isnan(const td_real &a) { return a.isnan(); }
QD_API inline bool isfinite(const td_real &a) { return a.isfinite(); }
QD_API inline bool isinf(const td_real &a) { return a.isinf(); }

QD_API td_real operator+(const td_real &a, double b);
QD_API td_real operator+(double a, const td_real &b);
QD_API td_real operator+(const td_real &a, const dd_real &b);
QD_API td_real operator+(const dd_real &a, const td_real &b);
QD_API td_real operator+(const td_real &a, const td_real &b);

QD_API td_real operator-(const td_real &a, double b);
QD_API td_real operator-(double a, const td_real &b);
QD_API td_real operator-(const td_real &a, const dd_real &b);
QD_API td_real operator-(const dd_real &a, const td_real &b);
QD_API td_real operator-(const td_real &a, const td_real &b);

QD_API td_real operator*(const td_real &a, double b);
QD_API td_real operator*(double a, const td_real &b);
QD_API td_real operator*(const td_real &a, const dd_real &b);
QD_API td_real operator*(const dd_real &a, const td_real &b);
QD_API td_real operator*(const td_real &a, const td_real &b);

QD_API td_real operator/(const td_real &a, double b);
QD_API td_real operator/(double a, const td_real &b);
QD_API td_real operator/(const td_real &a, const dd_real &b);
QD_API td_real operator/(const dd_real &a, const td_real &b);
QD_API td_real operator/(const td_real &a, const td_real &b);

QD_API td_real sqr(const td_real &a);
QD_API td_real sqrt(const td_real &a);
QD_API td_real nroot(const td_real &a, int n);
QD_API td_real npwr(const td_real &a, int n);
QD_API td_real pow(const td_real &a, int n);
QD_API td_real pow(const td_real &a, const td_real &b);

QD_API td_real abs(const td_real &a);
QD_API td_real fabs(const td_real &a);

QD_API td_real ldexp(const td_real &a, int n);
QD_API td_real mul_pwr2(const td_real &a, double d);

QD_API bool operator==(const td_real &a, double b);
QD_API bool operator==(double a, const td_real &b);
QD_API bool operator==(const td_real &a, const dd_real &b);
QD_API bool operator==(const dd_real &a, const td_real &b);
QD_API bool operator==(const td_real &a, const td_real &b);

QD_API bool operator!=(const td_real &a, double b);
QD_API bool operator!=(double a, const td_real &b);
QD_API bool operator!=(const td_real &a, const dd_real &b);
QD_API bool operator!=(const dd_real &a, const td_real &b);
QD_API bool operator!=(const td_real &a, const td_real &b);

QD_API bool operator<(const td_real &a, double b);
QD_API bool operator<(double a, const td_real &b);
QD_API bool operator<(const td_real &a, const dd_real &b);
QD_API bool operator<(const dd_real &a, const td_real &b);
QD_API bool operator<(const td_real &a, const td_real &b);

QD_API bool operator>(const td_real &a, double b);
QD_API bool operator>(double a, const td_real &b);
QD_API bool operator>(const td_real &a, const dd_real &b);
QD_API bool operator>(const dd_real &a, const td_real &b);
QD_API bool operator>(const td_real &a, const td_real &b);

QD_API bool operator<=(const td_real &a, double b);
QD_API bool operator<=(double a, const td_real &b);
QD_API bool operator<=(const td_real &a, const dd_real &b);
QD_API bool operator<=(const dd_real &a, const td_real &b);
QD_API bool operator<=(const td_real &a, const td_real &b);

QD_API bool operator>=(const td_real &a, double b);
QD_API bool operator>=(double a, const td_real &b);
QD_API bool operator>=(const td_real &a, const dd_real &b);
QD_API bool operator>=(const dd_real &a, const td_real &b);
QD_API bool operator>=(const td_real &a, const td_real &b);

QD_API dd_real to_dd_real(const td_real &a);
QD_API td_real to_td_real(const dd_real &a);
QD_API td_real to_td_real(const qd_real &a);
QD_API qd_real to_qd_real(const td_real &a);
QD_API double to_double(const td_real &a);
QD_API int to_int(const td_real &a);

QD_API td_real exp(const td_real &a);
QD_API td_real log(const td_real &a);
QD_API td_real log10(const td_real &a);

QD_API td_real sin(const td_real &a);
QD_API td_real cos(const td_real &a);
QD_API td_real tan(const td_real &a);
QD_API void sincos(const td_real &a, td_real &s, td_real &c);

QD_API td_real asin(const td_real &a);
QD_API td_real acos(const td_real &a);
QD_API td_real atan(const td_real &a);
QD_API td_real atan2(const td_real &y, const td_real &x);

QD_API td_real sinh(const td_real &a);
QD_API td_real cosh(const td_real &a);
QD_API td_real tanh(const td_real &a);
QD_API void sincosh(const td_real &a, td_real &s, td_real &c);

QD_API td_real asinh(const td_real &a);
QD_API td_real acosh(const td_real &a);
QD_API td_real atanh(const td_real &a);
QD_API td_real nint(const td_real &a);

QD_API std::ostream &operator<<(std::ostream &s, const td_real &a);
QD_API std::istream &operator>>(std::istream &s, td_real &a);

QD_API extern bool td_suppress_error_messages;

#endif /* _QD_TD_REAL_H */
