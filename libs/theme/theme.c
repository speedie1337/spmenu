/* See LICENSE file for copyright and license details. */

void theme_load(void) {
    char *xdg_conf;
    char *theme = NULL;
    char *home = NULL;
    const char *dest;

    // don't load configuration
    if (!loadtheme) return;

    // get path for configuration file
    if (!themefile || !strcmp(themefile, "NULL")) {
        if (!(xdg_conf = getenv("XDG_CONFIG_HOME"))) {
            // ~/.config/spmenu/theme.conf
            home = getenv("HOME");

            // malloc
            if (!(theme = malloc(snprintf(NULL, 0, "%s/%s", home, ".config/spmenu/theme.conf") + 1))) {
                die("spmenu: failed to malloc theme");
            }

            sprintf(theme, "%s/%s", home, ".config/spmenu/theme.conf");
        } else {
            // malloc
            if (!(theme = malloc(snprintf(NULL, 0, "%s/%s", xdg_conf, "spmenu/theme.conf") + 1))) {
                die("spmenu: failed to malloc theme");
            }

            // XDG_CONFIG_HOME is set, so let's use that instead
            sprintf(theme, "%s/%s", xdg_conf, "spmenu/theme.conf");
        }
    } else { // custom config path
        if (!(theme = malloc(snprintf(NULL, 0, "%s", themefile) + 1))) {
            die("spmenu: failed to malloc theme");
        }

        sprintf(theme, "%s", themefile);
    }

    // don't bother trying to load if it doesn't exist.
    if (access(theme, F_OK) != 0) {
        return;
    }

    // init config
    config_t cfg;
    config_init(&cfg);

    // attempt to read theme
    if (!config_read_file(&cfg, theme)) {
        fprintf(stderr, "spmenu: Invalid theme.\n"); // invalid configuration, but let's try to read it anyway
    }

    // load options theme.window
    config_setting_t *window_setting = config_lookup(&cfg, "theme.window");
    if (window_setting != NULL) {
        // look up window entries
        for (unsigned int i = 0; i < config_setting_length(window_setting); ++i) {
            config_setting_t *conf = config_setting_get_elem(window_setting, i);

            config_setting_lookup_int(conf, "position", &menuposition); // theme.window.menuposition
            config_setting_lookup_int(conf, "paddingv", &menupaddingv); // theme.window.paddingv
            config_setting_lookup_int(conf, "paddingh", &menupaddingh); // theme.window.paddingh
            config_setting_lookup_int(conf, "padding-vertical", &menupaddingv); // theme.window.padding-vertical
            config_setting_lookup_int(conf, "padding-horizontal", &menupaddingh); // theme.window.padding-horizontal
            config_setting_lookup_int(conf, "margin-vertical", &menumarginv); // theme.window.margin-vertical
            config_setting_lookup_int(conf, "margin-horizontal", &menumarginh); // theme.window.margin-horizontal
            config_setting_lookup_int(conf, "x", &xpos); // theme.window.x
            config_setting_lookup_int(conf, "y", &xpos); // theme.window.y
            config_setting_lookup_int(conf, "width", &menuwidth); // theme.window.width
            config_setting_lookup_int(conf, "border", &borderwidth); // theme.window.border
            config_setting_lookup_int(conf, "alpha", &alpha); // theme.window.alpha
        }
    }

    // load options theme.powerline
    config_setting_t *pwl_setting = config_lookup(&cfg, "theme.powerline");
    if (pwl_setting != NULL) {
        for (unsigned int i = 0; i < config_setting_length(pwl_setting); ++i) {
            config_setting_t *conf = config_setting_get_elem(pwl_setting, i);

            // look up
            config_setting_lookup_int(conf, "promptstyle", &promptpwlstyle); // theme.powerline.promptstyle
            config_setting_lookup_int(conf, "matchcountstyle", &matchcountpwlstyle); // theme.powerline.matchcountstyle
            config_setting_lookup_int(conf, "modestyle", &modepwlstyle); // theme.powerline.modestyle
            config_setting_lookup_int(conf, "capsstyle", &capspwlstyle); // theme.powerline.capsstyle
            config_setting_lookup_int(conf, "prompt", &powerlineprompt); // theme.powerline.prompt
            config_setting_lookup_int(conf, "matchcount", &powerlinecount); // theme.powerline.matchcount
            config_setting_lookup_int(conf, "mode", &powerlinemode); // theme.powerline.mode
            config_setting_lookup_int(conf, "caps", &powerlinecaps); // theme.powerline.caps
        }
    }

    // load options theme.center
    config_setting_t *center_setting = config_lookup(&cfg, "theme.center");
    if (center_setting != NULL) {
        for (unsigned int i = 0; i < config_setting_length(center_setting); ++i) {
            config_setting_t *conf = config_setting_get_elem(center_setting, i);

            config_setting_lookup_int(conf, "width", &minwidth); // theme.center.width
        }
    }

    // load options theme.text
    config_setting_t *text_setting = config_lookup(&cfg, "theme.text");
    if (text_setting != NULL) {
        for (unsigned int i = 0; i < config_setting_length(text_setting); ++i) {
            config_setting_t *conf = config_setting_get_elem(text_setting, i);

            // look up
            if (config_setting_lookup_string(conf, "font", &dest)) // theme.text.font
                strcpy(font, strdup(dest));

            config_setting_lookup_int(conf, "padding", &textpadding); // theme.text.padding
            config_setting_lookup_int(conf, "normitempadding", &normitempadding); // theme.text.normitempadding
            config_setting_lookup_int(conf, "selitempadding", &selitempadding); // theme.text.selitempadding
            config_setting_lookup_int(conf, "priitempadding", &priitempadding); // theme.text.priitempadding

            if (config_setting_lookup_string(conf, "leftarrow", &dest)) // theme.text.leftarrow
                leftarrow = strdup(dest);

            if (config_setting_lookup_string(conf, "rightarrow", &dest)) // theme.text.rightarrow
                rightarrow = strdup(dest);

            if (config_setting_lookup_string(conf, "password", &dest))
                password = strdup(dest);

            if (config_setting_lookup_string(conf, "prompt", &dest))
                prompt = strdup(dest);

            if (config_setting_lookup_string(conf, "capslockon", &dest))
                capslockontext = strdup(dest);

            if (config_setting_lookup_string(conf, "capslockoff", &dest))
                capslockofftext = strdup(dest);

            if (config_setting_lookup_string(conf, "input", &dest))
                input = strdup(dest);
        }
    }

    // load options theme.alpha
    config_setting_t *alpha_setting = config_lookup(&cfg, "theme.alpha");
    if (alpha_setting != NULL) {
        for (unsigned int i = 0; i < config_setting_length(alpha_setting); ++i) {
            config_setting_t *conf = config_setting_get_elem(alpha_setting, i);

            // look up
            config_setting_lookup_int(conf, "itemnormfg", &alpha_itemnormfg); // theme.alpha.itemnormfg
            config_setting_lookup_int(conf, "itemnormbg", &alpha_itemnormbg); // theme.alpha.itemnormbg

            if (!config_setting_lookup_int(conf, "itemnormfg2", &alpha_itemnormfg2)) // theme.alpha.itemnormfg2
                config_setting_lookup_int(conf, "itemnormfg", &alpha_itemnormfg2);

            if (!config_setting_lookup_int(conf, "itemnormbg2", &alpha_itemnormbg2)) // theme.alpha.itemnormbg2
                config_setting_lookup_int(conf, "itemnormbg", &alpha_itemnormbg2);

            config_setting_lookup_int(conf, "itemselfg", &alpha_itemselfg); // theme.alpha.itemselfg
            config_setting_lookup_int(conf, "itemselbg", &alpha_itemselbg); // theme.alpha.itemselbg

            if (!config_setting_lookup_int(conf, "itemmarkedfg", &alpha_itemmarkedfg))
                config_setting_lookup_int(conf, "itemselfg", &alpha_itemmarkedfg);

            if (!config_setting_lookup_int(conf, "itemmarkedbg", &alpha_itemmarkedbg))
                config_setting_lookup_int(conf, "itemselbg", &alpha_itemmarkedbg);

            config_setting_lookup_int(conf, "itemnormprifg", &alpha_itemnormprifg); // theme.alpha.itemnormprifg
            config_setting_lookup_int(conf, "itemnormpribg", &alpha_itemnormpribg); // theme.alpha.itemnormpribg
            config_setting_lookup_int(conf, "itemselprifg", &alpha_itemselprifg); // theme.alpha.itemselprifg
            config_setting_lookup_int(conf, "itemselpribg", &alpha_itemselpribg); // theme.alpha.itemselpribg

            config_setting_lookup_int(conf, "inputfg", &alpha_inputfg); // theme.alpha.inputfg
            config_setting_lookup_int(conf, "inputbg", &alpha_inputbg); // theme.alpha.inputbg

            config_setting_lookup_int(conf, "menu", &alpha_menu); // theme.alpha.menu

            config_setting_lookup_int(conf, "promptfg", &alpha_promptfg); // theme.alpha.promptfg
            config_setting_lookup_int(conf, "promptbg", &alpha_promptbg); // theme.alpha.promptbg

            config_setting_lookup_int(conf, "larrowfg", &alpha_larrowfg); // theme.alpha.larrowfg
            config_setting_lookup_int(conf, "larrowbg", &alpha_larrowbg); // theme.alpha.larrowbg
            config_setting_lookup_int(conf, "rarrowfg", &alpha_rarrowfg); // theme.alpha.rarrowfg
            config_setting_lookup_int(conf, "rarrowbg", &alpha_rarrowbg); // theme.alpha.rarrowbg

            config_setting_lookup_int(conf, "hlnormfg", &alpha_hlnormfg); // theme.alpha.hlnormfg
            config_setting_lookup_int(conf, "hlnormbg", &alpha_hlnormbg); // theme.alpha.hlnormbg
            config_setting_lookup_int(conf, "hlselfg", &alpha_hlselfg); // theme.alpha.hlselfg
            config_setting_lookup_int(conf, "hlselbg", &alpha_hlselbg); // theme.alpha.hlselbg

            config_setting_lookup_int(conf, "numfg", &alpha_numfg); // theme.alpha.numfg
            config_setting_lookup_int(conf, "numbg", &alpha_numbg); // theme.alpha.numbg

            config_setting_lookup_int(conf, "border", &alpha_border); // theme.alpha.border

            config_setting_lookup_int(conf, "caretfg", &alpha_caretfg); // theme.alpha.caretfg
            config_setting_lookup_int(conf, "caretbg", &alpha_caretbg); // theme.alpha.caretbg

            config_setting_lookup_int(conf, "modefg", &alpha_modefg); // theme.alpha.modefg
            config_setting_lookup_int(conf, "modebg", &alpha_modebg); // theme.alpha.modebg

            config_setting_lookup_int(conf, "capsfg", &alpha_capsfg); // theme.alpha.capsfg
            config_setting_lookup_int(conf, "capsbg", &alpha_capsbg); // theme.alpha.capsbg
        }
    }

    // load options theme.color
    config_setting_t *color_setting = config_lookup(&cfg, "theme.color");
    if (color_setting != NULL) {
        for (unsigned int i = 0; i < config_setting_length(color_setting); ++i) {
            config_setting_t *conf = config_setting_get_elem(color_setting, i);

            // items
            if (config_setting_lookup_string(conf, "itemnormfg", &dest))
                strcpy(col_itemnormfg, strdup(dest));

            if (config_setting_lookup_string(conf, "itemnormbg", &dest))
                strcpy(col_itemnormbg, strdup(dest));

            if (config_setting_lookup_string(conf, "itemnormfg2", &dest))
                strcpy(col_itemnormfg2, strdup(dest));
            else if (config_setting_lookup_string(conf, "itemnormfg", &dest))
                strcpy(col_itemnormfg2, strdup(dest));

            if (config_setting_lookup_string(conf, "itemnormbg2", &dest))
                strcpy(col_itemnormbg2, strdup(dest));
            else if (config_setting_lookup_string(conf, "itemnormbg", &dest))
                strcpy(col_itemnormbg2, strdup(dest));

            if (config_setting_lookup_string(conf, "itemselfg", &dest))
                strcpy(col_itemselfg, strdup(dest));

            if (config_setting_lookup_string(conf, "itemselbg", &dest))
                strcpy(col_itemselbg, strdup(dest));

            if (config_setting_lookup_string(conf, "itemmarkedfg", &dest))
                strcpy(col_itemmarkedfg, strdup(dest));
            else if (config_setting_lookup_string(conf, "itemselfg", &dest))
                strcpy(col_itemmarkedfg, strdup(dest));

            if (config_setting_lookup_string(conf, "itemmarkedbg", &dest))
                strcpy(col_itemmarkedbg, strdup(dest));
            else if (config_setting_lookup_string(conf, "itemselbg", &dest))
                strcpy(col_itemmarkedbg, strdup(dest));

            // items with priority
            if (config_setting_lookup_string(conf, "itemnormprifg", &dest))
                strcpy(col_itemnormprifg, strdup(dest));

            if (config_setting_lookup_string(conf, "itemnormpribg", &dest))
                strcpy(col_itemnormpribg, strdup(dest));

            if (config_setting_lookup_string(conf, "itemselprifg", &dest))
                strcpy(col_itemselprifg, strdup(dest));

            if (config_setting_lookup_string(conf, "itemselpribg", &dest))
                strcpy(col_itemselpribg, strdup(dest));

            // input
            if (config_setting_lookup_string(conf, "inputfg", &dest))
                strcpy(col_inputfg, strdup(dest));

            if (config_setting_lookup_string(conf, "inputbg", &dest))
                strcpy(col_inputbg, strdup(dest));

            // menu
            if (config_setting_lookup_string(conf, "menu", &dest))
                strcpy(col_menu, strdup(dest));

            // prompt
            if (config_setting_lookup_string(conf, "promptfg", &dest))
                strcpy(col_promptfg, strdup(dest));

            if (config_setting_lookup_string(conf, "promptbg", &dest))
                strcpy(col_promptbg, strdup(dest));

            // arrows
            if (config_setting_lookup_string(conf, "larrowfg", &dest))
                strcpy(col_larrowfg, strdup(dest));

            if (config_setting_lookup_string(conf, "larrowbg", &dest))
                strcpy(col_larrowbg, strdup(dest));

            if (config_setting_lookup_string(conf, "rarrowfg", &dest))
                strcpy(col_rarrowfg, strdup(dest));

            if (config_setting_lookup_string(conf, "rarrowbg", &dest))
                strcpy(col_rarrowbg, strdup(dest));

            // highlight
            if (config_setting_lookup_string(conf, "hlnormfg", &dest))
                strcpy(col_hlnormfg, strdup(dest));

            if (config_setting_lookup_string(conf, "hlnormbg", &dest))
                strcpy(col_hlnormbg, strdup(dest));

            if (config_setting_lookup_string(conf, "hlselfg", &dest))
                strcpy(col_hlselfg, strdup(dest));

            if (config_setting_lookup_string(conf, "hlselbg", &dest))
                strcpy(col_hlselbg, strdup(dest));

            // number
            if (config_setting_lookup_string(conf, "numfg", &dest))
                strcpy(col_numfg, strdup(dest));

            if (config_setting_lookup_string(conf, "numbg", &dest))
                strcpy(col_numbg, strdup(dest));

            // mode
            if (config_setting_lookup_string(conf, "modefg", &dest))
                strcpy(col_modefg, strdup(dest));

            if (config_setting_lookup_string(conf, "modebg", &dest))
                strcpy(col_modebg, strdup(dest));

            // caps
            if (config_setting_lookup_string(conf, "capsfg", &dest))
                strcpy(col_capsfg, strdup(dest));

            if (config_setting_lookup_string(conf, "capsbg", &dest))
                strcpy(col_capsbg, strdup(dest));

            // border
            if (config_setting_lookup_string(conf, "border", &dest))
                strcpy(col_border, strdup(dest));

            // caret
            if (config_setting_lookup_string(conf, "caretfg", &dest))
                strcpy(col_caretfg, strdup(dest));

            if (config_setting_lookup_string(conf, "caretbg", &dest))
                strcpy(col_caretbg, strdup(dest));

            // sgr colors
            if (config_setting_lookup_string(conf, "sgr0", &dest))
                strcpy(textcolors[0], strdup(dest));
            if (config_setting_lookup_string(conf, "sgr1", &dest))
                strcpy(textcolors[1], strdup(dest));
            if (config_setting_lookup_string(conf, "sgr2", &dest))
                strcpy(textcolors[2], strdup(dest));
            if (config_setting_lookup_string(conf, "sgr3", &dest))
                strcpy(textcolors[3], strdup(dest));
            if (config_setting_lookup_string(conf, "sgr4", &dest))
                strcpy(textcolors[4], strdup(dest));
            if (config_setting_lookup_string(conf, "sgr5", &dest))
                strcpy(textcolors[5], strdup(dest));
            if (config_setting_lookup_string(conf, "sgr6", &dest))
                strcpy(textcolors[6], strdup(dest));
            if (config_setting_lookup_string(conf, "sgr7", &dest))
                strcpy(textcolors[7], strdup(dest));
            if (config_setting_lookup_string(conf, "sgr8", &dest))
                strcpy(textcolors[8], strdup(dest));
            if (config_setting_lookup_string(conf, "sgr9", &dest))
                strcpy(textcolors[9], strdup(dest));
            if (config_setting_lookup_string(conf, "sgr10", &dest))
                strcpy(textcolors[10], strdup(dest));
            if (config_setting_lookup_string(conf, "sgr11", &dest))
                strcpy(textcolors[11], strdup(dest));
            if (config_setting_lookup_string(conf, "sgr12", &dest))
                strcpy(textcolors[12], strdup(dest));
            if (config_setting_lookup_string(conf, "sgr13", &dest))
                strcpy(textcolors[13], strdup(dest));
            if (config_setting_lookup_string(conf, "sgr14", &dest))
                strcpy(textcolors[14], strdup(dest));
            if (config_setting_lookup_string(conf, "sgr15", &dest))
                strcpy(textcolors[15], strdup(dest));

            // coloritems int
            config_setting_lookup_int(conf, "coloritems", &coloritems);
            config_setting_lookup_int(conf, "sgr", &sgr);
        }
    }

    // load options theme.image
    config_setting_t *img_setting = config_lookup(&cfg, "theme.image");
    if (img_setting != NULL) {
        for (unsigned int i = 0; i < config_setting_length(img_setting); ++i) {
            config_setting_t *conf = config_setting_get_elem(img_setting, i);

            // look up
            config_setting_lookup_int(conf, "width", &imagewidth); // theme.image.width
            config_setting_lookup_int(conf, "height", &imageheight); // theme.image.height
            config_setting_lookup_int(conf, "gaps", &imageheight); // theme.image.gaps
            config_setting_lookup_int(conf, "position", &imageposition); // theme.image.position
        }
    }

    // load options theme.mode
    config_setting_t *mode_setting = config_lookup(&cfg, "theme.mode");
    if (mode_setting != NULL) {
        for (unsigned int i = 0; i < config_setting_length(mode_setting); ++i) {
            config_setting_t *conf = config_setting_get_elem(mode_setting, i);

            config_setting_lookup_string(conf, "normal_text", &dest); // theme.mode.normal_text
            normtext = strdup(dest);

            config_setting_lookup_string(conf, "insert_text", &dest); // theme.mode.insert_text
            instext = strdup(dest);
        }
    }

    // load options theme.line
    config_setting_t *line_setting = config_lookup(&cfg, "theme.line");
    if (line_setting != NULL) {
        for (unsigned int i = 0; i < config_setting_length(line_setting); ++i) {
            config_setting_t *conf = config_setting_get_elem(line_setting, i);

            // look up
            config_setting_lookup_int(conf, "height", &lineheight); // theme.line.height
            config_setting_lookup_int(conf, "indentitems", &indentitems); // theme.line.indentitems
        }
    }

    // load options theme.hide
    config_setting_t *hide_setting = config_lookup(&cfg, "theme.hide");
    if (hide_setting != NULL) {
        for (unsigned int i = 0; i < config_setting_length(hide_setting); ++i) {
            config_setting_t *conf = config_setting_get_elem(hide_setting, i);

            // look up
            config_setting_lookup_int(conf, "input", &hideinput); // theme.hide.input
            config_setting_lookup_int(conf, "larrow", &hidelarrow); // theme.hide.larrow
            config_setting_lookup_int(conf, "rarrow", &hiderarrow); // theme.hide.rarrow
            config_setting_lookup_int(conf, "prompt", &hideprompt); // theme.hide.prompt
            config_setting_lookup_int(conf, "items",  &hideitem); // theme.hide.items
            config_setting_lookup_int(conf, "powerline", &hidepowerline); // theme.hide.powerline
            config_setting_lookup_int(conf, "caret", &hidecaret); // theme.hide.caret
            config_setting_lookup_int(conf, "highlight", &hidehighlight); // theme.hide.highlight
            config_setting_lookup_int(conf, "matchcount", &hidematchcount); // theme.hide.matchcount
            config_setting_lookup_int(conf, "mode", &hidemode); // theme.hide.mode
            config_setting_lookup_int(conf, "caps", &hidecaps); // theme.hide.caps
            config_setting_lookup_int(conf, "hideimage", &hideimage); // theme.hide.image
        }
    }

    // load options theme.pango
    config_setting_t *pango_setting = config_lookup(&cfg, "theme.pango");
    if (pango_setting != NULL) {
        for (unsigned int i = 0; i < config_setting_length(pango_setting); ++i) {
            config_setting_t *conf = config_setting_get_elem(pango_setting, i);

            // look up
            config_setting_lookup_int(conf, "item", &pango_item); // theme.pango.input
            config_setting_lookup_int(conf, "highlight", &pango_highlight); // theme.pango.highlight
            config_setting_lookup_int(conf, "prompt", &pango_prompt); // theme.pango.rarrow
            config_setting_lookup_int(conf, "input", &pango_input); // theme.pango.input
            config_setting_lookup_int(conf, "leftarrow", &pango_leftarrow); // theme.pango.leftarrow
            config_setting_lookup_int(conf, "rightarrow", &pango_rightarrow); // theme.pango.rightarrow
            config_setting_lookup_int(conf, "numbers", &pango_numbers); // theme.pango.numbers
            config_setting_lookup_int(conf, "mode", &pango_mode); // theme.pango.mode
            config_setting_lookup_int(conf, "caps", &pango_caps); // theme.pango.caps
            config_setting_lookup_int(conf, "password", &pango_password); // theme.pango.mode
        }
    }

    config_setting_t *file_setting = config_lookup(&cfg, "theme.file");
    if (file_setting != NULL) {
        for (unsigned int i = 0; i < config_setting_length(file_setting); ++i) {
            config_setting_t *conf = config_setting_get_elem(file_setting, i);

            // look up
            config_setting_lookup_int(conf, "global", &globalcolors); // spmenu.file.global
        }
    }

    // we're done here
    config_destroy(&cfg);
    return;
}
