/* See LICENSE file for copyright and license details. */
#include <ctype.h>
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <unistd.h>

/* check if we should enable right to left language support */
#ifdef NORTL
#define USERTL 0
#else
#define USERTL 1
#endif

/* check if we should enable image support */
#ifdef NOIMAGE
#define USEIMAGE 0
#else
#define USEIMAGE 1
#endif

#ifdef XINERAMA
#define USEXINERAMA 1
#else
#define USEXINERAMA 0
#endif

#if USERTL
#include <fribidi.h>
#endif

#if USEIMAGE
#include <errno.h>
#include <pwd.h>
#include <Imlib2.h>
#include <openssl/md5.h>
#endif

#if USEXINERAMA
#include <X11/extensions/Xinerama.h>
#endif

#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <X11/Xft/Xft.h>
#include <pango/pango.h>

#include "draw.h"
#include "main.h"

/* macros */
#define CLEANMASK(mask)         (mask & ~(numlockmask|LockMask) & (ShiftMask|ControlMask|Mod1Mask|Mod2Mask|Mod3Mask|Mod4Mask|Mod5Mask))
#define BUTTONMASK              (ButtonPressMask|ButtonReleaseMask)
#define INTERSECT(x,y,w,h,r)  (MAX(0, MIN((x)+(w),(r).x_org+(r).width)  - MAX((x),(r).x_org)) \
                             && MAX(0, MIN((y)+(h),(r).y_org+(r).height) - MAX((y),(r).y_org)))
#define LENGTH(X)             (sizeof X / sizeof X[0])
#define TEXTW(X)              (drw_font_getwidth(drw, (X), False) + lrpad)
#define TEXTWM(X)             (drw_font_getwidth(drw, (X), True) + lrpad)

/* number */
#define NUMBERSMAXDIGITS      100
#define NUMBERSBUFSIZE        (NUMBERSMAXDIGITS * 2) + 1

/* user friendly names for all the modifiers */
#define CONTROL ControlMask
#define SHIFT ShiftMask
#define ALT Mod1Mask
#define ALTR Mod3Mask
#define SUPER Mod4Mask
#define SUPERR Mod5Mask

/* alpha */
#define opaque 0xffU

/* include enums */
#include "libs/schemes.h"
#include "libs/arg.h"

static char text[BUFSIZ] = "";

struct item {
	char *text;
    char *image;
	struct item *left, *right;
	double distance;
};

typedef struct {
	unsigned int mod;
	KeySym keysym;
	void (*func)(const Arg *);
	const Arg arg;
} Key;

static char numbers[NUMBERSBUFSIZE] = "";
static char *embed;
static int numlockmask = 0;
static int bh, mw, mh;
static int reallines = 0;
static int reqlineheight; /* required menu height */
static int clineheight; /* menu height added through argument */
static int dmx = 0; /* put spmenu at this x offset */
static int dmy = 0; /* put spmenu at this y offset (measured from the bottom if menuposition is 0) */
static unsigned int dmw = 0; /* make spmenu this wide */
static int inputw = 0, promptw, passwd = 0;
static int lrpad; /* sum of left and right padding */
static int vp;    /* vertical padding for bar */
static int sp;    /* side padding for bar */
static size_t cursor;
static unsigned int selected = 0;
static struct item *items = NULL, *backup_items;
static struct item *matches, *matchend;
static struct item *prev, *curr, *next, *sel;
static int mon = -1, screen;
static int managed = 0;

#include "libs/mode.h"

static Atom clip, utf8, types, dock;
static Display *dpy;
static Window root, parentwin, win;
static XIC xic;

static int useargb = 0;
static Visual *visual;
static int depth;
static Colormap cmap;

static Drw *drw;
static Clr *scheme[SchemeLast];
static Clr textclrs[256];

static char *histfile;
static char **history;
static size_t histsz, histpos;

/* keybind functions */
static void move(const Arg *arg);
static void moveend(const Arg *arg);
static void movestart(const Arg *arg);
static void movenext(const Arg *arg);
static void moveprev(const Arg *arg);
static void paste(const Arg *arg);
static void restoresel(const Arg *arg);
static void clear(const Arg *arg);
static void viewhist(const Arg *arg);
static void moveword(const Arg *arg);
static void navhistory(const Arg *arg);
static void backspace(const Arg *arg);
static void selectitem(const Arg *arg);
static void quit(const Arg *arg);
static void complete(const Arg *arg);
static void savehistory(char *input);

static void drawmenu(void);
static void calcoffsets(void);

#include "libs/xrdb.h"

/* user configuration */
#include "options.h" /* Include main header */
#include "keybinds.h" /* Include keybinds */
#include "colors.h" /* Include colors */
#include "xresources.h" /* Include .Xresources */

static char * cistrstr(const char *s, const char *sub);
static int (*fstrncmp)(const char *, const char *, size_t) = strncasecmp;
static char *(*fstrstr)(const char *, const char *) = cistrstr;

#include "libs/xrdb.c"
#include "libs/mode.c"

#if USEIMAGE
static int longestedge = 0; /* longest edge */

#include "libs/img.h"
#include "libs/img.c"
#endif
#if USERTL
#include "libs/rtl.h"
#include "libs/rtl.c"
#endif

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
		for (item = matchend; item && item->left; item = item->left)
			numer++;
	}
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
	else
        /* hide match count */
        if (hidematchcount) {
		    n = mw - (promptw + inputw + TEXTW(leftarrow) + TEXTW(rightarrow) + TEXTW(modetext));
        } else {
            n = mw - (promptw + inputw + TEXTW(leftarrow) + TEXTW(rightarrow) + TEXTW(numbers) + TEXTW(modetext));
        }
	/* calculate which items will begin the next page and previous page */
	for (i = 0, next = curr; next; next = next->right)
		if ((i += (lines > 0) ? bh : MIN(TEXTWM(next->text), n)) > n)
			break;
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
    if (image) {
        imlib_free_image();
        image = NULL;
    }
    #endif
	XUngrabKey(dpy, AnyKey, AnyModifier, root);
	for (i = 0; i < SchemeLast; i++)
		free(scheme[i]);
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
		            tolower((unsigned char)h[i]); ++i)
			;
		if (n[i] == '\0')
			return (char *)h;
	}
	return NULL;
}

void
drawhighlights(struct item *item, int x, int y, int maxw)
{
	char restorechar, tokens[sizeof text], *highlight,  *token;
	int indentx, highlightlen;

	drw_setscheme(drw, scheme[item == sel ? SchemeSelHighlight : SchemeNormHighlight]);
	strcpy(tokens, text);
	for (token = strtok(tokens, " "); token; token = strtok(NULL, " ")) {
		highlight = fstrstr(item->text, token);
		while (highlight) {
			// Move item str end, calc width for highlight indent, & restore
			highlightlen = highlight - item->text;
			restorechar = *highlight;
			item->text[highlightlen] = '\0';
			indentx = TEXTW(item->text);
			item->text[highlightlen] = restorechar;

			// Move highlight str end, draw highlight, & restore
			restorechar = highlight[strlen(token)];
			highlight[strlen(token)] = '\0';
			if (indentx - (lrpad / 2) - 1 < maxw)
				drw_text(
					drw,
					x + indentx - (lrpad / 2) - 1,
					y,
					MIN(maxw - indentx, TEXTW(highlight) - lrpad),
					bh, 0, highlight, 0, True
				);
			highlight[strlen(token)] = restorechar;

			if (strlen(highlight) - strlen(token) < strlen(token)) break;
			highlight = fstrstr(highlight + strlen(token), token);
		}
	}
}

int
drawitem(struct item *item, int x, int y, int w)
{
    char buffer[sizeof(item->text) + lrpad / 2];
    Clr scm[3];
    int lp = lrpad / 2; /* padding */
    int wr, rd;
	int rw; /* width of text */
	int fg = 7;
	int bg = 0;
    int bgfg = 0;
    int ib = 0;

    if (item == sel) {
        memcpy(scm, scheme[SchemeItemSel], sizeof(scm));
    } else {
        memcpy(scm, scheme[SchemeItemNorm], sizeof(scm));
    }

    /* set scheme */
    drw_setscheme(drw, scm);

    /* parse item text */
	for (wr = 0, rd = 0; item->text[rd]; rd++) {
		if (item->text[rd] == '' && item->text[rd + 1] == '[') {
			size_t alen = strspn(item->text + rd + 2,
					     "0123456789;");
			if (item->text[rd + alen + 2] == 'm') { /* character is 'm' which is the last character in the sequence */
				buffer[wr] = '\0'; /* clear out character */

                /* draw text */
                rw = TEXTWM(buffer) - lrpad;
                #if USERTL
                apply_fribidi(buffer);
				drw_text(drw, x, y, rw + lp, bh, lp, fribidi_text, 0, pango_item ? True : False);
                #else
				drw_text(drw, x, y, rw + lp, bh, lp, buffer, 0, pango_item ? True : False);
                #endif

				x += rw + lp;
                ib = 1;
                lp = 0; /* no padding */

				char *ep = item->text + rd + 1;

                /* parse hex colors in scm */
				while (*ep != 'm') {
					unsigned v = strtoul(ep + 1, &ep, 10);
					if (v == 1) {
						fg |= 8;
						scm[0] = textclrs[fg];
					} else if (v == 22) {
						fg &= ~8;
						scm[0] = textclrs[fg];
					} else if (v >= 30 && v <= 37) {
						fg = v % 10 | (fg & 8);
						scm[0] = textclrs[fg];
                    } else if (v == 38) {
						bgfg = 2;
					} else if (v >= 40 && v <= 47) {
						bg = v % 10;
						scm[1] = textclrs[bg];
					} else if (v == 48) {
                        bgfg = 3;
                    }
				}

				rd += alen + 2;
				wr = 0;

				drw_setscheme(drw, scm);

				continue;
			}
		}

		buffer[wr++] = item->text[rd];
	}

	buffer[wr] = '\0';

    /* draw any text that doesn't use sgr sequences */
    #if USERTL
    apply_fribidi(buffer);
	int r = drw_text(drw, x, y, w - rw, bh, lp, fribidi_text, 0, pango_item ? True : False);
    #else
	int r = drw_text(drw, x, y, w - rw, bh, lp, buffer, 0, pango_item ? True : False);
    #endif

    if (!hidehighlight && !ib) drawhighlights(item, x, y, w - rw);
    return r;
}

char
sixd_to_8bit(int x)
{
	return x == 0 ? 0 : 0x37 + 0x28 * x;
}

void
drawmenu(void)
{
	unsigned int curpos;
	struct item *item;
	int x = 0, y = 0, fh = drw->font->h, w;
	char *censort;

	drw_setscheme(drw, scheme[SchemeMenu]);
	drw_rect(drw, 0, 0, mw, mh, 1, 1);

    if (hidemode)
        strcpy(modetext, "");

	if (prompt && *prompt) {
        if (colorprompt) {
		    drw_setscheme(drw, scheme[SchemePrompt]);
        }

		x = drw_text(drw, x, 0, promptw, bh, lrpad / 2, prompt, 0, pango_prompt ? True : False);
	}
	/* draw input field */
	w = (lines > 0 || !matches) ? mw - x : inputw;
	drw_setscheme(drw, scheme[SchemeInput]);
	if (passwd) {
	    censort = ecalloc(1, sizeof(text));
		memset(censort, '.', strlen(text));
        #if USERTL
        apply_fribidi(censort);
		drw_text(drw, x, 0, w, bh, lrpad / 2, fribidi_text, 0, pango_password ? True : False);
        #else
		drw_text(drw, x, 0, w - LENGTH(censort), bh, lrpad / 2, censort, 0, pango_password ? True : False);
        #endif
        free(censort);
	} else {
        #if USERTL
        apply_fribidi(text);
        drw_text(drw, x, 0, w, bh, lrpad / 2, fribidi_text, 0, pango_input ? True : False);
        #else
        drw_text(drw, x, 0, w, bh, lrpad / 2, text, 0, pango_input ? True : False);
        #endif
    }

	curpos = TEXTW(text) - TEXTW(&text[cursor]);
	if ((curpos += lrpad / 2 - 1) < w) {
		drw_setscheme(drw, scheme[SchemeCaret]);
		drw_rect(drw, x + curpos, 2 + (bh - fh) / 2, 2, fh - 4, 1, 0);
	}

    /* get match count */
    if (!hidematchcount) recalculatenumbers();

	if (lines > 0) {
        #if USEIMAGE
        if (imagewidth)
            x += imagegaps + imagewidth;
        #endif
		/* draw grid */
		int i = 0;
		for (item = curr; item != next; item = item->right, i++)
			drawitem(
				item,
				x + ((i / lines) *  ((mw - x) / columns)),
				y + (((i % lines) + 1) * bh),
				(mw - x) / columns
			);
	} else if (matches) {
		/* draw horizontal list */
		x += inputw;
		w = TEXTW(leftarrow);
		if (curr->left) {
			drw_setscheme(drw, scheme[SchemeLArrow]);
			drw_text(drw, x, 0, w, bh, lrpad / 2, leftarrow, 0, pango_leftarrow ? True : False);
		}
		x += w;
		for (item = curr; item != next; item = item->right)
            if (hidematchcount) {
                x = drawitem(item, x, 0, MIN(TEXTWM(item->text), mw - x - TEXTW(rightarrow) - TEXTW(modetext)));
            } else {
                x = drawitem(item, x, 0, MIN(TEXTWM(item->text), mw - x - TEXTW(rightarrow) - TEXTW(numbers) - TEXTW(modetext)));
            }
		if (next) {
			w = TEXTW(rightarrow);
			drw_setscheme(drw, scheme[SchemeRArrow]);

            if (hidematchcount) {
                drw_text(drw, mw - w - TEXTW(modetext), 0, w, bh, lrpad / 2, rightarrow, 0, pango_rightarrow ? True : False);
            } else {
                drw_text(drw, mw - w - TEXTW(numbers) - TEXTW(modetext), 0, w, bh, lrpad / 2, rightarrow, 0, pango_rightarrow ? True : False);
            }
		}
	}

    if (!hidematchcount) {
        drw_setscheme(drw, scheme[SchemeNumber]);
        drw_text(drw, mw - TEXTW(numbers) - TEXTW(modetext), 0, TEXTW(numbers), bh, lrpad / 2, numbers, 0, pango_numbers ? True : False);
    }

    if (!hidemode) {
        drw_setscheme(drw, scheme[SchemeMode]);
        drw_text(drw, mw - TEXTW(modetext), 0, TEXTW(modetext), bh, lrpad / 2, modetext, 0, pango_mode ? True : False);
    }

	drw_map(drw, win, 0, 0, mw, mh);
}

void
grabfocus(void)
{
	struct timespec ts = { .tv_sec = 0, .tv_nsec = 10000000  };
	Window focuswin;
	int i, revertwin;

	for (i = 0; i < 100; ++i) {
		XGetInputFocus(dpy, &focuswin, &revertwin);
		if (focuswin == win)
			return;
		if (managed) {
		XTextProperty prop;
		char *windowtitle = prompt != NULL ? prompt : "spmenu";
		Xutf8TextListToTextProperty(dpy, &windowtitle, 1, XUTF8StringStyle, &prop);
		XSetWMName(dpy, win, &prop);
		XSetTextProperty(dpy, win, &prop, XInternAtom(dpy, "_NET_WM_NAME", False));
		XFree(prop.value);
	} else {
		XSetInputFocus(dpy, win, RevertToParent, CurrentTime);
	}

		nanosleep(&ts, NULL);
	}
	die("cannot grab focus");
}

void
grabkeyboard(void)
{
	struct timespec ts = { .tv_sec = 0, .tv_nsec = 1000000  };
	int i;

	if (embed || managed)
		return;
	/* try to grab keyboard, we may have to wait for another process to ungrab */
	for (i = 0; i < 1000; i++) {
		if (XGrabKeyboard(dpy, DefaultRootWindow(dpy), True, GrabModeAsync,
		                  GrabModeAsync, CurrentTime) == GrabSuccess)
			return;
		nanosleep(&ts, NULL);
	}
	die("cannot grab keyboard");
}

int
compare_distance(const void *a, const void *b)
{
	struct item *da = *(struct item **) a;
	struct item *db = *(struct item **) b;

	if (!db)
		return 1;
	if (!da)
		return -1;

	return da->distance == db->distance ? 0 : da->distance < db->distance ? -1 : 1;
}

void
fuzzymatch(void)
{
	/* bang - we have so much memory */
	struct item *it;
	struct item **fuzzymatches = NULL;
	char c;
	int number_of_matches = 0, i, pidx, sidx, eidx;
	int text_len = strlen(text), itext_len;

	matches = matchend = NULL;

	/* walk through all items */
	for (it = items; it && it->text; it++) {
		if (text_len) {
			itext_len = strlen(it->text);
			pidx = 0; /* pointer */
			sidx = eidx = -1; /* start of match, end of match */
			/* walk through item text */
			for (i = 0; i < itext_len && (c = it->text[i]); i++) {
				/* fuzzy match pattern */
				if (!fstrncmp(&text[pidx], &c, 1)) {
					if(sidx == -1)
						sidx = i;
					pidx++;
					if (pidx == text_len) {
						eidx = i;
						break;
					}
				}
			}
			/* build list of matches */
			if (eidx != -1) {
				/* compute distance */
				/* add penalty if match starts late (log(sidx+2))
				 * add penalty for long a match without many matching characters */
				it->distance = log(sidx + 2) + (double)(eidx - sidx - text_len);
				/* fprintf(stderr, "distance %s %f\n", it->text, it->distance); */
				appenditem(it, &matches, &matchend);
				number_of_matches++;
			}
		} else {
			appenditem(it, &matches, &matchend);
		}
	}

	if (number_of_matches) {
		/* initialize array with matches */
		if (!(fuzzymatches = realloc(fuzzymatches, number_of_matches * sizeof(struct item*))))
			die("cannot realloc %u bytes:", number_of_matches * sizeof(struct item*));
		for (i = 0, it = matches; it && i < number_of_matches; i++, it = it->right) {
			fuzzymatches[i] = it;
		}
		/* sort matches according to distance */
		qsort(fuzzymatches, number_of_matches, sizeof(struct item*), compare_distance);
		/* rebuild list of matches */
		matches = matchend = NULL;
		for (i = 0, it = fuzzymatches[i];  i < number_of_matches && it && \
				it->text; i++, it = fuzzymatches[i]) {
			appenditem(it, &matches, &matchend);
		}
		free(fuzzymatches);
	}

	curr = sel = matches;

    for (i = 0; i < preselected; i++) {
		if (sel && sel->right && (sel = sel->right) == next) {
			curr = next;
			calcoffsets();
		}
	}

	calcoffsets();
}

void
match(void)
{
	if (fuzzy) {
		fuzzymatch();
		return;
	}
	static char **tokv = NULL;
	static int tokn = 0;

	char buf[sizeof text], *s;
	int i, tokc = 0;
	size_t len, textsize;
	struct item *item, *lprefix, *lsubstr, *prefixend, *substrend;

	strcpy(buf, text);
	/* separate input text into tokens to be matched individually */
	for (s = strtok(buf, " "); s; tokv[tokc - 1] = s, s = strtok(NULL, " "))
		if (++tokc > tokn && !(tokv = realloc(tokv, ++tokn * sizeof *tokv)))
			die("cannot realloc %u bytes:", tokn * sizeof *tokv);
	len = tokc ? strlen(tokv[0]) : 0;

	matches = lprefix = lsubstr = matchend = prefixend = substrend = NULL;
	textsize = strlen(text) + 1;
	for (item = items; item && item->text; item++) {
		for (i = 0; i < tokc; i++)
			if (!fstrstr(item->text, tokv[i]))
				break;
		if (i != tokc) /* not all tokens match */
			continue;
		/* exact matches go first, then prefixes, then substrings */
		if (!tokc || !fstrncmp(text, item->text, textsize))
			appenditem(item, &matches, &matchend);
		else if (!fstrncmp(tokv[0], item->text, len))
			appenditem(item, &lprefix, &prefixend);
		else
			appenditem(item, &lsubstr, &substrend);
	}
	if (lprefix) {
		if (matches) {
			matchend->right = lprefix;
			lprefix->left = matchend;
		} else
			matches = lprefix;
		matchend = prefixend;
	}
	if (lsubstr) {
		if (matches) {
			matchend->right = lsubstr;
			lsubstr->left = matchend;
		} else
			matches = lsubstr;
		matchend = substrend;
	}
	curr = sel = matches;
	calcoffsets();
}

void
insert(const char *str, ssize_t n)
{
	if (strlen(text) + n > sizeof text - 1)
		return;
	/* move existing text out of the way, insert new text, and update cursor */
	memmove(&text[cursor + n], &text[cursor], sizeof text - cursor - MAX(n, 0));
	if (n > 0)
		memcpy(&text[cursor], str, n);
	cursor += n;
	match();
}

size_t
nextrune(int inc)
{
	ssize_t n;

	/* return location of next utf8 rune in the given direction (+1 or -1) */
	for (n = cursor + inc; n + inc >= 0 && (text[n] & 0xc0) == 0x80; n += inc)
		;
	return n;
}

void
movewordedge(int dir)
{
	if (dir < 0) { /* move cursor to the start of the word*/
		while (cursor > 0 && strchr(worddelimiters, text[nextrune(-1)]))
			cursor = nextrune(-1);
		while (cursor > 0 && !strchr(worddelimiters, text[nextrune(-1)]))
			cursor = nextrune(-1);
	} else { /* move cursor to the end of the word */
		while (text[cursor] && strchr(worddelimiters, text[cursor]))
			cursor = nextrune(+1);
		while (text[cursor] && !strchr(worddelimiters, text[cursor]))
			cursor = nextrune(+1);
	}
}

void
loadhistory(void)
{
	FILE *fp = NULL;
	static size_t cap = 0;
	size_t llen;
	char *line;

	if (!histfile) {
		return;
	}

	fp = fopen(histfile, "r");
	if (!fp) {
		return;
	}

	for (;;) {
		line = NULL;
		llen = 0;
		if (-1 == getline(&line, &llen, fp)) {
			if (ferror(fp)) {
				die("failed to read history");
			}
			free(line);
			break;
		}

		if (cap == histsz) {
			cap += 64 * sizeof(char*);
			history = realloc(history, cap);
			if (!history) {
				die("failed to realloc memory");
			}
		}
		strtok(line, "\n");
		history[histsz] = line;
		histsz++;
	}
	histpos = histsz;

	if (fclose(fp)) {
		die("failed to close file %s", histfile);
	}
}

void
navighistory(int dir)
{
	static char def[BUFSIZ];
	char *p = NULL;
	size_t len = 0;

	if (!history || histpos + 1 == 0)
		return;

	if (histsz == histpos) {
		strncpy(def, text, sizeof(def));
	}

	switch(dir) {
	case 1:
		if (histpos < histsz - 1) {
			p = history[++histpos];
		} else if (histpos == histsz - 1) {
			p = def;
			histpos++;
		}
		break;
	case -1:
		if (histpos > 0) {
			p = history[--histpos];
		}
		break;
	}
	if (p == NULL) {
		return;
	}

	len = MIN(strlen(p), BUFSIZ - 1);
	strncpy(text, p, len);
	text[len] = '\0';
	cursor = len;
	match();
}

void
paste(const Arg *arg)
{
    int clipboard;

    if (arg->i == 1) {
        clipboard = XA_PRIMARY;
    } else {
        clipboard = clip;
    }

	XConvertSelection(dpy, clipboard, utf8, utf8, win, CurrentTime);
	return;

}

void
quit(const Arg *arg)
{
	cleanup();
	exit(1);
}

void
viewhist(const Arg *arg)
{
    int i;

    if (histfile) {
		if (!backup_items) {
			backup_items = items;
			items = calloc(histsz + 1, sizeof(struct item));

			if (!items) {
				die("spmenu: cannot allocate memory");
			}

			for (i = 0; i < histsz; i++) {
				items[i].text = history[i];
			}
		} else {
			free(items);
			items = backup_items;
			backup_items = NULL;
		}
	}

	match();
    drawmenu();
}

void
moveword(const Arg *arg)
{
    movewordedge(arg->i);
    drawmenu();
}

void
backspace(const Arg *arg)
{
    if (cursor == 0)
        return;

    insert(NULL, nextrune(-1) - cursor);
    drawmenu();
}

void
selectitem(const Arg *arg)
{
    char *selection;

    if (sel) {
        selection = sel->text;
    } else {
        selection = text;
    }

    if (!selection)
        return;

    puts(selection);
    savehistory(selection);

	cleanup();
	exit(0);
}

void
movenext(const Arg *arg)
{
	if (!next)
		return;

	sel = curr = next;
	calcoffsets();
    drawmenu();
}

void
moveprev(const Arg *arg)
{
    if (!prev)
        return;

    sel = curr = prev;
    calcoffsets();
    drawmenu();
}

void
movestart(const Arg *arg)
{
  	if (sel == matches) {
		cursor = 0;
        drawmenu();
        return;
	}

	sel = curr = matches;
	calcoffsets();
    drawmenu();
}

void
moveend(const Arg *arg)
{
    if (text[cursor] != '\0') {
		cursor = strlen(text);
        drawmenu();
        return;
	}

	if (next) {
		curr = matchend;
		calcoffsets();
		curr = prev;
		calcoffsets();

		while (next && (curr = curr->right))
			calcoffsets();
	}

	sel = matchend;
    drawmenu();
}

void
navhistory(const Arg *arg)
{
    navighistory(arg->i);
    drawmenu();
}

void
restoresel(const Arg *arg)
{
    text[cursor] = '\0';
    match();
    drawmenu();
}

void
clear(const Arg *arg)
{
    insert(NULL, 0 - cursor);
    drawmenu();
}

void
complete(const Arg *arg)
{
 	if (!sel)
		return;

	strncpy(text, sel->text, sizeof text - 1);
	text[sizeof text - 1] = '\0';
	cursor = strlen(text);
	match();
    drawmenu();
}

void
move(const Arg *arg)
{
    struct item *tmpsel;
    int i, offscreen = 0;

    if (arg->i == 3) { /* left */
    	if (columns > 1) {
			if (!sel)
				return;
			tmpsel = sel;
			for (i = 0; i < lines; i++) {
				if (!tmpsel->left || tmpsel->left->right != tmpsel) {
					if (offscreen)
						drawmenu();
					return;
				}
				if (tmpsel == curr)
					offscreen = 1;
				tmpsel = tmpsel->left;
			}
			sel = tmpsel;
			if (offscreen) {
				curr = prev;
				calcoffsets();
			}
			drawmenu();
		}

		if (cursor > 0 && (!sel || !sel->left || lines > 0)) {
			cursor = nextrune(-1);
			drawmenu();
		}
		if (lines > 0)
			return;
    } else if (arg->i == 4) {
        	if (columns > 1) {
			if (!sel)
				return;
			tmpsel = sel;
			for (i = 0; i < lines; i++) {
				if (!tmpsel->right ||  tmpsel->right->left != tmpsel) {
					if (offscreen)
						drawmenu();
					return;
				}
				tmpsel = tmpsel->right;
				if (tmpsel == next)
					offscreen = 1;
			}
			sel = tmpsel;
			if (offscreen) {
				curr = next;
				calcoffsets();
			}
			drawmenu();
		}

		if (text[cursor] != '\0') {
			cursor = nextrune(+1);
			drawmenu();
		}

		if (lines > 0)
			return;
    } else if (arg->i == 2) {
       	if (sel && sel->right && (sel = sel->right) == next) {
			curr = next;
			calcoffsets();
		}
		drawmenu();
    } else if (arg->i == 1) {
        if (sel && sel->left && (sel = sel->left)->right == curr) {
			curr = prev;
			calcoffsets();
		}
        drawmenu();
    }
}

void
updatenumlockmask(void)
{
	unsigned int i, j;
	XModifierKeymap *modmap;

	numlockmask = 0;
	modmap = XGetModifierMapping(dpy);
	for (i = 0; i < 8; i++)
		for (j = 0; j < modmap->max_keypermod; j++)
			if (modmap->modifiermap[i * modmap->max_keypermod + j]
				== XKeysymToKeycode(dpy, XK_Num_Lock))
				numlockmask = (1 << i);
	XFreeModifiermap(modmap);
}

void
keypress(XEvent *e)
{
   	updatenumlockmask();
    {
        unsigned int i;
        KeySym keysym;
        XKeyEvent *ev;
        char buf[64];
        char keyArray;
        KeySym ksym = NoSymbol;
        Status status;

        int len = 0;
        ev = &e->xkey;
        len = XmbLookupString(xic, ev, buf, sizeof buf, &ksym, &status);

        keysym = XkbKeycodeToKeysym(dpy, (KeyCode)ev->keycode, 0, 0);

        if (selkeys) {
            for (i = 0; i < LENGTH(inskeys); i++) {
                if (keysym == inskeys[i].keysym && CLEANMASK(inskeys[i].mod) == CLEANMASK(ev->state) && inskeys[i].func)
                    inskeys[i].func(&(inskeys[i].arg));
            }
        } else {
            for (i = 0; i < LENGTH(normkeys); i++) {
                if (keysym == normkeys[i].keysym && CLEANMASK(normkeys[i].mod) == CLEANMASK(ev->state) && normkeys[i].func)
                    normkeys[i].func(&(normkeys[i].arg));
            }
        }

        if (!iscntrl(*buf) && type && selkeys ) {

            if (allowkeys) {
                insert(buf, len);
            } else {
                allowkeys = !allowkeys;
            }

            drawmenu();
        }
    }
}

void
savehistory(char *input)
{
	unsigned int i;
	FILE *fp;

	if (!histfile ||
	    0 == maxhist ||
	    0 == strlen(input)) {
		goto out;
	}

	fp = fopen(histfile, "w");
	if (!fp) {
		die("failed to open %s", histfile);
	}
	for (i = histsz < maxhist ? 0 : histsz - maxhist; i < histsz; i++) {
		if (0 >= fprintf(fp, "%s\n", history[i])) {
			die("failed to write to %s", histfile);
		}
	}
	if (histsz == 0 || !histnodup || (histsz > 0 && strcmp(input, history[histsz-1]) != 0)) { /* TODO */
		if (0 >= fputs(input, fp)) {
			die("failed to write to %s", histfile);
		}
	}
	if (fclose(fp)) {
		die("failed to close file %s", histfile);
	}

out:
	for (i = 0; i < histsz; i++) {
		free(history[i]);
	}
	free(history);
}

void
buttonpress(XEvent *e)
{
	struct item *item;
	XButtonPressedEvent *ev = &e->xbutton;
	int x = 0, y = 0, h = bh, w, item_num = 0;

	if (ev->window != win)
		return;

	/* right-click: exit */
	if (ev->button == Button3)
		exit(1);

	if (prompt && *prompt)
		x += promptw;

	/* input field */
	w = (lines > 0 || !matches) ? mw - x : inputw;

	/* left-click on input: clear input,
	 * NOTE: if there is no left-arrow the space for < is reserved so
	 *       add that to the input width */
	if (ev->button == Button1 &&
	   ((lines <= 0 && ev->x >= 0 && ev->x <= x + w +
	   ((!prev || !curr->left) ? TEXTW(leftarrow) : 0)) ||
	   (lines > 0 && ev->y >= y && ev->y <= y + h))) {
		insert(NULL, -cursor);
		drawmenu();
		return;
	}
	/* middle-mouse click: paste selection */
	if (ev->button == Button2) {
		XConvertSelection(dpy, (ev->state & ShiftMask) ? clip : XA_PRIMARY,
		                  utf8, utf8, win, CurrentTime);
		drawmenu();
		return;
	}
	/* scroll up */
	if (ev->button == Button4 && prev) {
		sel = curr = prev;
		calcoffsets();
		drawmenu();
		return;
	}
	/* scroll down */
	if (ev->button == Button5 && next) {
		sel = curr = next;
		calcoffsets();
		drawmenu();
		return;
	}
	if (ev->button != Button1)
		return;
	if (ev->state & ~ControlMask)
		return;
	if (lines > 0) {
		/* vertical list: (ctrl)left-click on item */
		w = mw - x;
		for (item = curr; item != next; item = item->right) {
			if (item_num++ == lines){
				item_num = 1;
				x += w / columns;
				y = 0;
			}
			y += h;
			if (ev->y >= y && ev->y <= (y + h) &&
			    ev->x >= x && ev->x <= (x + w / columns)) {
				puts(item->text);
				if (!(ev->state & ControlMask))
					exit(0);
				sel = item;
				if (sel) {
					drawmenu();
				}
				return;
			}
		}
	} else if (matches) {
		/* left-click on left arrow */
		x += inputw;
		w = TEXTW(leftarrow);
		if (prev && curr->left) {
			if (ev->x >= x && ev->x <= x + w) {
				sel = curr = prev;
				calcoffsets();
				drawmenu();
				return;
			}
		}
		/* horizontal list: (ctrl)left-click on item */
		for (item = curr; item != next; item = item->right) {
			x += w;
			w = MIN(TEXTW(item->text), mw - x - TEXTW(rightarrow));
			if (ev->x >= x && ev->x <= x + w) {
				puts(item->text);
				if (!(ev->state & ControlMask))
					exit(0);
				sel = item;
				if (sel) {
					drawmenu();
				}
				return;
			}
		}
		/* left-click on right arrow */
		w = TEXTW(rightarrow);
		x = mw - w;
		if (next && ev->x >= x && ev->x <= x + w) {
			sel = curr = next;
			calcoffsets();
			drawmenu();
			return;
		}
	}
}

void
pastesel(void)
{
	char *p, *q;
	int di;
	unsigned long dl;
	Atom da;

	/* we have been given the current selection, now insert it into input */
	if (XGetWindowProperty(dpy, win, utf8, 0, (sizeof text / 4) + 1, False,
	                   utf8, &da, &di, &dl, &dl, (unsigned char **)&p)
	    == Success && p) {
		insert(p, (q = strchr(p, '\n')) ? q - p : (ssize_t)strlen(p));
		XFree(p);
	}
	drawmenu();
}

void
xinitvisual()
{
	XVisualInfo *infos;
	XRenderPictFormat *fmt;
	int nitems;
	int i;

	XVisualInfo tpl = {
		.screen = screen,
		.depth = 32,
		.class = TrueColor
	};
	long masks = VisualScreenMask | VisualDepthMask | VisualClassMask;

	infos = XGetVisualInfo(dpy, masks, &tpl, &nitems);
	visual = NULL;
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

	if (!visual || !alpha) {
		visual = DefaultVisual(dpy, screen);
		depth = DefaultDepth(dpy, screen);
		cmap = DefaultColormap(dpy, screen);
	}
}

void
readstdin(void)
{
	char buf[sizeof text], *p;
	size_t i, imax = 0, itemsiz = 0;
	unsigned int tmpmax = 0;
    int w, h;
    char *limg = NULL;

	if (passwd){
    	inputw = lines = 0;
    	return;
  	}


	/* read each line from stdin and add it to the item list */
	for (i = 0; fgets(buf, sizeof buf, stdin); i++) {
      	if (i + 1 >= itemsiz) {
			itemsiz += 256;
			if (!(items = realloc(items, itemsiz * sizeof(*items))))
				die("cannot realloc %zu bytes:", itemsiz * sizeof(*items));
		}
		if ((p = strchr(buf, '\n')))
			*p = '\0';
		if (!(items[i].text = strdup(buf)))
			die("cannot strdup %u bytes:", strlen(buf) + 1);
		drw_font_getexts(drw->font, buf, strlen(buf), &tmpmax, NULL, True);
		if (tmpmax > inputw) {
			inputw = tmpmax;
			imax = i;
		}

#if USEIMAGE
        if(!strncmp("IMG:", items[i].text, strlen("IMG:"))) {
            if(!(items[i].image = malloc(strlen(items[i].text)+1)))
                fprintf(stderr, "spmenu: cannot malloc %lu bytes\n", strlen(items[i].text));
            if(sscanf(items[i].text, "IMG:%[^\t]", items[i].image)) {
                if(!(items[i].image = realloc(items[i].image, strlen(items[i].image)+1)))
                    fprintf(stderr, "spmenu: cannot realloc %lu bytes\n", strlen(items[i].image)+1);
                items[i].text += strlen("IMG:")+strlen(items[i].image)+1;
            } else {
                free(items[i].image);
                items[i].image = NULL;
            }
        } else {
            items[i].image = NULL;
        }

        if (generatecache && longestedge <= 256 && items[i].image && strcmp(items[i].image, limg?limg:"")) {
            loadimagecache(items[i].image, &w, &h);
            fprintf(stdout, "-!- Generating thumbnail for: %s\n", items[i].image);
        }

        if(items[i].image)
            limg = items[i].image;
#endif
	}

	if (items) {
#if USEIMAGE
        items[i].image = NULL;
#endif
		items[i].text = NULL;
    }
#if USEIMAGE
    if (!limg)
        longestedge = imagegaps = 0;
#endif
	inputw = items ? TEXTWM(items[imax].text) : 0;
	lines = MIN(lines, i);
}

void
run(void)
{
	XEvent ev;
    #if USEIMAGE
    int width = 0, height = 0;
	char *limg = NULL;
    #endif

	while (!XNextEvent(dpy, &ev)) {
		if (XFilterEvent(&ev, win))
			continue;
		switch(ev.type) {
		case DestroyNotify:
			if (ev.xdestroywindow.window != win)
				break;
			cleanup();
			exit(1);
		case ButtonPress:
			buttonpress(&ev);
			break;
		case Expose:
			if (ev.xexpose.count == 0)
				drw_map(drw, win, 0, 0, mw, mh);
			break;
		case FocusIn:
			/* regrab focus from parent window */
			if (ev.xfocus.window != win)
				grabfocus();
			break;
		case KeyPress:
			keypress(&ev);
			break;
		case SelectionNotify:
			if (ev.xselection.property == utf8)
				pastesel();
			break;
		case VisibilityNotify:
			if (ev.xvisibility.state != VisibilityUnobscured)
				XRaiseWindow(dpy, win);
			break;
		}

#if USEIMAGE
        if (!lines)
            continue;

        if (sel && sel->image && strcmp(sel->image, limg ? limg : "")) {
            if (longestedge)
                loadimagecache(sel->image, &width, &height);
        } else if ((!sel || !sel->image) && image) {
            imlib_free_image();
            image = NULL;
        } if (image && longestedge) {
            int leftmargin = imagegaps;

           	if(mh != bh + height + imagegaps * 2) {
			    resizetoimageheight(height);
		    }

            if (!imageposition) { /* top mode = 0 */
                if (height > width)
                    width = height;
                imlib_render_image_on_drawable(leftmargin+(imagewidth-width)/2, bh+imagegaps);
            } else if (imageposition == 1) { /* bottom mode = 1 */
                if (height > width)
                    width = height;
                imlib_render_image_on_drawable(leftmargin+(imagewidth-width)/2, mh-height-imagegaps);
            } else if (imageposition == 2) { /* center mode = 2 */
                imlib_render_image_on_drawable(leftmargin+(imagewidth-width)/2, (mh-bh-height)/2+bh);
            } else {
                int minh = MIN(height, mh-bh-imagegaps*2);
                imlib_render_image_on_drawable(leftmargin+(imagewidth-width)/2, (minh-height)/2+bh+imagegaps);
            }


        } if (sel) {
            limg = sel->image;
        } else {
            limg = NULL;
        }
#endif
	}
}

void
setup(void)
{
	int x, y, i, j;
	unsigned int du;
    unsigned int tmp, minstrlen = 0, curstrlen = 0;
    int numwidthchecks = 100;
    struct item *item;
	XSetWindowAttributes swa;
	XIM xim;
	Window w, dw, *dws;
	XWindowAttributes wa;
	XClassHint ch = { class, class };
    #if USEXINERAMA
	XineramaScreenInfo *info;
	Window pw;
	int a, di, n, area = 0;
    #endif
    char cbuf[8];
	/* init appearance */
	for (j = 0; j < SchemeLast; j++) {
		scheme[j] = drw_scm_create(drw, colors[j], alphas[j], 2);
    }

    for (i = 0; i < LENGTH(textcolors) && i < LENGTH(textclrs); i++)
 		drw_clr_create(drw, &textclrs[i], textcolors[i], 0);
	if (i == 0 && colorsupport)
		drw_clr_create(drw, &textclrs[i++], "#000000", 0);
	for (; i < 7; i++) {
        if (!colorsupport)
            break;
		snprintf(cbuf, sizeof(cbuf), "#%02x%02x%02x",
			 !!(i & 1) * 0x7f,
			 !!(i & 2) * 0x7f,
			 !!(i & 4) * 0x7f);
		drw_clr_create(drw, &textclrs[i], cbuf, 0);
	}
	if (i == 7 && colorsupport)
		drw_clr_create(drw, &textclrs[i++], "#000000", 0);
	if (i == 8 && colorsupport)
		drw_clr_create(drw, &textclrs[i++], "#333333", 0);
	for (; i < 16; i++) {
        if (!colorsupport)
            break;
		snprintf(cbuf, sizeof(cbuf), "#%02x%02x%02x",
			 !!(i & 1) * 0xff,
			 !!(i & 2) * 0xff,
			 !!(i & 4) * 0xff);
		drw_clr_create(drw, &textclrs[i], cbuf, 0);
	}
	for (; i < 6 * 6 * 6 + 16; i++) {
        if (!colorsupport)
            break;
		snprintf(cbuf, sizeof(cbuf), "#%02x%02x%02x",
			 sixd_to_8bit(((i - 16) / 36) % 6),
			 sixd_to_8bit(((i - 16) / 6) % 6),
			 sixd_to_8bit(((i - 16)) % 6));
		drw_clr_create(drw, &textclrs[i], cbuf, 0);
	}
	for (; i < 256; i++) {
        if (!colorsupport)
            break;
		snprintf(cbuf, sizeof(cbuf), "#%02x%02x%02x",
			 0x08 + (i - 6 * 6 * 6 - 16) * 0x0a,
			 0x08 + (i - 6 * 6 * 6 - 16) * 0x0a,
			 0x08 + (i - 6 * 6 * 6 - 16) * 0x0a);
		drw_clr_create(drw, &textclrs[i], cbuf, 0);
	}

	clip = XInternAtom(dpy, "CLIPBOARD",   False);
	utf8 = XInternAtom(dpy, "UTF8_STRING", False);
   	types = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", False);
	dock = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DOCK", False);

    if (!clineheight) {
        reqlineheight += lineheight;
    } else {
        reqlineheight = clineheight;
    }

	/* calculate menu geometry */
	bh = drw->font->h + 2 + reqlineheight;
	lines = MAX(lines, 0);
    reallines = lines;
#if USEIMAGE
    if (image)
        resizetoimageheight(imageheight);
#endif
	mh = (lines + 1) * bh;
	promptw = (prompt && *prompt) ? TEXTWM(prompt) - lrpad / 4 : 0;

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
    #if USEXINERAMA
	i = 0;
	if (parentwin == root && (info = XineramaQueryScreens(dpy, &n))) {
		XGetInputFocus(dpy, &w, &di);
		if (mon >= 0 && mon < n)
			i = mon;
		else if (w != root && w != PointerRoot && w != None) {
			/* find top-level window containing current input focus */
			do {
				if (XQueryTree(dpy, (pw = w), &dw, &w, &dws, &du) && dws)
					XFree(dws);
			} while (w != root && w != pw);
			/* find xinerama screen with which the window intersects most */
			if (XGetWindowAttributes(dpy, pw, &wa))
				for (j = 0; j < n; j++)
					if ((a = INTERSECT(wa.x, wa.y, wa.width, wa.height, info[j])) > area) {
						area = a;
						i = j;
					}
		}
		/* no focused window is on screen, so use pointer location instead */
		if (mon < 0 && !area && XQueryPointer(dpy, root, &dw, &dw, &x, &y, &di, &di, &du))
			for (i = 0; i < n; i++)
				if (INTERSECT(x, y, 1, 1, info[i]))
					break;

		if (centered) {
			mw = MIN(MAX(max_textw() + promptw, minwidth), info[i].width);
			x = info[i].x_org + ((info[i].width  - mw) / 2);
			//y = info[i].y_org + 0;
			y = info[i].y_org + ((info[i].height - mh) / 2);
		} else {
		    x = info[i].x_org + dmx;
			y = info[i].y_org + (menuposition ? 0 : info[i].height - mh - dmy);
			//y = info[i].y_org + 0;
			mw = (dmw>0 ? dmw : info[i].width);
		}

		XFree(info);
	} else
    #endif
	{
		if (!XGetWindowAttributes(dpy, parentwin, &wa))
			die("could not get embedding window attributes: 0x%lx",
			    parentwin);

		if (centered) {
			mw = MIN(MAX(max_textw() + promptw, minwidth), wa.width);
			x = (wa.width  - mw) / 2;
			y = (wa.height - mh) / 2;
		} else {
			x = 0;
			y = 0;
			mw = wa.width;
		}
	}

	/* might be faster in some instances, most of the time unnecessary */
    if (!accuratewidth) inputw = MIN(inputw, mw/3);

	match();

	/* create menu window */
	swa.override_redirect = managed ? False : True;
	swa.background_pixel = 0;
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | VisibilityChangeMask | ButtonPressMask | PointerMotionMask;

	if (!bordercentered) {
	win = XCreateWindow(dpy, parentwin, x + sp, y + vp, mw - 2 * sp, mh, borderwidth,
	                    depth, InputOutput, visual,
						CWOverrideRedirect|CWBackPixel|CWBorderPixel|CWColormap|CWEventMask, &swa);
    } else {
		if (!centered) {
				win = XCreateWindow(dpy, parentwin, x + sp, y + vp, mw - 2 * sp, mh, 0,
						depth, InputOutput, visual,
						CWOverrideRedirect|CWBackPixel|CWBorderPixel|CWColormap|CWEventMask, &swa);
		} else {
				win = XCreateWindow(dpy, parentwin, x + sp, y + vp, mw - 2 * sp, mh, borderwidth,
	                    depth, InputOutput, visual,
						CWOverrideRedirect|CWBackPixel|CWBorderPixel|CWColormap|CWEventMask, &swa);
				}

    }
    XSetWindowBorder(dpy, win, scheme[SchemeBorder][ColBg].pixel);
	XSetClassHint(dpy, win, &ch);
   	XChangeProperty(dpy, win, types, XA_ATOM, 32, PropModeReplace, (unsigned char *) &dock, 1);

    #if USEIMAGE
    imlib_set_cache_size(8192 * 1024);
	imlib_context_set_blend(1);
	imlib_context_set_dither(1);
	imlib_set_color_usage(128);
	imlib_context_set_display(dpy);
	imlib_context_set_visual(visual);
	imlib_context_set_colormap(cmap);
	imlib_context_set_drawable(win);
    #endif

	/* input methods */
	if ((xim = XOpenIM(dpy, NULL, NULL, NULL)) == NULL)
		die("XOpenIM failed: could not open input device");

	xic = XCreateIC(xim, XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
	                XNClientWindow, win, XNFocusWindow, win, NULL);

	XMapRaised(dpy, win);
	if (embed) {
		XSelectInput(dpy, parentwin, FocusChangeMask | SubstructureNotifyMask);
		if (XQueryTree(dpy, parentwin, &dw, &w, &dws, &du) && dws) {
			for (i = 0; i < du && dws[i] != win; ++i)
				XSelectInput(dpy, dws[i], FocusChangeMask);
			XFree(dws);
		}
		grabfocus();
	}
	drw_resize(drw, mw, mh);
	drawmenu();
}

void
usage(void)
{
	fputs("spmenu: dynamic menu\n\n"
		  "- Arguments -\n"
		  "spmenu -l <line>      Set line count to stdin\n"
		  "spmenu -h <height>    Set spmenu line height to <height>\n"
		  "spmenu -g <grid>      Set the number of grids to <grid>\n"
          "spmenu -gc            Generate image cache\n"
          "spmenu -rw            Enable relative input width\n"
          "spmenu -nrw           Disable relative input width\n"
          "spmenu -f             Grabs keyboard before reading stdin\n"
		  "spmenu -F             Enable fuzzy matching\n"
		  "spmenu -P             Hide characters\n"
		  "spmenu -p <text>      Set spmenu prompt text to <text>\n"
          "spmenu -a             Enable alpha\n"
          "spmenu -na            Disable alpha\n"
          "spmenu -cp            Color prompt\n"
          "spmenu -ncp           Don't color prompt\n"
          "spmenu -tp            Allow the user to type\n"
          "spmenu -nt            Don't allow typing, the user must select an option\n"
		  "spmenu -x <x offset>  Offset spmenu x position by <x offset>\n"
		  "spmenu -y <y offset>  Offset spmenu y position by <y offset>\n"
          "spmenu -n <line>      Preselect <line> in the list of items\n"
		  "spmenu -z <width>     Width of the spmenu window\n"
          "spmenu -nmt <text>    Set normal mode text to <text>\n"
          "spmenu -imt <text>    Set insert mode text to <text>\n"
		  "spmenu -bw            Width of the border. 0 will disable the border\n"
		  "spmenu -s             Use case-sensitive matching\n"
		  "spmenu -i             Use case-insensitive matching\n"
          "spmenu -nm            Start spmenu in normal mode\n"
          "spmenu -im            Start spmenu in insert mode\n"
		  "spmenu -t             Position spmenu at the top of the screen\n"
		  "spmenu -b             Position spmenu at the bottom of the screen\n"
		  "spmenu -c             Position spmenu at the center of the screen\n"
          "spmenu -hm            Hide mode indicator\n"
          "spmenu -hmc           Hide match count\n"
          "spmenu -sm            Show mode indicator\n"
          "spmenu -smc           Show match count\n"
          "spmenu -xrdb          Load .Xresources on runtime\n"
          "spmenu -nxrdb         Don't load .Xresources on runtime\n"
		  "spmenu -m <monitor>   Specify a monitor to run spmenu on\n"
		  "spmenu -w <window id> Embed spmenu inside <window id>\n"
		  "spmenu -H <hist file> Specify a path to save the history to\n"
          "spmenu -ig <gaps>     Set image gaps to <gaps>\n"
		  "spmenu -lp <padding>  Set the vertical padding\n"
		  "spmenu -hp <padding>  Set the horizontal padding\n"
          "spmenu -la <symbol>   Set the left arrow to <symbol>\n"
          "spmenu -ra <symbol>   Set the right arrow to <symbol>\n"
          "spmenu -is <size>     Image size\n"
          "spmenu -it            Position the image at the top\n"
          "spmenu -ib            Position the image at the bottom\n"
          "spmenu -ic            Position the image in the center\n"
          "spmenu -itc           Position the image in the top center\n"
          "spmenu -wm            Spawn spmenu as a window manager controlled client/window. Useful for testing\n"
          "spmenu -v             Print spmenu version to stdout\n"
          "\n", stdout);
          fputs("- Appearance arguments -\n"
		  "spmenu -fn  <font>    Set the spmenu font to <font>\n"
          "spmenu -nif <color>   Set the normal item foreground color\n"
          "spmenu -nib <color>   Set the normal item background color\n"
          "spmenu -sif <color>   Set the selected item foreground color\n"
          "spmenu -sib <color>   Set the selected item background color\n"
          "spmenu -pfg <color>   Set the prompt foreground color\n"
          "spmenu -pbg <color>   Set the prompt background color\n"
          "spmenu -ifg <color>   Set input foreground color\n"
          "spmenu -ibg <color>   Set input background color\n"
          "spmenu -mbg <color>   Set the menu background color\n"
		  "spmenu -nhf <color>   Set the normal highlight foreground color\n"
		  "spmenu -nhb <color>   Set the normal highlight background color\n"
		  "spmenu -shf <color>   Set the selected highlight foreground color\n"
		  "spmenu -shb <color>   Set the selected highlight background color\n"
		  "spmenu -shb <color>   Set the selected highlight background color\n"
          "spmenu -nfg <color>   Set the foreground color for the match count\n"
          "spmenu -nbg <color>   Set the background color for the match count\n"
          "spmenu -mfg <color>   Set the foreground color for the mode indicator\n"
          "spmenu -mbg <color>   Set the background color for the mode indicator\n"
          "spmenu -laf <color>   Set the left arrow foreground color\n"
          "spmenu -raf <color>   Set the right arrow foreground color\n"
          "spmenu -lab <color>   Set the left arrow background color\n"
          "spmenu -rab <color>   Set the right arrow background color\n"
          "spmenu -cc  <color>   Set the caret color\n"
          "spmenu -bc  <color>   Set the border color\n"
		  "spmenu -sgr0          Set the SGR 0 color\n"
		  "spmenu -sgr1          Set the SGR 1 color\n"
		  "spmenu -sgr2          Set the SGR 2 color\n"
		  "spmenu -sgr3          Set the SGR 3 color\n"
		  "spmenu -sgr4          Set the SGR 4 color\n"
		  "spmenu -sgr5          Set the SGR 5 color\n"
		  "spmenu -sgr6          Set the SGR 6 color\n"
		  "spmenu -sgr7          Set the SGR 7 color\n"
		  "spmenu -sgr8          Set the SGR 8 color\n"
		  "spmenu -sgr9          Set the SGR 9 color\n"
		  "spmenu -sgr10         Set the SGR 10 color\n"
		  "spmenu -sgr11         Set the SGR 11 color\n"
		  "spmenu -sgr12         Set the SGR 12 color\n"
          "spmenu -sgr13         Set the SGR 13 color\n"
		  "spmenu -sgr14         Set the SGR 14 color\n"
		  "spmenu -sgr15         Set the SGR 15 color\n"
          "\n", stdout);
          fputs("- dmenu compatibility -\n"
	      "spmenu -nb <color>    Set the normal background color\n"
		  "spmenu -nf <color>    Set the normal foreground color\n"
		  "spmenu -sb <color>    Set the selected background color\n"
		  "spmenu -sf <color>    Set the selected foreground color\n"
          , stdout);
	exit(1);
}

int
main(int argc, char *argv[])
{
	XWindowAttributes wa;
	int i, fast = 0;

	XrmInitialize();

	for (i = 1; i < argc; i++) {
        /* xrdb first as it overrides other options */
		if (!strcmp(argv[i], "-xrdb"))   /* xresources */
			xresources = 1;
		else if (!strcmp(argv[i], "-nxrdb"))   /* no xresources */
			xresources = 0;
    }

    if (xresources) {
        load_xresources();
    }

    if (casesensitive) {
        fstrncmp = strncmp;
        fstrstr = strstr;
    }

	/* these options take no arguments */
	for (i = 1; i < argc; i++)
        if (!strcmp(argv[i], "-v")) {      /* prints version information */
			puts("spmenu-"VERSION);
			exit(0);
		} else if (!strcmp(argv[i], "-it")) { /* image: top */
			imageposition = 0;
		} else if (!strcmp(argv[i], "-ib")) { /* image: bottom */
			imageposition = 1;
		} else if (!strcmp(argv[i], "-ic")) { /* image: center */
			imageposition = 2;
		} else if (!strcmp(argv[i], "-itc")) { /* image: top center */
			imageposition = 3;
		} else if (!strcmp(argv[i], "-b")) { /* appears at the bottom of the screen */
			menuposition = 0;
		} else if (!strcmp(argv[i], "-t")) { /* appears at the top of the screen */
			menuposition = 1;
		} else if (!strcmp(argv[i], "-nm")) { /* normal mode */
			mode = 0;
		} else if (!strcmp(argv[i], "-im")) { /* insert mode */
			mode = 1;
        } else if (!strcmp(argv[i], "-c")) {  /* appears at the center of the screen */
		    centered = 1;
        } else if (!strcmp(argv[i], "-f")) {   /* grabs keyboard before reading stdin */
			fast = 1;
        } else if (!strcmp(argv[i], "-rw")) {  /* relative width */
			accuratewidth = 1;
        } else if (!strcmp(argv[i], "-nrw")) {   /* no relative width */
			accuratewidth = 0;
        } else if (!strcmp(argv[i], "-xrdb"))   /* xresources */
			xresources = 1;
		else if (!strcmp(argv[i], "-nxrdb"))   /* no xresources */
			xresources = 0;
		else if (!strcmp(argv[i], "-F"))   /* fuzzy matching */
		     fuzzy = 0;
		else if (!strcmp(argv[i], "-s")) { /* case-sensitive item matching */
			fstrncmp = strncmp;
			fstrstr = strstr;
		} else if (!strcmp(argv[i], "-i")) { /* case-sensitive item matching, for compatibility reasons */
		    fstrncmp = strncasecmp;
		    fstrstr = cistrstr;
        } else if (!strcmp(argv[i], "-gc")) { /* generate image cache */
            generatecache = 1;
		} else if (!strcmp(argv[i], "-wm")) { /* display as managed wm window */
				managed = 1;
		} else if (!strcmp(argv[i], "-na")) { /* disable alpha */
				alpha = 0;
		} else if (!strcmp(argv[i], "-a")) { /* alpha */
				alpha = 1;
        } else if (!strcmp(argv[i], "-cp")) { /* color prompt */
                colorprompt = 1;
        } else if (!strcmp(argv[i], "-ncp")) { /* no color prompt */
                colorprompt = 0;
        } else if (!strcmp(argv[i], "-tp")) { /*  allow the user to type */
                type = 1;
        } else if (!strcmp(argv[i], "-nt")) { /*  don't allow the user to type */
                type = 0;
		} else if (!strcmp(argv[i], "-P")) {  /* is the input a password */
				passwd = 1;
		} else if (!strcmp(argv[i], "-hmc")) {   /* hide match count */
				hidematchcount = 1;
		} else if (!strcmp(argv[i], "-smc")) {  /* don't hide match count */
				hidematchcount = 0;
		} else if (!strcmp(argv[i], "-hm")) {   /* hide mode indicator */
				hidemode = 1;
		} else if (!strcmp(argv[i], "-sm")) {  /* don't hide mode indicator */
				hidemode = 0;
        } else if (i + 1 == argc)
			usage();

		/* these options take one argument */
		else if (!strcmp(argv[i], "-g")) {   /* number of columns in grid */
			columns = atoi(argv[++i]);
			if (lines == 0) lines = 1;
		} else if (!strcmp(argv[i], "-l")) { /* number of lines in grid */
			lines = atoi(argv[++i]);
		} else if (!strcmp(argv[i], "-h")) { /* minimum height of one menu line */
	        clineheight += atoi(argv[++i]);
			if (columns == 0) columns = 1;
		} else if (!strcmp(argv[i], "-lp")) {
		    menupaddingv = atoi(argv[++i]);
		} else if (!strcmp(argv[i], "-hp")) {
		    menupaddingh = atoi(argv[++i]);
		} else if (!strcmp(argv[i], "-ig")) {
		    imagegaps = atoi(argv[++i]);
		} else if (!strcmp(argv[i], "-la")) {
		    leftarrow = argv[++i];
		} else if (!strcmp(argv[i], "-ra")) {
		    rightarrow = argv[++i];
		} else if (!strcmp(argv[i], "-m")) /* monitor */
			mon = atoi(argv[++i]);
		else if (!strcmp(argv[i], "-bw")) { /* border width */
            bordercentered = 0;
		    borderwidth = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "-H")) /* hist file location */
			histfile = argv[++i];
	    else if (!strcmp(argv[i], "-x"))   /* window x offset */
			dmx = atoi(argv[++i]);
		else if (!strcmp(argv[i], "-y"))   /* window y offset (from bottom up if -b) */
			dmy = atoi(argv[++i]);
		else if (!strcmp(argv[i], "-z"))   /* make spmenu this wide */
			dmw = atoi(argv[++i]);
		else if (!strcmp(argv[i], "-p"))   /* adds prompt to left of input field */
			prompt = argv[++i];
		else if (!strcmp(argv[i], "-fn"))  /* font or font set */
			strcpy(font, argv[++i]); /* font[0] = argv[++i]; */
		else if (!strcmp(argv[i], "-nmt"))  /* normal mode text */
			strcpy(normtext, argv[++i]);
		else if (!strcmp(argv[i], "-imt")) {  /* insert mode text */
			strcpy(instext, argv[++i]);

        /* dmenu compatibility options */
        } else if (!strcmp(argv[i], "-nb")) {  /* normal background color */
			colors[SchemeItemNorm][ColBg] = argv[++i];
            colors[SchemeMenu][ColBg] = argv[++i];
            colors[SchemeInput][ColBg] = argv[++i];
            colors[SchemePrompt][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-nf")) {  /* normal foreground color */
			colors[SchemeItemNorm][ColFg] = argv[++i];
            colors[SchemeMenu][ColFg] = argv[++i];
            colors[SchemeInput][ColFg] = argv[++i];
            colors[SchemePrompt][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-sb")) {  /* selected background color */
			colors[SchemeItemSel][ColBg] = argv[++i];
            colors[SchemeMenu][ColBg] = argv[++i];
            colors[SchemeInput][ColBg] = argv[++i];
            colors[SchemePrompt][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-sf")) {  /* selected foreground color */
			colors[SchemeItemSel][ColFg] = argv[++i];
            colors[SchemeMenu][ColFg] = argv[++i];
            colors[SchemeInput][ColBg] = argv[++i];
            colors[SchemePrompt][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-is")) { /* image size */
            char buf[255];
            memset(buf, 0, sizeof(buf));
            memcpy(buf, argv[++i], sizeof(buf)-1);

            if(sscanf(buf, "%dx%d", &imagewidth, &imageheight) == 1)
                imageheight = imagewidth;

        /* spmenu colors */
        } else if (!strcmp(argv[i], "-nif")) { /* normal item foreground color */
			colors[SchemeItemNorm][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-nib")) { /* normal item background color */
			colors[SchemeItemNorm][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-sif")) { /* selected item foreground color */
			colors[SchemeItemSel][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-sib")) { /* selected item background color */
			colors[SchemeItemSel][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-mbg")) { /* menu color */
			colors[SchemeMenu][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-pfg")) { /* prompt fg color */
			colors[SchemePrompt][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-pbg")) { /* prompt bg color */
			colors[SchemePrompt][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-ifg")) { /* input fg color */
			colors[SchemeInput][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-pfg")) { /* input bg color */
			colors[SchemeInput][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-shf")) { /* selected highlight foreground color */
			colors[SchemeSelHighlight][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-shf")) { /* selected highlight foreground color */
			colors[SchemeSelHighlight][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-nhf")) {  /* normal highlight foreground color */
			colors[SchemeNormHighlight][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-shb")) { /* selected highlight foreground color */
			colors[SchemeSelHighlight][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-nhb")) { /* normal highlight foreground color */
			colors[SchemeNormHighlight][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-nbg")) { /* numbgcolor */
			colors[SchemeNumber][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-nfg")) { /* numfgcolor */
			colors[SchemeNumber][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-mbg")) { /* mode */
			colors[SchemeMode][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-mfg")) { /* mode */
			colors[SchemeMode][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-laf")) { /* left arrow fg */
			colors[SchemeLArrow][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-raf")) { /* right arrow fg */
			colors[SchemeRArrow][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-lab")) { /* left arrow bg */
			colors[SchemeLArrow][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-rab")) { /* right arrow bg */
			colors[SchemeRArrow][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-bc")) { /* border */
			colors[SchemeBorder][ColBg] = argv[++i];
        }

        /* sgr colors */
		else if (!strcmp(argv[i], "-sgr0")) textcolors[0] = argv[++i]; /* sgr color 0 */
		else if (!strcmp(argv[i], "-sgr1")) textcolors[1] = argv[++i]; /* sgr color 1 */
		else if (!strcmp(argv[i], "-sgr2")) textcolors[2] = argv[++i]; /* sgr color 2 */
		else if (!strcmp(argv[i], "-sgr3")) textcolors[3] = argv[++i]; /* sgr color 3 */
		else if (!strcmp(argv[i], "-sgr4")) textcolors[4] = argv[++i]; /* sgr color 4 */
		else if (!strcmp(argv[i], "-sgr5")) textcolors[5] = argv[++i]; /* sgr color 5 */
		else if (!strcmp(argv[i], "-sgr6")) textcolors[6] = argv[++i]; /* sgr color 6 */
		else if (!strcmp(argv[i], "-sgr7")) textcolors[7] = argv[++i]; /* sgr color 7 */
		else if (!strcmp(argv[i], "-sgr8")) textcolors[8] = argv[++i]; /* sgr color 8 */
		else if (!strcmp(argv[i], "-sgr9")) textcolors[9] = argv[++i]; /* sgr color 9 */
		else if (!strcmp(argv[i], "-sgr10")) textcolors[10] = argv[++i]; /* sgr color 10 */
		else if (!strcmp(argv[i], "-sgr11")) textcolors[11] = argv[++i]; /* sgr color 11 */
		else if (!strcmp(argv[i], "-sgr12")) textcolors[12] = argv[++i]; /* sgr color 12 */
		else if (!strcmp(argv[i], "-sgr13")) textcolors[13] = argv[++i]; /* sgr color 13 */
		else if (!strcmp(argv[i], "-sgr14")) textcolors[14] = argv[++i]; /* sgr color 14 */
		else if (!strcmp(argv[i], "-sgr15")) textcolors[15] = argv[++i]; /* sgr color 15 */
		else if (!strcmp(argv[i], "-cc"))  /* caret color */
			colors[SchemeCaret][ColFg] = argv[++i];
		else if (!strcmp(argv[i], "-w"))   /* embedding window id */
			embed = argv[++i];
    	else if (!strcmp(argv[i], "-n"))   /* preselected item */
		    preselected = atoi(argv[++i]);
		else
			usage();

    longestedge = MAX(imagewidth, imageheight);

    if (mode) {
        selkeys = 1;
        allowkeys = 1;

        strcpy(modetext, instext);
    } else {
        selkeys = 0;
        allowkeys = !selkeys;

        strcpy(modetext, normtext);
    }

	if (!setlocale(LC_CTYPE, "") || !XSupportsLocale())
		fputs("warning: no locale support\n", stderr);
	if (!(dpy = XOpenDisplay(NULL)))
		die("cannot open display");
	screen = DefaultScreen(dpy);
	root = RootWindow(dpy, screen);
	if (!embed || !(parentwin = strtol(embed, NULL, 0)))
		parentwin = root;
	if (!XGetWindowAttributes(dpy, parentwin, &wa))
		die("could not get embedding window attributes: 0x%lx",
		    parentwin);
	xinitvisual();
	drw = drw_create(dpy, screen, root, wa.width, wa.height, visual, depth, cmap);

	if (!drw_font_create(drw, font))
	    die("no fonts could be loaded.");
	lrpad = drw->font->h;

	sp = menupaddingh;
	vp = (menuposition == 1) ? menupaddingv : - menupaddingv;

#ifdef __OpenBSD__
	if (pledge("stdio rpath", NULL) == -1)
		die("pledge");
#endif

	loadhistory();

	if (fast && !isatty(0)) {
		grabkeyboard();
		readstdin();
	} else {
		readstdin();
		grabkeyboard();
	}
	setup();
	run();

	return 1; /* unreachable */
}
