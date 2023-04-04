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
         fprintf(stderr, "spmenu: '%s' is not a valid configuration file.\n", cfgfile);
         config_destroy(&cfg);
         return;
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
}
