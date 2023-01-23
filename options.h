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

static int colorsupport                = 1; /* Support 256 colors? Otherwise the default 16 colors will be used. */

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
static int accuratewidth               = 1; /* Enable accurate width. May have a performance hit if you are matching a lot of items at once */
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
 * Item colors */
static char col_itemnormfg[]           = "#bbbbbb"; /* Normal foreground item color */
static char col_itemnormbg[]           = "#222222"; /* Normal background item color */
static char col_itemselfg[]            = "#eeeeee"; /* Selected foreground item color */
static char col_itemselbg[]            = "#35638A"; /* Selected background item color */

/* Input colors */
static char col_inputbg[]              = "#222222"; /* Input field background color */
static char col_inputfg[]              = "#eeeeee"; /* Input field foreground color */

/* Menu colors */
static char col_menubgcolor[]          = "#222222"; /* Menu background color */

/* Prompt colors */
static char col_promptfg[]             = "#eeeeee"; /* Prompt foreground color */
static char col_promptbg[]             = "#35526b"; /* Prompt background color */

/* Arrow colors */
static char col_larrowfg[]             = "#bbbbbb"; /* Left arrow color */
static char col_rarrowfg[]             = "#bbbbbb"; /* Right arrow color */
static char col_larrowbg[]             = "#222222"; /* Left arrow color */
static char col_rarrowbg[]             = "#222222"; /* Right arrow color */

/* Normal highlight colors */
static char col_normhlfgcolor[]        = "#ffffff"; /* Text highlight color for unselected */
static char col_normhlbgcolor[]        = "#000000"; /* Background highlight color for unselected */

/* Selected highlight colors */
static char col_selhlfgcolor[]         = "#ffffff"; /* Text highlight color for selected */
static char col_selhlbgcolor[]         = "#000000"; /* Background highlight color for selected */

/* Match count colors */
static char col_numfgcolor[]           = "#ffffff"; /* Match count text color */
static char col_numbgcolor[]           = "#2d3856"; /* Match count background color */

/* Border color */
static char col_bordercolor[]          = "#35638A"; /* Border color */

/* Caret colors */
static char col_caretfgcolor[]         = "#ffffff"; /* Caret color */

/* SGR colors */
static char col_sgrcolor0[]            = "#000000"; /* SGR color #0 */
static char col_sgrcolor1[]            = "#7f0000"; /* SGR color #1 */
static char col_sgrcolor2[]            = "#007f00"; /* SGR color #2 */
static char col_sgrcolor3[]            = "#7f7f00"; /* SGR color #3 */
static char col_sgrcolor4[]            = "#00007f"; /* SGR color #4 */
static char col_sgrcolor5[]            = "#7f007f"; /* SGR color #5 */
static char col_sgrcolor6[]            = "#007f7f"; /* SGR color #6 */
static char col_sgrcolor7[]            = "#cccccc"; /* SGR color #7 */
static char col_sgrcolor8[]            = "#333333"; /* SGR color #8 */
static char col_sgrcolor9[]            = "#ff0000"; /* SGR color #9 */
static char col_sgrcolor10[]           = "#00ff00"; /* SGR color #10 */
static char col_sgrcolor11[]           = "#ffff00"; /* SGR color #11 */
static char col_sgrcolor12[]           = "#0000ff"; /* SGR color #12 */
static char col_sgrcolor13[]           = "#ff00ff"; /* SGR color #13 */
static char col_sgrcolor14[]           = "#00ffff"; /* SGR color #14 */
static char col_sgrcolor15[]           = "#ffffff"; /* SGR color #15 */

/* Alpha options */
#define fgalpha                        opaque /* Foreground alpha */
#define bgalpha                        200 /* Background alpha */
#define borderalpha                    opaque /* Border alpha */

/* Misc */
static char worddelimiters[]           = " "; /* Word delimiters, " " is default. */
