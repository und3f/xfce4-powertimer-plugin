#ifndef __POWERTIMER_H__
#define __POWERTIMER_H__

typedef struct
{
  XfcePanelPlugin *plugin;
  GtkWidget *ebox, *hbox;
  GtkWidget *icon, *progressBar;
  GtkWidget *stopTimer;
  guint timerId;

  GDateTime *poweroffTime;
} PowerTimer;

PowerTimer *powerTimerCreate(XfcePanelPlugin *plugin);
void powerTimerDestroy(PowerTimer *powerTimer);

void removeTimer(PowerTimer *timer);
void setTimer(PowerTimer *timer, guint timeout);

#endif