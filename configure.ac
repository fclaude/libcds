#                                               -*- Autoconf -*-
# To rebuild the configure script and Makefile.in files (as well as some
# supporting files mainly in build-aux/ and m4/) run the following
# command:
#     autoreconf -vif

AC_PREREQ([2.68])
AC_INIT([libcds], [1.0], [fclaude@gmail.com], [cds],
        [https://github.com/fclaude/libcds])
AC_CONFIG_SRCDIR([src/static/bitsequence/BitSequenceBuilderDArray.cpp])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([build-aux/config.h])
AM_INIT_AUTOMAKE([1.11 subdir-objects])
AM_SILENT_RULES([yes])
LT_INIT([shared static])
AC_SUBST([LIBTOOL_DEPS])
AC_SUBST([CDS_SO_VERSION], [1:3:0])
DX_INIT_DOXYGEN([libcds], [Doxyfile], [docs/])

# Checks for programs.
AC_PROG_CXX
AC_LANG([C++])
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdlib.h sys/timeb.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([select])

AC_CONFIG_FILES([Makefile
                 cds.pc
                 src/Makefile
                 tests/Makefile
                 tutorial/src/Makefile
                 tutorial/ssa/Makefile])
AC_OUTPUT
