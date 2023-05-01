#!/bin/sh
# install.sh
# This script installs all the profiles
CONFDIR="${XDG_CONFIG_HOME:-$HOME/.config}"
mkdir -p "$CONFDIR/spmenu/themes"
cp ./*.conf "$CONFDIR/spmenu/themes"
