void
drawhighlights(struct item *item, int x, int y, int w)
{
	int i, indent;
	char *highlight;
	char c;

    if (columns > 5)
        return;

	char *itemtext = item->text;

	if (!(strlen(itemtext) && strlen(text)))
		return;

	drw_setscheme(drw, scheme[item == sel
	                   ? SchemeSelHighlight
	                   : SchemeNormHighlight]);
	for (i = 0, highlight = itemtext; *highlight && text[i];) {
        if (((fuzzy && !fstrncmp(&(*highlight), &text[i], 1)) || (!fuzzy && *highlight == text[i]))) {
			c = *highlight;
			*highlight = '\0';
			indent = TEXTW(itemtext) - lrpad;
			*highlight = c;

			// highlight character
			c = highlight[1];
			highlight[1] = '\0';
			drw_text(
				drw,
				x + indent + (lrpad / 2),
				y,
				MIN(w - indent - lrpad, TEXTW(highlight) - lrpad),
				bh, 0, highlight, 0, pango_highlight ? True : False);
			highlight[1] = c;
			i++;
		}
		highlight++;
	}
}

int
drawitem(struct item *item, int x, int y, int w)
{
    char buffer[MAXITEMLENGTH];
    Clr scm[3];
    int lp = lrpad / 2; // padding
    int wr, rd;
	int rw = 0; // width of text
    int orw = 0;
	int fg = 7;
	int bg = 0;
    int bgfg = 0;
    int ignore = 0;
    int ib = 0;

    if (item == sel) {
        memcpy(scm, scheme[SchemeItemSel], sizeof(scm));

        if (item->hp)
            memcpy(scm, scheme[SchemeItemSelPri], sizeof(scm));
    } else {
        memcpy(scm, scheme[SchemeItemNorm], sizeof(scm));

        if (item->hp)
            memcpy(scm, scheme[SchemeItemNormPri], sizeof(scm));
    }

    drw_setscheme(drw, scm); // set scheme

    // parse item text
	for (wr = 0, rd = 0; item->text[rd]; rd++) {
		if (item->text[rd] == '' && item->text[rd + 1] == '[') {
			size_t alen = strspn(item->text + rd + 2,
					     "0123456789;");
			if (item->text[rd + alen + 2] == 'm') { // last character in sequence is always 'm'
				buffer[wr] = '\0';

                apply_fribidi(buffer);
                rw = MIN(w, TEXTW(buffer) - lrpad);
				drw_text(drw, x, y, rw + lp, bh, lp, isrtl ? fribidi_text : buffer, 0, pango_item ? True : False);

				x += rw + lp;
                orw += rw; // width of all colored text, we add this to the full width later
                ib = 1;
                lp = 0;

				char *ep = item->text + rd + 1;

                // parse hex colors in scm
				while (*ep != 'm') {
					unsigned v = strtoul(ep + 1, &ep, 10);
                    if (ignore)
						continue;
					if (bgfg) {
						if (bgfg < 4 && v == 5) {
							bgfg <<= 1;
							continue;
						}
						if (bgfg == 4)
							scm[0] = textclrs[fg = v];
						else if (bgfg == 6)
							scm[1] = textclrs[bg = v];
						ignore = 1;

						continue;
					}

					if (v == 1) {
						fg |= 8;
						scm[0] = textclrs[fg];
					} else if (v == 22) {
						fg &= ~8;
						scm[0] = textclrs[fg];
					} else if (v >= 30 && v <= 37) {
						fg = v % 10 | (fg & 8);
						scm[0] = textclrs[fg];
                    } else if (v == 38) {
						bgfg = 2;
					} else if (v >= 40 && v <= 47) {
						bg = v % 10;
						scm[1] = textclrs[bg];
					} else if (v == 48) {
                        bgfg = 3;
                    }
				}

				rd += alen + 2;
				wr = 0;

				drw_setscheme(drw, scm); // set scheme

				continue;
			}
		}

		buffer[wr++] = item->text[rd];
	}

	buffer[wr] = '\0';

    w -= orw;

    // now draw any non-colored text
    apply_fribidi(buffer);
	int r = drw_text(drw, x, y, w, bh, lp, isrtl ? fribidi_text : buffer, 0, pango_item ? True : False);
    if (!hidehighlight && !ib) drawhighlights(item, x, y, w);

    return r;
}

void
drawmenu(void)
{
	unsigned int curpos = 0;
	struct item *item;
	int x = 0, y = 0, fh = drw->font->h, w;
    #if USEIMAGE
    int ox = 0; // original x position
    #endif
	char *censort; // censor text (password)
    plw = hidepowerline ? 0 : drw->font->h / 2 + 1; // powerline size
    Clr *prevscheme, *nextscheme;

    // draw menu first using menu scheme
	drw_setscheme(drw, scheme[SchemeMenu]);
	drw_rect(drw, 0, 0, mw, mh, 1, 1);

    int numberWidth = 0;
    int modeWidth = 0;
    int larrowWidth = 0;
    int rarrowWidth = 0;

    // add width
    if (!hidemode) modeWidth = pango_mode ? TEXTWM(modetext) : TEXTW(modetext);
    if (!hidelarrow) larrowWidth = pango_leftarrow ? TEXTWM(leftarrow) : TEXTW(leftarrow);
    if (!hiderarrow) rarrowWidth = pango_rightarrow ? TEXTWM(rightarrow) : TEXTW(rightarrow);

	if (prompt && *prompt) {
        prevscheme = scheme[SchemePrompt];
		drw_setscheme(drw, scheme[SchemePrompt]);

        #if USEIMAGE
        ox = x;
        #endif
		x = drw_text(drw, x, 0, promptw, bh, lrpad / 2, prompt, 0, pango_prompt ? True : False);

        drw_settrans(drw, scheme[SchemePrompt], scheme[SchemeMenu]);
        drw_arrow(drw, x, 0, plw, bh, 1, 0);

        x += plw;
	}

    // draw input
	w = (lines > 0 || !matches) ? mw - x : inputw;
	drw_setscheme(drw, scheme[SchemeInput]);
	if (passwd && !hideprompt) {
	    censort = ecalloc(pango_input ? TEXTWM(password) : TEXTW(password), sizeof(text));
		memset(censort, *password, strlen(text));

        apply_fribidi(censort);
		drw_text(drw, x, 0, w, bh, lrpad / 2, isrtl ? fribidi_text : censort, 0, pango_password ? True : False);

	    curpos = TEXTW(censort) - TEXTW(&text[cursor]);

        free(censort);
	} else if (!passwd && !hideprompt) {
        apply_fribidi(text);
        drw_text(drw, x, 0, w, bh, lrpad / 2, isrtl ? fribidi_text : text, 0, pango_input ? True : False);

	    curpos = TEXTW(text) - TEXTW(&text[cursor]);
    }

	if ((curpos += lrpad / 2 - 1) < w && !hidecursor && !hideprompt) {
		drw_setscheme(drw, scheme[SchemeCaret]);
		drw_rect(drw, x + curpos, 2 + (bh - fh) / 2, 2, fh - 4, 1, 0);
	}

    if (!hidematchcount) {
        recalculatenumbers();
        numberWidth = TEXTW(numbers);

        // mode indicator is always going to be at the right
        if (hidemode) {
            numberWidth += 2 * sp + borderwidth;
        } else {
            modeWidth += 2 * sp + borderwidth;
        }

    }

    // draw items and image
	if (lines > 0) {
		int i = 0;
        int xpad = 0;

        // indent items to prompt width?
        if (indentitems) {
            xpad = 0; // x -= 0
        } else {
            xpad = promptw; // x -= prompt width so no indentation to prompt width
        }

        // draw image first
        #if USEIMAGE
        if (!hideimage && longestedge != 0) {
            x = ox;
            x += (imagegaps * 2) + imagewidth;
            x += xpad;
        }
        #endif

		for (item = curr; item != next; item = item->right, i++) {
			drawitem(
				item,
				x + ((i / lines) *  ((mw - x) / columns)) - xpad,
				y + (((i % lines) + 1) * bh),
				(mw - x) / columns
			);
        }

    // horizontal list
	} else if (matches) {
		x += inputw;
		w = larrowWidth;

		if (curr->left && !hidelarrow) { // draw left arrow
			drw_setscheme(drw, scheme[SchemeLArrow]);
			drw_text(drw, x, 0, w, bh, lrpad / 2, leftarrow, 0, pango_leftarrow ? True : False);
		}

		x += w;

		for (item = curr; item != next; item = item->right) // draw items
            x = drawitem(item, x, 0, MIN(pango_item ? TEXTWM(item->text) : TEXTW(item->text), mw - x - rarrowWidth - numberWidth - modeWidth));

		if (next && !hiderarrow) { // draw right arrow
			w = rarrowWidth;
			drw_setscheme(drw, scheme[SchemeRArrow]);

            drw_text(drw, mw - w - numberWidth - modeWidth, 0, w, bh, lrpad / 2, rightarrow, 0, pango_rightarrow ? True : False);
		}
	}

    if (!hidematchcount) { // draw match count
        prevscheme = scheme[SchemeNumber];
        drw_setscheme(drw, scheme[SchemeNumber]);

        drw_text(drw, mw - numberWidth - modeWidth, 0, numberWidth, bh, lrpad / 2 + plw / 2, numbers, 0, pango_numbers ? True : False);

        // draw powerline for match count
        if (!hidepowerline) {
            drw_settrans(drw, scheme[SchemeNumber], scheme[SchemeMenu]);
            drw_arrow(drw, mw - numberWidth - modeWidth, 0, plw, bh, 0, 0);

            x += plw;
        }
    }

    if (!hidemode) { // draw mode indicator
        prevscheme = scheme[SchemeMode];
        drw_setscheme(drw, scheme[SchemeMode]);

        drw_text(drw, mw - modeWidth, 0, modeWidth, bh, lrpad / 2 + plw / 2, modetext, 0, pango_mode ? True : False);

        // draw powerline for match count
        if (!hidepowerline) {
            drw_settrans(drw, scheme[SchemeMode], hidematchcount ? scheme[SchemeMenu] : scheme[SchemeNumber]);
            drw_arrow(drw, mw - modeWidth, 0, plw, bh, 0, 0);

            x += plw;
        }
    }

    // map the drawing
	drw_map(drw, win, 0, 0, mw, mh);
}
