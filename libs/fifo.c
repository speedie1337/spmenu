/* See LICENSE file for copyright and license details. */

#if FIFO

#include <fcntl.h>
#include <sys/stat.h>

static void init_fifo(void);
static void execute_fifo_cmd(void);
static int done = 1;

void execute_fifo_cmd(void) {
    int fd;
    char fifot[BUFSIZ];

    done = 0;

    fd = open(fifofile, O_RDONLY);
    int r = read(fd, fifot, sizeof(fifot));

    if (!r) {
        close(fd);
        return;
    }

    /* These are the different commands that we can run
     * by outputting text to the FIFO.
     */
    if (!strcmp(fifot, "drawmenu")) {
        drawmenu();
    } else if (!strcmp(fifot, "match")) {
        match();
    } else if (!strcmp(fifot, "update")) {
        match();
        drawmenu();
    } else if (!strcmp(fifot, "test")) {
        fprintf(stderr, "Test print\n");
    } else if (!strcmp(fifot, "die")) {
        close(fd);
        die("FIFO told me to die.");
    } else if (!strcmp(fifot, "loadconfig")) {
#if CONFIG
        conf_init();
#endif
    } else if (!strcmp(fifot, "output")) {
        if (selecteditem) {
            for (int i = 0; i < sel_size; i++) {
                if (sel_index[i] != -1 && (!selecteditem || selecteditem->index != sel_index[i])) {
                    puts(items[sel_index[i]].text);
                }
            }
        }
    } else if (!strcmp(fifot, "output_index")) {
        if (printindex && selecteditem) {
            fprintf(stdout, "%d\n", selecteditem->index);
        }
    } else if (!strcmp(fifot, "toggleinput")) {
        Arg *arg;
        toggleinput(arg);
    } else if (!strcmp(fifot, "togglelarrow")) {
        Arg *arg;
        togglelarrow(arg);
    } else if (!strcmp(fifot, "togglerarrow")) {
        Arg *arg;
        togglerarrow(arg);
    } else if (!strcmp(fifot, "toggleitem")) {
        Arg *arg;
        toggleitem(arg);
    } else if (!strcmp(fifot, "toggleprompt")) {
        Arg *arg;
        toggleprompt(arg);
    } else if (!strcmp(fifot, "togglecaps")) {
        Arg *arg;
        togglecaps(arg);
    } else if (!strcmp(fifot, "togglepowerline")) {
        Arg *arg;
        togglepowerline(arg);
    } else if (!strcmp(fifot, "togglecaret")) {
        Arg *arg;
        togglecaret(arg);
    } else if (!strcmp(fifot, "togglehighlight")) {
        Arg *arg;
        togglehighlight(arg);
    } else if (!strcmp(fifot, "togglematchcount")) {
        Arg *arg;
        togglematchcount(arg);
    } else if (!strcmp(fifot, "togglemode")) {
        Arg *arg;
        togglemode(arg);
    } else if (!strcmp(fifot, "toggleregex")) {
        Arg *arg;
        toggleregex(arg);
    } else if (!strcmp(fifot, "togglefuzzy")) {
        Arg *arg;
        togglefuzzy(arg);
    } else if (!strcmp(fifot, "toggleimg")) {
        Arg *arg;
        toggleimg(arg);
    } else if (!strcmp(fifot, "toggleimgtype")) {
        Arg *arg;
        toggleimgtype(arg);
    } else if (!strcmp(fifot, "exit_0")) {
        exit(0);
    } else if (!strcmp(fifot, "exit_1")) {
        exit(1);
    } else {
        fprintf(stderr, "spmenu: Unknown command: %s\n", fifot);
    }

    close(fd);

    done = 1;
}

void *fifocmd(void *n) {
    for (;;) {
        msleep(0.1);

        if (done) {
            execute_fifo_cmd();
        }
    }
}

void init_fifo(void) {
    remove(fifofile);
    mkfifo(fifofile, 0666);

    pthread_t tid;
    pthread_create(&tid, NULL, &fifocmd, NULL);
}
#endif
