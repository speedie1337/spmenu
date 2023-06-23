/* See LICENSE file for copyright and license details. */

void prepare_window_size_wl(void) {
    sp.sp = menupaddingh;
    sp.vp = (menuposition == 1) ? menupaddingv : - menupaddingv;

    sp.bh = MAX(draw->font->h, draw->font->h + 2 + lineheight);
    lines = MAX(lines, 0);
#if USEIMAGE
    img.setlines = lines;
#endif

    sp.lrpad = draw->font->h + textpadding;

    return;
}

void handle_wl(void) {
    if (!setlocale(LC_CTYPE, "")) {
        die("spmenu: no locale support");
    }

    prepare_window_size_wl();
    sp.promptw = (prompt && *prompt) ?  pango_prompt ? TEXTWM(prompt) : TEXTW(prompt) - sp.lrpad / 4 : 0;

    allow_draw = 1;

    init_keys(&state);
    init_disp(&state);

    if (no_display) {
        die("spmenu: failed to connect to wayland display");
    }

    create_layer(&state, "spmenu");

    sp.mw = (menuwidth > 0 ? menuwidth : output_width);
    get_mh();

    if (menuposition == 2) {
        sp.mw = MIN(MAX(max_textw() + sp.promptw, minwidth), output_width);
    }

    state.width = sp.mw;
    state.height = sp.mh;

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
