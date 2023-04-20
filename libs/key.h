typedef struct {
    unsigned int mode;
	unsigned int mod;
	KeySym keysym;
	void (*func)(const Arg *);
	const Arg arg;
} Key;

static void updatenumlockmask(void);
static void keypress(XEvent *e);
static void grabkeyboard(void);
static void getcapsstate(void);

static char capstext[16] = "Caps Lock";
