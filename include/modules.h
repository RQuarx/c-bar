#pragma once
#ifndef __MODULES__MODULES_H__
#define __MODULES__MODULES_H__

#include <glib.h>

static const char
    *const SOCKET_CMD = "/usr/bin/sway --get-socketpath",
    *const MAGIC      = "i3-ipc",
    *const BAT_FILE   = "/sys/class/power_supply/BAT0/capacity";

static const int32_t
    SWAY_IPC_MESSAGE_TYPE = 2,
    EVENT_WORKSPACE       = (1 << 1);

typedef struct _GtkLabel GtkLabel;


struct data
{
    GtkLabel *label;
    gchar     text[20];
};


static struct data data = {
    .label = nullptr
};


/**
 * @brief Returns a string containing a sway socket path.
 *
 * @return A valid path if the socket path is found, or nullptr.
 */
gchar *get_sway_socket_path(void);


/**
 * @brief Sends an IPC message to @p fd .
 *
 * @param fd      IPC socket file descriptor.
 * @param type    The type of the message.
 * @param payload The received message.
 * @return TRUE on success, or FALSE on failure.
 */
gboolean send_ipc_message(gint32                fd,
                          guint32               type,
                          const gchar *restrict payload);


/**
 * @brief Reads a JSON and returns the current workspace name.
 *
 * @param workspace_name A malloc'ed string containing the workspace_name.
 * @param fd             The sock's file descriptor.
 * @param length         The length of the received payload from the socket.
 */
void read_workspace_json(gchar *restrict workspace_name,
                         gint32          fd,
                         guint32         length);


gpointer

    /** @brief Thread function for sway's workspace listener. */
    ws_listener(gpointer args),

    /** @brief Thread function for current time listener. */
    time_listener(gpointer args),

    /** @brief Thread function for current battery level listener. */
    battery_listener(gpointer reader);


#endif /* __MODULES__MODULES_H__ */
