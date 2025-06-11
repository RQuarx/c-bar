#include "modules.h"
#include "bar.h"


gint32
main(gint32   argc,
     gchar  **argv)
{
    gtk_init(&argc, &argv);

    struct bar_widgets widgets = {};

    create_bar(&widgets);

    pthread_t
        ws_thread   = 0,
        bat_thread  = 0,
        time_thread = 0;

    pthread_create(&time_thread, nullptr, time_listener,    &widgets);
    pthread_create(&ws_thread,   nullptr, ws_listener,      &widgets);
    pthread_create(&bat_thread,  nullptr, battery_listener, &widgets);

    gtk_main();
    return EXIT_SUCCESS;
}
