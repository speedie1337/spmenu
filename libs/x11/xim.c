void open_xim(void) {
    XIM xim;

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
}
