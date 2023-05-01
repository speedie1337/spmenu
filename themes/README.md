# spmenu themes

spmenu has a profile feature, which allows you to quickly switch between
configuration files. For this reason, it is possible to quickly switch between colorschemes.

legacy/ contains themes applied through .Xresources, while the current
directory contains themes that are supposed to replace ~/.config/spmenu/theme.conf.

Feel free to contribute your own colorschemes here in either form.

## Usage (.Xresources)

**NOTE: These is nothing wrong with using the .Xresources themes, despite them
having the 'legacy' name. They're being phased out though as of spmenu 1.1
in favor of the config file like themes.**

To switch between profiles in spmenu, you use Ctrl+Shift+p. It will spawn a
list of installed profiles, but also allows you to add/remove profiles.

If you want to install a profile, simply copy it to
`~/.config/spmenu/profiles/`. Pressing the keybind will list it. Simply select
it to start using it!

To switch back remove the profile or switch back to the default profile using 'Default'.

Alternatively, run `install.sh` to install all the themes instantly.

## Usage (spmenurc)

Copy any of the themes to ~/.config/spmenu/theme.conf. Make sure `theme` is enabled in
your config file though.
