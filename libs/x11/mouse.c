/* See LICENSE file for copyright and license details. */

void buttonpress_x11(XEvent *e) {
    struct item *item;
    XButtonPressedEvent *ev = &e->xbutton;
    int x = 0, y = 0, h = sp.bh, w, item_num = 0;
    unsigned int i, click;
    int yp = 0;

    // margin
    x += menumarginh;

    int larroww = 0;
    int rarroww = 0;
    int numberw = 0;
    int modew = 0;
    int capsw = 0;

    if (!hidelarrow) larroww = pango_leftarrow ? TEXTWM(leftarrow) : TEXTW(leftarrow);
    if (!hiderarrow) rarroww = pango_rightarrow ? TEXTWM(rightarrow) : TEXTW(rightarrow);
    if (!hidematchcount) numberw = pango_numbers ? TEXTWM(tx.numbers) : TEXTW(tx.numbers);
    if (!hidemode) modew = pango_mode ? TEXTWM(tx.modetext) : TEXTW(tx.modetext);
    if (!hidecaps) capsw = pango_caps ? TEXTWM(tx.capstext) : TEXTW(tx.capstext);

    if (!strcmp(tx.capstext, ""))
        capsw = 0;

    if ((hideprompt && hideinput && hidemode && hidematchcount && hidecaps) && lines) {
        yp = 1;
    } else if (lines && ev->y < h + menumarginv && ev->y > menumarginv) {
        yp = 1;
    } else if (!lines) {
        yp = 1;
    }

    if (ev->window != win) return; // if incorrect or wrong window, return

    click = ClickWindow; // clicking anywhere, we use this and override it if we clicked on something specific

    // check click position and override the value of click
    if (yp && ev->x < x + sp.promptw + powerlineprompt ? sp.plw : 0) { // prompt
        click = ClickPrompt;
    } else if (yp && (ev->x > sp.mw - capsw - 2 * sp.sp - 2 * borderwidth - menumarginh) && !hidecaps && capsw) { // caps lock indicator
        click = ClickCaps;
    } else if (yp && ev->x > sp.mw - modew - capsw - 2 * sp.sp - 2 * borderwidth - menumarginh) { // mode indicator
        click = ClickMode;
    } else if (yp && ev->x > sp.mw - modew - numberw - capsw - 2 * sp.sp - 2 * borderwidth - menumarginh) { // match count
        click = ClickNumber;
    } else if (yp && !hideinput) { // input
        w = (lines > 0 || !matches) ? sp.mw - x : sp.inputw;

        if ((lines <= 0 && ev->x >= 0 && ev->x <= x + w + sp.promptw +
                    ((!previousitem || !currentitem->left) ? larroww : 0)) ||
                (lines > 0 && ev->y >= y && ev->y <= y + h)) {

            click = ClickInput;
        }
    }

#if USEIMAGE
    if (!hideimage && img.longestedge != 0 && imagetype) {
        x += MAX((img.imagegaps * 2) + img.imagewidth, indentitems ? sp.promptw : 0);
    }
#endif

    // item click
    if (lines > 0) {
        w = sp.mw - x;

        ev->y -= menumarginv;

        if (hideprompt && hideinput && hidemode && hidematchcount && hidecaps) {
            ev->y += h;
        }

        for (item = currentitem; item != nextitem; item = item->right) {
            if (item_num++ == lines) {
                item_num = 1;
                x += w / columns;
                y = 0;
            }

            y += h;

            // ClickSelItem, called function doesn't matter
            if (ev->y >= y && ev->y <= (y + h) && ev->x >= x + (powerlineitems ? sp.plw : 0) && ev->x <= (x + w / columns) + (powerlineitems ? sp.plw : 0)) {
                click = ClickItem;
                mouseitem = item;
#if USEIMAGE
            } else if (ev->y >= y && ev->y <= (y + h) && ev->x >= x + (powerlineitems ? sp.plw : 0) - MAX((img.imagegaps * 2) + img.imagewidth, indentitems ? sp.promptw : 0) && ev->x <= (x - MAX((img.imagegaps * 2) + img.imagewidth, indentitems ? sp.promptw : 0) + w / columns) + (powerlineitems ? sp.plw : 0)) {
                click = ClickImage;
                mouseitem = item;
#endif
            }
        }
    } else if (matches) { // a single line, meaning it could be arrows too, so we check that here
        x += sp.inputw;
        w = larroww;

        if (previousitem && currentitem->left) {
            if (ev->x >= x && ev->x <= x + w) {
                click = ClickLArrow;
            }
        }

        // right arrow
        w = rarroww;
        x = sp.mw - w;
        if (nextitem && ev->x >= x && ev->x <= x + w) {
            click = ClickRArrow;
        }
    }

    // go through mouse button array and run function
    for (i = 0; i < LENGTH(buttons); i++) {
        if (sp.ignoreglobalmouse) break;
        if ((click == buttons[i].click || buttons[i].click == ClickNone) && buttons[i].func && buttons[i].button == ev->button)
            buttons[i].func(&buttons[i].arg);
    }

    // go through mouse config array and run function
    for (i = 0; i < LENGTH(cbuttons); i++) {
        if (sp.ignoreconfmouse) break;
        if ((click == cbuttons[i].click || cbuttons[i].click == ClickNone) && cbuttons[i].func && cbuttons[i].button == ev->button)
            cbuttons[i].func(&cbuttons[i].arg);
    }
}
