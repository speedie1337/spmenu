void
switchmode(const Arg *arg)
{
    selkeys = !selkeys;
    allowkeys = !selkeys;

    if (!selkeys) {
        strcpy(modetext, "Normal");
    } else {
        strcpy(modetext, "Insert");
    }

    if (hidemode) strcpy(modetext, "");

    drawmenu();
}
