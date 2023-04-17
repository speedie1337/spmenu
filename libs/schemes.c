char
sixd_to_8bit(int x)
{
	return x == 0 ? 0 : 0x37 + 0x28 * x;
}

void
init_appearance(void)
{
    int i, j, k;

    char cbuf[8];

    // set alpha
    // TODO: allow these to be individually set, instead of all using fg/bgalpha
    alphas[SchemeLArrow][ColFg] = fgalpha;
    alphas[SchemeLArrow][ColBg] = bgalpha;
    alphas[SchemeRArrow][ColFg] = fgalpha;
    alphas[SchemeRArrow][ColBg] = bgalpha;
    alphas[SchemeItemNorm][ColFg] = fgalpha;
    alphas[SchemeItemNorm][ColBg] = bgalpha;
    alphas[SchemeItemSel][ColFg] = fgalpha;
    alphas[SchemeItemSel][ColBg] = bgalpha;
    alphas[SchemeItemNormPri][ColFg] = fgalpha;
    alphas[SchemeItemNormPri][ColBg] = bgalpha;
    alphas[SchemeItemSelPri][ColFg] = fgalpha;
    alphas[SchemeItemSelPri][ColBg] = bgalpha;
    alphas[SchemeMenu][ColBg] = bgalpha;
    alphas[SchemeMenu][ColFg] = fgalpha; // this one isn't actually used
    alphas[SchemeInput][ColFg] = fgalpha;
    alphas[SchemeInput][ColBg] = bgalpha;
    alphas[SchemePrompt][ColFg] = fgalpha;
    alphas[SchemePrompt][ColBg] = bgalpha;
    alphas[SchemeNormHighlight][ColFg] = fgalpha;
    alphas[SchemeNormHighlight][ColBg] = bgalpha;
    alphas[SchemeSelHighlight][ColFg] = fgalpha;
    alphas[SchemeSelHighlight][ColBg] = bgalpha;
    alphas[SchemeCaret][ColFg] = fgalpha;
    alphas[SchemeCaret][ColBg] = bgalpha;
    alphas[SchemeNumber][ColFg] = fgalpha;
    alphas[SchemeNumber][ColBg] = bgalpha;
    alphas[SchemeMode][ColFg] = fgalpha;
    alphas[SchemeMode][ColBg] = bgalpha;
    alphas[SchemeBorder][ColFg] = fgalpha; // this one isn't actually used
    alphas[SchemeBorder][ColBg] = bgalpha;

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
