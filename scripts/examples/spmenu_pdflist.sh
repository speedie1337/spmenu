#!/bin/sh
c="$(find *.pdf -type f | wc -l)"
i=1
[ -z "$prefix" ] && prefix="/tmp/.list_pdfs"; mkdir -p ${prefix}
[ -z "$RUNLAUNCHER" ] && RUNLAUNCHER="spmenu"
[ -z "$RUNLAUNCHER_ARGS" ] && RUNLAUNCHER_ARGS="-l 40 -g 1 -is 400 -ngc"

print_list() {
    while true; do
        file="$(find *.pdf -type f | sed -n "$i","$i"p | grep pdf)"
        [ ! -f "$file" ] && continue

        pdftoppm -jpeg -f 1 -singlefile "$file" > "${prefix}/file-$i.jpg"

        [ -f "$prefix/file-$i.jpg" ] && imageprefix="IMG:" && image="$prefix/file-$i.jpg"

        printf "%s%s\t%s\n" "$imageprefix" "$image" "$file"

        # ++
        [ "$i" = "$c" ] || [ "$i" -gt "$c" ] && break;
        i="$(expr $i + 1)"
    done
}

sel_file="$(print_list | sed "s/\&/\&amp;/g" | $RUNLAUNCHER $RUNLAUNCHER_ARGS | sed "s/\&amp;/\&/g")"

[ -n "$sel_file" ] && zathura "$sel_file"
