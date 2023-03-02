void
drawhighlights(struct item *item, int x, int y, int maxw)
{
	int i, indent;
	char *highlight;
	char c;

	if (!(strlen(item->text) && strlen(text)))
		return;

	drw_setscheme(drw, scheme[item == sel ? SchemeSelHighlight : SchemeNormHighlight]);
	for (i = 0, highlight = item->text; *highlight && text[i];) {
		if (!fstrncmp(&text[i], highlight, 1)) {
			c = highlight[1];
			highlight[1] = '\0';

			/* get indentation */
			indent = TEXTW(item->text);

			/* highlight character */
			drw_text(drw, x + indent - lrpad, y, MIN(maxw - indent, TEXTW(highlight) - lrpad), bh, 0, highlight, 0, pango_highlight ? True : False);
			highlight[1] = c;
			i++;
		}
		highlight++;
	}
}

int
drawitem(struct item *item, int x, int y, int w)
{
    char buffer[sizeof(item->text) + lrpad / 2];
    Clr scm[3];
    int lp = lrpad / 2; /* padding */
    int wr, rd;
	int rw; /* width of text */
	int fg = 7;
	int bg = 0;
    int bgfg = 0;
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

                /* draw text */
                rw = TEXTWM(buffer) - lrpad;
                #if USERTL
                apply_fribidi(buffer);
				drw_text(drw, x, y, rw + lp, bh, lp, fribidi_text, 0, pango_item ? True : False);
                #else
				drw_text(drw, x, y, rw + lp, bh, lp, buffer, 0, pango_item ? True : False);
                #endif

				x += rw + lp;
                ib = 1;
                lp = 0; /* no padding */

				char *ep = item->text + rd + 1;

                /* parse hex colors in scm */
				while (*ep != 'm') {
					unsigned v = strtoul(ep + 1, &ep, 10);
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

    /* draw any text that doesn't use sgr sequences */
    #if USERTL
    apply_fribidi(buffer);
	int r = drw_text(drw, x, y, w - rw, bh, lp, fribidi_text, 0, pango_item ? True : False);
    #else
	int r = drw_text(drw, x, y, w - rw, bh, lp, buffer, 0, pango_item ? True : False);
    #endif

    if (!hidehighlight && !ib) drawhighlights(item, x, y, w - rw);
    return r;
}

void
drawmenu(void)
{
	unsigned int curpos;
	struct item *item;
	int x = 0, y = 0, fh = drw->font->h, w;
	char *censort;

	drw_setscheme(drw, scheme[SchemeMenu]);
	drw_rect(drw, 0, 0, mw, mh, 1, 1);

    if (hidemode)
        strcpy(modetext, "");

	if (prompt && *prompt) {
        if (colorprompt) {
		    drw_setscheme(drw, scheme[SchemePrompt]);
        }

		x = drw_text(drw, x, 0, promptw, bh, lrpad / 2, prompt, 0, pango_prompt ? True : False);
	}
	/* draw input field */
	w = (lines > 0 || !matches) ? mw - x : inputw;
	drw_setscheme(drw, scheme[SchemeInput]);
	if (passwd) {
	    censort = ecalloc(1, sizeof(text));
		memset(censort, '.', strlen(text));
        #if USERTL
        apply_fribidi(censort);
		drw_text(drw, x, 0, w, bh, lrpad / 2, fribidi_text, 0, pango_password ? True : False);
        #else
		drw_text(drw, x, 0, w - LENGTH(censort), bh, lrpad / 2, censort, 0, pango_password ? True : False);
        #endif
        free(censort);
	} else {
        #if USERTL
        apply_fribidi(text);
        drw_text(drw, x, 0, w, bh, lrpad / 2, fribidi_text, 0, pango_input ? True : False);
        #else
        drw_text(drw, x, 0, w, bh, lrpad / 2, text, 0, pango_input ? True : False);
        #endif
    }

	curpos = TEXTW(text) - TEXTW(&text[cursor]);
	if ((curpos += lrpad / 2 - 1) < w) {
		drw_setscheme(drw, scheme[SchemeCaret]);
		drw_rect(drw, x + curpos, 2 + (bh - fh) / 2, 2, fh - 4, 1, 0);
	}

    /* get match count */
    if (!hidematchcount) recalculatenumbers();

	if (lines > 0) {
        #if USEIMAGE
        if (!hideimage && longestedge != 0) {
            x += imagegaps + imagewidth;
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
		w = TEXTW(leftarrow);
		if (curr->left) {
			drw_setscheme(drw, scheme[SchemeLArrow]);
			drw_text(drw, x, 0, w, bh, lrpad / 2, leftarrow, 0, pango_leftarrow ? True : False);
		}
		x += w;
		for (item = curr; item != next; item = item->right)
            if (hidematchcount) {
                x = drawitem(item, x, 0, MIN(TEXTWM(item->text), mw - x - TEXTW(rightarrow) - TEXTW(modetext)));
            } else {
                x = drawitem(item, x, 0, MIN(TEXTWM(item->text), mw - x - TEXTW(rightarrow) - TEXTW(numbers) - TEXTW(modetext)));
            }
		if (next) {
			w = TEXTW(rightarrow);
			drw_setscheme(drw, scheme[SchemeRArrow]);

            if (hidematchcount) {
                drw_text(drw, mw - w - TEXTW(modetext), 0, w, bh, lrpad / 2, rightarrow, 0, pango_rightarrow ? True : False);
            } else {
                drw_text(drw, mw - w - TEXTW(numbers) - TEXTW(modetext), 0, w, bh, lrpad / 2, rightarrow, 0, pango_rightarrow ? True : False);
            }
		}
	}

    if (!hidematchcount) {
        drw_setscheme(drw, scheme[SchemeNumber]);
        drw_text(drw, mw - TEXTW(numbers) - TEXTW(modetext), 0, TEXTW(numbers), bh, lrpad / 2, numbers, 0, pango_numbers ? True : False);
    }

    if (!hidemode) {
        drw_setscheme(drw, scheme[SchemeMode]);
        drw_text(drw, mw - TEXTW(modetext), 0, TEXTW(modetext), bh, lrpad / 2, modetext, 0, pango_mode ? True : False);
    }

	drw_map(drw, win, 0, 0, mw, mh);
}
