/* See LICENSE file for copyright and license details. */

#include <cairo/cairo.h>
#include <cairo/cairo-ft.h>
#include <cairo/cairo-xlib.h>
#include <pango/pango.h>
#include <pango/pangocairo.h>

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
} Draw_t;

/* Cairo color convertion */
void draw_cairo_set_source_hex(cairo_t* cr, const char *col, int alpha);

/* Cairo image drawing */
void draw_img(Draw_t *draw, int x, int y);
void draw_set_img(Draw_t *draw, void *data, int w, int h);

/* Drawable abstraction */
Draw_t *draw_create_x11(Display *dpy, int screen, Window win, unsigned int w, unsigned int h, Visual *visual, unsigned int depth, Colormap cmap, int protocol);
Draw_t *draw_create_wl(int protocol);
void draw_create_surface_wl(Draw_t *draw, void *data, int32_t w, int32_t h);

void draw_resize(Draw_t *draw, unsigned int w, unsigned int h);
void draw_free(Draw_t *draw);

/* Fnt abstraction */
Fnt *draw_font_create(Draw_t* draw, char *font[], size_t fontcount);
void draw_font_free(Fnt* set);
unsigned int draw_fontset_getwidth_clamp(Draw_t *draw, const char *text, unsigned int n, Bool markup);
unsigned int draw_font_getwidth(Draw_t *draw, const char *text, Bool markup);
void draw_font_getexts(Fnt *font, const char *text, unsigned int len, unsigned int *w, unsigned int *h, Bool markup);

/* Drawing functions */
void draw_rect(Draw_t *draw, int x, int y, unsigned int w, unsigned int h, int filled, int invert, char *fgcol, char *bgcol, int fgalpha, int bgalpha);
int draw_text(Draw_t *draw, int x, int y, unsigned int w, unsigned int h, unsigned int lpad, const char *text, int invert, Bool markup, char *fgcol, char *bgcol, int fgalpha, int bgalpha);

/* Map functions */
void draw_map(Draw_t *draw, Window win, int x, int y, unsigned int w, unsigned int h);

/* Powerline functions */
void draw_arrow(Draw_t *draw, int x, int y, unsigned int w, unsigned int h, int direction, int slash, char *prevcol, char *nextcol, int prevalpha, int nextalpha);
void draw_circle(Draw_t *draw, int x, int y, unsigned int w, unsigned int h, int direction, char *prevcol, char *nextcol, int prevalpha, int nextalpha);

/* Screenshot functions */
void draw_save_screen(Draw_t *draw, const char *file);

/* Misc */
void draw_die(const char *fmt, ...);
void *draw_calloc(size_t nmemb, size_t size);
