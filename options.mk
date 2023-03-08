# spmenu
# See LICENSE file for copyright details.

# spmenu version
VERSION = 0.2.1

# includes and libs
INCS = -I$(X11INC) -I$(FREETYPEINC) $(BDINC) `pkg-config --cflags xft $(PANGO_CONF) $(PANGO_XFT_CONF)`
LIBS = -L$(X11LIB) -lX11 $(XINERAMALIBS) $(FREETYPELIBS) $(XRENDERLIBS) $(IMLIB2LIBS) $(BDLIBS) -lm `pkg-config --libs xft $(PANGO_CONF) $(PANGO_XFT_CONF)`
