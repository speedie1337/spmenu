#!/bin/sh
# proof of concept script allowing you to view images in spmenu
# usage: spmenu_imageviewer.sh image1 image2 ...
[ ! -f "$1" ] && printf "You must specify an image to view.\n" && exit 1

genlist() {
    argc="$(printf "%s\n" "$@" | wc -l)"

    for i in $(seq "$argc"); do
        arg="$(printf "%s\n" "$@" | sed -n "${i}","${i}"p)"
        [ -z "$arg" ] || [ ! -f "$arg" ] && continue
        printf "img://%s\t%s\n" "$arg" "$arg"
    done
}

main() {
    genlist "$@" | spmenu \
	    --hide-mode \
	    --hide-match-count \
	    --hide-left-arrow \
	    --hide-right-arrow \
	    --hide-prompt \
        --hide-input \
	    --hide-caret \
        --hide-caps \
	    --hide-highlighting \
	    --image-size 200 \
	    --generate-cache \
	    --lines 20 \
	    --columns 1
}

main "$@"
