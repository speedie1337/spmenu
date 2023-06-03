/* See LICENSE file for copyright and license details. */

#include "shm.c"

struct wl_buffer *create_buffer(struct state *state) {
    int32_t width = state->width;
    int32_t height = state->height;

    uint32_t stride = width * 4;
    size_t siz = stride * height;

    int fd = create_shm_file(siz);
    assert(fd != -1);

    void *data = mmap(NULL, siz, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (data == MAP_FAILED) {
        close(fd);
        return NULL;
    }

	struct wl_shm_pool *pool = wl_shm_create_pool(state->shm, fd, siz);

	struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool, 0,
			width, height, stride, WL_SHM_FORMAT_ARGB8888);

    wl_shm_pool_destroy(pool);

    wl_buffer_add_listener(buffer, &buffer_listener, state);

    state->data = data;

    return buffer;
}

/* I fucking hate this code, but I'm not sure how else to do it.
 *
 * PLEASE submit a patch if you have an improvement.
 */
int is_correct_modifier(struct state *state, char *modifier) {
    int altPress = xkb_state_mod_name_is_active(state->xkb_state, XKB_MOD_NAME_ALT, XKB_STATE_MODS_DEPRESSED | XKB_STATE_MODS_LATCHED);
    int shiftPress = xkb_state_mod_name_is_active(state->xkb_state, XKB_MOD_NAME_SHIFT, XKB_STATE_MODS_DEPRESSED | XKB_STATE_MODS_LATCHED);
    int ctrlPress = xkb_state_mod_name_is_active(state->xkb_state, XKB_MOD_NAME_CTRL, XKB_STATE_MODS_DEPRESSED | XKB_STATE_MODS_LATCHED);
    int logoPress = xkb_state_mod_name_is_active(state->xkb_state, XKB_MOD_NAME_LOGO, XKB_STATE_MODS_DEPRESSED | XKB_STATE_MODS_LATCHED);

    if (!strcmp(modifier, WL_CtrlShift) && shiftPress && ctrlPress) {
        return 0;
    } else if (!strcmp(modifier, WL_CtrlShiftSuper) && shiftPress && ctrlPress && logoPress) {
        return 0;
    } else if (!strcmp(modifier, WL_CtrlSuper) && ctrlPress && logoPress) {
        return 0;
    } else if (!strcmp(modifier, WL_CtrlAlt) && altPress && ctrlPress) {
        return 0;
    } else if (!strcmp(modifier, WL_CtrlAltShift) && ctrlPress && altPress && shiftPress) {
        return 0;
    } else if (!strcmp(modifier, WL_CtrlAltShiftSuper) && ctrlPress && altPress && shiftPress && logoPress) {
        return 0;
    } else if (!strcmp(modifier, WL_CtrlAltSuper) && ctrlPress && altPress && logoPress) {
        return 0;
    } else if (!strcmp(modifier, WL_AltShift) && altPress && shiftPress) {
        return 0;
    } else if (!strcmp(modifier, WL_Shift) && shiftPress) {
        return 0;
    } else if (!strcmp(modifier, WL_Ctrl) && ctrlPress) {
        return 0;
    } else if (!strcmp(modifier, WL_Alt) && altPress) {
        return 0;
    } else if (!strcmp(modifier, WL_Super) && logoPress) {
        return 0;
    } else if (!strcmp(modifier, WL_None) && !altPress && !shiftPress && !ctrlPress && !logoPress) {
        return 0;
    }

    return 1;
}

void keypress_wl(struct state *state, enum wl_keyboard_key_state key_state, xkb_keysym_t sym) {
    int i = 0;
    char buf[8];

    if (key_state != WL_KEYBOARD_KEY_STATE_PRESSED) { // Only activate on press, not release
        return;
    }

    if (xkb_keysym_to_lower(sym) == wlhkeys[0].keysym && !is_correct_modifier(state, wlhkeys[0].modifier) && wlhkeys[0].func) {
        wlhkeys[0].func(&(wlhkeys[0].arg));
    }

    for (i = 0; i < LENGTH(wl_keys); i++) {
        if (ignoreglobalkeys) break;

        if (xkb_keysym_to_lower(sym) == wl_keys[i].keysym && !is_correct_modifier(state, wl_keys[i].modifier) && wl_keys[i].func) {
            if ((wl_keys[i].mode && curMode) || wl_keys[i].mode == -1) {
                wl_keys[i].func(&(wl_keys[i].arg));
                return;
            } else if (!wl_keys[i].mode && !curMode) {
                wl_keys[i].func(&(wl_keys[i].arg));
            } else {
                continue;
            }
        }
    }

    for (i = 0; i < LENGTH(wl_ckeys); i++) {
        if (ignoreconfkeys) break;

        if (xkb_keysym_to_lower(sym) == wl_ckeys[i].keysym && !is_correct_modifier(state, wl_ckeys[i].modifier) && wl_ckeys[i].func) {
            if ((wl_ckeys[i].mode && curMode) || wl_ckeys[i].mode == -1) {
                wl_ckeys[i].func(&(wl_ckeys[i].arg));
                return;
            } else if (!wl_ckeys[i].mode && !curMode) {
                wl_ckeys[i].func(&(wl_ckeys[i].arg));
            } else {
                continue;
            }
        }
    }

    if (xkb_keysym_to_lower(sym) == XKB_KEY_Escape) {
        return;
    }

    if (!type || !curMode) {
        return;
    }

    if (xkb_keysym_to_utf8(sym, buf, 8)) {
        if (allowkeys) {
            insert(buf, strnlen(buf, 8));
        } else {
            allowkeys = !allowkeys;
        }

        drawmenu();
    }
}

void keyboard_modifiers(void *data, struct wl_keyboard *keyboard, uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group) {
	struct state *state = data;
    int ocapslockstate = capslockstate;

	xkb_state_update_mask(state->xkb_state, mods_depressed, mods_latched, mods_locked, 0, 0, group);

    if (xkb_state_mod_name_is_active(state->xkb_state, XKB_MOD_NAME_CAPS, XKB_STATE_MODS_EFFECTIVE)) {
        capslockstate = 1;
    } else {
        capslockstate = 0;
    }

    if (ocapslockstate != capslockstate) {
        strncpy(capstext, capslockstate ? capslockontext : capslockofftext, 15);
        drawmenu();
    }
}

void keyboard_repeat_info(void *data, struct wl_keyboard *wl_keyboard, int32_t rate, int32_t delay) {
	struct state *state = data;

	state->delay = delay;

	if (rate > 0) {
		state->period = 1000 / rate;
	} else {
		state->period = -1;
	}
}

void keyboard_key(void *data, struct wl_keyboard *wl_keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t _key_state) {
	struct state *state = data;

	enum wl_keyboard_key_state key_state = _key_state;

    strncpy(capstext, capslockstate ? capslockontext : capslockofftext, 15);

	xkb_keysym_t sym = xkb_state_key_get_one_sym(state->xkb_state, key + 8);
	keypress_wl(state, key_state, sym);

	if (key_state == WL_KEYBOARD_KEY_STATE_PRESSED && state->period >= 0) {
		state->repeat_key_state = key_state;
		state->repeat_sym = sym;

		struct itimerspec spec = { 0 };

		spec.it_value.tv_sec = state->delay / 1000;
		spec.it_value.tv_nsec = (state->delay % 1000) * 1000000l;

		timerfd_settime(state->timer, 0, &spec, NULL);
	} else if (key_state == WL_KEYBOARD_KEY_STATE_RELEASED) {
		struct itimerspec spec = { 0 };

		timerfd_settime(state->timer, 0, &spec, NULL);
	}
}

void keyboard_keymap(void *data, struct wl_keyboard *wl_keyboard, uint32_t format, int32_t fd, uint32_t size) {
	struct state *state = data;
	if (format != WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1) {
		close(fd);
        exit(1);
		return;
	}
	char *map_shm = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
	if (map_shm == MAP_FAILED) {
		close(fd);
        die("MAP_FAILED");
		return;
	}
	state->xkb_keymap = xkb_keymap_new_from_string(state->xkb_context, map_shm, XKB_KEYMAP_FORMAT_TEXT_V1, 0);
	munmap(map_shm, size);
	close(fd);
	state->xkb_state = xkb_state_new(state->xkb_keymap);
}

void buttonpress_wl(uint32_t button, double ex, double ey) {
    struct item *item;
    int x = 0;
    int y = 0;
    int w;
    int h = bh;
    int xpad = 0;
    int item_num = 0;
    unsigned int i, click;

    if (ex == 0 && ey == 0) {
        return; // While it is possible to click at this position, usually it means we're outside the window area.
    }

    if (!hidepowerline) {
        x = xpad = plw;
    }

    x += menumarginh;

    int larrowWidth = 0;
    int rarrowWidth = 0;
    int numberWidth = 0;
    int modeWidth = 0;
    int capsWidth = 0;

    if (!hidelarrow) larrowWidth = pango_leftarrow ? TEXTWM(leftarrow) : TEXTW(leftarrow);
    if (!hiderarrow) rarrowWidth = pango_rightarrow ? TEXTWM(rightarrow) : TEXTW(rightarrow);
    if (!hidematchcount) numberWidth = pango_numbers ? TEXTWM(numbers) : TEXTW(numbers);
    if (!hidemode) modeWidth = pango_mode ? TEXTWM(modetext) : TEXTW(modetext);
    if (!hidecaps) capsWidth = pango_caps ? TEXTWM(capstext) : TEXTW(capstext);

    if (!strcmp(capstext, "")) capsWidth = 0; // No caps lock width for no chars

    click = ClickWindow; // Used as a default, will be overriden.

    // check click position and override the value of click
    if (ex < x + promptw + powerlineprompt ? plw : 0) { // prompt
        click = ClickPrompt;
    } else if ((ex > mw - capsWidth - 2 * sp - 2 * borderwidth - menumarginh) && !hidecaps && capsWidth) { // caps lock indicator
        click = ClickCaps;
    } else if (ex > mw - modeWidth - capsWidth - 2 * sp - 2 * borderwidth - menumarginh) { // mode indicator
        click = ClickMode;
    } else if (ex > mw - modeWidth - numberWidth - capsWidth - 2 * sp - 2 * borderwidth - menumarginh) { // match count
        click = ClickNumber;
    } else { // input
        w = (lines > 0 || !matches) ? mw - x : inputw;

        if ((lines <= 0 && ex >= 0 && ex <= x + w + promptw +
                    ((!prev || !curr->left) ? larrowWidth : 0)) ||
                (lines > 0 && ey >= y && ey <= y + h)) {

            click = ClickInput;
        }
    }

    // item click
    if (lines > 0) {
        // vertical list
        w = mw - x;
        for (item = curr; item != next; item = item->right) {
            if (item_num++ == lines) {
                item_num = 1;
                x += w / columns;
                y = 0;
            }

            y += h;

            // ClickSelItem, called function doesn't matter
            if (ey >= y && ey <= (y + h) && ex >= x && ex <= (x + w / columns)) {
                for (i = 0; i < LENGTH(wl_buttons); i++) {
                    if (ignoreglobalmouse) break;
                    if (wl_buttons[i].click == ClickSelItem && wl_buttons[i].button == button) {
                        puts(item->text);
                        exit(0);
                    } else if (wl_buttons[i].click == ClickItem) {
                        click = ClickItem;
                    }
                }
                for (i = 0; i < LENGTH(wl_cbuttons); i++) {
                    if (ignoreconfmouse) break;
                    if (wl_cbuttons[i].click == ClickSelItem && wl_cbuttons[i].button == button) {
                        puts(item->text);
                        exit(0);
                    } else if (wl_cbuttons[i].click == ClickItem) {
                        click = ClickItem;
                    }
                }
            }
        }
    } else if (matches) {
        x += inputw;
        w = larrowWidth;
        if (prev && curr->left) {
            if (ex >= x && ex <= x + w) {
                click = ClickLArrow;
            }
        }

        for (item = curr; item != next; item = item->right) {
            x += w;
            w = MIN(TEXTW(item->text), mw - x - rarrowWidth);
            if (ex >= x && ex <= x + w) {
                for (i = 0; i < LENGTH(wl_buttons); i++) {
                    if (ignoreglobalmouse) break;
                    if (wl_buttons[i].click == ClickSelItem && wl_buttons[i].button == button) {
                        puts(item->text);
                        exit(0);
                    } else if (wl_buttons[i].click == ClickItem) {
                        click = ClickItem;
                    }
                }
                for (i = 0; i < LENGTH(wl_cbuttons); i++) {
                    if (ignoreconfmouse) break;
                    if (wl_cbuttons[i].click == ClickSelItem && wl_cbuttons[i].button == button) {
                        puts(item->text);
                        exit(0);
                    } else if (wl_cbuttons[i].click == ClickItem) {
                        click = ClickItem;
                    }
                }

            }
        }
        // right arrow
        w = rarrowWidth;
        x = mw - w;
        if (next && ex >= x && ex <= x + w) {
            click = ClickRArrow;
        }
    }

    // go through mouse button array and run function
    for (i = 0; i < LENGTH(wl_buttons); i++) {
        if (ignoreglobalmouse) break;
        if (click == wl_buttons[i].click && wl_buttons[i].func && wl_buttons[i].button == button)
            wl_buttons[i].func(&wl_buttons[i].arg);
    }

    // go through mouse config array and run function
    for (i = 0; i < LENGTH(wl_cbuttons); i++) {
        if (ignoreconfmouse) break;
        if (click == wl_cbuttons[i].click && wl_cbuttons[i].func && wl_cbuttons[i].button == button)
            wl_cbuttons[i].func(&wl_cbuttons[i].arg);
    }
}

void pointer_motion_handler(void *data, struct wl_pointer *pointer, uint32_t time, wl_fixed_t x, wl_fixed_t y) {
    mouse_x = wl_fixed_to_int(x);
    mouse_y = wl_fixed_to_int(y);
}

void pointer_button_handler(void *data, struct wl_pointer *pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state) {
    if (state != WL_POINTER_BUTTON_STATE_PRESSED) {
        return; // We don't want a release event to count as a click, only the initial press.
    }

    buttonpress_wl(button, mouse_x, mouse_y);
}

void seat_capabilities(void *data, struct wl_seat *seat, enum wl_seat_capability caps) {
    struct state *state = data;

    if (caps & WL_SEAT_CAPABILITY_KEYBOARD) {
        state->keyboard = wl_seat_get_keyboard(seat);
        state->pointer = wl_seat_get_pointer(seat);
        wl_keyboard_add_listener(state->keyboard, &keyboard_listener, state);
        wl_pointer_add_listener(state->pointer, &pointer_listener, state);
    }
}

void output_geometry(void *data, struct wl_output *wl_output, int32_t x, int32_t y, int32_t physical_width, int32_t physical_height, int32_t subpixel, const char *make, const char *model, int32_t output_transform) {
    output_physical_width = physical_width;
    output_physical_height = physical_height;
}

void output_mode(void *data, struct wl_output *wl_output, uint32_t flags, int32_t width, int32_t height, int32_t refresh) {
    output_width = width;
    output_height = height;
}

void output_scale(void *data, struct wl_output *wl_output, int32_t factor) {
	struct output *output = data;

	output->scale = factor;
}

void output_name(void *data, struct wl_output *wl_output, const char *name) {
	struct output *output = data;
	struct state *state = output->state;

	char *outname = state->output_name;

	if (!state->output && outname && strcmp(outname, name) == 0) {
		state->output = output;
	}
}

void layer_surface_configure(void *data, struct zwlr_layer_surface_v1 *surface, uint32_t serial, uint32_t width, uint32_t height) {
	zwlr_layer_surface_v1_ack_configure(surface, serial);
}

void layer_surface_closed(void *data, struct zwlr_layer_surface_v1 *surface) {
    cleanup();
    exit(0);
}

void zero() {
    // Nothing.
}

void draw_sf(struct state *state) {
    if (!allow_draw) { // No drawing if disabled
        return;
    }

    // create buffer to draw on
    state->buffer = create_buffer(state);

    if (drw == NULL) {
        die("spmenu: drw == NULL");
    }

    if (state->buffer == NULL) {
        die("state->buffer == NULL");
    }

    drw_create_surface_wl(drw, state->data, state->width, state->height);

    drawmenu_layer();

    wl_surface_set_buffer_scale(state->surface, 1);
	wl_surface_attach(state->surface, state->buffer, 0, 0);
	wl_surface_damage(state->surface, 0, 0, state->width, state->height);
	wl_surface_commit(state->surface);
}

void global_handler(void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version) {
	struct state *state = data;
	if (strcmp(interface, wl_compositor_interface.name) == 0) {
		state->compositor = wl_registry_bind(registry, name,
				&wl_compositor_interface, 4);
	} else if (strcmp(interface, wl_shm_interface.name) == 0) {
		state->shm = wl_registry_bind(registry, name, &wl_shm_interface, 1);
	} else if (strcmp(interface, wl_seat_interface.name) == 0 && has_keys) {
		struct wl_seat *seat = wl_registry_bind(registry, name,
				&wl_seat_interface, 4);
		wl_seat_add_listener(seat, &seat_listener, state);
	} else if (strcmp(interface, zwlr_layer_shell_v1_interface.name) == 0) {
		state->layer_shell = wl_registry_bind(registry, name,
				&zwlr_layer_shell_v1_interface, 1);
	} else if (strcmp(interface, wl_output_interface.name) == 0) {
		struct output *output = calloc(1, sizeof(struct output));
		output->output = wl_registry_bind(registry, name,
				&wl_output_interface, 4);
		output->state = state;
		output->scale = 1;

		wl_output_set_user_data(output->output, output);
		wl_output_add_listener(output->output, &output_listener, output);
	}
}

void surface_enter(void *data, struct wl_surface *surface, struct wl_output *wl_output) {
    struct state *state = data;
    state->output = wl_output_get_user_data(wl_output);

    match();
    draw_sf(state);
}

int roundtrip(struct state *state) {
    if (wl_display_roundtrip(state->display)) {
        return 0;
    } else {
        return 1;
    }
}

int connect_display(struct state *state) {
    state->display = wl_display_connect(NULL);

    if (state->display) {
        return 0;
    } else {
        return 1;
    }
}

/* If this function returns 1, something went wrong.
 * This may be that the user is using X11, or a compositor like Mutter.
 * In this case, it may be a good idea to fall back to X11.
 */
int init_disp(struct state *state) {
    if (!state->display) {
        return 1;
    } else {
        no_display = 0;
    }

    struct wl_registry *registry = wl_display_get_registry(state->display);
    wl_registry_add_listener(registry, &registry_listener, state);
    wl_display_roundtrip(state->display);

    assert(state->compositor != NULL);
    assert(state->layer_shell != NULL);
    assert(state->shm != NULL);

    wl_display_roundtrip(state->display);

    if (state->output_name && !state->output) {
        return 1;
    }

    return 0;
}

void resizeclient_wl(struct state *state) {
    struct item *item;
    int omh = mh;
    int ic = 0;

    // walk through all items
    for (item = items; item && item->text; item++)
        ic++;

    bh = MAX(drw->font->h, drw->font->h + 2 + lineheight);
    lines = MIN(ic, MAX(lines, 0));
    reallines = lines;
    mh = (lines + 1) * bh + 2 * menumarginv;

    if (mh == omh) {
        return;
    }

    // why have an empty line? when there's nothing to draw there anyway?
    if (hideprompt && hideinput && hidemode && hidematchcount)
        mh += bh;

    state->width = mw;
    state->height = mh;

    state->buffer = create_buffer(state);

    if (drw == NULL) {
        die("spmenu: drw == NULL");
    }

    if (state->buffer == NULL) {
        die("state->buffer == null");
    }

    drw_create_surface_wl(drw, state->data, state->width, state->height);

    set_layer_size(state, state->width, state->height);
    wl_surface_set_buffer_scale(state->surface, 1);
	wl_surface_attach(state->surface, state->buffer, 0, 0);
	wl_surface_damage(state->surface, 0, 0, state->width, state->height);
	wl_surface_commit(state->surface);
}

/* It is advised you call this function right before calling init_disp()
 * It sets up keybinds for you.
 */
int init_keys(struct state *state) {
    state->xkb_context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);

    if (!state->xkb_context) {
        return 1;
    } else {
        has_keys = 1;
    }

    state->timer = timerfd_create(CLOCK_MONOTONIC, 0);

    if (state->timer < 0) {
        has_keys = 0;
        fprintf(stderr, "timerfd_create() failed\n");

        return 1;
    }

    return 0;
}

int create_layer(struct state *state, char *name) {
    assert(state->compositor != NULL);

    state->surface = wl_compositor_create_surface(state->compositor);

    assert(state->surface != NULL);
    assert(state->layer_shell != NULL);

    wl_surface_add_listener(state->surface, &surface_listener, state);
    state->layer_surface = zwlr_layer_shell_v1_get_layer_surface(
            state->layer_shell,
            state->surface,
            NULL,
            ZWLR_LAYER_SHELL_V1_LAYER_TOP,
            name);

    assert(state->layer_surface != NULL);

    return 0;
}

int anchor_layer(struct state *state, int position) {
    assert(state->layer_surface != NULL);
    zwlr_layer_surface_v1_set_anchor(
            state->layer_surface,
            ZWLR_LAYER_SURFACE_V1_ANCHOR_LEFT |
            ZWLR_LAYER_SURFACE_V1_ANCHOR_RIGHT |
            (position == 0 ? ZWLR_LAYER_SURFACE_V1_ANCHOR_BOTTOM :
             position == 1 ? ZWLR_LAYER_SURFACE_V1_ANCHOR_TOP :
             0));

    return 0;
}

int set_layer_size(struct state *state, int32_t width, int32_t height) {
    assert(state->layer_surface != NULL);

    zwlr_layer_surface_v1_set_size(state->layer_surface, width, height);

    return 0;
}

int set_exclusive_zone(struct state *state, unsigned int val) {
    assert(state->layer_surface != NULL);

    zwlr_layer_surface_v1_set_exclusive_zone(state->layer_surface, val);

    return 0;
}

int set_keyboard(struct state *state, int interactivity) {
    assert(state->layer_surface != NULL);

    zwlr_layer_surface_v1_set_keyboard_interactivity(state->layer_surface, interactivity ? true : false);

    return 0;
}

int add_layer_listener(struct state *state) {
    assert(state->layer_surface != NULL);

    zwlr_layer_surface_v1_add_listener(state->layer_surface, &layer_surface_listener, state);

    return 0;
}

int set_visible_layer(struct state *state) {
    assert(state->layer_surface != NULL);
    assert(state->surface != NULL);

    wl_surface_commit(state->surface);

    return 0;
}