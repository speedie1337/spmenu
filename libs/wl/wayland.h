/* See LICENSE file for copyright and license details. */

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <sys/timerfd.h>
#include <assert.h>
#include <signal.h>
#include <poll.h>
#include <wayland-client.h>
#include <wayland-client-protocol.h>
#include <xkbcommon/xkbcommon.h>
#include "wlr-layer-shell-unstable-v1-client-protocol.h"
#include "xdg-shell-client-protocol.h"
#include "xdg-output-unstable-v1-client-protocol.h"

struct output {
    struct state *state;
    struct wl_output *output;

    int32_t scale;
};

typedef struct {
    unsigned int mode;
    char *modifier;
    xkb_keysym_t keysym;
    void (*func)(Arg *);
    Arg arg;
} WlKey;

typedef struct {
    unsigned int click;
    unsigned int button;
    void (*func)(Arg *arg);
    Arg arg;
} WlMouse;

static WlKey wl_ckeys[256];
static WlMouse wl_cbuttons[256];

#define WL_CtrlShift "CtrlShift"
#define WL_CtrlShiftSuper "CtrlShiftSuper"
#define WL_CtrlSuper "CtrlSuper"
#define WL_CtrlAlt "CtrlAlt"
#define WL_CtrlAltShift "CtrlAltShift"
#define WL_CtrlAltShiftSuper "CtrlAltShiftSuper"
#define WL_CtrlAltSuper "CtrlAltSuper"
#define WL_AltShift "AltShift"
#define WL_Shift "Shift"
#define WL_Ctrl "Ctrl"
#define WL_Alt "Alt"
#define WL_Super "Super"
#define WL_None "None"

#define WL_Left	0x110
#define WL_Right 0x111
#define WL_Middle 0x112
#define WL_Side	0x113
#define WL_Extra 0x114
#define WL_Forward 0x115
#define WL_Back	0x116
#define WL_Task	0x117

static WlKey wlhkeys[1] = { { -1, WL_CtrlAlt, XKB_KEY_Delete, quit, {0} } };

struct state {
    struct output *output;
	char *output_name;

	struct wl_display *display;
	struct wl_compositor *compositor;
	struct wl_shm *shm;
    struct wl_seat *seat;
	struct wl_keyboard *keyboard;
    struct wl_pointer *pointer;
	struct wl_surface *surface;

	struct zwlr_layer_shell_v1 *layer_shell;
	struct zwlr_layer_surface_v1 *layer_surface;

	int width;
	int height;

    /* Taken from wmenu */
    int timer;
    int delay;
    int period;

    struct xkb_context *xkb_context;
	struct xkb_keymap *xkb_keymap;
	struct xkb_state *xkb_state;

    enum wl_keyboard_key_state repeat_key_state;

    xkb_keysym_t repeat_sym;

    void *data;
    struct wl_buffer *buffer;
};

struct state state = { 0 };

static int output_physical_width = 0;
static int output_physical_height = 0;
static int output_width = 0;
static int output_height = 0;

static int mouse_x = 0;
static int mouse_y = 0;

static void zero();
static void resizeclient_wl(struct state *state);
static void output_scale(void *data, struct wl_output *wl_output, int32_t factor);
static void output_name(void *data, struct wl_output *wl_output, const char *name);
static void output_geometry(void *data, struct wl_output *wl_output, int32_t x, int32_t y, int32_t physical_width, int32_t physical_height, int32_t subpixel, const char *make, const char *model, int32_t output_transform);
static void output_mode(void *data, struct wl_output *wl_output, uint32_t flags, int32_t width, int32_t height, int32_t refresh);
static void layer_surface_configure(void *data, struct zwlr_layer_surface_v1 *surface, uint32_t serial, uint32_t width, uint32_t height);
static void layer_surface_closed(void *data, struct zwlr_layer_surface_v1 *surface);
static void draw_sf(struct state *state);
static void global_handler(void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version);
static void seat_capabilities(void *data, struct wl_seat *seat, enum wl_seat_capability caps);
static void surface_enter(void *data, struct wl_surface *surface, struct wl_output *wl_output);
static void keyboard_keymap(void *data, struct wl_keyboard *wl_keyboard, uint32_t format, int32_t fd, uint32_t size);
static void keyboard_key(void *data, struct wl_keyboard *wl_keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t _key_state);
static void keyboard_modifiers(void *data, struct wl_keyboard *keyboard, uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group);
static void keyboard_repeat_info(void *data, struct wl_keyboard *wl_keyboard, int32_t rate, int32_t delay);
static void keypress_wl(struct state *state, enum wl_keyboard_key_state key_state, xkb_keysym_t sym);
static void pointer_button_handler(void *data, struct wl_pointer *pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state);
static void pointer_motion_handler(void *data, struct wl_pointer *pointer, uint32_t time, wl_fixed_t x, wl_fixed_t y);
static void buttonpress_wl(uint32_t button, double ex, double ey);
static int is_correct_modifier(struct state *state, char *modifier);
static int roundtrip(struct state *state);
static int init_disp(struct state *state);
static int connect_display(struct state *state);
static int init_keys(struct state *state);
static int create_layer(struct state *state, char *name);
static int anchor_layer(struct state *state, int position);
static int set_layer_size(struct state *state, int32_t width, int32_t height);
static int set_exclusive_zone(struct state *state, unsigned int val);
static int set_keyboard(struct state *state, int interactivity);
static int add_layer_listener(struct state *state);
static int set_visible_layer(struct state *state);
struct wl_buffer *create_buffer(struct state *state);

/* Set to 0 if the connection was successful
 * You can use this for say, X11 compatibility mode.
 */
static int no_display = 1;

/* Note that you MUST override this before calling create_layer() because
 * otherwise nothing will be drawn. Do not forget to do this.
 */
static int allow_draw = 0;

/* Set automatically */
static int has_keys = 0;

/* See global_handler */
static const struct wl_registry_listener registry_listener = {
    .global = global_handler,
    .global_remove = zero,
};

/* See surface_enter */
static const struct wl_surface_listener surface_listener = {
    .enter = surface_enter,
    .leave = zero,
};

static const struct wl_keyboard_listener keyboard_listener = {
	.keymap = keyboard_keymap,
	.enter = zero,
	.leave = zero,
	.key = keyboard_key,
	.modifiers = keyboard_modifiers,
	.repeat_info = keyboard_repeat_info,
};

static const struct wl_pointer_listener pointer_listener = {
    .enter = zero,
    .leave = zero,
    .motion = pointer_motion_handler,
    .button = pointer_button_handler,
    .axis = zero,
};

struct wl_output_listener output_listener = {
    .geometry = output_geometry,
    .mode = output_mode,
    .done = zero,
    .scale = output_scale,
    .name = output_name,
    .description = zero,
};

struct zwlr_layer_surface_v1_listener layer_surface_listener = {
    .configure = layer_surface_configure,
    .closed = layer_surface_closed,
};

struct wl_seat_listener seat_listener = {
    .capabilities = seat_capabilities,
    .name = zero,
};

static const struct wl_buffer_listener buffer_listener = {
    .release = zero,
};
