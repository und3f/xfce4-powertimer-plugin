#include <stdio.h>
#include <stdbool.h>
#include <gtk/gtk.h>

#include <libxfce4ui/libxfce4ui.h>
#include <libxfce4util/libxfce4util.h>
#include <libxfce4panel/libxfce4panel.h>
#include <libxfce4panel/xfce-panel-convenience.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "power-timer.h"

static void checkPoweroffTime(PowerTimer *this);
static void handleStopTimerClick(GtkWidget *button, PowerTimer *powerTimer);
static int tickCallback(PowerTimer *this);
static void shutdown(void);

PowerTimer *powerTimerCreate(XfcePanelPlugin *plugin)
{
    PowerTimer *this = g_slice_new0(PowerTimer);
    this->plugin = plugin;

    this->ebox = gtk_event_box_new();

    this->hbox = gtk_box_new(
        xfce_panel_plugin_get_orientation(plugin),
        GTK_BOX_SPACING);
    gtk_container_add(GTK_CONTAINER(this->ebox), this->hbox);

    this->icon = xfce_panel_image_new_from_source(ICON_NAME);

    gint size = xfce_panel_plugin_get_size(this->plugin);
    xfce_panel_image_set_size(XFCE_PANEL_IMAGE(this->icon), size);

    this->progressBar = gtk_progress_bar_new();
    gtk_orientable_set_orientation(GTK_ORIENTABLE(this->progressBar),
                                   GTK_ORIENTATION_VERTICAL);

    gtk_box_pack_start(GTK_BOX(this->hbox), this->icon, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(this->hbox), this->progressBar, FALSE, FALSE, 0);

    this->poweroffTime = NULL;

    this->stopTimer = gtk_menu_item_new_with_label(_("Stop timer"));
    gtk_widget_set_sensitive(this->stopTimer, FALSE);
    gtk_widget_show(this->stopTimer);
    g_signal_connect(G_OBJECT(this->stopTimer), "activate",
                     G_CALLBACK(handleStopTimerClick), this);
    xfce_panel_plugin_menu_insert_item(plugin, GTK_MENU_ITEM(this->stopTimer));

    gtk_widget_show_all(this->ebox);

    this->timerId = g_timeout_add_seconds(TICK, (GSourceFunc)tickCallback, this);

    return this;
}

void powerTimerDestroy(PowerTimer *this)
{
    gtk_widget_destroy(this->hbox);

    g_slice_free(PowerTimer, this);
}

void handleStopTimerClick(GtkWidget *button, PowerTimer *powerTimer)
{
    removeTimer(powerTimer);
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
        xfce_dialog_show_error(NULL, err, "Failed to create DBUX proxy");
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

    if (!result)
    {
        if (err != NULL)
        {
            xfce_dialog_show_error(NULL, err, "Failed to send shutdown command");
            g_clear_error(&err);
            return;
        }
    }
}

static void checkPoweroffTime(PowerTimer *this)
{
    if (NULL == this->poweroffTime)
    {
        return;
    }

    gtk_progress_bar_pulse(GTK_PROGRESS_BAR(this->progressBar));

    GDateTime *now = g_date_time_new_now_local();
    GTimeSpan timeDiff = g_date_time_difference(this->poweroffTime, now);
    g_date_time_unref(now);

    if (timeDiff <= 0)
    {
        // TODO: ask for shutdown
        shutdown();
    }
}

static int tickCallback(PowerTimer *this)
{
    checkPoweroffTime(this);

    return TRUE;
}

void removeTimer(PowerTimer *this)
{
    if (NULL != this->poweroffTime)
    {
        g_date_time_unref(this->poweroffTime);
        this->poweroffTime = NULL;
    }

    gtk_widget_set_sensitive(this->stopTimer, FALSE);
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(this->progressBar), 0);
}

void setTimer(PowerTimer *this, guint timeout)
{
    printf("Set timeout: %u\n", timeout);
    removeTimer(this);

    gtk_widget_set_sensitive(this->stopTimer, TRUE);

    GDateTime *now = g_date_time_new_now_local();
    this->poweroffTime = g_date_time_add_seconds(now, timeout);
    g_date_time_unref(now);
}
