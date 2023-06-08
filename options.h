/* spmenu - fancy dynamic menu
 *
 * Below is a configuration file which is technically C source code.
 * See LICENSE file for copyright and license details.
 */

/* spmenu options */
static char *class                     = "spmenu"; /* Class for spmenu */
static int protocol                    = 1; /* Protocol to try first (0: X11, 1: Wayland) */
static int fast                        = 0; /* Grab keyboard first */
static int xresources                  = 1; /* Enable .Xresources support */
static int globalcolors                = 1; /* Recognize global colors (such as colors generated by Pywal) */
static int loadconfig                  = 1; /* Load configuration (~/.config/spmenu/spmenu.conf) on runtime */
static int loadtheme                   = 1; /* Load theme (~/.config/spmenu/theme.conf) on runtime */
static int loadbinds                   = 1; /* Load keybind file (~/.config/spmenu/binds.conf) on runtime */
static int mon                         = -1; /* Monitor to run spmenu on */

/* Config file options */
#if USECONFIG
static char *configfile                = NULL; /* Config file path. Default is ~/.config/spmenu/spmenu.conf */
static char *themefile                 = NULL; /* Theme file path. Default is ~/.config/spmenu/theme.conf */
static char *bindsfile                 = NULL; /* Keybind file path. Default is ~/.config/spmenu/binds.conf */
#endif

/* Window options */
static int alpha                       = 1; /* Enable alpha */
static int menuposition                = 2; /* Position of the menu (0: Bottom, 1: Top, 2: Center */
static int menupaddingv				   = 0; /* Vertical padding inside the menu (in pixels) */
static int menupaddingh				   = 0; /* Horizontal padding inside the menu (in pixels) */
static int menuwidth                   = 0; /* spmenu width */
static int menumarginv                 = 0; /* Vertical padding around the menu */
static int menumarginh                 = 0; /* Horizontal padding around the menu */
static int minwidth                    = 1000; /* Minimum width when centered */
static int xpos                        = 0; /* X position to offset spmenu */
static int ypos                        = 0; /* Y position to offset spmenu */
static int managed                     = 0; /* Let your window manager manage spmenu? */

/* Powerline options */
static int powerlineprompt             = 1; /* Enable powerline for the prompt */
static int powerlinecount              = 1; /* Enable powerline for the match count */
static int powerlinemode               = 1; /* Enable powerline for the mode indicator */
static int powerlinecaps               = 1; /* Enable powerline for the caps lock indicator */
static int promptpwlstyle              = 2; /* Prompt powerline style (0: >, 1: \, 2: )) */
static int matchcountpwlstyle          = 2; /* Match count powerline style (0: <, 1: /, 2: () */
static int modepwlstyle                = 2; /* Mode indicator powerline style (0: <, 1: /, 2: () */
static int capspwlstyle                = 2; /* Caps lock indicator powerline style (0: <, 1: /, 2: () */

/* Window properties */
static int dockproperty                = 1; /* Set _NET_WM_WINDOW_TYPE_DOCK */

/* Image options */
static int imagewidth                  = 200; /* Default image width */
static int imageheight                 = 200; /* Default image height */
static int imagegaps                   = 0; /* Image gaps */
static int imageposition               = 0; /* Image position (0: Top, 1: Bottom, 2: Center, 3: Top center) */
static int generatecache               = 1; /* Generate image cache by default */
static int maxcache                    = 512; /* Max image size to cache */
static char *cachedir                  = "default"; /* Cache directory. Default means spmenu will determine automatically */

/* Mode options */
static int mode                        = 0; /* Mode to start speedwm in (0: Normal mode, 1: Insert mode) */
static char *normtext                  = "Normal"; /* Text to display for normal mode */
static char *instext                   = "Insert"; /* Text to display for insert mode */
static char *capslockontext            = "Caps Lock"; /* Text to display for the caps lock indicator when caps lock is on */
static char *capslockofftext           = ""; /* Text to display for the caps lock indicator when caps lock is off */

/* Window border options */
static int borderwidth                 = 0; /* Width of the border */

/* Font options */
static char font[]                     = "Noto Sans Mono 8"; /* Font to draw text and Pango markup with. */
static int textpadding                 = 0; /* Global text padding */
static int normitempadding             = 0; /* Text padding for normal items */
static int selitempadding              = 0; /* Text padding for the selected item */
static int priitempadding              = 0; /* Text padding for the high priority items */

/* Text options */
static char *leftarrow                 = "<"; /* Left arrow, used to indicate you can move to the left */
static char *rightarrow                = ">"; /* Right arrow, used to indicate you can move to the right */
static char *password                  = "*"; /* Password character, when the -P argument is active this will replace all characters typed */
static char *prompt                    = NULL; /* Default prompt */
static char *input                     = NULL; /* Default input text */

/* Match options */
static int type                        = 1; /* Allow typing into spmenu or only allow keybinds. */
static int passwd                      = 0; /* Replace input with another character and don't read stdin */
static int sortmatches                 = 1; /* Sort matches (0/1) */
static int casesensitive               = 0; /* Case-sensitive by default? (0/1) */
static int mark                        = 1; /* Enable marking items (multi selection) (0/1) */
static int preselected                 = 0; /* Which line should spmenu preselect? */
static int fuzzy                       = 1; /* Whether or not to enable fuzzy matching by default */
static char *listfile                  = NULL; /* File to read entries from instead of stdin. NULL means read from stdin instead. */

/* Line options */
static int lineheight                  = 1; /* Line height (0: Calculate automatically) */
static int lines                       = 0; /* Default number of lines */
static int columns                     = 10; /* Default number of columns */

/* History options */
static int maxhist                     = 64; /* Max number of history entries */
static int histdup                     = 0;	/* If 1, record repeated histories */

/* Prompt options */
static int indentitems                 = 1; /* Indent items to prompt width? (0/1) */

/* Hide options */
static int hideinput                   = 0; /* Hide input (0/1) */
static int hidelarrow                  = 0; /* Hide left arrow (0/1) */
static int hiderarrow                  = 0; /* Hide right arrow (0/1) */
static int hideitem                    = 0; /* Hide item (0/1) */
static int hideprompt                  = 0; /* Hide prompt (0/1) */
static int hidecaps                    = 0; /* Hide caps lock indicator (0/1) */
static int hidepowerline               = 0; /* Hide powerline (0/1) */
static int hidecaret                   = 0; /* Hide caret (0/1) */
static int hidehighlight               = 0; /* Hide highlight (0/1) */
static int hidematchcount              = 0; /* Hide match count (0/1) */
static int hidemode                    = 0; /* Hide mode (0/1) */
static int hideimage                   = 0; /* Hide image (0/1) */

/* Color options
 *
 * Item colors */
static char col_itemnormfg[]           = "#bbbbbb"; /* Normal foreground item color */
static char col_itemnormbg[]           = "#222222"; /* Normal background item color */
static char col_itemnormfg2[]          = "#bbbbbb"; /* Normal foreground item colors for the next item */
static char col_itemnormbg2[]          = "#222222"; /* Normal background item colors for the next item */
static char col_itemselfg[]            = "#eeeeee"; /* Selected foreground item color */
static char col_itemselbg[]            = "#35638A"; /* Selected background item color */
static char col_itemmarkedfg[]         = "#eeeeee"; /* Marked foreground item color */
static char col_itemmarkedbg[]         = "#35638A"; /* Marked background item color */
static char col_itemnormprifg[]        = "#bbbbbb"; /* Normal foreground item (high priority) color */
static char col_itemnormpribg[]        = "#222222"; /* Normal background item (high priority) color */
static char col_itemselprifg[]         = "#eeeeee"; /* Selected foreground item (high priority) color */
static char col_itemselpribg[]         = "#35638A"; /* Selected background item (high priority) color */

/* Input colors */
static char col_inputbg[]              = "#222222"; /* Input field background color */
static char col_inputfg[]              = "#eeeeee"; /* Input field foreground color */

/* Menu colors */
static char col_menu[]                 = "#222222"; /* Menu background color */

/* Prompt colors */
static char col_promptfg[]             = "#eeeeee"; /* Prompt foreground color */
static char col_promptbg[]             = "#35526b"; /* Prompt background color */

/* Arrow colors */
static char col_larrowfg[]             = "#bbbbbb"; /* Left arrow color */
static char col_rarrowfg[]             = "#bbbbbb"; /* Right arrow color */
static char col_larrowbg[]             = "#222222"; /* Left arrow color */
static char col_rarrowbg[]             = "#222222"; /* Right arrow color */

/* Normal highlight colors */
static char col_hlnormfg[]             = "#ffffff"; /* Normal text highlight color */
static char col_hlnormbg[]             = "#000000"; /* Normal background highlight color */

/* Selected highlight colors */
static char col_hlselfg[]              = "#ffffff"; /* Selected text highlight color */
static char col_hlselbg[]              = "#000000"; /* Selected background highlight color */

/* Match count colors */
static char col_numfg[]                = "#ffffff"; /* Match count text color */
static char col_numbg[]                = "#2d3856"; /* Match count background color */

/* Border color */
static char col_border[]               = "#35638A"; /* Border color */

/* Caret colors */
static char col_caretfg[]              = "#ffffff"; /* Foreground caret color */
static char col_caretbg[]              = "#222222"; /* Background caret color */

/* Mode colors */
static char col_modefg[]               = "#ffffff"; /* Mode text color */
static char col_modebg[]               = "#35638A"; /* Mode background color */

/* Caps lock colors */
static char col_capsfg[]               = "#ffffff"; /* Caps lock text color */
static char col_capsbg[]               = "#45638A"; /* Caps lock background color */

/* SGR colors */
static char col_sgr0[]                 = "#20201d"; /* SGR color #0 */
static char col_sgr1[]                 = "#d73737"; /* SGR color #1 */
static char col_sgr2[]                 = "#60ac39"; /* SGR color #2 */
static char col_sgr3[]                 = "#cfb017"; /* SGR color #3 */
static char col_sgr4[]                 = "#6684e1"; /* SGR color #4 */
static char col_sgr5[]                 = "#b854d4"; /* SGR color #5 */
static char col_sgr6[]                 = "#1fad83"; /* SGR color #6 */
static char col_sgr7[]                 = "#a6a28c"; /* SGR color #7 */
static char col_sgr8[]                 = "#7d7a68"; /* SGR color #8 */
static char col_sgr9[]                 = "#d73737"; /* SGR color #9 */
static char col_sgr10[]                = "#60ac39"; /* SGR color #10 */
static char col_sgr11[]                = "#cfb017"; /* SGR color #11 */
static char col_sgr12[]                = "#6684e1"; /* SGR color #12 */
static char col_sgr13[]                = "#b854d4"; /* SGR color #13 */
static char col_sgr14[]                = "#1fad83"; /* SGR color #14 */
static char col_sgr15[]                = "#fefbec"; /* SGR color #15 */

/* Alpha options */
static int alpha_itemnormfg            = 255; /* Alpha for normal item foreground (0-255) */
static int alpha_itemnormbg            = 200; /* Alpha for normal item background (0-255) */
static int alpha_itemnormfg2           = 255; /* Alpha for next normal item foreground (0-255) */
static int alpha_itemnormbg2           = 200; /* Alpha for next normal item background (0-255) */
static int alpha_itemselfg             = 255; /* Alpha for selected item foreground (0-255) */
static int alpha_itemselbg             = 200; /* Alpha for selected item background (0-255) */
static int alpha_itemmarkedfg          = 255; /* Alpha for marked item foreground (0-255) */
static int alpha_itemmarkedbg          = 200; /* Alpha for marked item background (0-255) */
static int alpha_itemnormprifg         = 255; /* alpha for normal priority item foreground (0-255) */
static int alpha_itemnormpribg         = 200; /* Alpha for normal priority item background (0-255) */
static int alpha_itemselprifg          = 255; /* Alpha for selected priority item foreground (0-255) */
static int alpha_itemselpribg          = 200; /* Alpha for selected priority item background (0-255) */
static int alpha_inputfg               = 255; /* Alpha for input foreground (0-255) */
static int alpha_inputbg               = 200; /* Alpha for input background (0-255) */
static int alpha_menu                  = 200; /* Alpha for menu background (0-255) */
static int alpha_promptfg              = 255; /* Alpha for prompt foreground (0-255) */
static int alpha_promptbg              = 200; /* Alpha for prompt background (0-255) */
static int alpha_larrowfg              = 255; /* Alpha for left arrow foreground (0-255) */
static int alpha_larrowbg              = 200; /* Alpha for left arrow background (0-255) */
static int alpha_rarrowfg              = 255; /* Alpha for right arrow foreground (0-255) */
static int alpha_rarrowbg              = 200; /* Alpha for right arrow background (0-255) */
static int alpha_hlnormfg              = 255; /* Alpha for normal highlight foreground (0-255) */
static int alpha_hlnormbg              = 200; /* Alpha for normal highlight background (0-255) */
static int alpha_hlselfg               = 255; /* Alpha for selected highlight foreground (0-255) */
static int alpha_hlselbg               = 200; /* Alpha for selected highlight background (0-255) */
static int alpha_numfg                 = 255; /* Alpha for match count foreground (0-255) */
static int alpha_numbg                 = 200; /* Alpha for the match count background (0-255) */
static int alpha_border                = 255; /* Alpha for the border (0-255) */
static int alpha_caretfg               = 255; /* Alpha for the caret foreground (0-255) */
static int alpha_caretbg               = 200; /* Alpha for the caret background (0-255) */
static int alpha_modefg                = 255; /* Alpha for the mode indicator foreground (0-255) */
static int alpha_modebg                = 200; /* Alpha for the mode indicator background (0-255) */
static int alpha_capsfg                = 255; /* Alpha for the caps lock indicator foreground (0-255) */
static int alpha_capsbg                = 200; /* Alpha for the caps lock indicator background (0-255) */

/* Pango options */
static int pango_item                  = 1; /* Enable support for pango markup for the items */
static int pango_highlight             = 1; /* Enable support for pango markup for the highlighting */
static int pango_prompt                = 1; /* Enable support for pango markup for the prompt */
static int pango_caps                  = 1; /* Enable support for pango markup for the caps lock indicator */
static int pango_input                 = 1; /* Enable support for pango markup for user input */
static int pango_leftarrow             = 0; /* Enable support for pango markup for the left arrow */
static int pango_rightarrow            = 0; /* Enable support for pango markup for the right arrow */
static int pango_numbers               = 0; /* Enable support for pango markup for the match count */
static int pango_mode                  = 0; /* Enable support for pango markup for the mode indicator */
static int pango_password              = 0; /* Enable support for pango markup for the password text */

/* Misc */
static int printindex                  = 0; /* Print index instead of the text itself (0/1) */
static int requirematch                = 0; /* Require input text to match an item (0/1) */
static int incremental                 = 0; /* Print text every time a key is pressed (0/1) */
static int coloritems                  = 1; /* Color items (0/1) */
static int sgr                         = 1; /* Support SGR sequences (0/1) */
static char *worddelimiters            = " "; /* Word delimiters used for keybinds that change words, Space is default. */
