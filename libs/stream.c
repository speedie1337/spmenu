/* See LICENSE file for copyright and license details. */

void readstdin(void) {
	char *line = NULL;
	size_t i, junk, size = 0;
	ssize_t len;
#if USEIMAGE
    int oneitem = 0;
#endif

    if (passwd) { // -P: No items should be displayed
        sp.inputw = lines = 0;
        return;
    }

    if (listfile) { // -lf: List file is used so no need to read stdin
        readfile();
        return;
    }

	/* read each line from stdin and add it to the item list */
	for (i = 0; (len = getline(&line, &junk, stdin)) != -1; i++, line = NULL) {
		if (i + 1 >= size / sizeof *items)
			if (!(items = realloc(items, (size += BUFSIZ))))
				die("cannot realloc %zu bytes:", size);
		if (line[len - 1] == '\n')
			line[len - 1] = '\0';

		items[i].text = line;
        items[i].index = i;
        items[i].hp = arrayhas(hpitems, hplength, items[i].text);

        if (parsemarkup(i)) {
#if USEIMAGE
            oneitem = 1;
#endif
        }

	}
    free(line);

	if (items) {
		items[i].text = NULL;
#if USEIMAGE
        items[i].image = NULL;
#endif
    }

#if USEIMAGE
    if (!oneitem) {
        img.longestedge = img.imagegaps = 0;
    }
#endif

	lines = MIN(lines, i);
}

void readfile(void) {
    if (passwd){
        sp.inputw = lines = 0;
        return;
    }

    size_t len;
    static size_t c = 0;
    char *l;

    FILE *lf = fopen(listfile, "r");

    if (!lf) return;

    items = NULL;
    listsize = 0;

    for (;;) {
        l = NULL;
        len = 0;

        if (-1 == getline(&l, &len, lf)) {
            if (ferror(lf)) die("spmenu: failed to read file\n");
            free(l);
            break;
        }

        if (c == listsize) {
            c += 64 * sizeof(char*);
            list = realloc(list, c);
            if (!list) die("spmenu: failed to realloc memory");
        }

        strtok(l, "\n");
        list[listsize] = l;
        listsize++;
    }

    if (fclose(lf)) {
        die("spmenu: failed to close file %s\n", listfile);
    }

    if (!list_items) {
        list_items = items;
        items = calloc(listsize + 1, sizeof(struct item));
        if (!items) die("spmenu: cannot alloc memory\n");

        int i = 0;
        int o = 0;

        for (i = 0; i < listsize; i++) {
            items[i].text = list[i];

            if (parsemarkup(i)) {
                o = 1;
            }

#if !USEIMAGE
            if (o) {
                ;
            }
#endif
        }

        lines = columns == 1 ? i : MIN(i, lines); // i = number of items

#if USEIMAGE
        if (!o) img.longestedge = img.imagegaps = 0;
#endif

        if (i == sp.listcount) {
            sp.listchanged = 0;
            sp.listcount = i;
        } else {
            sp.listcount = i;
            sp.listchanged = 1;

            // prevents state->buffer from being NULL
            if (!protocol) {
                resizeclient();
            }
        }
    } else {
        free(items);
        items = list_items;
        list_items = NULL;
    }
}

int parsemarkup(int index) {
#if USEIMAGE
    int w, h;
    char *limg = NULL;
#endif

    // parse image markup
#if USEIMAGE
    if (!strncmp("IMG:", items[index].text, strlen("IMG:")) || !strncmp("img://", items[index].text, strlen("img://"))) {
        if(!(items[index].image = malloc(strlen(items[index].text)+1)))
            fprintf(stderr, "spmenu: cannot malloc %lu bytes\n", strlen(items[index].text));
        if (sscanf(items[index].text, "IMG:%[^\t]", items[index].image)) {
            items[index].text += strlen("IMG:")+strlen(items[index].image)+1;
        } else if (sscanf(items[index].text, "img://%[^\t]", items[index].image)) {
            items[index].text += strlen("img://")+strlen(items[index].image)+1;
        } else {
            free(items[index].image);
            items[index].image = NULL;
        }
    } else {
        items[index].image = NULL;
    }

    // load image cache (or generate)
    if (generatecache && img.longestedge <= maxcache && items[index].image && strcmp(items[index].image, limg ? limg : "")) {
        loadimagecache(items[index].image, &w, &h);
    }

    if(items[index].image) {
        limg = items[index].image;
    }

    if (limg) {
        return 1;
    } else {
        return 0;
    }
#else // remove the data, just for convenience
    char *data;

    if (!strncmp("IMG:", items[index].text, strlen("IMG:")) || !strncmp("img://", items[index].text, strlen("img://"))) {
        if(!(data = malloc(strlen(items[index].text)+1)))
            fprintf(stderr, "spmenu: cannot malloc %lu bytes\n", strlen(items[index].text));
        if(sscanf(items[index].text, "IMG:%[^\t]", data)) {
            items[index].text += strlen("IMG:")+strlen(data)+1;
        } else if(sscanf(items[index].text, "img://%[^\t]", data)) {
            items[index].text += strlen("img://")+strlen(data)+1;
        }
    }

    return 0;
#endif
}
