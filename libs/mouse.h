typedef struct {
	unsigned int click;
	unsigned int mask;
	unsigned int button;
	void (*func)(const Arg *arg);
	const Arg arg;
} Mouse;

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

static void buttonpress(XEvent *e);
