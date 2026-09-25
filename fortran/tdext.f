! module file describing the C++-to-Fortran interface found in f_td.cpp.
!
! Copyright (c) 2026, Nakata Maho
! All rights reserved.
!
! Redistribution and use in source and binary forms, with or without
! modification, are permitted provided that the following conditions are met:
!
! 1. Redistributions of source code must retain the above copyright notice,
!    this list of conditions and the following disclaimer.
! 2. Redistributions in binary form must reproduce the above copyright notice,
!    this list of conditions and the following disclaimer in the documentation
!    and/or other materials provided with the distribution.
!
! THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
! AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
! IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
! ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
! LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
! CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
! SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
! INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
! CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
! ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
! POSSIBILITY OF SUCH DAMAGE.

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

    pure subroutine f_td_aint(a, b)
      real*8, intent(in) :: a(3)
      real*8, intent(out) :: b(3)
    end subroutine

    pure subroutine f_td_floor(a, b)
      real*8, intent(in) :: a(3)
      real*8, intent(out) :: b(3)
    end subroutine

    pure subroutine f_td_ceil(a, b)
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

    subroutine f_td_rand(a)
      real*8, intent(out) :: a(3)
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
