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
    clickrarrow,
    clicknumber,
    clickmode,
};

static unsigned int textw_clamp(const char *str, unsigned int n);
static void motionevent(XButtonEvent *ev);
static void buttonpress(XEvent *e);
