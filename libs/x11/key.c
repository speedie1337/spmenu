/* See LICENSE file for copyright and license details. */

void updatenumlockmask(void) {
    unsigned int i, j;
    XModifierKeymap *modmap;

    x11.numlockmask = 0;
    modmap = XGetModifierMapping(dpy);
    for (i = 0; i < 8; i++)
        for (j = 0; j < modmap->max_keypermod; j++)
            if (modmap->modifiermap[i * modmap->max_keypermod + j]
                    == XKeysymToKeycode(dpy, XK_Num_Lock))
                x11.numlockmask = (1 << i);
    XFreeModifiermap(modmap);
}

void keypress_x11(XEvent *e) {
    updatenumlockmask();
    {
        unsigned int i;
        KeySym keysym = NoSymbol;
        KeySym keysym_case = NoSymbol;
        XKeyEvent *ev;
        char buf[64];
        KeySym ksym = NoSymbol;
        Status status;

        int len = 0;
        ev = &e->xkey;
        len = XmbLookupString(xic, ev, buf, sizeof buf, &ksym, &status);

        // keysym = XkbKeycodeToKeysym(dpy, (KeyCode)ev->keycode, 0, 0);
        XConvertCase(ksym, &keysym, &keysym_case);

        // this makes sure we always have a way to exit if we unbind our quit key
        if (keysym == hkeys[0].keysym && CLEANMASK(hkeys[0].mod) == CLEANMASK(ev->state) && hkeys[0].func) hkeys[0].func(&(hkeys[0].arg));

        for (i = 0; i < LENGTH(keys); i++) {
            if (sp.ignoreglobalkeys) break;
            if (keysym == keys[i].keysym && CLEANMASK(keys[i].mod) == CLEANMASK(ev->state) && keys[i].func) {
                if ((keys[i].mode && sp.mode) || keys[i].mode == -1) {
                    keys[i].func(&(keys[i].arg));
                    return;
                } else if (!keys[i].mode && !sp.mode) {
                    keys[i].func(&(keys[i].arg));
                } else {
                    continue;
                }
            }
        }

        for (i = 0; i < LENGTH(ckeys); i++) {
            if (sp.ignoreconfkeys) break;
            if (keysym == ckeys[i].keysym && CLEANMASK(ckeys[i].mod) == CLEANMASK(ev->state) && ckeys[i].func) {
                if ((ckeys[i].mode && sp.mode) || ckeys[i].mode == -1) {
                    ckeys[i].func(&(ckeys[i].arg));
                    return;
                } else if (!ckeys[i].mode && !sp.mode) {
                    ckeys[i].func(&(ckeys[i].arg));
                } else {
                    continue;
                }
            }
        }

        if (!iscntrl(*buf) && type && sp.mode ) {
            if (sp.allowkeys) {
                insert(buf, len);
            } else {
                sp.allowkeys = !sp.allowkeys;
            }

            drawmenu();
        }
    }
}

void grabkeyboard_x11(void) {
    struct timespec ts = { .tv_sec = 0, .tv_nsec = 1000000  };
    int i;

    // don't grab if embedded
    if (x11.embed || managed)
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

void getcapsstate(void) {
    unsigned int cs = 0;

    XkbGetIndicatorState(dpy, XkbUseCoreKbd, &cs);
    sp.capslockstate = (cs & 0x01) == 1;

    strncpy(tx.capstext, sp.capslockstate ? capslockontext : capslockofftext, 15);
}
