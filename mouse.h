/* Mouse bindings
 * These are all your hardcoded mouse bindings. It is recommended you set these in ~/.config/spmenu/spmenu.conf though.
 *
 * See https://spmenu.speedie.site/index.php/mouse.h+documentation for more information.
 * See LICENSE file for copyright and license details.
 */

/* Only applies to X11 */
#if USEX
static Mouse buttons[] = {
    { ClickInput,           Button1,         clear,        {0} },
    { ClickPrompt,          Button1,         clear,        {0} },
    { ClickMode,            Button1,         switchmode,   {0} },
    { ClickNumber,          Button1,         viewhist,     {0} },
    { ClickSelItem,         Button1,         NULL,         {0} },
};
#endif

/* Only applies to Wayland */
#if USEWAYLAND
static WlMouse wl_buttons[] = {
    { ClickInput,           WL_Left,         clear,        {0} },
    { ClickPrompt,          WL_Left,         clear,        {0} },
    { ClickMode,            WL_Left,         switchmode,   {0} },
    { ClickNumber,          WL_Left,         viewhist,     {0} },
    { ClickSelItem,         WL_Left,         NULL,         {0} },
};
#endif
