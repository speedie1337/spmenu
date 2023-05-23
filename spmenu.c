/* spmenu - fancy dynamic menu
 *
 * If you're looking for functions used to draw text, see 'libs/draw.c'
 * If you're looking for wrapper functions used inside the draw functions, see 'libs/libdrw/drw.c'
 * If you're looking for functions used to draw images, see 'libs/img.c'
 * If you're looking for the .Xresources array, see 'libs/x11/xresources.h'
 *
 * You don't need to edit spmenu.c if you aren't making big changes to the software.
 *
 * After making changes, run `./build.sh` to attempt to build the software.
 * `scripts/make/generate-docs.sh` will generate a man page from 'docs/docs.md', which is a Markdown document. Run this before commiting.
 *
 * See LICENSE file for copyright and license details.
 */
#include <ctype.h>
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <unistd.h>

// set version
#ifndef VERSION
#define VERSION "unknown"
#endif

// check if we should enable right to left language support
#ifndef RTL
#define USERTL 0
#else
#define USERTL 1
#endif

// check if we should enable pango or use xft
#ifndef PANGO
#define USEPANGO 0
#else
#define USEPANGO 1
#endif

// check if we should enable image support
#ifndef IMAGE
#define USEIMAGE 0
#else
#define USEIMAGE 1
#endif

// check if we should enable multimonitor support using libXinerama
#ifndef XINERAMA
#define USEXINERAMA 0
#else
#define USEXINERAMA 1
#endif

// check if we should enable config file support using libconfig
#ifndef CONFIG
#define USECONFIG 0
#else
#define USECONFIG 1
#endif

// check if we should enable .Xresources support
#ifndef XRESOURCES
#define USEXRESOURCES 0
#else
#define USEXRESOURCES 1
#endif

// include fribidi used for right to left language support
#if USERTL
#include <fribidi.h>
#endif

// include libraries used for image support
#if USEIMAGE
#include <errno.h>
#include <pwd.h>
#include <Imlib2.h>
#include <openssl/md5.h>
#endif

// include macros and other defines
#include "libs/define.c"

// mode
static char modetext[16]; // default mode text
static int curMode; // 0 is command mode
static int allowkeys; // whether or not to interpret a keypress as an insertion

// various headers
#include "libs/libdrw/drw.h"
#include "libs/sl/main.h"
#include "libs/draw.h"
#include "libs/stream.h"
#include "libs/schemes.h"
#include "libs/arg.h"
#include "libs/x11/inc.h" // include x11
#include "libs/sort.h"
#include "libs/history.h"

// text
static char text[BUFSIZ] = "";
static char numbers[NUMBERSBUFSIZE] = "";

// keybinds
static int numlockmask = 0;
static int capslockstate = 0;

static int bh, mw, mh; // height of each item, menu width, menu height
static int reallines = 0; // temporary integer which holds lines
static int inputw = 0; // input width
static int promptw; // prompt width
static int plw = 0; // powerline width
static int lrpad; // sum of left and right padding
static int vp; // vertical padding for bar
static int sp; // side padding for bar
static int cursorstate = 1; // cursor state
static int itemnumber = 0; // item number
static size_t cursor;
static struct item *items = NULL, *backup_items, *list_items;
static struct item *matches, *matchend; // matches, final match
static struct item *prev, *curr, *next, *sel; // previous, current, next, selected
static int hplength = 0; // high priority
static char **hpitems = NULL; // high priority
static int *sel_index = NULL;
static unsigned int sel_size = 0;
static int itemn = 0; // item number

static char *embed; // X11 embed
static int screen; // screen

// item struct
struct item {
    char *text;
    char *clntext;
#if USEIMAGE
    char *image;
#endif
    char *ex;
    struct item *left, *right;
    int hp;
    int index;
    double distance;
};

// image globals
#if USEIMAGE
static int flip = 0;
static int rotation = 0;
static int needredraw = 1;
static int longestedge = 0;
static int imagew = 0;
static int imageh = 0;
static int imageg = 0;
static int ow = 0;
static int oh = 0;
#endif
static int fullscreen = 0;

// config file
#if USECONFIG
static char *argconf = NULL; // arg config path
static char *argtheme = NULL; // arg theme path
static char *argbinds = NULL; // arg binds path
#endif

// set an integer to 1 if we have rtl enabled, this saves a lot of lines and duplicate code
#if USERTL
static int isrtl = 1;
#else
static int isrtl = 0;
#endif

static int ignoreconfkeys = 0; // can be set globally if you don't want to override keybinds with config file keys
static int ignoreglobalkeys = 0; // should be set in the config file, if 1, the Keys keys array is ignored
static int ignoreconfmouse = 0; // same for mouse
static int ignoreglobalmouse = 0; // same for mouse

// colors
static int useargb = 0;
static Visual *visual;
static int depth;
static Colormap cmap;
static Drw *drw;
static Clr **scheme;
static Clr textclrs[256];

// declare functions
static int is_selected(size_t index);
static void calcoffsets(void);
static void recalculatenumbers(void);
static void insert(const char *str, ssize_t n);
static void cleanup(void);
static void navigatehistfile(int dir);
static void grabfocus(void);
static void pastesel(void);
static void appenditem(struct item *item, struct item **list, struct item **last);
static void xinitvisual(void);
static int max_textw(void);
static size_t nextrune(int inc);
static char * cistrstr(const char *s, const char *sub);
static int (*fstrncmp)(const char *, const char *, size_t) = strncasecmp;
static char *(*fstrstr)(const char *, const char *) = cistrstr;

static char **list;
static size_t listsize;
static int listcount;
static int listchanged = 0;

// user configuration
#include "options.h"
#include "keybinds.h"
#include "mouse.h"

static char *fonts[] = { font };

// color array
#include "libs/colors.h"

// config file
#if USECONFIG
#include "libs/conf/config.h"
#include "libs/conf/config.c"
#endif

// include functions
#include "libs/img.h"
#include "libs/img.c"
#include "libs/rtl.h"
#include "libs/rtl.c"
#include "libs/sort.c"
#include "libs/draw.c"
#include "libs/schemes.c"
#include "libs/argv.h"
#include "libs/argv.c"
#include "libs/match.h"
#include "libs/match.c"
#include "libs/x11/inc.c" // include x11
#include "libs/history.c"
#include "libs/arg.c"
#include "libs/stream.c"
#include "libs/event.h"
#include "libs/event.c"

int is_selected(size_t index) {
    for (int i = 0; i < sel_size; i++) {
        if (sel_index[i] == index) {
            return 1;
        }
    }

    return 0;
}

void appenditem(struct item *item, struct item **list, struct item **last) {
    if (*last)
        (*last)->right = item;
    else
        *list = item;

    item->left = *last;
    item->right = NULL;
    *last = item;
}

void recalculatenumbers(void) {
    unsigned int numer = 0, denom = 0, selected = 0;
    struct item *item;
    if (matchend) {
        numer++;

        // walk through items that match and add to numer
        for (item = matchend; item && item->left; item = item->left)
            numer++;
    }

    // walk through all items, matching or not and add to denom
    for (item = items; item && item->text; item++)
        denom++;

    for (int i = 0; i < sel_size; i++) {
        if (sel_index[i] == -1) {
            break;
        }

        selected++;
    }

    if (selected) {
        snprintf(numbers, NUMBERSBUFSIZE, "%d/%d/%d", numer, denom, selected);
    } else {
        snprintf(numbers, NUMBERSBUFSIZE, "%d/%d", numer, denom);
    }
}

void calcoffsets(void) {
    int i, n;

    if (lines > 0)
        n = lines * columns * bh;
    else { // no lines, therefore the size of items must be decreased to fit the menu elements
        int numberWidth = 0;
        int modeWidth = 0;
        int larrowWidth = 0;
        int rarrowWidth = 0;
        int capsWidth = 0;

        if (!hidematchcount) numberWidth = pango_numbers ? TEXTWM(numbers) : TEXTW(numbers);
        if (!hidemode) modeWidth = pango_mode ? TEXTWM(modetext) : TEXTW(modetext);
        if (!hidelarrow) larrowWidth = pango_leftarrow ? TEXTWM(leftarrow) : TEXTW(leftarrow);
        if (!hiderarrow) rarrowWidth = pango_rightarrow ? TEXTWM(rightarrow) : TEXTW(rightarrow);
        if (!hidecaps) capsWidth = pango_caps ? TEXTWM(capstext) : TEXTW(capstext);

        if (!strcmp(capstext, ""))
            capsWidth = 0;

        n = mw - (promptw + inputw + larrowWidth + rarrowWidth + modeWidth + numberWidth + capsWidth + menumarginv);
    }

    // calculate which items will begin the next page
    for (i = 0, next = curr; next; next = next->right)
        if ((i += (lines > 0) ? bh : MIN(TEXTWM(next->text), n)) > n)
            break;

    // calculate which items will begin the previous page
    for (i = 0, prev = curr; prev && prev->left; prev = prev->left)
        if ((i += (lines > 0) ? bh : MIN(TEXTWM(prev->left->text), n)) > n)
            break;
}

int max_textw(void) {
    int len = 0;

    for (struct item *item = items; item && item->text; item++)
        len = MAX(TEXTW(item->text), len);

    return len;
}

void cleanup(void) {
    size_t i;

#if USEIMAGE
    cleanupimage(); // function frees images
#endif

    XUngrabKey(dpy, AnyKey, AnyModifier, root); // ungrab keys

    // free color scheme
    for (i = 0; i < LENGTH(colors) + 1; i++)
        free(scheme[i]);

    // free high priority items
    for (i = 0; i < hplength; ++i)
        free(hpitems[i]);

    // free drawing and close the display
    drw_free(drw);
    XSync(dpy, False);
    XCloseDisplay(dpy);
    free(sel_index);
}

char * cistrstr(const char *h, const char *n) {
    size_t i;

    if (!n[0])
        return (char *)h;

    for (; *h; ++h) {
        for (i = 0; n[i] && tolower((unsigned char)n[i]) ==
                tolower((unsigned char)h[i]); ++i);

        if (n[i] == '\0')
            return (char *)h;
    }

    return NULL;
}

void grabfocus(void) {
    struct timespec ts = { .tv_sec = 0, .tv_nsec = 10000000  };
    Window focuswin;
    int i, revertwin;
    XWindowAttributes wa;

    XSync(dpy, False);
    XGetWindowAttributes(dpy, win, &wa);

    for (i = 0; i < 100; ++i) {
        XGetInputFocus(dpy, &focuswin, &revertwin);
        if (focuswin == win)
            return;

        // if it's a client, we can't just steal all the input for ourselves
        if (managed) {
            if (wa.map_state == IsViewable) {
                XTextProperty prop;
                char *windowtitle = prompt != NULL ? prompt : "spmenu";
                Xutf8TextListToTextProperty(dpy, &windowtitle, 1, XUTF8StringStyle, &prop);
                XSetWMName(dpy, win, &prop);
                XSetTextProperty(dpy, win, &prop, XInternAtom(dpy, "_NET_WM_NAME", False));
                XFree(prop.value);
            }
        } else { // spmenu is not managed, and is very greedy
            if (wa.map_state == IsViewable) // it must be viewable first, otherwise we get a BadMatch error
                XSetInputFocus(dpy, win, RevertToParent, CurrentTime);
        }

        nanosleep(&ts, NULL);
    }

    die("spmenu: cannot grab focus"); // not possible to grab focus, abort immediately
}

void insert(const char *str, ssize_t n) {
    if (strlen(text) + n > sizeof text - 1)
        return;

    static char l[BUFSIZ] = "";
    if (requirematch) memcpy(l, text, BUFSIZ);

    // move existing text out of the way, insert new text, and update cursor
    memmove(&text[cursor + n], &text[cursor], sizeof text - cursor - MAX(n, 0));

    // update cursor
    if (n > 0 && str && n)
        memcpy(&text[cursor], str, n);

    // add to cursor position and continue matching
    cursor += n;
    match();

    if (!matches && requirematch) {
        memcpy(text, l, BUFSIZ);
        cursor -= n;
        match();
    }
}

size_t nextrune(int inc) {
    ssize_t n;

    // return location of next utf8 rune in the given direction (+1 or -1)
    for (n = cursor + inc; n + inc >= 0 && (text[n] & 0xc0) == 0x80; n += inc)
        ;
    return n;
}

void pastesel(void) {
    char *p, *q;
    int di;
    unsigned long dl;
    Atom da;

    // we have been given the current selection, now insert it into input
    if (XGetWindowProperty(dpy, win, utf8, 0, (sizeof text / 4) + 1, False,
                utf8, &da, &di, &dl, &dl, (unsigned char **)&p)
            == Success && p) {
        insert(p, (q = strchr(p, '\n')) ? q - p : (ssize_t)strlen(p)); // insert selection
        XFree(p);
    }

    // draw the menu
    drawmenu();
}

void xinitvisual(void) {
    XVisualInfo *infos;
    XRenderPictFormat *fmt;
    int nitems;
    int i;

    // visual properties
    XVisualInfo tpl = {
        .screen = screen,
        .depth = 32,
        .class = TrueColor
    };

    long masks = VisualScreenMask | VisualDepthMask | VisualClassMask;

    infos = XGetVisualInfo(dpy, masks, &tpl, &nitems);
    visual = NULL;

    // create colormap
    for(i = 0; i < nitems; i ++) {
        fmt = XRenderFindVisualFormat(dpy, infos[i].visual);
        if (fmt->type == PictTypeDirect && fmt->direct.alphaMask) {
            visual = infos[i].visual;
            depth = infos[i].depth;
            cmap = XCreateColormap(dpy, root, visual, AllocNone);
            useargb = 1;
            break;
        }
    }

    XFree(infos);

    // no alpha, reset to default
    if (!visual || !alpha) {
        visual = DefaultVisual(dpy, screen);
        depth = DefaultDepth(dpy, screen);
        cmap = DefaultColormap(dpy, screen);
    }
}

int main(int argc, char *argv[]) {
    readargs(argc, argv); // start by reading arguments

    // open x11 display and create drawable
    handle_x11();

    // load fonts
    if (!drw_font_create(drw, fonts, LENGTH(fonts)))
        die("no fonts could be loaded.");

    // pledge limits what programs can do, so here we specify what spmenu should be allowed to do
#ifdef __OpenBSD__
    if (pledge("stdio rpath wpath cpath", NULL) == -1)
        die("pledge");
#endif

    loadhistory(); // read history entries

    // fast (-f) means we grab keyboard before reading standard input
    if (fast && !isatty(0)) {
        grabkeyboard();
        readstdin();
    } else {
        readstdin();
        grabkeyboard();
    }

    store_image_vars();
    set_mode();

    init_appearance(); // init colorschemes by reading arrays
    setupdisplay(); // set up display and create window
    eventloop(); // function is a loop which checks X11 events and calls other functions accordingly

    return 1; // should be unreachable
}
