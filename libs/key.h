typedef struct {
    unsigned int mode;
	unsigned int mod;
	KeySym keysym;
	void (*func)(Arg *);
	Arg arg;
} Key;

#if USECONFIG
typedef struct {
    char *key;
    KeySym keysym;
} KeyList;

typedef struct {
    char *mod;
    unsigned int modifier;
} ModList;

typedef struct {
    char *argument;
    Arg arg;
} ArgList;

typedef struct {
    char *modif;
    char *key;
    char *func;
} ConfKeys;

typedef struct {
    char *function;
    void (*func)(Arg *);
} FuncList;

// TODO: allow any value instead of this array
static ArgList al[] = {
    { "0",          {.i = 0 } },
    { "1",          {.i = 1 } },
    { "2",          {.i = 2 } },
    { "3",          {.i = 3 } },
    { "4",          {.i = 4 } },
    { "5",          {.i = 5 } },
    { "6",          {.i = 6 } },
    { "7",          {.i = 7 } },
    { "8",          {.i = 8 } },
    { "9",          {.i = 9 } },
    { "10",         {.i = 10 } },
    { "11",         {.i = 11 } },
    { "12",         {.i = 12 } },
    { "13",         {.i = 13 } },
    { "14",         {.i = 14 } },
    { "15",         {.i = 15 } },
    { "16",         {.i = 16 } },
    { "17",         {.i = 17 } },
    { "18",         {.i = 18 } },
    { "19",         {.i = 19 } },
    { "20",         {.i = 20 } },
    { "21",         {.i = 21 } },
    { "22",         {.i = 22 } },
    { "23",         {.i = 23 } },
    { "24",         {.i = 24 } },
    { "25",         {.i = 25 } },
    { "26",         {.i = 26 } },
    { "27",         {.i = 27 } },
    { "28",         {.i = 28 } },
    { "29",         {.i = 29 } },
    { "30",         {.i = 30 } },
    { "31",         {.i = 31 } },
    { "32",         {.i = 32 } },
    { "33",         {.i = 33 } },
    { "34",         {.i = 34 } },
    { "35",         {.i = 35 } },
    { "36",         {.i = 36 } },
    { "37",         {.i = 37 } },
    { "38",         {.i = 38 } },
    { "39",         {.i = 39 } },
    { "40",         {.i = 40 } },
    { "41",         {.i = 41 } },
    { "42",         {.i = 42 } },
    { "43",         {.i = 43 } },
    { "44",         {.i = 44 } },
    { "45",         {.i = 45 } },
    { "46",         {.i = 46 } },
    { "47",         {.i = 47 } },
    { "48",         {.i = 48 } },
    { "49",         {.i = 49 } },
    { "50",         {.i = 50 } },
    { "51",         {.i = 51 } },
    { "52",         {.i = 52 } },
    { "53",         {.i = 53 } },
    { "54",         {.i = 54 } },
    { "55",         {.i = 55 } },
    { "56",         {.i = 56 } },
    { "57",         {.i = 57 } },
    { "58",         {.i = 58 } },
    { "59",         {.i = 59 } },
    { "60",         {.i = 60 } },
    { "61",         {.i = 61 } },
    { "62",         {.i = 62 } },
    { "63",         {.i = 63 } },
    { "64",         {.i = 64 } },
    { "65",         {.i = 65 } },
    { "66",         {.i = 66 } },
    { "67",         {.i = 67 } },
    { "68",         {.i = 68 } },
    { "69",         {.i = 69 } },
    { "70",         {.i = 70 } },
    { "71",         {.i = 71 } },
    { "72",         {.i = 72 } },
    { "73",         {.i = 73 } },
    { "74",         {.i = 74 } },
    { "75",         {.i = 75 } },
    { "76",         {.i = 76 } },
    { "77",         {.i = 77 } },
    { "78",         {.i = 78 } },
    { "79",         {.i = 79 } },
    { "80",         {.i = 80 } },
    { "81",         {.i = 81 } },
    { "82",         {.i = 82 } },
    { "83",         {.i = 83 } },
    { "84",         {.i = 84 } },
    { "85",         {.i = 85 } },
    { "86",         {.i = 86 } },
    { "87",         {.i = 87 } },
    { "88",         {.i = 88 } },
    { "89",         {.i = 89 } },
    { "90",         {.i = 90 } },
    { "91",         {.i = 91 } },
    { "92",         {.i = 92 } },
    { "93",         {.i = 93 } },
    { "94",         {.i = 94 } },
    { "95",         {.i = 95 } },
    { "96",         {.i = 96 } },
    { "97",         {.i = 97 } },
    { "98",         {.i = 98 } },
    { "99",         {.i = 99 } },
    { "100",        {.i = 100 } },
    { "+0",         {.i = +0 } },
    { "+1",         {.i = +1 } },
    { "+2",         {.i = +2 } },
    { "+3",         {.i = +3 } },
    { "+4",         {.i = +4 } },
    { "+5",         {.i = +5 } },
    { "+6",         {.i = +6 } },
    { "+7",         {.i = +7 } },
    { "+8",         {.i = +8 } },
    { "+9",         {.i = +9 } },
    { "+10",        {.i = +10 } },
    { "+11",        {.i = +11 } },
    { "+12",        {.i = +12 } },
    { "+13",        {.i = +13 } },
    { "+14",        {.i = +14 } },
    { "+15",        {.i = +15 } },
    { "+16",        {.i = +16 } },
    { "+17",        {.i = +17 } },
    { "+18",        {.i = +18 } },
    { "+19",        {.i = +19 } },
    { "+20",        {.i = +20 } },
    { "+21",        {.i = +21 } },
    { "+22",        {.i = +22 } },
    { "+23",        {.i = +23 } },
    { "+24",        {.i = +24 } },
    { "+25",        {.i = +25 } },
    { "+26",        {.i = +26 } },
    { "+27",        {.i = +27 } },
    { "+28",        {.i = +28 } },
    { "+29",        {.i = +29 } },
    { "+30",        {.i = +30 } },
    { "+31",        {.i = +31 } },
    { "+32",        {.i = +32 } },
    { "+33",        {.i = +33 } },
    { "+34",        {.i = +34 } },
    { "+35",        {.i = +35 } },
    { "+36",        {.i = +36 } },
    { "+37",        {.i = +37 } },
    { "+38",        {.i = +38 } },
    { "+39",        {.i = +39 } },
    { "+40",        {.i = +40 } },
    { "+41",        {.i = +41 } },
    { "+42",        {.i = +42 } },
    { "+43",        {.i = +43 } },
    { "+44",        {.i = +44 } },
    { "+45",        {.i = +45 } },
    { "+46",        {.i = +46 } },
    { "+47",        {.i = +47 } },
    { "+48",        {.i = +48 } },
    { "+49",        {.i = +49 } },
    { "+50",        {.i = +50 } },
    { "+51",        {.i = +51 } },
    { "+52",        {.i = +52 } },
    { "+53",        {.i = +53 } },
    { "+54",        {.i = +54 } },
    { "+55",        {.i = +55 } },
    { "+56",        {.i = +56 } },
    { "+57",        {.i = +57 } },
    { "+58",        {.i = +58 } },
    { "+59",        {.i = +59 } },
    { "+60",        {.i = +60 } },
    { "+61",        {.i = +61 } },
    { "+62",        {.i = +62 } },
    { "+63",        {.i = +63 } },
    { "+64",        {.i = +64 } },
    { "+65",        {.i = +65 } },
    { "+66",        {.i = +66 } },
    { "+67",        {.i = +67 } },
    { "+68",        {.i = +68 } },
    { "+69",        {.i = +69 } },
    { "+70",        {.i = +70 } },
    { "+71",        {.i = +71 } },
    { "+72",        {.i = +72 } },
    { "+73",        {.i = +73 } },
    { "+74",        {.i = +74 } },
    { "+75",        {.i = +75 } },
    { "+76",        {.i = +76 } },
    { "+77",        {.i = +77 } },
    { "+78",        {.i = +78 } },
    { "+79",        {.i = +79 } },
    { "+80",        {.i = +80 } },
    { "+81",        {.i = +81 } },
    { "+82",        {.i = +82 } },
    { "+83",        {.i = +83 } },
    { "+84",        {.i = +84 } },
    { "+85",        {.i = +85 } },
    { "+86",        {.i = +86 } },
    { "+87",        {.i = +87 } },
    { "+88",        {.i = +88 } },
    { "+89",        {.i = +89 } },
    { "+90",        {.i = +90 } },
    { "+91",        {.i = +91 } },
    { "+92",        {.i = +92 } },
    { "+93",        {.i = +93 } },
    { "+94",        {.i = +94 } },
    { "+95",        {.i = +95 } },
    { "+96",        {.i = +96 } },
    { "+97",        {.i = +97 } },
    { "+98",        {.i = +98 } },
    { "+99",        {.i = +99 } },
    { "+100",       {.i = +100 } },
    { "-0",         {.i = -0 } },
    { "-1",         {.i = -1 } },
    { "-2",         {.i = -2 } },
    { "-3",         {.i = -3 } },
    { "-4",         {.i = -4 } },
    { "-5",         {.i = -5 } },
    { "-6",         {.i = -6 } },
    { "-7",         {.i = -7 } },
    { "-8",         {.i = -8 } },
    { "-9",         {.i = -9 } },
    { "-10",        {.i = -10 } },
    { "-11",        {.i = -11 } },
    { "-12",        {.i = -12 } },
    { "-13",        {.i = -13 } },
    { "-14",        {.i = -14 } },
    { "-15",        {.i = -15 } },
    { "-16",        {.i = -16 } },
    { "-17",        {.i = -17 } },
    { "-18",        {.i = -18 } },
    { "-19",        {.i = -19 } },
    { "-20",        {.i = -20 } },
    { "-21",        {.i = -21 } },
    { "-22",        {.i = -22 } },
    { "-23",        {.i = -23 } },
    { "-24",        {.i = -24 } },
    { "-25",        {.i = -25 } },
    { "-26",        {.i = -26 } },
    { "-27",        {.i = -27 } },
    { "-28",        {.i = -28 } },
    { "-29",        {.i = -29 } },
    { "-30",        {.i = -30 } },
    { "-31",        {.i = -31 } },
    { "-32",        {.i = -32 } },
    { "-33",        {.i = -33 } },
    { "-34",        {.i = -34 } },
    { "-35",        {.i = -35 } },
    { "-36",        {.i = -36 } },
    { "-37",        {.i = -37 } },
    { "-38",        {.i = -38 } },
    { "-39",        {.i = -39 } },
    { "-40",        {.i = -40 } },
    { "-41",        {.i = -41 } },
    { "-42",        {.i = -42 } },
    { "-43",        {.i = -43 } },
    { "-44",        {.i = -44 } },
    { "-45",        {.i = -45 } },
    { "-46",        {.i = -46 } },
    { "-47",        {.i = -47 } },
    { "-48",        {.i = -48 } },
    { "-49",        {.i = -49 } },
    { "-50",        {.i = -50 } },
    { "-51",        {.i = -51 } },
    { "-52",        {.i = -52 } },
    { "-53",        {.i = -53 } },
    { "-54",        {.i = -54 } },
    { "-55",        {.i = -55 } },
    { "-56",        {.i = -56 } },
    { "-57",        {.i = -57 } },
    { "-58",        {.i = -58 } },
    { "-59",        {.i = -59 } },
    { "-60",        {.i = -60 } },
    { "-61",        {.i = -61 } },
    { "-62",        {.i = -62 } },
    { "-63",        {.i = -63 } },
    { "-64",        {.i = -64 } },
    { "-65",        {.i = -65 } },
    { "-66",        {.i = -66 } },
    { "-67",        {.i = -67 } },
    { "-68",        {.i = -68 } },
    { "-69",        {.i = -69 } },
    { "-70",        {.i = -70 } },
    { "-71",        {.i = -71 } },
    { "-72",        {.i = -72 } },
    { "-73",        {.i = -73 } },
    { "-74",        {.i = -74 } },
    { "-75",        {.i = -75 } },
    { "-76",        {.i = -76 } },
    { "-77",        {.i = -77 } },
    { "-78",        {.i = -78 } },
    { "-79",        {.i = -79 } },
    { "-80",        {.i = -80 } },
    { "-81",        {.i = -81 } },
    { "-82",        {.i = -82 } },
    { "-83",        {.i = -83 } },
    { "-84",        {.i = -84 } },
    { "-85",        {.i = -85 } },
    { "-86",        {.i = -86 } },
    { "-87",        {.i = -87 } },
    { "-88",        {.i = -88 } },
    { "-89",        {.i = -89 } },
    { "-90",        {.i = -90 } },
    { "-91",        {.i = -91 } },
    { "-92",        {.i = -92 } },
    { "-93",        {.i = -93 } },
    { "-94",        {.i = -94 } },
    { "-95",        {.i = -95 } },
    { "-96",        {.i = -96 } },
    { "-97",        {.i = -97 } },
    { "-98",        {.i = -98 } },
    { "-99",        {.i = -99 } },
    { "-100",       {.i = -100 } },
};

static FuncList fl[] = {
    { "moveup",         moveup },
    { "movedown",       movedown },
    { "moveleft",       moveleft },
    { "moveright",      moveright },
    { "moveend",        moveend },
    { "movestart",      movestart },
    { "move",           movenext },
    { "move",           moveprev },
    { "paste",          paste },
    { "pastesel",       paste },
    { "restoresel",     restoresel },
    { "clear",          clear },
    { "clearins",       clearins },
    { "viewhist",       viewhist },
    { "moveword",       moveword },
    { "moveword",       moveword },
    { "deleteword",     deleteword },
    { "movecursor",     movecursor },
    { "movecursor",     movecursor },
    { "navhistory",     navhistory },
    { "navhistory",     navhistory },
    { "backspace",      backspace },
    { "selectitem",     selectitem },
    { "quit",           quit },
    { "complete",       complete },
    { "setimgsize",     setimgsize },
    { "setimgsize",     setimgsize },
    { "toggleimg",      toggleimg },
    { "defaultimg",     defaultimg },
    { "rotateimg",      rotateimg },
    { "flipimg",        flipimg },
    { "setimgpos",      setimgpos },
    { "setimgpos",      setimgpos },
    { "setimggaps",     setimggaps },
    { "setimggaps",     setimggaps },
    { "setlines",       setlines },
    { "setlines",       setlines },
    { "setcolumns",     setcolumns },
    { "togglehighlight",togglehighlight },
    { "setprofile",     setprofile },
    { "switchmode",     switchmode },
    { "spawn",          spawn },
};

// list of modifiers that can be used in the config file
static ModList ml[] = {
    { "Ctrl+Shift", ShiftMask|ControlMask },
    { "Ctrl+Shift+Super", ShiftMask|ControlMask|Mod4Mask },
    { "Ctrl+Super", Mod4Mask|ControlMask },
    { "Ctrl+Alt", ControlMask|Mod1Mask },
    { "Ctrl+Alt+Shift", ShiftMask|Mod1Mask|ControlMask },
    { "Ctrl+Alt+Shift+Super", ControlMask|Mod1Mask|ShiftMask|Mod4Mask },
    { "Ctrl+Alt+Super", ControlMask|Mod1Mask|Mod4Mask },
    { "Alt+Shift", Mod1Mask|ShiftMask },
    { "Shift",   ShiftMask },
    { "Ctrl",    ControlMask },
    { "Alt",     Mod1Mask },
    { "AltGr",   Mod3Mask },
    { "Super",   Mod4Mask },
    { "ShiftGr", Mod5Mask },
    { "None",    0 },
    { "0",       0 },
};

// list of keys that can be used in the config file
// expand this array if you want more
static KeyList kl[] = {
    { "None",       0 },
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
    { "j",          XK_j },
    { "k",          XK_k },
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
    { "Backspace",  XK_BackSpace },
    { "Up",         XK_Up },
    { "Down",       XK_Down },
    { "Left",       XK_Left },
    { "Right",      XK_Right },
    { "Next",       XK_Next },
    { "Prior",      XK_Prior },
};
#endif

static void updatenumlockmask(void);
static void keypress(XEvent *e);
static void grabkeyboard(void);
static void getcapsstate(void);

static Key ckeys[256];
static char capstext[16] = "Caps Lock";
