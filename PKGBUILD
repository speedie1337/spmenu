# Maintainer: speedie <speedie@speedie.site>

pkgname=spmenu
pkgver=VERSION
pkgrel=1
pkgdesc="Fancy dynamic menu for X11 and Wayland, compatible with dmenu!"
url="https://spmenu.speedie.site"
arch=(i686 x86_64)
license=(MIT)
depends=(
    sh
    libxinerama
    cairo
    pango
    wayland
    wayland-protocols
    libxkbcommon
    libx11
    imlib2
    fribidi
    libconfig
)

makedepends=(
    git
    meson
    ninja
)

provides=($pkgname)
conflicts=($pkgname)
source=(
    "$pkgname-$pkgver.tar.gz"
    "$pkgname-$pkgver.tar.gz.sig"
    #"https://ls.speedie.site/releases/$pkgname/$pkgname-$pkgver.tar.gz"
    #"https://ls.speedie.site/releases/$pkgname/$pkgname-$pkgver.tar.gz.sig"
)

md5sums=(
    MD5SUM
    'SKIP'
)

build(){
  cd $pkgname-$pkgver
  mkdir -p build/
  meson setup --reconfigure --prefix=${pkgdir}/usr build
  ninja -C build
}

package() {
  cd "$pkgname-$pkgver"

  meson install -C build
}
