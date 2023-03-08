void
drawhighlights(struct item *item, int x, int y, int maxw)
{
	char restorechar, tokens[sizeof text], *highlight,  *token;
	int indentx, highlightlen;
	char *itemtext = item->text;

	drw_setscheme(drw, scheme[item == sel ? SchemeSelHighlight : SchemeNormHighlight]);
	strcpy(tokens, text);

	for (token = strtok(tokens, " "); token; token = strtok(NULL, " ")) {
		highlight = fstrstr(itemtext, token);
		while (highlight) {
			highlightlen = highlight - itemtext;
			restorechar = *highlight;
			itemtext[highlightlen] = '\0';
			indentx = TEXTW(itemtext);
			itemtext[highlightlen] = restorechar;

			restorechar = highlight[strlen(token)];
			highlight[strlen(token)] = '\0';

			if (indentx - (lrpad / 2) - 1 < maxw)
				drw_text(drw, x + indentx - (lrpad / 2) - 1, y, MIN(maxw - indentx, TEXTW(highlight) - lrpad), bh, 0, highlight, 0, pango_highlight ? True : False);

			highlight[strlen(token)] = restorechar;

			if (strlen(highlight) - strlen(token) < strlen(token))
				break;

			highlight = fstrstr(highlight + strlen(token), token);
		}
	}
}

int
drawitem(struct item *item, int x, int y, int w)
{
    char buffer[sizeof(item->text) + lrpad / 2];
    Clr scm[3];
    int lp = lrpad / 2; /* padding */
    int wr, rd;
	int rw = 0; /* width of text */
    int orw = 0;
	int fg = 7;
	int bg = 0;
    int bgfg = 0;
    int ignore = 0;
    int ib = 0;

    if (item == sel) {
        memcpy(scm, scheme[SchemeItemSel], sizeof(scm));
    } else {
        memcpy(scm, scheme[SchemeItemNorm], sizeof(scm));
    }

    /* set scheme */
    drw_setscheme(drw, scm);

    /* parse item text */
	for (wr = 0, rd = 0; item->text[rd]; rd++) {
		if (item->text[rd] == '' && item->text[rd + 1] == '[') {
			size_t alen = strspn(item->text + rd + 2,
					     "0123456789;");
			if (item->text[rd + alen + 2] == 'm') { /* character is 'm' which is the last character in the sequence */
				buffer[wr] = '\0'; /* clear out character */

                apply_fribidi(buffer);
                rw = TEXTW(buffer) - lrpad;
				drw_text(drw, x, y, rw + lp, bh, lp, isrtl ? fribidi_text : buffer, 0, pango_item ? True : False);

				x += rw + lp;
                orw += rw;
                ib = 1;
                lp = 0; /* no padding */

				char *ep = item->text + rd + 1;

                /* parse hex colors in scm */
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

				drw_setscheme(drw, scm);

				continue;
			}
		}

		buffer[wr++] = item->text[rd];
	}

	buffer[wr] = '\0';

    w -= orw;

    /* draw any text that doesn't use sgr sequences */
    apply_fribidi(buffer);
	int r = drw_text(drw, x, y, w, bh, lp, isrtl ? fribidi_text : buffer, 0, pango_item ? True : False);

    if (!hidehighlight && !ib) drawhighlights(item, x, y, w - rw);
    return r;
}

void
drawmenu(void)
{
	unsigned int curpos = 0;
	struct item *item;
	int x = 0, y = 0, fh = drw->font->h, w;
    int ox = 0;
	char *censort;

	drw_setscheme(drw, scheme[SchemeMenu]);
	drw_rect(drw, 0, 0, mw, mh, 1, 1);

    int numberWidth = 0;
    int modeWidth = 0;
    int larrowWidth = 0;
    int rarrowWidth = 0;

    if (!hidemode) modeWidth = pango_mode ? TEXTWM(modetext) : TEXTW(modetext);
    if (!hidelarrow) larrowWidth = pango_leftarrow ? TEXTWM(leftarrow) : TEXTW(leftarrow);
    if (!hiderarrow) rarrowWidth = pango_rightarrow ? TEXTWM(rightarrow) : TEXTW(rightarrow);

	if (prompt && *prompt) {
        if (colorprompt) {
		    drw_setscheme(drw, scheme[SchemePrompt]);
        }

        ox = x;
		x = drw_text(drw, x, 0, promptw, bh, lrpad / 2, prompt, 0, pango_prompt ? True : False);
	}
	/* draw input field */
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

    /* get match count */
    if (!hidematchcount) {
        recalculatenumbers();
        numberWidth = TEXTW(numbers);
    }

    /* draw stuff */
	if (lines > 0) {

        /* draw image first */
        #if USEIMAGE
        if (!hideimage && longestedge != 0) {
            x = ox;
            x += (imagegaps * 2) + imagewidth;
        }
        #endif

		/* draw grid */
		int i = 0;

		for (item = curr; item != next; item = item->right, i++)
			drawitem(
				item,
				x + ((i / lines) *  ((mw - x) / columns)),
				y + (((i % lines) + 1) * bh),
				(mw - x) / columns
			);
	} else if (matches) {
		/* draw horizontal list */
		x += inputw;
		w = larrowWidth;

		if (curr->left && !hidelarrow) {
			drw_setscheme(drw, scheme[SchemeLArrow]);
			drw_text(drw, x, 0, w, bh, lrpad / 2, leftarrow, 0, pango_leftarrow ? True : False);
		}

		x += w;

		for (item = curr; item != next; item = item->right)
            x = drawitem(item, x, 0, MIN(pango_item ? TEXTWM(item->text) : TEXTW(item->text), mw - x - rarrowWidth - numberWidth - modeWidth));

		if (next && !hiderarrow) {
			w = rarrowWidth;
			drw_setscheme(drw, scheme[SchemeRArrow]);

            drw_text(drw, mw - w - numberWidth - modeWidth, 0, w, bh, lrpad / 2, rightarrow, 0, pango_rightarrow ? True : False);
		}
	}

    if (!hidematchcount) {
        drw_setscheme(drw, scheme[SchemeNumber]);
        drw_text(drw, mw - numberWidth - modeWidth, 0, numberWidth, bh, lrpad / 2, numbers, 0, pango_numbers ? True : False);
    }

    if (!hidemode) {
        drw_setscheme(drw, scheme[SchemeMode]);
        drw_text(drw, mw - modeWidth, 0, modeWidth, bh, lrpad / 2, modetext, 0, pango_mode ? True : False);
    }

	drw_map(drw, win, 0, 0, mw, mh);
}
