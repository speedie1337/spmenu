static char modetext[16] = "Insert"; // default mode text

// mode settings
static int curMode = 1; // 0 is command mode
static int allowkeys = 1; // whether or not to interpret a keypress as an insertion

// mode functions
static void switchmode(Arg *arg);
