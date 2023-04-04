# spmenu - fancy dynamic menu
# See LICENSE file for copyright details.

# Multi-monitor support.
# Requires libXinerama
# Comment these lines if you don't need it.
XINERAMALIBS    = -lXinerama
XINERAMATOGGLE  = -DXINERAMA

# Right to left language support
# Comment these lines if you don't need it.
BDLIBS          = -lfribidi
BDINC           = $(INCDIR)/fribidi
BDTOGGLE 	    = -DRTL

# Pango
# Comment these lines if you don't need it.
PANGOCONF       = pango
PANGOXFTCONF    = pangoxft
PANGOTOGGLE     = -DPANGO

# Image support
# Comment these lines if you don't need it.
IMLIB2LIBS      = -lImlib2
IMLIB2TOGGLE    = -DIMAGE
OPENSSLCONF     = openssl

# Config file support
# Comment these lines if you don't need it.
LIBCONFIGTOGGLE = -DCONFIG
LIBCONFIGCONF   = libconfig
