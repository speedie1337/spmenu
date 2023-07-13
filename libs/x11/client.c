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
    swa.colormap = x11.cmap;
    swa.event_mask =
        ExposureMask | // mapping the drawing
        KeyPressMask | // keypresses
        VisibilityChangeMask | // whether or not client is visible
        ButtonPressMask |  // see buttonpress in libs/mouse.c for usage
        PointerMotionMask; // we need pointer for selecting entries using the mouse

    // create client
    win = XCreateWindow(dpy, root, x, y, w, h, borderwidth,
            x11.depth, InputOutput, x11.visual,
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

    if (!XAllocColor(dpy, x11.cmap, &col)) {
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
    int mh = sp.mh;
    int x, y;
    struct item *item;
    int ic = 0; // item count

    // walk through all items
    for (item = items; item && item->text; item++)
        ic++;

    sp.bh = MAX(draw->font->h, draw->font->h + 2 + lineheight);
    lines = MIN(ic, MAX(lines, 0));
#if USEIMAGE
    img.setlines = lines;

    // resize client to image height
    if (image) resizetoimageheight(img.imageheight);
#endif

    get_mh();

    if (hideprompt && hideinput && hidemode && hidematchcount && hidecaps) {
        sp.mh -= sp.bh;
    }

    if (menuposition == 2) { // centered
        sp.mw = MIN(MAX(max_textw() + sp.promptw, centerwidth), mo.output_width);
        x = (mo.output_width - sp.mw) / 2 + xpos;
        y = (mo.output_height - sp.mh) / 2 - ypos;
    } else { // top or bottom
        x = 0;
        y = menuposition ? 0 : mo.output_height - sp.mh - ypos;
        sp.mw = (menuwidth > 0 ? menuwidth : mo.output_width);
    }

    // no window/invalid window or menu height we had before is the same as the current window height
    if (!win || mh == sp.mh) return;

    XMoveResizeWindow(dpy, win, x + sp.sp, y + sp.vp, sp.mw - 2 * sp.sp - borderwidth * 2, sp.mh);
    draw_resize(draw, sp.mw - 2 * sp.sp - borderwidth * 2, sp.mh);
}

void xinitvisual(void) {
    XVisualInfo *infos;
    XRenderPictFormat *fmt;
    int nitems;
    int i;

    // visual properties
    XVisualInfo tpl = {
        .screen = x11.screen,
        .depth = 32,
        .class = TrueColor
    };

    long masks = VisualScreenMask | VisualDepthMask | VisualClassMask;

    infos = XGetVisualInfo(dpy, masks, &tpl, &nitems);
    x11.visual = NULL;

    // create colormap
    for(i = 0; i < nitems; i ++) {
        fmt = XRenderFindVisualFormat(dpy, infos[i].visual);
        if (fmt->type == PictTypeDirect && fmt->direct.alphaMask) {
            x11.visual = infos[i].visual;
            x11.depth = infos[i].depth;
            x11.cmap = XCreateColormap(dpy, root, x11.visual, AllocNone);
            x11.useargb = 1;
            break;
        }
    }

    XFree(infos);

    // no alpha, reset to default
    if (!x11.visual || !alpha) {
        x11.visual = DefaultVisual(dpy, x11.screen);
        x11.depth = DefaultDepth(dpy, x11.screen);
        x11.cmap = DefaultColormap(dpy, x11.screen);
    }
}
