# spmenu

spmenu is an X11 and Wayland menu application which takes standard input, parses
it, lets the user choose an option and sends the selected option to standard output.

Its core functionality is built around taking and parsing input and returning
output, making it a customizable and versatile utility for use with
shell or Python scripts.

In addition to this, it also serves as a run launcher/dmenu_run replacement
through the included shell script `spmenu_run`, which handles both
$PATH listing, .desktop entries and file listing. See spmenu_run(1)
for more information regarding using spmenu as a run launcher.

## Usage

On runtime, spmenu reads from standard input (stdin). spmenu items are
separated by a newline (`\n`). When (by default) Enter is pressed, the selected
item will be piped to stdout.

This allows things like `printf "Apple\nOrange\nPear\n" | spmenu`. This command
will spawn an spmenu window with three items, 'Apple', 'Orange' and 'Pear'. The
user can select an item using the keyboard or mouse and the output can then be
parsed. This can be used in shell scripts to create interactive menus.

Note that spmenu doesn't strictly read standard input, it can read from files
too using the `-lf` or `--list-file` argument.

On top of this, you can specify arguments to change the behavior of spmenu.
See a list below for a list.

## Arguments

You may use long, descriptive arguments or the shorter arguments. Note that none
need to be specified for a basic spmenu prompt. The most commonly used arguments
are `-l` or `--lines` and `-g` or `--columns`. All of the arguments can be defined
permanently in the configuration file. Arguments take priority over settings defined
in the config files.

`-mh, --line-height height`
:   Set spmenu line height to height

`-cw, --center-width width`
:   Set width to width when centered

`-l, --lines lines`
:   Set the number of lines to lines

`-g, --columns grid`
:   Set the number of grids to grid

`-ml, --min-lines`
:   Set minimum number of lines allowed to lines

`-gc, --generate-cache`
:   Generate image cache

`-ngc, --no-generate-cache`
:   Don't generate image cache

`-mc, --max-cache size`
:   Set max image cache size to size

`-cd, --cache-dir dir`
: Set cache directory to dir

`-ix, --print-index`
:   Print index instead of actual text

`-nix, --no-print-index`
:   Don't print index instead of actual text

`-f, --fast`
:   Grabs keyboard before reading stdin

`-r, --incremental`
:   Print text every time a key is pressed

`-nr, --no-incremental`
:   Don't print text every time a key is pressed

`-rm, --require-match`
:   Require that input text matches an item

`-nrm, --no-require-match`
:   Don't require that input text matches an item

`-ma, --mark-items`
:   Allow marking/selecting multiple items

`-nma, --no-mark-items`
:   Don't allow marking/selecting multiple items

`-F, --fuzzy`
:   Enable fuzzy matching

`-NF, --no-fuzzy`
:   Disable fuzzy matching

`-R, --regex`
:   Enable regex matching

`-NR, --no-regex`
:   Disable regex matching

`-P, --password`
:   Hide characters

`-nP, --no-password`
:   Don't hide characters

`-p, --prompt text`
:   Set spmenu prompt text to text

`-pt, --pretext text`
:   Set spmenu pretext to text

`-It, --input text`
:   Set initial input text to text

`-ip, --indent`
:   Indent items to prompt width

`-nip, --no-indent`
:   Don't indent items to prompt width

`-ci, --color-items`
:   Color items

`-nci, --no-color-items`
:   Don't color items

`-sgr, --sgr`
:   Interpret SGR sequences

`-nsgr, --no-sgr`
:   Display SGR sequences as text

`-a, --alpha`
:   Enable alpha

`-na, --no-alpha`
:   Disable alpha

`-tp, --allow-typing`
:   Allow the user to type

`-nt, --no-allow-typing`
:   Don't allow typing, the user must select an option

`-ol, --override-lines`
:   Allow keybinds to override lines

`-oc, --override-columns`
:   Allow keybinds to override columns

`-nol, --no-override-lines`
:   Don't allow keybinds to override lines

`-noc, --no-override-columns`
:   Don't allow keybinds to override columns

`-x, --x-position x offset`
:   Offset spmenu x position by x offset (X11 only)

`-y, --y-position y offset`
:   Offset spmenu y position by y offset (X11 only)

`-n, --preselect line`
:   Preselect line line in the list of items

`-z, --width width`
:   Width of the spmenu window

`-nmt, --normal-mode-text text`
:   Set normal mode text to text

`-imt, --insert-mode-text text`
:   Set insert mode text to text

`-clon, --caps-lock-on-text text`
: Set caps lock on text to text

`-clof, --caps-lock-off-text text`
: Set caps lock off text to text

`-bw, --border-width width`
:   Set width of the border to width. 0 will disable the border (X11 only)

`-so, --sort`
:   Sort matches

`-nso, --no-sort`
:   Don't sort matches

`-pri, --priority pri1,pri2,pri3`
:   Specify a list of items that take priority

`-s, --case-sensitive`
:   Use case-sensitive matching

`-ns, --case-insensitive`
:   Use case-insensitive matching

`-nm, --normal`
:   Start spmenu in normal mode

`-im, --insert`
:   Start spmenu in insert mode

`-t, --top`
:   Position spmenu at the top of the screen

`-b, --bottom`
:   Position spmenu at the bottom of the screen

`-c, --center`
:   Position spmenu at the center of the screen

`-itt, --item-top`
:   Position items above all other elements

`-itb, --item-bottom`
:   Position items below all other elements

`-hm, --hide-mode`
:   Hide mode indicator

`-hit, --hide-item`
:   Hide items

`-hmc, --hide-match-count`
:   Hide match count

`-hla, --hide-left-arrow`
:   Hide left arrow

`-hra, --hide-right-arrow`
:   Hide right arrow

`-hpr, --hide-prompt`
:   Hide prompt

`-hpt, --hide-pretext`
:   Hide pretext

`-hip, --hide-input`
:   Hide input

`-hpl, --hide-powerline`
:   Hide powerline

`-hc, --hide-caret, --hide-cursor`
:   Hide caret

`-hhl, --hide-highlighting`
:   Hide highlight

`-hi, --hide-image`
:   Hide image

`-hcl, --hide-caps`
:   Hide caps lock indicator

`-sm, --show-mode`
:   Show mode indicator

`-sit, --show-item`
:   Show items

`-smc, --show-match-count`
:   Show match count

`-sla, --show-left-arrow`
:   Show left arrow

`-sra, --show-right-arrow`
:   Show right arrow

`-spr, --show-prompt`
:   Show prompt

`-sin, --show-input`
:   Show input

`-spl, --show-powerline`
:   Show powerline

`-sc, --show-caret, --show-cursor`
:   Show caret

`-shl, --show-highlighting`
:   Show highlight

`-si, --show-image`
:   Show image

`-scl, --show-caps`
:   Show caps lock indicator

`-xrdb, --xrdb`
:   Load .Xresources on runtime (X11 only)

`-nxrdb, --no-xrdb`
:   Don't load .Xresources on runtime (X11 only)

`-m, --monitor monitor`
:   Specify a monitor to run spmenu on (X11 only)

`-w, --embed window id`
:   Embed spmenu inside window id (X11 only)

`-H, --hist-file hist file`
:   Specify a file to save the history to

`-lf, --list-file list file`
:   Specify a file to load entries from

`-ig, --image-gaps gaps`
:   Set image gaps to gaps

`-txp, --text-padding padding`
:   Set text padding to padding

`-vem, --vertical-margin margin`
:   Set the vertical margin to margin

`-hom, --horizontal-margin margin`
:   Set the horizontal margin to margin

`-lp, --vertical-padding padding`
:   Set the vertical padding to padding

`-hp, --horizontal-padding padding`
:   Set the horizontal padding to padding

`-la, --left-arrow-symbol symbol`
:   Set the left arrow to symbol

`-ra, --right-arrow-symbol symbol`
:   Set the right arrow to symbol

`-is, --image-size size`
:   Image size

`-it, --image-top`
:   Position the image at the top

`-ib, --image-bottom`
:   Position the image at the bottom

`-ic, --image-center`
:   Position the image in the center

`-itc, --image-topcenter`
:   Position the image in the top center

`-ir, --image-resize`
:   Allow spmenu to resize itself to fit the image

`-nir, --no-image-resize`
:   Don't allow spmenu to resize itself to fit the image

`-di, --display-icons`
:   Display the images as icons

`-df, --display-image`
:   Display the images as images in the image pane

`-wm, --managed, --x11-client`
:   Spawn spmenu as a window manager controlled client/window (X11 only)

`-nwm, --unmanaged`
:   Don't spawn spmenu as a window manager controlled client/window (X11 only)

`-cf, --config-file file`
:   Set config file to load to file

`-lcfg, --load-config`
:   Load spmenu configuration (~/.config/spmenu/spmenu.conf) on runtime

`-ncfg, --no-load-config`
:   Don't load spmenu configuration (~/.config/spmenu/spmenu.conf) on runtime

`-bf, --bind-file file`
:   Set bind file to load to file

`-lbi, --load-binds`
:   Exclusively load binds from file (~/.config/spmenu/binds.conf) on runtime

`-nlbi, --no-load-binds`
:   Don't exclusively load binds from file (~/.config/spmenu/binds.conf) on runtime

`-tm, --theme theme`
:   Load theme 'theme' on runtime

`-ltm, --load-theme`
:   Load theme (~/.config/spmenu/theme.conf) on runtime

`-nltm, --no-load-theme`
:   Don't load theme (~/.config/spmenu/theme.conf) on runtime

`-x11, --x11`
:   Run spmenu in X11 mode

`-wl, --wayland`
:   Run spmenu in Wayland mode

`-v, --version`
:   Print spmenu version to stdout

`-rv, --raw-version`
:   Print raw spmenu version number to stdout

`-fl, --feature-list`
:   List the state of all features that can be toggled

`-fn, --font font`
:   Set the spmenu font to font

`-nif, --normal-item-foreground color`
:   Set the normal item foreground color

`-nib, --normal-item-background color`
:   Set the normal item background color

`-nnif, --normal-next-item-foreground color`
:   Set the normal next item foreground color

`-nnib, --normal-next-item-background color`
:   Set the normal next item background color

`-sif, --selected-item-foreground color`
:   Set the selected item foreground color

`-sib, --selected-item-background color`
:   Set the selected item background color

`-npf, --normal-item-priority-foreground color`
:   Set the normal item (high priority) foreground color

`-npb, --normal-item-priority-background color`
:   Set the normal item (high priority) background color

`-spf, --selected-item-priority-foreground color`
:   Set the selected item (high priority) foreground color

`-spb, --selected-item-priority-background color`
:   Set the selected item (high priority) background color

`-pfg, --prompt-foreground color`
:   Set the prompt foreground color

`-pbg, --prompt-background color`
:   Set the prompt background color

`-ifg, --input-foreground color`
:   Set input foreground color

`-ibg, --input-background color`
:   Set input background color

`-ptfg, --pretext-foreground color`
:   Set pretext foreground color

`-ptbg, --pretext-background color`
:   Set pretext background color

`-mnbg, --menu-background color`
:   Set the menu background color

`-nhf, --normal-highlight-foreground color`
:   Set the normal highlight foreground color

`-nhb, --normal-highlight-background color`
:   Set the normal highlight background color

`-shf, --selected-highlight-foreground color`
:   Set the selected highlight foreground color

`-shb, --selected-highlight-background color`
:   Set the selected highlight background color

`-nfg, --number-foreground color`
:   Set the foreground color for the match count

`-nbg, --number-background color`
:   Set the background color for the match count

`-mfg, --mode-foreground color`
:   Set the foreground color for the mode indicator

`-mbg, --mode-background color`
:   Set the background color for the mode indicator

`-laf, --left-arrow-foreground color`
:   Set the left arrow foreground color

`-raf, --right-arrow-foreground color`
:   Set the right arrow foreground color

`-lab, --left-arrow-background color`
:   Set the left arrow background color

`-rab, --right-arrow-background color`
:   Set the right arrow background color

`-cfc, --caret-foreground color`
:   Set the caret foreground color

`-cbc, --caret-background color`
:   Set the caret background color

`-bc, --border-background color`
:   Set the border color

`-sgr0, --sgr0 color`
:   Set the SGR 0 color

`-sgr1, --sgr1 color`
:   Set the SGR 1 color

`-sgr2, --sgr2 color`
:   Set the SGR 2 color

`-sgr3, --sgr3 color`
:   Set the SGR 3 color

`-sgr4, --sgr4 color`
:   Set the SGR 4 color

`-sgr5, --sgr5 color`
:   Set the SGR 5 color

`-sgr6, --sgr6 color`
:   Set the SGR 6 color

`-sgr7, --sgr7 color`
:   Set the SGR 7 color

`-sgr8, --sgr8 color`
:   Set the SGR 8 color

`-sgr9, --sgr9 color`
:   Set the SGR 9 color

`-sgr10, --sgr10 color`
:   Set the SGR 10 color

`-sgr11, --sgr11 color`
:   Set the SGR 11 color

`-sgr12, --sgr12 color`
:   Set the SGR 12 color

`-sgr13, --sgr13 color`
:   Set the SGR 13 color

`-sgr14, --sgr14 color`
:   Set the SGR 14 color

`-sgr15, --sgr15 color`
:   Set the SGR 15 color

dmenu compatibility can be achieved using these arguments:

`-S`
:    Don't sort matches

`-i`
:    Use case-insensitive matching

`-nb color`
:    Set the normal background color

`-nf color`
:    Set the normal foreground color

`-sb color`
:    Set the selected background color

`-sf color`
:    Set the selected foreground color

There are more options, that can be set in the configuration file but not using
arguments passed to spmenu.

## Matching

`printf "Apple\nPear\nBanana\n" | spmenu`

With the default configuration, typing in `Apple`, `apple`, `aPpLe` and `pple`
will match `Apple` in this example. Matching is case insensitive, and fuzzy
matching is enabled by default. You can disable fuzzy matching and enable
case sensitivity using arguments, or by enabling it in the configuration.

`printf "1 Apple\nOne Apple\n" | spmenu`

spmenu also supports regex matching, but it is not enabled by default. Therefore,
typing in `[0-9]` will return no matches. In the default configuration, you can
press Ctrl+r to enable regex matching. Now typing in `[0-9]` will return the
`1 Apple` entry, but not the `One Apple` entry. Of course, more advanced
regex can be used as well.

## Modes

There are two modes. Normal mode and Insert mode. These modes are of
course similar to Vim. While modes are used by default, it is possible
to move all keybinds to Insert mode, restoring the original dmenu
behavior.

Normal mode is the mode spmenu starts in unless a mode argument is specified
or another mode is set in the configuration file. Note that if `forceinsertmode`
is enabled, Normal mode cannot be used and spmenu will start in Insert mode
instead.

In normal mode, all keys perform some action, but you cannot type any actual
text to filter items. This mode is commonly used for navigation, general
keybinds, as well as quickly selecting an item. By default though,
this mode is not used.

Insert mode is entered through (by default) pressing `i` in normal mode.
In this mode, most keybinds do nothing. When you are in insert mode,
you filter items by typing text into the field.

Once you're done with insert mode and normal mode is enabled, you can press
(by default) Escape to enter normal mode again.

All of these keybinds can be overriden in the configuration file. Should you
unbind your switchmode key, you can always press `Ctrl+Alt+Delete` to
exit spmenu, allowing you to fix your spmenu configuration.

## History buffer

spmenu allows you to specify a history file using the `-H` argument. When this
argument is specified, the selected item(s) will be appended to the file.
In spmenu.conf, you can specify a max number of entries, and whether you
want duplicate entries or not.

To access the history buffer, call `viewhist`. By default, the keybind for
that is Shift+h in normal mode. You can also access it by clicking the
match indicator. To hide the history buffer again, call `viewhist`.

If `-H` is not specified, the history buffer will not be available, and
calling `viewhist` will do nothing.

## -p option

spmenu has a `-p` or `--prompt` option. It allows you to specify
text to display next to the item list. It is displayed on the left side of the
spmenu window. It should be noted that the prompt is purely visual though.

It may be useful when you want to display information, such as the current
directory or what the items actually do. This is a field that can be overriden
with almost any text.

## Displaying images

spmenu supports displaying images. This image is placed on the left side of
the menu window, as long as spmenu isn't a single line.

To use an image, pipe `img:///path/to/image` to spmenu. If you want you can
specify arguments like usual. Note that you should add a Tab (`\t`) character
after the path to the image file. Otherwise the text after will be interpreted
as part of the filename and the image will not be drawn.

Any text after the Tab character will be interpreted as a regular item.
In practice, drawing an image might look like this:

`printf "img:///path/to/image\tLook at that image, isn't it awesome?\n" | spmenu`

There are also a few image related arguments, such as:

`-is`, `-ig`, `-it`, `-ib`, `-ic`, `-itc` and `-gc`.

Vector images (such as .svg) can be displayed too in the same way. This is all
done using `imlib2` and `cairo` so as long as imlib2 support it, it can be used.

If the image cannot be located, isn't a valid format or cannot be displayed
for some reason, the space where the image would be displayed is blank.

## Colored text

spmenu supports colored text through SGR sequences. This is the same colors
that you might already be using in your shell scripts. This means you can
pipe practically any colored shell script straight into spmenu,
no need to filter the output or anything.

Not only does it support colored text, but it also supports colored backgrounds.
This allows something similar to the emoji highlight patch on the suckless website,
except even more useful.

Example: `printf "\033[0;44m😀\033[0m Emoji highlighting\n" | spmenu --columns 1`

It should be noted that font sequences are not yet supported. See 'SGR sequences'
for more information.

## SGR sequences

SGR sequences (ANSI escape codes) can be used to set the color of spmenu items. Here's
a simple table of good SGR sequences. Note that sequences can also be combined,
and that this isn't the only way to format them.

| Sequence           | Description                                                                       |
| :----------------- | :-------------------------------------------------------------------------------- |
| \033[0m            | Reset foreground and background color and alpha                                   |
| \033[0;3nm         | Set foreground color to normal color index 'n' (0-7)                              |
| \033[1;3nm         | Set foreground color to bright color index 'n' (0-7)                              |
| \033[0;4nm         | Set background color to normal color index 'n' (0-7)                              |
| \033[1;4nm         | Set background color to bright color index 'n' (0-7)                              |
| \033[38;2;r;g;bm   | Set foreground color to a specified RGB color, r is red, g is green, b is blue    |
| \033[48;2;r;g;bm   | Set background color to a specified RGB color, r is red, g is green, b is blue    |
| \033[38;5;nm       | Set foreground color to color index 'n' (0-256)                                   |
| \033[48;5;nm       | Set background color to color index 'n' (0-256)                                   |
| \033[39m           | Reset foreground color and alpha                                                  |
| \033[49m           | Reset background color and alpha                                                  |
| ;                  | Semicolon is used as a separator                                                  |
| m                  | Ends the sequence                                                                 |

Other sequences *may* be supported but are not listed here. In any case, this allows
for all RGB colors to be theoretically used all at the same time.

For example, to set the foreground color to red and print 'Hello world',
one could do the following: `printf '\033[0;31mHello world\n" | spmenu`
This will set the foreground color to `sgr1` in the config/theme file.
You can do this for `sgr0` through `sgr7`. To access `sgr8` through `sgr15`
you use `\033[1` rather than `\033[0`, specifying that you want bright
colors to be used.

As for 256 colors, you simply specify a value between 0 and 256. These colors
are built into spmenu and cannot be overridden. They are only really implemented
into spmenu for compatibility, in practice you should use true color sequences
instead, as they are much more flexible.

True color is slightly more complicated. For example, to print black text on a
white background, one could do something like this:
`printf "\033[48;2;255;255;255;38;2;0;0;0mTest\033[0m\n" | spmenu`

This might look confusing if you aren't familiar with these sequences, but it's fairly
simple. First we set the background color and specify that this is a true color
sequence. (`48;2`) Then we set the red, green and blue channel to fully
opaque (`255;255;255` for red;green;blue), resulting in white. Then we repeat this
for a foreground color (`38;2`) but replace `255` with `0`, which results in
black. Do however note that you don't need to specify *both* a foreground and
background color.

**Note: Background colors will used until a reset sequence is found.**

## FIFO

spmenu has support for FIFO. This means you can tell spmenu to perform certain
actions while it is running. spmenu checks the FIFO every 0.1 seconds. To use it,
simply append a valid name to the FIFO file, which by default is /tmp/spmenu.fifo.

You must append **without** a newline. Otherwise it will be considered invalid.
It is recommended that you sleep for 0.1 seconds after appending to the file
for performance reasons.

| Name               | Description                                                                       |
| :----------------- | :-------------------------------------------------------------------------------- |
| drawmenu           | Draw the menu                                                                     |
| match              | Match entries again, useful if you're loading items from file                     |
| update             | Match and then draw the menu. Both drawmenu and match one after another           |
| output             | Output selected item text                                                         |
| output_index       | Output selected item index                                                        |
| loadconfig         | Reload config                                                                     |
| test               | Print out 'Test print' to standard output                                         |
| die                | Print out 'FIFO told me to die.' using the die() function                         |
| toggleinput        | Toggle input                                                                      |
| togglepretext      | Toggle pretext                                                                    |
| togglelarrow       | Toggle left arrow                                                                 |
| togglerarrow       | Toggle right arrow                                                                |
| toggleitem         | Toggle item                                                                       |
| toggleprompt       | Toggle prompt                                                                     |
| togglecaps         | Toggle caps lock indicator                                                        |
| togglepowerline    | Toggle powerline                                                                  |
| togglecaret        | Toggle caret                                                                      |
| togglehighlight    | Toggle highlighting                                                               |
| togglematchcount   | Toggle match count                                                                |
| togglemode         | Toggle mode indicator                                                             |
| toggleregex        | Toggle regex                                                                      |
| togglefuzzy        | Toggle fuzzy                                                                      |
| toggleimg          | Toggle images                                                                     |
| toggleimgtype      | Toggle image type                                                                 |
| screenshot         | Screenshot spmenu                                                                 |
| setprofile         | Open profile menu                                                                 |
| setlines+          | Increase lines by 1                                                               |
| setlines-          | Decrease lines by 1                                                               |
| setcolumns+        | Increase columns by 1                                                             |
| setcolumns-        | Decrease columns by 1                                                             |
| setx+              | Increase X position by 1                                                          |
| setx-              | Decrease X position by 1                                                          |
| sety+              | Increase Y position by 1                                                          |
| sety-              | Decrease Y position by 1                                                          |
| setw+              | Increase width by 1                                                               |
| setw-              | Decrease width by 1                                                               |
| moveup             | Move up one item                                                                  |
| movedown           | Move down one item                                                                |
| moveleft           | Move left one item                                                                |
| moveright          | Move right one item                                                               |
| movestart          | Move to the start                                                                 |
| moveend            | Move to the end                                                                   |
| movenext           | Move to the next page                                                             |
| moveprev           | Move to the previous page                                                         |
| moveword+          | Move caret to the next word                                                       |
| moveword-          | Move caret to the previous word                                                   |
| movecaret+         | Move caret to the next character                                                  |
| movecaret-         | Move caret to the previous character                                              |
| clear              | Clear input                                                                       |
| viewhist           | View history buffer                                                               |
| backspace          | Backspace                                                                         |
| deleteword         | Remove one full word forward                                                      |
| exit_0             | Exit with exit code 0                                                             |
| exit_1             | Exit with exit code 1                                                             |

## Pango markup and text formatting

Because spmenu uses Pango and Cairo for text drawing, Pango markup can be used
to format text and other elements.

Pango markup allows you to style text similar to an HTML document. It also
provides the `<span>` tag, which can be used to do surprisingly complex things,
such as drawing shapes.

There are many convenient tags as well which can be used to
avoid using a `<span>` tag, such as:

`<b>`
:    **Bold** text (`<b>Bold</b>`)

`<i>`
:    *Italic* text (`<i>Italic</i>`)

`<s>`
:    Strikethrough text (`<s>Strikethrough</s>`)

`<u>`
:    Underline text (`<u>Underline</u>`)

`<sub>`
:    Subscript (`<sub>Subscript</sub>`)

`<sup>`
:    Superscript (`<sup>Supscript</sup>`)

`<tt>`
:    Monospace font (`<tt>Monospaced font is used here</tt>`)

`<small>`
:    Small text (`<small>text is so small here</small>`)

`<big>`
:    Big text (`<big>text is so big here</big>`)

You can also color foreground or background text using the `<span>` tag.
For example `<span foreground="#FFFF00" background="#00FFFF">text</span>`
will color the foreground and background. Of course, this is a bad way
to color text in spmenu because SGR sequences can be used instead.

Note that Pango markup is NOT escaped, and is piped to stdout. Therefore you
need to parse it manually. Doing so with `sed` is very easy. For example:
`... | spmenu ... | sed 's/<big>//g; s/</big>//g'`

See [this page](https://docs.gtk.org/Pango/pango_markup.html) for more information.

## Configuration

Unlike dmenu, spmenu has a configuration file which can be edited by hand. It
is located in ~/.config/spmenu/spmenu.conf, but you can override this by
exporting `$XDG_CONFIG_HOME`.

When spmenu is installed, it copies a sample configuration
to /usr/share/spmenu/spmenu.conf. You can copy this to your `.config/spmenu` directory.
This configuration file is loaded on startup.

You can also include other configuration files in the configuration file using
`@include "path/to/config"`, useful if you wish to split your config file up
into multiple segments.

Note that spmenu also has a `binds.conf` configuration file, which isn't used
very much. This file is documented well [here](https://spmenu.speedie.site/binds.conf+documentation).

## Keybinds

You can set keybinds through the config file. A default config file is available
after installing spmenu. This configuration file has identical keybinds to the
default hardcoded keybinds.

By default, the configuration file will ignore all hardcoded keybinds to
prevent keybind conflicts, but if you do not like this behavior you can
simply set `ignoreglobalkeys = 0`.

Note that if you do unbind your exit or switchmode key by accident and find that
you cannot exit spmenu, press `Ctrl+Alt+Delete`. This is a hardcoded keybind
which cannot be disabled from the configuration file.

These are the default keybinds.

| Mode               | Modifier           | Key                | Function           | Argument           |
| :----------------- | :----------------- | :----------------- | :----------------- | :----------------- |
| -1                 | 0                  | Return             | selectitem         | +1                 |
| -1                 | Shift              | Return             | selectitem         | 0                  |
| -1                 | Ctrl               | Return             | markitem           | 0                  |
| -1                 | 0                  | Tab                | complete           | 0                  |
| -1                 | Ctrl               | v                  | paste              | 2                  |
| -1                 | Ctrl+Shift         | v                  | paste              | 1                  |
| -1                 | 0                  | BackSpace          | backspace          | 0                  |
| -1                 | Ctrl               | BackSpace          | deleteword         | 0                  |
| -1                 | Ctrl               | Left               | moveword           | -1                 |
| -1                 | Ctrl               | Right              | moveword           | +1                 |
| -1                 | 0                  | Left               | movecursor         | -1                 |
| -1                 | 0                  | Right              | movecursor         | +1                 |
| -1                 | Ctrl+Shift         | p                  | setprofile         | 0                  |
| -1                 | Ctrl               | k                  | setlines           | +1                 |
| -1                 | Ctrl               | j                  | setlines           | -1                 |
| -1                 | Ctrl               | h                  | setcolumns         | +1                 |
| -1                 | Ctrl               | l                  | setcolumns         | -1                 |
| -1                 | 0                  | Print              | screenshot         | 0                  |
| 0                  | 0                  | i                  | switchmode         | 0                  |
| 0                  | Ctrl               | equal              | setimgsize         | +10                |
| 0                  | Ctrl               | minus              | setimgsize         | -10                |
| 0                  | Shift              | 0                  | defaultimg         | 0                  |
| 0                  | 0                  | o                  | setimgpos          | +1                 |
| 0                  | Ctrl               | 1                  | setimggaps         | -10                |
| 0                  | Ctrl               | 2                  | setimggaps         | +10                |
| 0                  | 0                  | t                  | toggleimg          | 0                  |
| 0                  | 0                  | p                  | paste              | 2                  |
| 0                  | 0                  | q                  | flipimg            | 1                  |
| 0                  | 0                  | w                  | flipimg            | 0                  |
| 0                  | 0                  | k                  | moveup             | 0                  |
| 0                  | 0                  | j                  | movedown           | 0                  |
| 0                  | 0                  | h                  | moveleft           | 0                  |
| 0                  | 0                  | l                  | moveright          | 0                  |
| 0                  | Ctrl               | u                  | moveup             | 5                  |
| 0                  | Ctrl               | d                  | movedown           | 5                  |
| 0                  | 0                  | u                  | togglehighlight    | 0                  |
| 0                  | Shift              | h                  | viewhist           | 0                  |
| 0                  | 0                  | d                  | clear              | 0                  |
| 0                  | Shift              | d                  | clearins           | 0                  |
| 0                  | 0                  | Escape             | quit               | 0                  |
| 0                  | 0                  | g                  | movestart          | 0                  |
| 0                  | Shift              | g                  | moveend            | 0                  |
| 0                  | Ctrl               | p                  | navhistory         | -1                 |
| 0                  | Ctrl               | n                  | navhistory         | +1                 |
| 1                  | 0                  | Escape             | switchmode         | 0                  |
| 1                  | Ctrl               | r                  | toggleregex        | 0                  |

## Mouse

spmenu supports mouse clicks. The following parts can be clicked on:

- `ClickWindow`
  - Clicking on the spmenu window
- `ClickPrompt`
  - Clicking on the prompt
- `ClickInput`
  - Clicking on the input
- `ClickLArrow`
  - Clicking on the left arrow
- `ClickRArrow`
  - Clicking on the right arrow
- `ClickItem`
  - Clicking on an item
- `ClickImage`
  - Clicking on an image
- `ClickNumber`
  - Clicking on the match count indicator
- `ClickCaps`
  - Clicking on the caps lock indicator
- `ClickMode`
  - Clicking on the mode indicator
- `None`
  - Any click

For information on the different functions you can pass, see the bottom of this
man page, 'List of all configurable functions'.

You can set mouse binds through the config file. A default config file is available
after installing spmenu. This configuration file has identical keybinds to the
default hardcoded keybinds.

By default, the configuration file will ignore all hardcoded mouse binds to
prevent conflicts, but if you do not like this behavior you can
simply set `ignoreglobalmouse = 0`.


These are the default mouse binds. Mode does not apply for mouse binds.

| Click              | Button             | Function           | Argument           |
| :----------------- | :----------------- | :----------------- | :----------------- |
| ClickInput         | Left Click         | clear              | 0                  |
| ClickPrompt        | Left Click         | clear              | 0                  |
| ClickMode          | Left Click         | switchmode         | 0                  |
| ClickNumber        | Left Click         | viewhist           | 0                  |
| ClickItem          | Left Click         | selecthover        | 0                  |
| ClickItem          | Right Click        | markhover          | 0                  |
| None               | Scroll Up          | moveprev           | 0                  |
| None               | Scroll Down        | movenext           | 0                  |

## .Xresources

**NOTE: Only applies for X11 users**

spmenu also has .Xresources (xrdb) support built in. It reads the xrdb
(.Xresources database) on runtime. You may disable it by passing -nxrdb,
or enable it by passing -xrdb. You can also set this in the config file.

You can also use wildcards (such as `*`) to achieve a global colorscheme.
Programs like `pywal` do this to apply universal colorschemes.

`spmenu.color0` through `spmenu.color15` and `spmenu.font` preferences
are read by spmenu, these alter the font and colorscheme spmenu uses.

| Xresources name    | spmenu name   |
| :----------------- | :-------------|
| font               | font          |
| color10            | caretfg       |
| color0             | caretbg       |
| color4             | larrowfg      |
| color4             | rarrowfg      |
| color10            | itemnormfg    |
| color10            | itemnormfg2   |
| color10            | itemnormprifg |
| color10            | inputfg       |
| color10            | pretextfg     |
| color0             | itemnormbg    |
| color0             | itemnormbg2   |
| color0             | itemnormpribg |
| color0             | menu          |
| color0             | larrowbg      |
| color0             | rarrowbg      |
| color0             | itemselfg     |
| color0             | itemmarkedfg  |
| color0             | itemselprifg  |
| color0             | inputbg       |
| color0             | pretextbg     |
| color12            | promptbg      |
| color0             | promptfg      |
| color7             | capsbg        |
| color0             | capsfg        |
| color6             | itemselbg     |
| color6             | itemmarkedbg  |
| color6             | itemselpribg  |
| color6             | border        |
| color0             | numfg         |
| color5             | numbg         |
| color0             | modefg        |
| color11            | modebg        |
| color2             | hlnormbg      |
| color3             | hlselbg       |
| color0             | hlnormfg      |
| color0             | hlselfg       |
| color0             | sgr0          |
| color1             | sgr1          |
| color2             | sgr2          |
| color3             | sgr3          |
| color4             | sgr4          |
| color5             | sgr5          |
| color6             | sgr6          |
| color7             | sgr7          |
| color8             | sgr8          |
| color9             | sgr9          |
| color10            | sgr10         |
| color11            | sgr11         |
| color12            | sgr12         |
| color13            | sgr13         |
| color14            | sgr14         |
| color15            | sgr15         |

## Themes

You could just `@include` themes from the aforementioned `spmenu.conf`, but
it's kind of inconvenient. For this reason, spmenu reads `.config/spmenu/theme.conf`
on startup as well. To apply a basic theme,
you simply replace theme.conf with the theme you want to use.

There is a [Git repository](https://git.speedie.site/speedie/spmenu-themes)
and [wiki article](https://spmenu.speedie.site/User+themes) which contains
a bunch of themes written for spmenu, and you can use them as a template
when making your own themes. You may also contribute to this repository
if you have a theme to show.

Do however note that the theme file is **not** the same as the config file.
There are quite a lot of differences, and many options are not available.
This is by design, as these options should be set by the user, not the theme.

Having a single theme file is not very convenient if you have many themes
because you constantly have to replace your theme file, so theme managers
exist to make this a bit easier. [spmenuify](https://git.speedie.site/speedie/spmenuify)
is the official theme manager, but you could use another one or write your own.

For more information on the theme.conf configuration file,
see [this page](https://spmenu.speedie.site/theme.conf+documentation).

## List of all configurable functions

This is a list of all functions that can be called using a key or mouse
bind:

- `moveup`
  - Move up by MAX(passed argument, 1)
- `movedown`
  - Move down by MAX(passed argument, 1)
- `moveleft`
  - Move left by MAX(passed argument, 1)
- `moveright`
  - Move right by MAX(passed argument, 1)
- `movestart`
  - Move to the first item
- `moveend`
  - Move to the last item
- `movenext`
  - Move to the next page
- `moveprev`
  - Move to the previous page
- `moveitem`
  - Move to item passed argument
- `paste`
  - Paste from clipboard, 0 means clipboard selection, 1 means primary selection
- `restoresel`
  - Move back to the first item
- `clear`
  - Clear input
- `clearins`
  - Clear input text and switch to insert mode
- `viewhist`
  - Toggle history and item buffer
- `moveword`
  - Navigate through input text by 1 word, -1 means start, +1 means end
- `deleteword`
  - Delete input text by 1 word
- `movecursor`
  - Move cursor (caret) by MAX(passed argument, 1)
- `navhistory`
  - Navigate through history buffer, -1 means forward, +1 means backwards
- `backspace`
  - Backspace 1 character from input text
- `selectitem`
  - Select the current selected item
- `quit`
  - Quit spmenu
- `complete`
  - Tab complete the selected item
- `setimgsize`
  - Set image size to passed argument
- `defaultimg`
  - Reset image size to the default
- `toggleinput`
  - Toggle input
- `togglepretext`
  - Toggle pretext
- `togglelarrow`
  - Toggle left arrow
- `togglerarrow`
  - Toggle right arrow
- `toggleitem`
  - Toggle items
- `toggleprompt`
  - Toggle prompt
- `togglecaps`
  - Toggle caps lock indicator
- `togglepowerline`
  - Toggle powerlines
- `togglecaret`
  - Toggle caret (cursor)
- `togglehighlight`
  - Toggle highlighting
- `togglematchcount`
  - Toggle match count
- `togglemode`
  - Toggle mode
- `toggleregex`
  - Toggle regex mode
- `togglefuzzy`
  - Toggle fuzzy matching
- `toggleimg`
  - Toggle images
- `toggleimgtype`
  - Toggle image type
- `flipimg`
  - Flip image, 0 means vertically, 1 means horizontally
- `setimgpos`
  - Move to the next image position
- `setimggaps`
  - Set image gaps to passed argument
- `setlines`
  - Set lines to passed argument
- `setcolumns`
  - Set columns to passed argument
- `setx`
  - Set X position to passed argument
- `sety`
  - Set Y position to passed argument
- `setlineheight`
  - Set line height to passed argument
- `setprofile`
  - Open profile menu if available
- `switchmode`
  - Switch between Normal mode and Insert mode
- `selecthover`
  - Output the item clicked on
- `markhover`
  - Mark the item clicked on
- `screenshot`
  - Take a screenshot of spmenu

## License

spmenu is licensed under the MIT license. The documentation is licensed
under the CC-BY-SA 4.0 license.

## Reporting issues

Please report issues on the [Git repository](https://git.speedie.site/speedie/spmenu)
or the [GitHub mirror](https://github.com/speediegq/spmenu).

## See also

- spmenu_run(1)
- spmenu_test(1)
- [spmenu wiki](https://spmenu.speedie.site)
- [spmenu git repository](https://git.speedie.site/speedie/spmenu)
- [spmenu GitHub mirror](https://github.com/speediegq/spmenu)
- [spmenuify](https://git.speedie.site/speedie/spmenuify)
