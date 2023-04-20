#!/bin/sh
# install.sh
# This script installs all the profiles, allowing spmenu to see and choose them.
CONFDIR="${XDG_CONFIG_HOME:-$HOME/.config}"
mkdir -p "$CONFDIR/spmenu/profiles/" "$CONFDIR/spmenu/configs"
cp ./legacy/* "$CONFDIR/spmenu/profiles/"
cp ./*.theme ./*.conf "$CONFDIR/spmenu/configs"
rm -f "$CONFDIR/spmenu/profiles/install.sh" "$CONFDIR/spmenu/profiles/README.md" "$CONFDIR/spmenu/configs/sample.theme"
