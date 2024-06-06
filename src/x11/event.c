/* See LICENSE file for copyright and license details. */
void eventloop_x11(void) {
    XEvent ev;

    while (!XNextEvent(dpy, &ev)) {
        if (XFilterEvent(&ev, None))
            continue;

        switch(ev.type) {
            case DestroyNotify:
                if (ev.xdestroywindow.window != win)
                    break;
                cleanup();
                exit(1);
            case ButtonPress:
                buttonpress_x11(&ev);
                break;
            case MotionNotify: // currently does nothing
                break;
            case Expose:
                if (ev.xexpose.count == 0)
                    draw_map(draw, win, 0, 0, sp.mw, sp.mh);
                break;
            case FocusIn:
                // regrab focus from parent window
                if (ev.xfocus.window != win)
                    grabfocus_x11();
                break;
            case KeyPress: // read key array and call functions
                keypress_x11(&ev);
                break;
            case SelectionNotify: // paste selection
                if (ev.xselection.property == utf8)
                    pastesel_x11();
                break;
            case VisibilityNotify:
                if (ev.xvisibility.state != VisibilityUnobscured)
                    XRaiseWindow(dpy, win);
                break;
            case KeyRelease:
                getcapsstate();
                drawmenu();
                break;
        }
    }
}
