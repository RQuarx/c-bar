#pragma once
#ifndef __MODULES__WORKSPACE_H__
#define __MODULES__WORKSPACE_H__

#include <stdint.h>

#include <cJSON.h>
#include <glib.h>


static const char
    *const SOCKET_CMD = "/usr/bin/sway --get-socketpath",
    *const MAGIC      = "i3-ipc";

static const int32_t
    SWAY_IPC_MESSAGE_TYPE = 2,
    EVENT_WORKSPACE       = (1 << 1);


/**
 * @brief Returns a malloc'd string containing sway socket path.
 */
char *get_sway_socket_path(void);

int32_t send_ipc_message(int32_t sockfd, uint32_t type, const char *payload);

void *ws_listener_thread(void *arg);

char *read_workspace_json(int32_t sockfd, uint32_t length);


#endif /* __MODULES__WORKSPACE_H__ */
