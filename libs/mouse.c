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
    for (item = items; item && item->text; item++)
        itemCount++;

    // to prevent slowdown, arbritary limit of 50 items
    if (itemCount > 50)
        return;

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
    int xpad = 0;

    if (!hidepowerline) {
        x = xpad = plw;
    }

    // if incorrect or wrong window, return
	if (ev->window != win)
		return;

	// right-click: exit
	if (ev->button == Button3)
		exit(1);

	if (prompt && *prompt)
		x += promptw + plw;

	// input field
	w = (lines > 0 || !matches) ? mw - x : inputw;

	// left-click on input: clear input
	if (ev->button == Button1 &&
	   ((lines <= 0 && ev->x >= 0 && ev->x <= x + w +
	   ((!prev || !curr->left) ? TEXTW(leftarrow) : 0)) ||
	   (lines > 0 && ev->y >= y && ev->y <= y + h))) {
		insert(NULL, -cursor);
		drawmenu();
		return;
	}
	// middle-mouse click: paste selection
	if (ev->button == Button2) {
		XConvertSelection(dpy, (ev->state & ShiftMask) ? clip : XA_PRIMARY,
		                  utf8, utf8, win, CurrentTime);
		drawmenu();
		return;
	}
	// scroll up
	if (ev->button == Button4 && prev) {
		sel = curr = prev;
		calcoffsets();
		drawmenu();
		return;
	}
	// scroll down
	if (ev->button == Button5 && next) {
		sel = curr = next;
		calcoffsets();
		drawmenu();
		return;
	}
	if (ev->button != Button1)
		return;
	if (ev->state & ~ControlMask)
		return;
	if (lines > 0) {
		// vertical list: (ctrl)left-click on item
		w = mw - x;
		for (item = curr; item != next; item = item->right) {
			if (item_num++ == lines){
				item_num = 1;
				x += w / columns;
				y = 0;
			}
			y += h;
			if (ev->y >= y && ev->y <= (y + h) &&
			    ev->x >= x && ev->x <= (x + w / columns)) {
				puts(item->text);
				if (!(ev->state & ControlMask))
					exit(0);
				sel = item;
				if (sel) {
					drawmenu();
				}
				return;
			}
		}
	} else if (matches) {
		// left-click on left arrow
		x += inputw;
		w = TEXTW(leftarrow);
		if (prev && curr->left) {
			if (ev->x >= x && ev->x <= x + w) {
				sel = curr = prev;
				calcoffsets();
				drawmenu();
				return;
			}
		}
		// horizontal list: (ctrl)left-click on item
		for (item = curr; item != next; item = item->right) {
			x += w;
			w = MIN(TEXTW(item->text), mw - x - TEXTW(rightarrow));
			if (ev->x >= x && ev->x <= x + w) {
				puts(item->text);
				if (!(ev->state & ControlMask))
					exit(0);
				sel = item;
				if (sel) {
					drawmenu();
				}
				return;
			}
		}
		// left-click on right arrow
		w = TEXTW(rightarrow);
		x = mw - w;
		if (next && ev->x >= x && ev->x <= x + w) {
			sel = curr = next;
			calcoffsets();
			drawmenu();
			return;
		}
	}
}
