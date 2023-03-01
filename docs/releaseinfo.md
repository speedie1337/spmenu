### **spmenu 0.2.1**

New release of spmenu.

### Changelog

- Improve image support, images can now be much larger without scaling issues.
- Add pango markup toggles
- Change default font
- If an image is bigger than the width/height of the X11 client, the X11 client will be resized on-the-fly.
- Fix image gaps and importantly width being added on when there is no image to display

### Installation

Simply download and unpack the tarball, and `make clean install` as root.

- If you are on Gentoo, you can install `x11-misc/spmenu` by adding my [overlay](https://codeberg.org/speedie/speedie-overlay).
- If you are on Arch, you can install `spmenu` through my [repository](https://git.speedie.gq/speedie-aur).
