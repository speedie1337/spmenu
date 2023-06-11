/* See LICENSE file for copyright and license details. */

void hexconv(const char *hex, unsigned short *r, unsigned short *g, unsigned short *b) {
    unsigned int col;

    sscanf(hex, "#%06X", &col);

    *r = (col >> 16) & 0xFF;
    *g = (col >> 8) & 0xFF;
    *b = col & 0xFF;
}

void create_window_x11(int x, int y, int w, int h) {
    XSetWindowAttributes swa;

    swa.override_redirect = managed ? False : True;
    swa.background_pixel = 0;
    swa.colormap = cmap;
    swa.event_mask =
        ExposureMask | // mapping the drawing
        KeyPressMask | // keypresses
        VisibilityChangeMask | // whether or not client is visible
        ButtonPressMask |  // see buttonpress in libs/mouse.c for usage
        PointerMotionMask; // we need pointer for selecting entries using the mouse

    // create client
    win = XCreateWindow(dpy, root, x, y, w, h, borderwidth,
            depth, InputOutput, visual,
            CWOverrideRedirect|CWBackPixel|CWBorderPixel|CWColormap|CWEventMask, &swa);

    return;
}

void set_window_x11(void) {
    XColor col;
    XClassHint ch = { class, class };

    unsigned short r;
    unsigned short g;
    unsigned short b;

    hexconv(col_border, &r, &g, &b);

    col.red = r << 8;
    col.green = g << 8;
    col.blue = b << 8;

    if (!XAllocColor(dpy, cmap, &col)) {
        die("spmenu: failed to allocate xcolor");
    }

    // set border and class
    XSetWindowBorder(dpy, win, col.pixel);
    XSetClassHint(dpy, win, &ch);

    return;
}

void set_prop_x11(void) {
    // set properties indicating what spmenu handles
    clip = XInternAtom(dpy, "CLIPBOARD",   False);
    utf8 = XInternAtom(dpy, "UTF8_STRING", False);
    types = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", False);

    // set dock property
    if (dockproperty) {
        dock = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DOCK", False);
        XChangeProperty(dpy, win, types, XA_ATOM, 32, PropModeReplace, (unsigned char *) &dock, 1); // set dock property
    }
}

void resizeclient_x11(void) {
    int omh = mh;
    int x, y;
#if USEXINERAMA
    int j, di, a, n, area = 0;
    XineramaScreenInfo *info;
    Window pw;
    unsigned int du;
    Window w, dw, *dws;
#endif
    XWindowAttributes wa;
    struct item *item;
    int ic = 0; // item count

    // walk through all items
    for (item = items; item && item->text; item++)
        ic++;

    bh = MAX(drw->font->h, drw->font->h + 2 + lineheight);
    lines = MIN(ic, MAX(lines, 0));
    reallines = lines;

    // resize client to image height
#if USEIMAGE
    if (image) resizetoimageheight(imagewidth, imageheight);
#endif

    get_mh();

    if (hideprompt && hideinput && hidemode && hidematchcount && hidecaps) {
        mh -= bh;
    }

    // init xinerama screens
#if USEXINERAMA
    int i = 0;
    if (parentwin == root && (info = XineramaQueryScreens(dpy, &n))) {
        XGetInputFocus(dpy, &w, &di);
        if (mon >= 0 && mon < n) {
            i = mon;
        } else if (w != root && w != PointerRoot && w != None) {
            do {
                if (XQueryTree(dpy, (pw = w), &dw, &w, &dws, &du) && dws)
                    XFree(dws);
            } while (w != root && w != pw);
            if (XGetWindowAttributes(dpy, pw, &wa)) {
                for (j = 0; j < n; j++) {
                    if ((a = INTERSECT(wa.x, wa.y, wa.width, wa.height, info[j])) > area) {
                        area = a;
                        i = j;
                    }
                }
            }

            if (mon < 0 && !area && XQueryPointer(dpy, root, &dw, &dw, &x, &y, &di, &di, &du)) {
                for (i = 0; i < n; i++) {
                    if (INTERSECT(x, y, 1, 1, info[i])) {
                        break;
                    }
                }
            }
        }

        // calculate x/y position
        if (menuposition == 2) { // centered
            mw = MIN(MAX(max_textw() + promptw, minwidth), info[i].width);
            x = info[i].x_org + ((info[i].width  - mw) / 2);
            y = info[i].y_org + ((info[i].height - mh) / 2);
        } else { // top or bottom
            x = info[i].x_org + xpos;
            y = info[i].y_org + (menuposition ? 0 : info[i].height - mh - ypos);
            mw = (menuwidth>0 ? menuwidth : info[i].width);
        }

        XFree(info);
    } else
#endif
    {
        if (!XGetWindowAttributes(dpy, parentwin, &wa))
            die("spmenu: could not get embedding window attributes: 0x%lx",
                    parentwin); // die because unable to get attributes for the parent window
        if (menuposition == 2) { // centered
            mw = MIN(MAX(max_textw() + promptw, minwidth), wa.width);
            x = (wa.width  - mw) / 2;
            y = (wa.height - mh) / 2;
        } else { // top or bottom
            x = 0;
            y = menuposition ? 0 : wa.height - mh - ypos;
            mw = (menuwidth > 0 ? menuwidth : wa.width);
        }
    }

    // no window/invalid window or menu height we had before is the same as the current window height
    if (!win || omh == mh) return;

    XMoveResizeWindow(dpy, win, x + sp, y + vp, mw - 2 * sp - borderwidth * 2, mh);
    drw_resize(drw, mw - 2 * sp - borderwidth * 2, mh);
}

void xinitvisual(void) {
    XVisualInfo *infos;
    XRenderPictFormat *fmt;
    int nitems;
    int i;

    // visual properties
    XVisualInfo tpl = {
        .screen = screen,
        .depth = 32,
        .class = TrueColor
    };

    long masks = VisualScreenMask | VisualDepthMask | VisualClassMask;

    infos = XGetVisualInfo(dpy, masks, &tpl, &nitems);
    visual = NULL;

    // create colormap
    for(i = 0; i < nitems; i ++) {
        fmt = XRenderFindVisualFormat(dpy, infos[i].visual);
        if (fmt->type == PictTypeDirect && fmt->direct.alphaMask) {
            visual = infos[i].visual;
            depth = infos[i].depth;
            cmap = XCreateColormap(dpy, root, visual, AllocNone);
            useargb = 1;
            break;
        }
    }

    XFree(infos);

    // no alpha, reset to default
    if (!visual || !alpha) {
        visual = DefaultVisual(dpy, screen);
        depth = DefaultDepth(dpy, screen);
        cmap = DefaultColormap(dpy, screen);
    }
}
