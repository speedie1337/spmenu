/* See LICENSE file for copyright and license details. */
void drawhighlights(struct item *item, int x, int y, int w, int p, const char *ittext) {
    int i, indent;
    char *highlight;
    char c;

    // limitation in order to prevent highlighting from drawing when the text isn't visible
    if (columns > 5 && lines > 1) return;

    char *itemtext = strdup(ittext);

    if (!(strlen(itemtext) && strlen(text))) return;

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
                    x + indent + (p),
                    y,
                    MIN(w - indent - lrpad, TEXTW(highlight) - lrpad),
                    bh, 0, highlight, 0, pango_highlight ? True : False,
                    item == sel ? col_hlselfg : col_hlnormfg,
                    item == sel ? col_hlselbg : col_hlnormbg,
                    item == sel ? alpha_hlselfg : alpha_hlnormfg,
                    item == sel ? alpha_hlselbg : alpha_hlnormbg);
            highlight[1] = c;
            i++;
        }
        highlight++;
    }
}

int drawitemtext(struct item *item, int x, int y, int w) {
    char buffer[MAXITEMLENGTH]; // buffer containing item text
    int leftpadding = lrpad / 2; // padding
    int wr, rd; // character
    int fg = 7; // foreground
    int bg = 0; // background
    int bgfg = 0; // both
    int ignore = 0; // ignore colors
    int selitem = 0;
    int priitem = 0;
    char *bgcol;
    char *fgcol;
    int bga;
    int fga;

    // memcpy the correct scheme
    if (item == sel) {
        selitem = 1;
        bgcol = col_itemselbg;
        fgcol = col_itemselfg;
        bga = alpha_itemselbg;
        fga = alpha_itemselfg;

        if (item->hp) {
            priitem = 1;
            bgcol = col_itemselpribg;
            fgcol = col_itemselprifg;

            fga = alpha_itemselprifg;
            bga = alpha_itemselpribg;
        }
    } else {
        if (itemn) {
            bgcol = col_itemnormbg2;
            fgcol = col_itemnormfg2;
            fga = alpha_itemnormfg2;
            bga = alpha_itemnormbg2;
        } else {
            bgcol = col_itemnormbg;
            fgcol = col_itemnormfg;
            fga = alpha_itemnormfg;
            bga = alpha_itemnormbg;
        }

        if (item->hp) {
            priitem = 1;
            bgcol = col_itemnormpribg;
            fgcol = col_itemnormprifg;
            fga = alpha_itemnormprifg;
            bga = alpha_itemnormpribg;
        }
    }

    if (is_selected(item->index)) {
        selitem = (lines ? 1 : selitem);
        bgcol = col_itemmarkedbg;
        fgcol = col_itemmarkedfg;
        fga = alpha_itemmarkedfg;
        bga = alpha_itemmarkedbg;
    }

    // apply extra padding
    if ((selitem && !priitem) && lines) {
        leftpadding += selitempadding;
    } else if (priitem && lines) {
        leftpadding += priitempadding;
    } else if (lines) {
        leftpadding += normitempadding;
    }

    // don't color
    if (!coloritems) {
        bgcol = itemn ? col_itemnormbg2 : col_itemnormbg;
        fgcol = itemn ? col_itemnormfg2 : col_itemnormfg;
        bga = itemn ? alpha_itemnormbg2 : alpha_itemnormbg;
        fga = itemn ? alpha_itemnormfg2 : alpha_itemnormfg;
    }

    if (!hidepowerline && powerlineitems && selitem) {
        if (itempwlstyle == 2) {
            drw_circle(drw, x - plw, y, plw, bh, 0, col_menu, bgcol, alpha_menu, bga);
        } else {
            drw_arrow(drw, x - plw, y, plw, bh, 0, itempwlstyle, col_menu, bgcol, alpha_menu, bga);
        }
    }

    // parse item text
    for (wr = 0, rd = 0; item->text[rd]; rd++) {
        if (item->text[rd] == '' && item->text[rd + 1] == '[') {
            size_t alen = strspn(item->text + rd + 2, "0123456789;");
            if (item->text[rd + alen + 2] == 'm' && sgr) { // last character in sequence is always 'm'
                buffer[wr] = '\0';

                if (!lines) {
                    w -= item->text[rd + alen];
                }

                apply_fribidi(buffer);
                drw_text(drw, x, y, MIN(w, TEXTW(buffer) - lrpad) + leftpadding, bh, leftpadding, isrtl ? fribidi_text : buffer, 0, pango_item ? True : False, fgcol, bgcol, fga, bga);
                drawhighlights(item, x, y, MIN(w, TEXTW(buffer) - lrpad) + leftpadding, leftpadding, isrtl ? fribidi_text : buffer);

                // position and width
                x += MIN(w, TEXTW(buffer) - lrpad) + leftpadding;
                w -= MIN(w, TEXTW(buffer) - lrpad) + leftpadding;

                // no highlighting if colored text
                leftpadding = 0;

                char *character = item->text + rd + 1; // current character

                // parse hex colors, m is always the last character
                while (*character != 'm') {
                    unsigned nextchar = strtoul(character + 1, &character, 10);
                    if (ignore)
                        continue;
                    if (bgfg) {
                        if (bgfg < 4 && nextchar == 5) {
                            bgfg <<= 1;
                            continue;
                        }
                        if (bgfg == 4) {
                            fgcol = txtcols[fg = nextchar];
                        } else if (bgfg == 6) {
                            bgcol = txtcols[bg = nextchar];
                        }

                        ignore = 1;
                        continue;
                    }

                    if (nextchar == 1) {
                        fg |= 8;
                        fgcol = txtcols[fg];
                    } else if (nextchar == 22) {
                        fg &= ~8;
                        fgcol = txtcols[fg];
                    } else if (nextchar == 38) {
                        bgfg = 2;
                    } else if (nextchar >= 30 && nextchar <= 37) {
                        fg = nextchar % 10 | (fg & 8);
                        fgcol = txtcols[fg];
                    } else if (nextchar >= 40 && nextchar <= 47) {
                        bg = nextchar % 10;
                        bgcol = txtcols[bg];
                    } else if (nextchar == 48) {
                        bgfg = 3;
                    } else if (nextchar == 0) {
                        // memcpy the correct scheme
                        if (item == sel) {
                            selitem = 1;
                            bgcol = col_itemselbg;
                            fgcol = col_itemselfg;
                            bga = alpha_itemselbg;
                            fga = alpha_itemselfg;

                            if (item->hp) {
                                priitem = 1;
                                bgcol = col_itemselpribg;
                                fgcol = col_itemselprifg;

                                fga = alpha_itemselprifg;
                                bga = alpha_itemselpribg;
                            }
                        } else {
                            if (itemn) {
                                bgcol = col_itemnormbg2;
                                fgcol = col_itemnormfg2;
                                fga = alpha_itemnormfg2;
                                bga = alpha_itemnormbg2;
                            } else {
                                bgcol = col_itemnormbg;
                                fgcol = col_itemnormfg;
                                fga = alpha_itemnormfg;
                                bga = alpha_itemnormbg;
                            }

                            if (item->hp) {
                                priitem = 1;
                                bgcol = col_itemnormpribg;
                                fgcol = col_itemnormprifg;
                                fga = alpha_itemnormprifg;
                                bga = alpha_itemnormpribg;
                            }
                        }

                        if (is_selected(item->index)) {
                            selitem = (lines ? 1 : selitem);
                            bgcol = col_itemmarkedbg;
                            fgcol = col_itemmarkedfg;
                            fga = alpha_itemmarkedfg;
                            bga = alpha_itemmarkedbg;
                        }

                        // don't color
                        if (!coloritems) {
                            bgcol = itemn ? col_itemnormbg2 : col_itemnormbg;
                            fgcol = itemn ? col_itemnormfg2 : col_itemnormfg;
                            bga = itemn ? alpha_itemnormbg2 : alpha_itemnormbg;
                            fga = itemn ? alpha_itemnormfg2 : alpha_itemnormfg;
                        }
                    }
                }

                rd += alen + 2;
                wr = 0;

                continue;
            }
        }

        buffer[wr++] = item->text[rd];
    }

    buffer[wr] = '\0';

    // now draw any non-colored text
    apply_fribidi(buffer);
    int r = drw_text(drw, x, y, w, bh, leftpadding, isrtl ? fribidi_text : buffer, 0, pango_item ? True : False, fgcol, bgcol, fga, bga);
    if (!hidehighlight) drawhighlights(item, x, y, w, leftpadding, buffer);

    // copy current buffer to item->clntext instead of item->text, this way SGR sequences aren't drawn
    item->clntext = malloc(sizeof(buffer));
    memcpy(item->clntext, buffer, sizeof(buffer));

    if (!hidepowerline && powerlineitems && selitem) {
        if (itempwlstyle == 2) {
            drw_circle(drw, r, y, plw, bh, 1, col_menu, bgcol, alpha_menu, bga);
        } else {
            drw_arrow(drw, r, y, plw, bh, 1, itempwlstyle, col_menu, bgcol, alpha_menu, bga);
        }
    }

    return r;
}

int drawitem(int x, int y, int w) {
    struct item *item;

    int numberWidth = 0;
    int modeWidth = 0;
    int larrowWidth = 0;
    int rarrowWidth = 0;
    int capsWidth = 0;

    // add width
    if (!hidelarrow) larrowWidth = pango_leftarrow ? TEXTWM(leftarrow) : TEXTW(leftarrow);
    if (!hiderarrow) rarrowWidth = pango_rightarrow ? TEXTWM(rightarrow) : TEXTW(rightarrow);
    if (!hidemode) modeWidth = pango_mode ? TEXTWM(modetext) : TEXTW(modetext);
    if (!hiderarrow) rarrowWidth = pango_rightarrow ? TEXTWM(rightarrow) : TEXTW(rightarrow);
    if (!hidematchcount) numberWidth = pango_numbers ? TEXTWM(numbers) : TEXTW(numbers);
    if (!hidecaps) capsWidth = pango_caps ? TEXTWM(capstext) : TEXTW(capstext);

    if (!strcmp(capstext, ""))
        capsWidth = 0;

#if USEIMAGE
    int ox = 0; // original x position
#endif

    // draw items and image
    if (lines > 0) {
        int i = 0;
        int rx = 0;

        // draw image first
#if USEIMAGE
        if (!hideimage && longestedge != 0) {
            rx = ox;
            rx += MAX((imagegaps * 2) + imagewidth + menumarginh, indentitems ? x : 0);
        } else
#endif
            if (!indentitems) {
                rx = 0;
            } else {
                rx = x;
            }

        int itemoverride = 1;
        itemn = 0;

        for (item = curr; item != next; item = item->right, i++) {
            x = drawitemtext(
                    item,
                    rx + ((i / lines) *  ((mw - rx) / columns)) + (powerlineitems ? plw : 0),
                    y + (((i % lines) + 1) * bh),
                    (mw - rx) / columns - (powerlineitems ? 2 * plw : 0)
            );

            if (item == sel && itemoverride) {
                itemnumber = i;
                itemoverride = 0;
            }

            itemn = !itemn;
        }

        // horizontal list
    } else if (matches) {
        x += inputw;

        if (!hidelarrow) {
            w = larrowWidth;
            x = drawlarrow(x, y, w);
        }

        itemnumber = 0;
        int itemoverride = 1;

        for (item = curr; item != next; item = item->right) { // draw items
            x = drawitemtext(item, x + (powerlineitems ? plw : 0), y, MIN(pango_item ? TEXTWM(item->text) : TEXTW(item->text),
                        mw - x -
                        rarrowWidth -
                        numberWidth -
                        modeWidth -
                        capsWidth -
                        menumarginh -
                        2 * sp -
                        2 * borderwidth
                        ));

            if (itemoverride) {
                itemnumber++;
            }

            if (item == sel) {
                itemoverride = 0;
            }
        }

        if (!hiderarrow) {
            w = rarrowWidth + numberWidth + modeWidth + capsWidth + menumarginh + 2 * sp + 2 * borderwidth;
            x = drawrarrow(mw - w, y, w);
        }
    }

    return x;
}

int drawprompt(int x, int y, int w) {
    if (prompt && *prompt && !hideprompt) {
        x = drw_text(drw, x, y, w, bh, lrpad / 2, prompt, 0, pango_prompt ? True : False, col_promptfg, col_promptbg, alpha_promptfg, alpha_promptbg);

        if (!hidepowerline && powerlineprompt) {
            if (promptpwlstyle == 2) {
                drw_circle(drw, x, y, plw, bh, 1, col_menu, col_promptbg, alpha_menu, alpha_promptbg);
            } else {
                drw_arrow(drw, x, y, plw, bh, 1, promptpwlstyle, col_menu, col_promptbg, alpha_menu, alpha_promptbg);
            }

            x += plw;
        }
    }

    return x;
}

int drawinput(int x, int y, int w) {
    char *censort; // censor text (password)
    unsigned int curpos = 0;
    int fh = caretheight;
    int fw = MAX(2, caretwidth);
    int fp = caretpadding;

    if (fh > bh) {
        fh = bh;
    } else if (!fh) {
        fh = drw->font->h;
    }

    if (passwd) {
        censort = ecalloc(1, sizeof(text));

        for (int i = 0; i < strlen(text); i++)
            memcpy(&censort[i], password, strlen(text));

        apply_fribidi(censort);
        drw_text(drw, x, y, w, bh, lrpad / 2, isrtl ? fribidi_text : censort, 0, pango_password ? True : False, col_inputfg, col_inputbg, alpha_inputfg, alpha_inputbg);

        curpos = TEXTW(censort) - TEXTW(&text[cursor]);

        free(censort);
    } else if (!passwd) {
        apply_fribidi(text);
        drw_text(drw, x, y, w, bh, lrpad / 2, isrtl ? fribidi_text : text, 0, pango_input ? True : False, col_inputfg, col_inputbg, alpha_inputfg, alpha_inputbg);

        curpos = TEXTW(text) - TEXTW(&text[cursor]);
    }

    if ((curpos += lrpad / 2 - 1) < w && !hidecaret && cursorstate) {
        curpos += fp;
        drw_rect(drw, x + curpos, 2 + (bh - fh) / 2 + y, fw, fh - 4, 1, 0, col_caretfg, col_caretbg, alpha_caretfg, alpha_caretbg);
    }

    return x;
}

int drawlarrow(int x, int y, int w) {
    if (hidelarrow) return x;

    if (curr->left) { // draw left arrow
        drw_text(drw, x, y, w, bh, lrpad / 2, leftarrow, 0, pango_leftarrow ? True : False, col_larrowfg, col_larrowbg, alpha_larrowfg, alpha_larrowbg);
        x += w;
    }

    return x;
}

int drawrarrow(int x, int y, int w) {
    if (hiderarrow) return x;

    if (next) { // draw right arrow
        drw_text(drw, mw - w, y, w, bh, lrpad / 2, rightarrow, 0, pango_rightarrow ? True : False, col_rarrowfg, col_rarrowbg, alpha_rarrowfg, alpha_rarrowbg);
        x += w;
    }

    return x;
}

int drawnumber(int x, int y, int w) {
    if (hidematchcount) return x;

    int powerlinewidth = 0;

    if (!hidepowerline && powerlinecount) {
        powerlinewidth = plw / 2;
    }

    drw_text(drw, x, y, w, bh, lrpad / 2 + powerlinewidth, numbers, 0, pango_numbers ? True : False, col_numfg, col_numbg, alpha_numfg, alpha_numbg);

    // draw powerline for match count
    if (!hidepowerline && powerlinecount) {
        if (matchcountpwlstyle == 2) {
            drw_circle(drw, x, y, plw, bh, 0, col_menu, col_numbg, alpha_menu, alpha_numbg);
        } else {
            drw_arrow(drw, x, y, plw, bh, 0, matchcountpwlstyle, col_menu, col_numbg, alpha_menu, alpha_numbg);
        }

        x += plw;
    }

    return x;
}

int drawmode(int x, int y, int w) {
    if (!hidemode) { // draw mode indicator
        int powerlinewidth = 0;

        if (!hidepowerline && powerlinemode) {
            powerlinewidth = plw / 2;
        }

        drw_text(drw, x, y, w, bh, lrpad / 2 + powerlinewidth, modetext, 0, pango_mode ? True : False, col_modefg, col_modebg, alpha_modefg, alpha_modebg);

        // draw powerline for match count
        if (!hidepowerline && powerlinemode) {
            if (modepwlstyle == 2) {
                drw_circle(drw, x, y, plw, bh, 0,
                        hidematchcount ? col_menu : col_numbg, col_modebg,
                        hidematchcount ? alpha_menu : alpha_numbg, alpha_modebg);
            } else {
                drw_arrow(drw, x, y, plw, bh, 0, modepwlstyle,
                        hidematchcount ? col_menu : col_numbg, col_modebg,
                        hidematchcount ? alpha_menu : alpha_numbg, alpha_modebg);
            }

            x += plw;
        }
    }

    return x;
}

int drawcaps(int x, int y, int w) {
    if (!w) return x; // not caps lock

    if (!hidecaps) { // draw caps lock indicator
        int powerlinewidth = 0;

        if (!hidepowerline && powerlinecaps) {
            powerlinewidth = plw / 2;
        }

        drw_text(drw, x, y, w, bh, lrpad / 2 + powerlinewidth, capstext, 0, pango_caps ? True : False, col_capsfg, col_capsbg, alpha_capsfg, alpha_capsbg);

        // draw powerline for caps lock indicator
        if (!hidepowerline && powerlinecaps) {
            if (capspwlstyle == 2) {
                drw_circle(drw, x, y, plw, bh, 0,
                        hidemode ? hidematchcount ? col_menu : col_numbg : col_modebg, col_capsbg,
                        hidemode ? hidematchcount ? alpha_menu : alpha_numbg : alpha_modebg, alpha_capsbg);
            } else {
                drw_arrow(drw, x, y, plw, bh, 0, capspwlstyle,
                        hidemode ? hidematchcount ? col_menu : col_numbg : col_modebg, col_capsbg,
                        hidemode ? hidematchcount ? alpha_menu : alpha_numbg : alpha_modebg, alpha_capsbg);
            }

            x += plw;
        }
    }

    return x;
}

void drawmenu(void) {
#if USEWAYLAND
    if (protocol) {
        readfile();
        drawmenu_layer();

        wl_surface_set_buffer_scale(state.surface, 1);
        wl_surface_attach(state.surface, state.buffer, 0, 0);
        wl_surface_damage(state.surface, 0, 0, state.width, state.height);
        wl_surface_commit(state.surface);

#if USEIMAGE
        drawimage();
#endif
    } else {
        drawmenu_layer();
    }
#elif USEX
    drawmenu_layer();
#endif
}

void drawmenu_layer(void) {
    int x = 0, y = 0, w = 0;
    plw = hidepowerline ? 0 : drw->font->h / 2 + 1; // powerline size

    // draw menu first using menu scheme
    drw_rect(drw, 0, 0, mw, mh, 1, 1, col_menu, col_menu, alpha_menu, alpha_menu);

    int numberWidth = 0;
    int modeWidth = 0;
    int capsWidth = 0;

    // add width
    if (!hidemode) modeWidth = pango_mode ? TEXTWM(modetext) : TEXTW(modetext);
    if (!hidecaps) capsWidth = pango_caps ? TEXTWM(capstext) : TEXTW(capstext);

    if (!strcmp(capstext, ""))
        capsWidth = 0;

    // calculate match count
    if (!hidematchcount) {
        recalculatenumbers();
        numberWidth = TEXTW(numbers);
    }

    x += menumarginh;
    y += menumarginv;

    calcoffsets();
    get_mh();

    // bh must be removed from menu height resizing later
    if ((hideprompt && hideinput && hidemode && hidematchcount && hidecaps) && lines) {
        y -= bh;
    }

    if (!hideprompt && !fullscreen) {
        w = promptw;
        x = drawprompt(x, y, w);
    }

    if (!hideinput && !fullscreen) {
        w = (lines > 0 || !matches) ? mw - x : inputw;
        x = drawinput(x, y, w);
    }

    if (!hidemode && !fullscreen) modeWidth = pango_mode ? TEXTWM(modetext) : TEXTW(modetext);

    // draw the items, this function also calls drawrarrow() and drawlarrow()
    if (!hideitem) drawitem(x, y, w);

    if (!hidematchcount && !fullscreen) {
        w = numberWidth;
        drawnumber(mw - numberWidth - modeWidth - capsWidth - 2 * sp - 2 * borderwidth - menumarginh, y, w);
    }

    if (!hidemode && !fullscreen) {
        w = modeWidth;
        drawmode(mw - modeWidth - capsWidth - 2 * sp - 2 * borderwidth - menumarginh, y, w);
    }

    if (!hidecaps && !fullscreen) {
        w = capsWidth;
        drawcaps(mw - capsWidth - 2 * sp - 2 * borderwidth - menumarginh, y, w);
    }

#if USEX
#if USEIMAGE
    drawimage();
#endif
    drw_map(drw, win, 0, 0, mw, mh);
#endif
    }
