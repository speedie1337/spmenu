/* See LICENSE file for copyright and license details. */

static Atom clip, utf8, types, dock;
static Display *dpy;
static Window root, parentwin, win;

static void setupdisplay(void);
static void set_screen(Display *disp);
static void handle_x11(void);
static void cleanup_x11(Display *disp);
static Display * opendisplay(char *disp);
