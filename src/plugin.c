#include <gtk/gtk.h>

#include <libxfce4util/libxfce4util.h>
#include <libxfce4panel/libxfce4panel.h>
#include <libxfce4panel/xfce-panel-convenience.h>

#include "power-timer.h"
#include "about-dialog.h"
#include "configure-dialog.h"

static void orientationChanged(XfcePanelPlugin *plugin,
                               GtkOrientation orientation,
                               PowerTimer *powerTimer);

static gboolean sizeChanged(XfcePanelPlugin *plugin,
                            gint size,
                            PowerTimer *powerTimer);

static void powertimer_constructor(XfcePanelPlugin *plugin)
{
    PowerTimer *powerTimer = powerTimerNew(plugin);

    printf("Value %u\n", powerTimer->timerId);
    /* add the ebox to the panel */
    gtk_container_add(GTK_CONTAINER(plugin), powerTimer->ebox);

    /* show the panel's right-click menu on this ebox */
    xfce_panel_plugin_add_action_widget(plugin, powerTimer->ebox);

    g_signal_connect(G_OBJECT(plugin), "free-data",
                     G_CALLBACK(powerTimerDestructor), powerTimer);

    g_signal_connect(G_OBJECT(plugin), "size-changed",
                     G_CALLBACK(sizeChanged), powerTimer);

    g_signal_connect(G_OBJECT(plugin), "orientation-changed",
                     G_CALLBACK(orientationChanged), powerTimer);

    /* show the about menu item and connect signal */
    xfce_panel_plugin_menu_show_about(plugin);
    g_signal_connect(G_OBJECT(plugin), "about",
                     G_CALLBACK(aboutDialog), powerTimer);

    xfce_panel_plugin_menu_show_configure(plugin);
    g_signal_connect_swapped(plugin, "configure-plugin",
                     G_CALLBACK(configureDialog), powerTimer);
    g_signal_connect_swapped(G_OBJECT(plugin), "button_press_event",
                     G_CALLBACK(configureDialog), powerTimer);

}

XFCE_PANEL_PLUGIN_REGISTER(powertimer_constructor)

static void
orientationChanged(XfcePanelPlugin *plugin,
                   GtkOrientation orientation,
                   PowerTimer *powerTimer)
{
    /* change the orientation of the box */
    gtk_orientable_set_orientation(GTK_ORIENTABLE(powerTimer->hbox), orientation);
}

static gboolean
sizeChanged(XfcePanelPlugin *plugin,
            gint size,
            PowerTimer *powerTimer)
{
    GtkOrientation orientation;

    /* get the orientation of the plugin */
    orientation = xfce_panel_plugin_get_orientation(plugin);

    /* set the widget size */
    if (orientation == GTK_ORIENTATION_HORIZONTAL)
        gtk_widget_set_size_request(GTK_WIDGET(plugin), -1, size);
    else
        gtk_widget_set_size_request(GTK_WIDGET(plugin), size, -1);

    /* we handled the orientation */
    return TRUE;
}
