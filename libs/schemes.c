/* See LICENSE file for copyright and license details. */

char sixd_to_8bit(int x) {
    return x == 0 ? 0 : 0x37 + 0x28 * x;
}

void init_appearance(void) {
    int i = 0;

    char cbuf[8];

    if (!alpha && protocol) {
        alpha_itemnormfg = 255;
        alpha_itemnormbg = 255;
        alpha_itemnormfg2 = 255;
        alpha_itemnormbg2 = 255;
        alpha_itemselfg = 255;
        alpha_itemselbg = 255;
        alpha_itemmarkedfg = 255;
        alpha_itemmarkedbg = 255;
        alpha_itemnormprifg = 255;
        alpha_itemnormpribg = 255;
        alpha_itemselprifg = 255;
        alpha_itemselpribg = 255;
        alpha_inputfg = 255;
        alpha_inputbg = 255;
        alpha_menu = 255;
        alpha_promptfg = 255;
        alpha_promptbg = 255;
        alpha_larrowfg = 255;
        alpha_larrowbg = 255;
        alpha_rarrowfg = 255;
        alpha_rarrowbg = 255;
        alpha_hlnormfg = 255;
        alpha_hlnormbg = 255;
        alpha_hlselfg = 255;
        alpha_hlselbg = 255;
        alpha_numfg = 255;
        alpha_numbg = 255;
        alpha_border = 255;
        alpha_caretfg = 255;
        alpha_caretbg = 255;
        alpha_modefg = 255;
        alpha_modebg = 255;
        alpha_capsfg = 255;
        alpha_capsbg = 255;
    }

    for (i = 0; i < LENGTH(textcolors) && i < LENGTH(txtcols); i++) {
        txtcols[i] = textcolors[i];
    }
    if (i == 0) {
        txtcols[i] = "#000000";
    }
    for (; i < 7; i++) {
        snprintf(cbuf, sizeof(cbuf), "#%02x%02x%02x",
                !!(i & 1) * 0x7f,
                !!(i & 2) * 0x7f,
                !!(i & 4) * 0x7f);
        txtcols[i] = cbuf;
    }
    if (i == 7) {
        txtcols[i] = "#000000";
    }
    if (i == 8) {
        txtcols[i] = "#333333";
    }
    for (; i < 16; i++) {
        snprintf(cbuf, sizeof(cbuf), "#%02x%02x%02x",
                !!(i & 1) * 0xff,
                !!(i & 2) * 0xff,
                !!(i & 4) * 0xff);
        txtcols[i] = cbuf;
    }
    for (; i < 6 * 6 * 6 + 16; i++) {
        snprintf(cbuf, sizeof(cbuf), "#%02x%02x%02x",
                sixd_to_8bit(((i - 16) / 36) % 6),
                sixd_to_8bit(((i - 16) / 6) % 6),
                sixd_to_8bit(((i - 16)) % 6));
        txtcols[i] = cbuf;
    }
    for (; i < 256; i++) {
        snprintf(cbuf, sizeof(cbuf), "#%02x%02x%02x",
                0x08 + (i - 6 * 6 * 6 - 16) * 0x0a,
                0x08 + (i - 6 * 6 * 6 - 16) * 0x0a,
                0x08 + (i - 6 * 6 * 6 - 16) * 0x0a);
        txtcols[i] = cbuf;
    }
}
