ACLOCAL_AMFLAGS = -I macros $(ACLOCAL_FLAGS)
DISTCHECK_CONFIGURE_FLAGS = --enable-warnings=max

SUBDIRS = po src

dist_noinst_DATA =	    \
	intltool-extract.in \
	intltool-merge.in   \
	intltool-update.in

dist_noinst_SCRIPTS = autogen.sh

DISTCLEANFILES = intltool-extract intltool-merge intltool-update
