#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include <linux/limits.h>
#include <sys/socket.h>
#include <gtk/gtk.h>
#include <sys/un.h>

#include "modules/workspace.h"
#include "utils.h"
#include "bar.h"


char *
get_sway_socket_path(void)
{
    char path[PATH_MAX] = { 0 };
    FILE *fp            = popen(SOCKET_CMD, "r");

    if (fp == NULL) {
        print_err(
            "get_sway_socket_path(): Failed to run command %s: %s",
            SOCKET_CMD, ERR
        );
        return NULL;
    }

    if (fgets(path, sizeof(path), fp) != NULL) {
        path[strcspn(path, "\n")] = 0;
    } else {
        print_err("get_sway_socket_path(): Failed to fgets: %s", ERR);
        pclose(fp);
        return NULL;
    }

    pclose(fp);

    return s_strdup(path);
}


int32_t
send_ipc_message(int32_t sockfd, uint32_t type, const char *payload)
{
    size_t payload_len = strlen(payload);
    write(sockfd, MAGIC, 6);
    write(sockfd, &payload_len, sizeof(uint32_t));
    write(sockfd, &type, sizeof(uint32_t));
    write(sockfd, payload, payload_len);
    return 0;
}


void *
ws_listener_thread(void * /* arg */)
{
    char *socket_path = get_sway_socket_path();
    if (socket_path == NULL) return NULL;

    int32_t   sockfd = 0;
    unix_sock addr   = {};

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) {
        print_err(
            "socket(...): Failed to create a new socket: %s",
            ERR
        );
        return NULL;
    }

    memset(&addr, 0, sizeof(unix_sock));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, strlen(socket_path));

    if (connect(sockfd, SOCKADDR(&addr), sizeof(unix_sock)) == -1) {
        print_err("connect(...): Failed to connect to a socket: %s", ERR);
        close(sockfd);
        return NULL;
    }

    free(socket_path);
    send_ipc_message(sockfd, SWAY_IPC_MESSAGE_TYPE, "[\"workspace\"]");

    while (1) {
        char header[6];
        uint32_t
            length = 0,
            type   = 0;

        if (read(sockfd, header, 6) != 6 || strncmp(header, MAGIC, 6) != 0)
            break;

        read(sockfd, &length, sizeof(uint32_t));
        read(sockfd, &type,   sizeof(uint32_t));

        char *workspace_name = read_workspace_json(sockfd, length);
        if (workspace_name == NULL) continue;
        g_idle_add(update_workspace_label, workspace_name);
    }

    close(sockfd);
    return NULL;
}


char *
read_workspace_json(int32_t sockfd, uint32_t length)
{
    char
        *workspace_name = NULL,
        *payload        = s_malloc(length + 1);

    if (payload == NULL)                       return NULL;
    if (s_read(sockfd, payload, length) <= -1) return NULL;

    cJSON *root = cJSON_Parse(payload);
    free(payload);

    if (!root) {
        print_err(
            "read_workspace_json(...): Failed to parse JSON:\n%s",
            payload
        );
        return NULL;
    }

    cJSON *type = cJSON_GetObjectItem(root, "change");

    if (type == NULL) return NULL;

    if (strncmp(type->valuestring, "focus", 5) == 0) {
        cJSON *current = cJSON_GetObjectItem(root, "current");
        if (current == NULL) return NULL;

        cJSON *name = cJSON_GetObjectItem(current, "name");
        if (cJSON_IsString(name)) workspace_name = s_strdup(name->valuestring);
    }

    cJSON_Delete(root);
    return workspace_name;
}
