![spmenu logo](docs/spmenu.svg "spmenu")

spmenu
======

## What is spmenu?

spmenu is an X11 menu application which takes standard input, parses
it, and lets the user choose an option and sends the
selected option to standard output.

In addition to this, it also serves as a run launcher through the included
shell script `spmenu_run`, which handles both $PATH listing, .desktop entries
and file listing.

While spmenu is based on dmenu, and is also fully compatible with dmenu,
spmenu introduces many new features which can be useful in shell scripting.
There are way too many to list, but spmenu has a
[wiki](https://spmenu.speedie.site) which goes through features in more detail.

## Dependencies

- libX11
  - If you're using macOS, XQuartz is a dependency instead.
  - If you're using Wayland, `xorg-xwayland` is a dependency.
- libXrender
- freetype
- imlib2
  - Used for image support, can be disabled during compile time.
- libXinerama
  - Used for multi-monitor support, can be disabled during compile time.
- OpenSSL
  - Used to calculate MD5 of images if image support is enabled, can be
disabled during compile time.
- Pango
- Cairo
- libconfig
  - Can be disabled if you don't want/need config file support during compile time.
- meson
  - Used to compile spmenu, not optional unless you're experienced with build systems.

## Installation

- If you're using macOS you may want to consider looking at [this wiki artic
le](https://spmenu.speedie.site/index.php/Using+spmenu+on+macOS) for more information.

- If you are on Arch GNU/Linux, you can add
[my repository](https://git.speedie.site/speedie-repository) which includes
`spmenu` as well as other useful packages. Then simply `pacman -S spmenu`.

- Or if you are on Gentoo GNU/Linux, you can add
[my overlay](https://git.speedie.site/speedie-overlay) which includes
`x11-misc/spmenu` as well as other useful packages. Then simply `emerge spmenu`.

If you still need/want to manually compile, follow along with manual compilation.

Here we're manually compiling spmenu. This is likely what you'll want to do
if you're using any distribution but Arch or Gentoo.

Git is required to clone the repository, but you can also use
[releases](https://ls.speedie.site). Those can be unpacked
using `tar -xpvf /path/to/spmenu-version.tar.gz`.

To install Git:

- Gentoo: `emerge dev-vcs/git`

- Arch: `pacman -S git`

- Debian: `apt-get install git`

You will also need the dependencies for spmenu. You'll
have to find those packages in your distribution repositories.

To clone the repository using Git:

`git clone https://git.speedie.site/spmenu`

`cd spmenu/`

Configure the build by running these commands:

`mkdir -p build/ # Create a build/ directory, Meson will use this as the working
directory`

`meson setup build # This will check to make sure all dependencies are found.
If you're recompiling you may want to pass --reconfigure as an argument`

This is where you can enable/disable certain features, as well as set
compiler options.

Now, to build it run `ninja -C build`. If all went well you should have a
binary in the `build/` directory.

Finally, to install it all, run:

`meson install -C build --prefix /usr # /usr may be overriden to /usr/local
or anything else if you use macOS or simply want another destination directory`

To generate documentation, which may be necessary if you're pushing new changes
to your Git repository, run `scripts/make/generate-docs.sh` **in the current
directory**.

To generate a tarball, run `scripts/make/generate-pkg.sh` **in the current
directory**. If you want to generate a Pacman package, run
`scripts/make/generate-pacman-pkg.sh` instead.

See [this wiki article](https://spmenu.speedie.site/index.php/Using+spmenu+on+macOS)
for more information.

## TODO

Pull requests would be greatly appreciated for any of these issues!

### General

- Image support: Stop using OpenSSL for caching images, mostly because MD5()
is deprecated as of OpenSSL 3.0, but this would also make it very easy to
have LibreSSL compatibility.
- Image support: Ability to display icons, similar to rofi
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
  - Before this can even be done, deal with keybinds in some Wayland compatible
  way, remove .Xresources usage and figure out a way to preserve X11
  compatibility as I do not want to use Wayland as of now.
  - You can just use XWayland anyway if you happen to use Wayland, so it's not
like you will be unable to use spmenu in its current state.

### Bugs

None discovered yet! :)

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
