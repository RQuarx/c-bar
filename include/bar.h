#pragma once
#ifndef __BAR_H__
#define __BAR_H__

#include <gtk-layer-shell.h>
#include <gtk/gtk.h>


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
