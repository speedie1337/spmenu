# spmenu
![image](/docs/preview.png)

### What is spmenu?

spmenu is an X11 menu application based on
[dmenu](https://tools.suckless.org/dmenu) which takes standard input, parses
it, and lets the user choose an option and sends the
selected option to standard output.

It is designed to integrate well with my [dwm](https://dwm.suckless.org) fork, [speedwm](https://codeberg.org/speedie/speedwm).

### Special features

This build of spmenu has some features written for this build.
Of course if you want, this is free software so you can use it in your own build.

- dwm-like keybind array (See keybinds.h)
- 256 color support through SGR codes.
- Option to block typing.
- Rewritten arguments, old arguments still work though.
- Border only when centered option
- Hiding each part of the menu

### Other features

Note: This is an incomplete list, it's just here to give you an idea of what this build has to offer.
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

### Dependencies

- libX11
- libXrender
- freetype
- libXinerama
  - Can be disabled if you don't want/need multi-monitor support.
- tcc compiler (you can swap it out for GCC by passing CC="gcc" to the `make` command if you want)
- Pango (for drawing fonts)
  - If you do not want to use pango, consider my [older spmenu build](https://github.com/speedie-de/dmenu)

### Installation (most GNU/Linux distributions)

`emerge dev-vcs/git # Install dev-vcs/git using your favorite package manager`

`git clone https://codeberg.org/speedie/spmenu`

`cd spmenu/`

`make clean install # Run as root.`

### Installation (Gentoo)

If you are on Gentoo GNU/Linux, you can add
[my overlay](https://codeberg.org/speedie/speedie-overlay) which includes
`x11-misc/spmenu` as well as other useful packages.

### .Xresources values

This build allows you to define .Xresources values to load on startup. See docs/example.Xresources for a list of default values.

### Scripts

This build of spmenu should work with all spmenu scripts. [Here](https://codeberg.org/speedie/speedwm-extras) are a few I've written/use:

### Notes for users of Arch

This fork of spmenu is compiled using tcc for speed however tcc from the Arch repositories seems to be broken. I'm sure there's a better way to fix this but I just fix it by installing [this package](https://aur.archlinux.org/packages/tcc-ziyao) from the AUR.

### Notes for GCC users

If you're compiling with GCC, chances are you're seeing a lot of warnings.
This is because we're compiling with -Ofast. I can't seem to find any issues
with using -Ofast but if it bothers you, you can compile
with -Os or -O2 which don't spit out these warnings.
