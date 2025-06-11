#pragma once
#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <glib.h>

#define ERR              strerror(errno)
#define SOCKADDR(ptr)    ((struct sockaddr *)ptr)
#define SOCKADDR_UN(ptr) ((struct sockaddr_un *)ptr)

typedef struct sockaddr_un unix_sock;


void

    /**
     * @brief Prints the current time in format MM:SS.MS to @p stream.
     *
     * @param stream The stream that the time will be outputted to.
     */
    fprint_time(FILE *restrict stream),

    /**
     * @brief Prints @p label_text to @p stream .
     *
     * @param stream      The stream that the label will be outputted to.
     * @param label_text  The text that will be put inside the label.
     * @param label_color The color of @p label_text .
     */
    fprint_label(FILE        *restrict stream,
                 const gchar *restrict label_text,
                 guint8                label_color),

    /**
     * @brief Prints a formatted error message to stderr.
     *
     * @param fmt Format string.
     * @param ... Arguments for the format string.
     */
     print_err(const gchar *restrict fmt, ...),

    /**
     * @brief Prints a formatted log message to stdout.
     *
     * @param fmt Format string.
     * @param ... Arguments for the format string.
     */
    print_log(const gchar *restrict fmt, ...);

#endif /* __UTILS_H__ */
