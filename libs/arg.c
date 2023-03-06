void
move(const Arg *arg)
{
    struct item *tmpsel;
    int i, offscreen = 0;

    if (arg->i == 3) { /* left */
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
				curr = prev;
				calcoffsets();
			}
			drawmenu();
		}

		if (cursor > 0 && (!sel || !sel->left || lines > 0)) {
			cursor = nextrune(-1);
			drawmenu();
		}
		if (lines > 0)
			return;
    } else if (arg->i == 4) {
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
				curr = next;
				calcoffsets();
			}
			drawmenu();
		}

		if (text[cursor] != '\0') {
			cursor = nextrune(+1);
			drawmenu();
		}

		if (lines > 0)
			return;
    } else if (arg->i == 2) {
       	if (sel && sel->right && (sel = sel->right) == next) {
			curr = next;
			calcoffsets();
		}
		drawmenu();
    } else if (arg->i == 1) {
        if (sel && sel->left && (sel = sel->left)->right == curr) {
			curr = prev;
			calcoffsets();
		}
        drawmenu();
    }
}

void
complete(const Arg *arg)
{
 	if (!sel)
		return;

	strncpy(text, sel->text, sizeof text - 1);
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
moveword(const Arg *arg)
{
    movewordedge(arg->i);
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

    if (sel) {
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
		die("failed to open %s", histfile);
	}
	for (i = histsz < maxhist ? 0 : histsz - maxhist; i < histsz; i++) {
		if (0 >= fprintf(fp, "%s\n", history[i])) {
			die("failed to write to %s", histfile);
		}
	}
	if (histsz == 0 || !histnodup || (histsz > 0 && strcmp(input, history[histsz-1]) != 0)) { /* TODO */
		if (0 >= fputs(input, fp)) {
			die("failed to write to %s", histfile);
		}
	}
	if (fclose(fp)) {
		die("failed to close file %s", histfile);
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
    #if !USEIMAGE
    return;
    #endif

    /* this makes sure we cannot scale down the image too much */
    if ((!image && imageheight + arg->i < imageheight) || hideimage) return;

    cleanupimage();

    imageheight += arg->i;
    imagewidth += arg->i;

    drawimage();

    if (!image) {
        imageheight -= arg->i;
        imagewidth -= arg->i;
        return;
    } else {
        drawimage();
    }

    drawmenu();
}

void
flipimg(const Arg *arg)
{
    #if !USEIMAGE
    return;
    #endif

    if (!image) return;

    flip = flip ? 0 : arg->i ? 1 : 2;

    cleanupimage();
    drawimage();
    drawmenu();
}

void
rotateimg(const Arg *arg)
{
    #if !USEIMAGE
    return;
    #endif

    if (!image) return;

    rotation++;

    cleanupimage();
    drawimage();
    drawmenu();
}

void
toggleimg(const Arg *arg)
{
    #if !USEIMAGE
    return;
    #endif

    cleanupimage();

    hideimage = !hideimage;

    drawmenu();
    drawimage();
}

void
defaultimg(const Arg *arg)
{
    #if !USEIMAGE
    return;
    #endif

    if (hideimage || !image) return;

    if (imagew) {
        imagewidth = imagew;
        imageheight = imageh;
        imagegaps = imageg;
    }

    drawmenu();
    drawimage();
}
