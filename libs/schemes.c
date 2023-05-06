char sixd_to_8bit(int x) {
	return x == 0 ? 0 : 0x37 + 0x28 * x;
}

void init_appearance(void) {
    int i, j;

    char cbuf[8];

    // set alpha
    alphas[SchemeLArrow][ColFg] = alpha_larrowfg;
    alphas[SchemeLArrow][ColBg] = alpha_larrowbg;
    alphas[SchemeRArrow][ColFg] = alpha_rarrowfg;
    alphas[SchemeRArrow][ColBg] = alpha_rarrowbg;
    alphas[SchemeItemNorm][ColFg] = alpha_itemnormfg;
    alphas[SchemeItemNorm][ColBg] = alpha_itemnormbg;
    alphas[SchemeItemSel][ColFg] = alpha_itemselfg;
    alphas[SchemeItemSel][ColBg] = alpha_itemselbg;
    alphas[SchemeItemNormPri][ColFg] = alpha_itemnormprifg;
    alphas[SchemeItemNormPri][ColBg] = alpha_itemnormpribg;
    alphas[SchemeItemSelPri][ColFg] = alpha_itemselprifg;
    alphas[SchemeItemSelPri][ColBg] = alpha_itemselpribg;
    alphas[SchemeMenu][ColBg] = alpha_menu;
    alphas[SchemeInput][ColFg] = alpha_inputfg;
    alphas[SchemeInput][ColBg] = alpha_inputbg;
    alphas[SchemePrompt][ColFg] = alpha_promptfg;
    alphas[SchemePrompt][ColBg] = alpha_promptbg;
    alphas[SchemeNormHighlight][ColFg] = alpha_hlnormfg;
    alphas[SchemeNormHighlight][ColBg] = alpha_hlnormbg;
    alphas[SchemeSelHighlight][ColFg] = alpha_hlselfg;
    alphas[SchemeSelHighlight][ColBg] = alpha_hlselbg;
    alphas[SchemeCaret][ColFg] = alpha_caretfg;
    alphas[SchemeCaret][ColBg] = alpha_caretbg;
    alphas[SchemeNumber][ColFg] = alpha_numfg;
    alphas[SchemeNumber][ColBg] = alpha_numbg;
    alphas[SchemeMode][ColFg] = alpha_modefg;
    alphas[SchemeMode][ColBg] = alpha_modebg;
    alphas[SchemeCaps][ColFg] = alpha_capsfg;
    alphas[SchemeCaps][ColBg] = alpha_capsbg;
    alphas[SchemeBorder][ColBg] = alpha_border;

    // create color schemes from array
	for (j = 0; j < SchemeLast; j++) {
		scheme[j] = drw_scm_create(drw, colors[j], alphas[j], 2);
    }

    for (i = 0; i < LENGTH(textcolors) && i < LENGTH(textclrs); i++)
 		drw_clr_create(drw, &textclrs[i], textcolors[i], 0);
	if (i == 0)
		drw_clr_create(drw, &textclrs[i++], "#000000", 0);
	for (; i < 7; i++) {
		snprintf(cbuf, sizeof(cbuf), "#%02x%02x%02x",
			 !!(i & 1) * 0x7f,
			 !!(i & 2) * 0x7f,
			 !!(i & 4) * 0x7f);
		drw_clr_create(drw, &textclrs[i], cbuf, 0);
	}
	if (i == 7)
		drw_clr_create(drw, &textclrs[i++], "#000000", 0);
	if (i == 8)
		drw_clr_create(drw, &textclrs[i++], "#333333", 0);
	for (; i < 16; i++) {
		snprintf(cbuf, sizeof(cbuf), "#%02x%02x%02x",
			 !!(i & 1) * 0xff,
			 !!(i & 2) * 0xff,
			 !!(i & 4) * 0xff);
		drw_clr_create(drw, &textclrs[i], cbuf, 0);
	}
	for (; i < 6 * 6 * 6 + 16; i++) {
		snprintf(cbuf, sizeof(cbuf), "#%02x%02x%02x",
			 sixd_to_8bit(((i - 16) / 36) % 6),
			 sixd_to_8bit(((i - 16) / 6) % 6),
			 sixd_to_8bit(((i - 16)) % 6));
		drw_clr_create(drw, &textclrs[i], cbuf, 0);
	}
	for (; i < 256; i++) {
		snprintf(cbuf, sizeof(cbuf), "#%02x%02x%02x",
			 0x08 + (i - 6 * 6 * 6 - 16) * 0x0a,
			 0x08 + (i - 6 * 6 * 6 - 16) * 0x0a,
			 0x08 + (i - 6 * 6 * 6 - 16) * 0x0a);
		drw_clr_create(drw, &textclrs[i], cbuf, 0);
	}
}
