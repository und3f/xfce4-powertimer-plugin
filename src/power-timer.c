#include <stdio.h>
#include <stdbool.h>
#include <gtk/gtk.h>

#include <libxfce4util/libxfce4util.h>
#include <libxfce4panel/libxfce4panel.h>
#include <libxfce4panel/xfce-panel-convenience.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "power-timer.h"

static void
xfce_session_logout_notify_error(const gchar *message,
                                 GError *error)
{
    xfce_dialog_show_error(NULL, error, "%s", message);
}

static void shutdown()
{
    bool allow_save = TRUE;

    GError *err = NULL;
    GDBusProxy *proxy = g_dbus_proxy_new_for_bus_sync(
        G_BUS_TYPE_SESSION,
        G_DBUS_PROXY_FLAGS_NONE,
        NULL,
        "org.xfce.SessionManager",
        "/org/xfce/SessionManager",
        "org.xfce.Session.Manager",
        NULL,
        &err);

    if (proxy == NULL)
    {
        xfce_dialog_show_error(NULL, err);
        g_error_free(err);
        return;
    }

    GVariant *result = g_dbus_proxy_call_sync(
        proxy, "Shutdown",
        g_variant_new("(b)", allow_save),
        G_DBUS_CALL_FLAGS_NONE,
        -1,
        NULL,
        &err);

    if (!result) {
        if (err != NULL) {
            xfce_dialog_show_error(NULL, err);
            g_clear_error(&err);
            return;
        }
    }
}

static int timerCallback(PowerTimer *timer)
{
    timer->timerId = 0;
    
    shutdown();

    return FALSE;
}

void removeTimer(PowerTimer *timer)
{
    if (timer->timerId != 0)
    {
        g_source_remove(timer->timerId);
    }
    timer->timerId = 0;
}

void setTimer(PowerTimer *timer, guint timeout)
{
    printf("Set timeout: %u\n", timeout);
    printf("previous: %u\n", timer->timerId);
    removeTimer(timer);

    timer->timerId = g_timeout_add_seconds(timeout, timerCallback, timer);
}

PowerTimer *powerTimerNew(XfcePanelPlugin *plugin)
{
    PowerTimer *powertimer = g_slice_new0(PowerTimer);
    powertimer->plugin = plugin;

    powertimer->ebox = gtk_event_box_new();

    powertimer->hbox = gtk_box_new(
        xfce_panel_plugin_get_orientation(plugin),
        GTK_BOX_SPACING);
    gtk_container_add(GTK_CONTAINER(powertimer->ebox), powertimer->hbox);

    powertimer->icon = xfce_panel_image_new_from_source(ICON_NAME);

    gint size = xfce_panel_plugin_get_size(powertimer->plugin);
    xfce_panel_image_set_size(XFCE_PANEL_IMAGE(powertimer->icon), size);

    gtk_box_pack_start(GTK_BOX(powertimer->hbox), powertimer->icon, FALSE, FALSE, 0);

    GtkWidget *stopTimer = gtk_menu_item_new_with_label(_("Stop timer"));
    gtk_widget_set_sensitive(stopTimer, FALSE);
    gtk_widget_show(stopTimer);
    g_signal_connect(G_OBJECT(stopTimer), "activate",
                     G_CALLBACK(removeTimer), powertimer);
    xfce_panel_plugin_menu_insert_item(plugin, GTK_MENU_ITEM(stopTimer));

    gtk_widget_show_all(powertimer->ebox);

    return powertimer;
}

void powerTimerDestructor(PowerTimer *powerTimer)
{
    gtk_widget_destroy(powerTimer->hbox);

    g_slice_free(PowerTimer, powerTimer);
}
