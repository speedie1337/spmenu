void
readargs(int argc, char *argv[])
{
    int i = 0;
    int j = 0;

    int cxrdb = 0;
    int hxrdb = 0;

    // check if we should load the xrdb/config, because it needs to be loaded before arguments are checked (internal -> xresources -> arguments)
    for (j = 1; j < argc; j++) {
		if (!strcmp(argv[j], "-xrdb") || (!strcmp(argv[j], "--xrdb"))) {
			xresources = 1;
        } else if (!strcmp(argv[j], "-nxrdb") || (!strcmp(argv[j], "--no-xrdb"))) {
			xresources = 0;
        } else if (!strcmp(argv[j], "-lcfg") || (!strcmp(argv[j], "--load-config"))) {
			loadconfig = 1;
        } else if (!strcmp(argv[j], "-ncfg") || (!strcmp(argv[j], "--no-load-config"))) {
			loadconfig = 0;
        }
    }

    // init/read xrdb
    if (xresources) {
        XrmInitialize();

        if (loadconfig) {
            cxrdb = system("[ -e $HOME/.config/spmenu/spmenurc ] && xrdb -override $HOME/.config/spmenu/spmenurc");
            hxrdb = system("[ -e $HOME/.spmenurc ] && xrdb -override $HOME/.spmenurc");
        }

        // avoid an annoying warning gcc will spit out when you don't care about the result
        if (!cxrdb || !hxrdb || cxrdb || hxrdb || xresources) load_xresources();
    }

    // no arguments
	for (i = 1; i < argc; i++)
        if (!strcmp(argv[i], "-v") || (!strcmp(argv[i], "--version"))) {      // prints version information
			puts("spmenu-"VERSION);
			exit(0);
        } else if (!strcmp(argv[i], "-h") || (!strcmp(argv[i], "--help"))) { // help
            usage();
		} else if (!strcmp(argv[i], "-it") || (!strcmp(argv[i], "--image-top"))) { // image: top
			imageposition = 0;
		} else if (!strcmp(argv[i], "-ib") || (!strcmp(argv[i], "--image-bottom"))) { // image: bottom
			imageposition = 1;
		} else if (!strcmp(argv[i], "-ic") || (!strcmp(argv[i], "--image-center"))) { // image: center
			imageposition = 2;
		} else if (!strcmp(argv[i], "-itc") || (!strcmp(argv[i], "--image-topcenter"))) { // image: top center
			imageposition = 3;
		} else if (!strcmp(argv[i], "-b") || (!strcmp(argv[i], "--bottom"))) { // appears at the bottom of the screen
			menuposition = 0;
		} else if (!strcmp(argv[i], "-t") || (!strcmp(argv[i], "--top"))) { // appears at the top of the screen
			menuposition = 1;
        } else if (!strcmp(argv[i], "-c") || (!strcmp(argv[i], "--center"))) {  // appears at the center of the screen
		    menuposition = 2;
		} else if (!strcmp(argv[i], "-nm") || (!strcmp(argv[i], "--normal"))) { // normal mode
			mode = 0;
		} else if (!strcmp(argv[i], "-im") || (!strcmp(argv[i], "--insert"))) { // insert mode
			mode = 1;
        } else if (!strcmp(argv[i], "-f") || (!strcmp(argv[i], "--fast"))) { // grabs keyboard before reading stdin
			fast = 1;
        } else if (!strcmp(argv[i], "-rw") || (!strcmp(argv[i], "--relative-width"))) {  // relative width
			accuratewidth = 1;
        } else if (!strcmp(argv[i], "-nrw") || (!strcmp(argv[i], "--no-relative-width"))) {   // no relative width
			accuratewidth = 0;
        } else if (!strcmp(argv[i], "-F") || (!strcmp(argv[i], "--fuzzy"))) {   // fuzzy matching
		     fuzzy = 1;
        } else if (!strcmp(argv[i], "-NF") || (!strcmp(argv[i], "--no-fuzzy"))) {   // no fuzzy matching
		     fuzzy = 0;
        } else if (!strcmp(argv[i], "-s") || (!strcmp(argv[i], "--case-sensitive")) || (!strcmp(argv[i], "--sensitive"))) { // case-sensitive item matching
            casesensitive = 1;
        } else if (!strcmp(argv[i], "-ns") || (!strcmp(argv[i], "--case-insensitive")) || (!strcmp(argv[i], "--insensitive"))) { // case-insensitive item matching
            casesensitive = 0;
        } else if (!strcmp(argv[i], "-nso") || (!strcmp(argv[i], "--no-sort"))) { // don't sort
            sortmatches = 0;
        } else if (!strcmp(argv[i], "-so") || (!strcmp(argv[i], "--sort"))) { // sort
            sortmatches = 1;
        } else if (!strcmp(argv[i], "-gc") || (!strcmp(argv[i], "--generate-cache"))) { // generate image cache
            generatecache = 1;
        } else if (!strcmp(argv[i], "-ngc") || (!strcmp(argv[i], "--no-generate-cache"))) { // don't generate image cache
            generatecache = 0;
		} else if (!strcmp(argv[i], "-wm") || (!strcmp(argv[i], "--managed")) || (!strcmp(argv[i], "--x11-client"))) { // display as managed wm window
				managed = 1;
		} else if (!strcmp(argv[i], "-nwm") || (!strcmp(argv[i], "--unmanaged"))) { // don't display as managed wm window
				managed = 0;
		} else if (!strcmp(argv[i], "-na") || (!strcmp(argv[i], "--no-alpha"))) { // disable alpha
				alpha = 0;
		} else if (!strcmp(argv[i], "-a") || (!strcmp(argv[i], "--alpha"))) { // alpha
				alpha = 1;
        } else if (!strcmp(argv[i], "-tp") || (!strcmp(argv[i], "--allow-typing"))) { //  allow the user to type
                type = 1;
        } else if (!strcmp(argv[i], "-nt") || (!strcmp(argv[i], "--no-allow-typing"))) { //  don't allow the user to type
                type = 0;
		} else if (!strcmp(argv[i], "-P") || (!strcmp(argv[i], "--password"))) {  // is the input a password
				passwd = 1;
		} else if (!strcmp(argv[i], "-hmc") || (!strcmp(argv[i], "--hide-match-count"))) {   // hide match count
				hidematchcount = 1;
		} else if (!strcmp(argv[i], "-smc") || (!strcmp(argv[i], "--show-match-count"))) {  // show match count
				hidematchcount = 0;
		} else if (!strcmp(argv[i], "-hm") || (!strcmp(argv[i], "--hide-mode"))) {   // hide mode indicator
				hidemode = 1;
		} else if (!strcmp(argv[i], "-sm") || (!strcmp(argv[i], "--show-mode"))) {  // show mode indicator
				hidemode = 0;
		} else if (!strcmp(argv[i], "-hla") || (!strcmp(argv[i], "--hide-left-arrow"))) {   // hide left arrow
				hidelarrow = 1;
		} else if (!strcmp(argv[i], "-sla") || (!strcmp(argv[i], "--show-left-arrow"))) {  // show left arrow
				hidelarrow = 0;
		} else if (!strcmp(argv[i], "-hra") || (!strcmp(argv[i], "--hide-right-arrow"))) {   // hide right arrow
				hiderarrow = 1;
		} else if (!strcmp(argv[i], "-sra") || (!strcmp(argv[i], "--show-right-arrow"))) {  // show right arrow
				hiderarrow = 0;
		} else if (!strcmp(argv[i], "-hpr") || (!strcmp(argv[i], "--hide-prompt"))) {   // hide prompt
				hideprompt = 1;
		} else if (!strcmp(argv[i], "-hpl") || (!strcmp(argv[i], "--hide-powerline"))) {   // hide powerline
				hidepowerline = 1;
		} else if (!strcmp(argv[i], "-spr") || (!strcmp(argv[i], "--show-prompt"))) {  // show prompt
				hideprompt = 0;
		} else if (!strcmp(argv[i], "-spl") || (!strcmp(argv[i], "--show-powerline"))) {  // show prompt
				hidepowerline = 0;
		} else if (!strcmp(argv[i], "-hc") || (!strcmp(argv[i], "--hide-cursor"))) {   // hide cursor
				hidecursor = 1;
		} else if (!strcmp(argv[i], "-sc") || (!strcmp(argv[i], "--show-cursor"))) {  // show cursor
				hidecursor = 0;
		} else if (!strcmp(argv[i], "-hhl") || (!strcmp(argv[i], "--hide-highlighting"))) {   // hide highlighting
				hidehighlight = 1;
		} else if (!strcmp(argv[i], "-shl") || (!strcmp(argv[i], "--show-highlighting"))) {  // show highlighting
				hidehighlight = 0;
		} else if (!strcmp(argv[i], "-hi") || (!strcmp(argv[i], "--hide-image"))) {   // hide image
				hideimage = 1;
		} else if (!strcmp(argv[i], "-si") || (!strcmp(argv[i], "--show-image"))) {  // show image
				hideimage = 0;
		} else if (!strcmp(argv[i], "-ip") || (!strcmp(argv[i], "--indent"))) {  // indent to prompt width
				indentitems = 1;
		} else if (!strcmp(argv[i], "-nip") || (!strcmp(argv[i], "--no-indent"))) {  // don't indent to prompt width
				indentitems = 0;
		} else if (!strcmp(argv[i], "-ci") || (!strcmp(argv[i], "--color-items"))) {  // color items
				coloritems = 1;
		} else if (!strcmp(argv[i], "-nci") || (!strcmp(argv[i], "--no-color-items"))) {  // don't color items
				coloritems = 0;
        } else if (i + 1 == argc) {
                // any of the arguments we checked first
                if ((!strcmp(argv[i], "-xrdb")
                    || !strcmp(argv[i], "-nxrdb")
                    || !strcmp(argv[i], "--xrdb")
                    || !strcmp(argv[i], "--no-xrdb")
                    || !strcmp(argv[i], "-lcfg")
                    || !strcmp(argv[i], "-ncfg")
                    || !strcmp(argv[i], "--load-config")
                    || !strcmp(argv[i], "--no-load-config")
                ))
                    continue;
                else
                    fprintf(stderr, "spmenu: Invalid argument: '%s'\n", argv[i]);

        // dmenu compatibility arguments
		} else if (!strcmp(argv[i], "-i")) { // case-insensitive item matching, for compatibility reasons
            casesensitive = 0;
        } else if (!strcmp(argv[i], "-S")) { // don't sort
            sortmatches = 0;

		// these options take one argument
        } else if (!strcmp(argv[i], "-g") || (!strcmp(argv[i], "--columns"))) { // number of columns in grid
			columns = atoi(argv[++i]);
			if (lines == 0) lines = 1;
		} else if (!strcmp(argv[i], "-mc") || (!strcmp(argv[i], "--max-cache"))) { // max cache
			maxcache = atoi(argv[++i]);
		} else if (!strcmp(argv[i], "-") || (!strcmp(argv[i], "--password-symbol"))) { // password symbol
			password = argv[++i];
		} else if (!strcmp(argv[i], "-l") || (!strcmp(argv[i], "--lines"))) { // number of lines in grid
			lines = atoi(argv[++i]);
		} else if (!strcmp(argv[i], "-mh") || (!strcmp(argv[i], "--lineheight"))) { // line height
	        lineheight += atoi(argv[++i]);
			if (columns == 0) columns = 1;
		} else if (!strcmp(argv[i], "-mw") || (!strcmp(argv[i], "--min-width"))) { // line height
	        minwidth = atoi(argv[++i]);
		} else if (!strcmp(argv[i], "-lp") || (!strcmp(argv[i], "--vertical-padding"))) { // vertical padding
		    menupaddingv = atoi(argv[++i]);
		} else if (!strcmp(argv[i], "-hp") || (!strcmp(argv[i], "--horizontal-padding"))) { // horizontal padding
		    menupaddingh = atoi(argv[++i]);
		} else if (!strcmp(argv[i], "-pri") || (!strcmp(argv[i], "--priority"))) { // high priority (csv format)
            hpitems = tokenize(argv[++i], ",", &hplength);
		} else if (!strcmp(argv[i], "-ig") || (!strcmp(argv[i], "--image-gaps"))) { // gaps between image
		    imagegaps = atoi(argv[++i]);
		} else if (!strcmp(argv[i], "-la") || (!strcmp(argv[i], "--left-arrow-symbol"))) { // left arrow symbol
		    leftarrow = argv[++i];
		} else if (!strcmp(argv[i], "-ra") || (!strcmp(argv[i], "--right-arrow-symbol"))) { // right arrow symbol
		    rightarrow = argv[++i];
		} else if (!strcmp(argv[i], "-m") || (!strcmp(argv[i], "--monitor"))) // monitor
			mon = atoi(argv[++i]);
		else if (!strcmp(argv[i], "-bw") || (!strcmp(argv[i], "--border-width"))) { // border width
		    borderwidth = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "-H") || (!strcmp(argv[i], "--hist-file"))) // hist file location
			histfile = argv[++i];
	    else if (!strcmp(argv[i], "-x") || (!strcmp(argv[i], "--x-position")))  // window x offset
			dmx = atoi(argv[++i]);
		else if (!strcmp(argv[i], "-y") || (!strcmp(argv[i], "--y-position")))   // window y offset (from bottom up if -b)
			dmy = atoi(argv[++i]);
		else if (!strcmp(argv[i], "-z") || (!strcmp(argv[i], "--width")))   // make spmenu this wide
			dmw = atoi(argv[++i]);
		else if (!strcmp(argv[i], "-p") || (!strcmp(argv[i], "--prompt")))   // adds prompt to left of input field
			prompt = argv[++i];
		else if (!strcmp(argv[i], "-fn") || (!strcmp(argv[i], "--font")))  // font or font set
            fonts[0] = argv[++i];
		else if (!strcmp(argv[i], "-nmt") || (!strcmp(argv[i], "--normal-mode-text")))  // normal mode text
			strcpy(normtext, argv[++i]);
		else if (!strcmp(argv[i], "-imt") || (!strcmp(argv[i], "--insert-mode-text"))) {  // insert mode text
			strcpy(instext, argv[++i]);

        // dmenu compatibility options
        } else if (!strcmp(argv[i], "-nb")) {  // normal background color
			colors[SchemeItemNorm][ColBg] = argv[++i];
            colors[SchemeMenu][ColBg] = argv[++i];
            colors[SchemeInput][ColBg] = argv[++i];
            colors[SchemePrompt][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-nf")) {  // normal foreground color
			colors[SchemeItemNorm][ColFg] = argv[++i];
            colors[SchemeMenu][ColFg] = argv[++i];
            colors[SchemeInput][ColFg] = argv[++i];
            colors[SchemePrompt][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-sb")) {  // selected background color
			colors[SchemeItemSel][ColBg] = argv[++i];
            colors[SchemeMenu][ColBg] = argv[++i];
            colors[SchemeInput][ColBg] = argv[++i];
            colors[SchemePrompt][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-sf")) {  // selected foreground color
			colors[SchemeItemSel][ColFg] = argv[++i];
            colors[SchemeMenu][ColFg] = argv[++i];
            colors[SchemeInput][ColBg] = argv[++i];
            colors[SchemePrompt][ColFg] = argv[++i];

        // more
        } else if (!strcmp(argv[i], "-is") || (!strcmp(argv[i], "--image-size"))) { // image size
            char buf[255];
            memset(buf, 0, sizeof(buf));
            memcpy(buf, argv[++i], sizeof(buf)-1);

            if(sscanf(buf, "%dx%d", &imagewidth, &imageheight) == 1)
                imageheight = imagewidth;

        } else if (!strcmp(argv[i], "-w") || (!strcmp(argv[i], "--embed"))) { // embedding window id
			embed = argv[++i];
        } else if (!strcmp(argv[i], "-n") || (!strcmp(argv[i], "--preselect"))) { // preselected item
		    preselected = atoi(argv[++i]);

        // spmenu colors
        } else if (!strcmp(argv[i], "-nif") || (!strcmp(argv[i], "--normal-item-foreground"))) { // normal item foreground color
			colors[SchemeItemNorm][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-nib") || (!strcmp(argv[i], "--normal-item-background"))) { // normal item background color
			colors[SchemeItemNorm][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-sif") || (!strcmp(argv[i], "--selected-item-foreground"))) { // selected item foreground color
			colors[SchemeItemSel][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-sib") || (!strcmp(argv[i], "--selected-item-background"))) { // selected item background color
			colors[SchemeItemSel][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-npf") || (!strcmp(argv[i], "--normal-item-priority-foreground"))) { // normal item priority foreground color
			colors[SchemeItemNormPri][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-npb") || (!strcmp(argv[i], "--normal-item-priority-background"))) { // normal item priority background color
			colors[SchemeItemNormPri][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-spf") || (!strcmp(argv[i], "--selected-item-priority-foreground"))) { // selected item priority foreground color
			colors[SchemeItemSelPri][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-spb") || (!strcmp(argv[i], "--selected-item-priority-background"))) { // selected item priority background color
			colors[SchemeItemSelPri][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-mnbg") || (!strcmp(argv[i], "--menu-background"))) { // menu color
			colors[SchemeMenu][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-pfg") || (!strcmp(argv[i], "--prompt-foreground"))) { // prompt fg color
			colors[SchemePrompt][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-pbg") || (!strcmp(argv[i], "--prompt-background"))) { // prompt bg color
			colors[SchemePrompt][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-ifg") || (!strcmp(argv[i], "--input-foreground"))) { // input fg color
			colors[SchemeInput][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-pfg") || (!strcmp(argv[i], "--input-background"))) { // input bg color
			colors[SchemeInput][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-nhb") || (!strcmp(argv[i], "--normal-highlight-background"))) { // normal highlight background color
			colors[SchemeNormHighlight][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-shf") || (!strcmp(argv[i], "--normal-highlight-foreground"))) { // normal highlight foreground color
			colors[SchemeNormHighlight][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-nhf") || (!strcmp(argv[i], "--selected-highlight-foreground"))) { // selected highlight foreground color
			colors[SchemeSelHighlight][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-shb") || (!strcmp(argv[i], "--selected-highlight-background"))) { // selected highlight background color
			colors[SchemeSelHighlight][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-nbg") || (!strcmp(argv[i], "--number-background"))) { // numbgcolor
			colors[SchemeNumber][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-nfg") || (!strcmp(argv[i], "--number-foreground"))) { // numfgcolor
			colors[SchemeNumber][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-mbg") || (!strcmp(argv[i], "--mode-background"))) { // mode
			colors[SchemeMode][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-mfg") || (!strcmp(argv[i], "--mode-foreground"))) { // mode
			colors[SchemeMode][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-laf") || (!strcmp(argv[i], "--left-arrow-foreground"))) { // left arrow fg
			colors[SchemeLArrow][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-raf") || (!strcmp(argv[i], "--right-arrow-foreground"))) { // right arrow fg
			colors[SchemeRArrow][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-lab") || (!strcmp(argv[i], "--left-arrow-background"))) { // left arrow bg
			colors[SchemeLArrow][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-rab") || (!strcmp(argv[i], "--right-arrow-background"))) { // right arrow bg
			colors[SchemeRArrow][ColFg] = argv[++i];
        } else if (!strcmp(argv[i], "-bc") || (!strcmp(argv[i], "--border-background"))) { // border
			colors[SchemeBorder][ColBg] = argv[++i];
        } else if (!strcmp(argv[i], "-cc") || (!strcmp(argv[i], "--caret-foreground"))) {   // caret color
			colors[SchemeCaret][ColFg] = argv[++i];
        }

        // sgr colors
		else if (!strcmp(argv[i], "-sgr0") || (!strcmp(argv[i], "--sgr0"))) textcolors[0] = argv[++i]; // sgr color 0
		else if (!strcmp(argv[i], "-sgr1") || (!strcmp(argv[i], "--sgr1"))) textcolors[1] = argv[++i]; // sgr color 1
		else if (!strcmp(argv[i], "-sgr2") || (!strcmp(argv[i], "--sgr2"))) textcolors[2] = argv[++i]; // sgr color 2
		else if (!strcmp(argv[i], "-sgr3") || (!strcmp(argv[i], "--sgr3"))) textcolors[3] = argv[++i]; // sgr color 3
		else if (!strcmp(argv[i], "-sgr4") || (!strcmp(argv[i], "--sgr4"))) textcolors[4] = argv[++i]; // sgr color 4
		else if (!strcmp(argv[i], "-sgr5") || (!strcmp(argv[i], "--sgr5"))) textcolors[5] = argv[++i]; // sgr color 5
		else if (!strcmp(argv[i], "-sgr6") || (!strcmp(argv[i], "--sgr6"))) textcolors[6] = argv[++i]; // sgr color 6
		else if (!strcmp(argv[i], "-sgr7") || (!strcmp(argv[i], "--sgr7"))) textcolors[7] = argv[++i]; // sgr color 7
		else if (!strcmp(argv[i], "-sgr8") || (!strcmp(argv[i], "--sgr8"))) textcolors[8] = argv[++i]; // sgr color 8
		else if (!strcmp(argv[i], "-sgr9") || (!strcmp(argv[i], "--sgr9"))) textcolors[9] = argv[++i]; // sgr color 9
		else if (!strcmp(argv[i], "-sgr10") || (!strcmp(argv[i], "--sgr10"))) textcolors[10] = argv[++i]; // sgr color 10
		else if (!strcmp(argv[i], "-sgr11") || (!strcmp(argv[i], "--sgr11"))) textcolors[11] = argv[++i]; // sgr color 11
		else if (!strcmp(argv[i], "-sgr12") || (!strcmp(argv[i], "--sgr12"))) textcolors[12] = argv[++i]; // sgr color 12
		else if (!strcmp(argv[i], "-sgr13") || (!strcmp(argv[i], "--sgr13"))) textcolors[13] = argv[++i]; // sgr color 13
		else if (!strcmp(argv[i], "-sgr14") || (!strcmp(argv[i], "--sgr14"))) textcolors[14] = argv[++i]; // sgr color 14
		else if (!strcmp(argv[i], "-sgr15") || (!strcmp(argv[i], "--sgr15"))) textcolors[15] = argv[++i]; // sgr color 15
		else
            if ((!strcmp(argv[i], "-xrdb")
                || !strcmp(argv[i], "-nxrdb")
                || !strcmp(argv[i], "--xrdb")
                || !strcmp(argv[i], "--no-xrdb")
                || !strcmp(argv[i], "-lcfg")
                || !strcmp(argv[i], "-ncfg")
                || !strcmp(argv[i], "--load-config")
                || !strcmp(argv[i], "--no-load-config")
            ))
                continue;
            else
                fprintf(stderr, "spmenu: Invalid argument: '%s'\n", argv[i]);

    if (casesensitive) {
        fstrncmp = strncmp;
        fstrstr = strstr;
    } else {
	    fstrncmp = strncasecmp;
	    fstrstr = cistrstr;
    }
}

void
usage(void)
{
	fputs("spmenu: dynamic menu\n\n"
		  "- Arguments -\n"
		  "spmenu -l,       --lines <line>                              Set line count to stdin\n"
		  "spmenu -mh,      --lineheight <height>                       Set spmenu line height to <height>\n"
          "spmenu -mw,      --min-width <width>                         Set minimum width to <width>\n"
		  "spmenu -g,       --columns <grid>                            Set the number of grids to <grid>\n"
          "spmenu -gc,      --generate-cache                            Generate image cache\n"
          "spmenu -ngc,     --no-generate-cache                         Don't generate image cache\n"
          "spmenu -mc       --max-cache <size>                          Set max image cache size to <size>\n"
          "spmenu -rw,      --relative-width                            Enable relative input width\n"
          "spmenu -nrw,     --no-relative-width                         Disable relative input width\n"
          "spmenu -f,       --fast                                      Grabs keyboard before reading stdin\n"
		  "spmenu -F,       --fuzzy                                     Enable fuzzy matching\n"
		  "spmenu -NF,      --no-fuzzy                                  Disable fuzzy matching\n"
		  "spmenu -P,       --password                                  Hide characters\n"
          "spmenu -Ps,      --password-symbol <symbol>                  Set the password symbol to <symbol>\n"
		  "spmenu -p,       --prompt <text>                             Set spmenu prompt text to <text>\n"
          "spmenu -ip,      --indent                                    Indent items to prompt width\n"
          "spmenu -nip,     --no-indent                                 Don't indent items to prompt width\n"
          "spmenu -ci,      --color-items                               Color items\n"
          "spmenu -nci,     --no-color-items                            Don't color items\n"
          "spmenu -a,       --alpha                                     Enable alpha\n"
          "spmenu -na,      --no-alpha                                  Disable alpha\n"
          "spmenu -tp,      --allow-typing                              Allow the user to type\n"
          "spmenu -nt,      --no-allow-typing                           Don't allow typing, the user must select an option\n"
		  "spmenu -x,       --x-position <x offset>                     Offset spmenu x position by <x offset>\n"
		  "spmenu -y,       --y-position <y offset>                     Offset spmenu y position by <y offset>\n"
          "spmenu -n,       --preselect <line>                          Preselect <line> in the list of items\n"
		  "spmenu -z,       --width <width>                             Width of the spmenu window\n"
          "spmenu -nmt,     --normal-mode-text <text>                   Set normal mode text to <text>\n"
          "spmenu -imt,     --insert-mode-text <text>                   Set insert mode text to <text>\n"
		  "spmenu -bw,      --border-width <width>                      Set width of the border to <width>. 0 will disable the border\n"
          "spmenu -so,      --sort                                      Sort matches\n"
          "spmenu -nso,     --no-sort                                   Don't sort matches\n"
          "spmenu -pri,     --priority <pri1,pri2,pri3>                 Specify a list of items that take priority\n"
		  "spmenu -s,       --case-sensitive                            Use case-sensitive matching\n"
		  "spmenu -ns,      --case-insensitive                          Use case-insensitive matching\n"
          "spmenu -nm,      --normal                                    Start spmenu in normal mode\n"
          "spmenu -im,      --insert                                    Start spmenu in insert mode\n"
		  "spmenu -t,       --top                                       Position spmenu at the top of the screen\n"
		  "spmenu -b,       --bottom                                    Position spmenu at the bottom of the screen\n"
		  "spmenu -c,       --center                                    Position spmenu at the center of the screen\n"
          "\n", stdout);

    fputs("spmenu -hm,      --hide-mode                                 Hide mode indicator\n"
          "spmenu -hmc,     --hide-match-count                          Hide match count\n"
          "spmenu -hla,     --hide-left-arrow                           Hide left arrow\n"
          "spmenu -hra,     --hide-right-arrow                          Hide right arrow\n"
          "spmenu -hpr,     --hide-prompt                               Hide prompt\n"
          "spmenu -hpl,     --hide-powerline                            Hide powerline\n"
          "spmenu -hc,      --hide-cursor                               Hide cursor\n"
          "spmenu -hhl,     --hide-highlighting                         Hide highlight\n"
          "spmenu -hi,      --hide-image                                Hide image\n"
          "spmenu -sm,      --show-mode                                 Show mode indicator\n"
          "spmenu -smc,     --show-match-count                          Show match count\n"
          "spmenu -sla,     --show-left-arrow                           Show left arrow\n"
          "spmenu -sra,     --show-right-arrow                          Show right arrow\n"
          "spmenu -spr,     --show-prompt                               Show prompt\n"
          "spmenu -sc,      --show-cursor                               Show cursor\n"
          "spmenu -shl,     --show-highlighting                         Show highlight\n"
          "spmenu -si,      --show-image                                Show image\n"
          "spmenu -xrdb,    --xrdb                                      Load .Xresources on runtime\n"
          "spmenu -nxrdb,   --no-xrdb                                   Don't load .Xresources on runtime\n"
		  "spmenu -m,       --monitor <monitor>                         Specify a monitor to run spmenu on\n"
		  "spmenu -w,       --embed <window id>                         Embed spmenu inside <window id>\n"
		  "spmenu -H,       --hist-file <hist file>                     Specify a path to save the history to\n"
          "spmenu -ig,      --image-gaps <gaps>                         Set image gaps to <gaps>\n"
		  "spmenu -lp,      --vertical-padding <padding>                Set the vertical padding\n"
		  "spmenu -hp,      --horizontal-padding <padding>              Set the horizontal padding\n"
          "spmenu -la,      --left-arrow-symbol <symbol>                Set the left arrow to <symbol>\n"
          "spmenu -ra,      --right-arrow-symbol <symbol>               Set the right arrow to <symbol>\n"
          "spmenu -is,      --image-size <size>                         Image size\n"
          "spmenu -it,      --image-top                                 Position the image at the top\n"
          "spmenu -ib,      --image-bottom                              Position the image at the bottom\n"
          "spmenu -ic,      --image-center                              Position the image in the center\n"
          "spmenu -itc,     --image-topcenter                           Position the image in the top center\n"
          "spmenu -wm,      --managed, --x11-client                     Spawn spmenu as a window manager controlled client/window. Useful for testing\n"
          "spmenu -nwm,     --unmanaged                                 Don't spawn spmenu as a window manager controlled client/window. Useful for testing\n"
          "spmenu -lcfg,    --load-config                               Load spmenu configuration (~/.spmenu or ~/.config/spmenu/spmenurc)\n"
          "spmenu -ncfg,    --no-load-config                            Don't load spmenu configuration (~/.spmenu or ~/.config/spmenu/spmenurc)\n"
          "spmenu -v,       --version                                   Print spmenu version to stdout\n"
          "\n", stdout);

    fputs("- Appearance arguments -\n"
		  "spmenu -fn,      --font  <font>                              Set the spmenu font to <font>\n"
          "spmenu -nif,     --normal-item-foreground <color>            Set the normal item foreground color\n"
          "spmenu -nib,     --normal-item-background <color>            Set the normal item background color\n"
          "spmenu -sif,     --selected-item-foreground <color>          Set the selected item foreground color\n"
          "spmenu -sib,     --selected-item-background <color>          Set the selected item background color\n"
          "spmenu -npf,     --normal-item-priority-foreground <color>   Set the normal item (high priority) foreground color\n"
          "spmenu -npb,     --normal-item-priority-background <color>   Set the normal item (high priority) background color\n"
          "spmenu -spf,     --selected-item-priority-foreground <color> Set the selected item (high priority) foreground color\n"
          "spmenu -spb,     --selected-item-priority-background <color> Set the selected item (high priority) background color\n"
          "spmenu -pfg,     --prompt-foreground <color>                 Set the prompt foreground color\n"
          "spmenu -pbg,     --prompt-background <color>                 Set the prompt background color\n"
          "spmenu -ifg,     --input-foreground <color>                  Set input foreground color\n"
          "spmenu -ibg,     --input-background <color>                  Set input background color\n"
          "spmenu -mnbg,    --menu-background <color>                   Set the menu background color\n"
		  "spmenu -nhf,     --normal-highlight-foreground <color>       Set the normal highlight foreground color\n"
		  "spmenu -nhb,     --normal-highlight-background <color>       Set the normal highlight background color\n"
		  "spmenu -shf,     --selected-highlight-foreground <color>     Set the selected highlight foreground color\n"
		  "spmenu -shb,     --selected-highlight-background <color>     Set the selected highlight background color\n"
          "spmenu -nfg,     --number-foreground <color>                 Set the foreground color for the match count\n"
          "spmenu -nbg,     --number-background <color>                 Set the background color for the match count\n"
          "spmenu -mfg,     --mode-foreground <color>                   Set the foreground color for the mode indicator\n"
          "spmenu -mbg,     --mode-background <color>                   Set the background color for the mode indicator\n"
          "spmenu -laf,     --left-arrow-foreground <color>             Set the left arrow foreground color\n"
          "spmenu -raf,     --right-arrow-foreground <color>            Set the right arrow foreground color\n"
          "spmenu -lab,     --left-arrow-background <color>             Set the left arrow background color\n"
          "spmenu -rab,     --right-arrow-background <color>            Set the right arrow background color\n"
          "spmenu -cc,      --caret-foreground <color>                  Set the caret color\n"
          "spmenu -bc,      --border-background <color>                 Set the border color\n"
          "\n", stdout);
    fputs("spmenu -sgr0,    --sgr0 <color>                              Set the SGR 0 color\n"
		  "spmenu -sgr1,    --sgr1 <color>                              Set the SGR 1 color\n"
		  "spmenu -sgr2,    --sgr2 <color>                              Set the SGR 2 color\n"
		  "spmenu -sgr3,    --sgr3 <color>                              Set the SGR 3 color\n"
		  "spmenu -sgr4,    --sgr4 <color>                              Set the SGR 4 color\n"
		  "spmenu -sgr5,    --sgr5 <color>                              Set the SGR 5 color\n"
		  "spmenu -sgr6,    --sgr6 <color>                              Set the SGR 6 color\n"
		  "spmenu -sgr7,    --sgr7 <color>                              Set the SGR 7 color\n"
		  "spmenu -sgr8,    --sgr8 <color>                              Set the SGR 8 color\n"
		  "spmenu -sgr9,    --sgr9 <color>                              Set the SGR 9 color\n"
		  "spmenu -sgr10,   --sgr10 <color>                             Set the SGR 10 color\n"
		  "spmenu -sgr11,   --sgr11 <color>                             Set the SGR 11 color\n"
		  "spmenu -sgr12,   --sgr12 <color>                             Set the SGR 12 color\n"
          "spmenu -sgr13,   --sgr13 <color>                             Set the SGR 13 color\n"
		  "spmenu -sgr14,   --sgr14 <color>                             Set the SGR 14 color\n"
		  "spmenu -sgr15,   --sgr15 <color>                             Set the SGR 15 color\n"
          "\n", stdout);
    fputs("- dmenu compatibility -\n"
          "spmenu -S                                                    Don't sort matches\n"
		  "spmenu -i                                                    Use case-insensitive matching\n"
	      "spmenu -nb <color>                                           Set the normal background color\n"
		  "spmenu -nf <color>                                           Set the normal foreground color\n"
		  "spmenu -sb <color>                                           Set the selected background color\n"
		  "spmenu -sf <color>                                           Set the selected foreground color\n"
          , stdout);
	exit(1);
}
