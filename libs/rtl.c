/* See LICENSE file for copyright and license details. */

#if USERTL
#include <fribidi.h>
#endif

static char fribidi_text[BUFSIZ] = "";
static void apply_fribidi(char *str);

#if USERTL
void apply_fribidi(char *str) {
    FriBidiStrIndex len = strlen(str);
    FriBidiChar logical[BUFSIZ];
    FriBidiChar visual[BUFSIZ];
    FriBidiParType base = FRIBIDI_PAR_ON;
    FriBidiCharSet charset;
    fribidi_boolean result = 0;

    fribidi_text[0] = 0;

    if (len > 0) {
        charset = fribidi_parse_charset("UTF-8");
        len = fribidi_charset_to_unicode(charset, str, len, logical);
        result = fribidi_log2vis(logical, len, &base, visual, NULL, NULL, NULL);
        len = fribidi_unicode_to_charset(charset, visual, len, fribidi_text);
    }

    if (result||!result) return;
}
#else
void apply_fribidi(char *str) {
    return;
}
#endif
