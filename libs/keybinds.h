/* These are all your hardcoded keybinds.
 * See LICENSE file for copyright and license details.
 */

/* X11 hardcoded keybinds */
#if USEX
static Key keys[] = {
    { -1,      0,              XK_Return,    selectitem,      {.i = +1 } },
    { -1,      Shift,          XK_Return,    selectitem,      {0} },
    { -1,      Ctrl,           XK_Return,    markitem,        {0} },
    { -1,      0,              XK_Tab,       complete,        {0} },
    { -1,      Ctrl,           XK_v,         paste,           {.i = 2  } },
    { -1,      0,              XK_BackSpace, backspace,       {0} },
    { -1,      Ctrl,           XK_BackSpace, deleteword,      {0} },
    { -1,      Ctrl|Shift,     XK_p,         setprofile,      {0} },
    { -1,      0,              XK_Print,     screenshot,      {0} },
    { -1,      Ctrl,           XK_equal,     setimgsize,      {.i = +10 } },
    { -1,      Ctrl,           XK_minus,     setimgsize,      {.i = -10 } },
    { -1,      0,              XK_k,         moveup,          {0} },
    { -1,      0,              XK_j,         movedown,        {0} },
    { -1,      0,              XK_h,         moveleft,        {0} },
    { -1,      0,              XK_l,         moveright,       {0} },
    { -1,      Ctrl,           XK_u,         moveup,          {.i = 5  } },
    { -1,      Ctrl,           XK_d,         movedown,        {.i = 5  } },
    { -1,      Shift,          XK_h,         viewhist,        {0} },
    { -1,      0,              XK_Escape,    quit,            {0} },
    { -1,      Ctrl,           XK_p,         navhistory,      {.i = -1 } },
    { -1,      Ctrl,           XK_n,         navhistory,      {.i = +1 } },
};
#endif

/* Wayland hardcoded keybinds */
#if USEWAYLAND
static WlKey wl_keys[] = {
    { -1,      WL_None,              XKB_KEY_Return,    selectitem,      {.i = +1 } },
    { -1,      WL_Shift,             XKB_KEY_Return,    selectitem,      {0} },
    { -1,      WL_Ctrl,              XKB_KEY_Return,    markitem,        {0} },
    { -1,      WL_None,              XKB_KEY_Tab,       complete,        {0} },
    { -1,      WL_Ctrl,              XKB_KEY_v,         paste,           {.i = 2  } },
    { -1,      WL_None,              XKB_KEY_BackSpace, backspace,       {0} },
    { -1,      WL_Ctrl,              XKB_KEY_BackSpace, deleteword,      {0} },
    { -1,      WL_CtrlShift,         XKB_KEY_p,         setprofile,      {0} },
    { -1,      WL_None,              XKB_KEY_Print,     screenshot,      {0} },
    { -1,      WL_Ctrl,              XKB_KEY_equal,     setimgsize,      {.i = +10 } },
    { -1,      WL_Ctrl,              XKB_KEY_minus,     setimgsize,      {.i = -10 } },
    { -1,      WL_None,              XKB_KEY_k,         moveup,          {0} },
    { -1,      WL_None,              XKB_KEY_j,         movedown,        {0} },
    { -1,      WL_None,              XKB_KEY_h,         moveleft,        {0} },
    { -1,      WL_None,              XKB_KEY_l,         moveright,       {0} },
    { -1,      WL_Ctrl,              XKB_KEY_u,         moveup,          {.i = 5  } },
    { -1,      WL_Ctrl,              XKB_KEY_d,         movedown,        {.i = 5  } },
    { -1,      WL_Shift,             XKB_KEY_h,         viewhist,        {0} },
    { -1,      WL_None,              XKB_KEY_Escape,    quit,            {0} },
    { -1,      WL_Ctrl,              XKB_KEY_p,         navhistory,      {.i = -1 } },
    { -1,      WL_Ctrl,              XKB_KEY_n,         navhistory,      {.i = +1 } },

};
#endif
