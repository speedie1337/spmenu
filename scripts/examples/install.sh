#!/bin/sh
# install.sh
# Install all scripts in the current directory.
#
# See LICENSE file for copyright and license details.
[ "$(id -u)" != "0" ] && printf "Run me as root.\n" && exit 1
cp -f spmenu* /usr/bin
