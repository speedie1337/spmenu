# spmenu - dynamic menu
# See LICENSE file for copyright details.

# compiler
CC           = tcc

# optimization
OPT          = -O2

# paths
PREFIX       = /usr
MANPREFIX 	 = ${PREFIX}/share/man
INCDIR       = /usr/include

# library paths
#
# libx11
X11LIBS      = -lX11
X11INC       = /usr/X11R6/include
X11LIB       = /usr/X11R6/lib

# freetype
FREETYPELIBS = -lfontconfig -lXft
FREETYPEINC  = ${INCDIR}/freetype2

# xft
XFTCONF      = xft

# xrender
XRENDERLIBS  = -lXrender

# OpenBSD (uncomment)
#INCDIR      = $(X11INC)
#FREETYPEINC = $(INCDIR)/freetype2

# macOS with XQuartz (uncomment)
#INCDIR      = /usr/local/include
#FREETYPEINC = $(INCDIR)/freetype2
#X11INC      = /opt/X11/include
#X11LIB      = /opt/X11/lib
