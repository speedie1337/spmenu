#!/bin/sh
version="$(grep "version : '" meson.build | awk '{ print $3 }' | sed "s/'\"//g; s/\"',//g")"
[ ! -f "$1" ] && printf "You must specify an input file.\n" && exit 1
[ -z "$2" ] && printf "You must specify an output file.\n" && exit 1
pandoc -f markdown -t html5 --metadata title="spmenu documentation ${version}"  -s --toc -o "$2" "$1" || exit 1
exit 0
