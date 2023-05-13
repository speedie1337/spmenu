# Maintainer: speedie <speedie@speedie.site>

pkgname=spmenu
pkgver=VERSION
pkgrel=1
pkgdesc="Fancy dynamic menu, compatible with dmenu!"
url="https://spmenu.speedie.site"
arch=(i686 x86_64)
license=(MIT)
depends=(sh libxinerama libxft pango libx11 imlib2 fribidi libconfig)
makedepends=(git meson ninja)
provides=($pkgname)
conflicts=($pkgname)
source=(
    "$pkgname-$pkgver.tar.gz"
    #"https://ls.speedie.site/releases/$pkgname/$pkgname-$pkgver.tar.gz"
)

md5sums=(MD5SUM)

build(){
  cd $pkgname-$pkgver
  mkdir -p build/
  meson setup --reconfigure build
  ninja -C build
}

package() {
  cd "$pkgname-$pkgver"

  meson install -C build --destdir "$pkgdir"
}
