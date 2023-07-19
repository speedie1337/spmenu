/* Mouse bindings
 * See LICENSE file for copyright and license details.
 */

/* Only applies to X11 */
#if USEX
static Mouse buttons[] = {
    { ClickInput,           Button1,         clear,        {0} },
    { ClickPrompt,          Button1,         clear,        {0} },
    { ClickMode,            Button1,         switchmode,   {0} },
    { ClickNumber,          Button1,         viewhist,     {0} },
    { ClickItem,            Button1,         selecthover,  {0} },
    { ClickNone,            Button5,         movenext,     {0} },
    { ClickNone,            Button4,         moveprev,     {0} },
};
#endif

/* Only applies to Wayland */
#if USEWAYLAND
static WlMouse wl_buttons[] = {
    { ClickInput,           WL_Left,         clear,        {0} },
    { ClickPrompt,          WL_Left,         clear,        {0} },
    { ClickMode,            WL_Left,         switchmode,   {0} },
    { ClickNumber,          WL_Left,         viewhist,     {0} },
    { ClickItem,            WL_Left,         selecthover,  {0} },
    { ClickNone,            WL_Down,         movenext,     {0} },
    { ClickNone,            WL_Up,           moveprev,     {0} },
};
#endif
