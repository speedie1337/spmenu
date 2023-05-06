void readstdin(void) {
	char buf[sizeof text], *p;
	size_t i, imax = 0, itemsiz = 0;
	unsigned int tmpmax = 0;
    #if USEIMAGE
    int w, h;
    char *limg = NULL;
    #endif

	if (passwd) {
    	inputw = lines = 0;
    	return;
  	}

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

        // parse image markup
        #if USEIMAGE
        if(!strncmp("IMG:", items[i].text, strlen("IMG:"))) {
            if(!(items[i].image = malloc(strlen(items[i].text)+1)))
                fprintf(stderr, "spmenu: cannot malloc %lu bytes\n", strlen(items[i].text));
            if(sscanf(items[i].text, "IMG:%[^\t]", items[i].image)) {
                items[i].text += strlen("IMG:")+strlen(items[i].image)+1;
            } else {
                free(items[i].image);
                items[i].image = NULL;
            }
        } else {
            items[i].image = NULL;
        }

        // load image cache (or generate)
        if (generatecache && longestedge <= 256 && items[i].image && strcmp(items[i].image, limg ? limg : "")) {
            loadimagecache(items[i].image, &w, &h);
        }

        if(items[i].image) {
            limg = items[i].image;
        }
        #else // remove the data, just for convenience
        char *data;

        if(!strncmp("IMG:", items[i].text, strlen("IMG:"))) {
            if(!(data = malloc(strlen(items[i].text)+1)))
                fprintf(stderr, "spmenu: cannot malloc %lu bytes\n", strlen(items[i].text));
            if(sscanf(items[i].text, "IMG:%[^\t]", data)) {
                items[i].text += strlen("IMG:")+strlen(data)+1;
            }
        }
        #endif


        /* TODO: use this for something
         * current usage is not very useful, however it's here to be used later.
         */
        if(!(items[i].ex = malloc(strlen(items[i].text)+1)))
                fprintf(stderr, "spmenu: cannot malloc %lu bytes\n", strlen(items[i].text));
        if (!strncmp("spmenu:", items[i].text, strlen("spmenu:"))) {
            if (sscanf(items[i].text, "spmenu:%[^\t]", items[i].ex)) {
                items[i].text += strlen("spmenu:")+strlen(items[i].ex)+1;
            }

            // spmenu:version
            if (!strncmp("version", items[i].ex, strlen("version"))) {
                fprintf(stdout, "spmenu version %s", VERSION);
                exit(0);
            }

            // spmenu:license
            if (!strncmp("license", items[i].ex, strlen("license"))) {
                fprintf(stdout, "spmenu is licensed under the MIT license. See the included LICENSE file for more information.");
                exit(0);
            }

            // spmenu:test
            if (!strncmp("test", items[i].ex, strlen("test"))) {
                int i = system("command -v spmenu_test > /dev/null && spmenu_test");
                if (i||!i) exit(i);
            }
        }
	}

    // clean
	if (items) {
        #if USEIMAGE
        items[i].image = NULL;
        #endif
		items[i].text = NULL;
    }

    #if USEIMAGE
    if (!limg) longestedge = imagegaps = 0;
    #endif
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
    //list = NULL;
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

        for (i = 0; i < listsize; i++) {
            items[i].text = list[i];
        }

        if (i == olistcount) {
            listcount = i;
            listchanged = 0;
        } else {
            olistcount = listcount;
            listcount = i;
            listchanged = 1;
        }
    } else {
        free(items);
        items = list_items;
        list_items = NULL;
    }

    //match();
    //drawmenu();
}
