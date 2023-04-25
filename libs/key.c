void
updatenumlockmask(void)
{
	unsigned int i, j;
	XModifierKeymap *modmap;

	numlockmask = 0;
	modmap = XGetModifierMapping(dpy);
	for (i = 0; i < 8; i++)
		for (j = 0; j < modmap->max_keypermod; j++)
			if (modmap->modifiermap[i * modmap->max_keypermod + j]
				== XKeysymToKeycode(dpy, XK_Num_Lock))
				numlockmask = (1 << i);
	XFreeModifiermap(modmap);
}

void
keypress(XEvent *e)
{
   	updatenumlockmask();
    {
        unsigned int i;
        KeySym keysym;
        XKeyEvent *ev;
        char buf[64];
        KeySym ksym = NoSymbol;
        Status status;

        int len = 0;
        ev = &e->xkey;
        len = XmbLookupString(xic, ev, buf, sizeof buf, &ksym, &status);

        keysym = XkbKeycodeToKeysym(dpy, (KeyCode)ev->keycode, 0, 0);

        for (i = 0; i < LENGTH(keys); i++) {
            if (ignoreglobalkeys) break;
            if (keysym == keys[i].keysym && CLEANMASK(keys[i].mod) == CLEANMASK(ev->state) && keys[i].func) {
                if ((keys[i].mode && curMode) || keys[i].mode == -1) {
                    keys[i].func(&(keys[i].arg));
                    return;
                } else if (!keys[i].mode && !curMode) {
                    keys[i].func(&(keys[i].arg));
                } else {
                    continue;
                }
            }
        }

        for (i = 0; i < LENGTH(ckeys); i++) {
            if (ignoreconfkeys) break;
            if (keysym == ckeys[i].keysym && CLEANMASK(ckeys[i].mod) == CLEANMASK(ev->state) && ckeys[i].func) {
                if ((ckeys[i].mode && curMode) || ckeys[i].mode == -1) {
                    ckeys[i].func(&(ckeys[i].arg));
                    return;
                } else if (!ckeys[i].mode && !curMode) {
                    ckeys[i].func(&(ckeys[i].arg));
                } else {
                    continue;
                }
            }
        }

        if (!iscntrl(*buf) && type && curMode ) {
            if (allowkeys) {
                insert(buf, len);
            } else {
                allowkeys = !allowkeys;
            }

            drawmenu();
        }
    }
}

void
grabkeyboard(void)
{
	struct timespec ts = { .tv_sec = 0, .tv_nsec = 1000000  };
	int i;

    // don't grab if embedded
	if (embed || managed)
		return;
	// try to grab keyboard, we may have to wait for another process to ungrab
	for (i = 0; i < 1000; i++) {
		if (XGrabKeyboard(dpy, DefaultRootWindow(dpy), True, GrabModeAsync,
		                  GrabModeAsync, CurrentTime) == GrabSuccess) {
            getcapsstate();
			return;
        }
		nanosleep(&ts, NULL);
	}
	die("cannot grab keyboard");
}

void
getcapsstate(void)
{
    unsigned int cs = 0;

    XkbGetIndicatorState(dpy, XkbUseCoreKbd, &cs);
    capslockstate = (cs & 0x01) == 1;

    strncpy(capstext, capslockstate ? capslockontext : capslockofftext, 15);
}
