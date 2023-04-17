// alpha array
static unsigned int alphas[][2]      = {
	//                         fg   bg
	[SchemeLArrow]         = { 255, 200 },
	[SchemeRArrow]         = { 255, 200 },
	[SchemeItemNorm]       = { 255, 200 },
	[SchemeItemSel]        = { 255, 200 },
	[SchemeItemNormPri]    = { 255, 200 },
	[SchemeItemSelPri]     = { 255, 200 },
	[SchemeMenu]           = { 255, 200 },
    [SchemeInput]          = { 255, 200 },
	[SchemePrompt]         = { 255, 200 },
	[SchemeNormHighlight]  = { 255, 200 },
	[SchemeSelHighlight]   = { 255, 200 },
    [SchemeMode]           = { 255, 200 },
    [SchemeCaret]          = { 255, 200 },
    [SchemeNumber]         = { 255, 200 },
    [SchemeBorder]         = { 255, 200 },
};

// colorscheme array
static char *colors[SchemeLast][2] = {
	//                        fg                 bg
	[SchemeLArrow]        = { col_larrowfg,      col_larrowbg        },
	[SchemeRArrow]        = { col_rarrowfg,      col_rarrowbg        },
	[SchemeItemNorm]      = { col_itemnormfg,    col_itemnormbg      },
	[SchemeItemSel]       = { col_itemselfg,     col_itemselbg       },
	[SchemeItemNormPri]   = { col_itemnormprifg, col_itemnormpribg   },
	[SchemeItemSelPri]    = { col_itemselprifg,  col_itemselpribg    },
    [SchemeInput]         = { col_inputfg,       col_inputbg,        },
	[SchemeMenu]          = { NULL,              col_menubgcolor     },
	[SchemePrompt]        = { col_promptfg,      col_promptbg        },
    [SchemeNormHighlight] = { col_hlnormfgcolor, col_hlnormbgcolor   },
    [SchemeSelHighlight]  = { col_hlselfgcolor,  col_hlselbgcolor    },
    [SchemeCaret]         = { col_caretfgcolor,  NULL                },
    [SchemeMode]          = { col_modefgcolor,   col_modebgcolor     },
    [SchemeNumber]        = { col_numfgcolor,    col_numbgcolor      },
    [SchemeBorder]        = { NULL,              col_bordercolor     },
};

// sgr color array
// to enable 256 color support, append to this.
static char *textcolors[] = {
	col_sgrcolor0,
	col_sgrcolor1,
	col_sgrcolor2,
	col_sgrcolor3,
	col_sgrcolor4,
	col_sgrcolor5,
	col_sgrcolor6,
	col_sgrcolor7,
	col_sgrcolor8,
	col_sgrcolor9,
	col_sgrcolor10,
	col_sgrcolor11,
	col_sgrcolor12,
	col_sgrcolor13,
	col_sgrcolor14,
	col_sgrcolor15,
};
