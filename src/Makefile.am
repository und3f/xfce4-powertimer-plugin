plugindir = $(libdir)/xfce4/panel/plugins
plugin_LTLIBRARIES = libpowertimer.la

STRICT_CFLAGS=-Wall -Wstrict-prototypes -Wmissing-prototypes -Wshadow -Wconversion

libpowertimer_la_CFLAGS =	\
	-I$(top_srcdir)/include 			\
	$(LIBXFCE4UTIL_CFLAGS) 	\
	$(LIBXFCE4UI_CFLAGS) 	\
	$(LIBXFCE4PANEL_CFLAGS) \
	$(STRICT_CFLAGS) \
	$(PLATFORM_CFLAGS)

libpowertimer_la_LDFLAGS = 	\
	-avoid-version 			\
	-module 				\
	-no-undefined 			\
	-export-symbols-regex '^xfce_panel_module_(preinit|init|construct)'\
	$(PLATFORM_LDFLAGS)

libpowertimer_la_LIBADD =	\
	$(LIBXFCE4UTIL_LIBS) 	\
	$(LIBXFCE4UI_LIBS) 		\
	$(LIBXFCE4PANEL_LIBS)

libpowertimer_la_SOURCES =	\
	about-dialog.c 		\
	time-spin-utils.c 	\
	configure-dialog.c 	\
	power-timer.c		\
	plugin.c


# desktopdir = $(datadir)/xfce4/panel/plugins
# desktop_DATA = power-timer.desktop

# EXTRA_DIST = src/power-timer.desktop.in
# DISTCLEANFILES = $(desktop_DATA)
