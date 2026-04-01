!  tdmod.f
!
!  Fortran-90 module file to use with triple-double numbers.

module tdmodule
  use ddmodule, only: dd_real
  use qdmodule, only: qd_real
  use tdext
  implicit none

  type td_real
    sequence
    real*8 :: re(3)
  end type td_real

  type td_complex
    sequence
    real*8 :: cmp(6)
  end type td_complex

  real*8 d_td_eps
  parameter (d_td_eps = 5.47382212626881668d-48)

  type (td_real) td_one, td_zero, td_eps, td_huge, td_tiny
  parameter (td_one = td_real((/1.0d0, 0.0d0, 0.0d0/)))
  parameter (td_zero = td_real((/0.0d0, 0.0d0, 0.0d0/)))
  parameter (td_eps = td_real((/d_td_eps, 0.0d0, 0.0d0/)))
  parameter (td_huge = td_real((/ &
    1.79769313486231570815d+308, 9.97920154767359795037d+291, &
    5.53956966280111259858d+275/)))
  parameter (td_tiny = td_real((/1.80519437586482958d-276, &
    0.0d0, 0.0d0/)))

  interface assignment (=)
    module procedure assign_td_str
    module procedure assign_td
    module procedure assign_td_d
    module procedure assign_d_td
    module procedure assign_td_i
    module procedure assign_i_td
    module procedure assign_td_dd
    module procedure assign_dd_td
    module procedure assign_td_qd
    module procedure assign_qd_td
    module procedure assign_tdc
    module procedure assign_tdc_td
    module procedure assign_td_tdc
    module procedure assign_tdc_d
    module procedure assign_tdc_i
    module procedure assign_d_tdc
    module procedure assign_tdc_dc
    module procedure assign_dc_tdc
  end interface

  interface operator (+)
    module procedure add_td
    module procedure add_td_d
    module procedure add_d_td
    module procedure add_td_i
    module procedure add_i_td
    module procedure add_tdc
    module procedure add_tdc_td
    module procedure add_td_tdc
    module procedure add_tdc_d
    module procedure add_d_tdc
  end interface

  interface operator (-)
    module procedure sub_td
    module procedure sub_td_d
    module procedure sub_d_td
    module procedure neg_td
    module procedure sub_tdc
    module procedure sub_tdc_td
    module procedure sub_td_tdc
    module procedure sub_tdc_d
    module procedure sub_d_tdc
    module procedure neg_tdc
  end interface

  interface operator (*)
    module procedure mul_td
    module procedure mul_td_d
    module procedure mul_d_td
    module procedure mul_td_i
    module procedure mul_i_td
    module procedure mul_tdc
    module procedure mul_tdc_td
    module procedure mul_td_tdc
    module procedure mul_tdc_d
    module procedure mul_d_tdc
    module procedure mul_tdc_i
    module procedure mul_i_tdc
  end interface

  interface operator (/)
    module procedure div_td
    module procedure div_td_d
    module procedure div_d_td
    module procedure div_td_i
    module procedure div_i_td
    module procedure div_tdc
    module procedure div_tdc_td
    module procedure div_td_tdc
    module procedure div_tdc_d
  end interface

  interface operator (**)
    module procedure pwr_td
    module procedure pwr_td_i
    module procedure pwr_d_td
    module procedure pwr_tdc_i
  end interface

  interface tdreal
    module procedure to_td_i
    module procedure to_td_d
    module procedure to_td_dd
    module procedure to_td_qd
    module procedure to_td_td
    module procedure to_td_str
  end interface

  interface ddreal
    module procedure to_dd_td
  end interface

  interface qdreal
    module procedure to_qd_td
  end interface

  interface real
    module procedure to_d_td
    module procedure to_td_tdc
  end interface

  interface dble
    module procedure to_d_td
    module procedure to_d_tdc
  end interface

  interface tdcomplex
    module procedure to_tdc_td
    module procedure to_tdc_td2
    module procedure to_tdc_d
    module procedure to_tdc_dc
  end interface

  interface cmplx
    module procedure to_dc_tdc
  end interface

  interface conjg
    module procedure tdcconjg
  end interface

  interface int
    module procedure to_int_td
  end interface

  interface sin
    module procedure tdsin
  end interface
  interface cos
    module procedure tdcos
  end interface
  interface tan
    module procedure tdtan
  end interface
  interface sincos
    module procedure tdsincos
  end interface

  interface asin
    module procedure tdasin
  end interface
  interface acos
    module procedure tdacos
  end interface
  interface atan
    module procedure tdatan
  end interface
  interface atan2
    module procedure tdatan2
  end interface

  interface exp
    module procedure tdexp
    module procedure tdcexp
  end interface
  interface log
    module procedure tdlog
    module procedure tdclog
  end interface
  interface log10
    module procedure tdlog10
  end interface

  interface sqrt
    module procedure tdsqrt
  end interface
  interface sqr
    module procedure tdsqr
  end interface
  interface nroot
    module procedure tdnroot
  end interface

  interface sinh
    module procedure tdsinh
  end interface
  interface cosh
    module procedure tdcosh
  end interface
  interface tanh
    module procedure tdtanh
  end interface
  interface sincosh
    module procedure tdsincosh
  end interface

  interface asinh
    module procedure tdasinh
  end interface
  interface acosh
    module procedure tdacosh
  end interface
  interface atanh
    module procedure tdatanh
  end interface

  interface aint
    module procedure tdaint
  end interface

  interface floor
    module procedure tdfloor
  end interface

  interface ceil
    module procedure tdceil
  end interface

  interface nint
    module procedure tdnint
  end interface

  interface anint
    module procedure tdanint
  end interface

  interface abs
    module procedure tdabs
    module procedure tdcabs
  end interface

  interface sign
    module procedure tdsign
    module procedure tdsign_td_d
  end interface

  interface random_number
    module procedure tdrand
  end interface

  interface aimag
    module procedure td_aimag
  end interface

  interface operator (==)
    module procedure eq_td
    module procedure eq_td_d
    module procedure eq_d_td
    module procedure eq_td_i
    module procedure eq_i_td
    module procedure eq_tdc
    module procedure eq_tdc_td
    module procedure eq_td_tdc
  end interface

  interface operator (/=)
    module procedure ne_td
    module procedure ne_td_d
    module procedure ne_d_td
    module procedure ne_td_i
    module procedure ne_i_td
    module procedure ne_tdc
    module procedure ne_tdc_td
    module procedure ne_td_tdc
  end interface

  interface operator (>)
    module procedure gt_td
    module procedure gt_td_d
    module procedure gt_d_td
    module procedure gt_td_i
    module procedure gt_i_td
  end interface

  interface operator (<)
    module procedure lt_td
    module procedure lt_td_d
    module procedure lt_d_td
    module procedure lt_td_i
    module procedure lt_i_td
  end interface

  interface operator (>=)
    module procedure ge_td
    module procedure ge_td_d
    module procedure ge_d_td
    module procedure ge_td_i
    module procedure ge_i_td
  end interface

  interface operator (<=)
    module procedure le_td
    module procedure le_td_d
    module procedure le_d_td
    module procedure le_td_i
    module procedure le_i_td
  end interface

  interface read_scalar
    module procedure tdinpq
    module procedure tdcinpq
  end interface

  interface write_scalar
    module procedure tdoutq
    module procedure tdcoutq
  end interface

  interface tdread
    module procedure tdinpq
  end interface

  interface tdwrite
    module procedure tdoutq
  end interface

  interface tdcread
    module procedure tdcinpq
  end interface

  interface tdcwrite
    module procedure tdcoutq
  end interface

  interface tdpi
    module procedure td_pi
  end interface

  interface huge
    module procedure tdhuge
  end interface

  interface safe_huge
    module procedure td_safe_huge
  end interface

  interface tiny
    module procedure tdtiny
  end interface

  interface epsilon
    module procedure tdepsilon
  end interface

  interface radix
    module procedure td_radix
  end interface

  interface digits
    module procedure td_digits
  end interface

  interface maxexponent
    module procedure td_max_expn
  end interface

  interface minexponent
    module procedure td_min_expn
  end interface

  interface precision
    module procedure td_precision
  end interface

  interface range
    module procedure td_range
  end interface

  interface nan
    module procedure td_nan
  end interface

  interface min
    module procedure tdmin
    module procedure tdmin2
  end interface

  interface max
    module procedure tdmax
    module procedure tdmax2
  end interface

  interface mod
    module procedure tdmod
  end interface

contains

  subroutine assign_td_str(a, s)
    type (td_real), intent(inout) :: a
    character (len=*), intent(in) :: s
    character*80 t
    t = s
    call tdinpc(t, a%re)
  end subroutine assign_td_str

  elemental subroutine assign_td(a, b)
    type (td_real), intent(inout) :: a
    type (td_real), intent(in) :: b
    a%re = b%re
  end subroutine assign_td

  elemental subroutine assign_td_d(a, d)
    type (td_real), intent(inout) :: a
    real*8, intent(in) :: d
    a%re(1) = d
    a%re(2:3) = 0.0d0
  end subroutine assign_td_d

  elemental subroutine assign_d_td(d, a)
    real*8, intent(inout) :: d
    type (td_real), intent(in) :: a
    d = a%re(1)
  end subroutine assign_d_td

  elemental subroutine assign_td_i(a, i)
    type (td_real), intent(inout) :: a
    integer, intent(in) :: i
    a%re(1) = i
    a%re(2:3) = 0.0d0
  end subroutine assign_td_i

  elemental subroutine assign_i_td(i, a)
    integer, intent(inout) :: i
    type (td_real), intent(in) :: a
    i = a%re(1)
  end subroutine assign_i_td

  elemental subroutine assign_td_dd(td, dd)
    type (td_real), intent(inout) :: td
    type (dd_real), intent(in) :: dd
    td%re(1:2) = dd%re
    td%re(3) = 0.d0
  end subroutine assign_td_dd

  elemental subroutine assign_dd_td(dd, td)
    type (dd_real), intent(inout) :: dd
    type (td_real), intent(in) :: td
    dd%re = td%re(1:2)
  end subroutine assign_dd_td

  elemental subroutine assign_td_qd(td, qd)
    type (td_real), intent(inout) :: td
    type (qd_real), intent(in) :: qd
    td%re = qd%re(1:3)
  end subroutine assign_td_qd

  elemental subroutine assign_qd_td(qd, td)
    type (qd_real), intent(inout) :: qd
    type (td_real), intent(in) :: td
    qd%re(1:3) = td%re
    qd%re(4) = 0.d0
  end subroutine assign_qd_td

  elemental subroutine assign_tdc(a, b)
    type (td_complex), intent(inout) :: a
    type (td_complex), intent(in) :: b
    a%cmp = b%cmp
  end subroutine assign_tdc

  elemental subroutine assign_tdc_td(tdc, td)
    type (td_complex), intent(inout) :: tdc
    type (td_real), intent(in) :: td
    tdc%cmp(1:3) = td%re
    tdc%cmp(4:6) = 0.d0
  end subroutine assign_tdc_td

  elemental subroutine assign_td_tdc(td, tdc)
    type (td_real), intent(inout) :: td
    type (td_complex), intent(in) :: tdc
    td%re = tdc%cmp(1:3)
  end subroutine assign_td_tdc

  elemental subroutine assign_tdc_d(tdc, d)
    type (td_complex), intent(inout) :: tdc
    real*8, intent(in) :: d
    tdc%cmp(1) = d
    tdc%cmp(2:6) = 0.d0
  end subroutine assign_tdc_d

  elemental subroutine assign_tdc_i(tdc, i)
    type (td_complex), intent(inout) :: tdc
    integer, intent(in) :: i
    tdc%cmp(1) = i
    tdc%cmp(2:6) = 0.d0
  end subroutine assign_tdc_i

  elemental subroutine assign_d_tdc(d, tdc)
    real*8, intent(inout) :: d
    type (td_complex), intent(in) :: tdc
    d = tdc%cmp(1)
  end subroutine assign_d_tdc

  elemental subroutine assign_tdc_dc(tdc, dc)
    type (td_complex), intent(inout) :: tdc
    complex(kind(0.d0)), intent(in) :: dc
    tdc%cmp(1) = dble(dc)
    tdc%cmp(2:3) = 0.d0
    tdc%cmp(4) = aimag(dc)
    tdc%cmp(5:6) = 0.d0
  end subroutine assign_tdc_dc

  elemental subroutine assign_dc_tdc(dc, tdc)
    complex(kind(0.d0)), intent(inout) :: dc
    type (td_complex), intent(in) :: tdc
    dc = cmplx(tdc%cmp(1), tdc%cmp(4), kind(0.d0))
  end subroutine assign_dc_tdc

  elemental type (td_real) function to_td_i(ia)
    integer, intent(in) :: ia
    to_td_i%re(1) = ia
    to_td_i%re(2:3) = 0.d0
  end function to_td_i

  elemental type (td_real) function to_td_d(a)
    real*8, intent(in) :: a
    to_td_d%re(1) = a
    to_td_d%re(2:3) = 0.d0
  end function to_td_d

  elemental type (td_real) function to_td_dd(dd)
    type (dd_real), intent(in) :: dd
    to_td_dd%re(1:2) = dd%re
    to_td_dd%re(3) = 0.d0
  end function to_td_dd

  elemental type (td_real) function to_td_qd(qd)
    type (qd_real), intent(in) :: qd
    to_td_qd%re = qd%re(1:3)
  end function to_td_qd

  elemental type (td_real) function to_td_td(a)
    type (td_real), intent(in) :: a
    to_td_td%re = a%re
  end function to_td_td

  type (td_real) function to_td_str(s)
    character (len=*), intent(in) :: s
    character*80 t
    t = s
    call tdinpc(t, to_td_str%re)
  end function to_td_str

  elemental type (dd_real) function to_dd_td(td)
    type (td_real), intent(in) :: td
    to_dd_td%re = td%re(1:2)
  end function to_dd_td

  elemental type (qd_real) function to_qd_td(td)
    type (td_real), intent(in) :: td
    to_qd_td%re(1:3) = td%re
    to_qd_td%re(4) = 0.d0
  end function to_qd_td

  elemental real*8 function to_d_td(a)
    type (td_real), intent(in) :: a
    to_d_td = a%re(1)
  end function to_d_td

  elemental type (td_real) function to_td_tdc(tdc)
    type (td_complex), intent(in) :: tdc
    to_td_tdc%re = tdc%cmp(1:3)
  end function to_td_tdc

  elemental type (td_complex) function to_tdc_td(td)
    type (td_real), intent(in) :: td
    to_tdc_td%cmp(1:3) = td%re
    to_tdc_td%cmp(4:6) = 0.d0
  end function to_tdc_td

  elemental type (td_complex) function to_tdc_td2(x, y)
    type (td_real), intent(in) :: x, y
    to_tdc_td2%cmp(1:3) = x%re
    to_tdc_td2%cmp(4:6) = y%re
  end function to_tdc_td2

  elemental type (td_complex) function to_tdc_d(d)
    real*8, intent(in) :: d
    to_tdc_d%cmp(1) = d
    to_tdc_d%cmp(2:6) = 0.d0
  end function to_tdc_d

  elemental complex(kind(0.d0)) function to_dc_tdc(tdc)
    type (td_complex), intent(in) :: tdc
    to_dc_tdc = cmplx(tdc%cmp(1), tdc%cmp(4), kind(0.d0))
  end function to_dc_tdc

  elemental type (td_complex) function to_tdc_dc(dc)
    complex(kind(0.d0)), intent(in) :: dc
    to_tdc_dc%cmp(1) = dble(dc)
    to_tdc_dc%cmp(2:3) = 0.d0
    to_tdc_dc%cmp(4) = aimag(dc)
    to_tdc_dc%cmp(5:6) = 0.d0
  end function to_tdc_dc

  elemental real*8 function to_d_tdc(tdc)
    type (td_complex), intent(in) :: tdc
    to_d_tdc = tdc%cmp(1)
  end function to_d_tdc

  elemental integer function to_int_td(a)
    type (td_real), intent(in) :: a
    to_int_td = a%re(1)
  end function to_int_td

  elemental type (td_complex) function tdcconjg(tdc)
    type (td_complex), intent(in) :: tdc
    tdcconjg%cmp(1:3) = tdc%cmp(1:3)
    tdcconjg%cmp(4:6) = -tdc%cmp(4:6)
  end function tdcconjg

  elemental type (td_real) function add_td(a, b)
    type (td_real), intent(in) :: a, b
    call f_td_add(a%re, b%re, add_td%re)
  end function add_td

  elemental type (td_real) function add_td_d(a, b)
    type (td_real), intent(in) :: a
    real*8, intent(in) :: b
    call f_td_add_td_d(a%re, b, add_td_d%re)
  end function add_td_d

  elemental type (td_real) function add_d_td(a, b)
    real*8, intent(in) :: a
    type (td_real), intent(in) :: b
    add_d_td = add_td_d(b, a)
  end function add_d_td

  elemental type (td_real) function add_td_i(a, b)
    type (td_real), intent(in) :: a
    integer, intent(in) :: b
    call f_td_add_td_d(a%re, dble(b), add_td_i%re)
  end function add_td_i

  elemental type (td_real) function add_i_td(a, b)
    integer, intent(in) :: a
    type (td_real), intent(in) :: b
    add_i_td = add_td_i(b, a)
  end function add_i_td

  elemental type (td_complex) function add_tdc(a, b)
    type (td_complex), intent(in) :: a, b
    call f_td_add(a%cmp(1:3), b%cmp(1:3), add_tdc%cmp(1:3))
    call f_td_add(a%cmp(4:6), b%cmp(4:6), add_tdc%cmp(4:6))
  end function add_tdc

  elemental type (td_complex) function add_tdc_td(a, b)
    type (td_complex), intent(in) :: a
    type (td_real), intent(in) :: b
    call f_td_add(a%cmp(1:3), b%re, add_tdc_td%cmp(1:3))
    add_tdc_td%cmp(4:6) = a%cmp(4:6)
  end function add_tdc_td

  elemental type (td_complex) function add_td_tdc(a, b)
    type (td_real), intent(in) :: a
    type (td_complex), intent(in) :: b
    add_td_tdc = add_tdc_td(b, a)
  end function add_td_tdc

  elemental type (td_complex) function add_tdc_d(a, b)
    type (td_complex), intent(in) :: a
    real*8, intent(in) :: b
    type (td_real) :: tdb
    tdb%re(1) = b
    tdb%re(2:3) = 0.d0
    call f_td_add(a%cmp(1:3), tdb%re, add_tdc_d%cmp(1:3))
    add_tdc_d%cmp(4:6) = a%cmp(4:6)
  end function add_tdc_d

  elemental type (td_complex) function add_d_tdc(a, b)
    real*8, intent(in) :: a
    type (td_complex), intent(in) :: b
    add_d_tdc = add_tdc_d(b, a)
  end function add_d_tdc

  elemental type (td_real) function sub_td(a, b)
    type (td_real), intent(in) :: a, b
    call f_td_sub(a%re, b%re, sub_td%re)
  end function sub_td

  elemental type (td_real) function sub_td_d(a, b)
    type (td_real), intent(in) :: a
    real*8, intent(in) :: b
    call f_td_sub_td_d(a%re, b, sub_td_d%re)
  end function sub_td_d

  elemental type (td_real) function sub_d_td(a, b)
    real*8, intent(in) :: a
    type (td_real), intent(in) :: b
    call f_td_sub_d_td(a, b%re, sub_d_td%re)
  end function sub_d_td

  elemental type (td_real) function neg_td(a)
    type (td_real), intent(in) :: a
    neg_td%re = -a%re
  end function neg_td

  elemental type (td_complex) function sub_tdc(a, b)
    type (td_complex), intent(in) :: a, b
    call f_td_sub(a%cmp(1:3), b%cmp(1:3), sub_tdc%cmp(1:3))
    call f_td_sub(a%cmp(4:6), b%cmp(4:6), sub_tdc%cmp(4:6))
  end function sub_tdc

  elemental type (td_complex) function sub_tdc_td(a, b)
    type (td_complex), intent(in) :: a
    type (td_real), intent(in) :: b
    call f_td_sub(a%cmp(1:3), b%re, sub_tdc_td%cmp(1:3))
    sub_tdc_td%cmp(4:6) = a%cmp(4:6)
  end function sub_tdc_td

  elemental type (td_complex) function sub_td_tdc(a, b)
    type (td_real), intent(in) :: a
    type (td_complex), intent(in) :: b
    call f_td_sub(a%re, b%cmp(1:3), sub_td_tdc%cmp(1:3))
    sub_td_tdc%cmp(4:6) = -b%cmp(4:6)
  end function sub_td_tdc

  elemental type (td_complex) function sub_tdc_d(a, b)
    type (td_complex), intent(in) :: a
    real*8, intent(in) :: b
    type (td_real) :: tdb
    tdb%re(1) = b
    tdb%re(2:3) = 0.d0
    call f_td_sub(a%cmp(1:3), tdb%re, sub_tdc_d%cmp(1:3))
    sub_tdc_d%cmp(4:6) = a%cmp(4:6)
  end function sub_tdc_d

  elemental type (td_complex) function sub_d_tdc(a, b)
    real*8, intent(in) :: a
    type (td_complex), intent(in) :: b
    type (td_real) :: tda
    tda%re(1) = a
    tda%re(2:3) = 0.d0
    call f_td_sub(tda%re, b%cmp(1:3), sub_d_tdc%cmp(1:3))
    sub_d_tdc%cmp(4:6) = -b%cmp(4:6)
  end function sub_d_tdc

  elemental type (td_complex) function neg_tdc(a)
    type (td_complex), intent(in) :: a
    neg_tdc%cmp = -a%cmp
  end function neg_tdc

  elemental type (td_real) function mul_td(a, b)
    type (td_real), intent(in) :: a, b
    call f_td_mul(a%re, b%re, mul_td%re)
  end function mul_td

  elemental type (td_real) function mul_td_d(a, b)
    type (td_real), intent(in) :: a
    real*8, intent(in) :: b
    call f_td_mul_td_d(a%re, b, mul_td_d%re)
  end function mul_td_d

  elemental type (td_real) function mul_d_td(a, b)
    real*8, intent(in) :: a
    type (td_real), intent(in) :: b
    mul_d_td = mul_td_d(b, a)
  end function mul_d_td

  elemental type (td_real) function mul_td_i(a, b)
    type (td_real), intent(in) :: a
    integer, intent(in) :: b
    call f_td_mul_td_d(a%re, dble(b), mul_td_i%re)
  end function mul_td_i

  elemental type (td_real) function mul_i_td(a, b)
    integer, intent(in) :: a
    type (td_real), intent(in) :: b
    mul_i_td = mul_td_i(b, a)
  end function mul_i_td

  elemental type (td_complex) function mul_tdc(a, b)
    type (td_complex), intent(in) :: a, b
    type (td_real) :: t1, t2
    call f_td_mul(a%cmp(1:3), b%cmp(1:3), t1%re)
    call f_td_mul(a%cmp(4:6), b%cmp(4:6), t2%re)
    call f_td_sub(t1%re, t2%re, mul_tdc%cmp(1:3))
    call f_td_mul(a%cmp(1:3), b%cmp(4:6), t1%re)
    call f_td_mul(a%cmp(4:6), b%cmp(1:3), t2%re)
    call f_td_add(t1%re, t2%re, mul_tdc%cmp(4:6))
  end function mul_tdc

  elemental type (td_complex) function mul_tdc_td(a, b)
    type (td_complex), intent(in) :: a
    type (td_real), intent(in) :: b
    call f_td_mul(a%cmp(1:3), b%re, mul_tdc_td%cmp(1:3))
    call f_td_mul(a%cmp(4:6), b%re, mul_tdc_td%cmp(4:6))
  end function mul_tdc_td

  elemental type (td_complex) function mul_td_tdc(a, b)
    type (td_real), intent(in) :: a
    type (td_complex), intent(in) :: b
    call f_td_mul(a%re, b%cmp(1:3), mul_td_tdc%cmp(1:3))
    call f_td_mul(a%re, b%cmp(4:6), mul_td_tdc%cmp(4:6))
  end function mul_td_tdc

  elemental type (td_complex) function mul_tdc_d(a, b)
    type (td_complex), intent(in) :: a
    real*8, intent(in) :: b
    call f_td_mul_td_d(a%cmp(1:3), b, mul_tdc_d%cmp(1:3))
    call f_td_mul_td_d(a%cmp(4:6), b, mul_tdc_d%cmp(4:6))
  end function mul_tdc_d

  elemental type (td_complex) function mul_d_tdc(a, b)
    real*8, intent(in) :: a
    type (td_complex), intent(in) :: b
    call f_td_mul_td_d(b%cmp(1:3), a, mul_d_tdc%cmp(1:3))
    call f_td_mul_td_d(b%cmp(4:6), a, mul_d_tdc%cmp(4:6))
  end function mul_d_tdc

  elemental type (td_complex) function mul_tdc_i(a, b)
    type (td_complex), intent(in) :: a
    integer, intent(in) :: b
    call f_td_mul_td_d(a%cmp(1:3), dble(b), mul_tdc_i%cmp(1:3))
    call f_td_mul_td_d(a%cmp(4:6), dble(b), mul_tdc_i%cmp(4:6))
  end function mul_tdc_i

  elemental type (td_complex) function mul_i_tdc(a, b)
    integer, intent(in) :: a
    type (td_complex), intent(in) :: b
    call f_td_mul_td_d(b%cmp(1:3), dble(a), mul_i_tdc%cmp(1:3))
    call f_td_mul_td_d(b%cmp(4:6), dble(a), mul_i_tdc%cmp(4:6))
  end function mul_i_tdc

  elemental type (td_real) function div_td(a, b)
    type (td_real), intent(in) :: a, b
    call f_td_div(a%re, b%re, div_td%re)
  end function div_td

  elemental type (td_real) function div_td_d(a, b)
    type (td_real), intent(in) :: a
    real*8, intent(in) :: b
    call f_td_div_td_d(a%re, b, div_td_d%re)
  end function div_td_d

  elemental type (td_real) function div_d_td(a, b)
    real*8, intent(in) :: a
    type (td_real), intent(in) :: b
    call f_td_div_d_td(a, b%re, div_d_td%re)
  end function div_d_td

  elemental type (td_real) function div_td_i(a, b)
    type (td_real), intent(in) :: a
    integer, intent(in) :: b
    call f_td_div_td_d(a%re, dble(b), div_td_i%re)
  end function div_td_i

  elemental type (td_real) function div_i_td(a, b)
    integer, intent(in) :: a
    type (td_real), intent(in) :: b
    call f_td_div_d_td(dble(a), b%re, div_i_td%re)
  end function div_i_td

  elemental type (td_complex) function div_tdc(a, b)
    type (td_complex), intent(in) :: a, b
    type (td_real) :: t1, t2, t3, t4, t5
    call f_td_mul(a%cmp(1:3), b%cmp(1:3), t1%re)
    call f_td_mul(a%cmp(4:6), b%cmp(4:6), t2%re)
    call f_td_add(t1%re, t2%re, t3%re)
    call f_td_mul(a%cmp(1:3), b%cmp(4:6), t1%re)
    call f_td_mul(a%cmp(4:6), b%cmp(1:3), t2%re)
    call f_td_sub(t2%re, t1%re, t4%re)
    call f_td_mul(b%cmp(1:3), b%cmp(1:3), t1%re)
    call f_td_mul(b%cmp(4:6), b%cmp(4:6), t2%re)
    call f_td_add(t1%re, t2%re, t5%re)
    call f_td_div(t3%re, t5%re, div_tdc%cmp(1:3))
    call f_td_div(t4%re, t5%re, div_tdc%cmp(4:6))
  end function div_tdc

  elemental type (td_complex) function div_tdc_td(a, b)
    type (td_complex), intent(in) :: a
    type (td_real), intent(in) :: b
    call f_td_div(a%cmp(1:3), b%re, div_tdc_td%cmp(1:3))
    call f_td_div(a%cmp(4:6), b%re, div_tdc_td%cmp(4:6))
  end function div_tdc_td

  elemental type (td_complex) function div_td_tdc(a, b)
    type (td_real), intent(in) :: a
    type (td_complex), intent(in) :: b
    type (td_real) :: t1, t2, t3, t4, t5
    call f_td_mul(a%re, b%cmp(1:3), t1%re)
    call f_td_mul(a%re, b%cmp(4:6), t2%re)
    t2%re = -t2%re
    call f_td_mul(b%cmp(1:3), b%cmp(1:3), t3%re)
    call f_td_mul(b%cmp(4:6), b%cmp(4:6), t4%re)
    call f_td_add(t3%re, t4%re, t5%re)
    call f_td_div(t1%re, t5%re, div_td_tdc%cmp(1:3))
    call f_td_div(t2%re, t5%re, div_td_tdc%cmp(4:6))
  end function div_td_tdc

  elemental type (td_complex) function div_tdc_d(a, b)
    type (td_complex), intent(in) :: a
    real*8, intent(in) :: b
    call f_td_div_td_d(a%cmp(1:3), b, div_tdc_d%cmp(1:3))
    call f_td_div_td_d(a%cmp(4:6), b, div_tdc_d%cmp(4:6))
  end function div_tdc_d

  elemental type (td_real) function pwr_td(a, b)
    type (td_real), intent(in) :: a, b
    type (td_real) t1, t2
    call f_td_log(a%re, t1%re)
    call f_td_mul(t1%re, b%re, t2%re)
    call f_td_exp(t2%re, pwr_td%re)
  end function pwr_td

  elemental type (td_real) function pwr_td_i(a, n)
    type (td_real), intent(in) :: a
    integer, intent(in) :: n
    call f_td_npwr(a%re, n, pwr_td_i%re)
  end function pwr_td_i

  elemental type (td_real) function pwr_d_td(a, b)
    real*8, intent(in) :: a
    type (td_real), intent(in) :: b
    type (td_real) t1, t2, t3
    t1%re(1) = a
    t1%re(2:3) = 0.d0
    call f_td_log(t1%re, t2%re)
    call f_td_mul(t2%re, b%re, t3%re)
    call f_td_exp(t3%re, pwr_d_td%re)
  end function pwr_d_td

  elemental type (td_complex) function pwr_tdc_i(a, n)
    type (td_complex), intent(in) :: a
    integer, intent(in) :: n
    integer i2, n1
    type (td_real) t1, t2, t3
    type (td_complex) c1, c2

    intrinsic :: iabs, ishft

    if (n == 0) then
      if (all(a%cmp == 0.d0)) then
        call f_td_nan(pwr_tdc_i%cmp(1:3))
        call f_td_nan(pwr_tdc_i%cmp(4:6))
        return
      endif
      pwr_tdc_i%cmp(1) = 1.d0
      pwr_tdc_i%cmp(2:6) = 0.d0
      return
    endif

    n1 = iabs(n)
    i2 = ishft(1, n1 - 1)
    c1%cmp(1) = 1.d0
    c1%cmp(2:6) = 0.d0

110 continue
    if (n1 >= i2) then
      c2 = a * c1
      c1 = c2
      n1 = n1 - i2
    endif
    i2 = i2 / 2
    if (i2 >= 1) then
      c2 = c1 * c1
      c1 = c2
      goto 110
    endif

    if (n > 0) then
      pwr_tdc_i = c1
    else
      c1%cmp(4:6) = -c1%cmp(4:6)
      call f_td_mul(c1%cmp(1:3), c1%cmp(1:3), t1%re)
      call f_td_mul(c1%cmp(4:6), c1%cmp(4:6), t2%re)
      call f_td_add(t1%re, t2%re, t3%re)
      call f_td_div(c1%cmp(1:3), t3%re, pwr_tdc_i%cmp(1:3))
      call f_td_div(c1%cmp(4:6), t3%re, pwr_tdc_i%cmp(4:6))
    endif
  end function pwr_tdc_i

  elemental type (td_real) function tdsin(a)
    type (td_real), intent(in) :: a
    call f_td_sin(a%re, tdsin%re)
  end function tdsin

  elemental type (td_real) function tdcos(a)
    type (td_real), intent(in) :: a
    call f_td_cos(a%re, tdcos%re)
  end function tdcos

  elemental type (td_real) function tdtan(a)
    type (td_real), intent(in) :: a
    call f_td_tan(a%re, tdtan%re)
  end function tdtan

  subroutine tdsincos(a, s, c)
    type (td_real), intent(in) :: a
    type (td_real), intent(out) :: s, c
    call f_td_sincos(a%re, s%re, c%re)
  end subroutine tdsincos

  elemental type (td_real) function tdasin(a)
    type (td_real), intent(in) :: a
    call f_td_asin(a%re, tdasin%re)
  end function tdasin

  elemental type (td_real) function tdacos(a)
    type (td_real), intent(in) :: a
    call f_td_acos(a%re, tdacos%re)
  end function tdacos

  elemental type (td_real) function tdatan(a)
    type (td_real), intent(in) :: a
    call f_td_atan(a%re, tdatan%re)
  end function tdatan

  elemental type (td_real) function tdatan2(a, b)
    type (td_real), intent(in) :: a, b
    call f_td_atan2(a%re, b%re, tdatan2%re)
  end function tdatan2

  elemental type (td_real) function tdexp(a)
    type (td_real), intent(in) :: a
    call f_td_exp(a%re, tdexp%re)
  end function tdexp

  elemental type (td_complex) function tdcexp(a)
    type (td_complex), intent(in) :: a
    type (td_real) :: t1, t2, t3
    call f_td_exp(a%cmp(1:3), t1%re)
    call f_td_sincos(a%cmp(4:6), t3%re, t2%re)
    call f_td_mul(t1%re, t2%re, tdcexp%cmp(1:3))
    call f_td_mul(t1%re, t3%re, tdcexp%cmp(4:6))
  end function tdcexp

  elemental type (td_real) function tdlog(a)
    type (td_real), intent(in) :: a
    call f_td_log(a%re, tdlog%re)
  end function tdlog

  elemental type (td_complex) function tdclog(a)
    type (td_complex), intent(in) :: a
    type (td_real) :: t1, t2, t3
    call f_td_mul(a%cmp(1:3), a%cmp(1:3), t1%re)
    call f_td_mul(a%cmp(4:6), a%cmp(4:6), t2%re)
    call f_td_add(t1%re, t2%re, t3%re)
    call f_td_log(t3%re, t1%re)
    tdclog%cmp(1:3) = 0.5d0 * t1%re
    call f_td_atan2(a%cmp(4:6), a%cmp(1:3), tdclog%cmp(4:6))
  end function tdclog

  elemental type (td_real) function tdlog10(a)
    type (td_real), intent(in) :: a
    call f_td_log10(a%re, tdlog10%re)
  end function tdlog10

  elemental type (td_real) function tdsqrt(a)
    type (td_real), intent(in) :: a
    call f_td_sqrt(a%re, tdsqrt%re)
  end function tdsqrt

  elemental type (td_real) function tdsqr(a)
    type (td_real), intent(in) :: a
    call f_td_sqr(a%re, tdsqr%re)
  end function tdsqr

  elemental type (td_real) function tdnroot(a, n)
    type (td_real), intent(in) :: a
    integer, intent(in) :: n
    call f_td_nroot(a%re, n, tdnroot%re)
  end function tdnroot

  elemental type (td_real) function tdsinh(a)
    type (td_real), intent(in) :: a
    call f_td_sinh(a%re, tdsinh%re)
  end function tdsinh

  elemental type (td_real) function tdcosh(a)
    type (td_real), intent(in) :: a
    call f_td_cosh(a%re, tdcosh%re)
  end function tdcosh

  elemental type (td_real) function tdtanh(a)
    type (td_real), intent(in) :: a
    call f_td_tanh(a%re, tdtanh%re)
  end function tdtanh

  subroutine tdsincosh(a, s, c)
    type (td_real), intent(in) :: a
    type (td_real), intent(out) :: s, c
    call f_td_sincosh(a%re, s%re, c%re)
  end subroutine tdsincosh

  elemental type (td_real) function tdasinh(a)
    type (td_real), intent(in) :: a
    call f_td_asinh(a%re, tdasinh%re)
  end function tdasinh

  elemental type (td_real) function tdacosh(a)
    type (td_real), intent(in) :: a
    call f_td_acosh(a%re, tdacosh%re)
  end function tdacosh

  elemental type (td_real) function tdatanh(a)
    type (td_real), intent(in) :: a
    call f_td_atanh(a%re, tdatanh%re)
  end function tdatanh

  elemental type (td_real) function tdaint(a)
    type (td_real), intent(in) :: a
    call f_td_aint(a%re, tdaint%re)
  end function tdaint

  elemental type (td_real) function tdfloor(a)
    type (td_real), intent(in) :: a
    call f_td_floor(a%re, tdfloor%re)
  end function tdfloor

  elemental type (td_real) function tdceil(a)
    type (td_real), intent(in) :: a
    call f_td_ceil(a%re, tdceil%re)
  end function tdceil

  elemental type (td_real) function tdanint(a)
    type (td_real), intent(in) :: a
    call f_td_nint(a%re, tdanint%re)
  end function tdanint

  elemental integer function tdnint(a)
    type (td_real), intent(in) :: a
    tdnint = to_int_td(tdanint(a))
  end function tdnint

  elemental type (td_real) function tdabs(a)
    type (td_real), intent(in) :: a
    call f_td_abs(a%re, tdabs%re)
  end function tdabs

  elemental type (td_real) function tdcabs(tdc)
    type (td_complex), intent(in) :: tdc
    type (td_real) :: t1, t2, t3
    call f_td_mul(tdc%cmp(1:3), tdc%cmp(1:3), t1%re)
    call f_td_mul(tdc%cmp(4:6), tdc%cmp(4:6), t2%re)
    call f_td_add(t1%re, t2%re, t3%re)
    call f_td_sqrt(t3%re, tdcabs%re)
  end function tdcabs

  elemental type (td_real) function tdsign(a, b) result (c)
    type (td_real), intent(in) :: a, b
    if (b%re(1) .gt. 0.0d0) then
      if (a%re(1) .gt. 0.0d0) then
        c%re = a%re
      else
        c%re = -a%re
      end if
    else
      if (a%re(1) .gt. 0.0d0) then
        c%re = -a%re
      else
        c%re = a%re
      end if
    endif
  end function tdsign

  elemental type (td_real) function tdsign_td_d(a, b) result (c)
    type (td_real), intent(in) :: a
    real*8, intent(in) :: b
    if (b .gt. 0.0d0) then
      if (a%re(1) .gt. 0.0d0) then
        c%re = a%re
      else
        c%re = -a%re
      end if
    else
      if (a%re(1) .gt. 0.0d0) then
        c%re = -a%re
      else
        c%re = a%re
      end if
    endif
  end function tdsign_td_d

  subroutine tdrand(harvest)
    type (td_real), intent(out) :: harvest
    call f_td_rand(harvest%re)
  end subroutine tdrand

  elemental logical function eq_td(a, b)
    type (td_real), intent(in) :: a, b
    integer :: r
    call f_td_comp(a%re, b%re, r)
    eq_td = (r == 0)
  end function eq_td

  elemental logical function eq_td_d(a, b)
    type (td_real), intent(in) :: a
    real*8, intent(in) :: b
    integer :: r
    call f_td_comp_td_d(a%re, b, r)
    eq_td_d = (r == 0)
  end function eq_td_d

  elemental logical function eq_d_td(a, b)
    real*8, intent(in) :: a
    type (td_real), intent(in) :: b
    integer :: r
    call f_td_comp_d_td(a, b%re, r)
    eq_d_td = (r == 0)
  end function eq_d_td

  elemental logical function eq_td_i(a, b)
    type (td_real), intent(in) :: a
    integer, intent(in) :: b
    eq_td_i = eq_td_d(a, dble(b))
  end function eq_td_i

  elemental logical function eq_i_td(a, b)
    integer, intent(in) :: a
    type (td_real), intent(in) :: b
    eq_i_td = eq_d_td(dble(a), b)
  end function eq_i_td

  elemental logical function eq_tdc(a, b)
    type (td_complex), intent(in) :: a, b
    integer :: i1, i2
    call f_td_comp(a%cmp(1:3), b%cmp(1:3), i1)
    call f_td_comp(a%cmp(4:6), b%cmp(4:6), i2)
    eq_tdc = (i1 == 0 .and. i2 == 0)
  end function eq_tdc

  elemental logical function eq_tdc_td(a, b)
    type (td_complex), intent(in) :: a
    type (td_real), intent(in) :: b
    integer :: i1
    call f_td_comp(a%cmp(1:3), b%re, i1)
    eq_tdc_td = (i1 == 0 .and. all(a%cmp(4:6) == 0.d0))
  end function eq_tdc_td

  elemental logical function eq_td_tdc(a, b)
    type (td_real), intent(in) :: a
    type (td_complex), intent(in) :: b
    integer :: i1
    call f_td_comp(a%re, b%cmp(1:3), i1)
    eq_td_tdc = (i1 == 0 .and. all(b%cmp(4:6) == 0.d0))
  end function eq_td_tdc

  elemental logical function ne_td(a, b)
    type (td_real), intent(in) :: a, b
    ne_td = .not. eq_td(a, b)
  end function ne_td

  elemental logical function ne_td_d(a, b)
    type (td_real), intent(in) :: a
    real*8, intent(in) :: b
    ne_td_d = .not. eq_td_d(a, b)
  end function ne_td_d

  elemental logical function ne_d_td(a, b)
    real*8, intent(in) :: a
    type (td_real), intent(in) :: b
    ne_d_td = .not. eq_d_td(a, b)
  end function ne_d_td

  elemental logical function ne_td_i(a, b)
    type (td_real), intent(in) :: a
    integer, intent(in) :: b
    ne_td_i = .not. eq_td_i(a, b)
  end function ne_td_i

  elemental logical function ne_i_td(a, b)
    integer, intent(in) :: a
    type (td_real), intent(in) :: b
    ne_i_td = .not. eq_i_td(a, b)
  end function ne_i_td

  elemental logical function ne_tdc(a, b)
    type (td_complex), intent(in) :: a, b
    ne_tdc = .not. eq_tdc(a, b)
  end function ne_tdc

  elemental logical function ne_tdc_td(a, b)
    type (td_complex), intent(in) :: a
    type (td_real), intent(in) :: b
    ne_tdc_td = .not. eq_tdc_td(a, b)
  end function ne_tdc_td

  elemental logical function ne_td_tdc(a, b)
    type (td_real), intent(in) :: a
    type (td_complex), intent(in) :: b
    ne_td_tdc = .not. eq_td_tdc(a, b)
  end function ne_td_tdc

  elemental logical function gt_td(a, b)
    type (td_real), intent(in) :: a, b
    integer :: r
    call f_td_comp(a%re, b%re, r)
    gt_td = (r == 1)
  end function gt_td

  elemental logical function gt_td_d(a, b)
    type (td_real), intent(in) :: a
    real*8, intent(in) :: b
    integer :: r
    call f_td_comp_td_d(a%re, b, r)
    gt_td_d = (r == 1)
  end function gt_td_d

  elemental logical function gt_d_td(a, b)
    real*8, intent(in) :: a
    type (td_real), intent(in) :: b
    integer :: r
    call f_td_comp_d_td(a, b%re, r)
    gt_d_td = (r == 1)
  end function gt_d_td

  elemental logical function gt_td_i(a, b)
    type (td_real), intent(in) :: a
    integer, intent(in) :: b
    gt_td_i = gt_td_d(a, dble(b))
  end function gt_td_i

  elemental logical function gt_i_td(a, b)
    integer, intent(in) :: a
    type (td_real), intent(in) :: b
    gt_i_td = gt_d_td(dble(a), b)
  end function gt_i_td

  elemental logical function lt_td(a, b)
    type (td_real), intent(in) :: a, b
    integer :: r
    call f_td_comp(a%re, b%re, r)
    lt_td = (r == -1)
  end function lt_td

  elemental logical function lt_td_d(a, b)
    type (td_real), intent(in) :: a
    real*8, intent(in) :: b
    integer :: r
    call f_td_comp_td_d(a%re, b, r)
    lt_td_d = (r == -1)
  end function lt_td_d

  elemental logical function lt_d_td(a, b)
    real*8, intent(in) :: a
    type (td_real), intent(in) :: b
    integer :: r
    call f_td_comp_d_td(a, b%re, r)
    lt_d_td = (r == -1)
  end function lt_d_td

  elemental logical function lt_td_i(a, b)
    type (td_real), intent(in) :: a
    integer, intent(in) :: b
    lt_td_i = lt_td_d(a, dble(b))
  end function lt_td_i

  elemental logical function lt_i_td(a, b)
    integer, intent(in) :: a
    type (td_real), intent(in) :: b
    lt_i_td = lt_d_td(dble(a), b)
  end function lt_i_td

  elemental logical function ge_td(a, b)
    type (td_real), intent(in) :: a, b
    integer :: r
    call f_td_comp(a%re, b%re, r)
    ge_td = (r >= 0)
  end function ge_td

  elemental logical function ge_td_d(a, b)
    type (td_real), intent(in) :: a
    real*8, intent(in) :: b
    integer :: r
    call f_td_comp_td_d(a%re, b, r)
    ge_td_d = (r >= 0)
  end function ge_td_d

  elemental logical function ge_d_td(a, b)
    real*8, intent(in) :: a
    type (td_real), intent(in) :: b
    integer :: r
    call f_td_comp_d_td(a, b%re, r)
    ge_d_td = (r >= 0)
  end function ge_d_td

  elemental logical function ge_td_i(a, b)
    type (td_real), intent(in) :: a
    integer, intent(in) :: b
    ge_td_i = ge_td_d(a, dble(b))
  end function ge_td_i

  elemental logical function ge_i_td(a, b)
    integer, intent(in) :: a
    type (td_real), intent(in) :: b
    ge_i_td = ge_d_td(dble(a), b)
  end function ge_i_td

  elemental logical function le_td(a, b)
    type (td_real), intent(in) :: a, b
    integer :: r
    call f_td_comp(a%re, b%re, r)
    le_td = (r <= 0)
  end function le_td

  elemental logical function le_td_d(a, b)
    type (td_real), intent(in) :: a
    real*8, intent(in) :: b
    integer :: r
    call f_td_comp_td_d(a%re, b, r)
    le_td_d = (r <= 0)
  end function le_td_d

  elemental logical function le_d_td(a, b)
    real*8, intent(in) :: a
    type (td_real), intent(in) :: b
    integer :: r
    call f_td_comp_d_td(a, b%re, r)
    le_d_td = (r <= 0)
  end function le_d_td

  elemental logical function le_td_i(a, b)
    type (td_real), intent(in) :: a
    integer, intent(in) :: b
    le_td_i = le_td_d(a, dble(b))
  end function le_td_i

  elemental logical function le_i_td(a, b)
    integer, intent(in) :: a
    type (td_real), intent(in) :: b
    le_i_td = le_d_td(dble(a), b)
  end function le_i_td

  subroutine tdinpq(u, q1, q2, q3, q4, q5, q6, q7, q8, q9)
    integer, intent(in) :: u
    type (td_real), intent(inout) :: q1
    type (td_real), intent(inout), optional :: q2, q3, q4, q5, q6, q7, q8, q9

    call tdinp(u, q1%re)
    if (present(q2)) call tdinp(u, q2%re)
    if (present(q3)) call tdinp(u, q3%re)
    if (present(q4)) call tdinp(u, q4%re)
    if (present(q5)) call tdinp(u, q5%re)
    if (present(q6)) call tdinp(u, q6%re)
    if (present(q7)) call tdinp(u, q7%re)
    if (present(q8)) call tdinp(u, q8%re)
    if (present(q9)) call tdinp(u, q9%re)
  end subroutine tdinpq

  subroutine tdcinpq(u, q1, q2, q3, q4, q5, q6, q7, q8, q9)
    integer, intent(in) :: u
    type (td_complex), intent(inout) :: q1
    type (td_complex), intent(inout), optional :: q2, q3, q4, q5, q6, q7, q8, q9

    call tdinp(u, q1%cmp(1:3))
    call tdinp(u, q1%cmp(4:6))
    if (present(q2)) then
      call tdinp(u, q2%cmp(1:3))
      call tdinp(u, q2%cmp(4:6))
    end if
    if (present(q3)) then
      call tdinp(u, q3%cmp(1:3))
      call tdinp(u, q3%cmp(4:6))
    end if
    if (present(q4)) then
      call tdinp(u, q4%cmp(1:3))
      call tdinp(u, q4%cmp(4:6))
    end if
    if (present(q5)) then
      call tdinp(u, q5%cmp(1:3))
      call tdinp(u, q5%cmp(4:6))
    end if
    if (present(q6)) then
      call tdinp(u, q6%cmp(1:3))
      call tdinp(u, q6%cmp(4:6))
    end if
    if (present(q7)) then
      call tdinp(u, q7%cmp(1:3))
      call tdinp(u, q7%cmp(4:6))
    end if
    if (present(q8)) then
      call tdinp(u, q8%cmp(1:3))
      call tdinp(u, q8%cmp(4:6))
    end if
    if (present(q9)) then
      call tdinp(u, q9%cmp(1:3))
      call tdinp(u, q9%cmp(4:6))
    end if
  end subroutine tdcinpq

  subroutine tdoutq(u, q1, q2, q3, q4, q5, q6, q7, q8, q9)
    integer, intent(in) :: u
    type (td_real), intent(in) :: q1
    type (td_real), intent(in), optional :: q2, q3, q4, q5, q6, q7, q8, q9

    call tdout(u, q1%re)
    if (present(q2)) call tdout(u, q2%re)
    if (present(q3)) call tdout(u, q3%re)
    if (present(q4)) call tdout(u, q4%re)
    if (present(q5)) call tdout(u, q5%re)
    if (present(q6)) call tdout(u, q6%re)
    if (present(q7)) call tdout(u, q7%re)
    if (present(q8)) call tdout(u, q8%re)
    if (present(q9)) call tdout(u, q9%re)
  end subroutine tdoutq

  subroutine tdcoutq(u, q1, q2, q3, q4, q5, q6, q7, q8, q9)
    integer, intent(in) :: u
    type (td_complex), intent(in) :: q1
    type (td_complex), intent(in), optional :: q2, q3, q4, q5, q6, q7, q8, q9

    call tdout(u, q1%cmp(1:3))
    call tdout(u, q1%cmp(4:6))
    if (present(q2)) then
      call tdout(u, q2%cmp(1:3))
      call tdout(u, q2%cmp(4:6))
    end if
    if (present(q3)) then
      call tdout(u, q3%cmp(1:3))
      call tdout(u, q3%cmp(4:6))
    end if
    if (present(q4)) then
      call tdout(u, q4%cmp(1:3))
      call tdout(u, q4%cmp(4:6))
    end if
    if (present(q5)) then
      call tdout(u, q5%cmp(1:3))
      call tdout(u, q5%cmp(4:6))
    end if
    if (present(q6)) then
      call tdout(u, q6%cmp(1:3))
      call tdout(u, q6%cmp(4:6))
    end if
    if (present(q7)) then
      call tdout(u, q7%cmp(1:3))
      call tdout(u, q7%cmp(4:6))
    end if
    if (present(q8)) then
      call tdout(u, q8%cmp(1:3))
      call tdout(u, q8%cmp(4:6))
    end if
    if (present(q9)) then
      call tdout(u, q9%cmp(1:3))
      call tdout(u, q9%cmp(4:6))
    end if
  end subroutine tdcoutq

  pure type (td_real) function td_pi()
    call f_td_pi(td_pi%re)
  end function td_pi

  elemental type (td_real) function tdhuge(a)
    type (td_real), intent(in) :: a
    tdhuge = td_huge
  end function tdhuge

  elemental type (td_real) function td_safe_huge(a)
    type (td_real), intent(in) :: a
    td_safe_huge = td_real((/ &
      1.7976931080746007281d+308, 9.97920154767359795037d+291, &
      5.53956966280111259858d+275/))
  end function td_safe_huge

  elemental type (td_real) function tdtiny(a)
    type (td_real), intent(in) :: a
    tdtiny = td_tiny
  end function tdtiny

  elemental type (td_real) function tdepsilon(a)
    type (td_real), intent(in) :: a
    tdepsilon = td_eps
  end function tdepsilon

  elemental integer function td_radix(a)
    type (td_real), intent(in) :: a
    td_radix = 2
  end function td_radix

  elemental integer function td_digits(a)
    type (td_real), intent(in) :: a
    td_digits = 157
  end function td_digits

  elemental integer function td_max_expn(a)
    type (td_real), intent(in) :: a
    td_max_expn = 1023
  end function td_max_expn

  elemental integer function td_min_expn(a)
    type (td_real), intent(in) :: a
    td_min_expn = -915
  end function td_min_expn

  elemental integer function td_precision(a)
    type (td_real), intent(in) :: a
    td_precision = 47
  end function td_precision

  elemental integer function td_range(a)
    type (td_real), intent(in) :: a
    td_range = 276
  end function td_range

  elemental type (td_real) function td_nan(a)
    type (td_real), intent(in) :: a
    call f_td_nan(td_nan%re)
  end function td_nan

  elemental type (td_real) function td_aimag(a)
    type (td_complex), intent(in) :: a
    td_aimag%re = a%cmp(4:6)
  end function td_aimag

  elemental type (td_real) function tdmin2(a, b)
    type (td_real), intent(in) :: a, b
    integer :: r
    call f_td_comp(a%re, b%re, r)
    if (r <= 0) then
      tdmin2 = a
    else
      tdmin2 = b
    end if
  end function tdmin2

  elemental type (td_real) function tdmin(a1, a2, a3, a4, a5, a6, a7, a8, a9)
    type (td_real), intent(in) :: a1, a2, a3
    type (td_real), intent(in), optional :: a4, a5, a6, a7, a8, a9
    tdmin = tdmin2(tdmin2(a1, a2), a3)
    if (present(a4)) tdmin = tdmin2(tdmin, a4)
    if (present(a5)) tdmin = tdmin2(tdmin, a5)
    if (present(a6)) tdmin = tdmin2(tdmin, a6)
    if (present(a7)) tdmin = tdmin2(tdmin, a7)
    if (present(a8)) tdmin = tdmin2(tdmin, a8)
    if (present(a9)) tdmin = tdmin2(tdmin, a9)
  end function tdmin

  elemental type (td_real) function tdmax2(a, b)
    type (td_real), intent(in) :: a, b
    integer :: r
    call f_td_comp(a%re, b%re, r)
    if (r >= 0) then
      tdmax2 = a
    else
      tdmax2 = b
    end if
  end function tdmax2

  elemental type (td_real) function tdmax(a1, a2, a3, a4, a5, a6, a7, a8, a9)
    type (td_real), intent(in) :: a1, a2, a3
    type (td_real), intent(in), optional :: a4, a5, a6, a7, a8, a9
    tdmax = tdmax2(tdmax2(a1, a2), a3)
    if (present(a4)) tdmax = tdmax2(tdmax, a4)
    if (present(a5)) tdmax = tdmax2(tdmax, a5)
    if (present(a6)) tdmax = tdmax2(tdmax, a6)
    if (present(a7)) tdmax = tdmax2(tdmax, a7)
    if (present(a8)) tdmax = tdmax2(tdmax, a8)
    if (present(a9)) tdmax = tdmax2(tdmax, a9)
  end function tdmax

  elemental type (td_real) function tdmod(a, b)
    type (td_real), intent(in) :: a, b
    type (td_real) :: s1, s2
    call f_td_div(a%re, b%re, s1%re)
    call f_td_aint(s1%re, s2%re)
    call f_td_mul(s2%re, b%re, s1%re)
    call f_td_sub(a%re, s1%re, tdmod%re)
  end function tdmod

subroutine tdinp(iu, a)
  implicit none
  integer iu
  character*80 cs
  real*8 a(3)

  read (iu, '(a)', end = 100) cs
  call tdinpc(cs, a)
  goto 110

100 write (6, 1)
1 format ('*** tdinp: End-of-file encountered.')
  stop

110 return
end subroutine

subroutine tdinpc(a, b)
  implicit none
  integer i, id, ie, inz, ip, is, k, ln, lnn, beg
  parameter (ln = 80)
  real*8 bi
  character*80 a
  character*1 ai
  character*10 dig
  character*16 ca
  parameter (dig = '0123456789')
  real*8 b(3), f(3), s0(3), s1(3), s2(3)

  id = 0
  ip = -1
  is = 0
  inz = 0
  s1(1) = 0.d0
  s1(2) = 0.d0
  s1(3) = 0.d0

  beg = 0
  do i = 1, 80
    if (a(i:i) /= ' ') then
      beg = i
      goto 80
    end if
  end do

  goto 210
80 continue

  do i = beg, 80
    if (a(i:i) == ' ') then
      lnn = i - 1
      goto 90
    end if
  enddo

  lnn = 80
90 continue

  do i = beg, lnn
    ai = a(i:i)
    if (ai .eq. '.') then
      if (ip >= 0) goto 210
      ip = id
      inz = 1
    elseif (ai .eq. '+') then
      if (id .ne. 0 .or. ip >= 0 .or. is .ne. 0) goto 210
      is = 1
    elseif (ai .eq. '-') then
      if (id .ne. 0 .or. ip >= 0 .or. is .ne. 0) goto 210
      is = -1
    elseif (ai .eq. 'e' .or. ai .eq. 'E' .or. ai .eq. 'd' .or. ai .eq. 'D') then
      goto 100
    elseif (index(dig, ai) .eq. 0) then
      goto 210
    else
      bi = index(dig, ai) - 1
      if (inz > 0 .or. bi > 0.d0) then
        inz = 1
        id = id + 1
        call f_td_mul_td_d(s1, 10.d0, s0)
        f(1) = bi
        f(2) = 0.d0
        f(3) = 0.d0
        call f_td_add(s0, f, s1)
      endif
    endif
  enddo

100 continue
  if (is .eq. -1) then
    s1(1) = -s1(1)
    s1(2) = -s1(2)
    s1(3) = -s1(3)
  endif
  k = i
  if (ip == -1) ip = id
  ie = 0
  is = 0
  ca = ' '

  do i = k + 1, lnn
    ai = a(i:i)
    if (ai .eq. ' ') then
    elseif (ai .eq. '+') then
      if (ie .ne. 0 .or. is .ne. 0) goto 210
      is = 1
    elseif (ai .eq. '-') then
      if (ie .ne. 0 .or. is .ne. 0) goto 210
      is = -1
    elseif (index(dig, ai) .eq. 0) then
      goto 210
    else
      ie = ie + 1
      if (ie .gt. 3) goto 210
      ca(ie:ie) = ai
    endif
  enddo

  ie = dddigin(ca, 4)
  if (is .eq. -1) ie = -ie
  ie = ie + ip - id
  s0(1) = 10.d0
  s0(2) = 0.d0
  s0(3) = 0.d0
  call f_td_npwr(s0, ie, s2)
  call f_td_mul(s1, s2, b)
  goto 220

210 write (6, 1) a
1 format ('*** tdinpc: Syntax error in literal string: ', a)
  stop

220 return
end subroutine

subroutine tdout(iu, a)
  implicit none
  integer iu
  character cs(57)
  real*8 a(3)

  call tdoutc(a, cs)
  write (iu, '(57a)') cs
end subroutine

subroutine tdoutc(a, b)
  implicit none
  real*8 a(3)
  character b(57)

  b(1) = ' '
  b(2) = ' '
  call f_td_swrite(a, 47, b(3), 55)
end subroutine

  real*8 function dddigin(ca, n)
    implicit none
    real*8 d1
    character*(*), ca
    character*16 digits
    integer i, k, n
    parameter (digits = '0123456789')

    d1 = 0.d0
    do i = 1, n
      k = index(digits, ca(i:i)) - 1
      if (k < 0) then
        write (6, *) 'dddigin: non-digit in character string'
      elseif (k <= 9) then
        d1 = 10.d0 * d1 + k
      endif
    enddo
    dddigin = d1
  end function dddigin

end module tdmodule
