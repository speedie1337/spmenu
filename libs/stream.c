void readstdin(void) {
	char buf[sizeof text], *p;
	size_t i, imax = 0, itemsiz = 0;
	unsigned int tmpmax = 0;

	if (passwd) {
    	inputw = lines = 0;
    	return;
  	}

    int o = 0;

	// read each line from stdin and add it to the item list
	for (i = 0; fgets(buf, sizeof buf, stdin); i++) {
      	if (i + 1 >= itemsiz) {
			itemsiz += 256;
			if (!(items = realloc(items, itemsiz * sizeof(*items))))
				die("spmenu: cannot realloc %zu bytes:", itemsiz * sizeof(*items));
		}
		if ((p = strchr(buf, '\n')))
			*p = '\0';
		if (!(items[i].text = strdup(buf)))
			die("spmenu: cannot strdup %u bytes:", strlen(buf) + 1);
        items[i].hp = arrayhas(hpitems, hplength, items[i].text);
		drw_font_getexts(drw->font, buf, strlen(buf), &tmpmax, NULL, True);
		if (tmpmax > inputw) {
			inputw = tmpmax;
			imax = i;
		}

        items[i].index = i;

        if (parsemarkup(i)) {
            o = 1;
        }
	}

    if (!o) longestedge = imagegaps = 0;

    // clean
	if (items) {
		items[i].text = NULL;
        items[i].image = NULL;
    }

	inputw = items ? TEXTWM(items[imax].text) : 0;
	lines = MIN(lines, i);
}

void readfile(void) {
    if (passwd){
    	inputw = lines = 0;
    	return;
  	}

	size_t len;
    static size_t cap = 0;
    char *l;

    FILE *ef = fopen(listfile, "r");

    if (!ef) return;

    items = NULL;
    listsize = 0;

    for (;;) {
        l = NULL;
        len = 0;

        if (-1 == getline(&l, &len, ef)) {
            if (ferror(ef)) die("spmenu: failed to read file\n");
            free(l);
            break;
        }

        if (cap == listsize) {
            cap += 64 * sizeof(char*);
            list = realloc(list, cap);
            if (!list) die("spmenu: failed to realloc memory");
        }

        strtok(l, "\n");
        list[listsize] = l;
        listsize++;
    }

    if (fclose(ef)) {
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

            if (parsemarkup(i))
                o = 1;
        }

        inputw = items ? TEXTWM(items[i].text) : 0;
	    lines = i;

        if (!o) longestedge = imagegaps = 0;

        if (i == olistcount) {
            listcount = i;
            listchanged = 0;
        } else {
            olistcount = listcount;
            listcount = i;
            listchanged = 1;

            resizeclient();
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
        if(!strncmp("IMG:", items[index].text, strlen("IMG:"))) {
            if(!(items[index].image = malloc(strlen(items[index].text)+1)))
                fprintf(stderr, "spmenu: cannot malloc %lu bytes\n", strlen(items[index].text));
            if(sscanf(items[index].text, "IMG:%[^\t]", items[index].image)) {
                items[index].text += strlen("IMG:")+strlen(items[index].image)+1;
            } else {
                free(items[index].image);
                items[index].image = NULL;
            }
        } else {
            items[index].image = NULL;
        }

        // load image cache (or generate)
        if (generatecache && longestedge <= maxcache && items[index].image && strcmp(items[index].image, limg ? limg : "")) {
            loadimagecache(items[index].image, &w, &h);
        }

        if(items[index].image) {
            limg = items[index].image;
        }
        #else // remove the data, just for convenience
        char *data;

        if(!strncmp("IMG:", items[index].text, strlen("IMG:"))) {
            if(!(data = malloc(strlen(items[index].text)+1)))
                fprintf(stderr, "spmenu: cannot malloc %lu bytes\n", strlen(items[index].text));
            if(sscanf(items[index].text, "IMG:%[^\t]", data)) {
                items[index].text += strlen("IMG:")+strlen(data)+1;
            }
        }
        #endif

        /* TODO: use this for something
         * current usage is not very useful, however it's here to be used later.
         */
        if(!(items[index].ex = malloc(strlen(items[index].text)+1)))
                fprintf(stderr, "spmenu: cannot malloc %lu bytes\n", strlen(items[index].text));
        if (!strncmp("spmenu:", items[index].text, strlen("spmenu:"))) {
            if (sscanf(items[index].text, "spmenu:%[^\t]", items[index].ex)) {
                items[index].text += strlen("spmenu:")+strlen(items[index].ex)+1;
            }

            // spmenu:version
            if (!strncmp("version", items[index].ex, strlen("version"))) {
                fprintf(stdout, "spmenu version %s", VERSION);
                exit(0);
            }

            // spmenu:license
            if (!strncmp("license", items[index].ex, strlen("license"))) {
                fprintf(stdout, "spmenu is licensed under the MIT license. See the included LICENSE file for more information.");
                exit(0);
            }

            // spmenu:test
            if (!strncmp("test", items[index].ex, strlen("test"))) {
                int i = system("command -v spmenu_test > /dev/null && spmenu_test");
                if (i||!i) exit(i);
            }
        }

        if (limg) {
            return 1;
        } else {
            return 0;
        }
}
