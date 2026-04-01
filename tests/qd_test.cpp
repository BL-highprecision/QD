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

class TdTestSuite {
public:
  bool test1();
  bool test2();
  bool test3();
  bool test4();
  bool test5();
  bool test6();
  bool testall();
};

bool TdTestSuite::test1() {
  cout << endl;
  cout << "Test 1.  (Normalization invariants after arithmetic)." << endl;

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

bool TdTestSuite::test2() {
  cout << endl;
  cout << "Test 2.  (Cancellation-sensitive addition / subtraction)." << endl;

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

bool TdTestSuite::test3() {
  cout << endl;
  cout << "Test 3.  (Multiplication across mixed magnitudes)." << endl;

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

bool TdTestSuite::test4() {
  cout << endl;
  cout << "Test 4.  (Division sanity checks)." << endl;

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

bool TdTestSuite::test5() {
  cout << endl;
  cout << "Test 5.  (sqrt on exact squares and values near 1)." << endl;

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

bool TdTestSuite::test6() {
  cout << endl;
  cout << "Test 6.  (Parse / format round trips)." << endl;

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

bool TdTestSuite::testall() {
  bool pass = true;
  pass &= print_result(test1());
  pass &= print_result(test2());
  pass &= print_result(test3());
  pass &= print_result(test4());
  pass &= print_result(test5());
  pass &= print_result(test6());
  return pass;
}

}  // namespace

/* Test 1.   Polynomial Evaluation / Polynomial Solving */
template <class T>
bool TestSuite<T>::test1() {
  cout << endl;
  cout << "Test 1.  (Polynomial)." << endl;

  static const int n = 8;
  T *c = new T[n];
  T x, y;

  for (int i = 0; i < n; i++)
    c[i] = static_cast<double>(i+1);

  x = polyroot(c, n-1, T(0.0));
  y = polyeval(c, n-1, x);

  if (flag_verbose) {
    cout.precision(T::_ndigits);
    cout << "Root Found:  x  = " << x << endl;
    cout << "           p(x) = " << y << endl;
  }

  delete [] c;
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
  double i = 0;

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
  }

  if (flag_test_qd) {
    TestSuite<qd_real> qd_test;

    cout << endl;
    cout << "Testing qd_real ..." << endl;
    if (flag_verbose)
      cout << "sizeof(qd_real) = " << sizeof(qd_real) << endl;
    pass &= qd_test.testall();
  }

  if (flag_test_td) {
    TdTestSuite td_test;

    cout << endl;
    cout << "Testing td_real ..." << endl;
    if (flag_verbose)
      cout << "sizeof(td_real) = " << sizeof(td_real) << endl;
    pass &= td_test.testall();
  }
  
  fpu_fix_end(&old_cw);
  return (pass ? 0 : 1);
}
