#include <unistd.h>

#include <sys/socket.h>
#include <sys/un.h>

#include <cJSON.h>

#include "modules.h"
#include "utils.h"
#include "bar.h"


gchar *
get_sway_socket_path(void)
{
    gchar path[PATH_MAX] = { 0 };
    FILE *fp             = popen(SOCKET_CMD, "r");

    if (fp == NULL) {
        print_err("get_sway_socket_path(): Failed to run command %s: %s",
                  SOCKET_CMD, ERR);
        return NULL;
    }

    if (fgets(path, sizeof(path), fp) != NULL)
        path[strcspn(path, "\n")] = 0;
    else {
        print_err("get_sway_socket_path(): Failed to fgets: %s", ERR);
        pclose(fp);
        return NULL;
    }

    pclose(fp);
    return strdup(path);
}


gint32
send_ipc_message(gint32       sockfd,
                 guint32      type,
                 const gchar *payload)
{
    gsize payload_len = strlen(payload);
    if (write(sockfd, MAGIC, 6)             < 0) return FALSE;
    if (write(sockfd, &payload_len, 4)      < 0) return FALSE;
    if (write(sockfd, &type, 4)             < 0) return FALSE;
    if (write(sockfd, payload, payload_len) < 0) return FALSE;
    return TRUE;
}


void
read_workspace_json(gchar  *workspace_name,
                    gint32  sockfd,
                    guint32 length)
{
    gchar *payload = g_malloc(length + 1);
    if (payload == NULL || read(sockfd, payload, length) <= -1) goto err;

    cJSON
        *root    = cJSON_Parse(payload),
        *type    = NULL,
        *current = NULL,
        *name    = NULL;

    g_free(payload);
    if (!root) {
        print_err("read_workspace_json(...): Failed to parse JSON:\n%s",
                  payload);
        goto err;
    }

    type = cJSON_GetObjectItem(root, "change");
    if (type == NULL) goto err;

    if (strncmp(type->valuestring, "focus", 5) == 0) {
        current = cJSON_GetObjectItem(root, "current");
        if (current == NULL) goto err;

        name = cJSON_GetObjectItem(current, "name");
        if (!cJSON_IsString(name)) goto err;

        strncpy(workspace_name, name->valuestring, 3);
    }

    cJSON_Delete(root);
    return;

err:
    strncpy(workspace_name, "1\0", 2);
}


gpointer
ws_listener_thread(gpointer /* args */)
{
    gchar *socket_path = get_sway_socket_path();
    if    (socket_path == NULL) return NULL;

    gint32    sockfd = 0;
    unix_sock addr   = {};

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) {
        print_err("socket(...): Failed to create a new socket: %s",
                  ERR);
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

    g_free(socket_path);
    send_ipc_message(sockfd, SWAY_IPC_MESSAGE_TYPE, "[\"workspace\"]");

    gchar
        *workspace_name = g_malloc(3),
        *header         = g_malloc(6);
    guint32
        length = 0,
        type   = 0;

    while (TRUE) {
        if (read   (sockfd, header, 6) != 6 ||
            strncmp(header, MAGIC, 6)  != 0) break;

        read(sockfd, &length, 4);
        read(sockfd, &type,   4);

        read_workspace_json(workspace_name, sockfd, length);
        if (workspace_name == NULL) continue;
        g_idle_add_once(update_workspace_label, workspace_name);
    }

    g_free(workspace_name);
    g_free(header);
    close(sockfd);
    return NULL;
}


gpointer
bat_and_time_listener(gpointer /* args */)
{
    FILE      *bat_file  = NULL;
    struct tm *time_info = g_new(struct tm, 1);
    time_t     now       = 0;
    guint8     cycle     = 4;
    gchar
        *label     = g_malloc(20),
        *bat_level = g_malloc(5);

    while (TRUE) {
        if (cycle == 4) {
            bat_file     = fopen(BAT_FILE, "r");
            if (bat_file == NULL) break;

            for (guint8 i = 0; i < 5; i++) bat_level[i] = '\0';
            fseek(bat_file, 0, SEEK_SET);

            if (fgets(bat_level, 5, bat_file) == NULL) break;
            fclose(bat_file);

            for (guint8 i = 0; i < 4; i++) {
                if (bat_level[i] != '\n') continue;

                bat_level[i] = '%';
                break;
            }

            g_idle_add_once(update_battery_label, bat_level);
            cycle = 0;
        }

        now = time(NULL);
        localtime_r(&now, time_info);

        strftime(label, 20, "%H:%M %a %d %b", time_info);

        g_idle_add_once(update_time_label, label);

        cycle++;
        sleep(1);
    }

    g_free(bat_level);
    g_free(time_info);
    g_free(label);

    return NULL;
}
