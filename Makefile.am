@SET_MAKE@

SUBDIRS =               \
        src				\
        icons

ditsclean-local:
	rm -rf *.cache *~

.PHONY: ChangeLog

ChangeLog: Makefile
		(GIT_DIR=$(top_srcdir)/.git git log > .changelog.tmp \
		&& mv .changelog.tmp ChangeLog; rm -f .changelog.tmp) \
		|| (touch ChangeLog; echo 'Git directory not found: installing possibly empty changelog.' >&2)


EXTRA_DIST =            \
        README.md		\
        NEWS

DISTCLEANFILES =						\
	aclocal.m4							\
	compile								\
	config.guess						\
	config.h.in							\
	config.sub							\
	configure							\
	configure.ac						\
	depcomp								\
	install-sh							\
	ltmain.sh							\
	missing								\
	mkinstalldirs						\
	intltool-extract					\
	intltool-merge						\
	intltool-update
