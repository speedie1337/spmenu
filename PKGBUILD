# Maintainer: speedie <speedie.alt@gmail.com>

pkgname=spmenu
pkgver=VERSION
pkgrel=1
pkgdesc="Fancy dynamic menu, compatible with dmenu!"
url="https://spmenu.speedie.site"
arch=(i686 x86_64)
license=(MIT)
depends=(sh libxinerama libxft pango libx11 imlib2 fribidi libconfig)
makedepends=(git meson ninja pandoc)
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

  mkdir -p "$pkgdir/usr/share/spmenu"
  install -Dm644 docs/docs.md "$pkgdir/usr/share/spmenu/"
  install -Dm644 docs/code-docs.md "$pkgdir/usr/share/spmenu/"
  install -Dm644 docs/example.Xresources "$pkgdir/usr/share/spmenu/"
  install -Dm644 docs/spmenu.conf "$pkgdir/usr/share/spmenu/"
  install -Dm644 LICENSE "$pkgdir/usr/share/licenses/$pkgname/LICENSE"
  install -Dm755 scripts/spmenu_run "$pkgdir/usr/bin/spmenu"
  install -Dm755 scripts/spmenu_test "$pkgdir/usr/bin/spmenu"
}
