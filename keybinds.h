/* These are all your keybinds.
 *
 * Types
 *
 * MODIFIER1 is what you defined below, default is Super)
 *
 * SHIFT is unless changed going to be your Shift key.
 * CONTROL is unless changed going to be your Control key.
 * ALT unless changed is going to be your left Alt key.
 * ALTR unless changed is going to be your right Alt key.
 * SUPER unless changed is going to be your left Super (Windows/macOS Command) key.
 * SUPERR unless changed is going to be your right Super (Windows/macOS Command) key.
 *
 * Example keybind:
 *
 * { MODIFIER1, XK_1, myfunction, ${0} },
 *
 * Once you're done with your edits, run 'make clean install'.
 */

/* Modes
 * -1: Any mode
 * 0:  Normal mode
 * 1:  Insert mode
 *
 * Switch mode by calling switchmode
 */

/* Modifier keys
 * Mod4Mask | Super (Windows/command) key
 * Mod1Mask | Alt key
 *
 * NOTE: You do NOT want this to be the same as your window manager modifier.
 */
#define MODIFIER1              Mod1Mask

static Key keys[] = {
	/* insert mode
     *
     * mode    modifier                      key           function         argument  */
	{ 1,       0,                            XK_Escape,    switchmode,      {0} },

	/* normal mode
     *
     * mode    modifier                      key           function         argument  */
	{ 0,       0,                            XK_i,         switchmode,      {0} },
    { 0,       CONTROL,                      XK_equal,     setimgsize,      {.i = +1 } },
    { 0,       CONTROL,                      XK_minus,     setimgsize,      {.i = -1 } },
    { 0,       0,                            XK_equal,     setimgsize,      {.i = +10 } },
    { 0,       0,                            XK_minus,     setimgsize,      {.i = -10 } },
    { 0,       SHIFT,                        XK_equal,     setimgsize,      {.i = +100 } },
    { 0,       SHIFT,                        XK_minus,     setimgsize,      {.i = -100 } },
    { 0,       SHIFT,                        XK_0,         defaultimg,      {0} },
    { 0,       0,                            XK_r,         rotateimg,       {0} },
    { 0,       0,                            XK_p,         setimgpos,       {.i = +1 } },
    { 0,       CONTROL,                      XK_1,         setimggaps,      {.i = -1 } },
    { 0,       CONTROL,                      XK_2,         setimggaps,      {.i = +1 } },
    { 0,       0,                            XK_1,         setimggaps,      {.i = -10 } },
    { 0,       0,                            XK_2,         setimggaps,      {.i = +10 } },
    { 0,       SHIFT,                        XK_1,         setimggaps,      {.i = -100 } },
    { 0,       SHIFT,                        XK_2,         setimggaps,      {.i = +100 } },
    { 0,       0,                            XK_t,         toggleimg,       {0} },
    { 0,       0,                            XK_h,         flipimg,         {.i = 1  } },
    { 0,       0,                            XK_v,         flipimg,         {.i = 0  } },
	{ 0,       0,                            XK_k,         move,            {.i = 1  } },
	{ 0,       0,                            XK_j,         move,            {.i = 2  } },
	{ 0,       0,                            XK_h,         move,            {.i = 3  } },
	{ 0,       0,                            XK_l,         move,            {.i = 4  } },
	{ 0,       CONTROL,                      XK_k,         setlines,        {.i = +1 } },
	{ 0,       CONTROL,                      XK_j,         setlines,        {.i = -1 } },
	{ 0,       CONTROL,                      XK_h,         setcolumns,      {.i = +1 } },
	{ 0,       CONTROL,                      XK_l,         setcolumns,      {.i = -1 } },
    { 0,       CONTROL,                      XK_k,         restoresel,      {0} },
    { 0,       CONTROL|SHIFT,                XK_h,         viewhist,        {0} },
    { 0,       CONTROL,                      XK_u,         clear,           {0} },
    { 0,       0,                            XK_Escape,    quit,            {0} },
    { 0,       0,                            XK_Home,      movestart,       {0} },
    { 0,       0,                            XK_End,       moveend,         {0} },
    { 0,       0,                            XK_g,         movestart,       {0} },
    { 0,       SHIFT,                        XK_g,         moveend,         {0} },
    { 0,       0,                            XK_Next,      movenext,        {0} },
    { 0,       0,                            XK_Prior,     moveprev,        {0} },
    { 0,       MODIFIER1,                    XK_b,         moveword,        {.i = -1 } },
    { 0,       MODIFIER1,                    XK_f,         moveword,        {.i = +1 } },
    { 0,       MODIFIER1,                    XK_p,         navhistory,      {.i = -1 } },
    { 0,       MODIFIER1,                    XK_n,         navhistory,      {.i = +1 } },

    /* any mode
     *
     * mode    modifier                      key           function         argument  */
    { -1,      0,                            XK_Return,    selectitem,      {.i = +1 } },
    { -1,      0,                            XK_Tab,       complete,        {0} },
    { -1,      CONTROL,                      XK_v,         paste,           {.i = 1  } }, /* primary buffer */
    { -1,      CONTROL|SHIFT,                XK_v,         paste,           {.i = 2  } },
	{ -1,      0,                            XK_Up,        move,            {.i = 1  } },
	{ -1,      0,                            XK_Down,      move,            {.i = 2  } },
	{ -1,      0,                            XK_Left,      move,            {.i = 3  } },
	{ -1,      0,                            XK_Right,     move,            {.i = 4  } },
    { -1,      0,                            XK_BackSpace, backspace,       {0} },
};
