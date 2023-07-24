/* See LICENSE file for copyright and license details. */
void moveleft(Arg *arg) {
    struct item *tmpsel;
    int i, offscreen = 0;
    int argu = arg->i ? arg->i : 1;

    // If we cannot move left because !lines, moving left should move to the next item. Calling moveup() does this.
    if (!lines) {
        moveup(arg);
        return;
    }

    if (columns > 1) {
        if (!selecteditem) {
            return;
        }

        tmpsel = selecteditem;
        for (i = 0; i < lines; i++) {
            if (!tmpsel->left || tmpsel->left->right != tmpsel) {
                if (offscreen)
                    drawmenu();
                return;
            }
            if (tmpsel == currentitem)
                offscreen = 1;
            tmpsel = tmpsel->left;
        }
        selecteditem = tmpsel;
        if (offscreen) {
            for (int j = 0; j < argu; j++) {
                currentitem = previousitem;
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

    if (!lines) { // If we cannot move right because !lines, moving right should move to the previous item. Calling down() does this.
        movedown(arg);
        return;
    }

    if (columns > 1) {
        if (!selecteditem)
            return;
        tmpsel = selecteditem;
        for (i = 0; i < lines; i++) {
            if (!tmpsel->right ||  tmpsel->right->left != tmpsel) {
                if (offscreen)
                    drawmenu();
                return;
            }
            tmpsel = tmpsel->right;
            if (tmpsel == nextitem)
                offscreen = 1;
        }
        selecteditem = tmpsel;
        if (offscreen) {
            for (int j = 0; j < argu; j++)
                currentitem = nextitem;
        }
        calcoffsets();
    }

    drawmenu();
}

void movedown(Arg *arg) {
    int argu = arg->i ? arg->i : 1;

    for (int j = 0; j < argu; j++) {
        if (selecteditem && selecteditem->right && (selecteditem = selecteditem->right) == nextitem) {
            currentitem = nextitem; // Current item is now the next item
        }
    }

    calcoffsets();
    drawmenu();
}

void moveup(Arg *arg) {
    int argu = arg->i ? arg->i : 1;

    for (int j = 0; j < argu; j++) {
        if (selecteditem && selecteditem->left && (selecteditem = selecteditem->left)->right == currentitem) {
            currentitem = previousitem; // Current item is now the previous item
        }
    }

    calcoffsets();
    drawmenu();
}

void complete(Arg *arg) {
    int itc = 0;
    struct item *item;

    if (hideitem) return;

    if (matchend) {
        itc++;

        for (item = matchend; item && item->left; item = item->left)
            itc++;
    }

    if (!itc) {
        return;
    }

    strncpy(tx.text, selecteditem->nsgrtext, sizeof tx.text - 1);
    tx.text[sizeof tx.text - 1] = '\0';
    sp.cursor = strlen(tx.text);

    match();
    drawmenu();
}

void movenext(Arg *arg) {
    if (!nextitem) {
        return;
    }

    selecteditem = currentitem = nextitem; // next page
    drawmenu();
}

void moveprev(Arg *arg) {
    if (!previousitem) {
        return;
    }

    selecteditem = currentitem = previousitem; // previous page
    calcoffsets();
    drawmenu();
}

void moveitem(Arg *arg) {
    for (int i = 0; i < arg->i; i++) {
        if (selecteditem && selecteditem->right && (selecteditem = selecteditem->right) == nextitem) {
            currentitem = nextitem;
            calcoffsets();
        }
    }

    drawmenu();
}

void movestart(Arg *arg) {
    if (selecteditem == matches) {
        sp.cursor = 0;
        drawmenu();
        return;
    }

    selecteditem = currentitem = matches;
    calcoffsets();
    drawmenu();
}

void moveend(Arg *arg) {
    if (tx.text[sp.cursor] != '\0') {
        sp.cursor = strlen(tx.text);
        drawmenu();
        return;
    }

    if (nextitem) {
        currentitem = matchend;
        calcoffsets();
        currentitem = previousitem;
        calcoffsets();

        while (nextitem && (currentitem = currentitem->right))
            calcoffsets();
    }

    selecteditem = matchend;
    drawmenu();
}

void paste(Arg *arg) {
#if USEWAYLAND
    if (protocol) {
        paste_wl();
    } else {
#if USEX
        paste_x11(arg->i);
#endif
    }
#elif USEX
    paste_x11(arg->i);
#endif
}

void viewhist(Arg *arg) {
    int i;

    if (histfile) {
        if (!history_items) {
            history_items = items;
            items = calloc(histsz + 1, sizeof(struct item));

            if (!items) {
                die("spmenu: cannot allocate memory");
            }

            for (i = 0; i < histsz; i++) {
                items[i].text = history[i];
            }
        } else {
            free(items);
            items = history_items;
            history_items = NULL;
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
    if (selecteditem && is_selected(selecteditem->index)) {
        for (int i = 0; i < sel_size; i++) {
            if (sel_index[i] == selecteditem->index) {
                sel_index[i] = -1;
            }
        }
    } else {
        for (int i = 0; i < sel_size; i++) {
            if (sel_index[i] == -1) {
                sel_index[i] = selecteditem->index;
                return;
            }
        }

        sel_size++;
        sel_index = realloc(sel_index, (sel_size + 1) * sizeof(int));
        sel_index[sel_size - 1] = selecteditem->index;
    }
}

void selectitem(Arg *arg) {
    char *selection;

    // print index
    if (printindex && selecteditem && arg->i) {
        fprintf(stdout, "%d\n", selecteditem->index);
        cleanup();
        exit(0);
    }

    // selected item or input?
    if (selecteditem && arg->i && !hideitem) {
        selection = selecteditem->text;
    } else {
        selection = tx.text;
    }

    for (int i = 0; i < sel_size; i++) {
        if (sel_index[i] != -1 && (!selecteditem || selecteditem->index != sel_index[i])) {
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

void setlineheight(Arg *arg) {
    lineheight += arg->i;
    sp.bh = MAX(draw->font->h, draw->font->h + 2 + lineheight);
    resizeclient();
}

void setimgsize(Arg *arg) {
#if USEIMAGE
    setimagesize(img.imagewidth + arg->i, img.imageheight + arg->i);
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
    img.imagegaps += arg->i;

    if (img.imagegaps < 0)
        img.imagegaps = 0;

    // limitation to make sure we have a reasonable gap size
    if (img.imagegaps > (sp.mw - 2 * img.imagegaps) / 3)
        img.imagegaps -= arg->i;

    drawmenu();
#endif
}

void toggleinput(Arg *arg) {
    hideinput = !hideinput;
    drawmenu();
}

void togglelarrow(Arg *arg) {
    hidelarrow = !hidelarrow;
    drawmenu();
}

void togglerarrow(Arg *arg) {
    hiderarrow = !hiderarrow;
    drawmenu();
}

void toggleitem(Arg *arg) {
    hideitem = !hideitem;
    drawmenu();
}

void toggleprompt(Arg *arg) {
    hideprompt = !hideprompt;
    drawmenu();
}

void togglecaps(Arg *arg) {
    hidecaps = !hidecaps;
    drawmenu();
}

void togglepowerline(Arg *arg) {
    hidepowerline = !hidepowerline;
    drawmenu();
}

void togglecaret(Arg *arg) {
    hidecaret = !hidecaret;
    drawmenu();
}

void togglematchcount(Arg *arg) {
    hidematchcount = !hidematchcount;
    drawmenu();
}

void togglemode(Arg *arg) {
    hidemode = !hidemode;
    drawmenu();
}

void togglehighlight(Arg *arg) {
    hidehighlight = !hidehighlight;
    drawmenu();
}

void toggleregex(Arg *arg) {
#if USEREGEX
    regex = !regex;

    match();
    drawmenu();

#endif
}

void togglefuzzy(Arg *arg) {
    fuzzy = !fuzzy;

    match();
    drawmenu();
}

void toggleimg(Arg *arg) {
#if USEIMAGE

    hideimage = !hideimage;

    drawmenu();

#endif
}

void defaultimg(Arg *arg) {
#if USEIMAGE

    if (hideimage || !image) return;

    img.imagewidth = imagewidth;
    img.imageheight = imageheight;
    img.imagegaps = imagegaps;

    drawmenu();
#endif
}

void setlines(Arg *arg) {
    if (!overridelines) return;

    insert(NULL, 0 - sp.cursor);
    selecteditem = currentitem = matches;

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
    if (!overridecolumns) return;

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

void setprofile(Arg *arg) {
    if (!system("command -v spmenu_profile > /dev/null && spmenu_profile --spmenu-set-profile")) {
        die("spmenu: failed to run profile menu\n");
    } else {
        exit(0);
    }
}

void switchmode(Arg *arg) {
    if (sp.forceinsertmode) {
        return;
    }

    sp.mode = !sp.mode;

    if (!type) sp.mode = 0; // only normal mode allowed

    sp.allowkeys = !sp.mode;

    strncpy(tx.modetext, sp.mode ? instext : normtext, 15);
    drawmenu();
}

/* This function is basically a copy of the selectitem function.
 * The only difference is "selectitem" was replaced with "mouseitem" and tx.text output
 * was removed.
 */
void outputhover(Arg *arg) {
    char *selection;

    if (printindex && mouseitem && arg->i) {
        fprintf(stdout, "%d\n", mouseitem->index);
        cleanup();
        exit(0);
    }

    selection = mouseitem->text;

    for (int i = 0; i < sel_size; i++) {
        if (sel_index[i] != -1 && (!mouseitem || mouseitem->index != sel_index[i])) {
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

void selecthover(Arg *arg) {
    if (selecteditem != mouseitem) {
        selecteditem = mouseitem;
    } else {
        selecteditem = mouseitem;
        outputhover(arg);

        return;
    }

    drawmenu();
}

void markhover(Arg *arg) {
    if (!mark) return;
    if (mouseitem && is_selected(mouseitem->index)) {
        for (int i = 0; i < sel_size; i++) {
            if (sel_index[i] == mouseitem->index) {
                sel_index[i] = -1;
            }
        }
    } else {
        for (int i = 0; i < sel_size; i++) {
            if (sel_index[i] == -1) {
                sel_index[i] = mouseitem->index;
                return;
            }
        }

        sel_size++;
        sel_index = realloc(sel_index, (sel_size + 1) * sizeof(int));
        sel_index[sel_size - 1] = mouseitem->index;
    }

    drawmenu();
}

void screenshot(Arg *arg) {
    char *file = NULL;
    char *home = NULL;
    time_t time_ = time(NULL);
    struct tm t = *localtime(&time_);

    if (!screenshotfile) {
        if (!(home = getenv("HOME"))) {
            fprintf(stderr, "spmenu: failed to determine home directory\n");
            return;
        }

        if (!screenshotdir && !screenshotname) { // default
            if (!(file = malloc(snprintf(NULL, 0, "%s/%s-%02d-%02d-%02d%s", home, "spmenu-screenshot", t.tm_hour, t.tm_min, t.tm_sec, ".png") + 1))) {
                die("spmenu: failed to malloc screenshot file");
            }

            sprintf(file, "%s/%s-%02d-%02d-%02d%s", home, "spmenu-screenshot", t.tm_hour, t.tm_min, t.tm_sec, ".png");
        } else if (!screenshotdir && screenshotname) { // no dir but name
            if (!(file = malloc(snprintf(NULL, 0, "%s/%s", home, screenshotname) + 1))) {
                die("spmenu: failed to malloc screenshot file");
            }

            sprintf(file, "%s/%s", home, screenshotname);
        } else if (screenshotdir && !screenshotname) { // dir but no name
            if (!(file = malloc(snprintf(NULL, 0, "%s/%s-%02d-%02d-%02d%s", screenshotdir, "spmenu-screenshot", t.tm_hour, t.tm_min, t.tm_sec, ".png") + 1))) {
                die("spmenu: failed to malloc screenshot file");
            }

            sprintf(file, "%s/%s-%02d-%02d-%02d%s", screenshotdir, "spmenu-screenshot", t.tm_hour, t.tm_min, t.tm_sec, ".png");
        } else { // dir and name
            if (!(file = malloc(snprintf(NULL, 0, "%s/%s", screenshotdir, screenshotname) + 1))) {
                die("spmenu: failed to malloc screenshot file");
            }

            sprintf(file, "%s/%s", screenshotdir, screenshotname);
        }
    } else { // custom file
        if (!(file = malloc(snprintf(NULL, 0, "%s", screenshotfile) + 1))) {
            die("spmenu: failed to malloc screenshot file");
        }

        sprintf(file, "%s", screenshotfile);
    }

    draw_save_screen(draw, file);
}
