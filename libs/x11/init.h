/* See LICENSE file for copyright and license details. */

static Atom clip, utf8, types, dock;
static Display *dpy;
static Window root, parentwin, win;

static void setupdisplay_x11(void);
static void set_screen_x11(Display *disp);
static void handle_x11(void);
static void cleanup_x11(Display *disp);
static void prepare_window_size_x11(void);
static Display * opendisplay_x11(char *disp);
