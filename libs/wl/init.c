/* See LICENSE file for copyright and license details. */

void prepare_window_size_wl(void) {
    sp = menupaddingh;
    vp = (menuposition == 1) ? menupaddingv : - menupaddingv;

    bh = MAX(drw->font->h, drw->font->h + 2 + lineheight);
    lines = MAX(lines, 0);
    reallines = lines;

    lrpad = drw->font->h + textpadding;

    return;
}

void handle_wl(void) {
    if (!setlocale(LC_CTYPE, "")) {
        die("spmenu: no locale support");
    }

    prepare_window_size_wl();
    promptw = (prompt && *prompt) ?  pango_prompt ? TEXTWM(prompt) : TEXTW(prompt) - lrpad / 4 : 0;

    allow_draw = 1;

    init_keys(&state);
    init_disp(&state);

    if (no_display) {
        die("spmenu: failed to connect to wayland display");
    }

    create_layer(&state, "spmenu");

    mw = (menuwidth > 0 ? menuwidth : output_width);
    mh = (lines + 1) * bh + 2 * menumarginv;

    if (menuposition == 2) {
        mw = MIN(MAX(max_textw() + promptw, minwidth), output_width);
    }

    state.width = mw;
    state.height = mh;

    set_layer_size(&state, state.width, state.height);

    anchor_layer(&state, menuposition);
    set_exclusive_zone(&state, -1);
    set_keyboard(&state, 1);
    add_layer_listener(&state);
    set_visible_layer(&state);

    roundtrip(&state);

    match();
    draw_sf(&state);

    while (wl_display_dispatch(state.display) != -1) {
    }

    wl_display_disconnect(state.display);
}
