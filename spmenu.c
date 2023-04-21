/* spmenu - fancy dynamic menu
 *
 * If you're looking for functions used to draw text, see 'libs/draw.c'
 * If you're looking for wrapper functions used inside the draw functions, see 'libs/sl/draw.c'
 * If you're looking for functions used to draw images, see 'libs/img.c'
 * If you're looking for the .Xresources array, see 'libs/xresources.h'
 *
 * You don't need to edit spmenu.c if you aren't making big changes to the software.
 *
 * After making changes, run 'make clean install' to install and 'make' to attempt a compilation.
 * `make man` will generate a man page from 'docs/docs.md', which is a Markdown document. Run this before commiting.
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
// openssl is used to generate a checksum, used for caching
// TODO: remove this dependency by doing it some other way
#endif

// include xinerama used for multi monitor support
#if USEXINERAMA
#include <X11/extensions/Xinerama.h>
#endif

// include X11 headers
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <X11/Xft/Xft.h>

// include pango used for markup
#if USEPANGO
#include <pango/pango.h>
#endif

// include macros and other defines
#include "libs/define.c"

// various headers
#include "libs/sl/draw.h"
#include "libs/sl/main.h"
#include "libs/draw.h"
#include "libs/stream.h"
#include "libs/schemes.h"
#include "libs/arg.h"
#include "libs/mode.h"
#include "libs/xrdb.h"
#include "libs/key.h"
#include "libs/mouse.h"
#include "libs/sort.h"
#include "libs/history.h"

// text
static char text[BUFSIZ] = "";
static char numbers[NUMBERSBUFSIZE] = "";

// high priority
static int hplength = 0;
static char **hpitems = NULL;

// embed
static char *embed;

// keybinds
static int numlockmask = 0;
static int capslockstate = 0;

// height of each item, menu width, menu height
static int bh, mw, mh;
static int reallines = 0;
static int inputw = 0;
static int promptw;
static int plw = 0;
static int lrpad; // sum of left and right padding
static int vp;    // vertical padding for bar
static int sp;    // side padding for bar
static size_t cursor;
static struct item *items = NULL, *backup_items;
static struct item *matches, *matchend;
static struct item *prev, *curr, *next, *sel;
static int screen;

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
#endif

#if USECONFIG
static int cconf = 0; // custom config path
static char *argconf = NULL;
#endif

// set an integer to 1 if we have rtl enabled, this saves a lot of lines and duplicate code
#if USERTL
static int isrtl = 1;
#else
static int isrtl = 0;
#endif

// X11 properties
static Atom clip, utf8, types, dock;
static Display *dpy;
static Window root, parentwin, win;
static XIC xic;

// colors
static int useargb = 0;
static Visual *visual;
static int depth;
static Colormap cmap;
static Drw *drw;
static Clr *scheme[SchemeLast];
static Clr textclrs[256];

// declare functions
static void calcoffsets(void);
static void recalculatenumbers(void);
static void usage(void);
static void insert(const char *str, ssize_t n);
static void cleanup(void);
static void navigatehistfile(int dir);
static void grabfocus(void);
static void pastesel(void);
static void appenditem(struct item *item, struct item **list, struct item **last);
static int max_textw(void);
static size_t nextrune(int inc);

// user configuration
#include "options.h"
#include "keybinds.h"
#include "mouse.h"

// xresources/color arrays
#include "libs/xresources.h"
#include "libs/colors.h"

static char *fonts[] = { font };

// config file
#if USECONFIG
#include "libs/conf/config.h"
#include "libs/conf/config.c"
#endif

// matching
static char * cistrstr(const char *s, const char *sub);
static int (*fstrncmp)(const char *, const char *, size_t) = strncasecmp;
static char *(*fstrstr)(const char *, const char *) = cistrstr;

// include functions
#include "libs/img.h"
#include "libs/img.c"
#include "libs/rtl.h"
#include "libs/rtl.c"
#include "libs/event.h"
#include "libs/event.c"
#include "libs/key.c"
#include "libs/mouse.c"
#include "libs/sort.c"
#include "libs/draw.c"
#include "libs/schemes.c"
#include "libs/argv.h"
#include "libs/argv.c"
#include "libs/xrdb.c"
#include "libs/mode.c"
#include "libs/client.h"
#include "libs/client.c"
#include "libs/match.h"
#include "libs/match.c"
#include "libs/history.c"
#include "libs/arg.c"
#include "libs/stream.c"

void
appenditem(struct item *item, struct item **list, struct item **last)
{
	if (*last)
		(*last)->right = item;
	else
		*list = item;

	item->left = *last;
	item->right = NULL;
	*last = item;
}

void
recalculatenumbers(void)
{
	unsigned int numer = 0, denom = 0;
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

	snprintf(numbers, NUMBERSBUFSIZE, "%d/%d", numer, denom);
}

void
calcoffsets(void)
{
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

		n = mw - (promptw + inputw + larrowWidth + rarrowWidth + modeWidth + numberWidth + capsWidth + 2 * borderwidth + menumarginv);
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

int
max_textw(void)
{
	int len = 0;

	for (struct item *item = items; item && item->text; item++)
		len = MAX(TEXTW(item->text), len);

	return len;
}

void
cleanup(void)
{
	size_t i;

    #if USEIMAGE
    cleanupimage(); // function frees images
    #endif

	XUngrabKey(dpy, AnyKey, AnyModifier, root); // ungrab keys

    // free color scheme
	for (i = 0; i < SchemeLast; i++)
		free(scheme[i]);

    // free high priority items
    for (i = 0; i < hplength; ++i)
		free(hpitems[i]);

    // free drawing and close the display
	drw_free(drw);
	XSync(dpy, False);
	XCloseDisplay(dpy);
}

char *
cistrstr(const char *h, const char *n)
{
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

void
grabfocus(void)
{
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

void
insert(const char *str, ssize_t n)
{
	if (strlen(text) + n > sizeof text - 1)
		return;

	// move existing text out of the way, insert new text, and update cursor
	memmove(&text[cursor + n], &text[cursor], sizeof text - cursor - MAX(n, 0));

    // update cursor
	if (n > 0)
		memcpy(&text[cursor], str, n);

    // add to cursor position and continue matching
	cursor += n;
	match();
}

size_t
nextrune(int inc)
{
	ssize_t n;

	// return location of next utf8 rune in the given direction (+1 or -1)
	for (n = cursor + inc; n + inc >= 0 && (text[n] & 0xc0) == 0x80; n += inc)
        ;
	    return n;
}

void
pastesel(void)
{
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

void
xinitvisual()
{
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

void
setupdisplay(void)
{
	int x, y, i;
    #if USEXINERAMA
    int j, di;
    #endif
	unsigned int du;
    unsigned int tmp, minstrlen = 0, curstrlen = 0;
    int numwidthchecks = 100;
    struct item *item;
	XIM xim;
	Window w, dw, *dws;
    XWindowAttributes wa;
    #if USEXINERAMA
	XineramaScreenInfo *info;
	Window pw;
	int a, n, area = 0;
    #endif

    init_appearance(); // init colorschemes by reading arrays

    // set properties indicating what spmenu handles
	clip = XInternAtom(dpy, "CLIPBOARD",   False);
	utf8 = XInternAtom(dpy, "UTF8_STRING", False);
   	types = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", False);
	dock = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DOCK", False);

    // resize client
    bh = MAX(drw->font->h, drw->font->h + 2 + lineheight);
	lines = MAX(lines, 0);
    reallines = lines;

    // resize client to image height if deemed necessary
    #if USEIMAGE
    if (image) resizetoimageheight(imageheight);
    #endif

    mh = (lines + 1) * bh + 2 * menumarginv; // lines + 1 * bh is the menu height

    // set prompt width based on prompt size
    promptw = (prompt && *prompt)
        ? pango_prompt ? TEXTWM(prompt) : TEXTW(prompt) - lrpad / 4 : 0; // prompt width

    // get accurate width
    if (accuratewidth) {
        for (item = items; !lines && item && item->text; ++item) {
            curstrlen = strlen(item->text);
            if (numwidthchecks || minstrlen < curstrlen) {
                numwidthchecks = MAX(numwidthchecks - 1, 0);
                minstrlen = MAX(minstrlen, curstrlen);
                if ((tmp = MIN(TEXTW(item->text), mw/3) > inputw)) {
                    inputw = tmp;
                    if (tmp == mw/3)
                        break;
                }
            }
        }
    }

    // init xinerama screens
    #if USEXINERAMA
	i = 0;
	if (parentwin == root && (info = XineramaQueryScreens(dpy, &n))) {
		XGetInputFocus(dpy, &w, &di);
		if (mon >= 0 && mon < n)
			i = mon;
		else if (w != root && w != PointerRoot && w != None) {
			// find top-level window containing current input focus
			do {
				if (XQueryTree(dpy, (pw = w), &dw, &w, &dws, &du) && dws)
					XFree(dws);
			} while (w != root && w != pw);
			// find xinerama screen with which the window intersects most
			if (XGetWindowAttributes(dpy, pw, &wa))
				for (j = 0; j < n; j++)
					if ((a = INTERSECT(wa.x, wa.y, wa.width, wa.height, info[j])) > area) {
						area = a;
						i = j;
					}
		}
		// no focused window is on screen, so use pointer location instead
		if (mon < 0 && !area && XQueryPointer(dpy, root, &dw, &dw, &x, &y, &di, &di, &du))
			for (i = 0; i < n; i++)
				if (INTERSECT(x, y, 1, 1, info[i]))
					break;

        // calculate x/y position
		if (menuposition == 2) { // centered
			mw = MIN(MAX(max_textw() + promptw, minwidth), info[i].width);
			x = info[i].x_org + ((info[i].width  - mw) / 2);
			y = info[i].y_org + ((info[i].height - mh) / 2);
		} else { // top or bottom
		    x = info[i].x_org + xpos;
			y = info[i].y_org + (menuposition ? 0 : info[i].height - mh - ypos);
			mw = (menuwidth>0 ? menuwidth : info[i].width);
		}

		XFree(info);
	} else
    #endif
	{
		if (!XGetWindowAttributes(dpy, parentwin, &wa))
			die("spmenu: could not get embedding window attributes: 0x%lx",
			    parentwin); // die because unable to get attributes for the parent window

		if (menuposition == 2) { // centered
			mw = MIN(MAX(max_textw() + promptw, minwidth), wa.width);
			x = (wa.width  - mw) / 2;
			y = (wa.height - mh) / 2;
		} else { // top or bottom
			x = 0;
			y = menuposition ? 0 : wa.width - mh - ypos;
			mw = wa.width;
		}
	}

	// might be faster in some instances, most of the time unnecessary
    if (!accuratewidth) inputw = MIN(inputw, mw/3);

	match(); // match entries

	// create menu window and set properties for it
    create_window(x + sp, y + vp - (menuposition == 1 ? 0 : menuposition == 2 ? borderwidth : borderwidth * 2), mw - 2 * sp - borderwidth * 2, mh);
    set_window();
    set_prop();

    #if USEIMAGE
    setimageopts();
    #endif

	// input methods
    if ((xim = XOpenIM(dpy, NULL, NULL, NULL)) == NULL) {
		XSetLocaleModifiers("@im=local");
		if ((xim = XOpenIM(dpy, NULL, NULL, NULL)) == NULL) {
			XSetLocaleModifiers("@im=");
			if ((xim = XOpenIM(dpy, NULL, NULL, NULL)) == NULL)
				die("XOpenIM failed: could not open input device");
		}
	}

	xic = XCreateIC(xim, XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
	                XNClientWindow, win, XNFocusWindow, win, NULL);

	XMapRaised(dpy, win);

    XSync(dpy, False);
	XGetWindowAttributes(dpy, win, &wa);

    if (wa.map_state == IsViewable) // must be viewable, otherwise we get a BadMatch error
        XSetInputFocus(dpy, win, RevertToParent, CurrentTime);

    // embed spmenu inside parent window
	if (embed) {
		XSelectInput(dpy, parentwin, FocusChangeMask | SubstructureNotifyMask);
		if (XQueryTree(dpy, parentwin, &dw, &w, &dws, &du) && dws) {
			for (i = 0; i < du && dws[i] != win; ++i)
				XSelectInput(dpy, dws[i], FocusChangeMask);
			XFree(dws);
		}
		grabfocus();
	}

    // resize and draw
	drw_resize(drw, mw - 2 * sp - borderwidth * 2, mh);
	drawmenu();
}

int
main(int argc, char *argv[])
{
	XWindowAttributes wa;

    readargs(argc, argv); // start by reading arguments

    #if USEIMAGE
    longestedge = MAX(imagewidth, imageheight);
    #endif

    // set default mode, must be done before the event loop or keybindings will not work
    if (mode) {
        curMode = 1;
        allowkeys = 1;

        strcpy(modetext, instext);
    } else {
        curMode = 0;
        allowkeys = !curMode;

        strcpy(modetext, normtext);
    }

	if (!setlocale(LC_CTYPE, "") || !XSupportsLocale())
        fputs("warning: no locale support\n", stderr); // invalid locale, so notify the user about it

    if (!XSetLocaleModifiers(""))
		fputs("warning: no locale modifiers support\n", stderr);

    if (!(dpy = XOpenDisplay(NULL)))
        die("spmenu: cannot open display"); // failed to open display

    // set screen and root window
    screen = DefaultScreen(dpy);
	root = RootWindow(dpy, screen);

    // parent window is the root window (ie. window manager) because we're not embedding
    if (!embed || !(parentwin = strtol(embed, NULL, 0)))
		parentwin = root;

    if (!XGetWindowAttributes(dpy, parentwin, &wa))
		die("spmenu: could not get embedding window attributes: 0x%lx", parentwin);

	xinitvisual(); // init visual and create drawable after
	drw = drw_create(dpy, screen, root, wa.width, wa.height, visual, depth, cmap); // wrapper function creating a drawable

    // load fonts
	if (!drw_font_create(drw, fonts, LENGTH(fonts)))
	    die("no fonts could be loaded.");

    // resize window
    lrpad = drw->font->h + textpadding;
    prepare_window_size(); // this function sets padding size

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

    // set default values
    #if USEIMAGE
    if (!imagew || !imageh || !imageg) {
        imagew = imagewidth;
        imageh = imageheight;
        imagegaps = imagegaps;
    }
    #endif

	setupdisplay(); // set up display and create window
	eventloop(); // function is a loop which checks X11 events and calls other functions accordingly

	return 1; // should be unreachable
}
