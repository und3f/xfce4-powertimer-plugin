#include <gtk/gtk.h>

#include <libxfce4ui/libxfce4ui.h>
#include <libxfce4util/libxfce4util.h>
#include <libxfce4panel/libxfce4panel.h>

#include "configure-dialog.h"
#include "time-spin-utils.h"
#include "constants.h"

void configureDialog(GtkWidget *plugin, PowerTimer *powertimer)
{
  GtkWidget *dialog = xfce_titled_dialog_new_with_buttons(
      _("Set power timer"),
      GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(powertimer->plugin))),
      GTK_DIALOG_DESTROY_WITH_PARENT,
      _("Set timer"), GTK_RESPONSE_APPLY,
      _("Discard"), GTK_RESPONSE_CANCEL,
      NULL);

  GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
  GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
  gtk_box_pack_start(GTK_BOX(content_area), hbox, FALSE, FALSE, 0);

  GtkWidget *label_adj = gtk_label_new(_("Shutdown after:"));

  GtkAdjustment *adj = gtk_adjustment_new(TIMER_DEFAULT, 0, 24 * 60, TIMER_STEP, 4 * TIMER_STEP, 0);
  GtkWidget *spin_button = gtk_spin_button_new(GTK_ADJUSTMENT(adj), 0, 2);

  g_signal_connect(G_OBJECT(spin_button), "input", G_CALLBACK(time_spin_input), NULL);
  g_signal_connect(G_OBJECT(spin_button), "output", G_CALLBACK(time_spin_output), NULL);

  gtk_box_pack_start(GTK_BOX(hbox), label_adj, TRUE, TRUE, 8);
  gtk_box_pack_start(GTK_BOX(hbox), spin_button, TRUE, TRUE, 8);

  gtk_widget_show_all(dialog);

  gint response = gtk_dialog_run(GTK_DIALOG(dialog));
  fprintf(stderr, "response %u\n", response);
  if (response == GTK_RESPONSE_APPLY)
  {
    gint value = gtk_spin_button_get_value_as_int((GtkSpinButton *)spin_button);
    setTimer(powertimer, (guint)value * TIME_SECONDS);
  }

  gtk_widget_destroy(dialog);
}