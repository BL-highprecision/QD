/*
 * tests/qd_test.cpp
 *
 * This work was supported by the Director, Office of Science, Division
 * of Mathematical, Information, and Computational Sciences of the
 * U.S. Department of Energy under contract number DE-AC03-76SF00098.
 *
 * Copyright (c) 2000-2001
 *
 * This contains some simple tests to sanity check the double-double
 * and quad-double library.
 */

#include <cstdlib>
#include <cmath>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <vector>
#include <qd/qd_real.h>
#include <qd/td_real.h>
#include <qd/inline.h>
#include <qd/fpu.h>

using std::cout;
using std::cerr;
using std::endl;

using std::abs;
using std::sqrt;
using std::strcmp;
using std::exit;

// Global flags passed to the main program.
static bool flag_test_dd = false;
static bool flag_test_td = false;
static bool flag_test_qd = false;
bool flag_verbose = false;

bool print_result(bool result) {
  if (result)
    cout << "Test passed." << endl;
  else
    cout << "Test FAILED." << endl;
  return result;
}

template <class T>
class TestSuite {
  static const int double_digits;
public:
  bool test1();
  bool test2();
  bool test3();
  bool test4();
  bool test5();
  bool test6();
  bool test7();
  bool test8();
  bool testall();
};

template <class T>
const int TestSuite<T>::double_digits = 6;

td_real polyeval(const td_real *c, int n, const td_real &x) {
  std::vector<qd_real> qc(n + 1);

  for (int i = 0; i <= n; i++) {
    qc[i] = to_qd_real(c[i]);
  }

  qd_real y = ::polyeval(qc.data(), n, to_qd_real(x));
  return to_td_real(y);
}

td_real polyroot(const td_real *c, int n, const td_real &x0,
    int max_iter = 64, double thresh = 0.0) {
  std::vector<qd_real> qc(n + 1);

  for (int i = 0; i <= n; i++) {
    qc[i] = to_qd_real(c[i]);
  }

  qd_real x = ::polyroot(qc.data(), n, to_qd_real(x0), max_iter, thresh);
  return to_td_real(x);
}

namespace {

qd_real td_to_qd(const td_real &a) {
  return qd_real(a[0]) + qd_real(a[1]) + qd_real(a[2]);
}

bool td_nonoverlap(double hi, double lo) {
  if (hi == 0.0) {
    return lo == 0.0;
  }
  return std::abs(lo) <= 0.5 * std::ldexp(std::abs(hi), -52);
}

bool td_is_normalized(const td_real &a) {
  return std::abs(a[0]) >= std::abs(a[1]) &&
         std::abs(a[1]) >= std::abs(a[2]) &&
         td_nonoverlap(a[0], a[1]) &&
         td_nonoverlap(a[1], a[2]);
}

double td_abs_error(const td_real &a, const qd_real &ref) {
  return to_double(abs(td_to_qd(a) - ref));
}

double td_scale(const qd_real &ref) {
  return std::max(1.0, std::abs(to_double(ref)));
}

bool td_check_close(const td_real &a, const qd_real &ref, double factor) {
  return td_abs_error(a, ref) <= factor * td_real::_eps * td_scale(ref);
}

// RAII guard for td_suppress_error_messages.
struct SuppressGuard {
  ~SuppressGuard() { td_suppress_error_messages = false; }
};

class TdTestSuite {
public:
  bool test9();
  bool test10();
  bool test11();
  bool test12();
  bool test13();
  bool test14();
  bool test15();
  bool test16();
  bool test17();
  bool test18();
  bool test19();
  bool test20();
  bool test21();
  bool testall();
};

bool TdTestSuite::test9() {
  cout << endl;
  cout << "Test 9.  (Normalization invariants after arithmetic)." << endl;

  td_real a("1.2345678901234567890123456789012345678901");
  td_real b("9.8765432109876543210987654321098765432109e-40");
  td_real c("-8.7654321098765432109876543210987654321098e20");

  td_real sum = a + c;
  td_real diff = a - b;
  td_real prod = a * c;
  td_real quot = c / a;
  td_real root = sqrt(td_real("2.0"));

  bool pass = td_is_normalized(sum) && td_is_normalized(diff) &&
      td_is_normalized(prod) && td_is_normalized(quot) && td_is_normalized(root);

  if (flag_verbose) {
    cout << "sum  = " << sum << endl;
    cout << "diff = " << diff << endl;
    cout << "prod = " << prod << endl;
    cout << "quot = " << quot << endl;
    cout << "root = " << root << endl;
  }

  return pass;
}

bool TdTestSuite::test10() {
  cout << endl;
  cout << "Test 10.  (Cancellation-sensitive addition / subtraction)." << endl;

  const char *sa = "1.0000000000000002220446049250313080847263336181640625";
  const char *sb = "1.00000000000000011102230246251565404236316680908203125";
  td_real a(sa);
  td_real b(sb);
  td_real diff = a - b;
  td_real back = diff + b;
  qd_real qdiff = qd_real(sa) - qd_real(sb);
  qd_real qback = qdiff + qd_real(sb);

  bool pass = td_check_close(diff, qdiff, 32.0) &&
      td_check_close(back, qback, 32.0);

  if (flag_verbose) {
    cout << "diff = " << diff << endl;
    cout << "back = " << back << endl;
    cout << "diff err = " << td_abs_error(diff, qdiff) / td_real::_eps << " eps" << endl;
    cout << "back err = " << td_abs_error(back, qback) / td_real::_eps << " eps" << endl;
  }

  return pass;
}

bool TdTestSuite::test11() {
  cout << endl;
  cout << "Test 11.  (Multiplication across mixed magnitudes)." << endl;

  const char *sa = "1.2345678901234567890123456789012345678901e150";
  const char *sb = "9.8765432109876543210987654321098765432109e-120";
  td_real a(sa);
  td_real b(sb);
  td_real prod = a * b;
  qd_real qprod = qd_real(sa) * qd_real(sb);

  if (flag_verbose) {
    cout << "prod = " << prod << endl;
    cout << "err  = " << td_abs_error(prod, qprod) / td_real::_eps << " eps" << endl;
  }

  return td_check_close(prod, qprod, 64.0);
}

bool TdTestSuite::test12() {
  cout << endl;
  cout << "Test 12.  (Division sanity checks)." << endl;

  const char *sa = "1.2345678901234567890123456789012345678901e120";
  const char *sb = "9.8765432109876543210987654321098765432109e30";
  td_real a(sa);
  td_real b(sb);
  td_real q = a / b;
  td_real thirds = td_real("1.0") / td_real("3.0");
  qd_real qq = qd_real(sa) / qd_real(sb);
  qd_real qthirds = qd_real(1.0) / qd_real(3.0);

  bool pass = td_check_close(q, qq, 64.0) &&
      td_check_close(thirds, qthirds, 64.0) &&
      td_check_close(q * b, qd_real(sa), 128.0);

  if (flag_verbose) {
    cout << "q      = " << q << endl;
    cout << "thirds = " << thirds << endl;
  }

  return pass;
}

bool TdTestSuite::test13() {
  cout << endl;
  cout << "Test 13.  (sqrt on exact squares and values near 1)." << endl;

  td_real square("15241578750190521");
  td_real exact = sqrt(square);
  td_real near_one("1.0000000000000000000000000000000000000000001");
  td_real near_root = sqrt(near_one);
  qd_real qexact("123456789");
  qd_real qnear = sqrt(qd_real("1.0000000000000000000000000000000000000000001"));

  bool pass = td_check_close(exact, qexact, 32.0) &&
      td_check_close(near_root, qnear, 64.0);

  if (flag_verbose) {
    cout << "exact sqrt = " << exact << endl;
    cout << "near sqrt  = " << near_root << endl;
  }

  return pass;
}

bool TdTestSuite::test14() {
  cout << endl;
  cout << "Test 14.  (Parse / format round trips)." << endl;

  const char *samples[] = {
    "3.1415926535897932384626433832795028841971693993751",
    "-2.7182818284590452353602874713526624977572470937000e-120",
    "9.9999999999999999999999999999999999999999999999999e200",
    "1.2345678901234567890123456789012345678901234567890e-200"
  };

  bool pass = true;
  for (int i = 0; i < 4; i++) {
    td_real a(samples[i]);
    std::ostringstream os;
    os << std::setprecision(td_real::_ndigits) << std::scientific << a;
    std::istringstream is(os.str());
    td_real b;
    is >> b;
    pass &= td_check_close(b, td_to_qd(a), 64.0);

    if (flag_verbose) {
      cout << samples[i] << endl;
      cout << " -> " << os.str() << endl;
    }
  }

  return pass;
}

bool TdTestSuite::test15() {
  cout << endl;
  cout << "Test 15.  (Transcendental identity checks)." << endl;

  td_real x("1.234567890123456789");
  td_real y("0.625");
  td_real a("0.3");
  td_real s;
  td_real c;
  sincos(y, s, c);

  bool pass = true;
  pass &= td_check_close(exp(log(x)), td_to_qd(x), 64.0);
  pass &= td_check_close(log(exp(a)), td_to_qd(a), 64.0);
  pass &= td_check_close(sqr(s) + sqr(c), qd_real(1.0), 96.0);
  pass &= td_check_close(tan(y), td_to_qd(s / c), 96.0);
  pass &= td_check_close(atan(tan(a)), td_to_qd(a), 96.0);
  pass &= td_check_close(asin(sin(a)), td_to_qd(a), 96.0);
  pass &= td_check_close(acos(cos(a)), td_to_qd(a), 96.0);
  pass &= td_check_close(atan2(s, c), td_to_qd(y), 96.0);
  pass &= td_check_close(sqr(cosh(a)) - sqr(sinh(a)), qd_real(1.0), 128.0);

  if (flag_verbose) {
    cout << "exp(log(x)) = " << exp(log(x)) << endl;
    cout << "sin^2+cos^2 = " << (sqr(s) + sqr(c)) << endl;
    cout << "atan2(s,c)  = " << atan2(s, c) << endl;
  }

  return pass;
}

bool TdTestSuite::test16() {
  cout << endl;
  cout << "Test 16.  (Random-value qd oracle comparison)." << endl;

  bool pass = true;
  std::srand(12345);

  for (int i = 0; i < 40; i++) {
    double u = (std::rand() / static_cast<double>(RAND_MAX)) * 2.0 - 1.0;
    double v = (std::rand() / static_cast<double>(RAND_MAX)) * 2.0 - 1.0;
    td_real x = td_real(u) + td_real(v) * td_real("1e-16");
    td_real p = abs(x) + td_real("0.25");

    pass &= td_check_close(sin(x), sin(td_to_qd(x)), 32.0);
    pass &= td_check_close(cos(x), cos(td_to_qd(x)), 32.0);
    pass &= td_check_close(exp(x), exp(td_to_qd(x)), 32.0);
    pass &= td_check_close(log(p), log(td_to_qd(p)), 32.0);
    pass &= td_check_close(tanh(x), tanh(td_to_qd(x)), 32.0);
    pass &= td_check_close(atan(x), atan(td_to_qd(x)), 32.0);
  }

  return pass;
}

bool TdTestSuite::test17() {
  cout << endl;
  cout << "Test 17.  (Boundary-value checks)." << endl;

  td_real root_max = sqrt(td_real::_max);
  td_real log_min = log(td_real(td_real::_min_normalized));
  td_real exp_hi = exp(td_real("800.0"));
  td_real exp_lo = exp(td_real("-800.0"));
  qd_real qroot_max = sqrt(to_qd_real(td_real::_max));
  qd_real qlog_min = log(qd_real(td_real::_min_normalized));

  bool pass = true;
  pass &= root_max.isfinite();
  pass &= td_check_close(root_max, qroot_max, 256.0);
  pass &= td_check_close(log_min, qlog_min, 128.0);
  pass &= exp_hi.isinf();
  pass &= exp_lo.is_zero();

  if (flag_verbose) {
    cout << "sqrt(max) = " << root_max << endl;
    cout << "log(min)  = " << log_min << endl;
  }

  return pass;
}

bool TdTestSuite::test18() {
  cout << endl;
  cout << "Test 18.  (Mixed-mode arithmetic and conversion round trips)." << endl;

  dd_real dd("1.234567890123456789012345678901");
  td_real td("9.876543210987654321098765432109e-10");
  qd_real qd("3.1415926535897932384626433832795028841971");

  td_real mix_add = td + dd;
  td_real mix_mul = dd * td;
  td_real mix_div = td / dd;
  qd_real qd_add = qd + td;
  qd_real qd_sub = td - qd;
  qd_real qd_mul = qd * td;
  qd_real qd_div = qd / (td + td_real("1.0"));
  td_real from_dd = to_td_real(dd);
  td_real from_qd = to_td_real(qd);
  dd_real dd_round = to_dd_real(from_dd);
  qd_real qd_round = to_qd_real(from_qd);
  dd_real dd_self("1.25");
  td_real td_self("1.25");
  qd_real qd_self("1.25");
  td_real td_half("0.5");
  qd_real qd_half("0.5");
  qd_real dd_add_ref = qd_real(dd) + qd_real("0.75");
  qd_real qd_self_ref("0.875");

  bool pass = true;
  pass &= td_check_close(mix_add, qd_real(dd) + td_to_qd(td), 64.0);
  pass &= td_check_close(mix_mul, qd_real(dd) * td_to_qd(td), 64.0);
  pass &= td_check_close(mix_div, td_to_qd(td) / qd_real(dd), 64.0);
  pass &= to_double(abs(qd_add - (qd + td_to_qd(td)))) < 64.0 * td_real::_eps * td_scale(qd_add);
  pass &= to_double(abs(qd_sub - (td_to_qd(td) - qd))) < 64.0 * td_real::_eps * td_scale(qd_sub);
  pass &= to_double(abs(qd_mul - (qd * td_to_qd(td)))) < 64.0 * td_real::_eps * td_scale(qd_mul);
  pass &= to_double(abs(qd_div - (qd / (td_to_qd(td) + qd_real(1.0))))) < 64.0 * td_real::_eps * td_scale(qd_div);
  pass &= abs(to_double(qd_real(dd_round) - qd_real(dd))) < 16.0 * dd_real::_eps;
  pass &= td_check_close(from_qd, qd, 32.0);
  pass &= to_double(abs(qd_round - qd)) < 64.0 * td_real::_eps * td_scale(qd);

  dd_self += td_half;
  dd_self *= td_half;
  td_self += dd;
  td_self -= qd_half;
  qd_self += td_half;
  qd_self *= td_half;

  pass &= abs(to_double(qd_real(dd_self) - qd_real("0.875"))) < 32.0 * dd_real::_eps;
  pass &= td_check_close(td_self, dd_add_ref, 64.0);
  pass &= to_double(abs(qd_self - qd_self_ref)) < 64.0 * td_real::_eps * td_scale(qd_self_ref);
  pass &= (td < qd);
  pass &= (qd > td);

  if (flag_verbose) {
    cout << "mix_add = " << mix_add << endl;
    cout << "mix_mul = " << mix_mul << endl;
    cout << "mix_div = " << mix_div << endl;
  }

  return pass;
}

bool TdTestSuite::test19() {
  cout << endl;
  cout << "Test 19.  (Special values and string robustness)." << endl;

  td_real nan_value("nan");
  td_real inf_value("inf");
  td_real neg_inf("-inf");
  td_real neg_one("-1.0");
  td_suppress_error_messages = true;
  SuppressGuard suppress_guard;
  td_real log_nan = log(neg_one);

  td_real a("12345.678901234567890123456789");
  std::ostringstream sci;
  std::ostringstream fixed;
  sci << std::setprecision(td_real::_ndigits) << std::scientific << std::uppercase << std::showpos << a;
  fixed << std::setprecision(30) << std::fixed << a;

  std::istringstream sci_in(sci.str());
  std::istringstream fixed_in(fixed.str());
  td_real sci_rt;
  td_real fixed_rt;
  sci_in >> sci_rt;
  fixed_in >> fixed_rt;

  bool pass = true;
  pass &= nan_value.isnan();
  pass &= inf_value.isinf();
  pass &= neg_inf.isinf() && neg_inf.is_negative();
  pass &= log_nan.isnan();
  pass &= td_check_close(sci_rt, td_to_qd(a), 64.0);
  pass &= td_check_close(fixed_rt, td_to_qd(a), 128.0);

  if (flag_verbose) {
    cout << "scientific = " << sci.str() << endl;
    cout << "fixed      = " << fixed.str() << endl;
  }

  return pass;
}

bool TdTestSuite::test20() {
  cout << endl;
  cout << "Test 20.  (Deterministic transcendental spot and edge cases)." << endl;

  td_real pos_zero(0.0);
  td_real neg_zero(-0.0);
  td_real near_one("0.999999999999999999999999999999999999999999");
  td_real near_mone("-0.999999999999999999999999999999999999999999");
  td_real near_pi2 = td_real::_pi2 - td_real("1e-20");
  td_real large_angle("123456.125");
  td_real x("0.125");
  td_real y("-0.75");
  td_real s;
  td_real c;
  td_real sh;
  td_real ch;

  sincos(near_pi2, s, c);
  sincosh(x, sh, ch);

  bool pass = true;
  pass &= td_check_close(log10(td_real("1000.0")), qd_real(3.0), 64.0);
  pass &= sin(pos_zero).is_zero() && !std::signbit(sin(pos_zero)[0]);
  pass &= sin(neg_zero).is_zero() && std::signbit(sin(neg_zero)[0]);
  pass &= cos(pos_zero).is_one();
  pass &= td_check_close(s, sin(td_to_qd(near_pi2)), 96.0);
  pass &= td_check_close(c, cos(td_to_qd(near_pi2)), 128.0);
  pass &= td_check_close(tan(near_pi2) * c, td_to_qd(s), 128.0);
  pass &= td_check_close(sin(large_angle), sin(td_to_qd(large_angle)), 768.0);
  pass &= td_check_close(cos(large_angle), cos(td_to_qd(large_angle)), 768.0);
  pass &= td_check_close(asin(near_one), asin(td_to_qd(near_one)), 128.0);
  pass &= td_check_close(acos(near_mone), acos(td_to_qd(near_mone)), 128.0);
  pass &= td_check_close(atan2(y, x), atan2(td_to_qd(y), td_to_qd(x)), 96.0);
  pass &= td_check_close(sh, sinh(td_to_qd(x)), 64.0);
  pass &= td_check_close(ch, cosh(td_to_qd(x)), 64.0);
  pass &= td_check_close(tanh(y), tanh(td_to_qd(y)), 64.0);
  pass &= td_check_close(asinh(y), asinh(td_to_qd(y)), 96.0);
  pass &= td_check_close(acosh(td_real("1.5")), acosh(qd_real("1.5")), 128.0);
  pass &= td_check_close(atanh(td_real("0.125")), atanh(qd_real("0.125")), 96.0);

  if (flag_verbose) {
    cout << "sin(pi/2 - 1e-20) = " << s << endl;
    cout << "cos(pi/2 - 1e-20) = " << c << endl;
    cout << "sin(large)        = " << sin(large_angle) << endl;
    cout << "atan2(y, x)       = " << atan2(y, x) << endl;
  }

  return pass;
}

bool TdTestSuite::test21() {
  cout << endl;
  cout << "Test 21.  (Randomized native transcendental regression checks)." << endl;

  bool pass = true;
  std::srand(24680);

  for (int i = 0; i < 48; i++) {
    double u = (std::rand() / static_cast<double>(RAND_MAX)) * 10.0 - 5.0;
    double v = (std::rand() / static_cast<double>(RAND_MAX)) * 10.0 - 5.0;
    double w = (std::rand() / static_cast<double>(RAND_MAX)) * 0.98 - 0.49;

    td_real x = td_real(u) + td_real(v) * td_real("1e-15");
    td_real positive = abs(x) + td_real("0.125");
    td_real unit = td_real(w) + td_real("1e-16");
    td_real angle = x * td_real::_pi4;
    td_real sx;
    td_real cx;

    sincos(angle, sx, cx);

    pass &= td_check_close(exp(x), exp(td_to_qd(x)), 64.0);
    pass &= td_check_close(log(positive), log(td_to_qd(positive)), 64.0);
    pass &= td_check_close(log10(positive), log10(td_to_qd(positive)), 96.0);
    pass &= td_check_close(sx, sin(td_to_qd(angle)), 96.0);
    pass &= td_check_close(cx, cos(td_to_qd(angle)), 96.0);
    pass &= td_check_close(tan(unit), tan(td_to_qd(unit)), 128.0);
    pass &= td_check_close(atan(x), atan(td_to_qd(x)), 64.0);
    pass &= td_check_close(atan2(x, positive), atan2(td_to_qd(x), td_to_qd(positive)), 96.0);
    pass &= td_check_close(asin(unit), asin(td_to_qd(unit)), 128.0);
    pass &= td_check_close(acos(unit), acos(td_to_qd(unit)), 128.0);
    pass &= td_check_close(sinh(unit), sinh(td_to_qd(unit)), 64.0);
    pass &= td_check_close(cosh(unit), cosh(td_to_qd(unit)), 64.0);
    pass &= td_check_close(tanh(unit), tanh(td_to_qd(unit)), 64.0);
  }

  return pass;
}

bool TdTestSuite::testall() {
  bool pass = true;
  pass &= print_result(test9());
  pass &= print_result(test10());
  pass &= print_result(test11());
  pass &= print_result(test12());
  pass &= print_result(test13());
  pass &= print_result(test14());
  pass &= print_result(test15());
  pass &= print_result(test16());
  pass &= print_result(test17());
  pass &= print_result(test18());
  pass &= print_result(test19());
  pass &= print_result(test20());
  pass &= print_result(test21());
  return pass;
}

}  // namespace

/* Test 1.   Polynomial Evaluation / Polynomial Solving */
template <class T>
bool TestSuite<T>::test1() {
  cout << endl;
  cout << "Test 1.  (Polynomial)." << endl;

  static const int n = 8;
  std::vector<T> c(n);
  T x, y;

  for (int i = 0; i < n; i++)
    c[i] = static_cast<double>(i+1);

  x = polyroot(c.data(), n-1, T(0.0));
  y = polyeval(c.data(), n-1, x);

  if (flag_verbose) {
    cout.precision(T::_ndigits);
    cout << "Root Found:  x  = " << x << endl;
    cout << "           p(x) = " << y << endl;
  }

  return (to_double(y) < 4.0 * T::_eps);
}

/* Test 2.  Machin's Formula for Pi. */
template <class T>
bool TestSuite<T>::test2() {

  cout << endl;
  cout << "Test 2.  (Machin's Formula for Pi)." << endl;

  /* Use the Machin's arctangent formula:

       pi / 4  =  4 arctan(1/5) - arctan(1/239)

     The arctangent is computed based on the Taylor series expansion

       arctan(x) = x - x^3 / 3 + x^5 / 5 - x^7 / 7 + ...
  */

  T s1, s2, t, r;
  int k;
  int sign;
  double d;
  double err;

  /* Compute arctan(1/5) */
  d = 1.0;
  t = T(1.0) / 5.0;
  r = sqr(t);
  s1 = 0.0;
  k = 0;

  sign = 1;
  while (t > T::_eps) {
    k++;
    if (sign < 0)
      s1 -= (t / d);
    else
      s1 += (t / d);

    d += 2.0;
    t *= r;
    sign = -sign;
  }

  if (flag_verbose)
    cout << k << " Iterations" << endl;

  /* Compute arctan(1/239) */
  d = 1.0;
  t = T(1.0) / 239.0;
  r = sqr(t);
  s2 = 0.0;
  k = 0;

  sign = 1;
  while (t > T::_eps) {
    k++;
    if (sign < 0)
      s2 -= (t / d);
    else
      s2 += (t / d);

    d += 2.0;
    t *= r;
    sign = -sign;
  }

  if (flag_verbose)
    cout << k << " Iterations" << endl;

  T p = 4.0 * s1 - s2;

  p *= 4.0;
  err = abs(to_double(p - T::_pi));

  if (flag_verbose) {
    cout.precision(T::_ndigits);
    cout << "   pi = " << p << endl;
    cout << "  _pi = " << T::_pi << endl;

    cout.precision(double_digits);
    cout << "error = " << err << " = " << err / T::_eps << " eps" << endl;
  }

  return (err < 8.0 * T::_eps);
}

/* Test 3.  Salamin-Brent Quadratic Formula for Pi. */
template <class T>
bool TestSuite<T>::test3() {
  cout << endl;
  cout << "Test 3.  (Salamin-Brent Quadratic Formula for Pi)." << endl;
  cout.precision(T::_ndigits);

  T a, b, s, p;
  T a_new, b_new, p_old;
  double m;
  double err;
  const int max_iter = 20;

  a = 1.0;
  b = sqrt(T(0.5));
  s = 0.5;
  m = 1.0;

  p = 2.0 * sqr(a) / s;
  if (flag_verbose)
    cout << "Iteration  0: " << p << endl;
  for (int i = 1; i <= max_iter; i++) {
    m *= 2.0;
    a_new = 0.5 * (a + b);
    b_new = a * b;
    s -= m * (sqr(a_new) - b_new);
    a = a_new;
    b = sqrt(b_new);
    p_old = p;
    p = 2.0 * sqr(a) / s;
    if (flag_verbose)
      cout << "Iteration " << std::setw(2) << i << ": " << p << endl;
    if (abs(to_double(p - p_old)) < 64 * T::_eps)
      break;
  }

  err = abs(to_double(p - T::_pi));

  if (flag_verbose) {
    cout << "         _pi: " << T::_pi << endl;
    cout.precision(double_digits);
    cout << "       error: " << err << " = " << err / T::_eps << " eps" << endl;
  }

  // for some reason, this test gives relatively large error compared
  // to other tests.  May need to be looked at more closely.
  return (err < 1024.0 * T::_eps);
}

/* Test 4.  Borwein Quartic Formula for Pi. */
template <class T>
bool TestSuite<T>::test4() {
  cout << endl;
  cout << "Test 4.  (Borwein Quartic Formula for Pi)." << endl;
  cout.precision(T::_ndigits);

  T a, y, p, r, p_old;
  double m;
  double err;
  const int max_iter = 20;

  a = 6.0 - 4.0 * sqrt(T(2.0));
  y = sqrt(T(2.0)) - 1.0;
  m = 2.0;

  p = 1.0 / a;
  if (flag_verbose)
    cout << "Iteration  0: " << p << endl;

  for (int i = 1; i <= max_iter; i++) {
    m *= 4.0;
    r = nroot(1.0 - sqr(sqr(y)), 4);
    y = (1.0 - r) / (1.0 + r);
    a = a * sqr(sqr(1.0 + y)) - m * y * (1.0 + y + sqr(y));

    p_old = p;
    p = 1.0 / a;
    if (flag_verbose)
      cout << "Iteration " << std::setw(2) << i << ": " << p << endl;
    if (abs(to_double(p - p_old)) < 16 * T::_eps)
      break;
  }

  err = abs(to_double(p - T::_pi));
  if (flag_verbose) {
    cout << "         _pi: " << T::_pi << endl;
    cout.precision(double_digits);
    cout << "       error: " << err << " = " << err / T::_eps << " eps" << endl;
  }

  return (err < 256.0 * T::_eps);
}

/* Test 5.  Taylor Series Formula for E. */
template <class T>
bool TestSuite<T>::test5() {

  cout << endl;
  cout << "Test 5.  (Taylor Series Formula for E)." << endl;
  cout.precision(T::_ndigits);

  /* Use Taylor series

       e = 1 + 1 + 1/2! + 1/3! + 1/4! + ...

     To compute e.
  */

  T s = 2.0, t = 1.0;
  double n = 1.0;
  double delta;
  int i = 0;

  while (t > T::_eps) {
    i++;
    n += 1.0;
    t /= n;
    s += t;
  }

  delta = abs(to_double(s - T::_e));

  if (flag_verbose) {
    cout << "    e = " << s << endl;
    cout << "   _e = " << T::_e << endl;

    cout.precision(double_digits);
    cout << "error = " << delta << " = " << delta / T::_eps << " eps" << endl;
    cout << i << " iterations." << endl;
  }

  return (delta < 64.0 * T::_eps);
}

/* Test 6.  Taylor Series Formula for log 2.*/
template <class T>
bool TestSuite<T>::test6() {
  cout << endl;
  cout << "Test 6.  (Taylor Series Formula for Log 2)." << endl;
  cout.precision(T::_ndigits);

  /* Use the Taylor series

      -log(1-x) = x + x^2/2 + x^3/3 + x^4/4 + ...

     with x = 1/2 to get  log(1/2) = -log 2.
  */

  T s = 0.5;
  T t = 0.5;
  double delta;
  double n = 1.0;
  int i = 0;

  while (abs(t) > T::_eps) {
    i++;
    n += 1.0;
    t *= 0.5;
    s += (t/n);
  }

  delta = abs(to_double(s - T::_log2));

  if (flag_verbose) {
    cout << " log2 = " << s << endl;
    cout << "_log2 = " << T::_log2 << endl;

    cout.precision(double_digits);
    cout << "error = " << delta << " = " << (delta / T::_eps)
         << " eps" << endl;
    cout << i << " iterations." << endl;
  }

  return (delta < 4.0 * T::_eps);
}

/* Test 7.  Sanity check for exp. */
template <class T>
bool TestSuite<T>::test7() {
  cout << endl;
  cout << "Test 7.  (Sanity check for exp)." << endl;
  cout.precision(T::_ndigits);

  /* Do simple sanity check
   *
   *   e^2 = exp(2)
   *       = exp(-13/4) * exp(-9/4) * exp(-5/4) * exp(-1/4) *
   *         exp(3/4) * exp(7/4) * exp(11/4) * exp(15/4)
   */

  T t = -3.25;
  T p =  1.0;

  for (int i = 0; i < 8; i++, t += 1.0) {
    /* For some reason gcc-4.1.x on x86_64 miscompiles p *= exp(t) here. */
    p = p * exp(t);
  }

  T t1 = exp(T(2.0));
  T t2 = sqr(T::_e);
  double delta = std::max(abs(to_double(t1 - p)), abs(to_double(t2 - p)));

  if (flag_verbose) {
    cout << "result = " << p << endl;
    cout << "exp(2) = " << t1 << endl;
    cout << "   e^2 = " << t2 << endl;

    cout.precision(double_digits);

    cout << " error = " << delta << " = " << (delta / T::_eps)
         << " eps" << endl;
  }

  return (delta < 16.0 * T::_eps);
}

template <class T>
bool TestSuite<T>::test8() {
  cout << endl;
  cout << "Test 8.  (Sanity check for sin / cos)." << endl;
  cout.precision(T::_ndigits);

  /* Do simple sanity check
   *
   *  sin(x) = sin(5x/7)cos(2x/7) + cos(5x/7)sin(2x/7)
   *
   *  cos(x) = cos(5x/7)cos(2x/7) - sin(5x/7)sin(2x/7);
   */

  T x = T::_pi / 3.0;
  T x1 = 5.0 * x / 7.0;
  T x2 = 2.0 * x / 7.0;

  T r1 = sin(x1)*cos(x2) + cos(x1)*sin(x2);
  T r2 = cos(x1)*cos(x2) - sin(x1)*sin(x2);
  T t1 = sqrt(T(3.0)) / 2.0;
  T t2 = 0.5;

  double delta = std::max(abs(to_double(t1 - r1)), abs(to_double(t2 - r2)));

  if (flag_verbose) {
    cout << "  r1 = " << r1 << endl;
    cout << "  t1 = " << t1 << endl;
    cout << "  r2 = " << r2 << endl;
    cout << "  t2 = " << t2 << endl;

    cout.precision(double_digits);
    cout << " error = " << delta << " = " << (delta / T::_eps)
         << " eps" << endl;
  }

  return (delta < 4.0 * T::_eps);
}

bool test_qd_real_comparison() {
  cout << endl;
  cout << "Test 9.  (qd_real comparison normalization)." << endl;

  qd_real a(1.0, 0.5, 0.0, 0.0);
  qd_real b(1.5);
  qd_real diff = a - b;
  diff.renorm();

  bool pass = diff.is_zero();
  pass &= (a == b);
  pass &= !(a != b);
  pass &= !(a < b);
  pass &= !(a > b);
  pass &= (a <= b);
  pass &= (a >= b);

  pass &= (a == 1.5);
  pass &= (1.5 == a);
  pass &= (a == dd_real(1.5));
  pass &= (dd_real(1.5) == a);

  if (flag_verbose) {
    cout << "a = [" << a[0] << ", " << a[1] << ", "
         << a[2] << ", " << a[3] << "]" << endl;
    cout << "b = " << b << endl;
    cout << "renormalized diff is zero: " << diff.is_zero() << endl;
  }

  return pass;
}

bool test_dd_real_comparison() {
  cout << endl;
  cout << "Test 9.  (dd_real comparison normalization)." << endl;

  double lo = std::ldexp(1.0, -53);
  dd_real a(1.0 - lo, lo);
  dd_real b(1.0);
  dd_real diff = a - b;

  bool pass = diff.is_zero();
  pass &= (a == b);
  pass &= !(a != b);
  pass &= !(a < b);
  pass &= !(a > b);
  pass &= (a <= b);
  pass &= (a >= b);

  pass &= (a == 1.0);
  pass &= (1.0 == a);

  if (flag_verbose) {
    cout << "a = [" << a._hi() << ", " << a._lo() << "]" << endl;
    cout << "b = " << b << endl;
    cout << "diff is zero: " << diff.is_zero() << endl;
  }

  return pass;
}

template <class T>
bool TestSuite<T>::testall() {
  bool pass = true;
  pass &= print_result(test1());
  pass &= print_result(test2());
  pass &= print_result(test3());
  pass &= print_result(test4());
  pass &= print_result(test5());
  pass &= print_result(test6());
  pass &= print_result(test7());
  pass &= print_result(test8());
  return pass;
}

void print_usage() {
  cout << "qd_test [-h] [-dd] [-td] [-qd] [-all]" << endl;
  cout << "  Performs miscellaneous tests of the quad-double library," << endl;
  cout << "  such as polynomial root finding, computation of pi, etc." << endl;
  cout << endl;
  cout << "  -h -help  Prints this usage message." << endl;
  cout << "  -dd       Perform tests with double-double types." << endl;
  cout << "  -td       Perform tests with triple-double types." << endl;
  cout << "  -qd       Perform tests with quad-double types." << endl;
  cout << "  -all      Perform double-double, triple-double, and quad-double tests." << endl;
  cout << "  -v" << endl;
  cout << "  -verbose  Print detailed information for each test." << endl;

}

int main(int argc, char *argv[]) {

  bool pass = true;
  unsigned int old_cw;
  fpu_fix_start(&old_cw);

  /* Parse the arguments. */
  char *arg;
  for (int i = 1; i < argc; i++) {
    arg = argv[i];
    if (strcmp(arg, "-h") == 0 || strcmp(arg, "-help") == 0) {
      print_usage();
      exit(0);
    } else if (strcmp(arg, "-dd") == 0) {
      flag_test_dd = true;
    } else if (strcmp(arg, "-td") == 0) {
      flag_test_td = true;
    } else if (strcmp(arg, "-qd") == 0) {
      flag_test_qd = true;
    } else if (strcmp(arg, "-all") == 0) {
      flag_test_dd = flag_test_td = flag_test_qd = true;
    } else if (strcmp(arg, "-v") == 0 || strcmp(arg, "-verbose") == 0) {
      flag_verbose = true;
    } else {
      cerr << "Unknown flag `" << arg << "'." << endl;
    }
  }

  /* If no flag, test both double-double and quad-double. */
  if (!flag_test_dd && !flag_test_td && !flag_test_qd) {
    flag_test_dd = true;
    flag_test_td = true;
    flag_test_qd = true;
  }

  if (flag_test_dd) {
    TestSuite<dd_real> dd_test;

    cout << endl;
    cout << "Testing dd_real ..." << endl;
    if (flag_verbose)
      cout << "sizeof(dd_real) = " << sizeof(dd_real) << endl;
    pass &= dd_test.testall();
    pass &= print_result(test_dd_real_comparison());
  }

  if (flag_test_qd) {
    TestSuite<qd_real> qd_test;

    cout << endl;
    cout << "Testing qd_real ..." << endl;
    if (flag_verbose)
      cout << "sizeof(qd_real) = " << sizeof(qd_real) << endl;
    pass &= qd_test.testall();
    pass &= print_result(test_qd_real_comparison());
  }

  if (flag_test_td) {
    TestSuite<td_real> td_base_test;
    TdTestSuite td_test;

    cout << endl;
    cout << "Testing td_real ..." << endl;
    if (flag_verbose)
      cout << "sizeof(td_real) = " << sizeof(td_real) << endl;
    pass &= td_base_test.testall();
    pass &= td_test.testall();
  }

  fpu_fix_end(&old_cw);
  return (pass ? 0 : 1);
}
