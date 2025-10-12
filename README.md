# QD
A double-double and quad-double package for Fortran and C++.

# Cite this paper
Y. Hida, X. S. Li and D. H. Bailey, "Algorithms for quad-double precision floating point arithmetic," Proceedings 15th IEEE Symposium on Computer Arithmetic. ARITH-15 2001, Vail, CO, USA, 2001, pp. 155-162, doi: 10.1109/ARITH.2001.930115.

## End-user documentation

For historical reasons, the original user-focused documentation is
located in the `README` file, sans the `.md` extension. In-depth
technical documentation is also provided, as `docs/qd.pdf` in the
release tarballs. To build the PDF from its LaTeX sources, run

```
$ make -C docs qd.pdf
```

after installing the necessary LaTeX bits on your system.

## Tips for developers

Before you can build QD from the git repository, you will likely have
to generate the "autotools" files (such as `./configure`) that are
normally provided for you in a release tarball. The easiest way to do
that is to run,

```
$ autoreconf -fi
```

as a shortcut for running each of *autoconf*, *autoheader*, *aclocal*,
*automake*, and *libtoolize* as many times as necessary and in the
correct order. The `-fi` flags force autoreconf to regenerate
everything, and to install any missing auxiliary files.  You will of
course need all of the relevant autotools packages (autoconf,
automake, and libtool) installed for this to work.

Afterwards, you can `./configure` the package and build it like you
normally would from a release tarball.

### Running the test suite

The QD library comes with an automated test suite that should always
pass. To run it, execute `make check` after building the
library. Ignoring the noise from the compiler (the test suite itself
must be compiled), the output should look something like

```
$ ./configure
...
$ make
...
$ make check
...
PASS: qd_test
PASS: pslq_test
PASS: c_test
PASS: f_test
============================================================================
Testsuite summary for qd 2.3.23
============================================================================
# TOTAL: 4
# PASS:  4
# SKIP:  0
# XFAIL: 0
# FAIL:  0
# XPASS: 0
# ERROR: 0
============================================================================

```

### Making a release

There are several steps that need to be performed when making a new
release:

1. Ensure that all important user-facing changes are mentioned
   in the `NEWS` file.

2. Update the package version number in `configure.ac`.

3. Build a release tarball. First, run `git clean -x -f -d` to ensure
   that you're starting fresh. Then run `autoreconf -fi` to regenerate
   all of the autotools files. Run `./configure` to create your
   Makefiles, and finally, `make dist` to create the release tarball.

4. Run `make distcheck` to ensure that the release tarball works.

5. Tag the commit that corresponds to the release with `git tag -s
   <version-number>`.

6. Push everything to Github.

7. Upload the release tarball (created earlier) to the Github release
   page that corresponds to your new version tag. This ensures that
   end-users can run `./configure` and such "out of the box," without
   having to install the GNU autotools (or learn their commands).
