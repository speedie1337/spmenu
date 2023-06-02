/* See LICENSE file for copyright and license details. */

// clicks
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

typedef struct {
    unsigned int click;
    unsigned int button;
    void (*func)(Arg *arg);
    Arg arg;
} Mouse;

static Mouse cbuttons[256];
static void buttonpress_x11(XEvent *e);
