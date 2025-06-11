#include "modules.h"
#include "bar.h"


gint32
main(gint32   argc,
     gchar  **argv)
{
    gtk_init(&argc, &argv);

    struct bar_widgets widgets = {};
    create_bar(&widgets);

    pthread_t threads[3];
    pthread_create(&threads[0], nullptr, time_listener,    &widgets);
    pthread_create(&threads[1], nullptr, ws_listener,      &widgets);
    pthread_create(&threads[2], nullptr, battery_listener, &widgets);

    gtk_main();
    return EXIT_SUCCESS;
}
