void
readargs(int argc, char *argv[])
{
    int i = 0;

    XrmInitialize();

    for (i = 1; i < argc; i++) {
        /* xrdb first as it overrides other options */
		if (!strcmp(argv[i], "-xrdb"))   /* xresources */
			xresources = 1;
		else if (!strcmp(argv[i], "-nxrdb"))   /* no xresources */
			xresources = 0;
    }

    if (xresources) {
        load_xresources();
    }

    if (casesensitive) {
        fstrncmp = strncmp;
        fstrstr = strstr;
    }

	/* these options take no arguments */
	for (i = 1; i < argc; i++)
        if (!strcmp(argv[i], "-v")) {      /* prints version information */
			puts("spmenu-"VERSION);
			exit(0);
		} else if (!strcmp(argv[i], "-it")) { /* image: top */
			imageposition = 0;
		} else if (!strcmp(argv[i], "-ib")) { /* image: bottom */
			imageposition = 1;
		} else if (!strcmp(argv[i], "-ic")) { /* image: center */
			imageposition = 2;
		} else if (!strcmp(argv[i], "-itc")) { /* image: top center */
			imageposition = 3;
		} else if (!strcmp(argv[i], "-b")) { /* appears at the bottom of the screen */
			menuposition = 0;
		} else if (!strcmp(argv[i], "-t")) { /* appears at the top of the screen */
			menuposition = 1;
		} else if (!strcmp(argv[i], "-nm")) { /* normal mode */
			mode = 0;
		} else if (!strcmp(argv[i], "-im")) { /* insert mode */
			mode = 1;
        } else if (!strcmp(argv[i], "-c")) {  /* appears at the center of the screen */
		    centered = 1;
        } else if (!strcmp(argv[i], "-f")) {   /* grabs keyboard before reading stdin */
			fast = 1;
        } else if (!strcmp(argv[i], "-rw")) {  /* relative width */
			accuratewidth = 1;
        } else if (!strcmp(argv[i], "-nrw")) {   /* no relative width */
			accuratewidth = 0;
        } else if (!strcmp(argv[i], "-xrdb"))   /* xresources */
			xresources = 1;
		else if (!strcmp(argv[i], "-nxrdb"))   /* no xresources */
			xresources = 0;
		else if (!strcmp(argv[i], "-F"))   /* fuzzy matching */
		     fuzzy = 0;
		else if (!strcmp(argv[i], "-s")) { /* case-sensitive item matching */
			fstrncmp = strncmp;
			fstrstr = strstr;
		} else if (!strcmp(argv[i], "-i")) { /* case-sensitive item matching, for compatibility reasons */
		    fstrncmp = strncasecmp;
		    fstrstr = cistrstr;
        } else if (!strcmp(argv[i], "-gc")) { /* generate image cache */
            generatecache = 1;
		} else if (!strcmp(argv[i], "-wm")) { /* display as managed wm window */
				managed = 1;
		} else if (!strcmp(argv[i], "-na")) { /* disable alpha */
				alpha = 0;
		} else if (!strcmp(argv[i], "-a")) { /* alpha */
				alpha = 1;
        } else if (!strcmp(argv[i], "-cp")) { /* color prompt */
                colorprompt = 1;
        } else if (!strcmp(argv[i], "-ncp")) { /* no color prompt */
                colorprompt = 0;
        } else if (!strcmp(argv[i], "-tp")) { /*  allow the user to type */
                type = 1;
        } else if (!strcmp(argv[i], "-nt")) { /*  don't allow the user to type */
                type = 0;
		} else if (!strcmp(argv[i], "-P")) {  /* is the input a password */
				passwd = 1;
		} else if (!strcmp(argv[i], "-hmc")) {   /* hide match count */
				hidematchcount = 1;
		} else if (!strcmp(argv[i], "-smc")) {  /* don't hide match count */
				hidematchcount = 0;
		} else if (!strcmp(argv[i], "-hm")) {   /* hide mode indicator */
				hidemode = 1;
		} else if (!strcmp(argv[i], "-sm")) {  /* don't hide mode indicator */
				hidemode = 0;
        } else if (i + 1 == argc)
			usage();

		/* these options take one argument */
		else if (!strcmp(argv[i], "-g")) {   /* number of columns in grid */
			columns = atoi(argv[++i]);
			if (lines == 0) lines = 1;
		} else if (!strcmp(argv[i], "-l")) { /* number of lines in grid */
			lines = atoi(argv[++i]);
		} else if (!strcmp(argv[i], "-h")) { /* minimum height of one menu line */
	        clineheight += atoi(argv[++i]);
			if (columns == 0) columns = 1;
		} else if (!strcmp(argv[i], "-lp")) {
		    menupaddingv = atoi(argv[++i]);
		} else if (!strcmp(argv[i], "-hp")) {
		    menupaddingh = atoi(argv[++i]);
		} else if (!strcmp(argv[i], "-ig")) {
		    imagegaps = atoi(argv[++i]);
		} else if (!strcmp(argv[i], "-la")) {
		    leftarrow = argv[++i];
		} else if (!strcmp(argv[i], "-ra")) {
		    rightarrow = argv[++i];
		} else if (!strcmp(argv[i], "-m")) /* monitor */
			mon = atoi(argv[++i]);
		else if (!strcmp(argv[i], "-bw")) { /* border width */
            bordercentered = 0;
		    borderwidth = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "-H")) /* hist file location */
			histfile = argv[++i];
	    else if (!strcmp(argv[i], "-x"))   /* window x offset */
			dmx = atoi(argv[++i]);
		else if (!strcmp(argv[i], "-y"))   /* window y offset (from bottom up if -b) */
			dmy = atoi(argv[++i]);
		else if (!strcmp(argv[i], "-z"))   /* make spmenu this wide */
			dmw = atoi(argv[++i]);
		else if (!strcmp(argv[i], "-p"))   /* adds prompt to left of input field */
			prompt = argv[++i];
		else if (!strcmp(argv[i], "-fn"))  /* font or font set */
			strcpy(font, argv[++i]); /* font[0] = argv[++i]; */
		else if (!strcmp(argv[i], "-nmt"))  /* normal mode text */
			strcpy(normtext, argv[++i]);
		else if (!strcmp(argv[i], "-imt")) {  /* insert mode text */
			strcpy(instext, argv[++i]);

        /* dmenu compatibility options */
        } else if (!strcmp(argv[i], "-nb")) {  /* normal background color */
			colors[SchemeItemNorm][ColBg] = argv[++i];
            colors[SchemeMenu][ColBg] = argv[++i];
            colors[SchemeInput][ColBg] = argv[++i];
            colors[SchemePrompt][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-nf")) {  /* normal foreground color */
			colors[SchemeItemNorm][ColFg] = argv[++i];
            colors[SchemeMenu][ColFg] = argv[++i];
            colors[SchemeInput][ColFg] = argv[++i];
            colors[SchemePrompt][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-sb")) {  /* selected background color */
			colors[SchemeItemSel][ColBg] = argv[++i];
            colors[SchemeMenu][ColBg] = argv[++i];
            colors[SchemeInput][ColBg] = argv[++i];
            colors[SchemePrompt][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-sf")) {  /* selected foreground color */
			colors[SchemeItemSel][ColFg] = argv[++i];
            colors[SchemeMenu][ColFg] = argv[++i];
            colors[SchemeInput][ColBg] = argv[++i];
            colors[SchemePrompt][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-is")) { /* image size */
            char buf[255];
            memset(buf, 0, sizeof(buf));
            memcpy(buf, argv[++i], sizeof(buf)-1);

            if(sscanf(buf, "%dx%d", &imagewidth, &imageheight) == 1)
                imageheight = imagewidth;

        /* spmenu colors */
        } else if (!strcmp(argv[i], "-nif")) { /* normal item foreground color */
			colors[SchemeItemNorm][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-nib")) { /* normal item background color */
			colors[SchemeItemNorm][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-sif")) { /* selected item foreground color */
			colors[SchemeItemSel][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-sib")) { /* selected item background color */
			colors[SchemeItemSel][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-mbg")) { /* menu color */
			colors[SchemeMenu][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-pfg")) { /* prompt fg color */
			colors[SchemePrompt][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-pbg")) { /* prompt bg color */
			colors[SchemePrompt][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-ifg")) { /* input fg color */
			colors[SchemeInput][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-pfg")) { /* input bg color */
			colors[SchemeInput][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-shf")) { /* selected highlight foreground color */
			colors[SchemeSelHighlight][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-shf")) { /* selected highlight foreground color */
			colors[SchemeSelHighlight][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-nhf")) {  /* normal highlight foreground color */
			colors[SchemeNormHighlight][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-shb")) { /* selected highlight foreground color */
			colors[SchemeSelHighlight][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-nhb")) { /* normal highlight foreground color */
			colors[SchemeNormHighlight][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-nbg")) { /* numbgcolor */
			colors[SchemeNumber][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-nfg")) { /* numfgcolor */
			colors[SchemeNumber][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-mbg")) { /* mode */
			colors[SchemeMode][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-mfg")) { /* mode */
			colors[SchemeMode][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-laf")) { /* left arrow fg */
			colors[SchemeLArrow][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-raf")) { /* right arrow fg */
			colors[SchemeRArrow][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-lab")) { /* left arrow bg */
			colors[SchemeLArrow][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-rab")) { /* right arrow bg */
			colors[SchemeRArrow][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-bc")) { /* border */
			colors[SchemeBorder][ColBg] = argv[++i];
        }

        /* sgr colors */
		else if (!strcmp(argv[i], "-sgr0")) textcolors[0] = argv[++i]; /* sgr color 0 */
		else if (!strcmp(argv[i], "-sgr1")) textcolors[1] = argv[++i]; /* sgr color 1 */
		else if (!strcmp(argv[i], "-sgr2")) textcolors[2] = argv[++i]; /* sgr color 2 */
		else if (!strcmp(argv[i], "-sgr3")) textcolors[3] = argv[++i]; /* sgr color 3 */
		else if (!strcmp(argv[i], "-sgr4")) textcolors[4] = argv[++i]; /* sgr color 4 */
		else if (!strcmp(argv[i], "-sgr5")) textcolors[5] = argv[++i]; /* sgr color 5 */
		else if (!strcmp(argv[i], "-sgr6")) textcolors[6] = argv[++i]; /* sgr color 6 */
		else if (!strcmp(argv[i], "-sgr7")) textcolors[7] = argv[++i]; /* sgr color 7 */
		else if (!strcmp(argv[i], "-sgr8")) textcolors[8] = argv[++i]; /* sgr color 8 */
		else if (!strcmp(argv[i], "-sgr9")) textcolors[9] = argv[++i]; /* sgr color 9 */
		else if (!strcmp(argv[i], "-sgr10")) textcolors[10] = argv[++i]; /* sgr color 10 */
		else if (!strcmp(argv[i], "-sgr11")) textcolors[11] = argv[++i]; /* sgr color 11 */
		else if (!strcmp(argv[i], "-sgr12")) textcolors[12] = argv[++i]; /* sgr color 12 */
		else if (!strcmp(argv[i], "-sgr13")) textcolors[13] = argv[++i]; /* sgr color 13 */
		else if (!strcmp(argv[i], "-sgr14")) textcolors[14] = argv[++i]; /* sgr color 14 */
		else if (!strcmp(argv[i], "-sgr15")) textcolors[15] = argv[++i]; /* sgr color 15 */
		else if (!strcmp(argv[i], "-cc"))  /* caret color */
			colors[SchemeCaret][ColFg] = argv[++i];
		else if (!strcmp(argv[i], "-w"))   /* embedding window id */
			embed = argv[++i];
    	else if (!strcmp(argv[i], "-n"))   /* preselected item */
		    preselected = atoi(argv[++i]);
		else
			usage();
}
