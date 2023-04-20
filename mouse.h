/* Mouse bindings
 *
 * This header contains mouse binds.
 * Change them or remove them if you prefer. You can also add more if you want.
 *
 * clickwindow:  - spmenu window/client
 * clickinput:   - Input box
 * clickprompt:  - Prompt
 * clicklarrow:  - Left arrow
 * clickitem:    - Item
 * clickselitem: - Item hovered over (function doesn't matter, this will always select the item)
 * clickrarrow:  - Right arrow
 * clicknumber:  - Match count
 * clickmode:    - Mode indicator
 * clickcaps:    - Caps lock indicator
 *
 * Button1       - Left click
 * Button2       - Middle click
 * Button3       - Right click
 * Button4       - Scroll up
 * Button5       - Scroll down
 *
 * Note that clicking on an item will select it, as of now it's hardcoded.
 */

static Mouse buttons[] = {
    { clickinput,           0,         Button1,         clear,        {0} },
    { clickprompt,          0,         Button1,         clear,        {0} },
    { clickmode,            0,         Button1,         switchmode,   {0} },
    { clicknumber,          0,         Button1,         viewhist,     {0} },
    { clickselitem,         0,         Button1,         NULL,         {0} },
};
