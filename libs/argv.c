/* See LICENSE file for copyright and license details. */
void readargs(int argc, char *argv[]) {
    int i = 0;
    int j = 0;
    int k = 0;

    // check if we should load the xrdb/config, because it needs to be loaded before arguments are checked
    // priority: internal -> config -> xresources -> arguments
    for (j = 1; j < argc; j++) {
        if (!strcmp(argv[j], "-lcfg") || (!strcmp(argv[j], "--load-config"))) {
            loadconfig = 1;
        } else if (!strcmp(argv[j], "-ncfg") || (!strcmp(argv[j], "--no-load-config"))) {
            loadconfig = 0;
        } else if (!strcmp(argv[j], "-ltm") || (!strcmp(argv[j], "--load-theme"))) {
            loadtheme = 1;
        } else if (!strcmp(argv[j], "-nltm") || (!strcmp(argv[j], "--no-load-theme"))) {
            loadtheme = 0;
        } else if (!strcmp(argv[j], "-lbi") || (!strcmp(argv[j], "--load-binds"))) {
            loadbinds = 1;
        } else if (!strcmp(argv[j], "-nlbi") || (!strcmp(argv[j], "--no-load-binds"))) {
            loadbinds = 0;
        } else if (!strcmp(argv[j], "-x11") || (!strcmp(argv[j], "--x11"))) {
            protocol = 0;
            protocol_override = 1;
        } else if (!strcmp(argv[j], "-wl") || (!strcmp(argv[j], "--wayland"))) {
            protocol = 1;
            protocol_override = 1;
#if USECONFIG
        } else if (!strcmp(argv[j], "-cf") || (!strcmp(argv[j], "--config-file"))) { // specify a config file
            if (argv[j+1]) {
                configfile = argv[++j];
            } else {
                die("This argument requires a second argument.");
            }
        } else if (!strcmp(argv[j], "-bf") || (!strcmp(argv[j], "--bind-file"))) { // specify a bind file
            if (argv[j+1]) {
                bindsfile = argv[++j];
            } else {
                die("This argument requires a second argument.");
            }
        } else if (!strcmp(argv[j], "-tm") || (!strcmp(argv[j], "--theme"))) { // specify a theme
            if (argv[j+1]) {
                themefile = argv[++j];
            } else {
                die("This argument requires a second argument.");
            }
#endif
        }
    }

#if USECONFIG
    conf_init();
#endif

    for (k = 1; k < argc; k++) {
        if (!strcmp(argv[k], "-xrdb") || (!strcmp(argv[k], "--xrdb"))) {
            xresources = 1;
        } else if (!strcmp(argv[k], "-nxrdb") || (!strcmp(argv[k], "--no-xrdb"))) {
            xresources = 0;
        } else if (!strcmp(argv[k], "-gbc") || (!strcmp(argv[k], "--global-colors"))) {
            globalcolors = 1;
        } else if (!strcmp(argv[k], "-ngbc") || (!strcmp(argv[k], "--no-global-colors"))) {
            globalcolors = 0;
        }
    }

#if USEWAYLAND
    if (protocol) {
        if (connect_display(&state)) {
            protocol = 0;
        }
    }
#endif

    // init/read xrdb
#if USEX
    if (xresources && !protocol) {
#if USEXRESOURCES
        XrmInitialize();
        load_xresources();
#else
        ; // avoids a warning
#endif
    }
#endif

    // no arguments
    for (i = 1; i < argc; i++)
        if (!strcmp(argv[i], "-v") || (!strcmp(argv[i], "--version"))) { // prints version information
            puts("spmenu-"VERSION);
            exit(0);
        } else if (!strcmp(argv[i], "-rv") || (!strcmp(argv[i], "--raw-version"))) { // help
            puts(VERSION);
            exit(0);
        } else if (!strcmp(argv[i], "-h") || (!strcmp(argv[i], "--help"))) { // help
            usage(0);
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
        } else if (!strcmp(argv[i], "-r") || (!strcmp(argv[i], "--incremental"))) { // incremental
            incremental = 1;
        } else if (!strcmp(argv[i], "-nr") || (!strcmp(argv[i], "--no-incremental"))) { // no incremental
            incremental = 0;
        } else if (!strcmp(argv[i], "-rm") || (!strcmp(argv[i], "--require-match"))) { // require match
            requirematch = 1;
        } else if (!strcmp(argv[i], "-nrm") || (!strcmp(argv[i], "--no-require-match"))) { // no require match
            requirematch = 0;
        } else if (!strcmp(argv[i], "-ma") || (!strcmp(argv[i], "--mark-items"))) { // allow marking items
            mark = 1;
        } else if (!strcmp(argv[i], "-nma") || (!strcmp(argv[i], "--no-mark-items"))) { // don't allow marking items
            mark = 0;
        } else if (!strcmp(argv[i], "-F") || (!strcmp(argv[i], "--fuzzy"))) {   // fuzzy matching
            fuzzy = 1;
        } else if (!strcmp(argv[i], "-NF") || (!strcmp(argv[i], "--no-fuzzy"))) {   // no fuzzy matching
            fuzzy = 0;
        } else if (!strcmp(argv[i], "-ix") || (!strcmp(argv[i], "--print-index"))) {   // print index
            printindex = 1;
        } else if (!strcmp(argv[i], "-nix") || (!strcmp(argv[i], "--no-print-index"))) {   // no print index
            printindex = 0;
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
        } else if (!strcmp(argv[i], "-nP") || (!strcmp(argv[i], "--no-password"))) {  // is the input a password
            passwd = 0;
        } else if (!strcmp(argv[i], "-hmc") || (!strcmp(argv[i], "--hide-match-count"))) {   // hide match count
            hidematchcount = 1;
        } else if (!strcmp(argv[i], "-smc") || (!strcmp(argv[i], "--show-match-count"))) {  // show match count
            hidematchcount = 0;
        } else if (!strcmp(argv[i], "-hm") || (!strcmp(argv[i], "--hide-mode"))) {   // hide mode indicator
            hidemode = 1;
        } else if (!strcmp(argv[i], "-sm") || (!strcmp(argv[i], "--show-mode"))) {  // show mode indicator
            hidemode = 0;
        } else if (!strcmp(argv[i], "-hit") || (!strcmp(argv[i], "--hide-item"))) {   // hide items
            hideitem = 1;
        } else if (!strcmp(argv[i], "-sit") || (!strcmp(argv[i], "--show-item"))) {  // show items
            hideitem = 0;
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
        } else if (!strcmp(argv[i], "-hin") || (!strcmp(argv[i], "--hide-input"))) {   // hide input
            hideinput = 1;
        } else if (!strcmp(argv[i], "-spr") || (!strcmp(argv[i], "--show-prompt"))) {  // show prompt
            hideprompt = 0;
        } else if (!strcmp(argv[i], "-spl") || (!strcmp(argv[i], "--show-powerline"))) {  // show prompt
            hidepowerline = 0;
        } else if (!strcmp(argv[i], "-sin") || (!strcmp(argv[i], "--show-input"))) {  // show input
            hideinput = 0;
        } else if (!strcmp(argv[i], "-hc") || (!strcmp(argv[i], "--hide-cursor") || (!strcmp(argv[i], "--hide-caret")))) {   // hide cursor
            hidecaret = 1;
        } else if (!strcmp(argv[i], "-sc") || (!strcmp(argv[i], "--show-cursor") || (!strcmp(argv[i], "--show-caret")))) {  // show cursor
            hidecaret = 0;
        } else if (!strcmp(argv[i], "-hhl") || (!strcmp(argv[i], "--hide-highlighting"))) {   // hide highlighting
            hidehighlight = 1;
        } else if (!strcmp(argv[i], "-shl") || (!strcmp(argv[i], "--show-highlighting"))) {  // show highlighting
            hidehighlight = 0;
        } else if (!strcmp(argv[i], "-hi") || (!strcmp(argv[i], "--hide-image"))) {   // hide image
            hideimage = 1;
        } else if (!strcmp(argv[i], "-si") || (!strcmp(argv[i], "--show-image"))) {  // show image
            hideimage = 0;
        } else if (!strcmp(argv[i], "-hcl") || (!strcmp(argv[i], "--hide-caps"))) {  // hide caps
            hidecaps = 1;
        } else if (!strcmp(argv[i], "-scl") || (!strcmp(argv[i], "--show-caps"))) {  // hide caps
            hidecaps = 0;
        } else if (!strcmp(argv[i], "-ip") || (!strcmp(argv[i], "--indent"))) {  // indent to prompt width
            indentitems = 1;
        } else if (!strcmp(argv[i], "-nip") || (!strcmp(argv[i], "--no-indent"))) {  // don't indent to prompt width
            indentitems = 0;
        } else if (!strcmp(argv[i], "-ci") || (!strcmp(argv[i], "--color-items"))) {  // color items
            coloritems = 1;
        } else if (!strcmp(argv[i], "-nci") || (!strcmp(argv[i], "--no-color-items"))) {  // don't color items
            coloritems = 0;
        } else if (!strcmp(argv[i], "-sgr") || (!strcmp(argv[i], "--sgr"))) { // enable sgr seq support
            sgr = 1;
        } else if (!strcmp(argv[i], "-nsgr") || (!strcmp(argv[i], "--no-sgr"))) { // disable sgr seq support
            sgr = 0;
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
                        || !strcmp(argv[i], "-ltm")
                        || !strcmp(argv[i], "-nltm")
                        || !strcmp(argv[i], "--load-theme")
                        || !strcmp(argv[i], "--no-load-theme")
                        || !strcmp(argv[i], "-lbi")
                        || !strcmp(argv[i], "-nlbi")
                        || !strcmp(argv[i], "-wl")
                        || !strcmp(argv[i], "--wayland")
                        || !strcmp(argv[i], "-x11")
                        || !strcmp(argv[i], "--x11")
                        || !strcmp(argv[i], "--load-binds")
                        || !strcmp(argv[i], "--no-load-binds")
                        || !strcmp(argv[i], "-gbc")
                        || !strcmp(argv[i], "-ngbc")
                        || !strcmp(argv[i], "--global-colors")
                        || !strcmp(argv[i], "--no-global-colors")
#if USECONFIG
                        || !strcmp(argv[i], "-cf")
                        || !strcmp(argv[i], "--config-file")
                        || (configfile && !strcmp(argv[i], configfile))
                        || !strcmp(argv[i], "-tm")
                        || !strcmp(argv[i], "--theme")
                        || (themefile && !strcmp(argv[i], themefile))
                        || !strcmp(argv[i], "-bf")
                        || !strcmp(argv[i], "--bind-file")
                        || (bindsfile && !strcmp(argv[i], bindsfile))
#endif
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
        } else if (!strcmp(argv[i], "-cd") || (!strcmp(argv[i], "--cache-dir"))) { // cache directory
            cachedir = argv[++i];
            if (access(cachedir, F_OK) != 0) {
                cachedir = "default";
            }
        } else if (!strcmp(argv[i], "-l") || (!strcmp(argv[i], "--lines"))) { // number of lines in grid
            lines = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "-mh") || (!strcmp(argv[i], "--lineheight")) || (!strcmp(argv[i], "--line-height"))) { // line height
            lineheight += atoi(argv[++i]);
            if (columns == 0) columns = 1;
        } else if (!strcmp(argv[i], "-mw") || (!strcmp(argv[i], "--min-width"))) { // line height
            minwidth = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "-txp") || (!strcmp(argv[i], "--text-padding"))) { // text padding
            textpadding = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "-lp") || (!strcmp(argv[i], "--vertical-padding"))) { // vertical padding
            menupaddingv = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "-hp") || (!strcmp(argv[i], "--horizontal-padding"))) { // horizontal padding
            menupaddingh = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "-vem") || (!strcmp(argv[i], "--vertical-margin"))) { // vertical margin
            menumarginv = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "-hom") || (!strcmp(argv[i], "--horizontal-margin"))) { // horizontal margin
            menumarginh = atoi(argv[++i]);
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
            xpos = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-y") || (!strcmp(argv[i], "--y-position")))   // window y offset (from bottom up if -b)
            ypos = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-z") || (!strcmp(argv[i], "--width")))   // make spmenu this wide
            menuwidth = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-p") || (!strcmp(argv[i], "--prompt")))   // adds prompt to left of input field
            prompt = argv[++i];
        else if (!strcmp(argv[i], "-It") || (!strcmp(argv[i], "--input")))   // specify initial text
            input = argv[++i];
        else if (!strcmp(argv[i], "-fn") || (!strcmp(argv[i], "--font")))  // font or font set
            fonts[0] = argv[++i];
        else if (!strcmp(argv[i], "-lf") || (!strcmp(argv[i], "--list-file"))) // list file
            listfile = argv[++i];
        else if (!strcmp(argv[i], "-nmt") || (!strcmp(argv[i], "--normal-mode-text")))  // normal mode text
            strcpy(normtext, argv[++i]);
        else if (!strcmp(argv[i], "-imt") || (!strcmp(argv[i], "--insert-mode-text")))  // insert mode text
            strcpy(instext, argv[++i]);
        else if (!strcmp(argv[i], "-clon") || (!strcmp(argv[i], "--caps-lock-on-text")))  // caps lock on text
            strcpy(capslockontext, argv[++i]);
        else if (!strcmp(argv[i], "-clof") || (!strcmp(argv[i], "--caps-lock-off-text"))) {  // caps lock off text
            strcpy(capslockofftext, argv[++i]);

            // dmenu compatibility options
        } else if (!strcmp(argv[i], "-nb")) {  // normal background color
            int ix = ++i;
            strcpy(col_itemnormbg, argv[ix]);
            strcpy(col_itemnormbg2, argv[ix]);
            strcpy(col_menu, argv[ix]);
            strcpy(col_inputbg, argv[ix]);
            strcpy(col_promptbg, argv[ix]);
        } else if (!strcmp(argv[i], "-nf")) {  // normal foreground color
            int ix = ++i;
            strcpy(col_itemnormfg, argv[ix]);
            strcpy(col_itemnormfg2, argv[ix]);
            strcpy(col_inputfg, argv[ix]);
            strcpy(col_promptfg, argv[ix]);
        } else if (!strcmp(argv[i], "-sb")) {  // selected background color
            int ix = ++i;
            strcpy(col_itemselbg, argv[ix]);
            strcpy(col_menu, argv[ix]);
            strcpy(col_inputbg, argv[ix]);
            strcpy(col_promptbg, argv[ix]);
        } else if (!strcmp(argv[i], "-sf")) {  // selected foreground color
            int ix = ++i;
            strcpy(col_itemselfg, argv[ix]);
            strcpy(col_inputfg, argv[ix]);
            strcpy(col_promptfg, argv[ix]);

            // more
        } else if (!strcmp(argv[i], "-is") || (!strcmp(argv[i], "--image-size"))) { // image size
            char buf[255];
            memset(buf, 0, sizeof(buf));
            memcpy(buf, argv[++i], sizeof(buf)-1);

            if(sscanf(buf, "%dx%d", &imagewidth, &imageheight) == 1)
                imageheight = imagewidth;

        } else if (!strcmp(argv[i], "-w") || (!strcmp(argv[i], "--embed"))) { // embedding window id
#if USEX
            embed = argv[++i];
#endif
        } else if (!strcmp(argv[i], "-n") || (!strcmp(argv[i], "--preselect"))) { // preselected item
            preselected = atoi(argv[++i]);

            // spmenu colors
        } else if (!strcmp(argv[i], "-nif") || (!strcmp(argv[i], "--normal-item-foreground"))) { // normal item foreground color
            strcpy(col_itemnormfg, argv[++i]);
        } else if (!strcmp(argv[i], "-nib") || (!strcmp(argv[i], "--normal-item-background"))) { // normal item background color
            strcpy(col_itemnormbg, argv[++i]);
        } else if (!strcmp(argv[i], "-nnif") || (!strcmp(argv[i], "--normal-next-item-foreground"))) { // normal next item foreground color
            strcpy(col_itemnormfg2, argv[++i]);
        } else if (!strcmp(argv[i], "-nnib") || (!strcmp(argv[i], "--normal-next-item-background"))) { // normal next item background color
            strcpy(col_itemnormbg2, argv[++i]);
        } else if (!strcmp(argv[i], "-sif") || (!strcmp(argv[i], "--selected-item-foreground"))) { // selected item foreground color
            strcpy(col_itemselfg, argv[++i]);
        } else if (!strcmp(argv[i], "-sib") || (!strcmp(argv[i], "--selected-item-background"))) { // selected item background color
            strcpy(col_itemselbg, argv[++i]);
        } else if (!strcmp(argv[i], "-npf") || (!strcmp(argv[i], "--normal-item-priority-foreground"))) { // normal item priority foreground color
            strcpy(col_itemnormprifg, argv[++i]);
        } else if (!strcmp(argv[i], "-npb") || (!strcmp(argv[i], "--normal-item-priority-background"))) { // normal item priority background color
            strcpy(col_itemnormpribg, argv[++i]);
        } else if (!strcmp(argv[i], "-spf") || (!strcmp(argv[i], "--selected-item-priority-foreground"))) { // selected item priority foreground color
            strcpy(col_itemselprifg, argv[++i]);
        } else if (!strcmp(argv[i], "-spb") || (!strcmp(argv[i], "--selected-item-priority-background"))) { // selected item priority background color
            strcpy(col_itemselpribg, argv[++i]);
        } else if (!strcmp(argv[i], "-mnbg") || (!strcmp(argv[i], "--menu-background"))) { // menu color
            strcpy(col_menu, argv[++i]);
        } else if (!strcmp(argv[i], "-pfg") || (!strcmp(argv[i], "--prompt-foreground"))) { // prompt fg color
            strcpy(col_promptfg, argv[++i]);
        } else if (!strcmp(argv[i], "-pbg") || (!strcmp(argv[i], "--prompt-background"))) { // prompt bg color
            strcpy(col_promptbg, argv[++i]);
        } else if (!strcmp(argv[i], "-ifg") || (!strcmp(argv[i], "--input-foreground"))) { // input fg color
            strcpy(col_inputfg, argv[++i]);
        } else if (!strcmp(argv[i], "-pfg") || (!strcmp(argv[i], "--input-background"))) { // input bg color
            strcpy(col_inputbg, argv[++i]);
        } else if (!strcmp(argv[i], "-nhb") || (!strcmp(argv[i], "--normal-highlight-background"))) { // normal highlight background color
            strcpy(col_hlnormbg, argv[++i]);
        } else if (!strcmp(argv[i], "-nhf") || (!strcmp(argv[i], "--normal-highlight-foreground"))) { // normal highlight foreground color
            strcpy(col_hlnormfg, argv[++i]);
        } else if (!strcmp(argv[i], "-shf") || (!strcmp(argv[i], "--selected-highlight-foreground"))) { // selected highlight foreground color
            strcpy(col_hlselfg, argv[++i]);
        } else if (!strcmp(argv[i], "-shb") || (!strcmp(argv[i], "--selected-highlight-background"))) { // selected highlight background color
            strcpy(col_hlselbg, argv[++i]);
        } else if (!strcmp(argv[i], "-nbg") || (!strcmp(argv[i], "--number-background"))) { // numbg
            strcpy(col_numbg, argv[++i]);
        } else if (!strcmp(argv[i], "-nfg") || (!strcmp(argv[i], "--number-foreground"))) { // numfg
            strcpy(col_numfg, argv[++i]);
        } else if (!strcmp(argv[i], "-mbg") || (!strcmp(argv[i], "--mode-background"))) { // mode
            strcpy(col_modebg, argv[++i]);
        } else if (!strcmp(argv[i], "-mfg") || (!strcmp(argv[i], "--mode-foreground"))) { // mode
            strcpy(col_modefg, argv[++i]);
        } else if (!strcmp(argv[i], "-laf") || (!strcmp(argv[i], "--left-arrow-foreground"))) { // left arrow fg
            strcpy(col_larrowfg, argv[++i]);
        } else if (!strcmp(argv[i], "-raf") || (!strcmp(argv[i], "--right-arrow-foreground"))) { // right arrow fg
            strcpy(col_rarrowfg, argv[++i]);
        } else if (!strcmp(argv[i], "-lab") || (!strcmp(argv[i], "--left-arrow-background"))) { // left arrow bg
            strcpy(col_larrowbg, argv[++i]);
        } else if (!strcmp(argv[i], "-rab") || (!strcmp(argv[i], "--right-arrow-background"))) { // right arrow bg
            strcpy(col_rarrowbg, argv[++i]);
        } else if (!strcmp(argv[i], "-bc") || (!strcmp(argv[i], "--border-background"))) { // border
            strcpy(col_border, argv[++i]);
        } else if (!strcmp(argv[i], "-cc") || (!strcmp(argv[i], "-cfc")) || (!strcmp(argv[i], "--caret-foreground"))) {   // caret color
            strcpy(col_caretfg, argv[++i]);
        } else if (!strcmp(argv[i], "-cbc") || (!strcmp(argv[i], "--caret-background"))) {   // caret color
            strcpy(col_caretbg, argv[++i]);
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
                        || !strcmp(argv[i], "-ltm")
                        || !strcmp(argv[i], "-nltm")
                        || !strcmp(argv[i], "--load-theme")
                        || !strcmp(argv[i], "--no-load-theme")
                        || !strcmp(argv[i], "-lbi")
                        || !strcmp(argv[i], "-nlbi")
                        || !strcmp(argv[i], "-wl")
                        || !strcmp(argv[i], "--wayland")
                        || !strcmp(argv[i], "-x11")
                        || !strcmp(argv[i], "--x11")
                        || !strcmp(argv[i], "--load-binds")
                        || !strcmp(argv[i], "--no-load-binds")
                        || !strcmp(argv[i], "-gbc")
                        || !strcmp(argv[i], "-ngbc")
                        || !strcmp(argv[i], "--global-colors")
                        || !strcmp(argv[i], "--no-global-colors")
#if USECONFIG
                        || !strcmp(argv[i], "-cf")
                        || !strcmp(argv[i], "--config-file")
                        || (configfile && !strcmp(argv[i], configfile))
                        || !strcmp(argv[i], "-tm")
                        || !strcmp(argv[i], "--theme")
                        || (themefile && !strcmp(argv[i], themefile))
                        || !strcmp(argv[i], "-bf")
                        || !strcmp(argv[i], "--bind-file")
                        || (bindsfile && !strcmp(argv[i], bindsfile))
#endif
                        ))
                        continue;
            else
                fprintf(stderr, "spmenu: Invalid argument: '%s'\n", argv[i]);

#if !USEX
    protocol = 1;
#endif
#if !USEWAYLAND
    protocol = 0;
#endif

    if (casesensitive) {
        fstrncmp = strncmp;
        fstrstr = strstr;
    } else {
        fstrncmp = strncasecmp;
        fstrstr = cistrstr;
    }

    if (!columns)
        columns = 1;

    // initial text
    if (input)
        insert(input, strlen(input));
}

void usage(int status) {
    // print help
    fputs("spmenu ", status ? stderr : stdout);
    fputs(VERSION, status ? stderr : stdout);
    fputs(": fancy dynamic menu\n\n"
            "- Arguments -\n"
            "spmenu -l,       --lines <line>                              Set line count to stdin\n"
            "spmenu -mh,      --line-height <height>                      Set spmenu line height to <height>\n"
            "spmenu -mw,      --min-width <width>                         Set minimum width to <width>\n"
            "spmenu -g,       --columns <grid>                            Set the number of grids to <grid>\n"
            "spmenu -gc,      --generate-cache                            Generate image cache\n"
            "spmenu -ngc,     --no-generate-cache                         Don't generate image cache\n"
            "spmenu -mc,      --max-cache <size>                          Set max image cache size to <size>\n"
            "spmenu -cd,      --cache-dir <dir>                           Set cache directory to <dir>\n"
            "spmenu -ix,      --print-index                               Print index instead of actual text\n"
            "spmenu -nix,     --no-print-index                            Don't print index instead of actual text\n"
            "spmenu -f,       --fast                                      Grabs keyboard before reading stdin\n"
            "spmenu -r,       --incremental                               Print text every time a key is pressed\n"
            "spmenu -nr,      --no-incremental                            Don't print text every time a key is pressed\n"
            "spmenu -rm,      --require-match                             Require that input text matches an item\n"
            "spmenu -nrm,     --no-require-match                          Don't require that input text matches an item\n"
            "spmenu -ma,      --mark-items                                Allow marking/selecting multiple items\n"
            "spmenu -nma,     --no-mark-items                             Don't allow marking/selecting multiple items\n"
            "spmenu -F,       --fuzzy                                     Enable fuzzy matching\n"
            "spmenu -NF,      --no-fuzzy                                  Disable fuzzy matching\n"
            "spmenu -P,       --password                                  Hide characters\n"
            "spmenu -nP,      --no-password                               Don't hide characters\n"
            "spmenu -p,       --prompt <text>                             Set spmenu prompt text to <text>\n"
            "spmenu -It,      --input <text>                              Set initial input text to <text>\n"
            "spmenu -ip,      --indent                                    Indent items to prompt width\n"
            "spmenu -nip,     --no-indent                                 Don't indent items to prompt width\n"
            "spmenu -ci,      --color-items                               Color items\n"
            "spmenu -nci,     --no-color-items                            Don't color items\n"
            "spmenu -sgr,     --sgr                                       Interpret SGR sequences\n"
            "spmenu -nsgr,    --no-sgr                                    Display SGR sequences as text\n"
            "spmenu -a,       --alpha                                     Enable alpha\n"
            "spmenu -na,      --no-alpha                                  Disable alpha\n"
            "spmenu -tp,      --allow-typing                              Allow the user to type\n"
            "spmenu -nt,      --no-allow-typing                           Don't allow typing, the user must select an option\n"
            , status ? stderr : stdout);

    fputs("spmenu -x,       --x-position <x offset>                     Offset spmenu x position by <x offset>\n"
            "spmenu -y,       --y-position <y offset>                     Offset spmenu y position by <y offset>\n"
            "spmenu -n,       --preselect <line>                          Preselect <line> in the list of items\n"
            "spmenu -z,       --width <width>                             Width of the spmenu window\n"
            "spmenu -nmt,     --normal-mode-text <text>                   Set normal mode text to <text>\n"
            "spmenu -imt,     --insert-mode-text <text>                   Set insert mode text to <text>\n"
            "spmenu -clon,    --caps-lock-on-text <text>                  Set caps lock on text to <text>\n"
            "spmenu -clof,    --caps-lock-off-text <text>                 Set caps lock off text to <text>\n"
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
            , status ? stderr : stdout);

    // more args
    fputs("spmenu -hm,      --hide-mode                                 Hide mode indicator\n"
            "spmenu -hmc,     --hide-match-count                          Hide match count\n"
            "spmenu -hla,     --hide-left-arrow                           Hide left arrow\n"
            "spmenu -hra,     --hide-right-arrow                          Hide right arrow\n"
            "spmenu -hit,     --hide-item                                 Hide items\n"
            "spmenu -hpr,     --hide-prompt                               Hide prompt\n"
            "spmenu -hip,     --hide-input                                Hide input\n"
            "spmenu -hpl,     --hide-powerline                            Hide powerline\n"
            "spmenu -hc,      --hide-caret, --hide-cursor                 Hide caret\n"
            "spmenu -hhl,     --hide-highlighting                         Hide highlight\n"
            "spmenu -hi,      --hide-image                                Hide image\n"
            "spmenu -hcl,     --hide-caps                                 Hide caps lock indicator\n"
            "spmenu -sm,      --show-mode                                 Show mode indicator\n"
            "spmenu -smc,     --show-match-count                          Show match count\n"
            "spmenu -sla,     --show-left-arrow                           Show left arrow\n"
            "spmenu -sra,     --show-right-arrow                          Show right arrow\n"
            "spmenu -sit,     --show-item                                 Show items\n"
            "spmenu -spr,     --show-prompt                               Show prompt\n"
            "spmenu -sin,     --show-input                                Show input\n"
            "spmenu -spl,     --show-powerline                            Show powerline\n"
            "spmenu -sc,      --show-caret, --show-cursor                 Show caret\n"
            "spmenu -shl,     --show-highlighting                         Show highlight\n"
            "spmenu -si,      --show-image                                Show image\n"
            "spmenu -scl,     --show-caps                                 Show caps lock indicator\n"
            "spmenu -xrdb,    --xrdb                                      Load .Xresources on runtime\n"
            "spmenu -nxrdb,   --no-xrdb                                   Don't load .Xresources on runtime\n"
            "spmenu -gbc,     --global-colors                             Recognize global colors (such as *.color1) on runtime\n"
            "spmenu -ngbc,    --no-global-colors                          Don't recognize global colors (such as *.color1) on runtime\n"
            "spmenu -m,       --monitor <monitor>                         Specify a monitor to run spmenu on\n"
            "spmenu -w,       --embed <window id>                         Embed spmenu inside <window id>\n"
            "spmenu -H,       --hist-file <hist file>                     Specify a file to save the history to\n"
            "spmenu -lf,      --list-file <list file>                     Specify a file to load entries from\n"
            "spmenu -ig,      --image-gaps <gaps>                         Set image gaps to <gaps>\n"
            "spmenu -txp,     --text-padding <padding>                    Set text padding to <padding>\n"
            "spmenu -lp,      --vertical-padding <padding>                Set the vertical padding\n"
            "spmenu -hp,      --horizontal-padding <padding>              Set the horizontal padding\n"
            "spmenu -vem,     --vertical-margin <margin>                  Set the vertical margin\n"
            "spmenu -hom,     --horizontal-margin <margin>                Set the horizontal margin\n"
            "spmenu -la,      --left-arrow-symbol <symbol>                Set the left arrow to <symbol>\n"
            "spmenu -ra,      --right-arrow-symbol <symbol>               Set the right arrow to <symbol>\n"
            "spmenu -is,      --image-size <size>                         Image size\n"
            "spmenu -it,      --image-top                                 Position the image at the top\n"
            "spmenu -ib,      --image-bottom                              Position the image at the bottom\n"
            "spmenu -ic,      --image-center                              Position the image in the center\n"
            "spmenu -itc,     --image-topcenter                           Position the image in the top center\n"
            , status ? stderr : stdout);

    // general/config related
    fputs("spmenu -wm,      --managed, --x11-client                     Spawn spmenu as a window manager controlled client/window. Useful for testing\n"
            "spmenu -nwm,     --unmanaged                                 Don't spawn spmenu as a window manager controlled client/window. Useful for testing\n"
            "spmenu -cf,      --config-file <file>                        Set config file to load to <file>\n"
            "spmenu -lcfg,    --load-config                               Load spmenu configuration (~/.config/spmenu/spmenu.conf)\n"
            "spmenu -ncfg,    --no-load-config                            Don't load spmenu configuration (~/.config/spmenu/spmenu.conf)\n"
            "spmenu -bf,      --bind-file <file>                          Exclusively load binds from <file>\n"
            "spmenu -lbi,     --load-binds                                Load spmenu binds (~/.config/spmenu/binds.conf)\n"
            "spmenu -nlbi,    --no-load-binds                             Don't load spmenu binds (~/.config/spmenu/binds.conf)\n"
            "spmenu -tm,      --theme <theme>                             Load theme <theme>\n"
            "spmenu -ltm,     --load-theme                                Load theme\n"
            "spmenu -nltm,    --no-load-theme                             Don't load theme\n"
            "spmenu -x11,     --x11                                       Run spmenu in X11 mode\n"
            "spmenu -wl,      --wayland                                   Run spmenu in Wayland mode\n"
            "spmenu -rv,      --raw-version                               Print spmenu version number to stdout\n"
            "spmenu -v,       --version                                   Print spmenu version to stdout\n"
            "\n", status ? stderr : stdout);

    // colors
    fputs("- Appearance arguments -\n"
            "spmenu -fn,      --font  <font>                              Set the spmenu font to <font>\n"
            "spmenu -nif,     --normal-item-foreground <color>            Set the normal item foreground color\n"
            "spmenu -nib,     --normal-item-background <color>            Set the normal item background color\n"
            "spmenu -nnif,    --normal-next-item-foreground <color>       Set the normal next item foreground color\n"
            "spmenu -nnib,    --normal-next-item-background <color>       Set the normal next item background color\n"
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
            "spmenu -cfc,     --caret-foreground <color>                  Set the caret foreground color\n"
            "spmenu -cbc,     --caret-background <color>                  Set the caret background color\n"
            "spmenu -bc,      --border-background <color>                 Set the border color\n"
            , status ? stderr : stdout);

    // sgr sequences
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
            "\n", status ? stderr : stdout);

    // dmenu compat
    fputs("- dmenu compatibility -\n"
            "spmenu -S                                                    Don't sort matches\n"
            "spmenu -i                                                    Use case-insensitive matching\n"
            "spmenu -nb <color>                                           Set the normal background color\n"
            "spmenu -nf <color>                                           Set the normal foreground color\n"
            "spmenu -sb <color>                                           Set the selected background color\n"
            "spmenu -sf <color>                                           Set the selected foreground color\n"
            "\n"
            "See the spmenu(1) man page for more information.\n"
            , status ? stderr : stdout);

    exit(status);
}
