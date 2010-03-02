AC_DEFUN([PREDICTOR_ARG_WITH_IMMODULES_DIR],
	 [dnl 
	  AC_REQUIRE([PKG_PROG_PKG_CONFIG])[]dnl

AC_ARG_VAR([PREDICTOR_IMMODULES_DIR], [GTK+ immodules directory])
AC_ARG_WITH([immodules-dir], [AS_HELP_STRING(
			      [--with-immodules-dir=DIR],
			      [GTK+ immodules directory @<:@LIBDIR/gtk-2.0/BINVERSION/immodules@:>@])],
 			      [PREDICTOR_IMMODULES_DIR=$withval])[]dnl

AC_MSG_CHECKING([for GTK+ immodules directory])
AS_IF([test "x$PREDICTOR_IMMODULES_DIR" = x],
	    [
	     mp_gtk_binversion=`$PKG_CONFIG --variable=gtk_binary_version gtk+-2.0 2>&AS_MESSAGE_LOG_FD`
	     PREDICTOR_IMMODULES_DIR='${libdir}/gtk-2.0/'$mp_gtk_binversion'/immodules'
	    ])
	    AC_MSG_RESULT([$PREDICTOR_IMMODULES_DIR])
])
