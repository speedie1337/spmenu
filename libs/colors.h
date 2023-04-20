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
    [SchemeCaps]           = { 255, 200 },
};

// colorscheme array
static char *colors[SchemeLast][2] = {
	//                        fg                 bg
	[SchemeLArrow]        = { col_larrowfg,      col_larrowbg      },
	[SchemeRArrow]        = { col_rarrowfg,      col_rarrowbg      },
	[SchemeItemNorm]      = { col_itemnormfg,    col_itemnormbg    },
	[SchemeItemSel]       = { col_itemselfg,     col_itemselbg     },
	[SchemeItemNormPri]   = { col_itemnormprifg, col_itemnormpribg },
	[SchemeItemSelPri]    = { col_itemselprifg,  col_itemselpribg  },
    [SchemeInput]         = { col_inputfg,       col_inputbg,      },
	[SchemeMenu]          = { NULL,              col_menu          },
    [SchemeCaps]          = { col_capsfg,        col_capsbg        },
	[SchemePrompt]        = { col_promptfg,      col_promptbg      },
    [SchemeNormHighlight] = { col_hlnormfg,      col_hlnormbg      },
    [SchemeSelHighlight]  = { col_hlselfg,       col_hlselbg       },
    [SchemeCaret]         = { col_caretfg,       col_caretbg       },
    [SchemeMode]          = { col_modefg,        col_modebg        },
    [SchemeNumber]        = { col_numfg,         col_numbg         },
    [SchemeBorder]        = { NULL,              col_border        },
};

// sgr color array
// to enable 256 color support, append to this.
static char *textcolors[] = {
	col_sgr0,
	col_sgr1,
	col_sgr2,
	col_sgr3,
	col_sgr4,
	col_sgr5,
	col_sgr6,
	col_sgr7,
	col_sgr8,
	col_sgr9,
	col_sgr10,
	col_sgr11,
	col_sgr12,
	col_sgr13,
	col_sgr14,
	col_sgr15,
};
