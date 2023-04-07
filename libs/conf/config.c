#include <libconfig.h>

void
conf_init(void)
{
    char *xdg_conf;
    char *cfgfile = NULL;
    char *home = NULL;
    const char *dest;

    // don't load configuration
    if (!loadconfig) return;

    // get path for configuration file
    if (!cconf || !argconf) {
        if (!(xdg_conf = getenv("XDG_CONFIG_HOME"))) {
            // ~/.config/spmenu/spmenu.conf
            home = getenv("HOME");

            // malloc
            if (!(cfgfile = malloc(snprintf(NULL, 0, "%s/%s", home, ".config/spmenu/spmenu.conf") + 1))) {
                die("spmenu: failed to malloc cfgfile");
            }

            sprintf(cfgfile, "%s/%s", home, ".config/spmenu/spmenu.conf");
        } else {
            // malloc
            if (!(cfgfile = malloc(snprintf(NULL, 0, "%s/%s", xdg_conf, "spmenu/spmenu.conf") + 1))) {
                die("spmenu: failed to malloc cfgfile");
            }

            // XDG_CONFIG_HOME is set, so let's use that instead
            sprintf(cfgfile, "%s/%s", xdg_conf, "spmenu/spmenu.conf");
        }
    } else { // custom config path
            if (!(cfgfile = malloc(snprintf(NULL, 0, "%s", argconf) + 1))) {
                die("spmenu: failed to malloc cfgfile");
            }

            sprintf(cfgfile, "%s", argconf);
    }

    // don't bother trying to load if it doesn't exist.
    if (access(cfgfile, F_OK) != 0) {
        return;
    }

    // init config
    config_t cfg;
    config_setting_t *setting;
    int conflength = 0;

    config_init(&cfg);

    // attempt to read config file to cfg
    if (!config_read_file(&cfg, cfgfile)) {
         // invalid configuration, but let's try to read it anyway
         ;
    }

    // load options spmenu.window
    setting = config_lookup(&cfg, "spmenu.window");
    if (setting != NULL) {
        unsigned int i = 0;

        conflength = config_setting_length(setting);

        for (i = 0; i < conflength; ++i) {
            config_setting_t *conf = config_setting_get_elem(setting, i);

            // look up
            config_setting_lookup_int(conf, "position", &menuposition); // spmenu.window.menuposition
            config_setting_lookup_int(conf, "paddingv", &menupaddingv); // spmenu.window.paddingv
            config_setting_lookup_int(conf, "paddingh", &menupaddingh); // spmenu.window.paddingh
            config_setting_lookup_int(conf, "x", &xpos); // spmenu.window.x
            config_setting_lookup_int(conf, "y", &xpos); // spmenu.window.y
            config_setting_lookup_int(conf, "width", &menuwidth); // spmenu.window.width
            config_setting_lookup_int(conf, "border", &borderwidth); // spmenu.window.border
            config_setting_lookup_int(conf, "managed", &managed); // spmenu.window.managed
            config_setting_lookup_int(conf, "monitor", &mon); // spmenu.window.monitor
            config_setting_lookup_int(conf, "alpha", &alpha); // spmenu.window.alpha
        }
    }

    // load options spmenu.properties
    setting = config_lookup(&cfg, "spmenu.properties");
    if (setting != NULL) {
        unsigned int i = 0;

        conflength = config_setting_length(setting);

        for (i = 0; i < conflength; ++i) {
            config_setting_t *conf = config_setting_get_elem(setting, i);

            // look up
            config_setting_lookup_string(conf, "class", &dest); // spmenu.properties.class
            class = strdup(dest);

            config_setting_lookup_int(conf, "dock", &dockproperty); // spmenu.properties.dock
        }
    }

    // load options spmenu.center
    setting = config_lookup(&cfg, "spmenu.center");
    if (setting != NULL) {
        unsigned int i = 0;

        conflength = config_setting_length(setting);

        for (i = 0; i < conflength; ++i) {
            config_setting_t *conf = config_setting_get_elem(setting, i);

            // look up
            config_setting_lookup_int(conf, "width", &minwidth); // spmenu.center.width
        }
    }

    // load options spmenu.text
    setting = config_lookup(&cfg, "spmenu.text");
    if (setting != NULL) {
        unsigned int i = 0;

        conflength = config_setting_length(setting);

        for (i = 0; i < conflength; ++i) {
            config_setting_t *conf = config_setting_get_elem(setting, i);

            // look up
            config_setting_lookup_string(conf, "font", &dest); // spmenu.text.font
            strcpy(font, strdup(dest));

            config_setting_lookup_int(conf, "padding", &textpadding); // spmenu.text.padding

            config_setting_lookup_string(conf, "leftarrow", &dest); // spmenu.text.leftarrow
            leftarrow = strdup(dest);

            config_setting_lookup_string(conf, "rightarrow", &dest); // spmenu.text.rightarrow
            rightarrow = strdup(dest);

            config_setting_lookup_string(conf, "password", &dest);
            password = strdup(dest);

            config_setting_lookup_string(conf, "prompt", &dest);
            prompt = strdup(dest);

            config_setting_lookup_string(conf, "input", &dest);
            input = strdup(dest);
        }
    }

    // load options spmenu.color
    setting = config_lookup(&cfg, "spmenu.color");
    if (setting != NULL) {
        unsigned int i = 0;

        conflength = config_setting_length(setting);

        for (i = 0; i < conflength; ++i) {
            config_setting_t *conf = config_setting_get_elem(setting, i);

            // items
            if (config_setting_lookup_string(conf, "itemnormfg", &dest))
                strcpy(colors[SchemeItemNorm][ColFg], strdup(dest));

            if (config_setting_lookup_string(conf, "itemnormbg", &dest))
                strcpy(colors[SchemeItemNorm][ColBg], strdup(dest));

            if (config_setting_lookup_string(conf, "itemselfg", &dest))
                strcpy(colors[SchemeItemSel][ColFg], strdup(dest));

            if (config_setting_lookup_string(conf, "itemselbg", &dest))
                strcpy(colors[SchemeItemSel][ColBg], strdup(dest));

            // items with priority
            if (config_setting_lookup_string(conf, "itemnormprifg", &dest))
                strcpy(colors[SchemeItemNormPri][ColFg], strdup(dest));

            if (config_setting_lookup_string(conf, "itemnormpribg", &dest))
                strcpy(colors[SchemeItemNormPri][ColBg], strdup(dest));

            if (config_setting_lookup_string(conf, "itemselprifg", &dest))
                strcpy(colors[SchemeItemSelPri][ColFg], strdup(dest));

            if (config_setting_lookup_string(conf, "itemselpribg", &dest))
                strcpy(colors[SchemeItemSelPri][ColBg], strdup(dest));

            // input
            if (config_setting_lookup_string(conf, "inputfg", &dest))
                strcpy(colors[SchemeInput][ColFg], strdup(dest));

            if (config_setting_lookup_string(conf, "inputbg", &dest))
                strcpy(colors[SchemeInput][ColBg], strdup(dest));

            // menu
            if (config_setting_lookup_string(conf, "menubg", &dest))
                strcpy(colors[SchemeMenu][ColBg], strdup(dest));

            // prompt
            if (config_setting_lookup_string(conf, "promptfg", &dest))
                strcpy(colors[SchemePrompt][ColFg], strdup(dest));

            if (config_setting_lookup_string(conf, "promptbg", &dest))
                strcpy(colors[SchemePrompt][ColBg], strdup(dest));

            // arrows
            if (config_setting_lookup_string(conf, "larrowfg", &dest))
                strcpy(colors[SchemeLArrow][ColFg], strdup(dest));

            if (config_setting_lookup_string(conf, "larrowbg", &dest))
                strcpy(colors[SchemeLArrow][ColBg], strdup(dest));

            if (config_setting_lookup_string(conf, "rarrowfg", &dest))
                strcpy(colors[SchemeRArrow][ColFg], strdup(dest));

            if (config_setting_lookup_string(conf, "rarrowbg", &dest))
                strcpy(colors[SchemeRArrow][ColBg], strdup(dest));

            // highlight
            if (config_setting_lookup_string(conf, "hlnormfg", &dest))
                strcpy(colors[SchemeNormHighlight][ColFg], strdup(dest));

            if (config_setting_lookup_string(conf, "hlnormbg", &dest))
                strcpy(colors[SchemeNormHighlight][ColBg], strdup(dest));

            if (config_setting_lookup_string(conf, "hlselfg", &dest))
                strcpy(colors[SchemeSelHighlight][ColFg], strdup(dest));

            if (config_setting_lookup_string(conf, "hlselbg", &dest))
                strcpy(colors[SchemeSelHighlight][ColBg], strdup(dest));

            // number
            if (config_setting_lookup_string(conf, "numfg", &dest))
                strcpy(colors[SchemeNumber][ColFg], strdup(dest));

            if (config_setting_lookup_string(conf, "numbg", &dest))
                strcpy(colors[SchemeNumber][ColBg], strdup(dest));

            // mode
            if (config_setting_lookup_string(conf, "modefg", &dest))
                strcpy(colors[SchemeMode][ColFg], strdup(dest));

            if (config_setting_lookup_string(conf, "modebg", &dest))
                strcpy(colors[SchemeMode][ColBg], strdup(dest));

            // border
            if (config_setting_lookup_string(conf, "borderbg", &dest))
                strcpy(colors[SchemeBorder][ColBg], strdup(dest));

            // caret
            if (config_setting_lookup_string(conf, "caretfg", &dest))
                strcpy(colors[SchemeCaret][ColFg], strdup(dest));

            // sgr colors
            if (config_setting_lookup_string(conf, "sgrcolor0", &dest))
                strcpy(textcolors[0], strdup(dest));
            if (config_setting_lookup_string(conf, "sgrcolor1", &dest))
                strcpy(textcolors[1], strdup(dest));
            if (config_setting_lookup_string(conf, "sgrcolor2", &dest))
                strcpy(textcolors[2], strdup(dest));
            if (config_setting_lookup_string(conf, "sgrcolor3", &dest))
                strcpy(textcolors[3], strdup(dest));
            if (config_setting_lookup_string(conf, "sgrcolor4", &dest))
                strcpy(textcolors[4], strdup(dest));
            if (config_setting_lookup_string(conf, "sgrcolor5", &dest))
                strcpy(textcolors[5], strdup(dest));
            if (config_setting_lookup_string(conf, "sgrcolor6", &dest))
                strcpy(textcolors[6], strdup(dest));
            if (config_setting_lookup_string(conf, "sgrcolor7", &dest))
                strcpy(textcolors[7], strdup(dest));
            if (config_setting_lookup_string(conf, "sgrcolor8", &dest))
                strcpy(textcolors[8], strdup(dest));
            if (config_setting_lookup_string(conf, "sgrcolor9", &dest))
                strcpy(textcolors[9], strdup(dest));
            if (config_setting_lookup_string(conf, "sgrcolor10", &dest))
                strcpy(textcolors[10], strdup(dest));
            if (config_setting_lookup_string(conf, "sgrcolor11", &dest))
                strcpy(textcolors[11], strdup(dest));
            if (config_setting_lookup_string(conf, "sgrcolor12", &dest))
                strcpy(textcolors[12], strdup(dest));
            if (config_setting_lookup_string(conf, "sgrcolor13", &dest))
                strcpy(textcolors[13], strdup(dest));
            if (config_setting_lookup_string(conf, "sgrcolor14", &dest))
                strcpy(textcolors[14], strdup(dest));
            if (config_setting_lookup_string(conf, "sgrcolor15", &dest))
                strcpy(textcolors[15], strdup(dest));

            // coloritems int
            config_setting_lookup_int(conf, "coloritems", &coloritems);
        }
    }

    // load options spmenu.image
    setting = config_lookup(&cfg, "spmenu.image");
    if (setting != NULL) {
        unsigned int i = 0;

        conflength = config_setting_length(setting);

        for (i = 0; i < conflength; ++i) {
            config_setting_t *conf = config_setting_get_elem(setting, i);

            // look up
            config_setting_lookup_int(conf, "width", &imagewidth); // spmenu.image.width
            config_setting_lookup_int(conf, "height", &imageheight); // spmenu.image.height
            config_setting_lookup_int(conf, "gaps", &imageheight); // spmenu.image.gaps
            config_setting_lookup_int(conf, "position", &imageposition); // spmenu.image.position
            config_setting_lookup_int(conf, "cache", &generatecache); // spmenu.image.cache
            config_setting_lookup_int(conf, "maxcache", &maxcache); // spmenu.image.maxcache
        }
    }

    // load options spmenu.xrdb
    setting = config_lookup(&cfg, "spmenu.xrdb");
    if (setting != NULL) {
        unsigned int i = 0;

        conflength = config_setting_length(setting);

        for (i = 0; i < conflength; ++i) {
            config_setting_t *conf = config_setting_get_elem(setting, i);

            // look up
            config_setting_lookup_int(conf, "global", &globalcolors); // spmenu.xrdb.global
            config_setting_lookup_int(conf, "xresources", &xresources); // spmenu.xrdb.xresources
        }
    }

    // load options spmenu.input
    setting = config_lookup(&cfg, "spmenu.input");
    if (setting != NULL) {
        unsigned int i = 0;

        conflength = config_setting_length(setting);

        for (i = 0; i < conflength; ++i) {
            config_setting_t *conf = config_setting_get_elem(setting, i);

            // look up
            config_setting_lookup_int(conf, "fast", &fast); // spmenu.input.fast
            config_setting_lookup_int(conf, "type", &type); // spmenu.input.type
            config_setting_lookup_int(conf, "password", &passwd); // spmenu.input.password
        }
    }

    // load options spmenu.mode
    setting = config_lookup(&cfg, "spmenu.mode");
    if (setting != NULL) {
        unsigned int i = 0;

        conflength = config_setting_length(setting);

        for (i = 0; i < conflength; ++i) {
            config_setting_t *conf = config_setting_get_elem(setting, i);

            // look up
            config_setting_lookup_int(conf, "default", &mode); // spmenu.mode.default

            config_setting_lookup_string(conf, "normal_text", &dest); // spmenu.mode.normal_text
            normtext = strdup(dest);

            config_setting_lookup_string(conf, "insert_text", &dest); // spmenu.mode.insert_text
            instext = strdup(dest);
        }
    }

    // load options spmenu.match
    setting = config_lookup(&cfg, "spmenu.match");
    if (setting != NULL) {
        unsigned int i = 0;

        conflength = config_setting_length(setting);

        for (i = 0; i < conflength; ++i) {
            config_setting_t *conf = config_setting_get_elem(setting, i);

            // look up
            config_setting_lookup_int(conf, "sort", &sortmatches); // spmenu.match.sort
            config_setting_lookup_int(conf, "casesensitive", &casesensitive); // spmenu.match.casesensitive
            config_setting_lookup_int(conf, "fuzzy", &fuzzy); // spmenu.match.fuzzy
            config_setting_lookup_int(conf, "preselected", &preselected); // spmenu.match.preselected
            config_setting_lookup_int(conf, "accuratewidth", &accuratewidth); // spmenu.match.accuratewidth
            config_setting_lookup_string(conf, "delimiters", &dest); // spmenu.match.delimiters
            worddelimiters = strdup(dest);
        }
    }

    // load options spmenu.line
    setting = config_lookup(&cfg, "spmenu.line");
    if (setting != NULL) {
        unsigned int i = 0;

        conflength = config_setting_length(setting);

        for (i = 0; i < conflength; ++i) {
            config_setting_t *conf = config_setting_get_elem(setting, i);

            // look up
            config_setting_lookup_int(conf, "height", &lineheight); // spmenu.line.height
            config_setting_lookup_int(conf, "lines", &lines); // spmenu.line.lines
            config_setting_lookup_int(conf, "columns", &columns); // spmenu.line.columns
            config_setting_lookup_int(conf, "indentitems", &indentitems); // spmenu.line.indentitems
        }
    }

    // load options spmenu.history
    setting = config_lookup(&cfg, "spmenu.history");
    if (setting != NULL) {
        unsigned int i = 0;

        conflength = config_setting_length(setting);

        for (i = 0; i < conflength; ++i) {
            config_setting_t *conf = config_setting_get_elem(setting, i);

            // look up
            config_setting_lookup_int(conf, "max", &maxhist); // spmenu.history.max
            config_setting_lookup_int(conf, "duplicate", &histdup); // spmenu.history.duplicate
        }
    }

    // load options spmenu.hide
    setting = config_lookup(&cfg, "spmenu.hide");
    if (setting != NULL) {
        unsigned int i = 0;

        conflength = config_setting_length(setting);

        for (i = 0; i < conflength; ++i) {
            config_setting_t *conf = config_setting_get_elem(setting, i);

            // look up
            config_setting_lookup_int(conf, "input", &hideinput); // spmenu.hide.input
            config_setting_lookup_int(conf, "larrow", &hidelarrow); // spmenu.hide.larrow
            config_setting_lookup_int(conf, "rarrow", &hiderarrow); // spmenu.hide.rarrow
            config_setting_lookup_int(conf, "prompt", &hideprompt); // spmenu.hide.prompt
            config_setting_lookup_int(conf, "items",  &hideitem); // spmenu.hide.items
            config_setting_lookup_int(conf, "powerline", &hidepowerline); // spmenu.hide.powerline
            config_setting_lookup_int(conf, "cursor", &hidecursor); // spmenu.hide.cursor
            config_setting_lookup_int(conf, "highlight", &hidehighlight); // spmenu.hide.highlight
            config_setting_lookup_int(conf, "matchcount", &hidematchcount); // spmenu.hide.matchcount
            config_setting_lookup_int(conf, "mode", &hidemode); // spmenu.hide.mode
            config_setting_lookup_int(conf, "hideimage", &hideimage); // spmenu.hide.image
        }
    }

    // load options spmenu.pango
    setting = config_lookup(&cfg, "spmenu.pango");
    if (setting != NULL) {
        unsigned int i = 0;

        conflength = config_setting_length(setting);

        for (i = 0; i < conflength; ++i) {
            config_setting_t *conf = config_setting_get_elem(setting, i);

            // look up
            config_setting_lookup_int(conf, "item", &pango_item); // spmenu.pango.input
            config_setting_lookup_int(conf, "highlight", &pango_highlight); // spmenu.pango.highlight
            config_setting_lookup_int(conf, "prompt", &pango_prompt); // spmenu.pango.rarrow
            config_setting_lookup_int(conf, "input", &pango_input); // spmenu.pango.input
            config_setting_lookup_int(conf, "leftarrow", &pango_leftarrow); // spmenu.pango.leftarrow
            config_setting_lookup_int(conf, "rightarrow", &pango_rightarrow); // spmenu.pango.rightarrow
            config_setting_lookup_int(conf, "numbers", &pango_numbers); // spmenu.pango.numbers
            config_setting_lookup_int(conf, "mode", &pango_mode); // spmenu.pango.mode
            config_setting_lookup_int(conf, "password", &pango_password); // spmenu.pango.mode
        }
    }

    // we're done here
    config_destroy(&cfg);
    return;
}
