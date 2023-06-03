#!/bin/sh
# generate-headers.h
# Generate headers using wayland-scanner
#
# See LICENSE file for copyright and license details.
[ ! -x "$(command -v wayland-scanner)" ] && exit 1

wayland-scanner \
    client-header \
    protocols/wlr-layer-shell-unstable-v1.xml \
    libs/wl/wlr-layer-shell-unstable-v1-client-protocol.h

wayland-scanner \
    client-header \
    protocols/xdg-shell.xml \
    libs/wl/xdg-shell-client-protocol.h

wayland-scanner \
    client-header \
    protocols/xdg-output-unstable-v1.xml \
    libs/wl/xdg-output-unstable-v1-client-protocol.h

wayland-scanner \
    public-code \
    protocols/wlr-layer-shell-unstable-v1.xml \
    libs/wl/wlr-layer-shell-unstable-v1-protocol.c

wayland-scanner \
    public-code \
    protocols/xdg-shell.xml \
    libs/wl/xdg-shell-protocol.c

wayland-scanner \
    public-code \
    protocols/xdg-output-unstable-v1.xml \
    libs/wl/xdg-output-unstable-v1-protocol.c
