spmenu
======
![image](/docs/preview.png)

## What is spmenu?

spmenu is a program for X11 that reads standard input and allows the user to
select items separated by a newline. It is a fork of suckless's dmenu which is
a good more minimal alternative to spmenu.

Unlike dmenu and the many builds out there, spmenu has features like color
support, Vim-like modes, image support, proper keybind configuration, and more.

Most of these features can be disabled during compile time by editing `toggle.mk`.

## Usage

On runtime, spmenu reads from standard input (stdin). spmenu items are
separated by a newline (`\n`). When (by default) Enter is pressed, the selected
item will be piped to stdout.

This allows things like `printf "Apple\nOrange\nPear\n" | spmenu`. This command
will spawn an spmenu window with three items, 'Apple', 'Orange' and 'Pear'.
This can be used in shell scripts to create interactive menus.

On top of this, you can specify arguments to change the behavior of spmenu.
See a list below for a list.

## Arguments

Miscellanious:

- spmenu -l,       --lines line                              - Set line count to stdin
- spmenu -mh,      --lineheight height                       - Set spmenu line height to height
- spmenu -g,       --columns grid                            - Set the number of grids to grid
- spmenu -gc,      --generate-cache                          - Generate image cache
- spmenu -ngc,     --no-generate-cache                       - Don't generate image cache
- spmenu -rw,      --relative-width                          - Enable relative input width
- spmenu -nrw,     --no-relative-width                       - Disable relative input width
- spmenu -f,       --fast                                    - Grabs keyboard before reading stdin
- spmenu -F,       --fuzzy                                   - Enable fuzzy matching
- spmenu -NF,      --no-fuzzy                                - Disable fuzzy matching
- spmenu -P,       --password                                - Hide characters
- spmenu -Ps,      --password-symbol symbol                  - Set the password symbol to symbol
- spmenu -p,       --prompt text                             - Set spmenu prompt text to text
- spmenu -ip,      --indent                                  - Indent items to prompt width
- spmenu -nip,     --no-indent                               - Don't indent items to prompt width
- spmenu -a,       --alpha                                   - Enable alpha
- spmenu -na,      --no-alpha                                - Disable alpha
- spmenu -tp,      --allow-typing                            - Allow the user to type
- spmenu -nt,      --no-allow-typing                         - Don't allow typing, the user must select an option
- spmenu -x,       --x-position x offset                     - Offset spmenu x position by x offset
- spmenu -y,       --y-position y offset                     - Offset spmenu y position by y offset
- spmenu -n,       --preselect line                          - Preselect line   in the list of items
- spmenu -z,       --width width                             - Width of the spmenu window
- spmenu -nmt,     --normal-mode-text text                   - Set normal mode text to text
- spmenu -imt,     --insert-mode-text text                   - Set insert mode text to text
- spmenu -bw,      --border-width width                      - Set width of the border to width  . 0 will disable the border
- spmenu -so,      --sort                                    - Sort matches
- spmenu -nso,     --no-sort                                 - Don't sort matches
- spmenu -pri,     --priority pri1,pri2,pri3                 - Specify a list of items that take priority
- spmenu -s,       --case-sensitive                          - Use case-sensitive matching
- spmenu -ns,      --case-insensitive                        - Use case-insensitive matching
- spmenu -nm,      --normal                                  - Start spmenu in normal mode
- spmenu -im,      --insert                                  - Start spmenu in insert mode
- spmenu -t,       --top                                     - Position spmenu at the top of the screen
- spmenu -b,       --bottom                                  - Position spmenu at the bottom of the screen
- spmenu -c,       --center                                  - Position spmenu at the center of the screen
- spmenu -hm,      --hide-mode                               - Hide mode indicator
- spmenu -hmc,     --hide-match-count                        - Hide match count
- spmenu -hla,     --hide-left-arrow                         - Hide left arrow
- spmenu -hra,     --hide-right-arrow                        - Hide right arrow
- spmenu -hpr,     --hide-prompt                             - Hide prompt
- spmenu -hc,      --hide-cursor                             - Hide cursor
- spmenu -hhl,     --hide-highlighting                       - Hide highlight
- spmenu -hi,      --hide-image                              - Hide image
- spmenu -sm,      --show-mode                               - Show mode indicator
- spmenu -smc,     --show-match-count                        - Show match count
- spmenu -sla,     --show-left-arrow                         - Show left arrow
- spmenu -sra,     --show-right-arrow                        - Show right arrow
- spmenu -spr,     --show-prompt                             - Show prompt
- spmenu -sc,      --show-cursor                             - Show cursor
- spmenu -shl,     --show-highlighting                       - Show highlight
- spmenu -si,      --show-image                              - Show image
- spmenu -xrdb,    --xrdb                                    - Load .Xresources on runtime
- spmenu -nxrdb,   --no-xrdb                                 - Don't load .Xresources on runtime
- spmenu -m,       --monitor monitor                         - Specify a monitor to run spmenu on
- spmenu -w,       --embed window id                         - Embed spmenu inside window id
- spmenu -H,       --hist-file hist file                     - Specify a path to save the history to
- spmenu -ig,      --image-gaps gaps                         - Set image gaps to gaps
- spmenu -lp,      --vertical-padding padding                - Set the vertical padding
- spmenu -hp,      --horizontal-padding padding              - Set the horizontal padding
- spmenu -la,      --left-arrow-symbol symbol                - Set the left arrow to symbol
- spmenu -ra,      --right-arrow-symbol symbol               - Set the right arrow to symbol
- spmenu -is,      --image-size size                         - Image size
- spmenu -it,      --image-top                               - Position the image at the top
- spmenu -ib,      --image-bottom                            - Position the image at the bottom
- spmenu -ic,      --image-center                            - Position the image in the center
- spmenu -itc,     --image-topcenter                         - Position the image in the top center
- spmenu -wm,      --managed, --x11-client                   - Spawn spmenu as a window manager controlled client/window. Useful for testing
- spmenu -nwm,     --unmanaged                               - Don't spawn spmenu as a window manager controlled client/window. Useful for testing
- spmenu -lcfg,    --load-config                             - Load spmenu configuration (~/.spmenu or ~/.config/spmenu/spmenurc)
- spmenu -ncfg,    --no-load-config                          - Don't load spmenu configuration (~/.spmenu or ~/.config/spmenu/spmenurc)
- spmenu -v,       --version                                 - Print spmenu version to stdout

Appearance:

- spmenu -fn,      --font  font                              - Set the spmenu font to font
- spmenu -nif,     --normal-item-foreground color            - Set the normal item foreground color
- spmenu -nib,     --normal-item-background color            - Set the normal item background color
- spmenu -sif,     --selected-item-foreground color          - Set the selected item foreground color
- spmenu -sib,     --selected-item-background color          - Set the selected item background color
- spmenu -npf,     --normal-item-priority-foreground color   - Set the normal item (high priority) foreground color
- spmenu -npb,     --normal-item-priority-background color   - Set the normal item (high priority) background color
- spmenu -spf,     --selected-item-priority-foreground color - Set the selected item (high priority) foreground color
- spmenu -spb,     --selected-item-priority-background color - Set the selected item (high priority) background color
- spmenu -pfg,     --prompt-foreground color                 - Set the prompt foreground color
- spmenu -pbg,     --prompt-background color                 - Set the prompt background color
- spmenu -ifg,     --input-foreground color                  - Set input foreground color
- spmenu -ibg,     --input-background color                  - Set input background color
- spmenu -mnbg,    --menu-background color                   - Set the menu background color
- spmenu -nhf,     --normal-highlight-foreground color       - Set the normal highlight foreground color
- spmenu -nhb,     --normal-highlight-background color       - Set the normal highlight background color
- spmenu -shf,     --selected-highlight-foreground color     - Set the selected highlight foreground color
- spmenu -shb,     --selected-highlight-background color     - Set the selected highlight background color
- spmenu -nfg,     --number-foreground color                 - Set the foreground color for the match count
- spmenu -nbg,     --number-background color                 - Set the background color for the match count
- spmenu -mfg,     --mode-foreground color                   - Set the foreground color for the mode indicator
- spmenu -mbg,     --mode-background color                   - Set the background color for the mode indicator
- spmenu -laf,     --left-arrow-foreground color             - Set the left arrow foreground color
- spmenu -raf,     --right-arrow-foreground color            - Set the right arrow foreground color
- spmenu -lab,     --left-arrow-background color             - Set the left arrow background color
- spmenu -rab,     --right-arrow-background color            - Set the right arrow background color
- spmenu -cc,      --caret-foreground color                  - Set the caret color
- spmenu -bc,      --border-background color                 - Set the border color
- spmenu -sgr0,    --sgr0 color                              - Set the SGR 0 color
- spmenu -sgr1,    --sgr1 color                              - Set the SGR 1 color
- spmenu -sgr2,    --sgr2 color                              - Set the SGR 2 color
- spmenu -sgr3,    --sgr3 color                              - Set the SGR 3 color
- spmenu -sgr4,    --sgr4 color                              - Set the SGR 4 color
- spmenu -sgr5,    --sgr5 color                              - Set the SGR 5 color
- spmenu -sgr6,    --sgr6 color                              - Set the SGR 6 color
- spmenu -sgr7,    --sgr7 color                              - Set the SGR 7 color
- spmenu -sgr8,    --sgr8 color                              - Set the SGR 8 color
- spmenu -sgr9,    --sgr9 color                              - Set the SGR 9 color
- spmenu -sgr10,   --sgr10 color                             - Set the SGR 10 color
- spmenu -sgr11,   --sgr11 color                             - Set the SGR 11 color
- spmenu -sgr12,   --sgr12 color                             - Set the SGR 12 color
- spmenu -sgr13,   --sgr13 color                             - Set the SGR 13 color
- spmenu -sgr14,   --sgr14 color                             - Set the SGR 14 color
- spmenu -sgr15,   --sgr15 color                             - Set the SGR 15 color

There are also extra arguments recognized for dmenu compatibility. These are:

- spmenu -S                                                  - Don't sort matches
- spmenu -i                                                  - Use case-insensitive matching
- spmenu -nb color                                           - Set the normal background color
- spmenu -nf color                                           - Set the normal foreground color
- spmenu -sb color                                           - Set the selected background color
- spmenu -sf color                                           - Set the selected foreground color

## Keybinds

See `keybinds.h` for a list.

## Modes

One of the features that separate spmenu from dmenu is spmenu's different
modes. As of version 0.2, there are two modes. Normal mode and Insert mode.
These modes are of course similar to Vim.

Normal mode is the mode spmenu starts in unless a mode argument is specified.
In normal mode, all keys perform some action, but you cannot type any actual
text to filter items. This mode is used for navigation, as well
as quickly selecting an item.

Insert mode is entered through (by default) pressing `i` in normal mode. In
this mode, most keybinds do nothing. When you are in insert mode, you
filter items by typing text into the field. Once you're done
with insert mode, you can press Escape to enter normal mode again.

## -p option

spmenu has a -p option, which stands for prompt. It allows you to specify
text to display next to the item list. It is displayed on the left side of the
spmenu window. It should be noted that the prompt is purely visual though.

## Images

spmenu supports drawing images. This image is placed on the left side of
the menu window. To use an image, pipe `IMG:/path/to/image` to spmenu.
If you want you can specify arguments like usual. Note that you should add
a Tab (`\t`) character after the path to the image file. Otherwise the text
after will be interpreted as part of the filename and the image will not be drawn.

Any text after the Tab character will be interpreted as a regular item.
In practice, drawing an image might look like this:

`printf "IMG:/path/to/image\tLook at that image, isn't it awesome?\n" | spmenu`

There are also a few image related arguments, such as:

`-is`, `-ig`, `-it`, `-ib`, `-ic`, `-itc` and `-gc`.

## Colored text

spmenu supports colored text through SGR sequences. This is the same colors
that you might already be using in your shell scripts. This means you can
pipe practically any colored shell script straight into spmenu,
no need to filter the output or anything.

For 256 color support to work, you must add to the array. See `libs/color.h`
if you want this.

See 'SGR sequences' for more information.

## SGR sequences

A basic supported SGR sequence looks like this: `\033[X;YZm`

Here, X specifies if you want normal or bright colors. Y specifies if you
want background or foreground. Z specifies the color number.

Foreground colors: `30` through `37`
Background colors: `40` through `47`
Reset: `0`

NOTE: `;` is a separator, and in this example it separates the
color number and normal/bright. \033 may also be written as `^]` or simply `ESC`.

spmenu supports most color sequences, although not true color by default
(unless -sgr arguments are used).

There are a few arguments, you can override SGR colors on-the-fly
using the `-sgrX` arguments. See 'Arguments' for more information.

## Configuration

spmenu has .Xresources (xrdb) support built in. It reads the xrdb (.Xresources database)
on runtime. You may disable it by passing -nxrdb, or enable it by padding -xrdb.

spmenu loads `~/.config/spmenu/spmenurc` or alternatively if you're
old fashioned, `~/.spmenurc`. This requires that `xrdb` is available on your
operating system.

You can also use wildcards (such as `*`) to achieve a global colorscheme. Programs like
`pywal` do this.
