/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cairo/cairo.h>
#include <pango/pango.h>
#include <pango/pangocairo.h>
#include <math.h>

#include "draw.h"
#include "../main.h"

#ifndef X11
#define USEX 0
#else
#define USEX 1
#endif

#ifndef WAYLAND
#define USEWAYLAND 0
#else
#define USEWAYLAND 1
#endif

#if USEX
#include <X11/Xlib.h>
#endif

void cairo_set_source_hex(cairo_t* cr, const char *col, int alpha) {
    unsigned int hex;

    sscanf(col, "#%x", &hex);

    double r = ((hex >> 16) & 0xFF) / 255.0;
    double g = ((hex >> 8) & 0xFF) / 255.0;
    double b = (hex & 0xFF) / 255.0;

    cairo_set_source_rgba(cr, r, g, b, alpha / 255.0);
}

#if USEX
Draw_t *draw_create_x11(Display *dpy, int screen, Window root, unsigned int w, unsigned int h, Visual *visual, unsigned int depth, Colormap cmap, int protocol) {
    Draw_t *draw = ecalloc(1, sizeof(Draw_t));

    draw->protocol = protocol;
    draw->dpy = dpy;
    draw->screen = screen;
    draw->root = root;
    draw->w = w;
    draw->h = h;
    draw->visual = visual;
    draw->depth = depth;
    draw->cmap = cmap;
    draw->drawable = XCreatePixmap(dpy, root, w, h, depth);
    draw->gc = XCreateGC(dpy, draw->drawable, 0, NULL);
    XSetLineAttributes(dpy, draw->gc, 1, LineSolid, CapButt, JoinMiter);

    return draw;
}
#endif

#if USEWAYLAND
Draw_t *draw_create_wl(int protocol) {
    Draw_t *draw = ecalloc(1, sizeof(Draw_t));

    draw->protocol = protocol;

    return draw;
}

void draw_create_surface_wl(Draw_t *draw, void *data, int32_t w, int32_t h) {
    draw->data = data;
    draw->w = w;
    draw->h = h;
    draw->surface = cairo_image_surface_create_for_data(draw->data, CAIRO_FORMAT_ARGB32, draw->w, draw->h, draw->w * 4);
    draw->d = cairo_create(draw->surface);
}
#endif

void draw_resize(Draw_t *draw, unsigned int w, unsigned int h) {
    if (!draw)
        return;

    draw->w = w;
    draw->h = h;

#if USEX
    if (draw->drawable)
        XFreePixmap(draw->dpy, draw->drawable);

    draw->drawable = XCreatePixmap(draw->dpy, draw->root, w, h, draw->depth);
#endif
}

void draw_free(Draw_t *draw) {
#if USEX
    if (!draw->protocol) {
        XFreePixmap(draw->dpy, draw->drawable);
        XFreeGC(draw->dpy, draw->gc);
    }
#endif

    draw_font_free(draw->font);
    free(draw);
}

static Fnt *font_create(Draw_t *draw, const char *fontname) {
    Fnt *font;
    PangoFontMap *fontmap;
    PangoContext *context;
    PangoFontDescription *desc;
    PangoFontMetrics *metrics;

    if (!fontname) {
        die("spmenu: no font specified.");
    }

    font = ecalloc(1, sizeof(Fnt));
    font->dpy = draw->dpy;

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

Fnt* draw_font_create(Draw_t* draw, char *font[], size_t fontcount) {
    if (!draw || !font)
        return NULL;

    Fnt *fnt = NULL;

    fnt = font_create(draw, *font);

    return (draw->font = fnt);
}

void draw_font_free(Fnt *font) {
    if (font) {
        font_free(font);
    }
}

void draw_arrow(Draw_t *draw, int x, int y, unsigned int w, unsigned int h, int direction, int slash, char *prevcol, char *nextcol, int prevalpha, int nextalpha) {
    if (!draw)
        return;

    x = direction ? x : x + w;
    w = direction ? w : - w;

    double hh = slash ? (direction ? 0 : h) : h / 2;

    cairo_surface_t *sf = NULL;

    if (draw->protocol) {
        sf = cairo_image_surface_create_for_data(draw->data, CAIRO_FORMAT_ARGB32, draw->w, draw->h, draw->w * 4);
    } else {
        sf = cairo_xlib_surface_create(draw->dpy, draw->drawable, draw->visual, draw->w, draw->h);
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

void draw_circle(Draw_t *draw, int x, int y, unsigned int w, unsigned int h, int direction, char *prevcol, char *nextcol, int prevalpha, int nextalpha) {
    if (!draw)
        return;

    cairo_surface_t *sf = NULL;

    if (draw->protocol) {
        sf = cairo_image_surface_create_for_data(draw->data, CAIRO_FORMAT_ARGB32, draw->w, draw->h, draw->w * 4);
    } else {
        sf = cairo_xlib_surface_create(draw->dpy, draw->drawable, draw->visual, draw->w, draw->h);
    }

    cairo_t *cr = cairo_create(sf);

    cairo_set_source_hex(cr, prevcol, prevalpha);
    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);

    // draw rectangle
    cairo_rectangle(cr, x, y, w, h);
    cairo_fill(cr);

    double cx = direction ? x + w - h / 2.0 : x + h / 2.0;
    double cy = y + h / 2;
    double rad = h / 2.0;

    double start = direction ? -M_PI_2 : M_PI_2;
    double end = direction ? M_PI_2 : 3 * M_PI_2;

    // draw circle
    cairo_arc(cr, cx, cy, rad, start, end);
    cairo_close_path(cr);

    cairo_set_source_hex(cr, nextcol, nextalpha);
    cairo_fill(cr);

    cairo_destroy(cr);
    cairo_surface_destroy(sf);
}

void draw_rect(Draw_t *draw, int x, int y, unsigned int w, unsigned int h, int filled, int invert, char *fgcol, char *bgcol, int fgalpha, int bgalpha) {
    if (!draw) {
        return;
    }

    cairo_surface_t *sf;

    if (draw->protocol) {
        sf = cairo_image_surface_create_for_data(draw->data, CAIRO_FORMAT_ARGB32, draw->w, draw->h, draw->w * 4);
    } else {
        sf = cairo_xlib_surface_create(draw->dpy, draw->drawable, draw->visual, draw->w, draw->h);
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

int draw_text(Draw_t *draw, int x, int y, unsigned int w, unsigned int h, unsigned int lpad, const char *text, int invert, Bool markup, char *fgcol, char *bgcol, int fgalpha, int bgalpha) {
    char buf[1024];
    unsigned int ew = 0;

    size_t i, len;
    int render = x || y || w || h;
    char *t;

    if (!draw || !text || !draw->font) {
        return 0;
    }

    if (!render) {
        w = ~w;
    } else {
        x += lpad;
        w -= lpad;

        if (draw->protocol) {
            draw->surface = cairo_image_surface_create_for_data(draw->data, CAIRO_FORMAT_ARGB32, draw->w, draw->h, draw->w * 4);
        } else {
            draw->surface = cairo_xlib_surface_create(draw->dpy, draw->drawable, draw->visual, draw->w, draw->h);
        }
        draw->d = cairo_create(draw->surface);

        // draw bg
        cairo_set_source_hex(draw->d, invert ? fgcol : bgcol, invert ? fgalpha : bgalpha);
        cairo_set_operator(draw->d, CAIRO_OPERATOR_SOURCE);
        cairo_rectangle(draw->d, x - lpad, y, w + lpad, h);
        cairo_fill(draw->d);
    }

    t = strdup(text);
    len = strlen(t);

    if (len) {
        draw_font_getexts(draw->font, t, len, &ew, NULL, markup);

        // shorten text if necessary
        for (len = MIN(len, sizeof(buf) - 1); len && ew > w; draw_font_getexts(draw->font, t, len, &ew, NULL, markup))
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
                    pango_layout_set_markup(draw->font->layout, buf, len);
                } else {
                    pango_layout_set_text(draw->font->layout, buf, len);
                }

                pango_layout_set_single_paragraph_mode(draw->font->layout, True);

                // draw fg
                cairo_set_source_hex(draw->d, fgcol, fgalpha);
                cairo_move_to(draw->d, x, y + (h - draw->font->h) / 2);

                // update and show layout
                pango_cairo_update_layout(draw->d, draw->font->layout);
                pango_cairo_show_layout(draw->d, draw->font->layout);

                cairo_set_operator(draw->d, CAIRO_OPERATOR_SOURCE);

                if (markup) // clear markup attributes
                    pango_layout_set_attributes(draw->font->layout, NULL);
            }

            x += ew;
            w -= ew;
        }
    }

    return x + (render ? w : 0);
}

void draw_map(Draw_t *draw, Window win, int x, int y, unsigned int w, unsigned int h) {
    if (!draw)
        return;

#if USEX
    if (!draw->protocol) {
        XCopyArea(draw->dpy, draw->drawable, win, draw->gc, x, y, w, h, x, y);
        XSync(draw->dpy, False);
    }
#endif
}

unsigned int draw_font_getwidth(Draw_t *draw, const char *text, Bool markup) {
    if (!draw || !draw->font || !text)
        return 0;
    return draw_text(draw, 0, 0, 0, 0, 0, text, 0, markup, "#000000", "#000000", 255, 255);
}

void draw_font_getexts(Fnt *font, const char *text, unsigned int len, unsigned int *w, unsigned int *h, Bool markup) {
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

void draw_set_img(Draw_t *draw, void *data, int w, int h) {
    if (!w || !h || !draw) {
        return;
    }

    draw->img_data = data;
    draw->img_surface = cairo_image_surface_create_for_data(draw->img_data, CAIRO_FORMAT_ARGB32, w, h, w * 4);
}

void draw_img(Draw_t *draw, int x, int y) {
    if (!draw) {
        return;
    }

    cairo_set_operator(draw->d, CAIRO_OPERATOR_OVER);

    cairo_set_source_surface(draw->d, draw->img_surface, x, y);
    cairo_mask_surface(draw->d, draw->img_surface, x, y);

    cairo_set_source_surface(draw->d, draw->surface, draw->w, draw->h);
}

unsigned int draw_fontset_getwidth_clamp(Draw_t *draw, const char *text, unsigned int n, Bool markup) {
    unsigned int tmp = 0;
    if (draw && draw->font && text && n)
        tmp = draw_text(draw, 0, 0, 0, 0, 0, text, n, markup, "#000000", "#000000", 255, 255);
    return MIN(n, tmp);
}
