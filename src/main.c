#include "modules.h"
#include "bar.h"


gint32
main(gint32   argc,
     gchar  **argv)
{
    gtk_init(&argc, &argv);

    if (create_bar() == FALSE) return EXIT_FAILURE;

    pthread_t
        workspace_thread    = 0,
        bat_and_time_thread = 0;

    pthread_create(&workspace_thread,    NULL, ws_listener_thread,    NULL);
    pthread_create(&bat_and_time_thread, NULL, bat_and_time_listener, NULL);

    gtk_main();
    return EXIT_SUCCESS;
}
