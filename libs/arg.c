/* See LICENSE file for copyright and license details. */
void moveleft(Arg *arg) {
    struct item *tmpsel;
    int i, offscreen = 0;
    int argu = arg->i ? arg->i : 1;

    if (columns > 1) {
        if (!sel)
            return;
        tmpsel = sel;
        for (i = 0; i < lines; i++) {
            if (!tmpsel->left || tmpsel->left->right != tmpsel) {
                if (offscreen)
                    drawmenu();
                return;
            }
            if (tmpsel == curr)
                offscreen = 1;
            tmpsel = tmpsel->left;
        }
        sel = tmpsel;
        if (offscreen) {
            for (int j = 0; j < argu; j++) {
                curr = prev;
            }
        }

        drawmenu();
        calcoffsets();
    }
}

void moveright(Arg *arg) {
    struct item *tmpsel;
    int i, offscreen = 0;
    int argu = arg->i ? arg->i : 1;

    if (columns > 1) {
        if (!sel)
            return;
        tmpsel = sel;
        for (i = 0; i < lines; i++) {
            if (!tmpsel->right ||  tmpsel->right->left != tmpsel) {
                if (offscreen)
                    drawmenu();
                return;
            }
            tmpsel = tmpsel->right;
            if (tmpsel == next)
                offscreen = 1;
        }
        sel = tmpsel;
        if (offscreen) {
            for (int j = 0; j < argu; j++)
                curr = next;
        }
        calcoffsets();
    }

    drawmenu();
}

void movedown(Arg *arg) {
    int argu = arg->i ? arg->i : 1;

    for (int j = 0; j < argu; j++) {
        if (sel && sel->right && (sel = sel->right) == next) {
            curr = next;
        }
    }

    calcoffsets();
    drawmenu();
}

void moveup(Arg *arg) {
    int argu = arg->i ? arg->i : 1;

    for (int j = 0; j < argu; j++) {
        if (sel && sel->left && (sel = sel->left)->right == curr) {
            curr = prev;
        }
    }

    calcoffsets();
    drawmenu();
}

void complete(Arg *arg) {
    if (hideitem) return;

    strncpy(tx.text, sel->clntext, sizeof tx.text - 1);
    tx.text[sizeof tx.text - 1] = '\0';
    sp.cursor = strlen(tx.text);

    match();
    drawmenu();
}

void movenext(Arg *arg) {
    if (!next)
        return;

    sel = curr = next;
    drawmenu();
}

void moveprev(Arg *arg) {
    if (!prev)
        return;

    sel = curr = prev;
    calcoffsets();
    drawmenu();
}

void moveitem(Arg *arg) {
    for (int i = 0; i < arg->i; i++) {
        if (sel && sel->right && (sel = sel->right) == next) {
            curr = next;
            calcoffsets();
        }
    }

    drawmenu();
}

void movestart(Arg *arg) {
    if (sel == matches) {
        sp.cursor = 0;
        drawmenu();
        return;
    }

    sel = curr = matches;
    calcoffsets();
    drawmenu();
}

void moveend(Arg *arg) {
    if (tx.text[sp.cursor] != '\0') {
        sp.cursor = strlen(tx.text);
        drawmenu();
        return;
    }

    if (next) {
        curr = matchend;
        calcoffsets();
        curr = prev;
        calcoffsets();

        while (next && (curr = curr->right))
            calcoffsets();
    }

    sel = matchend;
    drawmenu();
}

void paste(Arg *arg) {
#if USEX
    if (!protocol) {
        paste_x11(arg->i);
    }
#endif
}

void viewhist(Arg *arg) {
    int i;

    if (histfile) {
        if (!backup_items) {
            backup_items = items;
            items = calloc(histsz + 1, sizeof(struct item));

            if (!items) {
                die("spmenu: cannot allocate memory");
            }

            for (i = 0; i < histsz; i++) {
                items[i].text = history[i];
            }
        } else {
            free(items);
            items = backup_items;
            backup_items = NULL;
        }
    }

    match();
    drawmenu();
}

void deleteword(Arg *arg) {
    if (sp.cursor == 0) return;

    while (sp.cursor > 0 && strchr(worddelimiters, tx.text[nextrune(-1)])) {
        insert(NULL, nextrune(-1) - sp.cursor);
    } while (sp.cursor > 0 && !strchr(worddelimiters, tx.text[nextrune(-1)])) {
        insert(NULL, nextrune(-1) - sp.cursor);
    }

    drawmenu();
}

void moveword(Arg *arg) {
    if (arg->i < 0) { // move sp.cursor to the start of the word
        while (sp.cursor > 0 && strchr(worddelimiters, tx.text[nextrune(-1)])) {
            sp.cursor = nextrune(-1);
        } while (sp.cursor > 0 && !strchr(worddelimiters, tx.text[nextrune(-1)])) {
            sp.cursor = nextrune(-1);
        }
    } else { // move sp.cursor to the end of the word
        while (tx.text[sp.cursor] && strchr(worddelimiters, tx.text[sp.cursor])) {
            sp.cursor = nextrune(+1);
        } while (tx.text[sp.cursor] && !strchr(worddelimiters, tx.text[sp.cursor])) {
            sp.cursor = nextrune(+1);
        }
    }

    drawmenu();
}

void movecursor(Arg *arg) {
    if (arg->i < 0) {
        if (sp.cursor > 0) {
            sp.cursor = nextrune(-1);
        }
    } else {
        if (tx.text[sp.cursor]) {
            sp.cursor = nextrune(+1);
        }
    }

    drawmenu();
}

void backspace(Arg *arg) {
    if (sp.cursor == 0)
        return;

    insert(NULL, nextrune(-1) - sp.cursor);
    drawmenu();
}

void markitem(Arg *arg) {
    if (!mark) return;
    if (sel && is_selected(sel->index)) {
        for (int i = 0; i < sel_size; i++) {
            if (sel_index[i] == sel->index) {
                sel_index[i] = -1;
            }
        }
    } else {
        for (int i = 0; i < sel_size; i++) {
            if (sel_index[i] == -1) {
                sel_index[i] = sel->index;
                return;
            }
        }

        sel_size++;
        sel_index = realloc(sel_index, (sel_size + 1) * sizeof(int));
        sel_index[sel_size - 1] = sel->index;
    }
}

void selectitem(Arg *arg) {
    char *selection;

    // print index
    if (printindex && sel && arg->i) {
        fprintf(stdout, "%d\n", sel->index);
        cleanup();
        exit(0);
    }

    // selected item or input?
    if (sel && arg->i && !hideitem) {
        selection = sel->text;
    } else {
        selection = tx.text;
    }

    for (int i = 0; i < sel_size; i++) {
        if (sel_index[i] != -1 && (!sel || sel->index != sel_index[i])) {
            puts(items[sel_index[i]].text);
        }
    }

    if (!selection)
        return;

    puts(selection);
    savehistory(selection);

    cleanup();
    exit(0);
}

void navhistory(Arg *arg) {
    navigatehistfile(arg->i);
    drawmenu();
}

void restoresel(Arg *arg) {
    tx.text[sp.cursor] = '\0';
    match();
    drawmenu();
}

void clear(Arg *arg) {
    insert(NULL, 0 - sp.cursor);
    drawmenu();
}

void clearins(Arg *arg) {
    insert(NULL, 0 - sp.cursor);

    sp.mode = 1;
    sp.allowkeys = 0;
    strncpy(tx.modetext, instext, 15);

    calcoffsets();
    drawmenu();
}

void quit(Arg *arg) {
    cleanup();
    exit(0);
}

void savehistory(char *input) {
    unsigned int i;
    FILE *fp;

    if (!histfile ||
            0 == maxhist ||
            0 == strlen(input)) {
        goto out;
    }

    fp = fopen(histfile, "w");
    if (!fp) {
        die("spmenu: failed to open %s", histfile);
    }
    for (i = histsz < maxhist ? 0 : histsz - maxhist; i < histsz; i++) {
        if (0 >= fprintf(fp, "%s\n", history[i])) {
            die("spmenu: failed to write to %s", histfile);
        }
    }
    if (histsz == 0 || histdup || (histsz > 0 && strcmp(input, history[histsz-1]) != 0)) {
        if (0 >= fputs(input, fp)) {
            die("spmenu: failed to write to %s", histfile);
        }
    }
    if (fclose(fp)) {
        die("spmenu: failed to close file %s", histfile);
    }

out:
    for (i = 0; i < histsz; i++) {
        free(history[i]);
    }
    free(history);
}

void setimgsize(Arg *arg) {
#if USEIMAGE
    setimagesize(imagewidth + arg->i, imageheight + arg->i);
    drawmenu();
#endif
}

void flipimg(Arg *arg) {
#if USEIMAGE

    if (!image) return;

    img.flip = img.flip ? 0 : arg->i ? 1 : 2;

    drawmenu();

#endif
}

void setimgpos(Arg *arg) {
#if USEIMAGE
    if (!image || hideimage) return;

    if (imageposition < 3) {
        imageposition += arg->i;
    } else {
        imageposition = 0;
    }

    drawmenu();
#endif
}

void setimggaps(Arg *arg) {
#if USEIMAGE
    imagegaps += arg->i;

    if (!image || hideimage) return;

    if (imagegaps < 0)
        imagegaps = 0;

    // limitation to make sure we have a reasonable gap size
    if (imagegaps > imagewidth / 2)
        imagegaps -= arg->i;

    drawmenu();
#endif
}

void toggleimg(Arg *arg) {
#if USEIMAGE

    hideimage = !hideimage;

    drawmenu();

#endif
}

void togglefullimg(Arg *arg) {
#if USEIMAGE
    fullscreen = image ? !fullscreen : 0;

    if (fullscreen) {
        img.ow = imagewidth;
        img.oh = imageheight;

        imagewidth = sp.mw;
        imageheight = sp.mh;
    } else {
        imagewidth = img.ow;
        imageheight = img.oh;
    }

    drawmenu();
#endif
}

void defaultimg(Arg *arg) {
#if USEIMAGE

    if (hideimage || !image) return;

    if (img.imagew) {
        imagewidth = img.imagew;
        imageheight = img.imageh;
        imagegaps = img.imageg;
    }

    drawmenu();
#endif
}

void setlines(Arg *arg) {
    if (fullscreen || !overridelines) return;

    lines += arg->i;

    if (lines < 0) {
        lines = 0;
    }

    if (lines == 0) {
        match();
    }

    resizeclient();
    calcoffsets();
    drawmenu();
}

void setcolumns(Arg *arg) {
    if (fullscreen || !overridecolumns) return;

    columns += arg->i;

    if (columns < 1) {
        columns = 1;
    }

    if (lines == 0) {
        match();
    }

    resizeclient();
    calcoffsets();
    drawmenu();
}

void spawn(Arg *arg) {
    if (!system(arg->c))
        die("spmenu: failed to execute command '%s'", arg->c);
    else
        exit(0);
}

void togglehighlight(Arg *arg) {
    hidehighlight = !hidehighlight;
    drawmenu();
}

void setprofile(Arg *arg) {
    if (!system("command -v spmenu_profile > /dev/null && spmenu_profile --spmenu-set-profile")) {
        die("spmenu: failed to run profile menu\n");
    } else {
        exit(0);
    }
}

void switchmode(Arg *arg) {
    sp.mode = !sp.mode;

    if (!type) sp.mode = 0; // only normal mode allowed

    sp.allowkeys = !sp.mode;

    strncpy(tx.modetext, sp.mode ? instext : normtext, 15);
    drawmenu();
}
