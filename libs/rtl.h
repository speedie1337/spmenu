#if USERTL
#include "rtl-c.h"
#else
static char fribidi_text[BUFSIZ] = "";
static void apply_fribidi(char *str);
#endif
