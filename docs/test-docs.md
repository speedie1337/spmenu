# spmenu_test

spmenu_test is a simple shell script which prints formatted text and colors.
It is mainly designed for spmenu development, but may also be nice for testing
colorschemes and themes.

## Usage

Run `spmenu_test` to run the script. It will spawn an spmenu window with
no transparency where many different colors and types of formatting can
be seen.

spmenu_test will pass all arguments directly to spmenu, it doesn't use any
arguments for itself. See spmenu(1) for a list of arguments you can
pass to spmenu_test. If no arguments are passed, spmenu will be launched
using `--pretext ... --hide-caret --no-allow-typing --top
--lines 9000 --columns 1 --prompt ...`.

## License

spmenu is licensed under the MIT license.

## Reporting issues

Please report issues on the [Git repository](https://git.speedie.site/speedie/spmenu)
or the [GitHub mirror](https://github.com/speediegq/spmenu).

## See also

- spmenu(1)
- spmenu_run(1)
- [spmenu wiki](https://spmenu.speedie.site)
- [spmenu git repository](https://git.speedie.site/speedie/spmenu)
- [spmenu GitHub mirror](https://github.com/speediegq/spmenu)
