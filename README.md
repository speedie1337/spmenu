# spmenu

![image](/docs/preview.png)

## What is spmenu?

spmenu is an X11 menu application based on
[dmenu](https://tools.suckless.org/dmenu) which takes standard input, parses
it, and lets the user choose an option and sends the
selected option to standard output.

In addition to this, it also serves as a run launcher through the included
shell script `spmenu_run`. Alternatively you may use `spmenu_desktop` which
lists .desktop entries instead of $PATH.

It is designed to integrate well with my [dwm](https://dwm.suckless.org) fork, [speedwm](https://git.speedie.gq/speedwm).

## Special features

This build of spmenu has some features written for this build.
Of course if you want, this is free software so you can use it in your own build.

- dwm-like keybind array (See keybinds.h)
- Vim-like modes, including indicator.
- The ability to move around items with keybinds.
- Customizable/dynamic line/column size.
- Powerlines
- Web browser like keybindings.
- 256 color support through SGR codes.
- Image support (somewhat based on [this repository](https://github.com/Cloudef/dmenu-pango-imlib))
- Option to block typing.
- Rewritten arguments, old arguments still work though.
- Hiding each part of the menu.

..and more!

## Other features

Note: This is an incomplete list, it's just here to give you an idea of what
this build has to offer.

- Pango markup support
- Alpha transparency
- Pywal/.Xresources support
- Grid
- Colored Emoji/Font support
- Highlighting
- Right-to-left language support
- Case-insensitive by default
- Padding; useful with patched dwm with barpadding or speedwm.
- Fuzzy-finding
- Preselect support
- Line-height
- History support
- Customizable bar height

## Dependencies

- libX11
- libXrender
- freetype
- imlib2
  - Used for image support, can be disabled if you don't want this by:
    - build.sh: Setting `imlib2=false` in `buildconf`.
    - Makefile: Editing `toggle.mk` and commenting out a few lines.
- libXinerama
  - Can be disabled if you don't want/need multi-monitor support by:
    - build.sh: Setting `xinerama=false` in `buildconf`.
    - Makefile: Editing `toggle.mk` and commenting out a few lines.
- tcc compiler
  - You can swap it out for GCC or any other C99 compatible compiler by:
    - Makefile: Passing `CC="gcc"` to the `make` command.
    - build.sh: Adding `CC="gcc"` to the `buildconf`.
command if you want)
- OpenSSL
  - Used to calculate MD5 of images if image support is enabled, can be
disabled if you don't want this by:
    - build.sh: Setting `imlib2=false` and `openssl=false` in `buildconf`.
    - Makefile: Editing `toggle.mk` and commenting out a few lines.
- Pango
  - Can be disabled if you don't want/need Pango markup by:
    - build.sh: Setting `pango=false` and `pangoxft=false` in `buildconf`.
    - Makefile: Editing `toggle.mk` and commenting out a few lines.

## Installation (GNU/Linux, \*BSD, macOS/OS X/Mac OS X)

`emerge dev-vcs/git # Install dev-vcs/git using your favorite package manager`

`git clone https://git.speedie.gq/spmenu`

`cd spmenu/`

`./build.sh # Run as root.`

Report any issues with the build.sh script. There's also a Makefile which
you can use but it may require editing host.mk and toggle.mk manually.

## Installation (Gentoo GNU/Linux)

If you are on Gentoo GNU/Linux, you can add
[my overlay](https://git.speedie.gq/speedie-overlay) which includes
`x11-misc/spmenu` as well as other useful packages.

## Installation (Arch GNU/Linux)

If you are on Arch GNU/Linux, you can add
[my repository](https://git.speedie.gq/speedie-repository) which includes
`spmenu` as well as other useful packages.

There's also a PKGBUILD. To use it, simply `make install_arch` in the directory
the PKGBUILD is located.

If you wish to package spmenu for your GNU/Linux distribution, feel free to add
it to this list.

## TODO

Pull requests would be greatly appreciated for any of these issues!

### General

- Key: Mouse bind array (Likely mouse.h).
- Config file: Add configuration file using (probably) libconfig, allowing
keybinds to be configured without recompiling spmenu.
- Image support: Stop using OpenSSL for caching images, mostly because MD5()
is deprecated as of OpenSSL 3.0, but this would also make it very easy to
have LibreSSL compatibility.
- Matching: Add support for contextual completions similar to xprompt
- Text drawing: Use cairo for text drawing over Xft.
- Matching: FIFO, used to dynamically refresh entries.

### Unlikely, but maybe at some point in the distant future

- Wayland: Wayland support, but only if it doesn't require writing any extra
code which as of now seems unlikely.
  - Before this can even be done, we need to add a configuration file using
anything but .Xresources, replace Xft with cairo, deal with keybinds in some
Wayland compatible way, and figure out a way to preserve X11 compatibility
as I do not want to use Wayland anyway.
  - You can just use XWayland anyway if you happen to use Wayland, so it's not
like you will be unable to use spmenu in its current state.

### Bugs

- Text drawing: Pango will sometimes spit out errors for invalid markup.
Silencing this would be a good idea.
- Items: If we have zero lines, the width of items that contain SGR sequences
will be the max size of one item. This is fine, and there is not much I can do
about it without significant rewrites I don't think are worth doing.
- Image support: Images take a long time to load sometimes, particularly when
items are selected using the cursor so what we really need is a way to
skip over images after a set time limit

## Scripts

This build of spmenu should work with all spmenu/dmenu scripts.
[Here](https://git.speedie.gq/speedwm-extras) are a few I've written/use.
If your scripts are useful enough, and written with spmenu in mind, feel free
to pull request and put your scripts here!

## More information

See the included `man` page.

## License

spmenu is licensed under the MIT license. See the included LICENSE file for
more information!
