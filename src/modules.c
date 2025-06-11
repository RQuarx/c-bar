#include <sys/socket.h>
#include <sys/un.h>

#include <cJSON.h>

#include "modules.h"
#include "bar.h"


static struct data data = { .label = nullptr };


gchar *
get_sway_socket_path(void)
{
    gchar path[PATH_MAX] = { '\0' };
    FILE *fp             = popen(SOCKET_CMD, "r");

    if (fp == nullptr) return nullptr;

    if (fgets(path, sizeof(path), fp) != nullptr)
        path[strcspn(path, "\n")] = 0;
    else {
        pclose(fp);
        return nullptr;
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
    if (payload == nullptr || read(sockfd, payload, length) <= -1) goto err;
    payload[length] = '\0';

    cJSON
        *root    = cJSON_Parse(payload),
        *type    = nullptr,
        *current = nullptr,
        *name    = nullptr;

    g_free(payload);
    if (!root) goto err;

    type = cJSON_GetObjectItem(root, "change");
    if (type == nullptr) goto err;

    if (strncmp(type->valuestring, "focus", 5) == 0) {
        current = cJSON_GetObjectItem(root, "current");
        name    = cJSON_GetObjectItem(current, "name");

        strncpy(workspace_name, name->valuestring, 3);
    }

    cJSON_Delete(root);
    return;

err:
    strncpy(workspace_name, "1", 2);
}


gpointer
ws_listener(gpointer args)
{
    gchar *socket_path = get_sway_socket_path();
    if    (socket_path == nullptr) return nullptr;

    gint32    sockfd = 0;
    unix_sock addr   = {};

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) return nullptr;

    memset(&addr, 0, sizeof(unix_sock));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, strlen(socket_path));

    if (connect(sockfd, SOCKADDR(&addr), sizeof(unix_sock)) == -1) {
        close(sockfd);
        return nullptr;
    }

    g_free(socket_path);
    send_ipc_message(sockfd, SWAY_IPC_MESSAGE_TYPE, "[\"workspace\"]");

    gchar header [6];
    guint32
        length = 0,
        type   = 0;

    while (TRUE) {
        if (read   (sockfd, header, 6) != 6 ||
            strncmp(header, MAGIC, 6)  != 0) break;

        read(sockfd, &length, 4);
        read(sockfd, &type,   4);

        read_workspace_json(data.text, sockfd, length);

        data.label = WIDGETS(args)->ws_label;
        g_idle_add_once(update_label, &data);
    }

    close(sockfd);
    return nullptr;
}


gpointer
battery_listener(gpointer args)
{
    FILE *bat_file = nullptr;

    while (TRUE) {
        bat_file     = fopen(BAT_FILE, "r");
        if (bat_file == nullptr) break;

        for (guint8 i = 0; i < 20; i++) data.text[i] = '\0';
        fseek(bat_file, 0, SEEK_SET);

        if (fgets(data.text, 5, bat_file) == nullptr) break;
        fclose(bat_file);

        for (guint8 i = 0; i < 4; i++) {
            if (data.text[i] != '\n') continue;

            data.text[i] = '%';
            break;
        }

        data.label = WIDGETS(args)->battery_label;
        g_idle_add_once(update_label, &data);
        sleep(60);
    }

    return nullptr;
}


gpointer
time_listener(gpointer args)
{
    struct tm time_info = {};
    time_t    now       = 0;

    while (TRUE) {
        now = time(nullptr);
        localtime_r(&now, &time_info);

        strftime(data.text, 20, "%H:%M %a %d %b", &time_info);

        data.label = WIDGETS(args)->time_label;
        g_idle_add_once(update_label, &data);

        now = time(nullptr);
        localtime_r(&now, &time_info);
        sleep(60 - time_info.tm_sec);
    }

    return nullptr;
}
