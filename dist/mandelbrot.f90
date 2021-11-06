! ------------------------------------------------------------------------------
! ------------------------------------------------------------------------------
! Author
!          Levi Overcast
! Date
!           Nov 5, 2021
! ------------------------------------------------------------------------------
! Description
!          A simple program to write a colored Mandelbrot set approximation to
!   a TGA image file. Implemented in modern Fortran with OpenMP multithreading.
! ------------------------------------------------------------------------------
! ------------------------------------------------------------------------------

! ------------------------------------------------------------------------------
!  ~~~ Helper Module ~~~
! ------------------------------------------------------------------------------

module m_compute_mandelbrot

  ! --------------------------
  !  Global Data & Parameters
  ! --------------------------

  implicit none                                             ! explicit type only
  integer, parameter :: w         = 2560                    ! w of image
  integer, parameter :: h         = 1920                    ! h of image
  integer, parameter :: loops     = 255                     ! mandelbrot iters.
  real,    parameter :: center    = -0.7                    ! center of image
  real,    parameter :: radius    = 3.077 / 2.              ! from ctr of image
  real,    parameter :: x_min     = center - radius         
  real,    parameter :: x_max     = center + radius
  real,    parameter :: y_min     = h * radius / w
  real,    parameter :: y_max     = -h * radius / w
  real,    parameter :: threshold = 4                       ! must be at least 4

  ! ------------------------------
  !  Helper Routines & Functions
  ! ------------------------------

  contains

  ! --------------------------------------------------------------
  !  ~~~ Mandelbrot algorithm ~~~
  ! --------------------------------------------------------------
  !  Input:
  !           A point c in the complex plane.
  !  Output:
  !           A color value represented by a character.
  ! --------------------------------------------------------------
  ! Mandelbrot set function z_c(t):
  !      z_c(0)   = 0,
  !      z_c(t+1) = z_c(t)^2 + c,
  ! for c an input point in the complex plane.
  ! --------------------------------------------------------------
  ! A complex point c is taken as the value c in the above 
  !   function. If after 'loops' time steps, the value z_c(t) does 
  !   not exceed the threshold, then it is classified as being in
  !   the Mandelbrot set and is colored black. Else, it is not in
  !   the set, and is shaded in proportion to how long it took to 
  !   diverge.
  ! --------------------------------------------------------------
  character function mandelbrot(c) result(color)
    implicit none
    complex, intent(in) :: c     ! input complex point
    complex             :: z     ! helper variable
    integer             :: i     ! iterator

    z = 0
    do i=1,loops
      z = z**2 + c
      if (norm_square(z) >= threshold) exit
    end do

    if (i > loops) then
      color = char(0)            ! black
    else
      color = char(i)            ! some other color
    end if
    return
  end function mandelbrot

  real function norm_square(z)
    implicit none
    complex, intent(in) :: z
    norm_square = real(z)**2 + imag(z)**2
    return
  end function norm_square

  ! -----------------------------------------------
  ! Map row, col pixels onto x, y cartesian coords:
  !          (1,1) -> (x_min,y_max),
  !          (w,h) -> (x_max,y_min),
  ! rows -> x, columns -> y.
  ! ------------------------------------------------
  subroutine map_points(row, col, x, y)
    implicit none
    integer, intent(in) :: row, col 
    real, intent(inout) :: x, y

    x = x_min + (real(row)-1.)*((x_max-x_min) / real(w))
    y = y_max - (real(col)-1.)*((y_max-y_min) / real(h))
  end subroutine map_points


  !--------------------------------
  ! Open an output file and write
  !  generated image data to it.
  !--------------------------------
  subroutine write_image_file(image)
    implicit none
    integer :: row, col                                   ! indexers
    character, intent(in) :: image(:,:)                   ! image data matrix
    character :: color                                    ! helper variable
    character :: h1(12) = char([0,0,2,0,0,0,0,0,0,0,0,0]) ! metadata
    character :: h2(6)  = char([mod(w,256), w / 256, &    ! metadata
                                mod(h,256), h / 256, &
                                24, 0])
    ! Open file & write headers.
    open(1, file='out.tga', form='unformatted', access='stream')
    write(1) h1
    write(1) h2

	! Loop through image matrix & write to file 
	! according to the rules for coloring.
    do col=1,h
      do row=1,w
        color = image(row,col)
        if (color == char(0)) then   ! black
          write(1) char(0)             ! b
          write(1) char(0)             ! g
          write(1) char(0)             ! r
        else                         ! other color
          write(1) char(255)           ! b
          write(1) color               ! g
          write(1) color               ! r
        end if
      end do
    end do
    close(1)
  end subroutine write_image_file

end module m_compute_mandelbrot


! -----------------------------------------------------------------------------
!  ~~~ Main ~~~
! -----------------------------------------------------------------------------
program compute_mandelbrot
  use m_compute_mandelbrot
  use omp_lib
  implicit none
  integer   :: i, j             ! iterators
  integer   :: row, col         ! pixel coordinates
  real      :: x, y             ! cartesian coordinates
  real*8    :: t_start, t_end   ! time markers
  character :: color            ! helper variable
  character :: image(w,h)       ! image data matrix

  ! start timer
  t_start = omp_get_wtime()

  ! ----------------------------------------------
  !  Fill rgb matrix with image data in parallel.
  ! ----------------------------------------------
  !$omp parallel do                                &
  !$omp& schedule(dynamic)                         &
  !$omp& private(i, row, col, x, y, color)         &
  !$omp& shared(image)                             &
  !$omp& collapse(2)
  do col=1,h
    do row=1,w
      call map_points(row, col, x, y)
      color           = mandelbrot(cmplx(x, y))
      image(row,col)  = color
    end do
  end do
  !$omp end parallel do

  ! end timer
  t_end = omp_get_wtime()

  print *, "Total time:", t_end-t_start
  call write_image_file(image)

end program compute_mandelbrot
