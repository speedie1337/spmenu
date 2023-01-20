# spmenu
# See LICENSE file for copyright details.

# compiler
CC = tcc

# paths
PREFIX = /usr

# library paths
#
# libx11
X11LIBS = -lX11
X11INC = /usr/X11R6/include
X11LIB = /usr/X11R6/lib

# freetype
FREETYPELIBS = -lfontconfig -lXft
FREETYPEINC = /usr/include/freetype2

# xft
XFT_CONF = xft

# pango
PANGO_CONF = pango
PANGOXFT_CONF = pangoxft

# xrender
XRENDERLIBS = -lXrender

# OpenBSD (uncomment)
#FREETYPEINC = $(X11INC)/freetype2

CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_XOPEN_SOURCE=700 -D_POSIX_C_SOURCE=200809L -DVERSION=\"$(VERSION)\" $(XINERAMAFLAGS)
CFLAGS   = -std=c99 -pedantic -Wall -Ofast $(INCS) $(CPPFLAGS)
LDFLAGS  = $(LIBS)
INCS = -I$(X11INC) -I$(FREETYPEINC) `pkg-config --cflags $(XFT_CONF) $(PANGO_CONF) $(PANGOXFT_CONF)`
LIBS = -L$(X11LIB) $(X11LIBS) $(XINERAMALIBS) $(FREETYPELIBS) $(XRENDERLIBS) -lm `pkg-config --libs $(XFT_CONF) $(PANGO_CONF) $(PANGOXFT_CONF)`
