SUBDIRS =               \
        src				\
        icons			\
        po

ditsclean-local:
	rm -rf *.cache *~

.PHONY: ChangeLog

ChangeLog: Makefile
	(GIT_DIR=$(top_srcdir)/.git git log > .changelog.tmp \
	&& mv .changelog.tmp ChangeLog; rm -f .changelog.tmp) \
	|| (touch ChangeLog; echo 'Git directory not found: installing possibly empty changelog.' >&2)

dist-hook: ChangeLog

#
# Desktop file
#
desktopdir = $(datadir)/xfce4/panel/plugins
desktop_in_files = powertimer.desktop.in
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@

EXTRA_DIST =            	\
        README.md			\
		include				\
		$(desktop_in_files) \
		intltool-extract.in	\
		intltool-merge.in	\
		intltool-update.in

DISTCLEANFILES =						\
	intltool-extract					\
	intltool-merge						\
	intltool-update						\
	$(desktop_DATA)
