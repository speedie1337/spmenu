/* See LICENSE file for copyright and license details. */

typedef struct {
    unsigned int click;
    unsigned int button;
    void (*func)(Arg *arg);
    Arg arg;
} Mouse;

static Mouse cbuttons[256];
static void buttonpress_x11(XEvent *e);
