/* See LICENSE file for copyright and license details. */

#include <cairo/cairo.h>
#include <cairo/cairo-ft.h>
#include <cairo/cairo-xlib.h>
#include <pango/pango.h>
#include <pango/pangocairo.h>

#define convert_color(x) (double)((x) / 65535.0)

typedef struct {
    Cursor cursor;
} Cur;

typedef XColor Clr;

typedef struct Fnt {
    Display *dpy;
    unsigned int h;
    PangoLayout *layout;
} Fnt;

enum { ColFg, ColBg, ColPwl }; /* Clr scheme index */

typedef struct {
    unsigned int w, h;
    Display *dpy;
    int screen;
    Window root;
    Visual *visual;
    unsigned int depth;
    Colormap cmap;
    Drawable drawable;
    GC gc;
    Clr *scheme;
    Fnt *font;
    cairo_surface_t *surface;
    cairo_t *d;
} Drw;

/* Drawable abstraction */
Drw *drw_create(Display *dpy, int screen, Window win, unsigned int w, unsigned int h, Visual *visual, unsigned int depth, Colormap cmap);
void drw_resize(Drw *drw, unsigned int w, unsigned int h);
void drw_free(Drw *drw);

/* Fnt abstraction */
Fnt *drw_font_create(Drw* drw, char *font[], size_t fontcount);
void drw_font_free(Fnt* set);
unsigned int drw_fontset_getwidth_clamp(Drw *drw, const char *text, unsigned int n, Bool markup);
unsigned int drw_font_getwidth(Drw *drw, const char *text, Bool markup);
void drw_font_getexts(Fnt *font, const char *text, unsigned int len, unsigned int *w, unsigned int *h, Bool markup);

/* Colorscheme abstraction */
void drw_clr_create(Drw *drw, Clr *dest, char *clrname, unsigned int alpha);
Clr *drw_scm_create(Drw *drw, char *clrnames[], unsigned int alphas[], size_t clrcount);

/* Cursor abstraction */
Cur *drw_cur_create(Drw *drw, int shape);
void drw_cur_free(Drw *drw, Cur *cursor);

/* Drawing context manipulation */
void drw_setscheme(Drw *drw, Clr *scm);

/* Drawing functions */
void drw_rect(Drw *drw, int x, int y, unsigned int w, unsigned int h, int filled, int invert);
int drw_text(Drw *drw, int x, int y, unsigned int w, unsigned int h, unsigned int lpad, const char *text, int invert, Bool markup);

/* Map functions */
void drw_map(Drw *drw, Window win, int x, int y, unsigned int w, unsigned int h);

/* Powerline functions */
void drw_settrans(Drw *drw, Clr *psc, Clr *nsc);
void drw_arrow(Drw* drw, int x, int y, unsigned int w, unsigned int h, int direction, int slash);
