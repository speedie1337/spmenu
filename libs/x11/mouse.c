/* See LICENSE file for copyright and license details. */

void buttonpress(XEvent *e) {
    struct item *item;
    XButtonPressedEvent *ev = &e->xbutton;
    int x = 0, y = 0, h = bh, w, item_num = 0;
    unsigned int i, click;
    int xpad = 0;

    if (!hidepowerline) {
        x = xpad = plw;
    }

    // margin
    x += menumarginh;

    int larrowWidth = 0;
    int rarrowWidth = 0;
    int numberWidth = 0;
    int modeWidth = 0;
    int capsWidth = 0;

    if (!hidelarrow) larrowWidth = pango_leftarrow ? TEXTWM(leftarrow) : TEXTW(leftarrow);
    if (!hiderarrow) rarrowWidth = pango_rightarrow ? TEXTWM(rightarrow) : TEXTW(rightarrow);
    if (!hidematchcount) numberWidth = pango_numbers ? TEXTWM(numbers) : TEXTW(numbers);
    if (!hidemode) modeWidth = pango_mode ? TEXTWM(modetext) : TEXTW(modetext);
    if (!hidecaps) capsWidth = pango_caps ? TEXTWM(capstext) : TEXTW(capstext);

    if (!strcmp(capstext, "")) capsWidth = 0; // no caps lock width for no chars
    if (ev->window != win) return; // if incorrect or wrong window, return

    click = ClickWindow; // clicking anywhere, we use this and override it if we clicked on something specific

    // check click position and override the value of click
    if (ev->x < x + promptw + powerlineprompt ? plw : 0) { // prompt
        click = ClickPrompt;
    } else if ((ev->x > mw - capsWidth - 2 * sp - 2 * borderwidth - menumarginh) && !hidecaps && capsWidth) { // caps lock indicator
        click = ClickCaps;
    } else if (ev->x > mw - modeWidth - capsWidth - 2 * sp - 2 * borderwidth - menumarginh) { // mode indicator
        click = ClickMode;
    } else if (ev->x > mw - modeWidth - numberWidth - capsWidth - 2 * sp - 2 * borderwidth - menumarginh) { // match count
        click = ClickNumber;
    } else { // input
        w = (lines > 0 || !matches) ? mw - x : inputw;

        if ((lines <= 0 && ev->x >= 0 && ev->x <= x + w + promptw +
                    ((!prev || !curr->left) ? larrowWidth : 0)) ||
                (lines > 0 && ev->y >= y && ev->y <= y + h)) {

            click = ClickInput;
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

            // ClickSelItem, called function doesn't matter
            if (ev->y >= y && ev->y <= (y + h) && ev->x >= x && ev->x <= (x + w / columns)) {
                for (i = 0; i < LENGTH(buttons); i++) {
                    if (ignoreglobalmouse) break;
                    if (buttons[i].click == ClickSelItem && buttons[i].button == ev->button && CLEANMASK(buttons[i].mask) == CLEANMASK(ev->state)) {
                        puts(item->text);
                        exit(0);
                    } else if (buttons[i].click == ClickItem) {
                        click = ClickItem;
                    }
                }
                for (i = 0; i < LENGTH(cbuttons); i++) {
                    if (ignoreconfmouse) break;
                    if (cbuttons[i].click == ClickSelItem && cbuttons[i].button == ev->button && CLEANMASK(cbuttons[i].mask) == CLEANMASK(ev->state)) {
                        puts(item->text);
                        exit(0);
                    } else if (cbuttons[i].click == ClickItem) {
                        click = ClickItem;
                    }
                }
            }
        }
    } else if (matches) { // a single line, meaning it could be arrows too, so we check that here
                          // left arrow
        x += inputw;
        w = larrowWidth;
        if (prev && curr->left) {
            if (ev->x >= x && ev->x <= x + w) {
                click = ClickLArrow;
            }
        }
        // item click
        // horizontal list
        for (item = curr; item != next; item = item->right) {
            x += w;
            w = MIN(TEXTW(item->text), mw - x - rarrowWidth);
            if (ev->x >= x && ev->x <= x + w) {
                for (i = 0; i < LENGTH(buttons); i++) {
                    if (ignoreglobalmouse) break;
                    if (buttons[i].click == ClickSelItem && buttons[i].button == ev->button && CLEANMASK(buttons[i].mask) == CLEANMASK(ev->state)) {
                        puts(item->text);
                        exit(0);
                    } else if (buttons[i].click == ClickItem) {
                        click = ClickItem;
                    }
                }
                for (i = 0; i < LENGTH(cbuttons); i++) {
                    if (ignoreconfmouse) break;
                    if (cbuttons[i].click == ClickSelItem && cbuttons[i].button == ev->button && CLEANMASK(cbuttons[i].mask) == CLEANMASK(ev->state)) {
                        puts(item->text);
                        exit(0);
                    } else if (cbuttons[i].click == ClickItem) {
                        click = ClickItem;
                    }
                }

            }
        }
        // right arrow
        w = rarrowWidth;
        x = mw - w;
        if (next && ev->x >= x && ev->x <= x + w) {
            click = ClickRArrow;
        }
    }

    // go through mouse button array and run function
    for (i = 0; i < LENGTH(buttons); i++) {
        if (ignoreglobalmouse) break;
        if (click == buttons[i].click && buttons[i].func && buttons[i].button == ev->button
                && CLEANMASK(buttons[i].mask) == CLEANMASK(ev->state))
            buttons[i].func(&buttons[i].arg);
    }

    // go through mouse config array and run function
    for (i = 0; i < LENGTH(cbuttons); i++) {
        if (ignoreconfmouse) break;
        if (click == cbuttons[i].click && cbuttons[i].func && cbuttons[i].button == ev->button
                && CLEANMASK(cbuttons[i].mask) == CLEANMASK(ev->state))
            cbuttons[i].func(&cbuttons[i].arg);
    }
}
