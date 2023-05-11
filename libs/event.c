void eventloop(void) {
    XEvent ev;
    int noimg = 0;

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
                buttonpress(&ev);
                noimg = 0;
                break;
            case MotionNotify: // currently does nothing
                break;
            case Expose:
                if (ev.xexpose.count == 0)
                    drw_map(drw, win, 0, 0, mw, mh);
                break;
            case FocusIn:
                // regrab focus from parent window
                if (ev.xfocus.window != win)
                    grabfocus();
                break;
            case KeyPress: // read key array and call functions
                if (incremental) {
                    puts(text);
                    fflush(stdout);
                }

                keypress(&ev);
                noimg = 1;
                break;
            case SelectionNotify: // paste selection
                if (ev.xselection.property == utf8)
                    pastesel();
                break;
            case VisibilityNotify:
                if (ev.xvisibility.state != VisibilityUnobscured)
                    XRaiseWindow(dpy, win);
                break;
            case KeyRelease:
                getcapsstate();
                drawmenu();
                noimg = 0;
                break;
        }

        if (listfile) {
            readfile();

            if (listchanged) {
                match();

                for (int i = 0; i < itemnumber; i++) {
                    if (sel && sel->right && (sel = sel->right) == next) {
                        curr = next;
                    }
                }

                drawmenu();
            }
        }

        // redraw image on X11 event
        if (!noimg)
#if USEIMAGE
            drawimage();
#else
        ;
#endif
    }
}
