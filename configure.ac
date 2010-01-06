#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([Aqua], [1.0], [ponton@jabster.pl])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([src/aqua.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADER([GL/gl.h])
AC_CHECK_HEADER([GL/glu.h])
AC_CHECK_HEADER([SDL/sdl.h])
AC_CHECK_HEADER([SDL/SDL_image.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 data/Makefile
                 data/clownfish/Makefile
                 data/goldenfish/Makefile
                 src/Makefile])
AC_OUTPUT
