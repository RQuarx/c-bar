#include "modules/battery.h"
#include "bar.h"

#include "utils.h"

void *
battery_level_reader(void * /* args */)
{
    char *bat_level = s_malloc(5);

    while (1) {
        battery_capacity_file = fopen(BAT_CAPACITY_FILE, "r");
        if (battery_capacity_file == NULL) break;

        for (uint8_t i = 0; i < 5; i++) bat_level[i] = '\0';
        fseek(battery_capacity_file, 0, SEEK_SET);

        if (fgets(bat_level, 5, battery_capacity_file) != NULL) {
            for (uint8_t i = 0; i < 3; i++) {
                if (bat_level[i] == '\n') {
                    bat_level[i] = '%';
                    break;
                }
            }

            g_idle_add(update_battery_label, bat_level);
        }

        fclose(battery_capacity_file);
        sleep(2);
    }

    free(bat_level);
    return NULL;
}
