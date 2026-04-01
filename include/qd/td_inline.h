/*
 * include/td_inline.h
 *
 * Thin inline wrappers for mixed-mode triple-double arithmetic.
 * This file is included after dd_real.h, qd_real.h, and td_real.h
 * are fully declared, so it can bridge the three public types
 * without introducing circular header dependencies.
 */
#ifndef _QD_TD_INLINE_H
#define _QD_TD_INLINE_H

#include <qd/inline.h>

#ifndef QD_INLINE
#define inline
#endif

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
  *this += to_qd_real(a);
  return *this;
}

inline qd_real &qd_real::operator-=(const td_real &a) {
  *this -= to_qd_real(a);
  return *this;
}

inline qd_real &qd_real::operator*=(const td_real &a) {
  *this *= to_qd_real(a);
  return *this;
}

inline qd_real &qd_real::operator/=(const td_real &a) {
  *this /= to_qd_real(a);
  return *this;
}

inline td_real &td_real::operator+=(const dd_real &a) {
  *this = *this + a;
  return *this;
}

inline td_real &td_real::operator+=(const qd_real &a) {
  *this = *this + a;
  return *this;
}

inline td_real &td_real::operator-=(const dd_real &a) {
  *this = *this - a;
  return *this;
}

inline td_real &td_real::operator-=(const qd_real &a) {
  *this = *this - a;
  return *this;
}

inline td_real &td_real::operator*=(const dd_real &a) {
  *this = *this * a;
  return *this;
}

inline td_real &td_real::operator*=(const qd_real &a) {
  *this = *this * a;
  return *this;
}

inline td_real &td_real::operator/=(const dd_real &a) {
  *this = *this / a;
  return *this;
}

inline td_real &td_real::operator/=(const qd_real &a) {
  *this = *this / a;
  return *this;
}

inline qd_real operator+(const td_real &a, const qd_real &b) {
  return to_qd_real(a) + b;
}

inline qd_real operator+(const qd_real &a, const td_real &b) {
  return a + to_qd_real(b);
}

inline qd_real operator-(const td_real &a, const qd_real &b) {
  return to_qd_real(a) - b;
}

inline qd_real operator-(const qd_real &a, const td_real &b) {
  return a - to_qd_real(b);
}

inline qd_real operator*(const td_real &a, const qd_real &b) {
  return to_qd_real(a) * b;
}

inline qd_real operator*(const qd_real &a, const td_real &b) {
  return a * to_qd_real(b);
}

inline qd_real operator/(const td_real &a, const qd_real &b) {
  return to_qd_real(a) / b;
}

inline qd_real operator/(const qd_real &a, const td_real &b) {
  return a / to_qd_real(b);
}

inline bool operator==(const td_real &a, const qd_real &b) {
  return to_qd_real(a) == b;
}

inline bool operator==(const qd_real &a, const td_real &b) {
  return a == to_qd_real(b);
}

inline bool operator!=(const td_real &a, const qd_real &b) {
  return to_qd_real(a) != b;
}

inline bool operator!=(const qd_real &a, const td_real &b) {
  return a != to_qd_real(b);
}

inline bool operator<(const td_real &a, const qd_real &b) {
  return to_qd_real(a) < b;
}

inline bool operator<(const qd_real &a, const td_real &b) {
  return a < to_qd_real(b);
}

inline bool operator>(const td_real &a, const qd_real &b) {
  return to_qd_real(a) > b;
}

inline bool operator>(const qd_real &a, const td_real &b) {
  return a > to_qd_real(b);
}

inline bool operator<=(const td_real &a, const qd_real &b) {
  return to_qd_real(a) <= b;
}

inline bool operator<=(const qd_real &a, const td_real &b) {
  return a <= to_qd_real(b);
}

inline bool operator>=(const td_real &a, const qd_real &b) {
  return to_qd_real(a) >= b;
}

inline bool operator>=(const qd_real &a, const td_real &b) {
  return a >= to_qd_real(b);
}

#endif /* _QD_TD_INLINE_H */
