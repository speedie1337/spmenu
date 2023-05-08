#!/bin/bash
# spmenu build script
PREFIX="${PREFIX:-/usr}"
DESTDIR="${DESTDIR:-}"
INCDIR="${INCDIR:-/usr/include}"
makebin="${makebin:-$(command -v make)}"
cc="${cc:-${CC:-gcc}}"
opt="${opt:-${OPT:--O2}}"
warn="${warn:-true}"
reconfigure="${reconfigure:-true}"

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
    [ ! -x "$(command -v "$cc")" ] && printf "%s not found in %s. Please make sure your system is set up properly." "$cc" "\$PATH"
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
    makebin="gnumake"
    GEN_MANUAL="false"
    PREFIX="/usr/local"
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
    [ "$GEN_MANUAL" != "false" ] && [ -x "$(command -v pandoc)" ] && command -v gen_manual > /dev/null && gen_manual

    cp -f meson.build meson.build.orig

    if [ "$libconfig" != "true" ]; then
        libconfig_text="libconfig = false"
    else
        libconfig_text="libconfig = true"
    fi

    if [ "$pango" != "true" ] || [ "$pangoxft" != "true" ]; then
        pango_text="pango = false"
        pangoxft_text="pangoxft = false"
    else
        pango_text="pango = true"
        pangoxft_text="pangoxft = false"
    fi

    if [ "$openssl" != "true" ] || [ "$imlib2" != "true" ]; then
        imlib2_text="imlib2 = false"
        openssl_text="openssl = false"
    else
        imlib2_text="imlib2 = true"
        openssl_text="openssl = true"
    fi

    if [ "$xinerama" != "true" ]; then
        xinerama_text="xinerama = false"
    else
        xinerama_text="xinerama = true"
    fi

    if [ "$fribidi" != "true" ]; then
        fribidi_text="fribidi = false"
    else
        fribidi_text="fribidi = true"
    fi

    if [ "$xresources" != "true" ]; then
        xresources_text="xresources = false"
    else
        xresources_text="xresources = true"
    fi

    sed -i "s/libconfig = true/$libconfig_text/" meson.build
    sed -i "s/pango = true/$pango_text/" meson.build
    sed -i "s/pangoxft = true/$pangoxft_text/" meson.build
    sed -i "s/imlib2 = true/$imlib2_text/" meson.build
    sed -i "s/openssl = true/$openssl_text/" meson.build
    sed -i "s/xinerama = true/$xinerama_text/" meson.build
    sed -i "s/fribidi = true/$fribidi_text/" meson.build
    sed -i "s/xresources = true/$xresources_text/" meson.build
    sed -i "s/opt = '-O2'/opt = '$opt'/g" meson.build

    mkdir -p build/

    if [ "$reconfigure" = "true" ]; then
        meson setup --reconfigure build
    else
        meson setup build
    fi

    ninja -C build

    mv meson.build.orig meson.build
}

install() {
    cd build/ || exit 1
    meson install
}

[ "$1" = "--no-install" ] && install=false

check_dist
check
loadconf
build
[ "$install" != "false" ] && install
