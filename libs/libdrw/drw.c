/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <cairo/cairo.h>
#include <cairo/cairo-ft.h>
#include <cairo/cairo-xlib.h>
#include <pango/pango.h>
#include <pango/pangocairo.h>
#include <iconv.h>

#include "drw.h"
#include "../sl/main.h"

Clr transcheme[3]; // transition colorscheme

Drw *drw_create(Display *dpy, int screen, Window root, unsigned int w, unsigned int h, Visual *visual, unsigned int depth, Colormap cmap) {
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

static Fnt *xfont_create(Drw *drw, const char *fontname) {
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

    fontmap = pango_cairo_font_map_new();
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

Fnt* drw_font_create(Drw* drw, char *font[], size_t fontcount) {
    if (!drw || !font)
        return NULL;

    Fnt *fnt = NULL;

    fnt = xfont_create(drw, *font);

    return (drw->font = fnt);
}

void drw_font_free(Fnt *font) {
    if (font) {
        xfont_free(font);
    }
}

void drw_clr_create(Drw *drw, Clr *dest, char *clrname, unsigned int alpha) {
    XColor color;

    if (!drw || !dest || !clrname)
        return;

    if (!XAllocNamedColor(drw->dpy, drw->cmap, clrname, &color, dest))
        die("spmenu: cannot allocate color '%s'", clrname);

    dest->red = color.red;
    dest->green = color.green;
    dest->blue = color.blue;

    dest->pixel = (dest->pixel & 0x00ffffffU) | (alpha << 24);
}

/* Wrapper to create color schemes. The caller has to call free(3) on the
 * returned color scheme when done using it. */
Clr * drw_scm_create(Drw *drw, char *clrnames[], unsigned int alphas[], size_t clrcount) {
    size_t i;
    Clr *ret;

    /* need at least two colors for a scheme */
    if (!drw || !clrnames || clrcount < 2 || !(ret = ecalloc(clrcount, sizeof(XColor))))
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

int drw_text(Drw *drw, int x, int y, unsigned int w, unsigned int h, unsigned int lpad, const char *text, int invert, Bool markup) {
    char buf[1024];
    int ty;
    unsigned int ew = 0;

    size_t i, len;
    int render = x || y || w || h;
    char *t;

    XSetErrorHandler(xerrordummy);

    if (!drw || (render && !drw->scheme) || !text || !drw->font)
        return 0;

    if (!render) {
        w = ~w;
    } else {
        XSetForeground(drw->dpy, drw->gc, drw->scheme[invert ? ColFg : ColBg].pixel);
        XFillRectangle(drw->dpy, drw->drawable, drw->gc, x, y, w, h);

        drw->surface = cairo_xlib_surface_create(drw->dpy, drw->drawable, drw->visual, drw->w, drw->h);
        drw->d = cairo_create(drw->surface);

        x += lpad;
        w -= lpad;
    }

    t = parse_utf(strdup(text), strlen(text));
    len = strlen(t);

    if (len) {
        drw_font_getexts(drw->font, t, len, &ew, NULL, markup);

        // shorten text if necessary
        for (len = MIN(len, sizeof(buf) - 1); len && ew > w; drw_font_getexts(drw->font, t, len, &ew, NULL, markup))
            len--;

        if (len) {
            memcpy(buf, t, len);
            buf[len] = '\0';
            if (len < strlen(t))
                for (i = len; i && i > len - 3; buf[--i] = '.')
                    ; // NOP

            if (!strstr(buf, "</")) // must contain </
                markup = 0;

            if (render) {
                ty = y + (h - drw->font->h) / 2;

                if (markup) {
                    pango_layout_set_markup(drw->font->layout, buf, len);
                } else {
                    pango_layout_set_text(drw->font->layout, buf, len);
                }

                pango_layout_set_single_paragraph_mode(drw->font->layout, True);

                cairo_set_source_rgb(drw->d, convert_color(drw->scheme->red), convert_color(drw->scheme->green), convert_color(drw->scheme->blue));
                cairo_move_to(drw->d, x, ty);

                pango_cairo_update_layout(drw->d, drw->font->layout);
                pango_cairo_show_layout(drw->d, drw->font->layout);

                if (markup) // clear markup attributes
                    pango_layout_set_attributes(drw->font->layout, NULL);
            }

            x += ew;
            w -= ew;
        }
    }

    return x + (render ? w : 0);
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
    if (!font || !text)
        return;

    char *t = strdup(text);

    t = parse_utf(t, len);

    if (!strstr(t, "</"))
        markup = 0;

    PangoRectangle r;

    if (markup)
        pango_layout_set_markup(font->layout, t, len);
    else
        pango_layout_set_text(font->layout, t, len);

    pango_layout_get_extents(font->layout, 0, &r);

    if (markup) // clear markup attributes
        pango_layout_set_attributes(font->layout, NULL);

    if (w)
        *w = r.width / PANGO_SCALE;

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
