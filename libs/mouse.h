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

typedef struct {
    char *click;
    unsigned int button;
} ButtonType;

typedef struct {
    char *tclick;
    unsigned int click;
} ClickType;

static ButtonType btp[] = {
    { "button1",      Button1 },
    { "button2",      Button2 },
    { "button3",      Button3 },
    { "button4",      Button4 },
    { "button5",      Button5 },

    { "left-click",   Button1 },
    { "middle-click", Button2 },
    { "right-click",  Button3 },
    { "scroll-up",    Button4 },
    { "scroll-down",  Button5 },
};

static ClickType ctp[] = {
    { "clickwindow",  clickwindow },
    { "clickprompt",  clickprompt },
    { "clickinput",   clickinput },
    { "clicklarrow",  clicklarrow },
    { "clickitem",    clickitem },
    { "clickselitem", clickselitem },
    { "clickrarrow",  clickrarrow },
    { "clicknumber",  clicknumber },
    { "clickcaps",    clickcaps },
    { "clickmode",    clickmode },
};

static Mouse cbuttons[256];
static void buttonpress(XEvent *e);
