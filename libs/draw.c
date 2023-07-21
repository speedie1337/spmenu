/* See LICENSE file for copyright and license details. */
void drawhighlights(struct item *item, int x, int y, int w, int p, const char *ittext) {
    int i, indent;
    char *highlight;
    char c;

    // limitation in order to prevent highlighting from drawing when the text isn't visible
    if (columns > 5 && lines > 1) return;

    char *itemtext = strdup(ittext);

    if (!(strlen(itemtext) && strlen(tx.text))) return;

    if (strstr(itemtext, "</")) return;

    for (i = 0, highlight = itemtext; *highlight && tx.text[i];) {
        if (((fuzzy && !fstrncmp(&(*highlight), &tx.text[i], 1)) || (!fuzzy && *highlight == tx.text[i]))) {
            c = *highlight;
            *highlight = '\0';
            indent = TEXTW(itemtext) - sp.lrpad;
            *highlight = c;

            // highlight character
            c = highlight[1];
            highlight[1] = '\0';
            draw_text(
                    draw,
                    x + indent + (p),
                    y,
                    MIN(w - indent - sp.lrpad, TEXTW(highlight) - sp.lrpad),
                    sp.bh, 0, highlight, 0, False,
                    item == selecteditem ? col_hlselfg : col_hlnormfg,
                    item == selecteditem ? col_hlselbg : col_hlnormbg,
                    item == selecteditem ? alpha_hlselfg : alpha_hlnormfg,
                    item == selecteditem ? alpha_hlselbg : alpha_hlnormbg);
            highlight[1] = c;
            i++;
        }
        highlight++;
    }
}

char* get_text_n_sgr(struct item *item) {
    char buffer[MAXITEMLENGTH];
    int character, escape;

    char *sgrtext = malloc(sizeof(buffer));
    sgrtext[0] = '\0';

    for (character = 0, escape = 0; item->text[escape]; escape++) {
        if (item->text[escape] == '' && item->text[escape + 1] == '[') {
            size_t colindex = strspn(item->text + escape + 2, "0123456789;");
            if (item->text[escape + colindex + 2] == 'm' && sgr) { // last character in sequence is always 'm'
                buffer[character] = '\0';

                strcat(sgrtext, buffer);

                escape += colindex + 2;
                character = 0;

                continue;
            }
        }

        buffer[character++] = item->text[escape];
    }

    buffer[character] = '\0';
    strcat(sgrtext, buffer);

    return sgrtext;
}

int drawitemtext(struct item *item, int x, int y, int w) {
    char buffer[MAXITEMLENGTH]; // buffer containing item text
    int character, escape;
    int leftpadding = sp.lrpad / 2; // padding
    int fg = 7; // foreground
    int bg = 0; // background
    int bgfg = 0; // both
    int ignore = 0; // ignore colors
    int selitem = 0;
    int priitem = 0;
    char *bgcol;
    char *fgcol;
    char *obgcol;
    char *ofgcol;
    int bga;
    int fga;
    int obga;
    int ofga;

    int ox;
    int oy;
    int ow;

    int oleftpadding;

    // memcpy the correct scheme
    if (item == selecteditem) {
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

    ofga = fga;
    obga = bga;
    ofgcol = fgcol;
    obgcol = bgcol;
    oleftpadding = leftpadding;

    if (!hidepowerline && powerlineitems && selitem) {
        if (itempwlstyle == 2) {
            draw_circle(draw, x - sp.plw, y, sp.plw, sp.bh, 0, col_menu, bgcol, alpha_menu, bga);
        } else {
            draw_arrow(draw, x - sp.plw, y, sp.plw, sp.bh, 0, itempwlstyle, col_menu, bgcol, alpha_menu, bga);
        }
    }

#if USEIMAGE
    if (!hideimage && !imagetype && lines) {
        draw_rect(draw, x, y, w, sp.bh, 1, 1, fgcol, bgcol, fga, bga);
        int nx = draw_icon(item, x, y + sp.lrpad / 4, sp.bh - sp.lrpad / 2, sp.bh - sp.lrpad / 2);

        if (nx != x) {
            x = nx;
            w -= sp.bh - sp.lrpad / 2;
        }
    }
#endif

    ox = x;
    oy = y;
    ow = w;

    // parse item text
    for (character = 0, escape = 0; item->text[escape]; escape++) {
        if (item->text[escape] == '' && item->text[escape + 1] == '[') {
            size_t colindex = strspn(item->text + escape + 2, "0123456789;");
            if (item->text[escape + colindex + 2] == 'm' && sgr) { // last character in sequence is always 'm'
                buffer[character] = '\0';

                apply_fribidi(buffer);
                draw_text(draw, x, y, w, sp.bh, leftpadding, isrtl ? fribidi_text : buffer, 0, pango_item ? True : False, fgcol, bgcol, fga, bga);

                // position and width
                x += MIN(w, TEXTW(buffer) - sp.lrpad + leftpadding);
                w -= MIN(w, TEXTW(buffer) - sp.lrpad + leftpadding);

                // no highlighting if colored text
                leftpadding = 0;

                char *c_character = item->text + escape + 1; // current character

                // parse hex colors, m is always the last character
                while (*c_character != 'm') {
                    unsigned nextchar = strtoul(c_character + 1, &c_character, 10);
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
                        int r, g, b, c;

                        c = strtoul(c_character + 1, NULL, 10);

                        if (c == 5) {
                            bgfg = 2;
                            continue;
                        }

                        strtoul(c_character + 1, &c_character, 10);

                        r = strtoul(c_character + 1, &c_character, 10);
                        g = strtoul(c_character + 1, &c_character, 10);
                        b = strtoul(c_character + 1, &c_character, 10);

                        fgcol = malloc(8 * sizeof(char));
                        snprintf(fgcol, 8, "#%02X%02X%02X", r, g, b);
                    } else if (nextchar == 39) {
                        fga = ofga;
                        fgcol = ofgcol;
                    } else if (nextchar >= 30 && nextchar <= 37) {
                        fg = nextchar % 10 | (fg & 8);
                        fgcol = txtcols[fg];
                    } else if (nextchar >= 40 && nextchar <= 47) {
                        bg = nextchar % 10;
                        bgcol = txtcols[bg];
                    } else if (nextchar == 48) {
                        int r, g, b, c;

                        c = strtoul(c_character + 1, NULL, 10);

                        if (c == 5) {
                            bgfg = 3;
                            continue;
                        }

                        strtoul(c_character + 1, &c_character, 10);

                        r = strtoul(c_character + 1, &c_character, 10);
                        g = strtoul(c_character + 1, &c_character, 10);
                        b = strtoul(c_character + 1, &c_character, 10);

                        bgcol = malloc(8 * sizeof(char));
                        snprintf(bgcol, 8, "#%02X%02X%02X", r, g, b);
                    } else if (nextchar == 49) {
                        bga = obga;
                        bgcol = obgcol;
                    } else if (nextchar == 0) {
                        fgcol = ofgcol;
                        bgcol = obgcol;
                        fga = ofga;
                        bga = obga;
                    }
                }

                escape += colindex + 2;
                character = 0;

                continue;
            }
        }

        buffer[character++] = item->text[escape];
    }

    buffer[character] = '\0';

    // now draw any non-colored text
    apply_fribidi(buffer);
    int ret = draw_text(draw, x, y, w, sp.bh, leftpadding, isrtl ? fribidi_text : buffer, 0, pango_item ? True : False, fgcol, bgcol, fga, bga);

    if (!hidehighlight)
        drawhighlights(item, ox, oy, ow, oleftpadding, item->nsgrtext);

    if (!hidepowerline && powerlineitems && selitem) {
        if (itempwlstyle == 2) {
            draw_circle(draw, ret, y, sp.plw, sp.bh, 1, col_menu, obgcol, alpha_menu, obga);
        } else {
            draw_arrow(draw, ret, y, sp.plw, sp.bh, 1, itempwlstyle, col_menu, obgcol, alpha_menu, obga);
        }
    }

    return ret;
}

int drawitem(int x, int y, int w) {
    struct item *item;

    int numberw = 0;
    int modew = 0;
    int larroww = 0;
    int rarroww = 0;
    int capsw = 0;

    // add width
    if (!hidelarrow) larroww = pango_leftarrow ? TEXTWM(leftarrow) : TEXTW(leftarrow);
    if (!hiderarrow) rarroww = pango_rightarrow ? TEXTWM(rightarrow) : TEXTW(rightarrow);
    if (!hidemode) modew = pango_mode ? TEXTWM(tx.modetext) : TEXTW(tx.modetext);
    if (!hiderarrow) rarroww = pango_rightarrow ? TEXTWM(rightarrow) : TEXTW(rightarrow);
    if (!hidematchcount) numberw = pango_numbers ? TEXTWM(tx.numbers) : TEXTW(tx.numbers);
    if (!hidecaps) capsw = pango_caps ? TEXTWM(tx.capstext) : TEXTW(tx.capstext);

    if (!strcmp(tx.capstext, ""))
        capsw = 0;

#if USEIMAGE
    int ox = 0; // original x position
#endif

    // draw items and image
    if (lines > 0) {
        int i = 0;
        int rx = 0;

        // draw image first
#if USEIMAGE
        if (!hideimage && img.longestedge != 0 && imagetype) {
            rx = ox;
            rx += MAX((img.imagegaps * 2) + img.imagewidth + menumarginh, indentitems ? x : 0);
        } else
#endif
            if (!indentitems) {
                rx = 0;
            } else {
                rx = x;
            }

        int itemoverride = 1;
        itemn = 0;

        for (item = currentitem; item != nextitem; item = item->right, i++) {
            item->nsgrtext = get_text_n_sgr(item);

            x = drawitemtext(
                    item,
                    rx + menumarginh + ((i / lines) *  ((sp.mw - rx) / columns)) + (powerlineitems ? sp.plw : 0),
                    y + (((i % lines) + 1) * sp.bh),
                    (sp.mw - rx) / columns - (powerlineitems ? 2 * sp.plw : 0) - (2 * menumarginh)
            );

            if (item == selecteditem && itemoverride) {
                sp.itemnumber = i;
                itemoverride = 0;
            }

            itemn = !itemn;
        }

        // horizontal list
    } else if (matches) {
        x += sp.inputw;

        if (!hidelarrow) {
            w = larroww;
            x = drawlarrow(x, y, w);
        }

        sp.itemnumber = 0;
        int itemoverride = 1;

        for (item = currentitem; item != nextitem; item = item->right) { // draw items
            item->nsgrtext = get_text_n_sgr(item);

            x = drawitemtext(item, x + (powerlineitems ? 2 * sp.plw : 0), y, MIN(pango_item ? TEXTWM(item->nsgrtext) : TEXTW(item->nsgrtext),
                        sp.mw - x -
                        rarroww -
                        numberw -
                        modew -
                        capsw -
                        menumarginh -
                        2 * sp.sp -
                        2 * borderwidth -
                        (powerlineitems ? 2 * sp.plw : 0)
                        ));

            if (itemoverride) {
                sp.itemnumber++;
            }

            if (item == selecteditem) {
                itemoverride = 0;
            }
        }

        if (!hiderarrow) {
            w = rarroww + numberw + modew + capsw + menumarginh + 2 * sp.sp + 2 * borderwidth;
            x = drawrarrow(sp.mw - w, y, w);
        }
    }

    return x;
}

int drawprompt(int x, int y, int w) {
    if (prompt && *prompt && !hideprompt) {
        x = draw_text(draw, x, y, w, sp.bh, sp.lrpad / 2, prompt, 0, pango_prompt ? True : False, col_promptfg, col_promptbg, alpha_promptfg, alpha_promptbg);

        if (!hidepowerline && powerlineprompt) {
            if (promptpwlstyle == 2) {
                draw_circle(draw, x, y, sp.plw, sp.bh, 1, col_menu, col_promptbg, alpha_menu, alpha_promptbg);
            } else {
                draw_arrow(draw, x, y, sp.plw, sp.bh, 1, promptpwlstyle, col_menu, col_promptbg, alpha_menu, alpha_promptbg);
            }

            x += sp.plw;
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

    if (fh > sp.bh || !fh) {
        fh = sp.bh - sp.lrpad / 4;
    }

    if (passwd) {
        censort = ecalloc(1, sizeof(tx.text));

        for (int i = 0; i < strlen(tx.text); i++)
            memcpy(&censort[i], password, strlen(tx.text));

        apply_fribidi(censort);
        draw_text(draw, x, y, w, sp.bh, sp.lrpad / 2, isrtl ? fribidi_text : censort, 0, pango_password ? True : False, col_inputfg, col_inputbg, alpha_inputfg, alpha_inputbg);

        curpos = TEXTW(censort) - TEXTW(&tx.text[sp.cursor]);

        free(censort);
    } else if (!passwd) {
        apply_fribidi(tx.text);
        draw_text(draw, x, y, w, sp.bh, sp.lrpad / 2, isrtl ? fribidi_text : tx.text, 0, pango_input ? True : False, col_inputfg, col_inputbg, alpha_inputfg, alpha_inputbg);

        curpos = TEXTW(tx.text) - TEXTW(&tx.text[sp.cursor]);
    }

    if ((curpos += sp.lrpad / 2 - 1) < w && !hidecaret) {
        curpos += fp;
        draw_rect(draw, x + curpos, 2 + (sp.bh - fh) / 2 + y, fw, fh - 4, 1, 0, col_caretfg, col_caretbg, alpha_caretfg, alpha_caretbg);
    }

    return x;
}

int drawlarrow(int x, int y, int w) {
    if (hidelarrow) return x;

    if (currentitem->left) { // draw left arrow
        draw_text(draw, x, y, w, sp.bh, sp.lrpad / 2, leftarrow, 0, pango_leftarrow ? True : False, col_larrowfg, col_larrowbg, alpha_larrowfg, alpha_larrowbg);
        x += w;
    }

    return x;
}

int drawrarrow(int x, int y, int w) {
    if (hiderarrow) return x;

    if (nextitem) { // draw right arrow
        draw_text(draw, sp.mw - w, y, w, sp.bh, sp.lrpad / 2, rightarrow, 0, pango_rightarrow ? True : False, col_rarrowfg, col_rarrowbg, alpha_rarrowfg, alpha_rarrowbg);
        x += w;
    }

    return x;
}

int drawnumber(int x, int y, int w) {
    if (hidematchcount) return x;

    int powerlinewidth = 0;

    if (!hidepowerline && powerlinecount) {
        powerlinewidth = sp.plw / 2;
    }

    draw_text(draw, x, y, w, sp.bh, sp.lrpad / 2 + powerlinewidth, tx.numbers, 0, pango_numbers ? True : False, col_numfg, col_numbg, alpha_numfg, alpha_numbg);

    // draw powerline for match count
    if (!hidepowerline && powerlinecount) {
        if (matchcountpwlstyle == 2) {
            draw_circle(draw, x, y, sp.plw, sp.bh, 0, col_menu, col_numbg, alpha_menu, alpha_numbg);
        } else {
            draw_arrow(draw, x, y, sp.plw, sp.bh, 0, matchcountpwlstyle, col_menu, col_numbg, alpha_menu, alpha_numbg);
        }

        x += sp.plw;
    }

    return x;
}

int drawmode(int x, int y, int w) {
    if (!hidemode) { // draw mode indicator
        int powerlinewidth = 0;

        if (!hidepowerline && powerlinemode) {
            powerlinewidth = sp.plw / 2;
        }

        draw_text(draw, x, y, w, sp.bh, sp.lrpad / 2 + powerlinewidth, tx.modetext, 0, pango_mode ? True : False, col_modefg, col_modebg, alpha_modefg, alpha_modebg);

        // draw powerline for match count
        if (!hidepowerline && powerlinemode) {
            if (modepwlstyle == 2) {
                draw_circle(draw, x, y, sp.plw, sp.bh, 0,
                        hidematchcount ? col_menu : col_numbg, col_modebg,
                        hidematchcount ? alpha_menu : alpha_numbg, alpha_modebg);
            } else {
                draw_arrow(draw, x, y, sp.plw, sp.bh, 0, modepwlstyle,
                        hidematchcount ? col_menu : col_numbg, col_modebg,
                        hidematchcount ? alpha_menu : alpha_numbg, alpha_modebg);
            }

            x += sp.plw;
        }
    }

    return x;
}

int drawcaps(int x, int y, int w) {
    if (!w) return x; // not caps lock

    if (!hidecaps) { // draw caps lock indicator
        int powerlinewidth = 0;

        if (!hidepowerline && powerlinecaps) {
            powerlinewidth = sp.plw / 2;
        }

        draw_text(draw, x, y, w, sp.bh, sp.lrpad / 2 + powerlinewidth, tx.capstext, 0, pango_caps ? True : False, col_capsfg, col_capsbg, alpha_capsfg, alpha_capsbg);

        // draw powerline for caps lock indicator
        if (!hidepowerline && powerlinecaps) {
            if (capspwlstyle == 2) {
                draw_circle(draw, x, y, sp.plw, sp.bh, 0,
                        hidemode ? hidematchcount ? col_menu : col_numbg : col_modebg, col_capsbg,
                        hidemode ? hidematchcount ? alpha_menu : alpha_numbg : alpha_modebg, alpha_capsbg);
            } else {
                draw_arrow(draw, x, y, sp.plw, sp.bh, 0, capspwlstyle,
                        hidemode ? hidematchcount ? col_menu : col_numbg : col_modebg, col_capsbg,
                        hidemode ? hidematchcount ? alpha_menu : alpha_numbg : alpha_modebg, alpha_capsbg);
            }

            x += sp.plw;
        }
    }

    return x;
}

void drawmenu(void) {
#if USEWAYLAND
    if (protocol) {
        drawmenu_layer();

#if USEIMAGE
        drawimage();
#endif
        if (listfile) {
            readstdin();

            if (sp.listchanged) {
                resizeclient();
                match();

                for (int i = 0; i < sp.itemnumber; i++) {
                    if (selecteditem && selecteditem->right && (selecteditem = selecteditem->right) == nextitem) {
                        currentitem = nextitem;
                    }
                }

                drawmenu_layer();
            }
        }

        wl_surface_set_buffer_scale(state.surface, 1);
        wl_surface_attach(state.surface, state.buffer, 0, 0);
        wl_surface_damage(state.surface, 0, 0, state.width, state.height);
        wl_surface_commit(state.surface);
    } else {
#endif
#if USEX
        if (listfile) {
            readstdin();

            if (sp.listchanged) {
                match();

                for (int i = 0; i < sp.itemnumber; i++) {
                    if (selecteditem && selecteditem->right && (selecteditem = selecteditem->right) == nextitem) {
                        currentitem = nextitem;
                    }
                }
            }
        }
        drawmenu_layer();
#endif
#if USEWAYLAND
    }
#endif
}

void drawmenu_layer(void) {
    int x = 0, y = 0, w = 0;
    sp.plw = hidepowerline ? 0 : draw->font->h / 2 + 1; // powerline size

    sp_strncpy(tx.modetext, sp.mode ? instext : normtext, sizeof(tx.modetext));

#if USEREGEX
    if (regex && regextext && sp.mode) {
        sp_strncpy(tx.modetext, regextext, sizeof(tx.modetext));
    }
#endif

    // draw menu first using menu scheme
    draw_rect(draw, 0, 0, sp.mw, sp.mh, 1, 1, col_menu, col_menu, alpha_menu, alpha_menu);

    int numberw = 0;
    int modew = 0;
    int capsw = 0;

    // add width
    if (!hidemode) modew = pango_mode ? TEXTWM(tx.modetext) : TEXTW(tx.modetext);
    if (!hidecaps) capsw = pango_caps ? TEXTWM(tx.capstext) : TEXTW(tx.capstext);

    if (!strcmp(tx.capstext, ""))
        capsw = 0;

    // calculate match count
    if (!hidematchcount) {
        recalculatenumbers();
        numberw = TEXTW(tx.numbers);
    }

    x += menumarginh;
    y += menumarginv;

    calcoffsets();
    get_mh();

    // sp.bh must be removed from menu height resizing later
    if ((hideprompt && hideinput && hidemode && hidematchcount && hidecaps) && lines) {
        y -= sp.bh;
    }

    if (!hideprompt) {
        w = sp.promptw;
        x = drawprompt(x, y, w);
    }

    if (!hideinput) {
        w = (lines > 0 || !matches) ? sp.mw - x : sp.inputw;
        x = drawinput(x, y, w);
    }

    if (!hidemode) modew = pango_mode ? TEXTWM(tx.modetext) : TEXTW(tx.modetext);

    // draw the items, this function also calls drawrarrow() and drawlarrow()
    if (!hideitem) drawitem(x, y, w);

    if (!hidematchcount) {
        w = numberw;
        drawnumber(sp.mw - numberw - modew - capsw - 2 * sp.sp - 2 * borderwidth - menumarginh, y, w);
    }

    if (!hidemode) {
        w = modew;
        drawmode(sp.mw - modew - capsw - 2 * sp.sp - 2 * borderwidth - menumarginh, y, w);
    }

    if (!hidecaps) {
        w = capsw;
        drawcaps(sp.mw - capsw - 2 * sp.sp - 2 * borderwidth - menumarginh, y, w);
    }

#if USEX
#if USEIMAGE
    drawimage();
#endif
    draw_map(draw, win, 0, 0, sp.mw, sp.mh);
#endif
    }
