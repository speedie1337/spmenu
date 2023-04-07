unsigned int
textw_clamp(const char *str, unsigned int n)
{
	unsigned int w;
    w = drw_fontset_getwidth_clamp(drw, str, n, True) + lrpad;

	return MIN(w, n);
}

void
motionevent(XButtonEvent *ev)
{
	struct item *item;
	int xy, ev_xy;

	if (ev->window != win || matches == 0)
		return;

    #if USEIMAGE
    if (image) return;
    #endif

    int itemCount = 0;

    // walk through all items
    for (item = items; item && item->text; item++) {
        itemCount++;
    }

    // to prevent slowdown, arbritary limit of 50 items
    if (itemCount > 50) {
        return;
    }

    int larrowWidth = 0;
    int rarrowWidth = 0;

    if (!hidelarrow) larrowWidth = pango_leftarrow ? TEXTWM(leftarrow) : TEXTW(leftarrow);
    if (!hiderarrow) rarrowWidth = pango_rightarrow ? TEXTWM(rightarrow) : TEXTW(rightarrow);

	xy = lines > 0 ? bh : inputw + promptw + larrowWidth;
	ev_xy = lines > 0 ? ev->y : ev->x;
	for (item = curr; item && item != next; item = item->right) {
		int wh = lines > 0 ? bh : textw_clamp(item->text, mw - xy - rarrowWidth);
		if (ev_xy >= xy && ev_xy < (xy + wh)) {
			sel = item;
			calcoffsets();
			drawmenu();
			break;
		}
		xy += wh;
	}
}

void
buttonpress(XEvent *e)
{
	struct item *item;
	XButtonPressedEvent *ev = &e->xbutton;
	int x = 0, y = 0, h = bh, w, item_num = 0;
    unsigned int i, click;
    int xpad = 0;

    if (!hidepowerline) {
        x = xpad = plw;
    }

    int larrowWidth = 0;
    int rarrowWidth = 0;
    int numberWidth = 0;
    int modeWidth = 0;

    if (!hidelarrow) larrowWidth = pango_leftarrow ? TEXTWM(leftarrow) : TEXTW(leftarrow);
    if (!hiderarrow) rarrowWidth = pango_rightarrow ? TEXTWM(rightarrow) : TEXTW(rightarrow);
    if (!hidematchcount) numberWidth = pango_numbers ? TEXTWM(numbers) : TEXTW(numbers);
    if (!hidemode) modeWidth = pango_mode ? TEXTWM(modetext) : TEXTW(modetext);

    // if incorrect or wrong window, return
	if (ev->window != win)
		return;

    // clicking anywhere, we use this as a base for a click
    click = clickwindow;

    // check if we clicked on the prompt or the input
    if (ev->x < x + promptw + powerlineprompt ? plw : 0) {
        click = clickprompt;
    } else if (ev->x > mw - modeWidth - 2 * sp - 2 * borderwidth) {
        click = clickmode;
    } else if (ev->x > mw - modeWidth - numberWidth - 2 * sp - 2 * borderwidth) {
        click = clicknumber;
    } else { // actually we clicked on the input
        w = (lines > 0 || !matches) ? mw - x : inputw;

        if ((lines <= 0 && ev->x >= 0 && ev->x <= x + w + promptw +
           ((!prev || !curr->left) ? larrowWidth : 0)) ||
           (lines > 0 && ev->y >= y && ev->y <= y + h)) {

            click = clickinput;
        }
    }

    // item click
	if (lines > 0) {
		// vertical list
		w = mw - x;
		for (item = curr; item != next; item = item->right) {
			if (item_num++ == lines) {
				item_num = 1;
				x += w / columns;
				y = 0;
			}

			y += h;

			if (ev->y >= y && ev->y <= (y + h) && ev->x >= x && ev->x <= (x + w / columns)) {
                for (i = 0; i < LENGTH(buttons); i++) {
                    if (buttons[i].click == clickselitem && buttons[i].button == ev->button && CLEANMASK(buttons[i].mask) == CLEANMASK(ev->state)) {
                        puts(item->text);
                        exit(0);
                    } else if (buttons[i].click == clickitem) {
                        click = clickitem;
                    }
                }
			}
		}
	} else if (matches) {
		// left-click on left arrow
		x += inputw;
		w = larrowWidth;
		if (prev && curr->left) {
			if (ev->x >= x && ev->x <= x + w) {
                click = clicklarrow;
			}
		}
        // item click
		// horizontal list
		for (item = curr; item != next; item = item->right) {
			x += w;
			w = MIN(TEXTW(item->text), mw - x - rarrowWidth);
			if (ev->x >= x && ev->x <= x + w) {
                for (i = 0; i < LENGTH(buttons); i++) {
                    if (buttons[i].click == clickselitem && buttons[i].button == ev->button && CLEANMASK(buttons[i].mask) == CLEANMASK(ev->state)) {
                        puts(item->text);
                        exit(0);
                    } else if (buttons[i].click == clickitem) {
                        click = clickitem;
                    }
                }

			}
		}
		// left-click on right arrow
		w = rarrowWidth;
		x = mw - w;
		if (next && ev->x >= x && ev->x <= x + w) {
            click = clickrarrow;
		}
    }

    // go through mouse button array
    for (i = 0; i < LENGTH(buttons); i++)
        if (click == buttons[i].click && buttons[i].func && buttons[i].button == ev->button
        && CLEANMASK(buttons[i].mask) == CLEANMASK(ev->state))
            buttons[i].func(&buttons[i].arg);
}
