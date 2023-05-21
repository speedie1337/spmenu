void setupdisplay(void) {
    int x, y, i;
#if USEXINERAMA
    int j, di;
#endif
    unsigned int du;
    unsigned int tmp, minstrlen = 0, curstrlen = 0;
    int numwidthchecks = 100;
    struct item *item;
    XIM xim;
    Window w, dw, *dws;
    XWindowAttributes wa;
#if USEXINERAMA
    XineramaScreenInfo *info;
    Window pw;
    int a, n, area = 0;
#endif

    // set properties indicating what spmenu handles
    clip = XInternAtom(dpy, "CLIPBOARD",   False);
    utf8 = XInternAtom(dpy, "UTF8_STRING", False);
    types = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", False);
    dock = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DOCK", False);

    // resize client
    bh = MAX(drw->font->h, drw->font->h + 2 + lineheight);
    lines = MAX(lines, 0);
    reallines = lines;

    // resize client to image height if deemed necessary
#if USEIMAGE
    if (image) resizetoimageheight(imageheight);
#endif

    mh = (lines + 1) * bh + 2 * menumarginv; // lines + 1 * bh is the menu height

    // set prompt width based on prompt size
    promptw = (prompt && *prompt)
        ? pango_prompt ? TEXTWM(prompt) : TEXTW(prompt) - lrpad / 4 : 0; // prompt width

    // get accurate width
    if (accuratewidth) {
        for (item = items; !lines && item && item->text; ++item) {
            curstrlen = strlen(item->text);
            if (numwidthchecks || minstrlen < curstrlen) {
                numwidthchecks = MAX(numwidthchecks - 1, 0);
                minstrlen = MAX(minstrlen, curstrlen);
                if ((tmp = MIN(TEXTW(item->text), mw/3) > inputw)) {
                    inputw = tmp;
                    if (tmp == mw/3)
                        break;
                }
            }
        }
    }

    // init xinerama screens
#if USEXINERAMA
    i = 0;
    if (parentwin == root && (info = XineramaQueryScreens(dpy, &n))) {
        XGetInputFocus(dpy, &w, &di);
        if (mon >= 0 && mon < n)
            i = mon;
        else if (w != root && w != PointerRoot && w != None) {
            // find top-level window containing current input focus
            do {
                if (XQueryTree(dpy, (pw = w), &dw, &w, &dws, &du) && dws)
                    XFree(dws);
            } while (w != root && w != pw);
            // find xinerama screen with which the window intersects most
            if (XGetWindowAttributes(dpy, pw, &wa))
                for (j = 0; j < n; j++)
                    if ((a = INTERSECT(wa.x, wa.y, wa.width, wa.height, info[j])) > area) {
                        area = a;
                        i = j;
                    }
        }
        // no focused window is on screen, so use pointer location instead
        if (mon < 0 && !area && XQueryPointer(dpy, root, &dw, &dw, &x, &y, &di, &di, &du))
            for (i = 0; i < n; i++)
                if (INTERSECT(x, y, 1, 1, info[i]))
                    break;

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
            y = menuposition ? 0 : wa.width - mh - ypos;
            mw = wa.width;
        }
    }

    // might be faster in some instances, most of the time unnecessary
    if (!accuratewidth) inputw = MIN(inputw, mw/3);

    match(); // match entries

    // create menu window and set properties for it
    create_window(x + sp, y + vp - (menuposition == 1 ? 0 : menuposition == 2 ? borderwidth : borderwidth * 2), mw - 2 * sp - borderwidth * 2, mh);
    set_window();
    set_prop();

#if USEIMAGE
    setimageopts();
#endif

    // input methods
    if ((xim = XOpenIM(dpy, NULL, NULL, NULL)) == NULL) {
        XSetLocaleModifiers("@im=local");
        if ((xim = XOpenIM(dpy, NULL, NULL, NULL)) == NULL) {
            XSetLocaleModifiers("@im=");
            if ((xim = XOpenIM(dpy, NULL, NULL, NULL)) == NULL)
                die("XOpenIM failed: could not open input device");
        }
    }

    xic = XCreateIC(xim, XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
            XNClientWindow, win, XNFocusWindow, win, NULL);

    XMapRaised(dpy, win);

    XSync(dpy, False);
    XGetWindowAttributes(dpy, win, &wa);

    if (wa.map_state == IsViewable) // must be viewable, otherwise we get a BadMatch error
        XSetInputFocus(dpy, win, RevertToParent, CurrentTime);

    // embed spmenu inside parent window
    if (embed) {
        XReparentWindow(dpy, win, parentwin, x, y);
        XSelectInput(dpy, parentwin, FocusChangeMask | SubstructureNotifyMask);
        if (XQueryTree(dpy, parentwin, &dw, &w, &dws, &du) && dws) {
            for (i = 0; i < du && dws[i] != win; ++i)
                XSelectInput(dpy, dws[i], FocusChangeMask);
            XFree(dws);
        }
        grabfocus();
    }

    // resize and draw
    drw_resize(drw, mw - 2 * sp - borderwidth * 2, mh);
    drawmenu();
}
