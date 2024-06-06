/* See LICENSE file for copyright and license details. */

static char **history;
static size_t histsz, histpos;
static void savehistory(char *input);

void loadhistory(void) {
    FILE *fp = NULL;
    static size_t cap = 0;
    size_t llen;
    char *line;

    // no history file, give up like i do with all things in life
    if (!histfile) {
        return;
    }

    // open history file, return if it failed
    fp = fopen(histfile, "r");
    if (!fp) {
        fprintf(stderr, "spmenu: failed to open history file\n");
        return;
    }

    for (;;) {
        line = NULL;
        llen = 0;

        if (-1 == getline(&line, &llen, fp)) {
            if (ferror(fp)) {
                die("spmenu: failed to read history");
            }

            free(line);
            break;
        }

        if (cap == histsz) {
            cap += 64 * sizeof(char*);
            history = realloc(history, cap);
            if (!history) {
                die("spmenu: failed to realloc memory");
            }
        }
        strtok(line, "\n");
        history[histsz] = line;
        histsz++;
    }

    histpos = histsz;

    if (fclose(fp)) {
        die("spmenu: failed to close file %s", histfile);
    }
}

void navigatehistfile(int dir) {
    static char def[BUFSIZ];
    char *p = NULL;
    size_t len = 0;

    if (!history || histpos + 1 == 0)
        return;

    if (histsz == histpos) {
        strncpy(def, tx.text, sizeof(def));
    }

    switch (dir) {
        case 1:
            if (histpos < histsz - 1) {
                p = history[++histpos];
            } else if (histpos == histsz - 1) {
                p = def;
                histpos++;
            }
            break;
        case -1:
            if (histpos > 0) {
                p = history[--histpos];
            }
            break;
    }

    if (p == NULL) {
        return;
    }

    len = MIN(strlen(p), BUFSIZ - 1);
    sp_strncpy(tx.text, p, sizeof(tx.text));
    tx.text[len] = '\0';
    sp.cursor = len;
    match();
}
