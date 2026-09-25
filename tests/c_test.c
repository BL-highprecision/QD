#include <stdio.h>
#include <math.h>
#include <qd/c_dd.h>
#include <qd/c_qd.h>
#include <qd/c_td.h>

/* Test 1.  Salamin-Brent quadratically convergent formula for pi. */
int test_1() {

  double a[4], b[4], s[4], p[4], t[4], t2[4];
  double a_new[4], b_new[4], p_old[4];
  double m, err;
  int r, i;
  const int max_iter = 20;

  puts("Test 1.  (Salamin-Brent quadratically convergent formula for pi)");

  c_qd_copy_d(1.0, a);  /* a = 1.0 */
  c_qd_copy_d(0.5, t);  /* t = 0.5 */
  c_qd_sqrt(t, b);      /* b = sqrt(t) */
  c_qd_copy_d(0.5, s);  /* s = 0.5 */
  m = 1.0;

  c_qd_sqr(a, p);
  c_qd_selfmul_d(2.0, p);
  c_qd_selfdiv(s, p);

  printf("  iteration 0: ");
  c_qd_write(p);
  for (i = 1; i <= max_iter; i++) {
    m *= 2.0;

    /* a_new = 0.5 * (a + b) */
    c_qd_add(a, b, a_new);
    c_qd_selfmul_d(0.5, a_new);

    c_qd_mul(a, b, b_new); /* b_new = a * b */

    /* Compute s = s - m * (a_new^2 - b) */
    c_qd_sqr(a_new, t);       /* t = a_new ^ 2 */
    c_qd_selfsub(b_new, t);   /* t -= b_new */
    c_qd_selfmul_d(m, t);     /* t *= m */
    c_qd_selfsub(t, s);       /* s -= t */

    c_qd_copy(a_new, a);
    c_qd_sqrt(b_new, b);
    c_qd_copy(p, p_old);

    /* Compute  p = 2.0 * a^2 / s */
    c_qd_sqr(a, p);
    c_qd_selfmul_d(2.0, p);
    c_qd_selfdiv(s, p);

    /* Test for convergence by looking at |p - p_old|. */
    c_qd_sub(p, p_old, t);
    c_qd_abs(t, t2);
    c_qd_comp_qd_d(t2, 1e-60, &r);
    if (r < 0) break;

    printf("  iteration %1d: ", i);
    c_qd_write(p);
  }

  c_qd_pi(p);   /* p = pi */
  printf("          _pi: ");
  c_qd_write(p);
  printf("        error: %.5e = %g eps\n", t2[0], t2[0] / ldexp(1.0, -209));

  return 0;
}

int test_2() {
  double x[3], y[3], s[3], c[3], t[3], u[3];
  int r;

  puts("Test 2.  (Triple-double C wrapper smoke test)");

  c_td_read("0.5", x);
  c_td_sincos(x, s, c);
  c_td_sqr(s, t);
  c_td_sqr(c, u);
  c_td_selfadd(u, t);
  c_td_sub_td_d(t, 1.0, u);
  c_td_abs(u, u);
  c_td_comp_td_d(u, 1e-40, &r);
  if (r > 0) {
    puts("  sin^2 + cos^2 != 1");
    return 1;
  }

  c_td_exp(x, y);
  c_td_log(y, t);
  c_td_sub(t, x, u);
  c_td_abs(u, u);
  c_td_comp_td_d(u, 1e-40, &r);
  if (r > 0) {
    puts("  log(exp(x)) too far from x");
    return 1;
  }

  return 0;
}

int test_3() {
  double dd[2], dd_out[2];
  double td[3], td_out[3];
  double qd[4], qd_out[4];
  int r;

  puts("Test 3.  (Cross-type C wrapper smoke test)");

  c_dd_copy_d(1.25, dd);
  c_td_copy_d(0.5, td);
  c_qd_copy_d(2.0, qd);

  c_dd_add_td_dd(td, dd, dd_out);
  c_dd_comp_dd_d(dd_out, 1.75, &r);
  if (r != 0) {
    puts("  c_dd_add_td_dd failed");
    return 1;
  }

  c_dd_add_qd_dd(qd, dd, dd_out);
  c_dd_comp_dd_d(dd_out, 3.25, &r);
  if (r != 0) {
    puts("  c_dd_add_qd_dd failed");
    return 1;
  }

  c_td_add_qd_td(qd, td, td_out);
  c_td_comp_td_d(td_out, 2.5, &r);
  if (r != 0) {
    puts("  c_td_add_qd_td failed");
    return 1;
  }

  c_td_copy_qd(qd, td_out);
  c_td_comp_td_d(td_out, 2.0, &r);
  if (r != 0) {
    puts("  c_td_copy_qd failed");
    return 1;
  }

  c_qd_add_td_qd(td, qd, qd_out);
  c_qd_comp_qd_d(qd_out, 2.5, &r);
  if (r != 0) {
    puts("  c_qd_add_td_qd failed");
    return 1;
  }

  c_qd_copy_td(td, qd_out);
  c_qd_comp_qd_d(qd_out, 0.5, &r);
  if (r != 0) {
    puts("  c_qd_copy_td failed");
    return 1;
  }

  return 0;
}

int main(void) {
  fpu_fix_start(NULL);
  return test_1() || test_2() || test_3();
}
