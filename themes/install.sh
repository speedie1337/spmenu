#!/bin/sh
mkdir -p "$HOME/.config/spmenu/profiles/"
cp ./* "$HOME/.config/spmenu/profiles/"
rm -f "$HOME/.config/spmenu/profiles/install.sh" "$HOME/.config/spmenu/profiles/README.md"
