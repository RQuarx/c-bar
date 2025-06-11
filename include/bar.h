#pragma once
#ifndef __BAR_H__
#define __BAR_H__

#include <gtk-layer-shell.h>
#include <gtk/gtk.h>

#define APP_DATA(ptr) ((app_data_t*)ptr)

static const int32_t
    W_WIDTH  = 1920,
    W_HEIGHT = 21;

static const char *css =
"#bar-window {"
"    font-family: 'JetBrainsMono Nerd Font';"
"    background-color: transparent;"
"    font-size: 11px;"
"    color: white;"
"    all: unset;"
"}"
"#workspace-corner,#time-corner-r,"
"#battery-corner,#time-corner-l {"
"    background-color: transparent;"
"    min-width: 50px;"
"}"
"#workspace-corner,#time-corner-r {"
"    box-shadow: -7px 10px #283039;"
"    border-radius: 0px 0px 0px 75%;"
"    margin: 0px 4px 0px -4px;"
"}"
"#battery-corner,#time-corner-l {"
"    box-shadow: 7px 10px #283039;"
"    border-radius: 0px 0px 75% 0px;"
"    margin: 0px -4px 0px 4px;"
"}"
"#bar-box {"
"    box-shadow: inset 0px -3px #283039;"
"    background-color: #1D2228;"
"}"
"#workspace-label-box,#time-label-box,#battery-label-box {"
"    background-color: #283039;"
"    margin-top: 1px;"
"}"
"#workspace-label { padding-left: 1em; }"
"#workspace-label-box {"
"    border-radius: 0px 100px 0px 0px;"
"    padding-right: 2em;"
"}"
"#time-label-box {"
"    border-radius: 100px 100px 0px 0px;"
"    padding: 0em 2em;"
"}"
"#battery-label-box {"
"    border-radius: 100px 0px 0px 0px;"
"    padding: 0em 1em 0em 2em;"
"}";


static GtkWindow *bar_window;
static GtkBox    *bar_box;

static GtkLabel
    *bar_ws_label,
    *bar_time_label,
    *bar_battery_label;


GtkBox *new_hbox(gint32 spacing);

gboolean create_bar(void);

void
    create_window(void),
    create_container(void),
    create_ws_indicator(void),
    create_time_indicator(void),
    create_bat_indicator(void);

void
    update_workspace_label(gpointer data),
    update_time_label     (gpointer data),
    update_battery_label  (gpointer data);


#endif /* __BAR_H__ */
