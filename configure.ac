# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([Skye], [0.3.1], [coryan@users.noreply.github.com])

# Move as many of the generated files as possible to a subdirectory
AC_CONFIG_AUX_DIR(config)

# Yes, this is a C++ project.
AC_LANG([C++])

# We are going to use Automake to generate the makefiles, so initialize it
AM_INIT_AUTOMAKE([subdir-objects foreign])
AM_SILENT_RULES([yes])

# Create a config header, but with a distinctive name
AM_CONFIG_HEADER([skye/skye-config.hpp])

# We are not going to use shared libraries, so avoid the libtool pain.
# In fact, Skye is a header-only library, but we need to have a Makefile for
# the tests and the installation rules.
# LT_PREREQ([2.4])
# LT_INIT
# LT_LANG([C++])
# LT_CONFIG_LTDL_DIR([ltdl])

# There is only one Makefile in this project, it is non-recursive.
AC_CONFIG_FILES([Makefile])

# Make sure there is a C++ compiler, it supports C++11 and enable all warnings.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB
AX_CXX_COMPILE_STDCXX_11(noext, mandatory)
AX_CXXFLAGS_WARN_ALL

# Checks for libraries.
AX_BOOST_BASE([1.53],[],[
  AC_MSG_ERROR([unable to find a suitable Boost library, need version >= 1.53])
])
AX_BOOST_UNIT_TEST_FRAMEWORK
AX_BOOST_ASIO
AX_BOOST_SYSTEM

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_OUTPUT
