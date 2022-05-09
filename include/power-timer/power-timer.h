#ifndef __POWERTIMER_H__
#define __POWERTIMER_H__

typedef struct
{
  XfcePanelPlugin *plugin;
  GtkWidget *ebox, *hbox;
  GtkWidget *icon, *label;
  guint timerId;
} PowerTimer;

PowerTimer *powerTimerNew(XfcePanelPlugin *plugin);
void powerTimerDestructor(PowerTimer *powerTimer);

void removeTimer(PowerTimer *timer);
void setTimer(PowerTimer *timer, guint timeout);

#endif