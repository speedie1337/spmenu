# spmenu - fancy dynamic menu
# See LICENSE file for copyright and license details.

include host.mk
include toggle.mk

# spmenu version
VERSION = 0.4.1

CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_XOPEN_SOURCE=700L -D_POSIX_C_SOURCE=200809L -DVERSION=\"$(VERSION)\" $(XINERAMATOGGLE) $(BDTOGGLE) $(PANGOTOGGLE) $(IMLIB2TOGGLE) $(LIBCONFIGTOGGLE)
CFLAGS   = -std=c99 -pedantic -Wall -Wno-deprecated-declarations -faggressive-loop-optimizations $(OPT) $(INCS) $(CPPFLAGS)
LDFLAGS  = $(LIBS)
INCS = -I$(X11INC) -I$(FREETYPEINC) -I$(BDINC) `pkg-config --cflags $(XFTCONF) $(PANGOCONF) $(PANGOXFTCONF) $(OPENSSLCONF) $(LIBCONFIGCONF)`
LIBS = -L$(X11LIB) $(X11LIBS) $(XINERAMALIBS) $(FREETYPELIBS) $(XRENDERLIBS) -lm `pkg-config --libs $(XFTCONF) $(PANGOCONF) $(PANGOXFTCONF) $(OPENSSLCONF) $(LIBCONFIGCONF)` $(BDLIBS) $(IMLIB2LIBS)

SRC = libs/sl/draw.c spmenu.c libs/sl/main.c
OBJ = $(SRC:.c=.o)

all: options spmenu

options:
	@echo spmenu build options:
	@echo "CFLAGS   = $(CFLAGS)"
	@echo "LDFLAGS  = $(LDFLAGS)"
	@echo "CC       = $(CC)"

.c.o:
	$(CC) -c $(CFLAGS) -g $<


$(OBJ): options.h libs/sl/draw.h

spmenu: spmenu.o libs/sl/draw.o libs/sl/main.o
	$(CC) -o $@ spmenu.o draw.o main.o $(LDFLAGS)

clean:
	rm -f spmenu spmenu-$(VERSION).tar.gz spmenu-$(VERSION).PKGBUILD *.o *zst*

dist: clean man
	mkdir -p spmenu-$(VERSION)
	cp -rf LICENSE Makefile *.h *.mk *.c scripts/ docs/ libs/ PKGBUILD build.sh spmenu-$(VERSION)
	[ -f buildconf ] && cp buildconf spmenu-$(VERSION) || :
	[ -f spmenu.1 ] && cp spmenu.1 spmenu-$(VERSION) || :
	tar -cf spmenu-$(VERSION).tar spmenu-$(VERSION)
	gzip spmenu-$(VERSION).tar
	rm -rf spmenu-$(VERSION)

install: spmenu
	rm -rf $(DESTDIR)$(PREFIX)/share/spmenu/
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(PREFIX)/share/spmenu
	cp docs/example.Xresources example.Xresources.orig && sed -i "s/VERSION/$(VERSION)/g" docs/example.Xresources
	cp -r docs/* $(DESTDIR)$(PREFIX)/share/spmenu/
	mv example.Xresources.orig docs/example.Xresources
	echo "$(VERSION)" > $(DESTDIR)$(PREFIX)/share/spmenu/version
	echo "$(CC)" > $(DESTDIR)$(PREFIX)/share/spmenu/cc
	echo "$(CFLAGS)" > $(DESTDIR)$(PREFIX)/share/spmenu/cflags
	echo "$$(date "+%D %T")" > $(DESTDIR)$(PREFIX)/share/spmenu/compile-date
	cp -r spmenu scripts/spmenu* $(DESTDIR)$(PREFIX)/bin
	[ -f spmenu.1 ] && mkdir -p $(DESTDIR)$(MANPREFIX)/man1 || :
	[ -f spmenu.1 ] && cp spmenu.1 $(DESTDIR)$(MANPREFIX)/man1/spmenu.1 || :
	chmod 755 $(DESTDIR)$(PREFIX)/bin/spmenu*
	rm -f *.o
	rm -f spmenu

compat:
	rm -f $(DESTDIR)$(PREFIX)/bin/dmenu
	rm -f $(DESTDIR)$(PREFIX)/bin/dmenu_run
	ln -s $(DESTDIR)$(PREFIX)/bin/spmenu $(DESTDIR)$(PREFIX)/bin/dmenu
	ln -s $(DESTDIR)$(PREFIX)/bin/spmenu_run $(DESTDIR)$(PREFIX)/bin/dmenu_run

uninstall:
		rm -f \
		  $(DESTDIR)$(PREFIX)/bin/spmenu*

help:
	@echo spmenu Makefile help
	@echo
	@echo install:      Installs spmenu. You may need to run this as root.
	@echo uninstall:    Uninstalls spmenu. You may need to run this as root.
	@echo install_arch: Uses the PKGBUILD to install spmenu using pacman.
	@echo dist:         Creates a release for spmenu.
	@echo clean:        Removes objects and tarballs.
	@echo docs:         Generate full documentation for spmenu
	@echo man:          Generate man page for spmenu
	@echo compat:       Installs compatibility with dmenu. WARNING: This will overwrite dmenu and dmenu_run
	@echo commit:       Commit to the repository
	@echo pkg_arch:     Creates an Arch package based on the PKGBUILD
	@echo help:         Displays this help sheet.

man:
	printf "%% spmenu(1) $(VERSION) | fancy dynamic menu\n" > .man.md
	grep -v docs/preview.png docs/docs.md >> .man.md
	pandoc --standalone --to man .man.md -o spmenu.1
	pandoc --standalone .man.md -o spmenu.html
	rm -f .man.md

docs: man
	command -v weasyprint || exit 1
	scripts/make/generate-code-docs.sh docs/code-docs.md code.html || exit 1
	pandoc --standalone README.md -o README.html
	weasyprint code.html code.pdf || exit 1
	weasyprint README.html README.pdf || exit 1
	weasyprint spmenu.html spmenu.pdf || exit 1

pkg_arch: dist
	command -v makepkg > /dev/null || exit 1
	[ -f PKGBUILD ] || exit 1
	cp -f PKGBUILD PKGBUILD.orig
	sed -i "s/VERSION/$(VERSION)/g; s/MD5SUM/$$(md5sum *.tar.gz | cut -d ' ' -f 1)/g" PKGBUILD
	makepkg -sfr --sign || exit 1
	rm -rf src/ pkg/ *.tar.gz
	cp PKGBUILD spmenu-$(VERSION).PKGBUILD; mv PKGBUILD.orig PKGBUILD

install_arch: dist
	command -v makepkg > /dev/null || exit 1
	[ -f PKGBUILD ] || exit 1
	cp -f PKGBUILD PKGBUILD.orig
	sed -i "s/VERSION/$(VERSION)/g; s/MD5SUM/$$(md5sum *.tar.gz | cut -d ' ' -f 1)/g" PKGBUILD
	makepkg -si || :
	rm -rf src/ pkg/ *.tar.gz
	cp PKGBUILD spmenu-$(VERSION).PKGBUILD; mv PKGBUILD.orig PKGBUILD

commit: docs
	command -v git > /dev/null || exit 1
	git add *
	git commit -a || :

.PHONY: all options clean dist install install_arch uninstall pkg_arch help man docs commit
