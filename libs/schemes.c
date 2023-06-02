/* See LICENSE file for copyright and license details. */

char sixd_to_8bit(int x) {
    return x == 0 ? 0 : 0x37 + 0x28 * x;
}

void init_appearance(void) {
    int i = 0;

    char cbuf[8];

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
