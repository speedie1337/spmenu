# Maintainer: speedie <speedie.alt@gmail.com>

pkgname=spmenu
pkgver=VERSION
pkgrel=1
pkgdesc="Fancy dynamic menu, compatible with dmenu!"
url="https://git.speedie.site/spmenu"
arch=(i686 x86_64)
license=(MIT)
depends=(sh libxinerama libxft pango libx11 imlib2 fribidi libconfig gcc)
makedepends=(git)
provides=($pkgname)
conflicts=($pkgname)
source=(
    "$pkgname-$pkgver.tar.gz"
    #"https://ls.speedie.site/releases/$pkgname/$pkgname-$pkgver.tar.gz"
)

md5sums=(MD5SUM)

build(){
  cd $pkgname-$pkgver
  make \
    X11INC=/usr/include/X11 \
    X11LIB=/usr/lib/X11 \
    CC=gcc
}

package() {
  cd "$pkgname-$pkgver"
  make PREFIX=/usr DESTDIR="$pkgdir" CC=gcc install
  install -Dm644 LICENSE "$pkgdir"/usr/share/licenses/$pkgname/LICENSE
}
