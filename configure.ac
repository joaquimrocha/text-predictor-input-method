##
## Copyright (C) 2010 Igalia, S.L.
##
## Author: Joaquim Rocha <jrocha@igalia.com>
##
## This file is part of im-predictor and is published under LGPL v2.1
##


AC_INIT([predictor-gtk-input-method], [0.1], [jrocha@igalia.com], [predictor-gtk-input-method])
AC_PREREQ([2.58])

AC_CONFIG_SRCDIR([src/im-predictor.c])
AC_CONFIG_MACRO_DIR([macros])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.8 -Wall gnu no-define])
AC_ARG_VAR([ACLOCAL_FLAGS], [aclocal flags, e.g. -I <macro dir>])

AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX

AM_PROG_CC_C_O()

AC_DISABLE_STATIC()
AC_PROG_LIBTOOL()

AC_SUBST([GETTEXT_PACKAGE], [predictor-gtk-input-method])
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"], [Define to the gettext package name.])
IT_PROG_INTLTOOL([0.35])
AM_GLIB_GNU_GETTEXT()

PKG_CHECK_MODULES([PREDICTOR_MODULES], [gtk+-2.0 >= 2.6.0])

AC_MSG_CHECKING([for presage.h])
AC_CHECK_LIB(presage, _ZN7Presage7predictEv, have_presage=yes, have_presage=no)
if test "$have_presage" = yes; then
   LIBPRESAGE="-lpresage"
else
	AC_MSG_ERROR(*** Presage library not found in the system. Please install this library. ***)
fi
AC_SUBST(LIBPRESAGE)

AC_ARG_VAR([GTK_QUERY_IMMODULES], [path to gtk-query-immodules-2.0 utility])
AC_CHECK_PROGS([GTK_QUERY_IMMODULES], [gtk-query-immodules-2.0 gtk-query-immodules-2.0-32])

PREDICTOR_ARG_WITH_IMMODULES_DIR()

AS_IF([test "x$GTK_QUERY_IMMODULES" = x],
      [AC_MSG_FAILURE([gtk-query-immodules-2.0 not found.])])

AC_CONFIG_FILES([
  Makefile
  src/Makefile
  po/Makefile.in
])
AC_OUTPUT()
#  examples/Makefile

