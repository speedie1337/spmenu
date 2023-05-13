/* See LICENSE file for copyright and license details. */

static char *histfile;
static char **history;
static size_t histsz, histpos;
static void savehistory(char *input);
