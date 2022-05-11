#include <math.h>
#include <gtk/gtk.h>

#include "time-spin-utils.h"

gint time_spin_input(GtkSpinButton *spin_button, gdouble *new_val)
{
  const gchar *text;
  gchar **str;
  gboolean found = FALSE;
  long hours, minutes;
  gchar *endh, *endm;

  text = gtk_entry_get_text(GTK_ENTRY(spin_button));
  str = g_strsplit(text, ":", 2);

  if (g_strv_length(str) == 2)
  {
    hours = strtol(str[0], &endh, 10);
    minutes = strtol(str[1], &endm, 10);
    if (!*endh && !*endm &&
        0 <= hours && hours < 24 &&
        0 <= minutes && minutes < 60)
    {
      *new_val = (gdouble)(hours * 60 + minutes);
      found = TRUE;
    }
  }

  g_strfreev(str);

  if (!found)
  {
    *new_val = 0.0;
    return GTK_INPUT_ERROR;
  }

  return TRUE;
}

gint time_spin_output(GtkSpinButton *spin_button)
{
  GtkAdjustment *adjustment;
  gchar *buf;
  gdouble hours;
  gdouble minutes;

  adjustment = gtk_spin_button_get_adjustment(spin_button);
  hours = gtk_adjustment_get_value(adjustment) / 60.0;
  minutes = (hours - floor(hours)) * 60.0;
  buf = g_strdup_printf("%02.0f:%02.0f", floor(hours), floor(minutes + 0.5));
  if (strcmp(buf, gtk_entry_get_text(GTK_ENTRY(spin_button))))
    gtk_entry_set_text(GTK_ENTRY(spin_button), buf);
  g_free(buf);

  return TRUE;
}