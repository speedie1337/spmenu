#!/bin/sh
[ ! -f "$1" ] && printf "You must specify an image to view.\n" && exit 1

genlist() {
    argc="$(printf "%s\n" "$@" | wc -l)"

    for i in $(seq "$argc"); do
        arg="$(printf "%s\n" "$@" | sed -n "${i}","${i}"p)"
        [ -z "$arg" ] || [ ! -f "$arg" ] && continue
        printf "IMG:%s\t%s\n" "$arg" "$arg"
    done
}

main() {
    genlist "$@" | spmenu --hide-mode --hide-match-count --hide-left-arrow --hide-right-arrow --hide-prompt --hide-cursor --hide-highlighting --image-size 200 --generate-cache
}

main "$@"
