// clicks
enum {
    clickwindow,
    clickprompt,
    clickinput,
    clicklarrow,
    clickitem,
    clickselitem,
    clickrarrow,
    clicknumber,
    clickcaps,
    clickmode,
};

typedef struct {
	unsigned int click;
	unsigned int mask;
	unsigned int button;
	void (*func)(Arg *arg);
	Arg arg;
} Mouse;

static Mouse cbuttons[256];
static void buttonpress(XEvent *e);
