void
prepare_window_size(void)
{
    // set horizontal and vertical padding
    sp = menupaddingh;
	vp = (menuposition == 1) ? menupaddingv : - menupaddingv;

    return;
}

void
create_window(int x, int y, int w, int h)
{
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
    win = XCreateWindow(dpy, parentwin, x, y, w, h, borderwidth,
                        depth, InputOutput, visual,
                        CWOverrideRedirect|CWBackPixel|CWBorderPixel|CWColormap|CWEventMask, &swa);


    return;
}

void
set_window(void)
{
    XClassHint ch = { class, class };

    // set border and class
    XSetWindowBorder(dpy, win, scheme[SchemeBorder][ColBg].pixel);
	XSetClassHint(dpy, win, &ch);
    return;
}

void
set_prop(void)
{
    if (dockproperty) XChangeProperty(dpy, win, types, XA_ATOM, 32, PropModeReplace, (unsigned char *) &dock, 1); // set dock property
    return;
}

void
resizeclient(void)
{
    int omh = mh;
    struct item *item;
    int itemCount = 0;

    // walk through all items
    for (item = items; item && item->text; item++)
        itemCount++;

    bh = MAX(drw->font->h, drw->font->h + 2 + lineheight);
	lines = MIN(itemCount, MAX(lines, 0));
    reallines = lines;

    // resize client to image height
    #if USEIMAGE
    if (image) resizetoimageheight(imageheight);
    #endif

    mh = (lines + 1) * bh;

    // why have an empty line? when there's nothing to draw there anyway?
    if (hideprompt && hideinput && hidemode && hidematchcount)
        mh += bh;

    // no window/invalid window or menu height we had before is the same as the current window height
    if (!win || omh == mh) return;

    XResizeWindow(dpy, win, mw, mh);
    drw_resize(drw, mw, mh);
}
