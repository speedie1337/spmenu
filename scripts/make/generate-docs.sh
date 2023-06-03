#!/bin/sh
# generate-docs.sh
# Generate documentation from docs.md
#
# See LICENSE file for copyright and license details.
[ ! -f spmenu.c ] && printf "You're probably in the wrong directory.\n" && exit 1
version="$(grep "version : '" meson.build | awk '{ print $3 }' | sed "s/'\"//g; s/\"',//g")"
printf "%% spmenu(1) ${version} | fancy dynamic menu\n" > .man.md
grep -v docs/preview.png docs/docs.md >> .man.md
pandoc --standalone --to man .man.md -o spmenu.1
pandoc --standalone .man.md -o spmenu.html --metadata title="spmenu man page"
rm -f .man.md

printf "%% spmenu_run(1) ${version} | \$PATH/.desktop launcher and file manager\n" > .man.md
grep -v docs/preview.png docs/run-docs.md >> .man.md
pandoc --standalone --to man .man.md -o spmenu_run.1
rm -f .man.md

pandoc --standalone README.md -o README.html --metadata title="spmenu README"
