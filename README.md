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
- Vim-like modes, including indicator
- 256 color support through SGR codes.
- Image support (from [this repository](https://github.com/Cloudef/dmenu-pango-imlib))
- Option to block typing.
- Rewritten arguments, old arguments still work though.
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
- Customizable bar height

### Dependencies

- libX11
- libXrender
- freetype
- imlib2
  - Used for image support, can be disabled if you don't want this by editing `toggle.mk`.
- libXinerama
  - Can be disabled if you don't want/need multi-monitor support, edit `toggle.mk`
- tcc compiler (you can swap it out for GCC by passing CC="gcc" to the `make` command if you want)
- OpenSSL
  - To calculate MD5
- Pango (for drawing fonts)
  - If you do not want to use pango, edit `toggle.mk`

### Installation (most GNU/Linux distributions)

`emerge dev-vcs/git # Install dev-vcs/git using your favorite package manager`

`git clone https://git.speedie.gq/spmenu`

`cd spmenu/`

`make clean install # Run as root.`

### Installation (Gentoo)

If you are on Gentoo GNU/Linux, you can add
[my overlay](https://git.speedie.gq/speedie-overlay) which includes
`x11-misc/spmenu` as well as other useful packages.

### Installation (Arch)

If you are on Arch GNU/Linux, you can add
[my repository](https://git.speedie.gq/speedie-repository) which includes
`spmenu` as well as other useful packages.

There's also a PKGBUILD. To use it, simply `makepkg -Si`.

### Images

To use an image, pipe `IMG:/path/to/image` to spmenu. If you want you can specify arguments.

`printf "IMG:$HOME/Wallpapers/Art/2.png\tThis is a cool image.\n" | spmenu`

### .Xresources values

This build allows you to define .Xresources values to load on startup. See docs/example.Xresources for a list of default values.

### Scripts

This build of spmenu should work with all spmenu/dmenu scripts. [Here](https://codeberg.org/speedie/speedwm-extras) are a few I've written/use.
