spmenu
======

spmenu is an X11 and Wayland menu application which takes standard input, parses
it, lets the user choose an option and sends the selected option to standard output.

In addition to this, it also serves as a run launcher through the included
shell script `spmenu_run`, which handles both $PATH listing, .desktop entries
and file listing. See spmenu_run(1) for more information related to
using spmenu as a run launcher.

While spmenu is based on dmenu, and is also fully compatible with dmenu,
spmenu introduces many new features which can be useful in shell scripting.

It also serves as a dmenu replacement for Wayland users.

## Usage

On runtime, spmenu reads from standard input (stdin). spmenu items are
separated by a newline (`\n`). When (by default) Enter is pressed, the selected
item will be piped to stdout.

This allows things like `printf "Apple\nOrange\nPear\n" | spmenu`. This command
will spawn an spmenu window with three items, 'Apple', 'Orange' and 'Pear'.
This can be used in shell scripts to create interactive menus.

spmenu doesn't strictly read standard input, it can read from files too using
the `-lf` or `--list-file` argument.

On top of this, you can specify arguments to change the behavior of spmenu.
See a list below for a list.

## Arguments

You may use long, descriptive arguments or the shorter arguments.

`-mh, --line-height height`
:   Set spmenu line height to height

`-mw, --min-width width`
:   Set minimum width to width

`-g, --columns grid`
:   Set the number of grids to grid

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

`-P, --password`
:   Hide characters

`-nP, --no-password`
:   Don't hide characters

`-p, --prompt text`
:   Set spmenu prompt text to text

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

`-x, --x-position x offset`
:   Offset spmenu x position by x offset

`-y, --y-position y offset`
:   Offset spmenu y position by y offset

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
:   Set width of the border to width  . 0 will disable the border

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
:   Load .Xresources on runtime

`-nxrdb, --no-xrdb`
:   Don't load .Xresources on runtime

`-gbc, --global-colors`
:   Recognize global colors (such as *.color1) on runtime

`-ngbc, --no-global-colors`
:   Don't recognize global colors (such as *.color1) on runtime

`-m, --monitor monitor`
:   Specify a monitor to run spmenu on

`-w, --embed window id`
:   Embed spmenu inside window id

`-H, --hist-file hist file`
:   Specify a file to save the history to

`-lf, --list-file list file`
:   Specify a file to load entries from

`-ig, --image-gaps gaps`
:   Set image gaps to gaps

`-txp, --text-padding padding`
:   Set text padding to padding

`-vem, --vertical-margin margin`
:   Set the vertical margin

`-hom, --horizontal-margin margin`
:   Set the horizontal margin

`-lp, --vertical-padding padding`
:   Set the vertical padding

`-hp, --horizontal-padding padding`
:   Set the horizontal padding

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

`-wm, --managed, --x11-client`
:   Spawn spmenu as a window manager controlled client/window. Useful for testing

`-nwm, --unmanaged`
:   Don't spawn spmenu as a window manager controlled client/window. Useful for testing

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

## Keybinds

You can set keybinds through the config file. A default config file is available
after installing spmenu. This configuration file has identical keybindings to the
default hardcoded keybinds.

By default, the configuration file will ignore all hardcoded keybindings to
prevent keybind conflicts, but if you do not like this behaviour you can
simply set `ignoreglobalkeys = 1`.

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
the menu window. To use an image, pipe `img:///path/to/image` to spmenu.
If you want you can specify arguments like usual. Note that you should add
a Tab (`\t`) character after the path to the image file. Otherwise the text
after will be interpreted as part of the filename and the image will not be drawn.

Any text after the Tab character will be interpreted as a regular item.
In practice, drawing an image might look like this:

`printf "img:///path/to/image\tLook at that image, isn't it awesome?\n" | spmenu`

There are also a few image related arguments, such as:

`-is`, `-ig`, `-it`, `-ib`, `-ic`, `-itc` and `-gc`.

Vector images (such as .svg) can be displayed too in the same way. This is all
done using `imlib2` so as long as imlib2 support it, it can be used.

NOTE: Also note that older spmenu scripts may use the `IMG:` prefix rather than
the newer `img://` prefix. It is recommended that you use the `img://` prefix,
but `IMG:` may be preferred if you need compatibility with older spmenu versions.

## Colored text

spmenu supports colored text through SGR sequences. This is the same colors
that you might already be using in your shell scripts. This means you can
pipe practically any colored shell script straight into spmenu,
no need to filter the output or anything.

Not only does it support colored text, but it also supports colored backgrounds.
This allows something similar to the emoji highlight patch, except even more useful.
Example: `printf "\033[0;44m😀\033[0m Emoji highlighting\n" | spmenu --columns 1`

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
The separator may be omitted for some sequences, such as `\033[0m` which
resets the colorscheme.

spmenu supports most color sequences, although not true color by default
(unless -sgr arguments are used).

There are a few arguments, you can override SGR colors on-the-fly
using the `-sgrX` arguments. See 'Arguments' for more information.

Just as a tip, you can pipe your colored spmenu output to
`sed -e 's/\x1b\[[0-9;]*m//g'`. This will clear the SGR sequences from
the output. This is useful when you want to check what the output actually is.

256 color sequences are also supported, but due to the complexity involved they
will not be covered in this man page.

## Pango markup

If spmenu was compiled with Pango enabled (default), you should be able to
utilize Pango markup in every part of spmenu. That is, the mode indicator,
items, input, prompt, etc.

Pango markup allows you to style text similar to an HTML document. It also
provides the `<span>` tag, which can be used to do surprisingly complex things.

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
`@include "path/to/config"`.

## Default keybinds

These are the default keybinds. You can generate these yourself from a
`keybinds.h` using `scripts/make/generate-keybind-list.sh`.

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
| 0                  | 0                  | i                  | switchmode         | 0                  |
| 0                  | 0                  | slash              | switchmode         | 0                  |
| 0                  | Ctrl               | equal              | setimgsize         | +1                 |
| 0                  | Ctrl               | minus              | setimgsize         | -1                 |
| 0                  | 0                  | equal              | setimgsize         | +10                |
| 0                  | 0                  | minus              | setimgsize         | -10                |
| 0                  | Shift              | equal              | setimgsize         | +100               |
| 0                  | Shift              | minus              | setimgsize         | -100               |
| 0                  | Shift              | 0                  | defaultimg         | 0                  |
| 0                  | 0                  | r                  | rotateimg          | 0                  |
| 0                  | 0                  | o                  | setimgpos          | +1                 |
| 0                  | Ctrl               | 1                  | setimggaps         | -1                 |
| 0                  | Ctrl               | 2                  | setimggaps         | +1                 |
| 0                  | 0                  | 1                  | setimggaps         | -10                |
| 0                  | 0                  | 2                  | setimggaps         | +10                |
| 0                  | Shift              | 1                  | setimggaps         | -100               |
| 0                  | Shift              | 2                  | setimggaps         | +100               |
| 0                  | 0                  | t                  | toggleimg          | 0                  |
| 0                  | 0                  | f                  | togglefullimg      | 0                  |
| 0                  | 0                  | p                  | paste              | 2                  |
| 0                  | 0                  | h                  | flipimg            | 1                  |
| 0                  | 0                  | v                  | flipimg            | 0                  |
| 0                  | 0                  | k                  | moveup             | 0                  |
| 0                  | 0                  | j                  | movedown           | 0                  |
| 0                  | 0                  | h                  | moveleft           | 0                  |
| 0                  | 0                  | l                  | moveright          | 0                  |
| 0                  | Ctrl               | u                  | moveup             | 5                  |
| 0                  | Ctrl               | d                  | movedown           | 5                  |
| 0                  | Ctrl               | k                  | setlines           | +1                 |
| 0                  | Ctrl               | j                  | setlines           | -1                 |
| 0                  | Ctrl+Alt+Shift     | k                  | setlines           | +5                 |
| 0                  | Ctrl+Alt+Shift     | j                  | setlines           | -5                 |
| 0                  | Ctrl               | h                  | setcolumns         | +1                 |
| 0                  | Ctrl               | l                  | setcolumns         | -1                 |
| 0                  | Ctrl+Alt+Shift     | h                  | setcolumns         | +5                 |
| 0                  | Ctrl+Alt+Shift     | l                  | setcolumns         | -5                 |
| 0                  | 0                  | u                  | togglehighlight    | 0                  |
| 0                  | Ctrl+Shift         | h                  | viewhist           | 0                  |
| 0                  | 0                  | d                  | clear              | 0                  |
| 0                  | Shift              | d                  | clearins           | 0                  |
| 0                  | 0                  | Escape             | quit               | 0                  |
| 0                  | 0                  | Home               | movestart          | 0                  |
| 0                  | 0                  | End                | moveend            | 0                  |
| 0                  | 0                  | g                  | movestart          | 0                  |
| 0                  | Shift              | g                  | moveend            | 0                  |
| 0                  | 0                  | Next               | movenext           | 0                  |
| 0                  | 0                  | Prior              | moveprev           | 0                  |
| 0                  | Alt                | p                  | navhistory         | -1                 |
| 0                  | Alt                | n                  | navhistory         | +1                 |
| 1                  | 0                  | Escape             | switchmode         | 0                  |

## .Xresources

**NOTE: Only applies for X11 users**

spmenu also has .Xresources (xrdb) support built in. It reads the xrdb
(.Xresources database) on runtime. You may disable it by passing -nxrdb,
or enable it by padding -xrdb. You can also set this in the config file.

You can also use wildcards (such as `*`) to achieve a global colorscheme.
Programs like `pywal` do this to apply universal colorschemes.

## Themes

You could just `@include` themes from the aforementioned `spmenu.conf`, but
it's kind of inconvenient. For this reason, spmenu reads `.theme.conf`
and `.config/spmenu/theme.conf` on startup as well. To apply a basic theme,
you simply replace theme.conf with the theme you want to use.

There is a [Git repository](https://git.speedie.site/spmenu-themes)
which contains a bunch of themes written for spmenu,
and you can use them as a template when making your own themes.
You may also contribute to this repository if you have a theme to show.

Do however note that the theme file is **not** the same as the config file.
There are quite a lot of differences, and many options are not available.
This is by design, as these options should be set by the user, not the theme.

This is not very convenient if you have many themes because you constantly
have to replace your theme file, so theme managers exist to make this a
bit easier. [spmenuify](https://git.speedie.site/spmenuify) is the official
theme manager, but you could use another one or write your own.

## Run launcher

spmenu includes a powerful Bash script called spmenu_run. It lists
executable programs in $PATH and displays them to the user in a list.
Not only that but it optionally shows recently run programs first in the list.

The selected option is piped to /bin/sh (by default). Unlike dmenu_run,
spmenu_run has some cool features. For example:

- Prepending `#` will spawn it in a terminal instead of just a shell.
- Prepending `?` will run the command in a function, most of the time used to
display the man page.
- Prepending `magnet` will open a magnet link in $TORRENT
- Prepending `www` will open a page in $BROWSER

Most of the time you don't need to prepend `www` though, for example
typing in `https://gnu.org` will open gnu.org in $BROWSER even
without the prefix. Same goes for magnet links.

You can also configure the run launcher through editing
`~/.config/spmenu/run/config` which is configured in shell syntax.

In addition to the $PATH listing, spmenu_run also allows listing out
and displaying .desktop entries. It does this in style, too by displaying
the program icon.

It can be configured through editing `~/.config/spmenu/run/config`. The
configuration file can also be moved by setting `${XDG_CONFIG_HOME}`.

## spmenu commands

spmenu has a few special commands. These work similar to the images. For example
to list the version, in addition to the `--version` argument you can also simply
run `printf 'spmenu:version' | spmenu`. There are a few of these.

`spmenu:version`
:    Print the spmenu version

`spmenu:license`
:    Print the spmenu license

## Wayland support

Note that Wayland support is still experimental, and some features do not
currently work under Wayland. Some will never work under Wayland due to limitations.
These are:

- Image support
  - Images simply will not be drawn on Wayland.
  - Will eventually be solved by replacing imlib2 with cairo.
- `--x-position` and `--y-position` arguments
  - These arguments do not work under Wayland, because the layer_shell
  protocol doesn't allow clients to be placed on a specific position.
- Embedding `-w` and window manager managed `-wm`
  - These arguments do not make much sense on Wayland, and embedding is not possible
  due to the original implementation using XEmbed. If the embed argument is passed
  it will simply be ignored and the window will be layered as normal.
- `--monitor` argument
- Window borders
- Pasting

## License

spmenu is licensed under the MIT license because that's the original suckless
license. See the included LICENSE file for more information.

## Reporting issues

Please report issues on the
[Codeberg repository](https://codeberg.org/speedie/spmenu) or alternatively
email me.

## See also

- spmenu_run(1)
- [spmenu wiki](https://spmenu.speedie.site)
- [spmenu git repository](https://git.speedie.site/spmenu)
- [spmenu GitHub mirror](https://github.com/speediegq/spmenu)
