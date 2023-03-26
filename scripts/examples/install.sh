#!/bin/sh
[ "$(id -u)" != "0" ] && printf "Run me as root.\n" && exit 1
cp -f spmenu* /usr/bin
