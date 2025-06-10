#pragma once
#ifndef __BAR_H__
#define __BAR_H__

#include <gtk-layer-shell.h>
#include <gtk/gtk.h>

#define APP_DATA(ptr) ((app_data_t*)ptr)

static const int32_t
    W_WIDTH  = 1920,
    W_HEIGHT = 21;


static GtkWindow *bar_window;
static GtkBox    *bar_box;

static GtkLabel
    *bar_ws_label,
    *bar_time_label,
    *bar_battery_label;


GtkBox *new_hbox(int32_t spacing);

gboolean
    create_bar(),
    create_window(),
    create_container(),
    create_ws_indicator(),
    create_time_indicator(),
    create_bat_indicator();

gboolean
    update_workspace_label(gpointer data),
    update_time_label     (gpointer data),
    update_battery_label  (gpointer data);


#endif /* __BAR_H__ */
