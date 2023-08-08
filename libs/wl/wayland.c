/* See LICENSE file for copyright and license details. */

#include "shm.c"

struct wl_buffer *create_buffer(struct state *state) {
    int32_t width = state->width;
    int32_t height = state->height;

    uint32_t stride = width * 4;
    size_t siz = stride * height;

    int fd = create_shm_file(siz);

    if (fd == -1) {
        die("create_buffer(): fd == -1");
    }

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
    int alt_pressed = xkb_state_mod_name_is_active(state->xkb_state, XKB_MOD_NAME_ALT, XKB_STATE_MODS_DEPRESSED | XKB_STATE_MODS_LATCHED);
    int shift_pressed = xkb_state_mod_name_is_active(state->xkb_state, XKB_MOD_NAME_SHIFT, XKB_STATE_MODS_DEPRESSED | XKB_STATE_MODS_LATCHED);
    int ctrl_pressed = xkb_state_mod_name_is_active(state->xkb_state, XKB_MOD_NAME_CTRL, XKB_STATE_MODS_DEPRESSED | XKB_STATE_MODS_LATCHED);
    int logo_pressed = xkb_state_mod_name_is_active(state->xkb_state, XKB_MOD_NAME_LOGO, XKB_STATE_MODS_DEPRESSED | XKB_STATE_MODS_LATCHED);

    if (!strcmp(modifier, WL_CtrlShift) && shift_pressed && ctrl_pressed) {
        return 0;
    } else if (!strcmp(modifier, WL_CtrlShiftSuper) && shift_pressed && ctrl_pressed && logo_pressed) {
        return 0;
    } else if (!strcmp(modifier, WL_CtrlSuper) && ctrl_pressed && logo_pressed) {
        return 0;
    } else if (!strcmp(modifier, WL_CtrlAlt) && alt_pressed && ctrl_pressed) {
        return 0;
    } else if (!strcmp(modifier, WL_CtrlAltShift) && ctrl_pressed && alt_pressed && shift_pressed) {
        return 0;
    } else if (!strcmp(modifier, WL_CtrlAltShiftSuper) && ctrl_pressed && alt_pressed && shift_pressed && logo_pressed) {
        return 0;
    } else if (!strcmp(modifier, WL_CtrlAltSuper) && ctrl_pressed && alt_pressed && logo_pressed) {
        return 0;
    } else if (!strcmp(modifier, WL_AltShift) && alt_pressed && shift_pressed) {
        return 0;
    } else if (!strcmp(modifier, WL_Shift) && shift_pressed) {
        return 0;
    } else if (!strcmp(modifier, WL_Ctrl) && ctrl_pressed) {
        return 0;
    } else if (!strcmp(modifier, WL_Alt) && alt_pressed) {
        return 0;
    } else if (!strcmp(modifier, WL_Super) && logo_pressed) {
        return 0;
    } else if (!strcmp(modifier, WL_None) && !alt_pressed && !shift_pressed && !ctrl_pressed && !logo_pressed) {
        return 0;
    }

    return 1;
}

/* This function is pretty garbage. However I don't feel like implementing all the garbage necessary to paste properly.
 * If anyone wants to do it, feel free to pull request.
 */
char *wl_clipboard(void) {
    FILE *fp;
    char output_text[1024];
    char *clipboard = malloc(sizeof(output_text));
    clipboard[0] = '\0';

    fp = popen("which wl-paste > /dev/null && wl-paste -t text/plain", "r");

    if (fp == NULL) {
        fprintf(stderr, "wl_clipboard(): Failed to open command\n");
        return NULL;
    }

    while (fgets(output_text, sizeof(output_text), fp) != NULL) {
        strcat(clipboard, output_text);
    }

    pclose(fp);

    return clipboard;
}

void paste_wl(void) {
    char *p, *q;

    p = wl_clipboard();

    insert(p, (q = strchr(p, '\n')) ? q - p : (ssize_t)strlen(p)); // insert selection

    drawmenu();
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
        if (sp.ignoreglobalkeys) break;

        if (xkb_keysym_to_lower(sym) == wl_keys[i].keysym && !is_correct_modifier(state, wl_keys[i].modifier) && wl_keys[i].func) {
            if ((wl_keys[i].mode && sp.mode) || wl_keys[i].mode == -1) {
                wl_keys[i].func(&(wl_keys[i].arg));
                return;
            } else if (!wl_keys[i].mode && !sp.mode) {
                wl_keys[i].func(&(wl_keys[i].arg));
            }
        }
    }

    for (i = 0; i < LENGTH(wl_ckeys); i++) {
        if (sp.ignoreconfkeys) break;

        if (xkb_keysym_to_lower(sym) == wl_ckeys[i].keysym && !is_correct_modifier(state, wl_ckeys[i].modifier) && wl_ckeys[i].func) {
            if ((wl_ckeys[i].mode && sp.mode) || wl_ckeys[i].mode == -1) {
                wl_ckeys[i].func(&(wl_ckeys[i].arg));
                return;
            } else if (!wl_ckeys[i].mode && !sp.mode) {
                wl_ckeys[i].func(&(wl_ckeys[i].arg));
            }
        }
    }

    if (xkb_keysym_to_lower(sym) == XKB_KEY_Escape) {
        return;
    }

    if (!type || !sp.mode) {
        return;
    }

    if (xkb_keysym_to_utf8(sym, buf, 8)) {
        if (sp.allowkeys) {
            insert(buf, strnlen(buf, 8));
        } else {
            sp.allowkeys = !sp.allowkeys;
        }
    }

    drawmenu();
}

void keyboard_modifiers(void *data, struct wl_keyboard *keyboard, uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group) {
	struct state *state = data;
    int ocapslockstate = sp.capslockstate;

	xkb_state_update_mask(state->xkb_state, mods_depressed, mods_latched, mods_locked, 0, 0, group);

    if (xkb_state_mod_name_is_active(state->xkb_state, XKB_MOD_NAME_CAPS, XKB_STATE_MODS_EFFECTIVE)) {
        sp.capslockstate = 1;
    } else {
        sp.capslockstate = 0;
    }

    if (ocapslockstate != sp.capslockstate) {
        strncpy(tx.capstext, sp.capslockstate ? capslockontext : capslockofftext, 15);
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

void keyboard_repeat(struct state *state) {
    keypress_wl(state, state->repeat_key_state, state->repeat_sym);

    struct itimerspec spec = { 0 };

    spec.it_value.tv_sec = state->period / 1000;
	spec.it_value.tv_nsec = (state->period % 1000) * 1000000l;

	timerfd_settime(state->timer, 0, &spec, NULL);
}

void keyboard_key(void *data, struct wl_keyboard *wl_keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t _key_state) {
	struct state *state = data;

	enum wl_keyboard_key_state key_state = _key_state;

    strncpy(tx.capstext, sp.capslockstate ? capslockontext : capslockofftext, 15);

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
        die("keyboard_keymap(): MAP_FAILED");
		return;
	}
	state->xkb_keymap = xkb_keymap_new_from_string(state->xkb_context, map_shm, XKB_KEYMAP_FORMAT_TEXT_V1, 0);
	munmap(map_shm, size);
	close(fd);
	state->xkb_state = xkb_state_new(state->xkb_keymap);
}

void buttonpress_wl(uint32_t button, double ex, double ey) {
    int x = 0;
    int y = 0;
    int w;
    int h = sp.bh;
    int xpad = 0;
    int item_num = 0;
    int yp = 0;
    unsigned int i, click;
    struct item *item;

    if (ex == 0 && ey == 0) {
        return; // While it is possible to click at this position, usually it means we're outside the window area.
    }

    if (!hidepowerline) {
        x = xpad = sp.plw;
    }

    x += menumarginh;

    int larroww = 0;
    int rarroww = 0;
    int numberw = 0;
    int modew = 0;
    int capsw = 0;

    if (!hidelarrow) larroww = pango_leftarrow ? TEXTWM(leftarrow) : TEXTW(leftarrow);
    if (!hiderarrow) rarroww = pango_rightarrow ? TEXTWM(rightarrow) : TEXTW(rightarrow);
    if (!hidematchcount) numberw = pango_numbers ? TEXTWM(tx.numbers) : TEXTW(tx.numbers);
    if (!hidemode) modew = pango_mode ? TEXTWM(tx.modetext) : TEXTW(tx.modetext);
    if (!hidecaps) capsw = pango_caps ? TEXTWM(tx.capstext) : TEXTW(tx.capstext);

    if (!strcmp(tx.capstext, ""))
        capsw = 0;

    if ((hideprompt && hideinput && hidemode && hidematchcount && hidecaps) && lines) {
        yp = 1;
    } else if (!itemposition && lines && ey <= h + menumarginv && ey >= menumarginv) {
        yp = 1;
    } else if (itemposition && lines && ey >= (sp.mh - h) + menumarginv) {
        yp = 1;
    } else if (!lines) {
        yp = 1;
    }

    click = ClickWindow; // Clicking anywhere, we use this and override it if we clicked on something specific

    // Check click position and override the value of click
    if (yp && ex < x + sp.promptw + powerlineprompt ? sp.plw : 0) {
        click = ClickPrompt;
    } else if (yp && (ex > sp.mw - capsw - 2 * sp.sp - 2 * borderwidth - menumarginh) && !hidecaps && capsw) {
        click = ClickCaps;
    } else if (yp && ex > sp.mw - modew - capsw - 2 * sp.sp - 2 * borderwidth - menumarginh) {
        click = ClickMode;
    } else if (yp && ex > sp.mw - modew - numberw - capsw - 2 * sp.sp - 2 * borderwidth - menumarginh) {
        click = ClickNumber;
    } else if (yp && !hideinput) {
        w = (lines > 0 || !matches) ? sp.mw - x : sp.inputw;

        if ((lines <= 0 && ex >= 0 && ex <= x + w + sp.promptw +
                    ((!previousitem || !currentitem->left) ? larroww : 0)) ||
                (lines > 0 && ey >= y && ey <= y + h)) {

            click = ClickInput;
        }
    }

#if IMAGE
    if (!hideimage && img.longestedge != 0 && imagetype) {
        x += MAX((img.imagegaps * 2) + img.imagewidth, indentitems ? sp.promptw : 0);
    }
#endif

    // It's an item click
    if (lines > 0) {
        w = sp.mw - x;

        ey -= menumarginv;

        if ((hideprompt && hideinput && hidemode && hidematchcount && hidecaps) || itemposition) {
            ey += h;
        }

        for (item = currentitem; item != nextitem; item = item->right) {
            if (item_num++ == lines) {
                item_num = 1;
                x += w / columns;
                y = 0;
            }

            y += h;

            if (ey >= y && ey <= (y + h) && ex >= x + (powerlineitems ? sp.plw : 0) && ex <= (x + w / columns) + (powerlineitems ? sp.plw : 0)) {
                click = ClickItem;
                mouseitem = item;
#if IMAGE
            } else if (ey >= y && ey <= (y + h) && ex >= x + (powerlineitems ? sp.plw : 0) - MAX((img.imagegaps * 2) + img.imagewidth, indentitems ? sp.promptw : 0) && ex <= (x - MAX((img.imagegaps * 2) + img.imagewidth, indentitems ? sp.promptw : 0) + w / columns) + (powerlineitems ? sp.plw : 0)) {
                click = ClickImage;
                mouseitem = item;
#endif
            }
        }
    } else if (matches) { // Single line
        x += sp.inputw;
        w = larroww;

        if (previousitem && currentitem->left) {
            if (ex >= x && ex <= x + w) {
                click = ClickLArrow;
            }
        }

        w = rarroww;
        x = sp.mw - w;

        if (nextitem && ex >= x && ex <= x + w) {
            click = ClickRArrow;
        }
    }

    for (i = 0; i < LENGTH(wl_buttons); i++) {
        if (sp.ignoreglobalmouse) break;
        if ((click == wl_buttons[i].click || wl_buttons[i].click == ClickNone) && wl_buttons[i].func && wl_buttons[i].button == button)
            wl_buttons[i].func(&wl_buttons[i].arg);
    }

    for (i = 0; i < LENGTH(wl_cbuttons); i++) {
        if (sp.ignoreconfmouse) break;
        if ((click == wl_cbuttons[i].click || wl_cbuttons[i].click == ClickNone) && wl_cbuttons[i].func && wl_cbuttons[i].button == button)
            wl_cbuttons[i].func(&wl_cbuttons[i].arg);
    }
}

void pointer_motion_handler(void *data, struct wl_pointer *pointer, uint32_t time, wl_fixed_t x, wl_fixed_t y) {
    mouse_x = wl_fixed_to_int(x);
    mouse_y = wl_fixed_to_int(y);
}

void pointer_axis_handler(void *data, struct wl_pointer *pointer, uint32_t time, uint32_t axis, wl_fixed_t value) {
    if (value > scrolldistance) {
        mouse_scroll_direction = 0;
    } else if (value < -scrolldistance) {
        mouse_scroll_direction = 1;
    } else {
        mouse_scroll = 0;
        mouse_scroll_direction = -1;

        return;
    }

    mouse_scroll = 1;

    buttonpress_wl(mouse_scroll_direction, mouse_x, mouse_y);

    mouse_scroll = 0;
    mouse_scroll_direction = -1;
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

void create_drawable(struct state *state) {
    if (!allow_draw) { // No drawing if disabled
        return;
    }

    // create buffer to draw on
    state->buffer = create_buffer(state);

    if (draw == NULL) {
        die("create_drawable(): A drawable was not created.");
    }

    if (state->buffer == NULL) {
        die("create_drawable(): Buffer state->buffer could not be created.");
    }

    draw_create_surface_wl(draw, state->data, state->width, state->height);
    drawmenu();
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
    create_drawable(state);
}

void commit_drawable(struct state *state) {
    wl_surface_set_buffer_scale(state->surface, 1);
    wl_surface_attach(state->surface, state->buffer, 0, 0);
    wl_surface_damage(state->surface, 0, 0, state->width, state->height);
    wl_surface_commit(state->surface);
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

int disconnect_display(struct state *state) {
    wl_display_disconnect(state->display);

    return 0;
}

int await_dispatch(struct state *state) {
    struct pollfd fds[] = {
        { wl_display_get_fd(state->display), POLLIN, 0 },
		{ state->timer, POLLIN, 0 },
	};

    const int nfds = sizeof(fds) / sizeof(*fds);

    for (;;) {
        errno = 0;

        do {
			if (wl_display_flush(state->display) == -1 && errno != EAGAIN) {
				fprintf(stderr, "await_dispatch(): wl_display_flush failed: %s\n", strerror(errno));
				break;
			}
		} while (errno == EAGAIN);

        if (poll(fds, nfds, -1) < 0) {
			fprintf(stderr, "await_dispatch(): poll failed: %s\n", strerror(errno));
			break;
		}

		if (fds[0].revents & POLLIN) {
			if (wl_display_dispatch(state->display) < 0) {
                break;
			}
		}

		if (fds[1].revents & POLLIN) {
			keyboard_repeat(state);
		}
    }

    return 0;
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

    if (state->compositor == NULL) {
        die("init_disp(): No Wayland compositor.");
    }

    if (state->layer_shell == NULL) {
        die("init_disp(): Your compositor does not implement the wlr-layer-shell protocol.");
    }

    if (state->shm == NULL) {
        die("init_disp(): Could not create shared memory buffer.\n");
    }

    wl_display_roundtrip(state->display);

    if (state->output_name && !state->output) {
        return 1;
    }

    return 0;
}

void resizeclient_wl(struct state *state) {
    struct item *item;
    int mh = sp.mh;
    int ic = 0;

    for (item = items; item && item->text; item++)
        ic++;

    lines = MIN(ic, MAX(lines, 0));
#if IMAGE
    img.setlines = lines;
#endif
    get_mh();

    if (hideprompt && hideinput && hidemode && hidematchcount && hidecaps) {
        sp.mh -= sp.bh;
    }

    if (sp.mh == mh) {
        return;
    }

    state->width = sp.mw;
    state->height = sp.mh;

    state->buffer = create_buffer(state);

    if (draw == NULL) {
        die("resizeclient_wl(): Draw_t is NULL.");
    }

    if (state->buffer == NULL) {
        die("state->buffer == null");
    }

    draw_create_surface_wl(draw, state->data, state->width, state->height);

    set_layer_size(state, state->width, state->height);
    commit_drawable(state);
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
    if (state->compositor == NULL) {
        die("create_layer(): No Wayland compositor.");
    }

    state->surface = wl_compositor_create_surface(state->compositor);

    if (state->surface == NULL) {
        die("create_layer(): Could not create surface state->surface using state->compositor");
    }

    if (state->layer_shell == NULL) {
        die("create_layer(): Your compositor does not implement the wlr-layer-shell protocol. Run spmenu in X11 mode.");
    }

    wl_surface_add_listener(state->surface, &surface_listener, state);
    state->layer_surface = zwlr_layer_shell_v1_get_layer_surface(
            state->layer_shell,
            state->surface,
            NULL,
            ZWLR_LAYER_SHELL_V1_LAYER_TOP,
            name);

    if (state->layer_surface == NULL) {
        die("create_layer(): Could not get layer_surface state->layer_surface");
    }

    return 0;
}

int anchor_layer(struct state *state, int position) {
    if (state->layer_surface == NULL) {
        die("anchor_layer(): Could not get layer_surface state->layer_surface");
    }

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
    if (state->layer_surface != NULL) {
        die("set_layer_size(): Could not get layer_surface state->layer_surface");
    }

    zwlr_layer_surface_v1_set_size(state->layer_surface, width, height);

    return 0;
}

int set_exclusive_zone(struct state *state, unsigned int val) {
    if (state->layer_surface == NULL) {
        die("set_exclusive_zone(): Could not get layer_surface state->layer_surface");
    }

    zwlr_layer_surface_v1_set_exclusive_zone(state->layer_surface, val);

    return 0;
}

int set_keyboard(struct state *state, int interactivity) {
    if (state->layer_surface == NULL) {
        die("set_keyboard(): Could not get layer_surface state->layer_surface");
    }

    zwlr_layer_surface_v1_set_keyboard_interactivity(state->layer_surface, interactivity ? true : false);

    return 0;
}

int add_layer_listener(struct state *state) {
    if (state->layer_surface == NULL) {
        die("add_layer_listener(): Could not get layer_surface state->layer_surface");
    }

    zwlr_layer_surface_v1_add_listener(state->layer_surface, &layer_surface_listener, state);

    return 0;
}

int set_visible_layer(struct state *state) {
    if (state->layer_surface == NULL) {
        die("set_visible_layer(): Could not get layer_surface state->layer_surface");
    }

    wl_surface_commit(state->surface);

    return 0;
}
