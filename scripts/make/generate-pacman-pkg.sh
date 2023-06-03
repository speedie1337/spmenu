#!/bin/sh
# generate-pacman-pkg.sh
# Generate spmenu package for Arch and Pacman
#
# See LICENSE file for copyright and license details.
[ ! -f spmenu.c ] && printf "You're probably in the wrong directory.\n" && exit 1
version="$(grep "version : '" meson.build | awk '{ print $3 }' | sed "s/'\"//g; s/\"',//g")"

scripts/make/generate-pkg.sh
command -v makepkg > /dev/null || exit 1
[ -f PKGBUILD ] || exit 1
cp -f PKGBUILD PKGBUILD.orig
sed -i "s/VERSION/${version}/g; s/MD5SUM/$(md5sum *.tar.gz | cut -d ' ' -f 1)/g" PKGBUILD
makepkg -sfr --sign || exit 1
rm -rf src/ pkg/ *.tar.gz
cp PKGBUILD spmenu-${version}.PKGBUILD; mv PKGBUILD.orig PKGBUILD
