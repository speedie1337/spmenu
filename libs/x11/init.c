/* See LICENSE file for copyright and license details. */
void setupdisplay_x11(void) {
    int x, y, i;
    unsigned int du;

    Window w, dw, *dws;
    XWindowAttributes wa;

    prepare_window_size_x11();

    // resize client to image height if deemed necessary
#if USEIMAGE
    if (image) resizetoimageheight(imagewidth, imageheight);
#endif

    // set prompt width based on prompt size
    promptw = (prompt && *prompt)
        ? pango_prompt ? TEXTWM(prompt) : TEXTW(prompt) - lrpad / 4 : 0; // prompt width

    // init xinerama screens
#if USEXINERAMA
    XineramaScreenInfo *info;
    Window pw;
    int a, n, area = 0;
    int j, di;

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
            x = info[i].x_org + xpos + ((info[i].width  - mw) / 2);
            y = info[i].y_org - ypos + ((info[i].height - mh) / 2);
        } else { // top or bottom
            x = info[i].x_org + xpos;
            y = info[i].y_org + (menuposition ? 0 : info[i].height - mh - ypos);
            mw = (menuwidth > 0 ? menuwidth : info[i].width);
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
            x = (wa.width  - mw) / 2 + xpos;
            y = (wa.height - mh) / 2 - ypos;
        } else { // top or bottom
            x = 0;
            y = menuposition ? 0 : wa.width - mh - ypos;
            mw = (menuwidth > 0 ? menuwidth : wa.width);
        }
    }

    // create menu window and set properties for it
    create_window_x11(
            x + sp,
            y + vp - (menuposition == 1 ? 0 : menuposition == 2 ? borderwidth : borderwidth * 2),
            mw - 2 * sp - borderwidth * 2,
            mh
    );

    set_window_x11();
    set_prop_x11();

#if USEIMAGE
    setimageopts();
#endif

    open_xim(); // open xim

    XMapRaised(dpy, win);
    XSync(dpy, False);
    XGetWindowAttributes(dpy, win, &wa);

    if (wa.map_state == IsViewable) { // must be viewable, otherwise we get a BadMatch error
        XSetInputFocus(dpy, win, RevertToParent, CurrentTime);
    }

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

    // resize window and draw
    drw_resize(drw, mw - 2 * sp - borderwidth * 2, mh);

    match();
    drawmenu();
}

void prepare_window_size_x11(void) {
    sp = menupaddingh;
    vp = (menuposition == 1) ? menupaddingv : - menupaddingv;

    bh = MAX(drw->font->h, drw->font->h + 2 + lineheight);
    lines = MAX(lines, 0);
    reallines = lines;

    lrpad = drw->font->h + textpadding;
    mh = (lines + 1) * bh + 2 * menumarginv; // lines + 1 * bh is the menu height

    return;
}

Display * opendisplay_x11(char *disp) {
    return XOpenDisplay(disp);
}

void set_screen_x11(Display *disp) {
    screen = DefaultScreen(disp);
    root = RootWindow(disp, screen);
}

void handle_x11(void) {
    XWindowAttributes wa;

    if (!setlocale(LC_CTYPE, "") || !XSupportsLocale())
        fputs("warning: no locale support\n", stderr); // invalid locale, so notify the user about it

    if (!XSetLocaleModifiers(""))
        fputs("warning: no locale modifiers support\n", stderr);

    if (!(dpy = opendisplay_x11(NULL)))
        die("spmenu: cannot open display"); // failed to open display

    // set screen and root window
    set_screen_x11(dpy);

    // parent window is the root window (ie. window manager) because we're not embedding
    if (!embed || !(parentwin = strtol(embed, NULL, 0)))
        parentwin = root;

    if (!XGetWindowAttributes(dpy, parentwin, &wa)) {
        die("spmenu: could not get embedding window attributes: 0x%lx", parentwin);
    }

    xinitvisual(); // init visual and create drawable after
    drw = drw_create_x11(dpy, screen, root, wa.width, wa.height, visual, depth, cmap, protocol);
}

void cleanup_x11(Display *disp) {
    XUngrabKey(disp, AnyKey, AnyModifier, root);
    XSync(disp, False);
    XCloseDisplay(disp);
}
