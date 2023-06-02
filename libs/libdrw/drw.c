/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <cairo/cairo.h>
#include <pango/pango.h>
#include <pango/pangocairo.h>
#include <iconv.h>

#include "drw.h"
#include "../sl/main.h"

void cairo_set_source_hex(cairo_t* cr, const char *col, int alpha) {
    unsigned int hex;

    sscanf(col, "#%x", &hex);

    double r = ((hex >> 16) & 0xFF) / 255.0;
    double g = ((hex >> 8) & 0xFF) / 255.0;
    double b = (hex & 0xFF) / 255.0;

    cairo_set_source_rgba(cr, r, g, b, alpha / 255.0);
}

Drw *drw_create_x11(Display *dpy, int screen, Window root, unsigned int w, unsigned int h, Visual *visual, unsigned int depth, Colormap cmap, int protocol) {
    Drw *drw = ecalloc(1, sizeof(Drw));

    drw->protocol = protocol;
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

Drw *drw_create_wl(int protocol) {
    Drw *drw = ecalloc(1, sizeof(Drw));

    drw->protocol = protocol;

    return drw;
}

void drw_create_surface_wl(Drw *drw, void *data, int32_t w, int32_t h) {
    drw->data = data;
    drw->w = w;
    drw->h = h;
    drw->surface = cairo_image_surface_create_for_data(drw->data, CAIRO_FORMAT_ARGB32, drw->w, drw->h, drw->w * 4);
    drw->d = cairo_create(drw->surface);
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

    if (!drw->protocol) {
        XFreePixmap(drw->dpy, drw->drawable);
        XFreeGC(drw->dpy, drw->gc);
    }

    drw_font_free(drw->font);
    free(drw);
}

static Fnt *font_create(Drw *drw, const char *fontname) {
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

void font_free(Fnt *font) {
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

    fnt = font_create(drw, *font);

    return (drw->font = fnt);
}

void drw_font_free(Fnt *font) {
    if (font) {
        font_free(font);
    }
}

void drw_arrow(Drw *drw, int x, int y, unsigned int w, unsigned int h, int direction, int slash, char *prevcol, char *nextcol, int prevalpha, int nextalpha) {
    if (!drw)
        return;

    x = direction ? x : x + w;
    w = direction ? w : - w;

    double hh = slash ? (direction ? 0 : h) : h / 2;

    cairo_surface_t *sf = NULL;

    if (drw->protocol) {
        sf = cairo_image_surface_create_for_data(drw->data, CAIRO_FORMAT_ARGB32, drw->w, drw->h, drw->w * 4);
    } else {
        sf = cairo_xlib_surface_create(drw->dpy, drw->drawable, drw->visual, drw->w, drw->h);
    }
    cairo_t *cr = cairo_create(sf);

    cairo_set_source_hex(cr, prevcol, prevalpha);
    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);

    cairo_rectangle(cr, x, y, w, h);
    cairo_fill(cr);

    cairo_move_to(cr, x, y);
    cairo_line_to(cr, x + w, y + hh);
    cairo_line_to(cr, x, y + h);
    cairo_close_path(cr);

    cairo_set_source_hex(cr, nextcol, nextalpha);
    cairo_fill(cr);

    cairo_destroy(cr);
    cairo_surface_destroy(sf);
}

void drw_rect(Drw *drw, int x, int y, unsigned int w, unsigned int h, int filled, int invert, char *fgcol, char *bgcol, int fgalpha, int bgalpha) {
    if (!drw) {
        return;
    }

    cairo_surface_t *sf;

    if (drw->protocol) {
        sf = cairo_image_surface_create_for_data(drw->data, CAIRO_FORMAT_ARGB32, drw->w, drw->h, drw->w * 4);
    } else {
        sf = cairo_xlib_surface_create(drw->dpy, drw->drawable, drw->visual, drw->w, drw->h);
    }
    cairo_t *cr = cairo_create(sf);

    if (!cr) {
        die("failed to create cairo context");
    }

    cairo_set_source_hex(cr, invert ? bgcol : fgcol, invert ? bgalpha : fgalpha);
    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);

    if (filled) {
        cairo_rectangle(cr, x, y, w, h);
        cairo_fill(cr);
    } else {
        cairo_rectangle(cr, x, y, w - 1, h - 1);
        cairo_stroke(cr);
    }

    cairo_destroy(cr);
    cairo_surface_destroy(sf);
}

int drw_text(Drw *drw, int x, int y, unsigned int w, unsigned int h, unsigned int lpad, const char *text, int invert, Bool markup, char *fgcol, char *bgcol, int fgalpha, int bgalpha) {
    char buf[1024];
    unsigned int ew = 0;

    size_t i, len;
    int render = x || y || w || h;
    char *t;

    if (!drw || !text || !drw->font) {
        return 0;
    }

    if (!render) {
        w = ~w;
    } else {
        x += lpad;
        w -= lpad;

        if (drw->protocol) {
            drw->surface = cairo_image_surface_create_for_data(drw->data, CAIRO_FORMAT_ARGB32, drw->w, drw->h, drw->w * 4);
        } else {
            drw->surface = cairo_xlib_surface_create(drw->dpy, drw->drawable, drw->visual, drw->w, drw->h);
        }
        drw->d = cairo_create(drw->surface);

        // draw bg
        cairo_set_source_hex(drw->d, invert ? fgcol : bgcol, invert ? fgalpha : bgalpha);
        cairo_set_operator(drw->d, CAIRO_OPERATOR_SOURCE);
        cairo_rectangle(drw->d, x - lpad, y, w + lpad, h);
        cairo_fill(drw->d);
    }

    t = strdup(text);
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
                if (markup) {
                    pango_layout_set_markup(drw->font->layout, buf, len);
                } else {
                    pango_layout_set_text(drw->font->layout, buf, len);
                }

                pango_layout_set_single_paragraph_mode(drw->font->layout, True);

                // draw fg
                cairo_set_source_hex(drw->d, fgcol, fgalpha);
                cairo_move_to(drw->d, x, y + (h - drw->font->h) / 2);

                // update and show layout
                pango_cairo_update_layout(drw->d, drw->font->layout);
                pango_cairo_show_layout(drw->d, drw->font->layout);

                cairo_set_operator(drw->d, CAIRO_OPERATOR_SOURCE);

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

    if (!drw->protocol) {
        XCopyArea(drw->dpy, drw->drawable, win, drw->gc, x, y, w, h, x, y);
        XSync(drw->dpy, False);
    }
}

unsigned int drw_font_getwidth(Drw *drw, const char *text, Bool markup) {
    if (!drw || !drw->font || !text)
        return 0;
    return drw_text(drw, 0, 0, 0, 0, 0, text, 0, markup, "#000000", "#000000", 255, 255);
}

void drw_font_getexts(Fnt *font, const char *text, unsigned int len, unsigned int *w, unsigned int *h, Bool markup) {
    if (!font || !text)
        return;

    char *t = strdup(text);

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
        tmp = drw_text(drw, 0, 0, 0, 0, 0, text, n, markup, "#000000", "#000000", 255, 255);
    return MIN(n, tmp);
}
