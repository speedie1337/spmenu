/* See LICENSE file for copyright and license details. */

#include <errno.h>
#include <time.h>

#ifndef MAX
#define MAX(A, B)               ((A) > (B) ? (A) : (B))
#endif
#ifndef MIN
#define MIN(A, B)               ((A) < (B) ? (A) : (B))
#endif
#ifndef BETWEEN
#define BETWEEN(X, A, B)        ((A) <= (X) && (X) <= (B))
#endif
#ifndef INTERSECT
#define INTERSECT(x,y,w,h,r)    (MAX(0, MIN((x)+(w),(r).x_org+(r).width)  - MAX((x),(r).x_org)) \
        && MAX(0, MIN((y)+(h),(r).y_org+(r).height) - MAX((y),(r).y_org)))
#endif
#ifndef LENGTH
#define LENGTH(X)               (sizeof X / sizeof X[0])
#endif
#ifndef TEXTW
#define TEXTW(X)                (draw_font_getwidth(draw, (X), False) + sp.lrpad)
#endif
#ifndef TEXTWM
#define TEXTWM(X)               (draw_font_getwidth(draw, (X), True) + sp.lrpad)
#endif
#ifndef NUMBERSMAXDIGITS
#define NUMBERSMAXDIGITS        100
#endif
#ifndef NUMBERSBUFSIZE
#define NUMBERSBUFSIZE          (NUMBERSMAXDIGITS * 2) + 1
#endif
#ifndef MAXITEMLENGTH
#define MAXITEMLENGTH           1024
#endif

size_t sp_strncpy(char *restrict dst, const char *restrict src, size_t size);
void die(const char *fmt, ...);
void *ecalloc(size_t nmemb, size_t size);

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

void *ecalloc(size_t nmemb, size_t size) {
    void *p;

    if (!(p = calloc(nmemb, size)))
        die("calloc:");
    return p;
}

int msleep(long sec) {
    struct timespec ts;
    int ret;

    if (sec < 0) {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = sec / 1000;
    ts.tv_nsec = (sec % 1000) * 1000000;

    do {
        ret = nanosleep(&ts, &ts);
    } while (ret && errno == EINTR);

    return ret;
}
