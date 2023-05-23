/* See LICENSE file for copyright and license details. */

static Atom clip, utf8, types, dock;
static Display *dpy;
static Window root, parentwin, win;

static void setupdisplay(void);
static void set_screen(Display *disp);
static void handle_x11(void);
static Display * opendisplay(char *disp);
