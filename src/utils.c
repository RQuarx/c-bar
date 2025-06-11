#include <stdarg.h>
#include <stdio.h>

#include <sys/time.h>

#include "utils.h"


void
fprint_time(FILE *restrict stream)
{
    struct timeval tv                  = {};
    struct tm     *tm_info             = g_new(struct tm, 1);
    time_t         seconds_since_epoch = time(NULL);

    gettimeofday(&tv, NULL);
    localtime_r(&seconds_since_epoch, tm_info);

    gint64 milliseconds = tv.tv_usec / 1000;
    gint32
        minutes = tm_info->tm_min,
        seconds = tm_info->tm_sec;

    g_free(tm_info);
    fprintf(stream, "%02d:%02d.%03ld", minutes, seconds, milliseconds);
}


void
fprint_label(FILE        *restrict stream,
             const gchar *restrict label_text,
             guint8                label_color)
{
    fprint_time(stream);
    fprintf    (stream,
                " \033[1;37m[\033[1;%hhdm%s\033[1;37m]\033[0;0m: ",
                label_color, label_text);
}


void
print_err(const gchar *restrict fmt, ...)
{
    va_list  arg = {};
    va_start(arg, fmt);

    fprint_label(stderr, "ERR", 31);
    vfprintf    (stderr, fmt, arg);
    fputc('\n',  stderr);
    fflush      (stderr);

    va_end(arg);
}


void
print_log(const gchar *restrict fmt, ...)
{
    va_list arg = {};
    va_start(arg, fmt);

    fprint_label(stdout, "LOG", 32);
    vfprintf    (stdout, fmt, arg);
    fputc('\n',  stdout);
    fflush      (stdout);

    va_end(arg);
}
