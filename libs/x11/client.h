/* See LICENSE file for copyright and license details. */

static void hexconv(const char *hex, unsigned short *r, unsigned short *g, unsigned short *b);
static void create_window_x11(int x, int y, int w, int h);
static void set_window_x11(void);
static void set_prop_x11(void);
static void resizeclient_x11(void);
static void store_image_vars(void);
static void set_mode(void);
static void xinitvisual(void);
