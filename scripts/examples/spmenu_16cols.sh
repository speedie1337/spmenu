#!/bin/sh
# spmenu_16cols.sh
# Prints 16 foreground and 16 background colors
#
# See LICENSE file for copyright and license details.

printf "\033[0;31mItem 1\n\033[0;32mItem 2\n\033[0;33mItem 3\n\033[0;34mItem 4\n\033[0;35mItem 5\n\033[0;36mItem 6\n\033[0;37mItem 7\n\033[0;38mItem 8\n\033[1;31mItem 9\n\033[1;32mItem 10\n\033[1;33mItem 11\n\033[1;34mItem 12\n\033[1;35mItem 13\n\033[1;36mItem 14\n\033[1;37mItem 15\n\033[1;38mItem 16\n\
\033[0;41mItem 1\n\033[0;42mItem 2\n\033[0;43mItem 3\n\033[0;44mItem 4\n\033[0;45mItem 5\n\033[0;46mItem 6\n\033[0;47mItem 7\n\033[0;48mItem 8\n\033[1;41mItem 9\n\033[1;42mItem 10\n\033[1;43mItem 11\n\033[1;44mItem 12\n\033[1;45mItem 13\n\033[1;46mItem 14\n\033[1;47mItem 15\n\033[1;48mItem 16\n" | spmenu "$@"
