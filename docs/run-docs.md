spmenu_run
==========

spmenu includes a powerful Bash script called spmenu_run. It has several
different components, but the run launcher component lists out
executable programs in $PATH and displays them to the user in a list.
Not only that but it optionally shows recently run programs first in the list,
which is very useful.

By default it also saves history, which can be viewed by calling
`viewhistory` in your spmenu config. By default the keybind
is Ctrl+h in normal mode.

The selected option is then piped to /bin/sh (by default). This component
allows you to select multiple entries by calling `markitem`. By default
the keybind is Ctrl+Enter in any mode. The selected item(s) are then
executed in order one after one. The run launcher also has some
extra awesome features such as:

- Prepending `#` will spawn it in a terminal instead of just a shell.
- Prepending `?` will run the command in a function, most of the time used to
display the man page.
- Prepending `magnet` will open a magnet link in $TORRENT
- Prepending `www` will open a page in $BROWSER

Most of the time you don't need to prepend `www` though, for example
typing in `https://gnu.org` will open gnu.org in $BROWSER even
without the prefix. Same goes for magnet links.

In addition to the $PATH listing, spmenu_run also allows listing out
and displaying .desktop entries through the desktop component.
It does this in style too by default, by displaying the program icon.

Finally, it also comes with a file manager component. Out of the box
this is very basic, but could be expanded to allow for features
proper file managers usually have. It is intended to be used in
your shell scripts with the `-o` flag to select a file. You can
see this in action in the spmenuify theme manager.

It can be configured through editing `~/.config/spmenu/run/config`. The
configuration file can also be moved by setting `${XDG_CONFIG_HOME}`.
For more information regarding configuration, see 'Configuration'.

## Usage

Running `spmenu_run` in a shell will execute spmenu_run using the
default component (defined in the configuration file). You can
override this using arguments. Note that you can always override
this using arguments, see Arguments for more information.

## Arguments

You may use long, descriptive arguments or the shorter arguments.

`-x, --run`
:   List entries in $PATH

`-f, --fm dir`
:   List files and directories in dir

`-d, --desktop`
:   List .desktop entries

`-p, --full-path`
:   Print the full path to the file selected (-fm)

`-np, --no-full-path`
:   Don't print the full path to the file selected (-fm)

`-cc, --clear-cache`
:   Clear cache, useful if you just installed/uninstalled a program. (-d)

`-dm, --dmenu`
: Run spmenu_run using dmenu instead of spmenu

`-ndm, --no-dmenu`
:   Run spmenu_run using spmenu instead of dmenu

`-h, --help`
:   Print help

`-o, --stdout`
:   Print to standard input and do not execute the selected item

`-no, --no-stdout`
:   Don't print to standard input, execute the selected item

`-a, --args args`
:   Pass args to spmenu

## Configuration

When spmenu_run is started for the first time, a default configuration file
is created at `~/.config/spmenu/run/config`. This configuration file unlike
the spmenu configuration file is configured in Bash, allowing you to use
POSIX compliant features as well as Bash features.

Most of the variables are already defined, and there are several functions
you can define yourself which spmenu_run will execute on a certain condition.
This allows for a very customizable run launcher, because it means almost anything
Bash can do can be done with spmenu_run.

spmenu_run will execute the following functions
(from your `~/.config/spmenu/run/config` file) if they are found:

- `run_pre_func` before spawning spmenu.
- `run_post_func` after spawning spmenu, selected item passed as an argument.
- `desktop_pre_func` before spawning spmenu.
- `desktop_post_func` after spawning spmenu, selected item passed as an argument.
- `fm_pre_func` before spawning spmenu.
- `fm_post_func` after spawning spmenu, selected item passed as an argument.
- `fm_pre_list_func` right before listing out files.
- `fm_post_list_func` right after listing out files.
- `fm_dir_func` before changing directory to the selected directory.
- `fm_line_func` for each line in ls output, the line is passed as an argument,
including SGR colors.
- `read_man` when reading a man page, selected item passed as an argument.

More detailed documentation regarding this is available in the spmenu wiki including
example functions that can be copied into your configuration.

## License

spmenu_run is licensed under the MIT license just like spmenu itself because
that's the original suckless license. See the included LICENSE file for more information.

## Reporting issues

Please report issues on the
[Codeberg repository](https://codeberg.org/speedie/spmenu) or alternatively
email me.
