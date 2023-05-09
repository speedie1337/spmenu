#!/bin/bash
# spmenu build script
opt="${opt:-${OPT:--O2}}"
warn="${warn:-true}"
reconfigure="${reconfigure:-true}"
version="${version:-1.1}"
xresources="${xresources:-true}"

check_dist() {
    [ -f "/etc/pacman.conf" ] && [ "$warn" != "false" ] && printf "hint: detected Pacman. if you want you can run 'makepkg' with proper arguments to install it using pacman.\n" && pacman=true
    [ -f "/System/Library/CoreServices/SystemVersion.plist" ] && [ "$warn" != "false" ] && printf "hint: detected a Mac. Please report any issues you find as it is untested.\n" && mac=true
    uname -a | grep -q OpenBSD && printf "hint: detected OpenBSD. Please report any issues you find as it is untested.\n" && openbsd=true
}

check() {
    if [ "$mac" != "true" ]; then
    [ ! -x "$(command -v ldconfig)" ] && printf "ldconfig not found in %s. Please make sure your system is set up properly." "\$PATH" && exit 1
    [ ! -x "$(command -v ninja)" ] && printf "ninja not found in %s. Please make sure your system is set up properly." "\$PATH" && exit 1
    [ ! -x "$(command -v meson)" ] && printf "meson not found in %s. Please make sure your system is set up properly." "\$PATH" && exit 1
    [ -n "$(ldconfig -p | grep Imlib2)" ] && printf "Imlib2 found\n" && imlib2=true || imlib2=false
    [ -n "$(ldconfig -p | grep libXft)" ] && printf "libXft found\n" && xft=true || xft=false
    [ -n "$(ldconfig -p | grep libX11)" ] && printf "libX11 found\n" && x11=true || x11=false
    [ -n "$(ldconfig -p | grep libXrender)" ] && printf "libXrender found\n" && xrender=true || xrender=false
    [ -n "$(ldconfig -p | grep libpango)" ] && printf "libpango found\n" && pango=true || pango=false
    [ -n "$(ldconfig -p | grep libpangoxft)" ] && printf "libpangoxft found\n" && pangoxft=true || pangoxft=false
    [ -n "$(ldconfig -p | grep libxcb-xinerama)" ] && printf "libxcb-xinerama found\n" && xinerama=true || xinerama=false
    [ -n "$(ldconfig -p | grep libgnutls-openssl)" ] && printf "openssl found\n" && openssl=true || openssl=false
    [ -n "$(ldconfig -p | grep fribidi)" ] && printf "fribidi found\n" && fribidi=true || fribidi=false
    [ -n "$(ldconfig -p | grep freetype)" ] && printf "freetype found\n" && freetype=true || freetype=false
    [ -n "$(ldconfig -p | grep libconfig)" ] && printf "libconfig found\n" && libconfig=true || libconfig=false
    else
    gen_manual="false"
    fi
}

loadconf() {
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

    if [ "$reconfigure" = "true" ]; then
        meson setup --reconfigure \
            -Dxresources="$xresources" \
            -Dfribidi="$fribidi" \
            -Dxinerama="$xinerama" \
            -Dimlib2="$imlib2" \
            -Dopenssl="$openssl" \
            -Dpango="$pango" \
            -Dpangoxft="$pangoxft" \
            -Dlibconfig="$libconfig" \
            build
    else
        meson setup \
            -Dxresources="$xresources" \
            -Dfribidi="$fribidi" \
            -Dxinerama="$xinerama" \
            -Dimlib2="$imlib2" \
            -Dopenssl="$openssl" \
            -Dpango="$pango" \
            -Dpangoxft="$pangoxft" \
            -Dlibconfig="$libconfig" \
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

[ "$1" = "--no-install" ] && install=false

check_dist
check
loadconf
build
[ "$install" != "false" ] && install
