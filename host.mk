# spmenu - dynamic menu
# See LICENSE file for copyright details.

# compiler
CC           = tcc

# optimization
OPT          = -O2

# paths
PREFIX       = /usr
MANPREFIX 	 = ${PREFIX}/share/man

# library paths
#
# libx11
X11LIBS      = -lX11
X11INC       = /usr/X11R6/include
X11LIB       = /usr/X11R6/lib

# freetype
FREETYPELIBS = -lfontconfig -lXft
FREETYPEINC  = /usr/include/freetype2

# xft
XFTCONF      = xft

# xrender
XRENDERLIBS  = -lXrender

# imlib2
IMLIB2LIBS   = -lImlib2

# openssl
OPENSSLCONF  = openssl

# OpenBSD (uncomment)
#FREETYPEINC = $(X11INC)/freetype2

# macOS with XQuartz (uncomment)
#FREETYPEINC = /usr/local/include/freetype2
#X11INC      = /opt/X11/include
#X11LIB      = /opt/X11/lib

CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_XOPEN_SOURCE=700 -D_POSIX_C_SOURCE=200809L -DVERSION=\"$(VERSION)\" $(XINERAMATOGGLE) $(BDTOGGLE) $(PANGOTOGGLE) $(IMLIB2TOGGLE)
CFLAGS   = -std=c99 -pedantic -Wall $(OPT) $(INCS) $(CPPFLAGS)
LDFLAGS  = $(LIBS)
INCS = -I$(X11INC) -I$(FREETYPEINC) -I$(BDINC) `pkg-config --cflags $(XFTCONF) $(PANGOCONF) $(PANGOXFTCONF) $(OPENSSLCONF)`
LIBS = -L$(X11LIB) $(X11LIBS) $(XINERAMALIBS) $(FREETYPELIBS) $(XRENDERLIBS) -lm `pkg-config --libs $(XFTCONF) $(PANGOCONF) $(PANGOXFTCONF) $(OPENSSLCONF)` $(BDLIBS) $(IMLIB2LIBS)
