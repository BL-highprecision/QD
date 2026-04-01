/*
 * src/td_const.cpp
 *
 * Defines constants used in triple-double package.
 */
#include "config.h"
#include <qd/td_real.h>
#include <qd/inline.h>

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
