/* See LICENSE file for copyright and license details. */
typedef union {
    int i; // integer
    unsigned int ui; // unsigned int
    float f; // float
    const void *v; // void
    const char *c; // char
} Arg;

/* declare keybind functions
 *
 * movement
 */
static void moveup(Arg *arg);
static void movedown(Arg *arg);
static void moveleft(Arg *arg);
static void moveright(Arg *arg);
static void moveend(Arg *arg);
static void movestart(Arg *arg);
static void movenext(Arg *arg);
static void moveprev(Arg *arg);
static void moveitem(Arg *arg);
static void moveword(Arg *arg);
static void movecursor(Arg *arg);

/* input */
static void paste(Arg *arg);
static void restoresel(Arg *arg);
static void clear(Arg *arg);
static void clearins(Arg *arg);
static void deleteword(Arg *arg);
static void backspace(Arg *arg);
static void complete(Arg *arg);

/* history */
static void viewhist(Arg *arg);
static void navhistory(Arg *arg);

/* selection */
static void markitem(Arg *arg);
static void selectitem(Arg *arg);

/* image */
static void setimgsize(Arg *arg);
static void defaultimg(Arg *arg);
static void flipimg(Arg *arg);
static void setimgpos(Arg *arg);
static void setimggaps(Arg *arg);

/* lines/columns */
static void setlines(Arg *arg);
static void setcolumns(Arg *arg);

/* misc */
static void spawn(Arg *arg);
static void screenshot(Arg *arg);
static void switchmode(Arg *arg);
static void setprofile(Arg *arg);
static void setlineheight(Arg *arg);
static void quit(Arg *arg);

/* toggle */
static void toggleinput(Arg *arg);
static void togglelarrow(Arg *arg);
static void togglerarrow(Arg *arg);
static void toggleitem(Arg *arg);
static void toggleprompt(Arg *arg);
static void togglecaps(Arg *arg);
static void togglepowerline(Arg *arg);
static void togglecaret(Arg *arg);
static void togglematchcount(Arg *arg);
static void togglemode(Arg *arg);
static void togglehighlight(Arg *arg);
static void toggleregex(Arg *arg);
static void togglefuzzy(Arg *arg);
static void toggleimg(Arg *arg);

/* hover */
static void selecthover(Arg *arg);
static void markhover(Arg *arg);
