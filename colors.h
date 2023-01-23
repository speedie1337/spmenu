/* Color options */

/* Alpha */
static const unsigned int alphas[][3]      = {
	/*                         fg       bg        border     */
	[SchemeLArrow]         = { fgalpha, bgalpha,  borderalpha },
	[SchemeRArrow]         = { fgalpha, bgalpha,  borderalpha },
	[SchemeItemNorm]       = { fgalpha, bgalpha,  borderalpha },
	[SchemeItemSel]        = { fgalpha, bgalpha,  borderalpha },
	[SchemeMenu]           = { fgalpha, bgalpha,  borderalpha },
	[SchemePrompt]         = { fgalpha, bgalpha,  borderalpha },
	[SchemeNormHighlight]  = { fgalpha, bgalpha,  borderalpha },
	[SchemeSelHighlight]   = { fgalpha, bgalpha,  borderalpha },
    [SchemeCaret]          = { fgalpha, bgalpha,  borderalpha },
    [SchemeNumber]         = { fgalpha, bgalpha,  borderalpha },
    [SchemeBorder]         = { fgalpha, bgalpha,  borderalpha },
};

/* Colors */
static const char *colors[SchemeLast][2] = {
	/*                        fg                 bg                  */
	[SchemeLArrow]        = { col_normfgcolor,   col_normbgcolor     },
	[SchemeRArrow]        = { col_normfgcolor,   col_normbgcolor     },
	[SchemeItemNorm]      = { col_normfgcolor,   col_normbgcolor     },
	[SchemeItemSel]       = { col_selfgcolor,    col_selbgcolor      },
	[SchemeMenu]          = { NULL,              col_normbgcolor     },
	[SchemePrompt]        = { col_selfgcolor,    col_selbgcolor      },
    [SchemeNormHighlight] = { col_normhlfgcolor, col_normhlbgcolor   },
    [SchemeSelHighlight]  = { col_selhlfgcolor,  col_selhlbgcolor    },
    [SchemeCaret]         = { col_caretfgcolor,  NULL                },
    [SchemeNumber]        = { col_numfgcolor,    col_numbgcolor      },
    [SchemeBorder]        = { NULL,              col_bordercolor     },
};

/* sgr colors */
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
