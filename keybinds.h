/* These are all your hardcoded keybinds.
 * It is recommended you set keybinds in ~/.config/spmenu/spmenu.conf instead.
 *
 * See https://spmenu.speedie.site/index.php/keybinds.h+documentation for more information.
 */

static Key keys[] = {
    /* mode    modifier        key           function         argument
     *
     * any mode
     */
    { -1,      0,              XK_Return,    selectitem,      {.i = +1 } },
    { -1,      Shift,          XK_Return,    selectitem,      {0} },
    { -1,      0,              XK_Tab,       complete,        {0} },
    { -1,      Ctrl,           XK_v,         paste,           {.i = 2  } },
    { -1,      Ctrl|Shift,     XK_v,         paste,           {.i = 1  } },
    { -1,      0,              XK_BackSpace, backspace,       {0} },
    { -1,      Ctrl,           XK_BackSpace, deleteword,      {0} },
    { -1,      Ctrl,           XK_Left,      moveword,        {.i = -1 } },
    { -1,      Ctrl,           XK_Right,     moveword,        {.i = +1 } },
    { -1,      0,              XK_Left,      movecursor,      {.i = -1 } },
    { -1,      0,              XK_Right,     movecursor,      {.i = +1 } },
    { -1,      Ctrl|Shift,     XK_p,         setprofile,      {0} },

    /* normal mode */
    { 0,       0,              XK_i,         switchmode,      {0} },
    { 0,       0,              XK_slash,     switchmode,      {0} },
    { 0,       Ctrl,           XK_equal,     setimgsize,      {.i = +1 } },
    { 0,       Ctrl,           XK_minus,     setimgsize,      {.i = -1 } },
    { 0,       0,              XK_equal,     setimgsize,      {.i = +10 } },
    { 0,       0,              XK_minus,     setimgsize,      {.i = -10 } },
    { 0,       Shift,          XK_equal,     setimgsize,      {.i = +100 } },
    { 0,       Shift,          XK_minus,     setimgsize,      {.i = -100 } },
    { 0,       Shift,          XK_0,         defaultimg,      {0} },
    { 0,       0,              XK_r,         rotateimg,       {0} },
    { 0,       0,              XK_p,         setimgpos,       {.i = +1 } },
    { 0,       Ctrl,           XK_1,         setimggaps,      {.i = -1 } },
    { 0,       Ctrl,           XK_2,         setimggaps,      {.i = +1 } },
    { 0,       0,              XK_1,         setimggaps,      {.i = -10 } },
    { 0,       0,              XK_2,         setimggaps,      {.i = +10 } },
    { 0,       Shift,          XK_1,         setimggaps,      {.i = -100 } },
    { 0,       Shift,          XK_2,         setimggaps,      {.i = +100 } },
    { 0,       0,              XK_t,         toggleimg,       {0} },
    { 0,       0,              XK_h,         flipimg,         {.i = 1  } },
    { 0,       0,              XK_v,         flipimg,         {.i = 0  } },
    { 0,       0,              XK_k,         moveup,          {0} },
    { 0,       0,              XK_j,         movedown,        {0} },
    { 0,       0,              XK_h,         moveleft,        {0} },
    { 0,       0,              XK_l,         moveright,       {0} },
    { 0,       Ctrl,           XK_u,         moveup,          {.i = 5  } },
    { 0,       Ctrl,           XK_d,         movedown,        {.i = 5  } },
    { 0,       Ctrl,           XK_k,         setlines,        {.i = +1 } },
    { 0,       Ctrl,           XK_j,         setlines,        {.i = -1 } },
    { 0,       Ctrl|Alt|Shift, XK_k,         setlines,        {.i = +5 } },
    { 0,       Ctrl|Alt|Shift, XK_j,         setlines,        {.i = -5 } },
    { 0,       Ctrl,           XK_h,         setcolumns,      {.i = +1 } },
    { 0,       Ctrl,           XK_l,         setcolumns,      {.i = -1 } },
    { 0,       Ctrl|Alt|Shift, XK_h,         setcolumns,      {.i = +5 } },
    { 0,       Ctrl|Alt|Shift, XK_l,         setcolumns,      {.i = -5 } },
    { 0,       0,              XK_u,         togglehighlight, {0} },
    { 0,       Ctrl|Shift,     XK_h,         viewhist,        {0} },
    { 0,       0,              XK_d,         clear,           {0} },
    { 0,       0,              XK_c,         clearins,        {0} },
    { 0,       0,              XK_Escape,    quit,            {0} },
    { 0,       0,              XK_Home,      movestart,       {0} },
    { 0,       0,              XK_End,       moveend,         {0} },
    { 0,       0,              XK_g,         movestart,       {0} },
    { 0,       Shift,          XK_g,         moveend,         {0} },
    { 0,       0,              XK_Next,      movenext,        {0} },
    { 0,       0,              XK_Prior,     moveprev,        {0} },
    { 0,       Alt,            XK_p,         navhistory,      {.i = -1 } },
    { 0,       Alt,            XK_n,         navhistory,      {.i = +1 } },

    /* insert mode */
    { 1,       0,              XK_Escape,    switchmode,      {0} },
};
