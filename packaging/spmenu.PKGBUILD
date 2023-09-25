# Maintainer: speedie <speedie@speedie.site>

pkgname=('spmenu' 'libspmenu')
pkgbase="spmenu"
pkgver=VERSION
pkgrel=1
pkgdesc="Fancy dynamic menu for X11 and Wayland, compatible with dmenu!"
url="https://spmenu.speedie.site"
arch=(i686 x86_64)
license=(MIT)
depends=(
    bash
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

optdepends=(
    'wl-clipboard: Clipboard support on Wayland'
    'libspmenu: C++ library for spmenu'
)

provides=($pkgname)
source=(
    "$pkgbase-$pkgver.tar.gz"
    "$pkgbase-$pkgver.tar.gz.sig"
)

md5sums=(
    MD5SUM
    'SKIP'
)

sha256sums=(
    SHA256SUM
    'SKIP'
)

package_spmenu() {
  cd "$pkgbase-$pkgver"
  meson setup --reconfigure --prefix=${pkgdir}/usr build
  cd build/
  meson install
}

package_libspmenu() {
  pkgdesc="C++ library for spmenu"
  optdepends=()

  cd "$pkgbase-$pkgver/src/libspmenu"
  meson setup --reconfigure --prefix=${pkgdir}/usr build
  cd build/
  meson install
}
