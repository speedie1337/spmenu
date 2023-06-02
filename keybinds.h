/* These are all your hardcoded keybinds.
 * It is recommended you set keybinds in ~/.config/spmenu/spmenu.conf instead.
 *
 * See https://spmenu.speedie.site/index.php/keybinds.h+documentation for more information.
 * See LICENSE file for copyright and license details.
 */

/* X11 hardcoded keybinds */
static Key keys[] = {
    /* mode    modifier        key           function         argument
     *
     * any mode
     */
    { -1,      0,              XK_Return,    selectitem,      {.i = +1 } },
    { -1,      Shift,          XK_Return,    selectitem,      {0} },
    { -1,      Ctrl,           XK_Return,    markitem,        {0} },
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
    { 0,       0,              XK_o,         setimgpos,       {.i = +1 } },
    { 0,       Ctrl,           XK_1,         setimggaps,      {.i = -1 } },
    { 0,       Ctrl,           XK_2,         setimggaps,      {.i = +1 } },
    { 0,       0,              XK_1,         setimggaps,      {.i = -10 } },
    { 0,       0,              XK_2,         setimggaps,      {.i = +10 } },
    { 0,       Shift,          XK_1,         setimggaps,      {.i = -100 } },
    { 0,       Shift,          XK_2,         setimggaps,      {.i = +100 } },
    { 0,       0,              XK_t,         toggleimg,       {0} },
    { 0,       0,              XK_f,         togglefullimg,   {0} },
    { 0,       0,              XK_p,         paste,           {.i = 2  } },
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
    { 0,       Shift,          XK_d,         clearins,        {0} },
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

/* Wayland hardcoded keybinds */
#if USEWAYLAND
static WlKey wl_keys[] = {
    /* mode    modifier              key                function         argument
     *
     * any mode
     */
    { -1,      WL_None,              XKB_KEY_Return,    selectitem,      {.i = +1 } },
    { -1,      WL_Shift,             XKB_KEY_Return,    selectitem,      {0} },
    { -1,      WL_Ctrl,              XKB_KEY_Return,    markitem,        {0} },
    { -1,      WL_None,              XKB_KEY_Tab,       complete,        {0} },
    { -1,      WL_Ctrl,              XKB_KEY_v,         paste,           {.i = 2  } },
    { -1,      WL_CtrlShift,         XKB_KEY_v,         paste,           {.i = 1  } },
    { -1,      WL_None,              XKB_KEY_BackSpace, backspace,       {0} },
    { -1,      WL_Ctrl,              XKB_KEY_BackSpace, deleteword,      {0} },
    { -1,      WL_Ctrl,              XKB_KEY_Left,      moveword,        {.i = -1 } },
    { -1,      WL_Ctrl,              XKB_KEY_Right,     moveword,        {.i = +1 } },
    { -1,      WL_None,              XKB_KEY_Left,      movecursor,      {.i = -1 } },
    { -1,      WL_None,              XKB_KEY_Right,     movecursor,      {.i = +1 } },
    { -1,      WL_CtrlShift,         XKB_KEY_p,         setprofile,      {0} },

    /* normal mode */
    { 0,       WL_None,              XKB_KEY_i,         switchmode,      {0} },
    { 0,       WL_None,              XKB_KEY_slash,     switchmode,      {0} },
    { 0,       WL_Ctrl,              XKB_KEY_equal,     setimgsize,      {.i = +1 } },
    { 0,       WL_Ctrl,              XKB_KEY_minus,     setimgsize,      {.i = -1 } },
    { 0,       WL_None,              XKB_KEY_equal,     setimgsize,      {.i = +10 } },
    { 0,       WL_None,              XKB_KEY_minus,     setimgsize,      {.i = -10 } },
    { 0,       WL_Shift,             XKB_KEY_equal,     setimgsize,      {.i = +100 } },
    { 0,       WL_Shift,             XKB_KEY_minus,     setimgsize,      {.i = -100 } },
    { 0,       WL_Shift,             XKB_KEY_0,         defaultimg,      {0} },
    { 0,       WL_None,              XKB_KEY_r,         rotateimg,       {0} },
    { 0,       WL_None,              XKB_KEY_o,         setimgpos,       {.i = +1 } },
    { 0,       WL_Ctrl,              XKB_KEY_1,         setimggaps,      {.i = -1 } },
    { 0,       WL_Ctrl,              XKB_KEY_2,         setimggaps,      {.i = +1 } },
    { 0,       WL_None,              XKB_KEY_1,         setimggaps,      {.i = -10 } },
    { 0,       WL_None,              XKB_KEY_2,         setimggaps,      {.i = +10 } },
    { 0,       WL_Shift,             XKB_KEY_1,         setimggaps,      {.i = -100 } },
    { 0,       WL_Shift,             XKB_KEY_2,         setimggaps,      {.i = +100 } },
    { 0,       WL_None,              XKB_KEY_t,         toggleimg,       {0} },
    { 0,       WL_None,              XKB_KEY_f,         togglefullimg,   {0} },
    { 0,       WL_None,              XKB_KEY_p,         paste,           {.i = 2  } },
    { 0,       WL_None,              XKB_KEY_h,         flipimg,         {.i = 1  } },
    { 0,       WL_None,              XKB_KEY_v,         flipimg,         {.i = 0  } },
    { 0,       WL_None,              XKB_KEY_k,         moveup,          {0} },
    { 0,       WL_None,              XKB_KEY_j,         movedown,        {0} },
    { 0,       WL_None,              XKB_KEY_h,         moveleft,        {0} },
    { 0,       WL_None,              XKB_KEY_l,         moveright,       {0} },
    { 0,       WL_Ctrl,              XKB_KEY_u,         moveup,          {.i = 5  } },
    { 0,       WL_Ctrl,              XKB_KEY_d,         movedown,        {.i = 5  } },
    { 0,       WL_Ctrl,              XKB_KEY_k,         setlines,        {.i = +1 } },
    { 0,       WL_Ctrl,              XKB_KEY_j,         setlines,        {.i = -1 } },
    { 0,       WL_CtrlAltShift,      XKB_KEY_k,         setlines,        {.i = +5 } },
    { 0,       WL_CtrlAltShift,      XKB_KEY_j,         setlines,        {.i = -5 } },
    { 0,       WL_Ctrl,              XKB_KEY_h,         setcolumns,      {.i = +1 } },
    { 0,       WL_Ctrl,              XKB_KEY_l,         setcolumns,      {.i = -1 } },
    { 0,       WL_CtrlAltShift,      XKB_KEY_h,         setcolumns,      {.i = +5 } },
    { 0,       WL_CtrlAltShift,      XKB_KEY_l,         setcolumns,      {.i = -5 } },
    { 0,       WL_None,              XKB_KEY_u,         togglehighlight, {0} },
    { 0,       WL_CtrlShift,         XKB_KEY_h,         viewhist,        {0} },
    { 0,       WL_None,              XKB_KEY_d,         clear,           {0} },
    { 0,       WL_Shift,             XKB_KEY_d,         clearins,        {0} },
    { 0,       WL_None,              XKB_KEY_Escape,    quit,            {0} },
    { 0,       WL_None,              XKB_KEY_Home,      movestart,       {0} },
    { 0,       WL_None,              XKB_KEY_End,       moveend,         {0} },
    { 0,       WL_None,              XKB_KEY_g,         movestart,       {0} },
    { 0,       WL_Shift,             XKB_KEY_g,         moveend,         {0} },
    { 0,       WL_None,              XKB_KEY_Next,      movenext,        {0} },
    { 0,       WL_None,              XKB_KEY_Prior,     moveprev,        {0} },
    { 0,       WL_Alt,               XKB_KEY_p,         navhistory,      {.i = -1 } },
    { 0,       WL_Alt,               XKB_KEY_n,         navhistory,      {.i = +1 } },

    /* insert mode */
    { 1,       WL_None,              XKB_KEY_Escape,    switchmode,      {0} },
};
#endif
