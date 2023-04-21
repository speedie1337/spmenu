void
moveleft(const Arg *arg)
{
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

	if (cursor > 0 && (!sel || !sel->left || lines > 0)) {
		cursor = nextrune(-1);
		drawmenu();
	}
}

void
moveright(const Arg *arg)
{
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

	if (text[cursor] != '\0') {
		cursor = nextrune(+1);
		drawmenu();
	}
}

void
movedown(const Arg *arg)
{

    int argu = arg->i ? arg->i : 1;

    for (int j = 0; j < argu; j++) {
       	if (sel && sel->right && (sel = sel->right) == next) {
			curr = next;
		}
    }

    calcoffsets();
	drawmenu();
}

void
moveup(const Arg *arg)
{
    int argu = arg->i ? arg->i : 1;

    for (int j = 0; j < argu; j++) {
        if (sel && sel->left && (sel = sel->left)->right == curr) {
			curr = prev;
		}
    }

	calcoffsets();
    drawmenu();
}

void
complete(const Arg *arg)
{
    if (hideitem) return;
	strncpy(text, sel->clntext, sizeof text - 1);
	text[sizeof text - 1] = '\0';
	cursor = strlen(text);
	match();
    drawmenu();
}

void
movenext(const Arg *arg)
{
	if (!next)
		return;

	sel = curr = next;
	calcoffsets();
    drawmenu();
}

void
moveprev(const Arg *arg)
{
    if (!prev)
        return;

    sel = curr = prev;
    calcoffsets();
    drawmenu();
}

void
movestart(const Arg *arg)
{
  	if (sel == matches) {
		cursor = 0;
        drawmenu();
        return;
	}

	sel = curr = matches;
	calcoffsets();
    drawmenu();
}

void
moveend(const Arg *arg)
{
    if (text[cursor] != '\0') {
		cursor = strlen(text);
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

void
paste(const Arg *arg)
{
    int clipboard;

    if (arg->i == 1) {
        clipboard = XA_PRIMARY;
    } else {
        clipboard = clip;
    }

	XConvertSelection(dpy, clipboard, utf8, utf8, win, CurrentTime);
	return;

}

void
viewhist(const Arg *arg)
{
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

void
deleteword(const Arg *arg)
{
    if (cursor == 0) return;

	while (cursor > 0 && strchr(worddelimiters, text[nextrune(-1)])) {
        insert(NULL, nextrune(-1) - cursor);
    } while (cursor > 0 && !strchr(worddelimiters, text[nextrune(-1)])) {
        insert(NULL, nextrune(-1) - cursor);
    }

    drawmenu();
}

void
moveword(const Arg *arg)
{
    if (arg->i < 0) { // move cursor to the start of the word
		while (cursor > 0 && strchr(worddelimiters, text[nextrune(-1)])) {
			cursor = nextrune(-1);
        } while (cursor > 0 && !strchr(worddelimiters, text[nextrune(-1)])) {
			cursor = nextrune(-1);
        }
	} else { // move cursor to the end of the word
		while (text[cursor] && strchr(worddelimiters, text[cursor])) {
			cursor = nextrune(+1);
        } while (text[cursor] && !strchr(worddelimiters, text[cursor])) {
			cursor = nextrune(+1);
        }
	}

    drawmenu();
}

void
movecursor(const Arg *arg)
{
    if (arg->i < 0) {
		if (cursor > 0) {
			cursor = nextrune(-1);
        }
	} else {
		if (text[cursor]) {
			cursor = nextrune(+1);
        }
	}

    drawmenu();
}

void
backspace(const Arg *arg)
{
    if (cursor == 0)
        return;

    insert(NULL, nextrune(-1) - cursor);
    drawmenu();
}

void
selectitem(const Arg *arg)
{
    char *selection;

    if (sel && arg->i && !hideitem) {
        selection = sel->text;
    } else {
        selection = text;
    }

    if (!selection)
        return;

    puts(selection);
    savehistory(selection);

	cleanup();
	exit(0);
}

void
navhistory(const Arg *arg)
{
    navigatehistfile(arg->i);
    drawmenu();
}

void
restoresel(const Arg *arg)
{
    text[cursor] = '\0';
    match();
    drawmenu();
}

void
clear(const Arg *arg)
{
    insert(NULL, 0 - cursor);
    drawmenu();
}

void
clearins(const Arg *arg)
{
    insert(NULL, 0 - cursor);

    curMode = 1;
    allowkeys = 0;

    if (!curMode) {
        strcpy(modetext, normtext);
    } else {
        strcpy(modetext, instext);
    }

    if (hidemode) strcpy(modetext, "");

    calcoffsets();
    drawmenu();
}

void
quit(const Arg *arg)
{
	cleanup();
	exit(1);
}

void
savehistory(char *input)
{
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

void
setimgsize(const Arg *arg)
{
    #if USEIMAGE
    setimagesize(imagewidth + arg->i, imageheight + arg->i);
    #endif
}

void
flipimg(const Arg *arg)
{
    #if USEIMAGE

    if (!image) return;

    flip = flip ? 0 : arg->i ? 1 : 2;

    drawmenu();

    #endif
}

void
setimgpos(const Arg *arg)
{
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

void
setimggaps(const Arg *arg)
{
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

void
rotateimg(const Arg *arg)
{
    #if USEIMAGE

    if (!image || hideimage) return;

    rotation += arg->i ? arg->i : 1;

    drawmenu();
    #endif
}

void
toggleimg(const Arg *arg)
{
    #if USEIMAGE

    hideimage = !hideimage;

    drawmenu();

    #endif
}

void
defaultimg(const Arg *arg)
{
    #if USEIMAGE

    if (hideimage || !image) return;

    if (imagew) {
        imagewidth = imagew;
        imageheight = imageh;
        imagegaps = imageg;
    }

    drawmenu();
    #endif
}

void
setlines(const Arg *arg)
{
    lines += arg->i;
    if (lines < 0) lines = 0;

    match();
    resizeclient();
    drawmenu();
}

void
setcolumns(const Arg *arg)
{
    columns += arg->i;
    if (columns < 1) columns = 1;

    match();
    resizeclient();
    drawmenu();
}

void
setprofile(const Arg *arg)
{
    // this just runs an external shell script to set the profile
    exit(system("command -v spmenu_profile > /dev/null && spmenu_profile --spmenu-set-profile > /dev/null"));
}

void
spawn(const Arg *arg)
{
    struct sigaction sa;

    if (fork() == 0) {
        if (dpy)
            close(ConnectionNumber(dpy));

        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sa.sa_handler = SIG_DFL;
        sigaction(SIGCHLD, &sa, NULL);

        setsid();
        execvp(((char **)arg->v)[1], ((char **)arg->v)+1);
        fprintf(stderr, "spmenu: failed to execute command '%s'", ((char **)arg->v)[0]);
    }
}
