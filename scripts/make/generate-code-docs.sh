#!/bin/sh
VERSION="$([ -f "Makefile" ] && grep "VERSION" Makefile | head -n 1 | awk '{ print $3 }' || printf "unknown\n")"
[ ! -f "$1" ] && printf "You must specify an input file.\n" && exit 1
[ -z "$2" ] && printf "You must specify an output file.\n" && exit 1
pandoc -f markdown -t html5 --metadata title="spmenu documentation $VERSION"  -s --toc -o "$2" "$1" || exit 1
exit 0
