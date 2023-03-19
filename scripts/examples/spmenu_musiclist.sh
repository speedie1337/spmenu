#!/bin/sh
# spmenu_musiclist.sh
#
# This script lists out songs in a path you give to it and also displays the cover art for that song if it can be found.
# It serves as an example of how image drawing can be done with spmenu.
[ -z "$dir" ] && dir="$1"
[ -z "$maxcount" ] && maxcount="$2"
[ -z "$startline" ] && startline="$3"
[ -z "$prefix" ] && prefix="/tmp/.list_songs"

[ -z "$RUNLAUNCHER" ] && RUNLAUNCHER="spmenu"
[ -z "$RUNLAUNCHER_ARGS" ] && RUNLAUNCHER_ARGS="-l 40 -g 1 -is 200 -ngc"

create_list() {
    [ -z "$i" ] && i=1
    [ -z "$c" ] && c="$(find "$dir"/* -type f | wc -l)"

    while true; do
        file="$(find "$dir"/* -type f | sed -n "${i}","${i}"p)" || exit 1
        [ -e "$(dirname "$file")/cover.jpg" ] && \
            cp "$(dirname "$file")"/cover.jpg "$prefix/cover-$i.jpg" || ffmpeg -i "$file" -map 0:1 "$prefix/cover-$i.jpg" -loglevel quiet
        [ -e "$prefix/cover-$i.jpg" ] && imageprefix="IMG:" && image="$prefix/cover-$i.jpg"
        [ "$(basename "$file")" = "cover.jpg" ] && i=$(expr $i + 1) && continue
        printf "%s%s\t%s\n" "$imageprefix" "$image" "$(basename "$file")"

        i=$(expr $i + 1)

        [ "$i" -eq "$c" ] || [ "$i" -gt "$c" ] && break
    done
}

usage() {
    printf "usage: $0 <music directory> <max file count> <start line>\n"
    exit 1
}

play() {
    [ -n "$sel_file" ] && \
        mpv "$(find "$dir"/* -type f | grep "$sel_file")"
}

main() {
    mkdir -p $prefix
    [ -z "$dir" ] && usage
    [ -z "$RUNLAUNCHER" ] && RUNLAUNCHER="spmenu"
    [ -z "$RUNLAUNCHER_ARGS" ] && RUNLAUNCHER_ARGS="-l 40 -g 1 -is 200 -ngc"
    [ -n "$maxcount" ] && c="$maxcount"
    [ -n "$startline" ] && c="$startline"

    sel_file="$(create_list | sed "s/\&/\&amp;/g" | $RUNLAUNCHER $RUNLAUNCHER_ARGS | sed "s/\&amp;/\&/g")"
    play
    rm -f "$prefix"/cover*.jpg # cleanup
}

main "$@"
