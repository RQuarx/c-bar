#include "utils.h"
#include "bar.h"


struct bar_widgets *
create_bar()
{
    GError         *err          = NULL;
    GtkCssProvider *css_provider = gtk_css_provider_new();
    if (css_provider == NULL) {
        print_err("Failed to create a GtkCssProvider.");
        return FALSE;
    }

    if (gtk_css_provider_load_from_data(css_provider,
                                        css,
                                        strlen(css),
                                        &err) == FALSE) {
        print_err("Failed to load css from data: %s", err->message);
        return FALSE;
    }

    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );
    g_object_unref(css_provider);

    struct bar_widgets *widgets = g_new(struct bar_widgets, 1);

    GtkWindow *window = NULL;

    create_window        (&window);
    create_container     (window, widgets);
    create_ws_indicator  (widgets);
    create_time_indicator(widgets);
    create_bat_indicator (widgets);

    gtk_widget_show_all(GTK_WIDGET(window));
    return widgets;
}


void
create_window(GtkWindow **window)
{
    *window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));

    gtk_layer_init_for_window(*window);

    gtk_widget_set_name(GTK_WIDGET(*window), "bar-window");
    gtk_window_set_title(*window, "c-bar");

    gtk_layer_set_anchor(*window, GTK_LAYER_SHELL_EDGE_BOTTOM, TRUE);
    gtk_layer_set_exclusive_zone(*window, 21);

    gtk_widget_set_size_request(GTK_WIDGET(*window), W_WIDTH, W_HEIGHT);
    gtk_window_resize(*window, W_WIDTH, W_HEIGHT);
}


void
create_container(GtkWindow          *window,
                 struct bar_widgets *widgets)
{
    widgets->box = GTK_BOX(new_hbox(0));

    gtk_widget_set_name(GTK_WIDGET(widgets->box), "bar-box");

    gtk_widget_set_halign(GTK_WIDGET(widgets->box), GTK_ALIGN_FILL);
    gtk_widget_set_hexpand(GTK_WIDGET(widgets->box), TRUE);

    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(widgets->box));
}


void
create_ws_indicator(struct bar_widgets *widgets)
{
    GtkBox
        *ws_box       = new_hbox(0),
        *ws_label_box = new_hbox(0),
        *ws_corner    = new_hbox(0);

    widgets->ws_label = GTK_LABEL(gtk_label_new("1"));

    /* Workspace container */
    gtk_widget_set_halign(GTK_WIDGET(ws_box), GTK_ALIGN_FILL);

    gtk_container_add(GTK_CONTAINER(widgets->box), GTK_WIDGET(ws_box));

    /* Workspace label container */
    gtk_widget_set_name(GTK_WIDGET(ws_label_box), "workspace-label-box");
    gtk_widget_set_halign(GTK_WIDGET(ws_label_box), GTK_ALIGN_START);

    gtk_container_add(GTK_CONTAINER(ws_box), GTK_WIDGET(ws_label_box));

    /* Workspace label */
    gtk_widget_set_name(GTK_WIDGET(widgets->ws_label), "workspace-label");
    gtk_widget_set_halign(GTK_WIDGET(widgets->ws_label), GTK_ALIGN_START);

    gtk_container_add(GTK_CONTAINER(ws_label_box),
                      GTK_WIDGET(widgets->ws_label));

    /* Workspace corner */
    gtk_widget_set_name(GTK_WIDGET(ws_corner), "workspace-corner");
    gtk_widget_set_halign(GTK_WIDGET(ws_corner), GTK_ALIGN_START);

    gtk_container_add(GTK_CONTAINER(ws_box), GTK_WIDGET(ws_corner));
}


void
create_time_indicator(struct bar_widgets *widgets)
{
    GtkBox
        *tm_box       = new_hbox(0),
        *tm_label_box = new_hbox(0),
        *tm_corner_l  = new_hbox(0),
        *tm_corner_r  = new_hbox(0);

    widgets->time_label = GTK_LABEL(gtk_label_new("00:00 Sun 0 Jan"));

    /* Time container */
    gtk_widget_set_halign(GTK_WIDGET(tm_box), GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(GTK_WIDGET(tm_box), TRUE);

    gtk_container_add(GTK_CONTAINER(widgets->box), GTK_WIDGET(tm_box));

    /* Time corner left */
    gtk_widget_set_name(GTK_WIDGET(tm_corner_l), "time-corner-l");
    gtk_widget_set_halign(GTK_WIDGET(tm_corner_l), GTK_ALIGN_START);

    gtk_container_add(GTK_CONTAINER(tm_box), GTK_WIDGET(tm_corner_l));

    /* Time label container */
    gtk_widget_set_name(GTK_WIDGET(tm_label_box), "time-label-box");
    gtk_widget_set_halign(GTK_WIDGET(tm_label_box), GTK_ALIGN_START);

    gtk_container_add(GTK_CONTAINER(tm_box), GTK_WIDGET(tm_label_box));

    /* Time label */
    gtk_widget_set_name(GTK_WIDGET(widgets->time_label), "time-label");
    gtk_widget_set_halign(GTK_WIDGET(widgets->time_label), GTK_ALIGN_START);

    gtk_container_add(GTK_CONTAINER(tm_label_box),
                      GTK_WIDGET(widgets->time_label));

    /* Time corner right */
    gtk_widget_set_name(GTK_WIDGET(tm_corner_r), "time-corner-r");
    gtk_widget_set_halign(GTK_WIDGET(tm_corner_r), GTK_ALIGN_START);

    gtk_container_add(GTK_CONTAINER(tm_box), GTK_WIDGET(tm_corner_r));
}


void
create_bat_indicator(struct bar_widgets *widgets)
{
    GtkBox
        *bat_box       = new_hbox(0),
        *bat_corner    = new_hbox(0),
        *bat_label_box = new_hbox(0);

    widgets->battery_label = GTK_LABEL(gtk_label_new("0%"));

    /* Battery container */
    gtk_widget_set_halign(GTK_WIDGET(bat_box), GTK_ALIGN_FILL);

    gtk_container_add(GTK_CONTAINER(widgets->box), GTK_WIDGET(bat_box));

    /* Battery corner */
    gtk_widget_set_name(GTK_WIDGET(bat_corner), "battery-corner");
    gtk_widget_set_halign(GTK_WIDGET(bat_corner), GTK_ALIGN_START);

    gtk_container_add(GTK_CONTAINER(bat_box), GTK_WIDGET(bat_corner));

    /* Battery label container */
    gtk_widget_set_name(GTK_WIDGET(bat_label_box), "battery-label-box");
    gtk_widget_set_halign(GTK_WIDGET(bat_label_box), GTK_ALIGN_START);

    gtk_container_add(GTK_CONTAINER(bat_box), GTK_WIDGET(bat_label_box));

    /* Battery label */
    gtk_widget_set_name(GTK_WIDGET(widgets->battery_label), "battery-label");
    gtk_widget_set_halign(GTK_WIDGET(widgets->battery_label), GTK_ALIGN_START);

    gtk_container_add(
        GTK_CONTAINER(bat_label_box), GTK_WIDGET(widgets->battery_label));
}


GtkBox *
new_hbox(gint32 spacing)
{
    GtkBox *box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, spacing));

    gtk_widget_set_halign(GTK_WIDGET(box), GTK_ALIGN_FILL);
    return box;
}


void
update_label(gpointer data)
{
    gtk_label_set_text(((struct data *)data)->label,
                       ((struct data *)data)->text);
}
