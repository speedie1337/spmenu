/* spmenu - fancy dynamic menu
 * See LICENSE file for copyright and license details.
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <math.h>
#include <unistd.h>
#include "main.c"

#include "draw/draw.h"

#ifndef VERSION
#define VERSION "unknown"
#endif

enum {
    ClickWindow,
    ClickPrompt,
    ClickInput,
    ClickLArrow,
    ClickItem,
    ClickRArrow,
    ClickNumber,
    ClickCaps,
    ClickMode,
    ClickImage,
    ClickNone,
};

struct item {
    char *text;
    char *nsgrtext;
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
    int isdrawing;

    int inputw; // input width
    int promptw; // prompt width
    int plw; // powerline width

    int itemnumber; // item number
    int listcount;
    int listchanged;

    int maxlen; // max length of text

    size_t cursor; // cursor width

    int ignoreconfkeys; // can be set globally if you don't want to override keybinds with config file keys
    int ignoreglobalkeys; // should be set in the config file, if 1, the Keys keys array is ignored
    int ignoreconfmouse; // same for mouse
    int ignoreglobalmouse; // same for mouse
};

struct mo {
    int output_width; // output width
    int output_height; // output height
    int output_xpos; // output x position
    int output_ypos; // output y position
};

#if IMAGE
struct img {
    int setlines; // actual lines
    int flip; // %=
    int longestedge; // MAX(imagewidth, imageheight)
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

#if X11
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
#if IMAGE
static struct img img = {0};
#endif
#if X11
static struct x11 x11 = {0};
#endif

static struct item *items = NULL;
static struct item *history_items;
static struct item *list_items;
static struct item *matches;
static struct item *matchend;

static struct item *previousitem;
static struct item *currentitem;
static struct item *nextitem;
static struct item *selecteditem;
static struct item *mouseitem;

static Draw_t *draw;

static int hplength = 0;
static char **hpitems = NULL;

static int theme_override = 0;
static int binds_override = 0;
static int protocol_override = 0;

static int *sel_index = NULL;
static unsigned int sel_size = 0;
static int itemn = 0;

#if RTL
static int isrtl = 1;
#else
static int isrtl = 0;
#endif

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

static char **list;
static size_t listsize;

// various headers
#include "options.h"
#include "draw.h"
#include "arg.h"

#include "x11/inc.h"
#include "wl/inc.h"

static char *fonts[] = { font };

#include "img.c"
#include "icon.c"
#include "rtl.c"
#include "sort.c"
#include "match.c"
#include "schemes.c"
#include "stream.c"
#include "draw.c"
#include "conf/config.c"
#include "argv.c"
#include "history.c"
#include "arg.c"
#include "fifo.c"

#if X11
static Key keys[] = {
    { -1,      0,              XK_Return,    selectitem,      {.i = +1 } },
    { -1,      Shift,          XK_Return,    selectitem,      {0} },
    { -1,      Ctrl,           XK_Return,    markitem,        {0} },
    { -1,      0,              XK_Tab,       complete,        {0} },
    { -1,      Ctrl,           XK_v,         paste,           {.i = 2  } },
    { -1,      0,              XK_BackSpace, backspace,       {0} },
    { -1,      Ctrl,           XK_BackSpace, deleteword,      {0} },
    { -1,      Ctrl|Shift,     XK_p,         setprofile,      {0} },
    { -1,      0,              XK_Print,     screenshot,      {0} },
    { -1,      Ctrl,           XK_equal,     setimgsize,      {.i = +10 } },
    { -1,      Ctrl,           XK_minus,     setimgsize,      {.i = -10 } },
    { -1,      0,              XK_Up,        moveup,          {0} },
    { -1,      0,              XK_Down,      movedown,        {0} },
    { -1,      0,              XK_Left,      moveleft,        {0} },
    { -1,      0,              XK_Right,     moveright,       {0} },
    { -1,      Ctrl,           XK_u,         moveup,          {.i = 5  } },
    { -1,      Ctrl,           XK_d,         movedown,        {.i = 5  } },
    { -1,      Shift,          XK_h,         viewhist,        {0} },
    { -1,      0,              XK_Escape,    quit,            {0} },
    { -1,      Ctrl,           XK_p,         navhistory,      {.i = -1 } },
    { -1,      Ctrl,           XK_n,         navhistory,      {.i = +1 } },
};
#endif
#if WAYLAND
static WlKey wl_keys[] = {
    { -1,      WL_None,              XKB_KEY_Return,    selectitem,      {.i = +1 } },
    { -1,      WL_Shift,             XKB_KEY_Return,    selectitem,      {0} },
    { -1,      WL_Ctrl,              XKB_KEY_Return,    markitem,        {0} },
    { -1,      WL_None,              XKB_KEY_Tab,       complete,        {0} },
    { -1,      WL_Ctrl,              XKB_KEY_v,         paste,           {.i = 2  } },
    { -1,      WL_None,              XKB_KEY_BackSpace, backspace,       {0} },
    { -1,      WL_Ctrl,              XKB_KEY_BackSpace, deleteword,      {0} },
    { -1,      WL_CtrlShift,         XKB_KEY_p,         setprofile,      {0} },
    { -1,      WL_None,              XKB_KEY_Print,     screenshot,      {0} },
    { -1,      WL_Ctrl,              XKB_KEY_equal,     setimgsize,      {.i = +10 } },
    { -1,      WL_Ctrl,              XKB_KEY_minus,     setimgsize,      {.i = -10 } },
    { -1,      WL_None,              XKB_KEY_Up,        moveup,          {0} },
    { -1,      WL_None,              XKB_KEY_Down,      movedown,        {0} },
    { -1,      WL_None,              XKB_KEY_Left,      moveleft,        {0} },
    { -1,      WL_None,              XKB_KEY_Right,     moveright,       {0} },
    { -1,      WL_Ctrl,              XKB_KEY_u,         moveup,          {.i = 5  } },
    { -1,      WL_Ctrl,              XKB_KEY_d,         movedown,        {.i = 5  } },
    { -1,      WL_Shift,             XKB_KEY_h,         viewhist,        {0} },
    { -1,      WL_None,              XKB_KEY_Escape,    quit,            {0} },
    { -1,      WL_Ctrl,              XKB_KEY_p,         navhistory,      {.i = -1 } },
    { -1,      WL_Ctrl,              XKB_KEY_n,         navhistory,      {.i = +1 } },

};
#endif
#if X11
static Mouse buttons[] = {
    { ClickInput,           Button1,         clear,        {0} },
    { ClickPrompt,          Button1,         clear,        {0} },
    { ClickMode,            Button1,         switchmode,   {0} },
    { ClickNumber,          Button1,         viewhist,     {0} },
    { ClickItem,            Button1,         selecthover,  {0} },
    { ClickItem,            Button2,         markhover,    {0} },
    { ClickNone,            Button5,         movenext,     {0} },
    { ClickNone,            Button4,         moveprev,     {0} },
};
#endif
#if WAYLAND
static WlMouse wl_buttons[] = {
    { ClickInput,           WL_Left,         clear,        {0} },
    { ClickPrompt,          WL_Left,         clear,        {0} },
    { ClickMode,            WL_Left,         switchmode,   {0} },
    { ClickNumber,          WL_Left,         viewhist,     {0} },
    { ClickItem,            WL_Left,         selecthover,  {0} },
    { ClickItem,            WL_Right,        markhover,    {0} },
    { ClickNone,            WL_Down,         movenext,     {0} },
    { ClickNone,            WL_Up,           moveprev,     {0} },
};
#endif

#include "x11/inc.c"
#include "wl/inc.c"

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

        for (item = matchend; item && item->left; item = item->left)
            numer++;
    }

    for (item = items; item && item->text; item++) {
        denom++;
    }

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
    int i, offset;
    int numberw = 0;
    int modew = 0;
    int larroww = 0;
    int rarroww = 0;
    int capsw = 0;

    if (!hidematchcount) numberw = pango_numbers ? TEXTWM(tx.numbers) : TEXTW(tx.numbers);
    if (!hidemode) modew = pango_mode ? TEXTWM(tx.modetext) : TEXTW(tx.modetext);
    if (!hidelarrow) larroww = pango_leftarrow ? TEXTWM(leftarrow) : TEXTW(leftarrow);
    if (!hiderarrow) rarroww = pango_rightarrow ? TEXTWM(rightarrow) : TEXTW(rightarrow);
    if (!hidecaps) capsw = pango_caps ? TEXTWM(tx.capstext) : TEXTW(tx.capstext);

    if (!strcmp(tx.capstext, "")) {
        capsw = 0;
    }

    if (lines > 0) {
        offset = lines * columns * sp.bh;
        sp.maxlen = sp.mw - (sp.promptw + modew + numberw + capsw + menumarginh);
    } else { /* no lines, therefore the size of items must be decreased to fit the menu elements */
        offset = sp.mw - (sp.promptw + sp.inputw + larroww + rarroww + modew + numberw + capsw + menumarginh);
        sp.maxlen = selecteditem ? sp.inputw : sp.mw - (sp.promptw + modew + numberw + capsw + (selecteditem ? larroww : 0) + (selecteditem ? rarroww : 0));
    }

    for (i = 0, nextitem = currentitem; nextitem; nextitem = nextitem->right) { // next page
        nextitem->nsgrtext = get_text_n_sgr(nextitem);

        if ((i += (lines > 0) ? sp.bh : MIN(TEXTWM(nextitem->nsgrtext) + (powerlineitems ? !lines ? 3 * sp.plw : 0 : 0), offset)) > offset)
            break;
    }

    for (i = 0, previousitem = currentitem; previousitem && previousitem->left; previousitem = previousitem->left) { // previous page
        previousitem->nsgrtext = get_text_n_sgr(previousitem);

        if ((i += (lines > 0) ? sp.bh : MIN(TEXTWM(previousitem->left->nsgrtext) + (powerlineitems ? !lines ? 3 * sp.plw : 0 : 0), offset)) > offset)
            break;
    }
}

int max_textw(void) {
    int len = 0;

    for (struct item *item = items; item && item->text; item++)
        len = MAX(TEXTW(item->text), len);

    return len;
}

void cleanup(void) {
    size_t i;

#if IMAGE
    cleanupimage();
#endif

    for (i = 0; i < hplength; ++i)
        free(hpitems[i]);

    draw_free(draw);

#if X11
    if (!protocol) {
        cleanup_x11(dpy);
    }
#endif

#if FIFO
    remove(fifofile);
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

void insert(const char *str, ssize_t n) {
    if (strlen(tx.text) + n > sizeof tx.text - 1)
        return;

    static char l[BUFSIZ] = "";

    if (requirematch) {
        memcpy(l, tx.text, BUFSIZ);
    }

    // move existing text out of the way, insert new text, and update cursor
    memmove(
            &tx.text[sp.cursor + n],
            &tx.text[sp.cursor],
            sizeof tx.text - sp.cursor - MAX(n, 0)
    );

    if (n > 0 && str && n) {
        memcpy(&tx.text[sp.cursor], str, n);
    }

    sp.cursor += n;
    match();

    if (!matches && requirematch) {
        memcpy(tx.text, l, BUFSIZ);
        sp.cursor -= n;
        match();
    }

    // output on insertion
    if (incremental) {
        puts(tx.text);
        fflush(stdout);
    }
}

size_t nextrune(int inc) {
    ssize_t rune;

    // return location of next utf8 rune in the given direction (+1 or -1)
    for (rune = sp.cursor + inc; rune + inc >= 0 && (tx.text[rune] & 0xc0) == 0x80; rune += inc)
        ;

    return rune;
}

void resizeclient(void) {
#if WAYLAND
    if (protocol) {
        resizeclient_wl(&state);
    } else {
#if X11
        resizeclient_x11();
#endif
    }
#elif X11
    resizeclient_x11();
#endif
}

/* Width reserved for input when !lines is a fixed size of the menu width * inputwidth
 * This is reasonable, but in rare cases may cause input text to overlap
 * items.
 */
void get_width(void) {
    sp.inputw = sp.mw * inputwidth;
}

void get_mh(void) {
    int epad;

    sp.mh = (lines + 1) * sp.bh;
    sp.mh += 2 * menumarginv;

    // subtract 1 line if there's nothing to draw on the top line
    if ((hideprompt && hideinput && hidemode && hidematchcount && hidecaps) && lines) {
        sp.mh -= sp.bh;
    }

    epad = 2 * menupaddingv;

    // the spmenu window should not exceed the screen resolution height
    if (mo.output_height && !xpos && !ypos) {
        sp.mh = MIN(sp.mh, mo.output_height - epad);

        if (sp.mh == mo.output_height - epad) {
            lines = ((mo.output_height - epad) / sp.bh);
        }
    }
}

void set_mode(void) {
    if (!type) { // no typing allowed, require normal mode
        sp.mode = 0;
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

    if (forceinsertmode) {
        sp.mode = 1;
        sp.allowkeys = 1;
    }
}

void handle(void) {
    if (!protocol) {
#if X11
        handle_x11();

        if (!draw_font_create(draw, fonts, LENGTH(fonts))) {
            die("no fonts could be loaded.");
        }

        loadhistory();
#if IMAGE
        store_image_vars();
#endif

        if (fast && !isatty(0)) {
            if (grabkeyboard) {
                grabkeyboard_x11();
            }

            readstdin();
        } else {
            readstdin();

            if (grabkeyboard) {
                grabkeyboard_x11();
            }
        }

        set_mode();

        init_appearance(); // init colorschemes by reading arrays

        setupdisplay_x11(); // set up display and create window
#if FIFO
        init_fifo();
#endif
        eventloop_x11(); // function is a loop which checks X11 events and calls other functions accordingly
#endif
#if WAYLAND
    } else {
        loadhistory();
#if IMAGE
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

#if FIFO
        init_fifo();
#endif

        handle_wl();
#endif
    }
}

int main(int argc, char *argv[]) {
    readargs(argc, argv);

    /* pledge limits what programs can do, so here we specify what spmenu should be allowed to do
     * TODO: Test this on an actual OpenBSD operating system
     */
#ifdef __OpenBSD__
    if (pledge("stdio rpath wpath cpath", NULL) == -1)
        die("pledge");
#endif

    handle();

    return 1;
}
