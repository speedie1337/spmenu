/* See LICENSE file for copyright and license details. */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

void * ecalloc(size_t nmemb, size_t size) {
    void *p;

    if (!(p = calloc(nmemb, size)))
        die("calloc:");
    return p;
}

size_t sp_strncpy(char *restrict dst, const char *restrict src, size_t size) {
    int offset;

    offset = 0;

    if (size > 0) {
        while (*(src+offset) != '\0' ) {
            if (offset == size) {
                offset--;
                break;
            }

            *(dst+offset) = *(src+offset);
            offset++;
        }
    }

    *(dst+offset) = '\0';

    while (*(src+offset) != '\0')
        offset++;

    return offset;
}

void die(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    if (fmt[0] && fmt[strlen(fmt)-1] == ':') {
        fputc(' ', stderr);
        perror(NULL);
    } else {
        fputc('\n', stderr);
    }

    exit(1);
}
