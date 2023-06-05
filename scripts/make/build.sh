#!/bin/bash
# spmenu build script
# See LICENSE file for copyright and license details.
warn="${warn:-true}"
reconfigure="${reconfigure:-true}"
version="${version:-1.1}"
xresources="${xresources:-true}"
cp_build="${cp_build:-false}"
prefix="${prefix:-/usr}"

check() { [ "$mac" != "false" ] && gen_manual="false" && prefix="/usr/local"; }

loadconf() {
    [ ! -f spmenu.c ] && printf "You're probably in the wrong directory.\n" && exit 1
    [ -x "buildconf" ] && [ ! -x "buildconf_dev" ] && source buildconf
    [ -x "buildconf_dev" ] && source buildconf_dev
}

build() {
    [ ! -f "meson.build" ] && printf "meson.build does not exist.\n" && exit 1
    [ "$gen_manual" != "false" ] && [ -x "$(command -v pandoc)" ] && scripts/make/generate-docs.sh
    [ -z "$wayland" ] && wayland=true
    [ -z "$x11" ] && x11=true

    cp -f meson.build meson.build.orig

    [ "$reconfigure" != "false" ] && rm -rf build/

    mkdir -p build/

    [ "$static" = "true" ] && STATIC="--prefer-static"

    if [ "$reconfigure" = "true" ]; then
        meson setup --reconfigure $STATIC \
            -Dxresources="$xresources" \
            -Dfribidi="$fribidi" \
            -Dxinerama="$xinerama" \
            -Dimlib2="$imlib2" \
            -Dopenssl="$openssl" \
            -Dlibconfig="$libconfig" \
            -Dwayland="$wayland" \
            -Dx11="$x11" \
            --prefix "$prefix" \
            build || exit 1
    else
        meson setup \
            -Dxresources="$xresources" $STATIC \
            -Dfribidi="$fribidi" \
            -Dxinerama="$xinerama" \
            -Dimlib2="$imlib2" \
            -Dopenssl="$openssl" \
            -Dlibconfig="$libconfig" \
            -Dwayland="$wayland" \
            -Dx11="$x11" \
            --prefix "$prefix" \
            build || exit 1
    fi

    ninja -C build

    [ "$cp_build" = "true" ] && cp meson.build meson.build.used
    mv meson.build.orig meson.build
}

install() {
    cd build/ || exit 1
    meson install
    cd .. || exit 1
    [ "$reconfigure" = "true" ] && rm -rf build/
}

[ "$(id -u)" != "0" ] && [ "$install" != "false" ] && printf "Run as root.\n" && exit 1
check
loadconf
build
[ "$install" != "false" ] && install
