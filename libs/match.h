/* See LICENSE file for copyright and license details. */

#if USEREGEX
#include "regex/regex.h"
#endif

static void fuzzymatch(void);
static void match(void);
static int matchregex(const char *t, const char *itt);
static int compare_distance(const void *a, const void *b);
