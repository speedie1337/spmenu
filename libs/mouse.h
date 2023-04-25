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
    { "Left Click",   Button1 },
    { "Middle Click", Button2 },
    { "Right Click",  Button3 },
    { "Scroll Up",    Button4 },
    { "Scroll Down",  Button5 },
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
