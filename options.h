/* spmenu
 * Below is a configuration file which is technically C source code.
 * See the LICENSE file for license details.
 */

/* spmenu options */
static char class[]                    = "spmenu"; /* Class for spmenu */
static int xresources                  = 1; /* Enable .Xresources support */

/* Window options */
static int alpha                       = 1; /* Enable alpha */
static int menuposition                = 1; /* Position of the menu */
static int menupaddingv				   = 0; /* Vertical padding of bar (in pixels) */
static int menupaddingh				   = 0; /* Horizontal padding of bar (in pixels) */
static int minwidth                    = 500; /* Minimum width */
static int centered                    = 0; /* Whether or not to center spmenu by default */

static int colorsupport                = 0; /* Support 256 colors? Otherwise the default 16 colors will be used. */

/* Window border options */
static int borderwidth                 = 2; /* Width of the border */
static int bordercentered              = 1; /* Draw border only when centered */

/* Font options */
static char font[]                     = "DejaVu Sans Mono 8"; /* Font to draw text and Pango markup with. */

/* Symbol options */
static char *leftarrow                 = "<"; /* Left arrow, used to indicate you can move to the left */
static char *rightarrow                = ">"; /* Right arrow, used to indicate you can move to the right */

/* Match options */
static int type                        = 1; /* Allow typing into spmenu or only allow keybinds. */
static int casesensitive               = 0; /* Case-sensitive by default? (0/1) */
static int preselected                 = 0; /* Which line should spmenu preselect? */
static int fuzzy                       = 1; /* Whether or not to enable fuzzy matching by default */

/* Line options */
static int lines                       = 0; /* Default number of lines */
static int columns                     = 10; /* Default number of columns */
static int lineheight                  = 20; /* Height of each line */
static int minlineheight               = 5; /* Minimum line height */

/* History options */
static unsigned int maxhist            = 64; /* Max number of history entries */
static int histnodup                   = 1;	/* If 0, record repeated histories */

/* Prompt options */
static int colorprompt                 = 1; /* Color prompt (0/1) */
static char *prompt                    = NULL; /* Default prompt, set to NULL (nothing) */

/* Hide options */
static int hidematchcount              = 0; /* Hide match count (0/1) */

/* Color options
 *
 * Normal foreground colors
 */
static char normfgcolor[]              = "#bbbbbb"; /* Text color for unselected */
static char normbgcolor[]              = "#222222"; /* Background color for unselected */

/* Selected foreground colors */
static char selfgcolor[]               = "#eeeeee"; /* Text color for selected */
static char selbgcolor[]               = "#5e81ac"; /* Background color for selected */

/* Normal highlight colors */
static char normhlfgcolor[]            = "#ffffff"; /* Text highlight color for unselected */
static char normhlbgcolor[]            = "#000000"; /* Background highlight color for unselected */

/* Selected highlight colors */
static char selhlfgcolor[]             = "#ffffff"; /* Text highlight color for selected */
static char selhlbgcolor[]             = "#000000"; /* Background highlight color for selected */

/* Match count colors */
static char numfgcolor[]               = "#ffffff"; /* Match count text color */
static char numbgcolor[]               = "#000000"; /* Match count background color */

/* Caret colors */
static char caretfgcolor[]             = "#222222"; /* Caret color */

/* SGR colors */
static char sgrcolor0[]                = "#000000"; /* SGR color #0 */
static char sgrcolor1[]                = "#7f0000"; /* SGR color #1 */
static char sgrcolor2[]                = "#007f00"; /* SGR color #2 */
static char sgrcolor3[]                = "#7f7f00"; /* SGR color #3 */
static char sgrcolor4[]                = "#00007f"; /* SGR color #4 */
static char sgrcolor5[]                = "#7f007f"; /* SGR color #5 */
static char sgrcolor6[]                = "#007f7f"; /* SGR color #6 */
static char sgrcolor7[]                = "#cccccc"; /* SGR color #7 */
static char sgrcolor8[]                = "#333333"; /* SGR color #8 */
static char sgrcolor9[]                = "#ff0000"; /* SGR color #9 */
static char sgrcolor10[]               = "#00ff00"; /* SGR color #10 */
static char sgrcolor11[]               = "#ffff00"; /* SGR color #11 */
static char sgrcolor12[]               = "#0000ff"; /* SGR color #12 */
static char sgrcolor13[]               = "#ff00ff"; /* SGR color #13 */
static char sgrcolor14[]               = "#00ffff"; /* SGR color #14 */
static char sgrcolor15[]               = "#ffffff"; /* SGR color #15 */

/* Alpha options */
#define baralpha                       200 /* Bar alpha */
#define borderalpha                    opaque /* Border alpha */

/* Misc */
static char worddelimiters[]           = " "; /* Word delimiters, " " is default. */
