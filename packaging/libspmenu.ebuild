# Copyright 1999-2022 Gentoo Authors
# Distributed under the terms of the GNU General Public License v2

EAPI=8
inherit toolchain-funcs meson

DESCRIPTION="C++ library for spmenu"
HOMEPAGE="https://spmenu.speedie.site"

if [[ ${PV} == 9999 ]]; then
	inherit git-r3
	EGIT_REPO_URI="https://git.speedie.site/speedie/spmenu"
else
	KEYWORDS="~amd64 ~arm ~arm64 ~hppa ~ppc ~ppc64 ~riscv ~x86"
	SRC_URI="https://ls.speedie.site/spmenu-${PV}.tar.gz -> ${P}.tar.gz"
	S="${WORKDIR}/spmenu-${PV}"
fi

EMESON_SOURCE="${S}/src/libspmenu"

LICENSE="MIT"
SLOT="0"
IUSE=""

RDEPEND="x11-misc/spmenu"

DEPEND="${RDEPEND}"
BDEPEND="virtual/pkgconfig"

src_prepare() {
	default
}

src_configure() {
	meson_src_configure
}

src_compile() {
	meson_src_compile
}

src_install() {
	meson_install
}
