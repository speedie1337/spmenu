typedef union {
	int i; // integer
	unsigned int ui; // unsigned int
	float f; // float
	const void *v; // void
    const char *c; // char
} Arg;

// declare keybind functions
static void moveup(const Arg *arg);
static void movedown(const Arg *arg);
static void moveleft(const Arg *arg);
static void moveright(const Arg *arg);
static void moveend(const Arg *arg);
static void movestart(const Arg *arg);
static void movenext(const Arg *arg);
static void moveprev(const Arg *arg);
static void paste(const Arg *arg);
static void restoresel(const Arg *arg);
static void clear(const Arg *arg);
static void clearins(const Arg *arg);
static void viewhist(const Arg *arg);
static void moveword(const Arg *arg);
static void deleteword(const Arg *arg);
static void movecursor(const Arg *arg);
static void navhistory(const Arg *arg);
static void backspace(const Arg *arg);
static void selectitem(const Arg *arg);
static void quit(const Arg *arg);
static void complete(const Arg *arg);
static void savehistory(char *input);
static void setimgsize(const Arg *arg);
static void toggleimg(const Arg *arg);
static void defaultimg(const Arg *arg);
static void rotateimg(const Arg *arg);
static void flipimg(const Arg *arg);
static void setimgpos(const Arg *arg);
static void setimggaps(const Arg *arg);
static void setlines(const Arg *arg);
static void setcolumns(const Arg *arg);
static void spawn(const Arg *arg);
static void togglehighlight(const Arg *arg);
