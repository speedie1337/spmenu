/* See LICENSE file for copyright and license details. */

#if USEXRESOURCES
void resource_load(XrmDatabase db, char *name, enum resource_type rtype, void *dst) {
    char *sdst = NULL;
    int *idst = NULL;
    float *fdst = NULL;
    sdst = dst;
    idst = dst;
    fdst = dst;
    char fullname[256];
    char *type;
    XrmValue ret;
    snprintf(fullname, sizeof(fullname), "%s.%s", "spmenu", name);
    fullname[sizeof(fullname) - 1] = '\0';
    XrmGetResource(db, fullname, "*", &type, &ret);
    if (!xresources) return;
    if (!(ret.addr == NULL || strncmp("String", type, 64))) {
        switch (rtype) { // type
            case STRING:
                sp_strncpy(sdst, ret.addr, strlen(sdst));
                break;
            case INTEGER:
                *idst = strtoul(ret.addr, NULL, 10);
                break;
            case FLOAT:
                *fdst = strtof(ret.addr, NULL);
                break;
        }
    }
}

void load_xresources(void) {
    Display *display;
    char *resm;
    XrmDatabase db;
    ResourcePref *p;
    display = XOpenDisplay(NULL);
    resm = XResourceManagerString(display);
    if (!resm || !xresources)
        return;
    db = XrmGetStringDatabase(resm);

    // recognize global colors
    if (globalcolors) {
        for (p = cols; p < cols + LENGTH(cols); p++)
            resource_load(db, p->name, p->type, p->dst);
    }

    XCloseDisplay(display);
}
#endif
