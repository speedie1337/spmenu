/* Color options */

/* Alpha */
static const unsigned int alphas[][3]      = {
	/*                         fg      bg        border     */
	[SchemeNorm]           = { opaque, baralpha, borderalpha },
	[SchemeSel]            = { opaque, baralpha, borderalpha },
	[SchemePrompt]         = { opaque, baralpha, borderalpha },
	[SchemeNormHighlight]  = { opaque, baralpha, borderalpha },
	[SchemeSelHighlight]   = { opaque, baralpha, borderalpha },
    [SchemeCaret]          = { opaque, baralpha, borderalpha },
    [SchemeNumber]         = { opaque, baralpha, borderalpha },
};

/* Colors */
static const char *colors[SchemeLast][2] = {
	/*                        fg             bg              */
	[SchemeNorm]          = { normfgcolor,   normbgcolor     },
	[SchemeSel]           = { selfgcolor,    selbgcolor      },
	[SchemePrompt]        = { selfgcolor,    selbgcolor      },
    [SchemeNormHighlight] = { normhlfgcolor, normhlbgcolor   },
    [SchemeSelHighlight]  = { selhlfgcolor,  selhlbgcolor    },
    [SchemeCaret]         = { caretfgcolor,  NULL            },
    [SchemeNumber]        = { numfgcolor,    numbgcolor      },
};

/* sgr colors */
static char *textcolors[] = {
	sgrcolor0,
	sgrcolor1,
	sgrcolor2,
	sgrcolor3,
	sgrcolor4,
	sgrcolor5,
	sgrcolor6,
	sgrcolor7,
	sgrcolor8,
	sgrcolor9,
	sgrcolor10,
	sgrcolor11,
	sgrcolor12,
	sgrcolor13,
	sgrcolor14,
	sgrcolor15,
};
