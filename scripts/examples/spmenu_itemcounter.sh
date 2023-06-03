#!/bin/sh
# spmenu_itemcounter.sh
# Example script which adds a new entry to spmenu every second.
#
# See LICENSE file for copyright and license details.
func() {
    rm -f /tmp/fl

    i=0 cnum=0
    while true; do
        printf "%s \033[0;3${cnum}m%d\n" "Item" "$i" >> /tmp/fl
        i=$((i+1))
        cnum=$((cnum+1))

        [ "$cnum" -gt "9" ] && cnum="1"

        sleep 1
    done
}
func | spmenu --list-file /tmp/fl --lines 40 --columns 1 --prompt "Updates automatically!"
