/* See LICENSE file for copyright and license details. */

void grabfocus_x11(void) {
    struct timespec ts = { .tv_sec = 0, .tv_nsec = 10000000  };
    Window focuswin;
    int i, revertwin;
    XWindowAttributes wa;

    XSync(dpy, False);
    XGetWindowAttributes(dpy, win, &wa);

    for (i = 0; i < 100; ++i) {
        XGetInputFocus(dpy, &focuswin, &revertwin);
        if (focuswin == win)
            return;

        // if it's a client, we can't just steal all the input for ourselves
        if (managed) {
            if (wa.map_state == IsViewable) {
                XTextProperty prop;
                char *windowtitle = prompt != NULL ? prompt : "spmenu";
                Xutf8TextListToTextProperty(dpy, &windowtitle, 1, XUTF8StringStyle, &prop);
                XSetWMName(dpy, win, &prop);
                XSetTextProperty(dpy, win, &prop, XInternAtom(dpy, "_NET_WM_NAME", False));
                XFree(prop.value);
            }
        } else { // spmenu is not managed, and is very greedy
            if (wa.map_state == IsViewable) // it must be viewable first, otherwise we get a BadMatch error
                XSetInputFocus(dpy, win, RevertToParent, CurrentTime);
        }

        nanosleep(&ts, NULL);
    }

    die("spmenu: cannot grab focus"); // not possible to grab focus, abort immediately
}
