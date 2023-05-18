#!/bin/bash
# generate-theme.sh
# Generate a theme from a list of colors

declare -a color

help() {
cat << EOF
usage: $0 [-c] color-list
EOF
exit 1
}

read_colors() {
    i=1
    color=()

    while true; do
        color[i]="$(sed -n ${i},${i}p $1)"
        ccol="${color[i]}"
        [ "${#ccol}" != "7" ] || [ "${ccol:0:1}" != "#" ] && color[i]="#000000"
        [ "$i" = "18" ] && break
        i=$((i+1))
    done
}

write_array() {
cat << EOF
/* spmenu colorscheme generated from ${1:-None} */
theme = {
    color = ( {
            itemnormfg = "${color[17]}";
            itemnormbg = "${color[18]}";
            itemnormfg2 = "${color[17]}";
            itemnormbg2 = "${color[18]}";
            itemselfg = "${color[8]}";
            itemselbg = "${color[2]}";
            itemmarkedfg = "${color[1]}";
            itemmarkedbg = "${color[7]}";
            itemnormprifg = "${color[17]}";
            itemnormpribg = "${color[18]}";
            itemselprifg = "${color[17]}";
            itemselpribg = "${color[13]}";
            menu = "${color[18]}";
            promptfg = "${color[18]}";
            promptbg = "${color[4]}";
            larrowfg = "${color[13]}";
            rarrowfg = "${color[13]}";
            larrowbg = "${color[18]}";
            rarrowbg = "${color[18]}";
            inputfg = "${color[17]}";
            inputbg = "${color[18]}";
            numfg = "${color[1]}";
            numbg = "${color[10]}";
            normhlfg = "${color[7]}";
            normhlbg = "${color[18]}";
            selhlfg = "${color[16]}";
            selhlbg = "${color[1]}";
            caretfg = "${color[17]}";
            border = "${color[10]}";
            modefg = "${color[1]}";
            modebg = "${color[3]}";
            capsfg = "${color[1]}";
            capsbg = "${color[5]}";
            sgr0 = "${color[1]}";
            sgr1 = "${color[2]}";
            sgr2 = "${color[3]}";
            sgr3 = "${color[4]}";
            sgr4 = "${color[5]}";
            sgr5 = "${color[6]}";
            sgr6 = "${color[7]}";
            sgr7 = "${color[8]}";
            sgr8 = "${color[9]}";
            sgr9 = "${color[10]}";
            sgr10 = "${color[11]}";
            sgr11 = "${color[12]}";
            sgr12 = "${color[13]}";
            sgr13 = "${color[14]}";
            sgr14 = "${color[15]}";
            sgr15 = "${color[16]}";
            } );

    file = ( { global = 0; } );
};
EOF
}

main() {
    [ ! -f "$2" ] && help
    [ "$(wc -l < "$2")" != "18" ] && printf "Color list is not 18 lines long.\n" && exit 1

    read_colors "$2"
    write_array "$2"
}

main "$@"
