#include <libxfce4util/libxfce4util.h>
#include <libxfce4panel/libxfce4panel.h>

#include "constants.h"
#include "about-dialog.h"

void aboutDialog(XfcePanelPlugin *plugin)
{
    const gchar *author[] = {
        "Serhii Zasenko <sergii@zasenko.name>",
        NULL};

    GdkPixbuf *icon = xfce_panel_pixbuf_from_source(ICON_NAME, NULL, 32);

    gtk_show_about_dialog(
        NULL,
        "logo", icon,
        "license", xfce_get_license_text(XFCE_LICENSE_TEXT_GPL),
        "version", PACKAGE_VERSION,
        "program-name", PACKAGE_NAME,
        "comments", _("Power timer"),
        "website", WEBSITE,
        "copyright", _(COPYRIGHT),
        "authors", author,
        NULL);
}