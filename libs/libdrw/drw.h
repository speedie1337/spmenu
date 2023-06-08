/* See LICENSE file for copyright and license details. */

#include <cairo/cairo.h>
#include <cairo/cairo-ft.h>
#include <cairo/cairo-xlib.h>
#include <pango/pango.h>
#include <pango/pangocairo.h>

typedef struct {
    Cursor cursor;
} Cur;

typedef struct Fnt {
    Display *dpy;
    unsigned int h;
    PangoLayout *layout;
} Fnt;

typedef struct {
    unsigned int w, h;
    int protocol;
    Display *dpy;
    int screen;
    Window root;
    Visual *visual;
    unsigned int depth;
    void *img_data;
    void *data;
    Colormap cmap;
    Drawable drawable;
    GC gc;
    Fnt *font;
    cairo_surface_t *surface;
    cairo_surface_t *img_surface;
    cairo_t *d;
    cairo_t *img_d;
} Drw;

/* Cairo color convertion */
void cairo_set_source_hex(cairo_t* cr, const char *col, int alpha);

/* Cairo image drawing */
void drw_img(Drw *drw, int x, int y);
void drw_set_img(Drw *drw, void *data, int w, int h);

/* Drawable abstraction */
Drw *drw_create_x11(Display *dpy, int screen, Window win, unsigned int w, unsigned int h, Visual *visual, unsigned int depth, Colormap cmap, int protocol);
Drw *drw_create_wl(int protocol);
void drw_create_surface_wl(Drw *drw, void *data, int32_t w, int32_t h);

void drw_resize(Drw *drw, unsigned int w, unsigned int h);
void drw_free(Drw *drw);

/* Fnt abstraction */
Fnt *drw_font_create(Drw* drw, char *font[], size_t fontcount);
void drw_font_free(Fnt* set);
unsigned int drw_fontset_getwidth_clamp(Drw *drw, const char *text, unsigned int n, Bool markup);
unsigned int drw_font_getwidth(Drw *drw, const char *text, Bool markup);
void drw_font_getexts(Fnt *font, const char *text, unsigned int len, unsigned int *w, unsigned int *h, Bool markup);

/* Cursor abstraction */
Cur *drw_cur_create(Drw *drw, int shape);
void drw_cur_free(Drw *drw, Cur *cursor);

/* Drawing functions */
void drw_rect(Drw *drw, int x, int y, unsigned int w, unsigned int h, int filled, int invert, char *fgcol, char *bgcol, int fgalpha, int bgalpha);
int drw_text(Drw *drw, int x, int y, unsigned int w, unsigned int h, unsigned int lpad, const char *text, int invert, Bool markup, char *fgcol, char *bgcol, int fgalpha, int bgalpha);

/* Map functions */
void drw_map(Drw *drw, Window win, int x, int y, unsigned int w, unsigned int h);

/* Powerline functions */
void drw_arrow(Drw *drw, int x, int y, unsigned int w, unsigned int h, int direction, int slash, char *prevcol, char *nextcol, int prevalpha, int nextalpha);
void drw_circle(Drw *drw, int x, int y, unsigned int w, unsigned int h, int direction, char *prevcol, char *nextcol, int prevalpha, int nextalpha);
