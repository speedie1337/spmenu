/* See LICENSE file for copyright and license details. */

// include libraries
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include <X11/Xproto.h>
#include <X11/extensions/Xrender.h>

// include xinerama
#if USEXINERAMA
#include <X11/extensions/Xinerama.h>
#endif

// include xresources
#if USEXRESOURCES
#include <X11/Xresource.h>
#endif
