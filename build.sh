#!/bin/bash
# spmenu build script
opt="${opt:-${OPT:--O2}}"
warn="${warn:-true}"
reconfigure="${reconfigure:-true}"
version="${version:-1.1}"
xresources="${xresources:-true}"
cp_build="${cp_build:-false}"

check() { [ "$mac" != "false" ] && gen_manual="false"; }

loadconf() {
    [ ! -f spmenu.c ] && printf "You're probably in the wrong directory.\n" && exit 1
    [ -x "buildconf" ] && [ ! -x "buildconf_dev" ] && source buildconf
    [ -x "buildconf_dev" ] && source buildconf_dev

    # mandatory deps
    [ "$freetype" = "false" ] && printf "Freetype not found. Install it.\n" && exit 1
    [ "$openssl" = "false" ] && [ "$imlib2" = "true" ] && printf "OpenSSL not found. Install it.\n" && exit 1
    [ "$xrender" = "false" ] && printf "libXrender not found. Install it.\n" && exit 1
    [ "$x11" = "false" ] && printf "libX11 not found. Install it.\n" && exit 1
    [ "$xft" = "false" ] && printf "libXft not found. Install it.\n" && exit 1
}

build() {
    [ ! -f "meson.build" ] && printf "meson.build does not exist.\n" && exit 1
    [ "$gen_manual" != "false" ] && [ -x "$(command -v pandoc)" ] && scripts/make/generate-docs.sh

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
            -Dpango="$pango" \
            -Dpangoxft="$pangoxft" \
            -Dlibconfig="$libconfig" \
            -Dopt="$opt" \
            -Dutf8="$utf8" \
            build
    else
        meson setup \
            -Dxresources="$xresources" $STATIC \
            -Dfribidi="$fribidi" \
            -Dxinerama="$xinerama" \
            -Dimlib2="$imlib2" \
            -Dopenssl="$openssl" \
            -Dpango="$pango" \
            -Dpangoxft="$pangoxft" \
            -Dlibconfig="$libconfig" \
            -Dopt="$opt" \
            -Dutf8="$utf8" \
            build
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

check
loadconf
build
[ "$install" != "false" ] && install
