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
		} else if (!strcmp(argv[i], "-Ps")) { /* password symbol */
			password = argv[++i];
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

void
usage(void)
{
	fputs("spmenu: dynamic menu\n\n"
		  "- Arguments -\n"
		  "spmenu -l <line>      Set line count to stdin\n"
		  "spmenu -h <height>    Set spmenu line height to <height>\n"
		  "spmenu -g <grid>      Set the number of grids to <grid>\n"
          "spmenu -gc            Generate image cache\n"
          "spmenu -rw            Enable relative input width\n"
          "spmenu -nrw           Disable relative input width\n"
          "spmenu -f             Grabs keyboard before reading stdin\n"
		  "spmenu -F             Enable fuzzy matching\n"
		  "spmenu -P             Hide characters\n"
          "spmenu -Ps <symbol>   Set the password symbol to <symbol>\n"
		  "spmenu -p <text>      Set spmenu prompt text to <text>\n"
          "spmenu -a             Enable alpha\n"
          "spmenu -na            Disable alpha\n"
          "spmenu -cp            Color prompt\n"
          "spmenu -ncp           Don't color prompt\n"
          "spmenu -tp            Allow the user to type\n"
          "spmenu -nt            Don't allow typing, the user must select an option\n"
		  "spmenu -x <x offset>  Offset spmenu x position by <x offset>\n"
		  "spmenu -y <y offset>  Offset spmenu y position by <y offset>\n"
          "spmenu -n <line>      Preselect <line> in the list of items\n"
		  "spmenu -z <width>     Width of the spmenu window\n"
          "spmenu -nmt <text>    Set normal mode text to <text>\n"
          "spmenu -imt <text>    Set insert mode text to <text>\n"
		  "spmenu -bw            Width of the border. 0 will disable the border\n"
		  "spmenu -s             Use case-sensitive matching\n"
		  "spmenu -i             Use case-insensitive matching\n"
          "spmenu -nm            Start spmenu in normal mode\n"
          "spmenu -im            Start spmenu in insert mode\n"
		  "spmenu -t             Position spmenu at the top of the screen\n"
		  "spmenu -b             Position spmenu at the bottom of the screen\n"
		  "spmenu -c             Position spmenu at the center of the screen\n"
          "spmenu -hm            Hide mode indicator\n"
          "spmenu -hmc           Hide match count\n"
          "spmenu -sm            Show mode indicator\n"
          "spmenu -smc           Show match count\n"
          "spmenu -xrdb          Load .Xresources on runtime\n"
          "spmenu -nxrdb         Don't load .Xresources on runtime\n"
		  "spmenu -m <monitor>   Specify a monitor to run spmenu on\n"
		  "spmenu -w <window id> Embed spmenu inside <window id>\n"
		  "spmenu -H <hist file> Specify a path to save the history to\n"
          "spmenu -ig <gaps>     Set image gaps to <gaps>\n"
		  "spmenu -lp <padding>  Set the vertical padding\n"
		  "spmenu -hp <padding>  Set the horizontal padding\n"
          "spmenu -la <symbol>   Set the left arrow to <symbol>\n"
          "spmenu -ra <symbol>   Set the right arrow to <symbol>\n"
          "spmenu -is <size>     Image size\n"
          "spmenu -it            Position the image at the top\n"
          "spmenu -ib            Position the image at the bottom\n"
          "spmenu -ic            Position the image in the center\n"
          "spmenu -itc           Position the image in the top center\n"
          "spmenu -wm            Spawn spmenu as a window manager controlled client/window. Useful for testing\n"
          "spmenu -v             Print spmenu version to stdout\n"
          "\n", stdout);
          fputs("- Appearance arguments -\n"
		  "spmenu -fn  <font>    Set the spmenu font to <font>\n"
          "spmenu -nif <color>   Set the normal item foreground color\n"
          "spmenu -nib <color>   Set the normal item background color\n"
          "spmenu -sif <color>   Set the selected item foreground color\n"
          "spmenu -sib <color>   Set the selected item background color\n"
          "spmenu -pfg <color>   Set the prompt foreground color\n"
          "spmenu -pbg <color>   Set the prompt background color\n"
          "spmenu -ifg <color>   Set input foreground color\n"
          "spmenu -ibg <color>   Set input background color\n"
          "spmenu -mbg <color>   Set the menu background color\n"
		  "spmenu -nhf <color>   Set the normal highlight foreground color\n"
		  "spmenu -nhb <color>   Set the normal highlight background color\n"
		  "spmenu -shf <color>   Set the selected highlight foreground color\n"
		  "spmenu -shb <color>   Set the selected highlight background color\n"
		  "spmenu -shb <color>   Set the selected highlight background color\n"
          "spmenu -nfg <color>   Set the foreground color for the match count\n"
          "spmenu -nbg <color>   Set the background color for the match count\n"
          "spmenu -mfg <color>   Set the foreground color for the mode indicator\n"
          "spmenu -mbg <color>   Set the background color for the mode indicator\n"
          "spmenu -laf <color>   Set the left arrow foreground color\n"
          "spmenu -raf <color>   Set the right arrow foreground color\n"
          "spmenu -lab <color>   Set the left arrow background color\n"
          "spmenu -rab <color>   Set the right arrow background color\n"
          "spmenu -cc  <color>   Set the caret color\n"
          "spmenu -bc  <color>   Set the border color\n"
		  "spmenu -sgr0          Set the SGR 0 color\n"
		  "spmenu -sgr1          Set the SGR 1 color\n"
		  "spmenu -sgr2          Set the SGR 2 color\n"
		  "spmenu -sgr3          Set the SGR 3 color\n"
		  "spmenu -sgr4          Set the SGR 4 color\n"
		  "spmenu -sgr5          Set the SGR 5 color\n"
		  "spmenu -sgr6          Set the SGR 6 color\n"
		  "spmenu -sgr7          Set the SGR 7 color\n"
		  "spmenu -sgr8          Set the SGR 8 color\n"
		  "spmenu -sgr9          Set the SGR 9 color\n"
		  "spmenu -sgr10         Set the SGR 10 color\n"
		  "spmenu -sgr11         Set the SGR 11 color\n"
		  "spmenu -sgr12         Set the SGR 12 color\n"
          "spmenu -sgr13         Set the SGR 13 color\n"
		  "spmenu -sgr14         Set the SGR 14 color\n"
		  "spmenu -sgr15         Set the SGR 15 color\n"
          "\n", stdout);
          fputs("- dmenu compatibility -\n"
	      "spmenu -nb <color>    Set the normal background color\n"
		  "spmenu -nf <color>    Set the normal foreground color\n"
		  "spmenu -sb <color>    Set the selected background color\n"
		  "spmenu -sf <color>    Set the selected foreground color\n"
          , stdout);
	exit(1);
}
