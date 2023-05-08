#!/bin/sh
version="$(grep "version : '" meson.build | awk '{ print $3 }' | sed "s/'\"//g; s/\"',//g")"
printf "%% spmenu(1) ${version} | fancy dynamic menu\n" > .man.md
grep -v docs/preview.png docs/docs.md >> .man.md
pandoc --standalone --to man .man.md -o spmenu.1
pandoc --standalone .man.md -o spmenu.html
rm -f .man.md

scripts/make/generate-code-docs.sh docs/code-docs.md code.html || return
pandoc --standalone README.md -o README.html
