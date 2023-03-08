#if PANGO
#define USEPANGO 1
#else
#define USEPANGO 0
#endif

#if USEPANGO
#include "draw-pango.c"
#else
#include "draw-xft.c"
#endif
