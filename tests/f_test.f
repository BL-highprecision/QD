! program fortran_test
subroutine f_main
! A simple test of the fortran wrappers

  use ddmodule
  use qdmodule
  use tdmodule
  implicit none
  integer*4 old_cw
  integer i
  type (dd_real) ddx
  type (td_real) tx, ty
  type (qd_real) x, y, z

  call f_fpu_fix_start (old_cw)

  ! Test for read/write
  z = "3.14159265358979323846264338327950288419716939937510582097494459230"
  call write_scalar(6, z)

  ! Test for atan/write
  do i=1,3
    x = qdreal(dble(i))
    call write_scalar(6, x)
    y = atan(x)
    call write_scalar(6, y)
  end do

  call write_scalar(6, nan(x))
  call write_scalar(6, qdcomplex(x, y))

  tx = tdreal("1.4142135623730950488016887242096980785696718753769")
  call write_scalar(6, tx)
  ddx = tx
  y = tx
  ty = ddx
  if (abs(real(ty) - 1.4142135623730951d0) .gt. 1.0d-30) stop 11
  ty = y
  if (abs(real(ty) - 1.4142135623730951d0) .gt. 1.0d-30) stop 12

  tx = exp(log(tdreal(2.0d0)))
  if (abs(tx - tdreal(2.0d0)) .gt. tdreal("1.0d-40")) stop 13

  ty = tdreal("1.25d0")
  ddx = ty
  y = ty
  if (abs(ddx%re(1) - 1.25d0) .gt. 1.0d-30) stop 14
  if (abs(y%re(1) - 1.25d0) .gt. 1.0d-30) stop 15

  call f_fpu_fix_end (old_cw)
end
