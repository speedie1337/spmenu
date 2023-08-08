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
    mkfifo(fifofile, 0666);

    pthread_t tid;
    pthread_create(&tid, NULL, &fifocmd, NULL);
}
#endif
