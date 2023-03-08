# spmenu

# See LICENSE file for copyright and license details.

include options.mk
include host.mk
include toggle.mk

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


$(OBJ): options.h options.mk libs/sl/draw.h

spmenu: spmenu.o libs/sl/draw.o libs/sl/main.o
	$(CC) -o $@ spmenu.o draw.o main.o $(LDFLAGS)

clean:
	rm -f spmenu *.o \
		spmenu-$(VERSION).tar.gz \
		*zst* spmenu-$(VERSION).PKGBUILD

dist: clean
	mkdir -p spmenu-$(VERSION)
	cp -rf LICENSE Makefile *.h *.mk *.c scripts/ docs/ libs/ PKGBUILD/ spmenu-$(VERSION)
	[ -f spmenu.1 ] && cp spmenu.1 spmenu-$(VERSION) || :
	tar -cf spmenu-$(VERSION).tar spmenu-$(VERSION)
	gzip spmenu-$(VERSION).tar
	rm -rf spmenu-$(VERSION)

install: all
	rm -rf $(DESTDIR)$(PREFIX)/share/spmenu/
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(PREFIX)/share/spmenu
	cp -r docs/* $(DESTDIR)$(PREFIX)/share/spmenu/
	echo "${VERSION}" > $(DESTDIR)$(PREFIX)/share/spmenu/version
	cp -r spmenu scripts/spmenu* $(DESTDIR)$(PREFIX)/bin
	[ -f spmenu.1 ] && mkdir -p ${DESTDIR}${MANPREFIX}/man1 || :
	[ -f spmenu.1 ] && cp spmenu.1 ${DESTDIR}${MANPREFIX}/man1/spmenu.1 || :
	chmod 755 $(DESTDIR)$(PREFIX)/bin/spmenu*
	rm -f *.o
	rm -f spmenu

compat:
	rm -f $(DESTDIR)$(PREFIX)/bin/dmenu
	rm -f $(DESTDIR)$(PREFIX)/bin/dmenu_run
	ln -s $(DESTDIR)$(PREFIX)/bin/spmenu $(DESTDIR)$(PREFIX)/bin/dmenu
	ln -s $(DESTDIR)$(PREFIX)/bin/spmenu_run $(DESTDIR)$(PREFIX)/bin/dmenu_run

uninstall:
		  $(DESTDIR)$(PREFIX)/bin/spmenu*\

help:
	@echo install: Installs spmenu. You may need to run this as root.
	@echo uninstall: Uninstalls spmenu. You may need to run this as root.
	@echo dist: Creates a release for spmenu.
	@echo clean: Removes objects and tarballs.
	@echo man: Generate man page for spmenu
	@echo compat: Installs compatibility with dmenu. WARNING: This will overwrite dmenu and dmenu_run
	@echo pkg_arch: Creates an Arch package based on the PKGBUILD
	@echo help: Displays this help sheet.

man:
	printf "%% spmenu(1) $(VERSION) | dynamic menu\n" > .man.md
	grep -v docs/preview.png docs/docs.md >> .man.md
	pandoc --standalone --to man .man.md -o spmenu.1
	rm -f .man.md

pkg_arch: dist
	command -v makepkg > /dev/null || exit 1
	[ -f PKGBUILD ] || exit 1
	cp -f PKGBUILD PKGBUILD.orig
	sed -i "s/VERSION/$(VERSION)/g; s/MD5SUM/$$(md5sum *.tar.gz | cut -d ' ' -f 1)/g" PKGBUILD
	makepkg -sfr --sign || exit 1
	rm -rf src/ pkg/ *.tar.gz
	cp PKGBUILD spmenu-$(VERSION).PKGBUILD; mv PKGBUILD.orig PKGBUILD

.PHONY: all options clean dist install uninstall pkg_arch help man
