/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>

#ifndef PANGO
#define USEPANGO 0
#else
#define USEPANGO 1
#ifndef UTF8
#define USEUTF8 0
#else
#define USEUTF8 1
#endif
#endif

#if USEPANGO
#include <pango/pango.h>
#include <pango/pangoxft.h>
#include <iconv.h>
#endif

#include "drw.h"
#include "../sl/main.h"

#if !USEPANGO
#define UTF_INVALID 0xFFFD
#define UTF_SIZ     4

static const unsigned char utfbyte[UTF_SIZ + 1] = {0x80,    0, 0xC0, 0xE0, 0xF0};
static const unsigned char utfmask[UTF_SIZ + 1] = {0xC0, 0x80, 0xE0, 0xF0, 0xF8};
static const long utfmin[UTF_SIZ + 1] = {       0,    0,  0x80,  0x800,  0x10000};
static const long utfmax[UTF_SIZ + 1] = {0x10FFFF, 0x7F, 0x7FF, 0xFFFF, 0x10FFFF};

static long utf8decodebyte(const char c, size_t *i) {
    for (*i = 0; *i < (UTF_SIZ + 1); ++(*i))
        if (((unsigned char)c & utfmask[*i]) == utfbyte[*i])
            return (unsigned char)c & ~utfmask[*i];
    return 0;
}

static size_t utf8validate(long *u, size_t i) {
    if (!BETWEEN(*u, utfmin[i], utfmax[i]) || BETWEEN(*u, 0xD800, 0xDFFF))
        *u = UTF_INVALID;
    for (i = 1; *u > utfmax[i]; ++i)
        ;
    return i;
}

static size_t utf8decode(const char *c, long *u, size_t clen) {
    size_t i, j, len, type;
    long udecoded;

    *u = UTF_INVALID;
    if (!clen)
        return 0;
    udecoded = utf8decodebyte(c[0], &len);
    if (!BETWEEN(len, 1, UTF_SIZ))
        return 1;
    for (i = 1, j = 1; i < clen && j < len; ++i, ++j) {
        udecoded = (udecoded << 6) | utf8decodebyte(c[i], &type);
        if (type)
            return j;
    }
    if (j < len)
        return 0;
    *u = udecoded;
    utf8validate(u, len);

    return len;
}
#else
static char *parse_utf(char *str, size_t clen);
#endif

Clr transcheme[3];

Drw * drw_create(Display *dpy, int screen, Window root, unsigned int w, unsigned int h, Visual *visual, unsigned int depth, Colormap cmap) {
    Drw *drw = ecalloc(1, sizeof(Drw));

    drw->dpy = dpy;
    drw->screen = screen;
    drw->root = root;
    drw->w = w;
    drw->h = h;
    drw->visual = visual;
    drw->depth = depth;
    drw->cmap = cmap;
    drw->drawable = XCreatePixmap(dpy, root, w, h, depth);
    drw->gc = XCreateGC(dpy, drw->drawable, 0, NULL);
    XSetLineAttributes(dpy, drw->gc, 1, LineSolid, CapButt, JoinMiter);

    return drw;
}

void drw_resize(Drw *drw, unsigned int w, unsigned int h) {
    if (!drw)
        return;

    drw->w = w;
    drw->h = h;
    if (drw->drawable)
        XFreePixmap(drw->dpy, drw->drawable);
    drw->drawable = XCreatePixmap(drw->dpy, drw->root, w, h, drw->depth);
}

void drw_free(Drw *drw) {
    XFreePixmap(drw->dpy, drw->drawable);
    XFreeGC(drw->dpy, drw->gc);
    drw_font_free(drw->font);
    free(drw);
}

#if USEPANGO
static Fnt * xfont_create(Drw *drw, const char *fontname) {
    Fnt *font;
    PangoFontMap *fontmap;
    PangoContext *context;
    PangoFontDescription *desc;
    PangoFontMetrics *metrics;

    if (!fontname) {
        die("spmenu: no font specified.");
    }

    font = ecalloc(1, sizeof(Fnt));
    font->dpy = drw->dpy;

    fontmap = pango_xft_get_font_map(drw->dpy, drw->screen);
    context = pango_font_map_create_context(fontmap);
    desc = pango_font_description_from_string(fontname);
    font->layout = pango_layout_new(context);
    pango_layout_set_font_description(font->layout, desc);

    metrics = pango_context_get_metrics(context, desc, pango_language_from_string ("en-us"));
    font->h = pango_font_metrics_get_height(metrics) / PANGO_SCALE;

    pango_font_metrics_unref(metrics);
    g_object_unref(context);

    return font;
}

void xfont_free(Fnt *font) {
    if (!font)
        return;
    if (font->layout)
        g_object_unref(font->layout);
    free(font);
}
#else
static Fnt * xfont_create(Drw *drw, const char *fontname, FcPattern *fontpattern) {
    Fnt *font;
    XftFont *xfont = NULL;
    FcPattern *pattern = NULL;

    if (fontname) {
        /* Using the pattern found at font->xfont->pattern does not yield the
         * same substitution results as using the pattern returned by
         * FcNameParse; using the latter results in the desired fallback
         * behaviour whereas the former just results in missing-character
         * rectangles being drawn, at least with some fonts. */
        if (!(xfont = XftFontOpenName(drw->dpy, drw->screen, fontname))) {
            fprintf(stderr, "error, cannot load font from name: '%s'\n", fontname);
            return NULL;
        }

        if (fontname[0] == '-')
            pattern = XftXlfdParse(fontname, False, False);
        else
            pattern = FcNameParse((FcChar8 *) fontname);

        if (!pattern) {
            fprintf(stderr, "error, cannot parse font name to pattern: '%s'\n", fontname);
            XftFontClose(drw->dpy, xfont);
            return NULL;
        }
    } else if (fontpattern) {
        if (!(xfont = XftFontOpenPattern(drw->dpy, fontpattern))) {
            fprintf(stderr, "spmenu: cannot load font from pattern.\n");
            return NULL;
        }
    } else {
        die("spmenu: no font specified.");
    }

    font = ecalloc(1, sizeof(Fnt));
    font->xfont = xfont;
    font->pattern = pattern;
    font->h = xfont->ascent + xfont->descent;
    font->dpy = drw->dpy;

    return font;
}

void xfont_free(Fnt *font) {
    if (!font)
        return;
    if (font->pattern)
        FcPatternDestroy(font->pattern);
    XftFontClose(font->dpy, font->xfont);
    free(font);
}
#endif

Fnt* drw_font_create(Drw* drw, char *font[], size_t fontcount) {
    if (!drw || !font)
        return NULL;

#if USEPANGO
    Fnt *fnt = NULL;

    fnt = xfont_create(drw, *font);
    return (drw->font = fnt);
#else
    Fnt *cur, *ret = NULL;
    size_t i;

    for (i = 1; i <= fontcount; i++) {
        if ((cur = xfont_create(drw, font[fontcount - i], NULL))) {
            cur->next = ret;
            ret = cur;
        }
    }
    return (drw->font = ret);
#endif
}

void drw_font_free(Fnt *font) {
    if (font) {
#if !USEPANGO
        drw_font_free(font->next);
#endif
        xfont_free(font);
    }
}

void drw_clr_create(Drw *drw, Clr *dest, char *clrname, unsigned int alpha) {
    if (!drw || !dest || !clrname)
        return;

    if (!XftColorAllocName(drw->dpy, drw->visual, drw->cmap,
                clrname, dest))
        die("spmenu: cannot allocate color '%s'", clrname);

    dest->pixel = (dest->pixel & 0x00ffffffU) | (alpha << 24);
}

/* Wrapper to create color schemes. The caller has to call free(3) on the
 * returned color scheme when done using it. */
Clr * drw_scm_create(Drw *drw, char *clrnames[], unsigned int alphas[], size_t clrcount) {
    size_t i;
    Clr *ret;

    /* need at least two colors for a scheme */
    if (!drw || !clrnames || clrcount < 2 || !(ret = ecalloc(clrcount, sizeof(XftColor))))
        return NULL;

    for (i = 0; i < clrcount; i++)
        drw_clr_create(drw, &ret[i], clrnames[i], alphas[i]);
    return ret;
}

void drw_setscheme(Drw *drw, Clr *scm) {
    if (drw)
        drw->scheme = scm;
}

void drw_settrans(Drw *drw, Clr *psc, Clr *nsc) {
    if (drw) {
        transcheme[0] = psc[ColBg]; transcheme[1] = nsc[ColBg]; transcheme[2] = psc[ColPwl];
        drw->scheme = transcheme;
    }
}

void drw_arrow(Drw *drw, int x, int y, unsigned int w, unsigned int h, int direction, int slash) {
    if (!drw || !drw->scheme)
        return;

    x = direction ? x : x + w;
    w = direction ? w : -w;
    unsigned int hh = slash ? (direction ? 0 : h) : h/2;

    XPoint points[] = {
        {x    , y      },
        {x + w, y + hh },
        {x    , y + h  },
    };

    XPoint bg[] = {
        {x    , y    },
        {x + w, y    },
        {x + w, y + h},
        {x    , y + h},
    };

    XSetForeground(drw->dpy, drw->gc, drw->scheme[ColBg].pixel);
    XFillPolygon(drw->dpy, drw->drawable, drw->gc, bg, 4, Convex, CoordModeOrigin);
    XSetForeground(drw->dpy, drw->gc, drw->scheme[ColFg].pixel);
    XFillPolygon(drw->dpy, drw->drawable, drw->gc, points, 3, Nonconvex, CoordModeOrigin);
}

void drw_rect(Drw *drw, int x, int y, unsigned int w, unsigned int h, int filled, int invert) {
    if (!drw || !drw->scheme)
        return;
    XSetForeground(drw->dpy, drw->gc, invert ? drw->scheme[ColBg].pixel : drw->scheme[ColFg].pixel);
    if (filled)
        XFillRectangle(drw->dpy, drw->drawable, drw->gc, x, y, w, h);
    else
        XDrawRectangle(drw->dpy, drw->drawable, drw->gc, x, y, w - 1, h - 1);
}

int xerrordummy(Display *dpy, XErrorEvent *ee) {
    return 0;
}

#if USEPANGO
char *parse_utf(char *str, size_t clen) {
    char *ostr = str;
    char *cnstr = calloc(clen + 1, sizeof(char));
    char *cstr = cnstr;
    size_t olen = clen;

#if USEUTF8
    iconv_t cd = iconv_open("UTF-8//IGNORE", "UTF-8");
#else
    iconv_t cd = iconv_open("UTF-8//IGNORE", "ASCII");
#endif

    if (cd == (iconv_t) - 1) {
        die("spmenu: iconv_open failed");
    }

    if (iconv(cd, &ostr, &olen, &cstr, &clen)) {
        ; // should be ok
    }

    iconv_close(cd);

    return cnstr;
}
#endif

int drw_text(Drw *drw, int x, int y, unsigned int w, unsigned int h, unsigned int lpad, const char *text, int invert, Bool markup) {
    XSetErrorHandler(xerrordummy);

#if USEPANGO // pango
    char buf[1024];
    int ty;
    unsigned int ew = 0;
    XftDraw *d = NULL;
    size_t i, len;
    int render = x || y || w || h;
    char *t;

    if (!drw || (render && !drw->scheme) || !text || !drw->font)
        return 0;

    if (!render) {
        w = ~w;
    } else {
        XSetForeground(drw->dpy, drw->gc, drw->scheme[invert ? ColFg : ColBg].pixel);
        XFillRectangle(drw->dpy, drw->drawable, drw->gc, x, y, w, h);
        d = XftDrawCreate(drw->dpy, drw->drawable, drw->visual, drw->cmap);
        x += lpad;
        w -= lpad;
    }

    len = strlen(text);
    t = strdup(text);

    t = parse_utf(t, len);

    if (len) {
        drw_font_getexts(drw->font, t, len, &ew, NULL, markup);
        /* shorten text if necessary */
        for (len = MIN(len, sizeof(buf) - 1); len && ew > w; drw_font_getexts(drw->font, t, len, &ew, NULL, markup))
            len--;

        if (len) {
            memcpy(buf, t, len);
            buf[len] = '\0';
            if (len < strlen(t))
                for (i = len; i && i > len - 3; buf[--i] = '.')
                    ; /* NOP */

            if (!strstr(buf, "</"))
                markup = 0;

            if (render) {
                ty = y + (h - drw->font->h) / 2;
                if(markup)
                    pango_layout_set_markup(drw->font->layout, buf, len);
                else
                    pango_layout_set_text(drw->font->layout, buf, len);
                pango_xft_render_layout(d, &drw->scheme[invert ? ColBg : ColFg],
                        drw->font->layout, x * PANGO_SCALE, ty * PANGO_SCALE);
                if(markup) /* clear markup attributes */
                    pango_layout_set_attributes(drw->font->layout, NULL);
            }

            x += ew;
            w -= ew;
        }
    }
    if (d)
        XftDrawDestroy(d);

    return x + (render ? w : 0);
#else // xft
    char buf[1024];
    int ty;
    unsigned int ew = 0;
    XftDraw *d = NULL;
    Fnt *usedfont, *curfont, *nextfont;
    size_t i, len;
    int utf8strlen, utf8charlen, render = x || y || w || h;
    long utf8codepoint = 0;
    const char *utf8str;
    FcCharSet *fccharset;
    FcPattern *fcpattern;
    FcPattern *match;
    XftResult result;
    int charexists = 0;

    if (!drw || (render && !drw->scheme) || !text || !drw->font)
        return 0;

    if (!render) {
        w = ~w;
    } else {
        XSetForeground(drw->dpy, drw->gc, drw->scheme[invert ? ColFg : ColBg].pixel);
        XFillRectangle(drw->dpy, drw->drawable, drw->gc, x, y, w, h);
        d = XftDrawCreate(drw->dpy, drw->drawable, drw->visual, drw->cmap);
        x += lpad;
        w -= lpad;
    }

    usedfont = drw->font;
    while (1) {
        utf8strlen = 0;
        utf8str = text;
        nextfont = NULL;
        while (*text) {
            utf8charlen = utf8decode(text, &utf8codepoint, UTF_SIZ);
            for (curfont = drw->font; curfont; curfont = curfont->next) {
                charexists = charexists || XftCharExists(drw->dpy, curfont->xfont, utf8codepoint);
                if (charexists) {
                    if (curfont == usedfont) {
                        utf8strlen += utf8charlen;
                        text += utf8charlen;
                    } else {
                        nextfont = curfont;
                    }
                    break;
                }
            }

            if (!charexists || nextfont)
                break;
            else
                charexists = 0;
        }

        if (utf8strlen) {
            drw_font_getexts(usedfont, utf8str, utf8strlen, &ew, NULL, True);
            /* shorten text if necessary */
            for (len = MIN(utf8strlen, sizeof(buf) - 1); len && ew > w; drw_font_getexts(usedfont, utf8str, len, &ew, NULL, True))
                len--;

            if (len) {
                memcpy(buf, utf8str, len);
                buf[len] = '\0';
                if (len < utf8strlen)
                    for (i = len; i && i > len - 3; buf[--i] = '.')
                        ; /* NOP */

                if (render) {
                    ty = y + (h - usedfont->h) / 2 + usedfont->xfont->ascent;
                    XftDrawStringUtf8(d, &drw->scheme[invert ? ColBg : ColFg],
                            usedfont->xfont, x, ty, (XftChar8 *)buf, len);
                }
                x += ew;
                w -= ew;
            }
        }

        if (!*text) {
            break;
        } else if (nextfont) {
            charexists = 0;
            usedfont = nextfont;
        } else {
            /* Regardless of whether or not a fallback font is found, the
             * character must be drawn. */
            charexists = 1;

            fccharset = FcCharSetCreate();
            FcCharSetAddChar(fccharset, utf8codepoint);

            if (!drw->font->pattern) {
                /* Refer to the comment in xfont_create for more information. */
                die("spmenu: the first font in the cache must be loaded from a font string.");
            }

            fcpattern = FcPatternDuplicate(drw->font->pattern);
            FcPatternAddCharSet(fcpattern, FC_CHARSET, fccharset);
            FcPatternAddBool(fcpattern, FC_SCALABLE, FcTrue);

            FcConfigSubstitute(NULL, fcpattern, FcMatchPattern);
            FcDefaultSubstitute(fcpattern);
            match = XftFontMatch(drw->dpy, drw->screen, fcpattern, &result);

            FcCharSetDestroy(fccharset);
            FcPatternDestroy(fcpattern);

            if (match) {
                usedfont = xfont_create(drw, NULL, match);
                if (usedfont && XftCharExists(drw->dpy, usedfont->xfont, utf8codepoint)) {
                    for (curfont = drw->font; curfont->next; curfont = curfont->next)
                        ; /* NOP */
                    curfont->next = usedfont;
                } else {
                    xfont_free(usedfont);
                    usedfont = drw->font;
                }
            }
        }
    }
    if (d)
        XftDrawDestroy(d);

    XSync(drw->dpy, False);

    return x + (render ? w : 0);
#endif

    XSync(drw->dpy, False);
}

void drw_map(Drw *drw, Window win, int x, int y, unsigned int w, unsigned int h) {
    if (!drw)
        return;

    XCopyArea(drw->dpy, drw->drawable, win, drw->gc, x, y, w, h, x, y);
    XSync(drw->dpy, False);
}

unsigned int drw_font_getwidth(Drw *drw, const char *text, Bool markup) {
    if (!drw || !drw->font || !text)
        return 0;
    return drw_text(drw, 0, 0, 0, 0, 0, text, 0, markup);
}

void drw_font_getexts(Fnt *font, const char *text, unsigned int len, unsigned int *w, unsigned int *h, Bool markup) {
#if !USEPANGO
    XGlyphInfo ext;
#endif

    if (!font || !text)
        return;

    char *t = strdup(text);

#if USEPANGO
    t = parse_utf(t, len);

    if (!strstr(t, "</"))
        markup = 0;
    PangoRectangle r;
    if(markup)
        pango_layout_set_markup(font->layout, t, len);
    else
        pango_layout_set_text(font->layout, t, len);
    pango_layout_get_extents(font->layout, 0, &r);
    if(markup) // clear markup attributes
        pango_layout_set_attributes(font->layout, NULL);
#else
    XftTextExtentsUtf8(font->dpy, font->xfont, (XftChar8 *)t, len, &ext);
#endif
    if (w)
#if USEPANGO
        *w = r.width / PANGO_SCALE;
#else
    *w = ext.xOff;
#endif
    if (h)
        *h = font->h;
}

Cur * drw_cur_create(Drw *drw, int shape) {
    Cur *cur;

    if (!drw || !(cur = ecalloc(1, sizeof(Cur))))
        return NULL;

    cur->cursor = XCreateFontCursor(drw->dpy, shape);

    return cur;
}

void drw_cur_free(Drw *drw, Cur *cursor) {
    if (!cursor)
        return;

    XFreeCursor(drw->dpy, cursor->cursor);
    free(cursor);
}

unsigned int drw_fontset_getwidth_clamp(Drw *drw, const char *text, unsigned int n, Bool markup) {
    unsigned int tmp = 0;
    if (drw && drw->font && text && n)
        tmp = drw_text(drw, 0, 0, 0, 0, 0, text, n, markup);
    return MIN(n, tmp);
}