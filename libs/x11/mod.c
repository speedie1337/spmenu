/* See LICENSE file for copyright and license details. */

int get_locale(char *modifier_list) {
    if (XSetLocaleModifiers(modifier_list)) {
        return 0;
    } else {
        return 1;
    }
}

int supports_locale() {
    if (XSupportsLocale()) {
        return 0;
    } else {
        return 1;
    }
}
