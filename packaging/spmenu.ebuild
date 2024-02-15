# Copyright 1999-2022 Gentoo Authors
# Distributed under the terms of the GNU General Public License v2

EAPI=8
inherit toolchain-funcs meson

DESCRIPTION="Fancy dynamic menu for X11 and Wayland, compatible with dmenu"
HOMEPAGE="https://spmenu.speedie.site"

if [[ ${PV} == 9999 ]]; then
	inherit git-r3
	EGIT_REPO_URI="https://git.speedie.site/speedie/${PN}"
else
	KEYWORDS="~amd64 ~arm ~arm64 ~hppa ~ppc ~ppc64 ~riscv ~x86"
	SRC_URI="https://ls.speedie.site/releases/${PN}/${PV}/${P}.tar.gz"
fi

LICENSE="MIT"
SLOT="0"
IUSE="+X +wayland +imlib2 +ssl +regex +fribidi +config +xinerama +xrdb +fifo +doc +run +test +man"
REQUIRED_USE="
	!wayland? ( X )
	!X? ( wayland )
	imlib2? ( ssl )
	ssl? ( imlib2 )
"

RDEPEND="
	X? ( x11-libs/libX11 x11-libs/libXrender )
	X? ( xrdb? ( x11-apps/xrdb ) )
	X? ( xinerama? ( x11-libs/libXinerama ) )
	wayland? ( dev-libs/wayland dev-util/wayland-scanner dev-libs/wayland-protocols )
	imlib2? ( media-libs/imlib2 )
	ssl? ( dev-libs/openssl )
	fribidi? ( dev-libs/fribidi )
	config? ( dev-libs/libconfig )
	x11-libs/pango
	x11-libs/cairo
"

DEPEND="${RDEPEND}"
BDEPEND="virtual/pkgconfig"

src_prepare() {
	default
}

src_configure() {
	local emesonargs=(
		$(meson_use wayland)
		$(meson_use X x11)
		$(meson_use xinerama)
		$(meson_use xrdb xresources)
		$(meson_use fribidi)
		$(meson_use config libconfig)
		$(meson_use regex)
		$(meson_use doc docs)
		$(meson_use man)
		$(meson_use run)
		$(meson_use test)
	)

	if use imlib2 && use ssl; then
		emesonargs+=($(meson_use ssl openssl))
		emesonargs+=($(meson_use imlib2))
	else
		emesonargs+=(-Dimlib2=false)
		emesonargs+=(-Dopenssl=false)
	fi


	meson_src_configure
}

src_compile() {
	meson_src_compile
}

src_install() {
	meson_install
}
