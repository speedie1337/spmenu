# spmenu themes

spmenu has a profile feature, which allows you to quickly switch between
configuration files. For this reason, it is possible to quickly switch between colorschemes.

legacy/ contains themes applied through .Xresources, while the current directory contains
themes included from spmenu.conf.

Feel free to contribute your own colorschemes here in either form.

## Usage (.Xresources)

**NOTE: These is nothing wrong with using the .Xresources themes, despite them
having the 'legacy' name.**

To switch between profiles in spmenu, you use Ctrl+Shift+p. It will spawn a
list of installed profiles, but also allows you to add/remove profiles.

If you want to install a profile, simply copy it to
`~/.config/spmenu/profiles/`. Pressing the keybind will list it. Simply select
it to start using it!

To switch back remove the profile or switch back to the default profile using 'Default'.

Alternatively, run `install.sh` to install all the themes instantly.

## Usage (spmenurc)

Install it, and `@include` it in your spmenu.conf. Or if you prefer, replace
your spmenu.conf with it.
