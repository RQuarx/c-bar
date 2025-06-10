#include "utils.h"
#include "bar.h"


gboolean
create_bar()
{
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "theme.css", NULL);
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(cssProvider),
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );

    if (create_window()         == FALSE) return FALSE;
    if (create_container()      == FALSE) return FALSE;
    if (create_ws_indicator()   == FALSE) return FALSE;
    if (create_time_indicator() == FALSE) return FALSE;
    if (create_bat_indicator()  == FALSE) return FALSE;

    gtk_widget_show_all(GTK_WIDGET(bar_window));
    return TRUE;
}


gboolean
create_window()
{
    bar_window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));

    if (bar_window == NULL) {
        print_err("create_window(): Failed to create window: %s", ERR);
        return FALSE;
    }

    gtk_layer_init_for_window(bar_window);

    gtk_widget_set_name(GTK_WIDGET(bar_window), "bar-window");
    gtk_window_set_title(bar_window, "c-bar");

    gtk_layer_set_anchor(bar_window, GTK_LAYER_SHELL_EDGE_BOTTOM, TRUE);
    gtk_layer_set_exclusive_zone(bar_window, 21);

    gtk_widget_set_size_request(GTK_WIDGET(bar_window), W_WIDTH, W_HEIGHT);
    gtk_window_resize(bar_window, W_WIDTH, W_HEIGHT);

    print_log("create_window(): Successfully created a window.");
    return TRUE;
}


gboolean
create_container()
{
    bar_box = GTK_BOX(new_hbox(0));

    if (bar_box == NULL) return FALSE;

    gtk_widget_set_name(GTK_WIDGET(bar_box), "bar-box");

    gtk_widget_set_halign(GTK_WIDGET(bar_box), GTK_ALIGN_FILL);
    gtk_widget_set_hexpand(GTK_WIDGET(bar_box), TRUE);

    gtk_container_add(GTK_CONTAINER(bar_window), GTK_WIDGET(bar_box));

    print_log("create_container(): Successfully created a container.");
    return TRUE;
}


gboolean
create_ws_indicator()
{
    GtkBox
        *ws_box       = new_hbox(0),
        *ws_label_box = new_hbox(0),
        *ws_corner    = new_hbox(0);

    bar_ws_label = GTK_LABEL(gtk_label_new("1"));

    if (ws_box    == NULL || ws_label_box == NULL||
        ws_corner == NULL || bar_ws_label == NULL
    ) {
        print_err(
            "create_ws_indicator(): Failed to create workspace indicator: %s",
            ERR
        );
        return FALSE;
    }

    /* Workspace container */
    gtk_widget_set_name(GTK_WIDGET(ws_box), "workspace-container");
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

    print_log(
        "create_ws_indicator(): Successfully created a workspace indicator."
    );
    return TRUE;
}


gboolean
create_time_indicator()
{
    GtkBox
        *tm_box       = new_hbox(0),
        *tm_label_box = new_hbox(0),
        *tm_corner_l  = new_hbox(0),
        *tm_corner_r  = new_hbox(0);

    bar_time_label = GTK_LABEL(gtk_label_new("00:00 Sun 0 Jan"));

    if (tm_box         == NULL || tm_label_box  == NULL||
        tm_corner_l    == NULL || tm_corner_r   == NULL ||
        bar_time_label == NULL
    ) {
        print_err(
            "create_time_indicator(): Failed to create time indicator: %s",
            ERR
        );
        return FALSE;
    }

    /* Time container */
    gtk_widget_set_name(GTK_WIDGET(tm_box), "time-container");
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

    print_log(
        "create_time_indicator(): Successfully created a time indicator."
    );

    return TRUE;
}


gboolean
create_bat_indicator()
{
    GtkBox
        *bat_box       = new_hbox(0),
        *bat_corner    = new_hbox(0),
        *bat_label_box = new_hbox(0);

    bar_battery_label = GTK_LABEL(gtk_label_new("0%"));

    if (bat_box    == NULL || bat_label_box     == NULL||
        bat_corner == NULL || bar_battery_label == NULL
    ) {
        print_err(
            "create_bat_indicator(): Failed to create battery indicator: %s",
            ERR
        );
        return FALSE;
    }

    /* Battery container */
    gtk_widget_set_name(GTK_WIDGET(bat_box), "battery-container");
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

    print_log(
        "create_bat_indicator(): Successfully created a battery indicator."
    );
    return TRUE;
}


GtkBox *
new_hbox(int32_t spacing)
{
    GtkBox *box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));

    gtk_widget_set_halign(GTK_WIDGET(box), GTK_ALIGN_FILL);

    if (box == NULL) {
        print_err("new_hbox(%d): Failed to create box.", spacing);
        return NULL;
    }

    return box;
}


gboolean
update_workspace_label(gpointer data)
{
    gtk_label_set_text(bar_ws_label, (char*)data);
    free(data);
    return FALSE;
}


gboolean
update_time_label(gpointer args)
{
    char *label = s_malloc(20);
    strftime(label, 20, "%H:%M %a %d %b", (struct tm *)args);

    gtk_label_set_text(bar_time_label, label);

    free(label);
    return FALSE;
}


gboolean
update_battery_label(gpointer args)
{
    gtk_label_set_text(bar_battery_label, (char *)args);
    return FALSE;
}
