#pragma once
#ifndef __MODULES__BATTERY_H__
#define __MODULES__BATTERY_H__

#include <stdio.h>

static const char *const BAT_CAPACITY_FILE =
    "/sys/class/power_supply/BAT0/capacity";

static FILE *battery_capacity_file = NULL;


void *battery_level_reader(void *args);

#endif /* __MODULES__BATTERY_H__ */
