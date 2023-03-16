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

- spmenu -l line        - Set line count to stdin
- spmenu -mh height      - Set spmenu line height to height
- spmenu -g grid        - Set the number of grids to grid
- spmenu -gc            - Generate image cache
- spmenu -ngc           - Don't generate image cache
- spmenu -rw            - Enable relative input width
- spmenu -nrw           - Disable relative input width
- spmenu -f             - Grabs keyboard before reading stdin
- spmenu -F             - Enable fuzzy matching
- spmenu -P             - Hide characters
- spmenu -Ps symbol     - Set the password symbol to symbol
- spmenu -p text        - Set spmenu prompt text to text
- spmenu -ip            - Indent items to prompt width
- spmenu -nip           - Don't indent items to prompt width
- spmenu -a             - Enable alpha
- spmenu -na            - Disable alpha
- spmenu -tp            - Allow the user to type
- spmenu -nt            - Don't allow typing, the user must select an option
- spmenu -x x offset    - Offset spmenu x position by x offset
- spmenu -y y offset    - Offset spmenu y position by y offset
- spmenu -n line        - Preselect line in the list of items
- spmenu -z width       - Width of the spmenu window
- spmenu -nmt text      - Set normal mode text to text
- spmenu -imt text      - Set insert mode text to text
- spmenu -bw            - Width of the border. 0 will disable the border
- spmenu -so            - Sort matches
- spmenu -nso           - Don't sort matches
- spmenu -pri priority  - Specify a list of items that take priority
- spmenu -s             - Use case-sensitive matching
- spmenu -i             - Use case-insensitive matching
- spmenu -nm            - Start spmenu in normal mode
- spmenu -im            - Start spmenu in insert mode
- spmenu -t             - Position spmenu at the top of the screen
- spmenu -b             - Position spmenu at the bottom of the screen
- spmenu -c             - Position spmenu at the center of the screen
- spmenu -hm            - Hide mode indicator
- spmenu -hmc           - Hide match count
- spmenu -hla           - Hide left arrow
- spmenu -hra           - Hide right arrow
- spmenu -hpr           - Hide prompt
- spmenu -hc            - Hide cursor
- spmenu -hhl           - Hide highlight
- spmenu -hi            - Hide image
- spmenu -sm            - Show mode indicator
- spmenu -smc           - Show match count
- spmenu -sla           - Show left arrow
- spmenu -sra           - Show right arrow
- spmenu -spr           - Show prompt
- spmenu -sc            - Show cursor
- spmenu -shl           - Show highlight
- spmenu -si            - Show image
- spmenu -xrdb          - Load .Xresources on runtime
- spmenu -nxrdb         - Don't load .Xresources on runtime
- spmenu -m monitor     - Specify a monitor to run spmenu on
- spmenu -w window id   - Embed spmenu inside window id
- spmenu -H hist file   - Specify a path to save the history to
- spmenu -ig gaps       - Set image gaps to gaps
- spmenu -lp padding    - Set the vertical padding
- spmenu -hp padding    - Set the horizontal padding
- spmenu -la symbol     - Set the left arrow to symbol
- spmenu -ra symbol     - Set the right arrow to symbol
- spmenu -is size       - Image size
- spmenu -it            - Position the image at the top
- spmenu -ib            - Position the image at the bottom
- spmenu -ic            - Position the image in the center
- spmenu -itc           - Position the image in the top center
- spmenu -wm            - Spawn spmenu as a window manager controlled client
- spmenu -nwm           - Don't spawn spmenu as a window manager controlled client
- spmenu -lcfg          - Load spmenu configuration (~/.spmenu or ~/.config/spmenu/spmenurc)
- spmenu -ncfg          - Don't load spmenu configuration (~/.spmenu or ~/.config/spmenu/spmenurc)
- spmenu -v             - Print spmenu version to stdout

- spmenu -fn  font      - Set the spmenu font to font
- spmenu -nif color     - Set the normal item foreground color
- spmenu -nib color     - Set the normal item background color
- spmenu -sif color     - Set the selected item foreground color
- spmenu -sib color     - Set the selected item background color
- spmenu -npf color     - Set the normal item (high priority) foreground color
- spmenu -npb color     - Set the normal item (high priority) background color
- spmenu -spf color     - Set the selected item (high priority) foreground color
- spmenu -spb color     - Set the selected item (high priority) background color
- spmenu -pfg color     - Set the prompt foreground color
- spmenu -pbg color     - Set the prompt background color
- spmenu -ifg color     - Set input foreground color
- spmenu -ibg color     - Set input background color
- spmenu -mbg color     - Set the menu background color
- spmenu -nhf color     - Set the normal highlight foreground color
- spmenu -nhb color     - Set the normal highlight background color
- spmenu -shf color     - Set the selected highlight foreground color
- spmenu -shb color     - Set the selected highlight background color
- spmenu -shb color     - Set the selected highlight background color
- spmenu -nfg color     - Set the foreground color for the match count
- spmenu -nbg color     - Set the background color for the match count
- spmenu -mfg color     - Set the foreground color for the mode indicator
- spmenu -mbg color     - Set the background color for the mode indicator
- spmenu -laf color     - Set the left arrow foreground color
- spmenu -raf color     - Set the right arrow foreground color
- spmenu -lab color     - Set the left arrow background color
- spmenu -rab color     - Set the right arrow background color
- spmenu -cc  color     - Set the caret color
- spmenu -bc  color     - Set the border color
- spmenu -sgr0          - Set the SGR 0 color
- spmenu -sgr1          - Set the SGR 1 color
- spmenu -sgr2          - Set the SGR 2 color
- spmenu -sgr3          - Set the SGR 3 color
- spmenu -sgr4          - Set the SGR 4 color
- spmenu -sgr5          - Set the SGR 5 color
- spmenu -sgr6          - Set the SGR 6 color
- spmenu -sgr7          - Set the SGR 7 color
- spmenu -sgr8          - Set the SGR 8 color
- spmenu -sgr9          - Set the SGR 9 color
- spmenu -sgr10         - Set the SGR 10 color
- spmenu -sgr11         - Set the SGR 11 color
- spmenu -sgr12         - Set the SGR 12 color
- spmenu -sgr13         - Set the SGR 13 color
- spmenu -sgr14         - Set the SGR 14 color
- spmenu -sgr15         - Set the SGR 15 color

There are also extra arguments recognized for dmenu compatibility. These are:

- spmenu -S             - Don't sort matches
- spmenu -nb color      - Set the normal background color
- spmenu -nf color      - Set the normal foreground color
- spmenu -sb color      - Set the selected background color
- spmenu -sf color      - Set the selected foreground color

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
