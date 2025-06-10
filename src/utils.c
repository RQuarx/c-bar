#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include "utils.h"


void
print_err(const char *restrict fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    fprintf(stderr, "err: ");
    vfprintf(stderr, fmt, arg);
    va_end(arg);
    fputc('\n', stderr);
    fflush(stderr);
}


void
print_log(const char *restrict fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    fprintf(stdout, "log: ");
    vfprintf(stdout, fmt, arg);
    va_end(arg);
    fputc('\n', stdout);
    fflush(stdout);
}


void
print_dbg(const char *restrict fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    fprintf(stdout, "debug: ");
    vfprintf(stdout, fmt, arg);
    va_end(arg);
    fputc('\n', stdout);
    fflush(stdout);
}

char *
s_strdup(const char *ptr)
{
    char *new_ptr = strdup(ptr);

    if (new_ptr == NULL) {
        print_err(
            "s_strdup(\"%s\"): Failed to duplicate string %s: %s",
            ptr, ptr, strerror(errno)
        );
        return NULL;
    }

    return new_ptr;
}


int8_t
s_read(int32_t fd, char *buff, size_t nbytes)
{
    size_t total = 0;

    while (total < nbytes) {
        ssize_t n = read(fd, buff + total, nbytes - total);
        if (n > 0) {
            total += n;
            continue;
        }

        if (n == 0) {
            free(buff);
            return -1;
        }

        print_err("s_read(...): Error occured while trying to read: %s", ERR);
    }

    buff[nbytes] = '\0';
    return 1;
}


void *
s_malloc(size_t nbytes)
{
    void *ptr = malloc(nbytes);
    if (ptr != NULL) return ptr;

    print_err(
        "s_malloc(%zu): Failed to allocate %zu bytes of memory: %s",
        nbytes, nbytes, ERR
    );
    return NULL;
}
