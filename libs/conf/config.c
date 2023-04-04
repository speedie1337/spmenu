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
            fonts[0] = strdup(dest);

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
            config_setting_lookup_string(conf, "itemnormfg", &dest);
            colors[SchemeItemNorm][ColFg] = strdup(dest);

            config_setting_lookup_string(conf, "itemnormbg", &dest);
            colors[SchemeItemNorm][ColBg] = strdup(dest);

            config_setting_lookup_string(conf, "itemselfg", &dest);
            colors[SchemeItemSel][ColFg] = strdup(dest);

            config_setting_lookup_string(conf, "itemselbg", &dest);
            colors[SchemeItemSel][ColBg] = strdup(dest);

            // items with priority
            config_setting_lookup_string(conf, "itemnormprifg", &dest);
            colors[SchemeItemNormPri][ColFg] = strdup(dest);

            config_setting_lookup_string(conf, "itemnormpribg", &dest);
            colors[SchemeItemNormPri][ColBg] = strdup(dest);

            config_setting_lookup_string(conf, "itemselprifg", &dest);
            colors[SchemeItemSelPri][ColFg] = strdup(dest);

            config_setting_lookup_string(conf, "itemselpribg", &dest);
            colors[SchemeItemSelPri][ColBg] = strdup(dest);

            // input
            config_setting_lookup_string(conf, "inputfg", &dest);
            colors[SchemeInput][ColFg] = strdup(dest);

            config_setting_lookup_string(conf, "inputbg", &dest);
            colors[SchemeInput][ColBg] = strdup(dest);

            // menu
            config_setting_lookup_string(conf, "menubg", &dest);
            colors[SchemeMenu][ColBg] = strdup(dest);

            // prompt
            config_setting_lookup_string(conf, "promptfg", &dest);
            colors[SchemePrompt][ColFg] = strdup(dest);

            config_setting_lookup_string(conf, "promptbg", &dest);
            colors[SchemePrompt][ColBg] = strdup(dest);

            // arrows
            config_setting_lookup_string(conf, "larrowfg", &dest);
            colors[SchemeLArrow][ColFg] = strdup(dest);

            config_setting_lookup_string(conf, "larrowbg", &dest);
            colors[SchemeLArrow][ColBg] = strdup(dest);

            config_setting_lookup_string(conf, "rarrowfg", &dest);
            colors[SchemeRArrow][ColFg] = strdup(dest);

            config_setting_lookup_string(conf, "rarrowbg", &dest);
            colors[SchemeRArrow][ColBg] = strdup(dest);

            // highlight
            config_setting_lookup_string(conf, "hlnormfg", &dest);
            colors[SchemeNormHighlight][ColFg] = strdup(dest);

            config_setting_lookup_string(conf, "hlnormbg", &dest);
            colors[SchemeNormHighlight][ColBg] = strdup(dest);

            config_setting_lookup_string(conf, "hlselfg", &dest);
            colors[SchemeSelHighlight][ColFg] = strdup(dest);

            config_setting_lookup_string(conf, "hlselbg", &dest);
            colors[SchemeSelHighlight][ColBg] = strdup(dest);

            // number
            config_setting_lookup_string(conf, "numfg", &dest);
            colors[SchemeNumber][ColFg] = strdup(dest);

            config_setting_lookup_string(conf, "numbg", &dest);
            colors[SchemeNumber][ColBg] = strdup(dest);

            // mode
            config_setting_lookup_string(conf, "modefg", &dest);
            colors[SchemeMode][ColFg] = strdup(dest);

            config_setting_lookup_string(conf, "modebg", &dest);
            colors[SchemeMode][ColBg] = strdup(dest);

            // border
            config_setting_lookup_string(conf, "borderbg", &dest);
            colors[SchemeBorder][ColBg] = strdup(dest);

            // caret
            config_setting_lookup_string(conf, "caretfg", &dest);
            colors[SchemeCaret][ColFg] = strdup(dest);

            // sgr colors
            config_setting_lookup_string(conf, "sgrcolor0", &dest);
            textcolors[0] = strdup(dest);
            config_setting_lookup_string(conf, "sgrcolor1", &dest);
            textcolors[1] = strdup(dest);
            config_setting_lookup_string(conf, "sgrcolor2", &dest);
            textcolors[2] = strdup(dest);
            config_setting_lookup_string(conf, "sgrcolor3", &dest);
            textcolors[3] = strdup(dest);
            config_setting_lookup_string(conf, "sgrcolor4", &dest);
            textcolors[4] = strdup(dest);
            config_setting_lookup_string(conf, "sgrcolor5", &dest);
            textcolors[5] = strdup(dest);
            config_setting_lookup_string(conf, "sgrcolor6", &dest);
            textcolors[6] = strdup(dest);
            config_setting_lookup_string(conf, "sgrcolor7", &dest);
            textcolors[7] = strdup(dest);
            config_setting_lookup_string(conf, "sgrcolor8", &dest);
            textcolors[8] = strdup(dest);
            config_setting_lookup_string(conf, "sgrcolor9", &dest);
            textcolors[9] = strdup(dest);
            config_setting_lookup_string(conf, "sgrcolor10", &dest);
            textcolors[10] = strdup(dest);
            config_setting_lookup_string(conf, "sgrcolor11", &dest);
            textcolors[11] = strdup(dest);
            config_setting_lookup_string(conf, "sgrcolor12", &dest);
            textcolors[12] = strdup(dest);
            config_setting_lookup_string(conf, "sgrcolor13", &dest);
            textcolors[13] = strdup(dest);
            config_setting_lookup_string(conf, "sgrcolor14", &dest);
            textcolors[14] = strdup(dest);
            config_setting_lookup_string(conf, "sgrcolor15", &dest);
            textcolors[15] = strdup(dest);

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
