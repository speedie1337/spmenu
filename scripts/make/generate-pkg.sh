#!/bin/sh
version="$(grep "version : '" meson.build | awk '{ print $3 }' | sed "s/'\"//g; s/\"',//g")"

rm -f spmenu spmenu-${version}.tar.gz spmenu-${version}.PKGBUILD *.o *zst*
mkdir -p spmenu-${version}
cp -rf LICENSE meson.build *.h *.c scripts/ docs/ libs/ PKGBUILD build.sh spmenu-${version}
[ -f buildconf ] && cp buildconf spmenu-${version} || :
[ -f Makefile ] && cp Makefile spmenu-${version} || :
[ -f host.mk ] && cp host.mk spmenu-${version} || :
[ -f toggle.mk ] && cp toggle.mk spmenu-${version} || :
[ -f spmenu.1 ] && cp spmenu.1 spmenu-${version} || :
tar -cf spmenu-${version}.tar spmenu-${version}
gzip spmenu-${version}.tar
rm -rf spmenu-${version}
