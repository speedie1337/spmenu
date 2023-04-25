typedef struct {
    unsigned int mode;
	unsigned int mod;
	KeySym keysym;
	void (*func)(Arg *);
	Arg arg;
} Key;

typedef struct {
    char *key;
    KeySym keysym;
} KeyList;

typedef struct {
    char *mod;
    unsigned int modifier;
} ModList;

typedef struct {
    char *modif;
    char *key;
    char *func;
} ConfKeys;

typedef struct {
    char *function;
    void (*func)(Arg *);
    Arg arg;
} FuncList;

static FuncList fl[] = {
    { "moveup",          moveup,          {0} },
    { "movedown",        movedown,        {0} },
    { "moveleft",        moveleft,        {0} },
    { "moveright",       moveright,       {0} },
    { "moveend",         moveend,         {0} },
    { "movestart",       movestart,       {0} },
    { "move+",           movenext,        {0} },
    { "move-",           moveprev,        {0} },
    { "paste",           paste,           {.i = 2 } },
    { "pastesel",        paste,           {.i = 1 } },
    { "restoresel",      restoresel,      {0} },
    { "clear",           clear,           {0} },
    { "clearins",        clearins,        {0} },
    { "viewhist",        viewhist,        {0} },
    { "moveword-",       moveword,        {.i = -1 } },
    { "moveword+",       moveword,        {.i = +1 } },
    { "deleteword",      deleteword,      {0} },
    { "movecursor-",     movecursor,      {.i = -1 } },
    { "movecursor+",     movecursor,      {.i = +1 } },
    { "navhistory-",     navhistory,      {.i = -1 } },
    { "navhistory+",     navhistory,      {.i = +1 } },
    { "backspace",       backspace,       {0} },
    { "selectitem",      selectitem,      {.i = +1 } },
    { "selectinput",     selectitem,      {0} },
    { "quit",            quit,            {0} },
    { "complete",        complete,        {0} },
    { "imgsize+",        setimgsize,      {.i = +1 } },
    { "imgsize-",        setimgsize,      {.i = -1 } },
    { "toggleimg",       toggleimg,       {0} },
    { "defaultimg",      defaultimg,      {0} },
    { "rotateimg",       rotateimg,       {0} },
    { "flipimg",         flipimg,         {0} },
    { "setimgpos+",      setimgpos,       {.i = +1 } },
    { "setimgpos-",      setimgpos,       {.i = -1 } },
    { "setimggaps+",     setimggaps,      {.i = +1 } },
    { "setimggaps-",     setimggaps,      {.i = -1 } },
    { "setlines+",       setlines,        {.i = +1 } },
    { "setlines-",       setlines,        {.i = -1 } },
    { "setcolumns+",     setcolumns,      {.i = +1 } },
    { "setcolumns-",     setcolumns,      {.i = -1 } },
    { "togglehighlight", togglehighlight, {0} },
};

// list of modifiers that can be used in the config file
static ModList ml[] = {
    { "Shift",   ShiftMask },
    { "Lock",    LockMask },
    { "Ctrl",    ControlMask },
    { "Alt",     Mod1Mask },
    { "AltGr",   Mod3Mask },
    { "Super",   Mod4Mask },
    { "ShiftGr", Mod5Mask },
};

// list of keys that can be used in the config file
// expand this array if you want more
static KeyList kl[] = {
    { "Space",      XK_space },
    { "Enter",      XK_Return },
    { "Tab",        XK_Tab },
    { "a",          XK_a },
    { "b",          XK_b },
    { "c",          XK_c },
    { "d",          XK_d },
    { "e",          XK_e },
    { "f",          XK_f },
    { "g",          XK_g },
    { "h",          XK_h },
    { "i",          XK_i },
    { "j",          XK_k },
    { "k",          XK_j },
    { "l",          XK_l },
    { "m",          XK_m },
    { "n",          XK_n },
    { "o",          XK_o },
    { "p",          XK_p },
    { "q",          XK_q },
    { "r",          XK_r },
    { "s",          XK_s },
    { "t",          XK_t },
    { "u",          XK_u },
    { "v",          XK_v },
    { "w",          XK_w },
    { "x",          XK_x },
    { "y",          XK_y },
    { "z",          XK_z },
    { "0",          XK_0 },
    { "1",          XK_1 },
    { "2",          XK_2 },
    { "3",          XK_3 },
    { "4",          XK_4 },
    { "5",          XK_5 },
    { "6",          XK_6 },
    { "7",          XK_7 },
    { "8",          XK_8 },
    { "9",          XK_9 },
    { "!",          XK_exclam },
    { "\"",         XK_quotedbl },
    { "#",          XK_numbersign },
    { "$",          XK_dollar },
    { "%",          XK_percent },
    { "&",          XK_ampersand },
    { "'",          XK_apostrophe },
    { "(",          XK_parenleft },
    { ")",          XK_parenright },
    { "*",          XK_asterisk },
    { "+",          XK_plus },
    { ",",          XK_comma },
    { "-",          XK_minus },
    { ".",          XK_period },
    { "/",          XK_slash },
    { ":",          XK_colon },
    { ";",          XK_semicolon },
    { "<",          XK_less },
    { "=",          XK_equal },
    { ">",          XK_greater },
    { "?",          XK_question },
    { "@",          XK_at },
    { "[",          XK_bracketleft },
    { "\\",         XK_backslash },
    { "]",          XK_bracketright },
    { "_",          XK_underscore },
    { "grave",      XK_grave },
    { "{",          XK_braceleft },
    { "bar",        XK_bar },
    { "}",          XK_braceright },
    { "~",          XK_asciitilde },
    { "F1",         XK_F1 },
    { "F2",         XK_F2 },
    { "F3",         XK_F3 },
    { "F4",         XK_F4 },
    { "F5",         XK_F5 },
    { "F6",         XK_F6 },
    { "F7",         XK_F7 },
    { "F8",         XK_F8 },
    { "F9",         XK_F9 },
    { "F10",        XK_F10 },
    { "F11",        XK_F11 },
    { "F12",        XK_F12 },
    { "PageUp",     XK_Page_Up },
    { "PageDown",   XK_Page_Down },
    { "Home",       XK_Home },
    { "End",        XK_End },
    { "Delete",     XK_Delete },
    { "PrintScr",   XK_Print },
    { "Esc",        XK_Escape },
    { "Pause",      XK_Pause },
    { "ScrollLock", XK_Scroll_Lock },
};

static void updatenumlockmask(void);
static void keypress(XEvent *e);
static void grabkeyboard(void);
static void getcapsstate(void);

static Key ckeys[256];
static char capstext[16] = "Caps Lock";
