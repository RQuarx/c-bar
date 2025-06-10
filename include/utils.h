#pragma once
#ifndef __UTILS_H__
#define __UTILS_H__

#include <stddef.h>
#include <stdint.h>

#define ERR              strerror(errno)
#define SOCKADDR(ptr)    ((struct sockaddr *)ptr)
#define SOCKADDR_UN(ptr) ((struct sockaddr_un *)ptr)

typedef struct sockaddr_un unix_sock;


void print_err(const char *restrict fmt, ...);
void print_log(const char *restrict fmt, ...);
void print_dbg(const char *restrict fmt, ...);


char   *s_strdup(const char *ptr);
int8_t  s_read(int32_t fd, char *buff, size_t nbytes);

void *s_malloc(size_t nbytes);


#endif /* __UTILS_H__ */
