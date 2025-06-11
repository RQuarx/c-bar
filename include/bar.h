#pragma once
#ifndef __BAR_H__
#define __BAR_H__

#include <gtk-layer-shell.h>
#include <gtk/gtk.h>

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

static const int32_t
    W_WIDTH  = 1920,
    W_HEIGHT = 21;

struct bar_widgets
{
    GtkBox *box;

    GtkLabel
        *ws_label,
        *time_label,
        *battery_label;
};

#define WIDGETS(ptr) ((struct bar_widgets *)ptr)


/** @brief Helper function with creating a GtkBox * object. */
GtkBox *new_hbox();


/**
 * @brief Creates the status bar.
 *
 * @param widgets A struct bar_widgets * object.
 * @return TRUE on success, or FALSE on failure.
 */
gboolean create_bar(struct bar_widgets *widgets);

void
    create_window        (GtkWindow **window),
    create_container     (GtkWindow  *window,
                          struct bar_widgets *widgets),
    create_ws_indicator  (struct bar_widgets *widgets),
    create_time_indicator(struct bar_widgets *widgets),
    create_bat_indicator (struct bar_widgets *widgets);

/** @brief Updates the specified label from @p data . */
void update_label(gpointer data);


#endif /* __BAR_H__ */
