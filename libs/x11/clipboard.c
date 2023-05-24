/* See LICENSE file for copyright and license details. */

void pastesel_x11(void) {
    char *p, *q;
    int di;
    unsigned long dl;
    Atom da;

    // we have been given the current selection, now insert it into input
    if (XGetWindowProperty(dpy, win, utf8, 0, (sizeof text / 4) + 1, False,
                utf8, &da, &di, &dl, &dl, (unsigned char **)&p)
            == Success && p) {
        insert(p, (q = strchr(p, '\n')) ? q - p : (ssize_t)strlen(p)); // insert selection
        XFree(p);
    }

    // draw the menu
    drawmenu();
}

int paste_x11(int sel) {
    if (XConvertSelection(dpy, sel ? XA_PRIMARY : clip, utf8, utf8, win, CurrentTime)) {
        return 0;
    } else {
        return 1;
    }
}
