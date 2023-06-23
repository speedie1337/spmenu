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
    sp.promptw = (prompt && *prompt)
        ? pango_prompt ? TEXTWM(prompt) : TEXTW(prompt) - sp.lrpad / 4 : 0; // prompt width

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
            sp.mw = MIN(MAX(max_textw() + sp.promptw, minwidth), info[i].width);
            x = info[i].x_org + xpos + ((info[i].width  - sp.mw) / 2);
            y = info[i].y_org - ypos + ((info[i].height - sp.mh) / 2);
        } else { // top or bottom
            x = info[i].x_org + xpos;
            y = info[i].y_org + (menuposition ? 0 : info[i].height - sp.mh - ypos);
            sp.mw = (menuwidth > 0 ? menuwidth : info[i].width);
        }

        XFree(info);
    } else
#endif
    {
        if (!XGetWindowAttributes(dpy, parentwin, &wa))
            die("spmenu: could not get embedding window attributes: 0x%lx",
                    parentwin); // die because unable to get attributes for the parent window

        if (menuposition == 2) { // centered
            sp.mw = MIN(MAX(max_textw() + sp.promptw, minwidth), wa.width);
            x = (wa.width  - sp.mw) / 2 + xpos;
            y = (wa.height - sp.mh) / 2 - ypos;
        } else { // top or bottom
            x = 0;
            y = menuposition ? 0 : wa.width - sp.mh - ypos;
            sp.mw = (menuwidth > 0 ? menuwidth : wa.width);
        }
    }

    // create menu window and set properties for it
    create_window_x11(
            x + sp.sp,
            y + sp.vp - (menuposition == 1 ? 0 : menuposition == 2 ? borderwidth : borderwidth * 2),
            sp.mw - 2 * sp.sp - borderwidth * 2,
            sp.mh
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
    if (x11.embed) {
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
    draw_resize(draw, sp.mw - 2 * sp.sp - borderwidth * 2, sp.mh);

    match();
    drawmenu();
}

void prepare_window_size_x11(void) {
    sp.sp = menupaddingh;
    sp.vp = (menuposition == 1) ? menupaddingv : - menupaddingv;

    sp.bh = MAX(draw->font->h, draw->font->h + 2 + lineheight);
    lines = MAX(lines, 0);
#if USEIMAGE
    img.setlines = lines;
#endif

    sp.lrpad = draw->font->h + textpadding;
    get_mh();

    return;
}

Display * opendisplay_x11(char *disp) {
    return XOpenDisplay(disp);
}

void set_screen_x11(Display *disp) {
    x11.screen = DefaultScreen(disp);
    root = RootWindow(disp, x11.screen);
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
    if (!x11.embed || !(parentwin = strtol(x11.embed, NULL, 0)))
        parentwin = root;

    if (!XGetWindowAttributes(dpy, parentwin, &wa)) {
        die("spmenu: could not get embedding window attributes: 0x%lx", parentwin);
    }

    xinitvisual(); // init visual and create drawable after
    draw = draw_create_x11(dpy, x11.screen, root, wa.width, wa.height, x11.visual, x11.depth, x11.cmap, protocol);
}

void cleanup_x11(Display *disp) {
    XUngrabKey(disp, AnyKey, AnyModifier, root);
    XSync(disp, False);
    XCloseDisplay(disp);
}
