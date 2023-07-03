/* spmenu - fancy dynamic menu
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
#include <signal.h>
#include <fcntl.h>

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

// check if we should enable Wayland support
#ifndef WAYLAND
#define USEWAYLAND 0
#else
#define USEWAYLAND 1
#endif

// check if we should enable X11 support
#ifndef X11
#define USEX 0
#else
#define USEX 1
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

// include Xlib
#if USEX
#include <X11/Xlib.h>
#endif

// include macros and other defines
#include "libs/define.c"

enum {
    ClickWindow,
    ClickPrompt,
    ClickInput,
    ClickLArrow,
    ClickItem,
    ClickSelItem,
    ClickRArrow,
    ClickNumber,
    ClickCaps,
    ClickMode,
};

struct item {
    char *text;
    char *clntext;
    char *image;
    char *ex;
    struct item *left, *right;
    int hp;
    int index;
    double distance;
};

struct sp {
    int bh; // height of each menu item
    int mw; // width
    int mh; // height
    int vp; // vertical padding for bar
    int sp; // side padding for bar
    int lrpad; // sum of left and right padding

    int mode; // current mode
    int allowkeys; // interpret a keypress as an insertion?
    int capslockstate; // caps lock state

    int inputw; // input width
    int promptw; // prompt width
    int plw; // powerline width

    int itemnumber; // item number

    size_t cursor; // cursor width

    int ignoreconfkeys; // can be set globally if you don't want to override keybinds with config file keys
    int ignoreglobalkeys; // should be set in the config file, if 1, the Keys keys array is ignored
    int ignoreconfmouse; // same for mouse
    int ignoreglobalmouse; // same for mouse
};

struct mo {
    int output_width; // output width
    int output_height; // output height
};

#if USEIMAGE
struct img {
    int setlines; // actual lines
    int flip; // %=
    int longestedge; // MAX(imagewidth, imagheight)
    int imagewidth; // current image width
    int imageheight; // current image height
    int imagegaps; // current image gaps
};
#endif

struct tx {
    char modetext[64]; // mode text
    char text[BUFSIZ]; // input text
    char numbers[NUMBERSBUFSIZE]; // number text
    char capstext[64]; // caps lock text
};

#if USEX
struct x11 {
    int numlockmask;
    int useargb;
    int depth;
    char *embed;
    int screen;
    Visual *visual;
    Colormap cmap;
};
#endif

static struct sp sp = {0};
static struct tx tx = {0};
static struct mo mo = {0};
#if USEIMAGE
static struct img img = {0};
#endif
#if USEX
static struct x11 x11 = {0};
#endif

static struct item *items = NULL, *backup_items, *list_items;
static struct item *matches, *matchend;
static struct item *prev, *curr, *next, *sel;

// various headers
#include "libs/draw/draw.h"
#include "libs/main.h"
#include "libs/draw.h"
#include "libs/stream.h"
#include "libs/schemes.h"
#include "libs/arg.h"
#include "libs/x11/inc.h" // include x11
#include "libs/wl/inc.h" // include wayland
#include "libs/sort.h"
#include "libs/history.h"

static Draw_t *draw;

// high priority
static int hplength = 0;
static char **hpitems = NULL;

static int *sel_index = NULL;
static unsigned int sel_size = 0;
static int protocol_override = 0;
static int itemn = 0;

#if USERTL
static int isrtl = 1;
#else
static int isrtl = 0;
#endif

// declare functions
static int is_selected(size_t index);
static void calcoffsets(void);
static void recalculatenumbers(void);
static void insert(const char *str, ssize_t n);
static void cleanup(void);
static void navigatehistfile(int dir);
static void resizeclient(void);
static void get_width(void);
static void get_mh(void);
static void set_mode(void);
static void handle(void);
static void appenditem(struct item *item, struct item **list, struct item **last);
static int max_textw(void);
static size_t nextrune(int inc);
static char * cistrstr(const char *s, const char *sub);
static int (*fstrncmp)(const char *, const char *, size_t) = strncasecmp;
static char *(*fstrstr)(const char *, const char *) = cistrstr;

#if USEX
static void pastesel(void);
static void grabfocus(void);
#endif

static char **list;
static size_t listsize;
static int listcount;
static int listchanged = 0;

// user configuration
#include "libs/options.h"
#include "libs/keybinds.h"
#include "libs/mouse.h"

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
#include "libs/icon.h"
#include "libs/icon.c"
#include "libs/rtl.h"
#include "libs/rtl.c"
#include "libs/sort.c"
#include "libs/match.h"
#include "libs/match.c"
#include "libs/draw.c"
#include "libs/schemes.c"
#include "libs/argv.h"
#include "libs/argv.c"

// include x11 code
#include "libs/x11/inc.c"
#include "libs/wl/inc.c"

// include more functions
#include "libs/history.c"
#include "libs/arg.c"
#include "libs/stream.c"

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
        snprintf(tx.numbers, NUMBERSBUFSIZE, "%d/%d/%d", numer, denom, selected);
    } else {
        snprintf(tx.numbers, NUMBERSBUFSIZE, "%d/%d", numer, denom);
    }
}

void calcoffsets(void) {
    int i, n;

    if (lines > 0)
        n = lines * columns * sp.bh;
    else { // no lines, therefore the size of items must be decreased to fit the menu elements
        int numberw = 0;
        int modeWidth = 0;
        int larroww = 0;
        int rarrowWidth = 0;
        int capsw = 0;

        if (!hidematchcount) numberw = pango_numbers ? TEXTWM(tx.numbers) : TEXTW(tx.numbers);
        if (!hidemode) modeWidth = pango_mode ? TEXTWM(tx.modetext) : TEXTW(tx.modetext);
        if (!hidelarrow) larroww = pango_leftarrow ? TEXTWM(leftarrow) : TEXTW(leftarrow);
        if (!hiderarrow) rarrowWidth = pango_rightarrow ? TEXTWM(rightarrow) : TEXTW(rightarrow);
        if (!hidecaps) capsw = pango_caps ? TEXTWM(tx.capstext) : TEXTW(tx.capstext);

        if (!strcmp(tx.capstext, ""))
            capsw = 0;

        n = sp.mw - (sp.promptw + sp.inputw + larroww + rarrowWidth + modeWidth + numberw + capsw + menumarginh);
    }

    // calculate which items will begin the next page
    for (i = 0, next = curr; next; next = next->right)
        if ((i += (lines > 0) ? sp.bh : MIN(TEXTWM(next->text) + (powerlineitems ? !lines ? 2 * sp.plw : 0 : 0), n)) > n)
            break;

    // calculate which items will begin the previous page
    for (i = 0, prev = curr; prev && prev->left; prev = prev->left)
        if ((i += (lines > 0) ? sp.bh : MIN(TEXTWM(prev->left->text) + (powerlineitems ? !lines ? 2 * sp.plw : 0 : 0), n)) > n)
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

    // free high priority items
    for (i = 0; i < hplength; ++i)
        free(hpitems[i]);

    // free drawing and close the display
    draw_free(draw);

#if USEX
    if (!protocol) {
        cleanup_x11(dpy);
    }
#endif

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

#if USEX
void grabfocus(void) {
    grabfocus_x11();
}
#endif

void insert(const char *str, ssize_t n) {
    if (strlen(tx.text) + n > sizeof tx.text - 1)
        return;

    static char l[BUFSIZ] = "";
    if (requirematch) memcpy(l, tx.text, BUFSIZ);

    // move existing text out of the way, insert new text, and update cursor
    memmove(&tx.text[sp.cursor + n], &tx.text[sp.cursor], sizeof tx.text - sp.cursor - MAX(n, 0));

    // update cursor
    if (n > 0 && str && n)
        memcpy(&tx.text[sp.cursor], str, n);

    // add to cursor position and continue matching
    sp.cursor += n;
    match();

    if (!matches && requirematch) {
        memcpy(tx.text, l, BUFSIZ);
        sp.cursor -= n;
        match();
    }
}

size_t nextrune(int inc) {
    ssize_t n;

    // return location of next utf8 rune in the given direction (+1 or -1)
    for (n = sp.cursor + inc; n + inc >= 0 && (tx.text[n] & 0xc0) == 0x80; n += inc)
        ;
    return n;
}

#if USEX
void pastesel(void) {
    if (!protocol) {
        pastesel_x11();
    }
}
#endif

void resizeclient(void) {
#if USEWAYLAND
    if (protocol) {
        resizeclient_wl(&state);
    } else {
#if USEX
        resizeclient_x11();
#endif
    }
#elif USEX
    resizeclient_x11();
#endif
}

void get_width(void) {
    sp.inputw = sp.mw / 3;
}

void get_mh(void) {
    sp.mh = (lines + 1) * sp.bh;
    sp.mh += 2 * menumarginv;

    if ((hideprompt && hideinput && hidemode && hidematchcount && hidecaps) && lines) {
        sp.mh -= sp.bh;
    }
}

void set_mode(void) {
    if (!type) { // no typing allowed, require normal mode
        mode = 0;
    }

    // set default mode, must be done before the event loop or keybindings will not work
    if (mode) {
        sp.mode = 1;
        sp.allowkeys = 1;

        sp_strncpy(tx.modetext, instext, sizeof(tx.modetext));
    } else {
        sp.mode = 0;
        sp.allowkeys = !sp.mode;

        sp_strncpy(tx.modetext, normtext, sizeof(tx.modetext));
    }
}

void handle(void) {
    if (!protocol) {
#if USEX
        handle_x11();

        if (!draw_font_create(draw, fonts, LENGTH(fonts))) {
            die("no fonts could be loaded.");
        }

        loadhistory(); // read history entries
#if USEIMAGE
        store_image_vars();
#endif

        // fast (-f) means we grab keyboard before reading standard input
        if (fast && !isatty(0)) {
            grabkeyboard_x11();
            readstdin();
        } else {
            readstdin();
            grabkeyboard_x11();
        }

        set_mode();

        init_appearance(); // init colorschemes by reading arrays
        setupdisplay_x11(); // set up display and create window
        eventloop_x11(); // function is a loop which checks X11 events and calls other functions accordingly
#endif
#if USEWAYLAND
    } else {
        loadhistory();
#if USEIMAGE
        store_image_vars();
        setimageopts();
#endif

        // Disable some X11 only features
        menupaddingv = menupaddingh = 0;
        xpos = ypos = 0;
        borderwidth = 0;
        managed = 0;

        draw = draw_create_wl(protocol);

        if (!draw_font_create(draw, fonts, LENGTH(fonts))) {
            die("no fonts could be loaded.");
        }

        readstdin();
        set_mode();
        init_appearance();

        handle_wl();
#endif
    }
}

int main(int argc, char *argv[]) {
    readargs(argc, argv); // start by reading arguments

    // pledge limits what programs can do, so here we specify what spmenu should be allowed to do
#ifdef __OpenBSD__
    if (pledge("stdio rpath wpath cpath", NULL) == -1)
        die("pledge");
#endif

    handle();

    return 1; // should be unreachable
}
