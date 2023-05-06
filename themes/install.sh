#!/bin/sh
# install.sh
# This script installs all the profiles
CONFDIR="${XDG_CONFIG_HOME:-$HOME/.config}"
mkdir -p "$CONFDIR/spmenu/themes"
cp ./*.conf "$CONFDIR/spmenu/themes"
echo "Installed."
echo "Use spmenuify (or another theme manager) to enable the themes."
echo "Alternatively copy whichever theme you want to use to ~/.config/spmenu/theme.conf"
