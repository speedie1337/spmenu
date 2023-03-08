#if PANGO
#define USEPANGO 1
#else
#define USEPANGO 0
#endif

#if USEPANGO
#include "draw-pango.h"
#else
#include "draw-xft.h"
#endif
