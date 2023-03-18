void
prepare_window_size(void)
{
    sp = menupaddingh;
	vp = (menuposition == 1) ? menupaddingv : - menupaddingv;
}

void
create_window(int x, int y, int w, int h)
{
    XSetWindowAttributes swa;

    swa.override_redirect = managed ? False : True;
	swa.background_pixel = 0;
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | VisibilityChangeMask | ButtonPressMask | PointerMotionMask;

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

    XSetWindowBorder(dpy, win, scheme[SchemeBorder][ColBg].pixel);
	XSetClassHint(dpy, win, &ch);

    return;
}

void
set_prop(void)
{
    XChangeProperty(dpy, win, types, XA_ATOM, 32, PropModeReplace, (unsigned char *) &dock, 1);

    return;
}

void
resizeclient(void)
{
    int omh = mh;

    bh = MAX(drw->font->h, drw->font->h + 2 + lineheight);
	lines = MAX(lines, 0);
    reallines = lines;

    #if USEIMAGE
    if (image)
        resizetoimageheight(imageheight);
    #endif

    mh = (lines + 1) * bh;

    if (!win || omh == mh) return;

    XResizeWindow(dpy, win, mw, mh);
    drw_resize(drw, mw, mh);
}
