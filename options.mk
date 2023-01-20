# spmenu
# See LICENSE file for copyright details.

# spmenu version
VERSION = 0.1

# includes and libs
INCS = -I$(X11INC) -I$(FREETYPEINC) `pkg-config --cflags xft pango pangoxft`
LIBS = -L$(X11LIB) -lX11 $(XINERAMALIBS) $(FREETYPELIBS) ${XRENDERLIBS} -lm `pkg-config --libs xft pango pangoxft`
