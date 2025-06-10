#include "modules/workspace.h"
#include "modules/battery.h"
#include "modules/time.h"
#include "bar.h"


int32_t
main(int32_t argc, char **argv)
{
    gtk_init(&argc, &argv);

    if (create_bar() == FALSE) return EXIT_FAILURE;

    pthread_t
        workspace_thread = 0,
        time_thread      = 0,
        battery_thread   = 0;

    pthread_create(&workspace_thread, NULL, ws_listener_thread, NULL);
    pthread_create(&time_thread,      NULL, time_listener, NULL);
    pthread_create(&battery_thread,   NULL, battery_level_reader, NULL);

    gtk_main();
    return EXIT_SUCCESS;
}
