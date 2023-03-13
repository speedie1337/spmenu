/* This header is for the .Xresources options.
 * These will be set on startup by xrdb.
 */

ResourcePref resources[] = {
	{ "font",			             STRING,  &font },
    { "col_caretfgcolor",            STRING,  &col_caretfgcolor },
    { "col_larrowfg",                STRING,  &col_larrowfg },
    { "col_rarrowfg",                STRING,  &col_rarrowfg },
    { "col_larrowbg",                STRING,  &col_larrowbg },
    { "col_rarrowbg",                STRING,  &col_rarrowbg },
    { "col_itemnormfg",              STRING,  &col_itemnormfg },
    { "col_itemnormbg",              STRING,  &col_itemnormbg },
    { "col_itemselfg",               STRING,  &col_itemselfg },
    { "col_itemselbg",               STRING,  &col_itemselbg },
    { "col_itemnormprifg",           STRING,  &col_itemnormprifg },
    { "col_itemnormpribg",           STRING,  &col_itemnormpribg },
    { "col_itemselprifg",            STRING,  &col_itemselprifg },
    { "col_itemselpribg",            STRING,  &col_itemselpribg },
    { "col_inputbg",                 STRING,  &col_inputbg },
    { "col_inputfg",                 STRING,  &col_inputfg },
    { "col_menubgcolor",             STRING,  &col_menubgcolor },
	{ "col_numfgcolor",              STRING,  &col_numfgcolor },
	{ "col_numbgcolor",              STRING,  &col_numbgcolor },
	{ "col_modefgcolor",             STRING,  &col_modefgcolor },
	{ "col_modebgcolor",             STRING,  &col_modebgcolor },
	{ "col_normhlfgcolor",           STRING,  &col_normhlfgcolor },
	{ "col_normhlbgcolor",           STRING,  &col_normhlbgcolor },
	{ "col_selhlfgcolor",            STRING,  &col_selhlfgcolor },
	{ "col_selhlbgcolor",            STRING,  &col_selhlbgcolor },
    { "col_bordercolor",             STRING,  &col_bordercolor },
    { "col_promptfg",                STRING,  &col_promptfg },
    { "col_promptbg",                STRING,  &col_promptbg },

	/* Pywal support    */
	{ "color10",             STRING,  &col_caretfgcolor },
	{ "color4",              STRING,  &col_larrowfg },
	{ "color4",              STRING,  &col_rarrowfg },
	{ "color10",             STRING,  &col_itemnormfg },
	{ "color10",             STRING,  &col_itemnormprifg },
	{ "color10",             STRING,  &col_inputfg },
	{ "color0",              STRING,  &col_itemnormbg },
	{ "color0",              STRING,  &col_itemnormpribg },
	{ "color0",              STRING,  &col_menubgcolor },
	{ "color0",              STRING,  &col_larrowbg },
	{ "color0",              STRING,  &col_rarrowbg },
	{ "color0",              STRING,  &col_itemselfg },
	{ "color0",              STRING,  &col_itemselprifg },
	{ "color0",              STRING,  &col_inputbg },
	{ "color12",             STRING,  &col_promptbg },
	{ "color0",              STRING,  &col_promptfg },
	{ "color6",              STRING,  &col_itemselbg },
	{ "color6",              STRING,  &col_itemselpribg },
	{ "color6",              STRING,  &col_bordercolor },
	{ "color0",              STRING,  &col_numfgcolor },
	{ "color5",              STRING,  &col_numbgcolor },
	{ "color0",              STRING,  &col_modefgcolor },
	{ "color7",              STRING,  &col_modebgcolor },
	{ "color2",              STRING,  &col_normhlbgcolor },
	{ "color3",              STRING,  &col_selhlbgcolor },
	{ "color0",              STRING,  &col_normhlfgcolor },
	{ "color0",              STRING,  &col_selhlfgcolor },

    /* sgr colors */
   	{ "col_sgrcolor0",           STRING,  &col_sgrcolor0 },
	{ "col_sgrcolor1",           STRING,  &col_sgrcolor1 },
	{ "col_sgrcolor2",           STRING,  &col_sgrcolor2 },
	{ "col_sgrcolor3",           STRING,  &col_sgrcolor3 },
	{ "col_sgrcolor4",           STRING,  &col_sgrcolor4 },
	{ "col_sgrcolor5",           STRING,  &col_sgrcolor5 },
	{ "col_sgrcolor6",           STRING,  &col_sgrcolor6 },
	{ "col_sgrcolor7",           STRING,  &col_sgrcolor7 },
	{ "col_sgrcolor8",           STRING,  &col_sgrcolor8 },
	{ "col_sgrcolor9",           STRING,  &col_sgrcolor9 },
	{ "col_sgrcolor10",          STRING,  &col_sgrcolor10 },
	{ "col_sgrcolor11",          STRING,  &col_sgrcolor11 },
	{ "col_sgrcolor12",          STRING,  &col_sgrcolor12 },
	{ "col_sgrcolor13",          STRING,  &col_sgrcolor13 },
	{ "col_sgrcolor14",          STRING,  &col_sgrcolor14 },
	{ "col_sgrcolor15",          STRING,  &col_sgrcolor15 },

    /* sgr colors */
   	{ "color0",              STRING,  &col_sgrcolor0 },
	{ "color1",              STRING,  &col_sgrcolor1 },
	{ "color2",              STRING,  &col_sgrcolor2 },
	{ "color3",              STRING,  &col_sgrcolor3 },
	{ "color4",              STRING,  &col_sgrcolor4 },
	{ "color5",              STRING,  &col_sgrcolor5 },
	{ "color6",              STRING,  &col_sgrcolor6 },
	{ "color7",              STRING,  &col_sgrcolor7 },
	{ "color8",              STRING,  &col_sgrcolor8 },
	{ "color9",              STRING,  &col_sgrcolor9 },
	{ "color10",             STRING,  &col_sgrcolor10 },
	{ "color11",             STRING,  &col_sgrcolor11 },
	{ "color12",             STRING,  &col_sgrcolor12 },
	{ "color13",             STRING,  &col_sgrcolor13 },
	{ "color14",             STRING,  &col_sgrcolor14 },
	{ "color15",             STRING,  &col_sgrcolor15 },

	{ "menuposition",        INTEGER, &menuposition },
	{ "menupaddingv",        INTEGER, &menupaddingv },
	{ "menupaddingh",        INTEGER, &menupaddingh },
    { "indentitems",         INTEGER, &indentitems },
    { "accuratewidth",       INTEGER, &accuratewidth },
    { "alpha",               INTEGER, &alpha },
    { "type",                INTEGER, &type },
    { "preselected",         INTEGER, &preselected },
    { "password",            STRING,  &password },
	{ "prompt",              STRING,  &prompt },
	{ "class",               STRING,  &class },
    { "leftarrow",           STRING,  &leftarrow },
    { "rightarrow",          STRING,  &rightarrow },
    { "normtext",            STRING,  &normtext },
    { "instext",             STRING,  &instext },
	{ "borderwidth",         INTEGER, &borderwidth },
	{ "lines",               INTEGER, &lines },
    { "lineheight",          INTEGER, &lineheight },
	{ "columns",             INTEGER, &columns },
	{ "maxhist",             INTEGER, &maxhist },
    { "hidematchcount",      INTEGER, &hidematchcount },
    { "hidehighlight",       INTEGER, &hidehighlight },
    { "hidemode",            INTEGER, &hidemode },
    { "hideimage",           INTEGER, &hideimage },
    { "hidelarrow",          INTEGER, &hidelarrow },
    { "hiderarrow",          INTEGER, &hiderarrow },
    { "hideprompt",          INTEGER, &hideprompt },
    { "hidecursor",          INTEGER, &hidecursor },
	{ "histnodup",           INTEGER, &histnodup },
    { "casesensitive",       INTEGER, &casesensitive },
    { "imagewidth",          INTEGER, &imagewidth },
    { "imageheight",         INTEGER, &imageheight },
    { "imagegaps",           INTEGER, &imagegaps },
    { "imageposition",       INTEGER, &imageposition },
    { "generatecache",       INTEGER, &generatecache },
    { "mode",                INTEGER, &mode },
    { "fast",                INTEGER, &fast },
    { "loadconfig",          INTEGER, &loadconfig },
    { "managed",             INTEGER, &managed },
    { "mon",                 INTEGER, &mon },
    { "pango_item",          INTEGER, &pango_item },
    { "pango_prompt",        INTEGER, &pango_prompt },
    { "pango_input",         INTEGER, &pango_input },
    { "pango_leftarrow",     INTEGER, &pango_leftarrow },
    { "pango_rightarrow",    INTEGER, &pango_rightarrow },
    { "pango_numbers",       INTEGER, &pango_numbers },
    { "pango_mode",          INTEGER, &pango_mode },
    { "pango_password",      INTEGER, &pango_password },
    { "pango_highlight",     INTEGER, &pango_highlight },
};
