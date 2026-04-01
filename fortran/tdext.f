! module file describing the C++-to-Fortran interface found in f_td.cpp.

module tdext
  implicit none

  interface
    pure subroutine f_td_add(a, b, c)
      real*8, intent(in) :: a(3), b(3)
      real*8, intent(out) :: c(3)
    end subroutine

    pure subroutine f_td_add_td_d(a, b, c)
      real*8, intent(in) :: a(3), b
      real*8, intent(out) :: c(3)
    end subroutine

    pure subroutine f_td_sub(a, b, c)
      real*8, intent(in) :: a(3), b(3)
      real*8, intent(out) :: c(3)
    end subroutine

    pure subroutine f_td_sub_td_d(a, b, c)
      real*8, intent(in) :: a(3), b
      real*8, intent(out) :: c(3)
    end subroutine

    pure subroutine f_td_sub_d_td(a, b, c)
      real*8, intent(in) :: a, b(3)
      real*8, intent(out) :: c(3)
    end subroutine

    pure subroutine f_td_mul(a, b, c)
      real*8, intent(in) :: a(3), b(3)
      real*8, intent(out) :: c(3)
    end subroutine

    pure subroutine f_td_mul_td_d(a, b, c)
      real*8, intent(in) :: a(3), b
      real*8, intent(out) :: c(3)
    end subroutine

    pure subroutine f_td_div(a, b, c)
      real*8, intent(in) :: a(3), b(3)
      real*8, intent(out) :: c(3)
    end subroutine

    pure subroutine f_td_div_td_d(a, b, c)
      real*8, intent(in) :: a(3), b
      real*8, intent(out) :: c(3)
    end subroutine

    pure subroutine f_td_div_d_td(a, b, c)
      real*8, intent(in) :: a, b(3)
      real*8, intent(out) :: c(3)
    end subroutine

    pure subroutine f_td_sqrt(a, b)
      real*8, intent(in) :: a(3)
      real*8, intent(out) :: b(3)
    end subroutine

    pure subroutine f_td_sqr(a, b)
      real*8, intent(in) :: a(3)
      real*8, intent(out) :: b(3)
    end subroutine

    pure subroutine f_td_abs(a, b)
      real*8, intent(in) :: a(3)
      real*8, intent(out) :: b(3)
    end subroutine

    pure subroutine f_td_npwr(a, n, b)
      real*8, intent(in) :: a(3)
      integer, intent(in) :: n
      real*8, intent(out) :: b(3)
    end subroutine

    pure subroutine f_td_nroot(a, n, b)
      real*8, intent(in) :: a(3)
      integer, intent(in) :: n
      real*8, intent(out) :: b(3)
    end subroutine

    pure subroutine f_td_nint(a, b)
      real*8, intent(in) :: a(3)
      real*8, intent(out) :: b(3)
    end subroutine

    pure subroutine f_td_log(a, b)
      real*8, intent(in) :: a(3)
      real*8, intent(out) :: b(3)
    end subroutine

    pure subroutine f_td_log10(a, b)
      real*8, intent(in) :: a(3)
      real*8, intent(out) :: b(3)
    end subroutine

    pure subroutine f_td_exp(a, b)
      real*8, intent(in) :: a(3)
      real*8, intent(out) :: b(3)
    end subroutine

    pure subroutine f_td_sin(a, b)
      real*8, intent(in) :: a(3)
      real*8, intent(out) :: b(3)
    end subroutine

    pure subroutine f_td_cos(a, b)
      real*8, intent(in) :: a(3)
      real*8, intent(out) :: b(3)
    end subroutine

    pure subroutine f_td_tan(a, b)
      real*8, intent(in) :: a(3)
      real*8, intent(out) :: b(3)
    end subroutine

    pure subroutine f_td_asin(a, b)
      real*8, intent(in) :: a(3)
      real*8, intent(out) :: b(3)
    end subroutine

    pure subroutine f_td_acos(a, b)
      real*8, intent(in) :: a(3)
      real*8, intent(out) :: b(3)
    end subroutine

    pure subroutine f_td_atan(a, b)
      real*8, intent(in) :: a(3)
      real*8, intent(out) :: b(3)
    end subroutine

    pure subroutine f_td_atan2(a, b, c)
      real*8, intent(in) :: a(3), b(3)
      real*8, intent(out) :: c(3)
    end subroutine

    pure subroutine f_td_sinh(a, b)
      real*8, intent(in) :: a(3)
      real*8, intent(out) :: b(3)
    end subroutine

    pure subroutine f_td_cosh(a, b)
      real*8, intent(in) :: a(3)
      real*8, intent(out) :: b(3)
    end subroutine

    pure subroutine f_td_tanh(a, b)
      real*8, intent(in) :: a(3)
      real*8, intent(out) :: b(3)
    end subroutine

    pure subroutine f_td_asinh(a, b)
      real*8, intent(in) :: a(3)
      real*8, intent(out) :: b(3)
    end subroutine

    pure subroutine f_td_acosh(a, b)
      real*8, intent(in) :: a(3)
      real*8, intent(out) :: b(3)
    end subroutine

    pure subroutine f_td_atanh(a, b)
      real*8, intent(in) :: a(3)
      real*8, intent(out) :: b(3)
    end subroutine

    pure subroutine f_td_sincos(a, s, c)
      real*8, intent(in) :: a(3)
      real*8, intent(out) :: s(3), c(3)
    end subroutine

    pure subroutine f_td_sincosh(a, s, c)
      real*8, intent(in) :: a(3)
      real*8, intent(out) :: s(3), c(3)
    end subroutine

    subroutine f_td_swrite(a, prec, str, maxlen)
      real*8, intent(in) :: a(3)
      integer, intent(in) :: prec, maxlen
      character, intent(out) :: str(maxlen)
    end subroutine

    pure subroutine f_td_comp(a, b, r)
      real*8, intent(in) :: a(3), b(3)
      integer, intent(out) :: r
    end subroutine

    pure subroutine f_td_comp_td_d(a, b, r)
      real*8, intent(in) :: a(3), b
      integer, intent(out) :: r
    end subroutine

    pure subroutine f_td_comp_d_td(a, b, r)
      real*8, intent(in) :: a, b(3)
      integer, intent(out) :: r
    end subroutine

    pure subroutine f_td_pi(a)
      real*8, intent(out) :: a(3)
    end subroutine

    pure subroutine f_td_nan(a)
      real*8, intent(out) :: a(3)
    end subroutine
  end interface
end
