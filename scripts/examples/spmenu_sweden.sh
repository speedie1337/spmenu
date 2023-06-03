#!/bin/sh
# spmenu_sweden.sh
# Just the swedish flag using SGR sequences, serves as a good example on how to do background colors.
#
# See LICENSE file for copyright and license details.

[ -z "$fg_blue" ] && fg_blue='\033[1;34m'
[ -z "$fg_yellow" ] && fg_yellow='\033[1;35m'
[ -z "$bg_blue" ] && bg_blue='\033[1;44m'
[ -z "$bg_yellow" ] && bg_yellow='\033[1;45m'
[ -z "$reset" ] && reset='\033[0m'
[ -z "$color1" ] && color1="#006AA7"
[ -z "$color2" ] && color3="#FECC02"

printf "\n\
${bg_blue}       ${bg_yellow}   ${bg_blue}\n\
${bg_blue}       ${bg_yellow}   ${bg_blue}\n\
${bg_yellow}                             \n\
${bg_blue}       ${bg_yellow}   ${bg_blue}\n\
${bg_blue}       ${bg_yellow}   ${bg_blue}\n\
\n\
${reset}${fg_blue}c${fg_yellow}o${fg_blue}o${fg_yellow}l\n\
" | spmenu --sgr4 "$color1" --sgr5 "$color3" --sgr12 "$color1" --sgr13 "$color3" --no-alpha --lines 40 --columns 7 "$@"
