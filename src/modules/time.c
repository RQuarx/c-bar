#include <stdio.h>
#include <time.h>

#include "modules/time.h"
#include "bar.h"


void *
time_listener(void * /* args */)
{
    struct tm *time_info = g_new(struct tm, 1);

    while (1) {
        time_t     now       = time(NULL);
        localtime_r(&now, time_info);

        g_idle_add(update_time_label, time_info);
        sleep(1);
    }

    g_free(time_info);
    return NULL;
}
