#include "utils.h"
#include "bar.h"



gboolean
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

    create_window();
    create_container();
    create_ws_indicator();
    create_time_indicator();
    create_bat_indicator();

    gtk_widget_show_all(GTK_WIDGET(bar_window));
    return TRUE;
}


void
create_window()
{
    bar_window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));

    gtk_layer_init_for_window(bar_window);

    gtk_widget_set_name(GTK_WIDGET(bar_window), "bar-window");
    gtk_window_set_title(bar_window, "c-bar");

    gtk_layer_set_anchor(bar_window, GTK_LAYER_SHELL_EDGE_BOTTOM, TRUE);
    gtk_layer_set_exclusive_zone(bar_window, 21);

    gtk_widget_set_size_request(GTK_WIDGET(bar_window), W_WIDTH, W_HEIGHT);
    gtk_window_resize(bar_window, W_WIDTH, W_HEIGHT);
}


void
create_container()
{
    bar_box = GTK_BOX(new_hbox(0));

    gtk_widget_set_name(GTK_WIDGET(bar_box), "bar-box");

    gtk_widget_set_halign(GTK_WIDGET(bar_box), GTK_ALIGN_FILL);
    gtk_widget_set_hexpand(GTK_WIDGET(bar_box), TRUE);

    gtk_container_add(GTK_CONTAINER(bar_window), GTK_WIDGET(bar_box));
}


void
create_ws_indicator()
{
    GtkBox
        *ws_box       = new_hbox(0),
        *ws_label_box = new_hbox(0),
        *ws_corner    = new_hbox(0);

    bar_ws_label = GTK_LABEL(gtk_label_new("1"));

    /* Workspace container */
    gtk_widget_set_halign(GTK_WIDGET(ws_box), GTK_ALIGN_FILL);

    gtk_container_add(GTK_CONTAINER(bar_box), GTK_WIDGET(ws_box));

    /* Workspace label container */
    gtk_widget_set_name(GTK_WIDGET(ws_label_box), "workspace-label-box");
    gtk_widget_set_halign(GTK_WIDGET(ws_label_box), GTK_ALIGN_START);

    gtk_container_add(GTK_CONTAINER(ws_box), GTK_WIDGET(ws_label_box));

    /* Workspace label */
    gtk_widget_set_name(GTK_WIDGET(bar_ws_label), "workspace-label");
    gtk_widget_set_halign(GTK_WIDGET(bar_ws_label), GTK_ALIGN_START);

    gtk_container_add(GTK_CONTAINER(ws_label_box), GTK_WIDGET(bar_ws_label));

    /* Workspace corner */
    gtk_widget_set_name(GTK_WIDGET(ws_corner), "workspace-corner");
    gtk_widget_set_halign(GTK_WIDGET(ws_corner), GTK_ALIGN_START);

    gtk_container_add(GTK_CONTAINER(ws_box), GTK_WIDGET(ws_corner));
}


void
create_time_indicator()
{
    GtkBox
        *tm_box       = new_hbox(0),
        *tm_label_box = new_hbox(0),
        *tm_corner_l  = new_hbox(0),
        *tm_corner_r  = new_hbox(0);

    bar_time_label = GTK_LABEL(gtk_label_new("00:00 Sun 0 Jan"));

    /* Time container */
    gtk_widget_set_halign(GTK_WIDGET(tm_box), GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(GTK_WIDGET(tm_box), TRUE);

    gtk_container_add(GTK_CONTAINER(bar_box), GTK_WIDGET(tm_box));

    /* Time corner left */
    gtk_widget_set_name(GTK_WIDGET(tm_corner_l), "time-corner-l");
    gtk_widget_set_halign(GTK_WIDGET(tm_corner_l), GTK_ALIGN_START);

    gtk_container_add(GTK_CONTAINER(tm_box), GTK_WIDGET(tm_corner_l));

    /* Time label container */
    gtk_widget_set_name(GTK_WIDGET(tm_label_box), "time-label-box");
    gtk_widget_set_halign(GTK_WIDGET(tm_label_box), GTK_ALIGN_START);

    gtk_container_add(GTK_CONTAINER(tm_box), GTK_WIDGET(tm_label_box));

    /* Time label */
    gtk_widget_set_name(GTK_WIDGET(bar_time_label), "time-label");
    gtk_widget_set_halign(GTK_WIDGET(bar_time_label), GTK_ALIGN_START);

    gtk_container_add(GTK_CONTAINER(tm_label_box), GTK_WIDGET(bar_time_label));

    /* Time corner right */
    gtk_widget_set_name(GTK_WIDGET(tm_corner_r), "time-corner-r");
    gtk_widget_set_halign(GTK_WIDGET(tm_corner_r), GTK_ALIGN_START);

    gtk_container_add(GTK_CONTAINER(tm_box), GTK_WIDGET(tm_corner_r));
}


void
create_bat_indicator()
{
    GtkBox
        *bat_box       = new_hbox(0),
        *bat_corner    = new_hbox(0),
        *bat_label_box = new_hbox(0);

    bar_battery_label = GTK_LABEL(gtk_label_new("0%"));

    /* Battery container */
    gtk_widget_set_halign(GTK_WIDGET(bat_box), GTK_ALIGN_FILL);

    gtk_container_add(GTK_CONTAINER(bar_box), GTK_WIDGET(bat_box));

    /* Battery corner */
    gtk_widget_set_name(GTK_WIDGET(bat_corner), "battery-corner");
    gtk_widget_set_halign(GTK_WIDGET(bat_corner), GTK_ALIGN_START);

    gtk_container_add(GTK_CONTAINER(bat_box), GTK_WIDGET(bat_corner));

    /* Battery label container */
    gtk_widget_set_name(GTK_WIDGET(bat_label_box), "battery-label-box");
    gtk_widget_set_halign(GTK_WIDGET(bat_label_box), GTK_ALIGN_START);

    gtk_container_add(GTK_CONTAINER(bat_box), GTK_WIDGET(bat_label_box));

    /* Battery label */
    gtk_widget_set_name(GTK_WIDGET(bar_battery_label), "battery-label");
    gtk_widget_set_halign(GTK_WIDGET(bar_battery_label), GTK_ALIGN_START);

    gtk_container_add(
        GTK_CONTAINER(bat_label_box), GTK_WIDGET(bar_battery_label));
}


GtkBox *
new_hbox(gint32 spacing)
{
    GtkBox *box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, spacing));

    gtk_widget_set_halign(GTK_WIDGET(box), GTK_ALIGN_FILL);
    return box;
}


void
update_workspace_label(gpointer data)
{ gtk_label_set_text(bar_ws_label, (const gchar *)data); }

void
update_time_label(gpointer args)
{ gtk_label_set_text(bar_time_label, (const gchar *)args); }

void
update_battery_label(gpointer args)
{ gtk_label_set_text(bar_battery_label, (const gchar *)args); }
