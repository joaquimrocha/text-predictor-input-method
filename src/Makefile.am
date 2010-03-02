conffiledir = $(sysconfdir)

immodulesdir = $(PREDICTOR_IMMODULES_DIR)
immodules_LTLIBRARIES = im-predictor.la

im_predictor_la_SOURCES =      \
	im-predictor.c         \
	im-predictor.h         \
	im-predictor-context.c \
	im-predictor-context.h \
	text-predictor.cpp     \
	text-predictor.h

mplocaledir = $(prefix)/$(DATADIRNAME)/locale

global_defs = -DPREDICTOR_LOCALEDIR=\""$(mplocaledir)"\" \
	      -DPREDICTOR_CONFDIR=\""$(conffiledir)"\"
AM_CPPFLAGS = $(global_defs) -I$(top_builddir) $(PREDICTOR_MODULES_CFLAGS)

im_predictor_la_LDFLAGS = -rpath $(immodulesdir) $(LIBPRESAGE) -avoid-version -module
im_predictor_la_LIBADD  = $(PREDICTOR_MODULES_LIBS) $(INTLLIBS)

gtk_confdir = $(sysconfdir)/gtk-2.0
gtk_immodules = $(gtk_confdir)/gtk.immodules

install-data-hook:
	@$(POST_INSTALL)
	@if test -z "$(DESTDIR)"; then \
	  echo " $(install_sh) -d $(gtk_confdir)"; \
	  $(install_sh) -d "$(gtk_confdir)" || exit 1; \
	  dir=`expr "X$(immodulesdir)" : 'X\(.*\)[\\/].*[\\/]'`; \
	  if test -z "$$GTK_PATH"; then GTK_PATH=$$dir; else GTK_PATH=$$dir:$$GTK_PATH; fi; \
	  echo " GTK_PATH=$$GTK_PATH $(GTK_QUERY_IMMODULES) >$(gtk_immodules)"; \
	  export GTK_PATH; $(GTK_QUERY_IMMODULES) >"$(gtk_immodules)" || exit 1; \
	else :; fi

uninstall-hook:
	@$(POST_UNINSTALL)
	if test -z "$(DESTDIR)" && test -f "$(gtk_immodules)"; then \
	  $(GTK_QUERY_IMMODULES) >$(gtk_immodules); \
	else :; fi
