# spmenu

![image](docs/preview.png)

## What is spmenu?

spmenu is an X11 menu application based on
[dmenu](https://tools.suckless.org/dmenu) which takes standard input, parses
it, and lets the user choose an option and sends the
selected option to standard output.

In addition to this, it also serves as a run launcher through the included
shell script `spmenu_run`, which handles both $PATH listing, .desktop entries
and file listing.

It is designed to integrate well with my [dwm](https://dwm.suckless.org) fork, [speedwm](https://git.speedie.site/speedwm).

## Improvements over dmenu

- Proper configuration file, including keybinds.
- dwm-like key/mouse bind array (See keybinds.h and mouse.h)
- Vim-like modes, including indicator.
- The ability to move around items with keybinds.
- Customizable/dynamic line/column size.
- Ability to update entries dynamically by reading from file
- IME support
  - Was removed from suckless dmenu years ago due to issues I've resolved
- Powerlines
- Web browser like keybindings.
- 256 color support through SGR codes.
- Image support
  - Somewhat based on [this repository](https://github.com/Cloudef/dmenu-pango-imlib)
- Option to block typing.
- Rewritten arguments, old arguments still work though.
- Hiding each part of the menu.

..and more! Note that spmenu is still **compatible** with dmenu.
Just pass the original arguments.

## Dependencies

- libX11
- libXrender
- freetype
- imlib2
  - Used for image support, can be disabled if you don't want this by:
    - build.sh: Setting `imlib2=false` in `buildconf`.
- libXinerama
  - Can be disabled if you don't want/need multi-monitor support by:
    - build.sh: Setting `xinerama=false` in `buildconf`.
- tcc compiler
  - You can swap it out for GCC or any other C99 compatible compiler by:
    - build.sh: Adding `CC="gcc"` to the `buildconf`.
command if you want)
- OpenSSL
  - Used to calculate MD5 of images if image support is enabled, can be
disabled if you don't want this by:
    - build.sh: Setting `imlib2=false` and `openssl=false` in `buildconf`.
- Pango
  - Can be disabled if you don't want/need Pango markup by:
    - build.sh: Setting `pango=false` and `pangoxft=false` in `buildconf`.
- libconfig
  - Can be disabled if you don't want/need config file support by:
    - build.sh: Setting `libconfig=false` in `buildconf`.
- meson
  - Used to compile spmenu

## Installation (GNU/Linux, \*BSD)

`emerge dev-vcs/git`, `pacman -S git`, `apt-get install git`

`git clone https://git.speedie.site/spmenu`

`cd spmenu/`

`./build.sh # Run as root. Install any missing dependencies.`

Report any issues with the build.sh script.

## Installation (macOS/OS X/Mac OS X)

NOTE: Adds Xquartz as a dependency. I highly recommend you use Homebrew
to install all the dependencies.

Follow 'Installation (GNU/Linux, \*BSD)' for the most part. macOS requires
some minor changes though, such as disabling pango, pangoxft, imlib2, and
openssl. It is not possible to compile spmenu with these as of now, unless
you're a developer.

PREFIX must also be set to /usr/local instead of the default /usr
if you're using macOS Catalina or newer, because Apple made /usr read-only.

See [this wiki article](https://spmenu.speedie.site/index.php/Using+spmenu+on+macOS)
for more information.

## Installation (Gentoo GNU/Linux)

If you are on Gentoo GNU/Linux, you can add
[my overlay](https://git.speedie.site/speedie-overlay) which includes
`x11-misc/spmenu` as well as other useful packages.

## Installation (Arch GNU/Linux)

If you are on Arch GNU/Linux, you can add
[my repository](https://git.speedie.site/speedie-repository) which includes
`spmenu` as well as other useful packages.

There's also a PKGBUILD. To use it, simply run
`scripts/make/generate-pacman-pkg.sh` in the directory the PKGBUILD is
located. You can then `pacman -U` this package, or add it to your repository.

If you wish to package spmenu for your GNU/Linux distribution, feel free to add
it to this list.

## TODO

Pull requests would be greatly appreciated for any of these issues!

### General

- Image support: Stop using OpenSSL for caching images, mostly because MD5()
is deprecated as of OpenSSL 3.0, but this would also make it very easy to
have LibreSSL compatibility.
- Image support: Ability to display icons, similar to rofi
- Text drawing: Use cairo for text drawing over Xft.
- Lines: Rofi-like newlines in the same entry
  - Just need to `XMoveResizeWindow()` as well as `mh += bh` and `y += bh`
  for each added line.
- Matching: Add support for contextual completions similar to xprompt
- Matching: Regex matching
  - Probably use some minimal public domain library for this, I'd
like to avoid adding more external dependencies unless it's a
common dependency most people already have.

### Unlikely, but maybe at some point in the distant future

- X11: Move from Xlib to libXcb
- Wayland: Wayland support, but only if it doesn't require writing any extra
code which as of now seems unlikely, or if someone makes a patch.
  - Before this can even be done, replace Xft with cairo,
  deal with keybinds in some Wayland compatible way, remove .Xresources
  usage and figure out a way to preserve X11 compatibility as I do
  not want to use Wayland as of now.
  - You can just use XWayland anyway if you happen to use Wayland, so it's not
like you will be unable to use spmenu in its current state.

### Bugs

- Text drawing: Pango will sometimes spit out errors for invalid markup.
Silencing this would be a good idea.
- Image support: Images take a long time to load sometimes, particularly when
items are selected using the cursor so what we really need is a way to
skip over images after a set time limit.

## Scripts

There's a page dedicated to user scripts
[over on the wiki](https://spmenu.speedie.site/index.php/User+scripts). Feel
free to contribute and try scripts on there.

## More information

See the included `man` page.

## License

spmenu is licensed under the MIT license. See the included LICENSE file for
more information!

## Wiki

spmenu has [a wiki](https://spmenu.speedie.site) for more extensive
documentation. Contributions to the wiki are appreciated!
