void
eventloop(void)
{
	XEvent ev;
    int noimg = 0;

	while (!XNextEvent(dpy, &ev)) {
		if (XFilterEvent(&ev, win))
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
        case MotionNotify:
            motionevent(&ev.xbutton);
            noimg = 1;
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
		case KeyPress:
			keypress(&ev);
			break;
		case SelectionNotify:
			if (ev.xselection.property == utf8)
				pastesel();
			break;
		case VisibilityNotify:
			if (ev.xvisibility.state != VisibilityUnobscured)
				XRaiseWindow(dpy, win);
			break;
		}

        // redraw image on X11 event
        #if USEIMAGE
        if (!noimg) {
            drawimage();
        } else {
            noimg = 0; // draw it next time
        }
        #endif
	}
}
