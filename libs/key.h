typedef struct {
    unsigned int mode;
	unsigned int mod;
	KeySym keysym;
	void (*func)(Arg *);
	Arg arg;
} Key;

static void updatenumlockmask(void);
static void keypress(XEvent *e);
static void grabkeyboard(void);
static void getcapsstate(void);

static Key ckeys[256];
static char capstext[16] = "Caps Lock";
