# Maintainer: speedie <speedie.alt@gmail.com>

pkgname=spmenu
pkgver=0.2
pkgrel=1
pkgdesc="speedie's fork of suckless dmenu with color support and more"
url="https://git.speedie/gq/spmenu"
arch=(i686 x86_64)
license=(MIT)
depends=(sh libxinerama libxft pango libx11 imlib2 fribidi tcc-ziyao)
makedepends=(git)
provides=($pkgname)
conflicts=($pkgname)
source=(
    "https://ls.speedie.gq/releases/$pkgname/$pkgname-$pkgver.tar.gz"
)

md5sums=(5d51737b7e3cac79d08c2ffe3de34090)

build(){
  cd $pkgname-$pkgver
  make \
    X11INC=/usr/include/X11 \
    X11LIB=/usr/lib/X11 CC=tcc
}

package() {
  cd "$pkgname-$pkgver"
  make PREFIX=/usr DESTDIR="$pkgdir" CC=tcc install
  install -Dm644 LICENSE "$pkgdir"/usr/share/licenses/$pkgname/LICENSE
}
