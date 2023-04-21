void
readstdin(void)
{
	char buf[sizeof text], *p;
	size_t i, imax = 0, itemsiz = 0;
	unsigned int tmpmax = 0;
    #if USEIMAGE
    int w, h;
    char *limg = NULL;
    #endif

	if (passwd){
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
