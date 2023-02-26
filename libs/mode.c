void
switchmode(const Arg *arg)
{
    selkeys = !selkeys;
    allowkeys = !selkeys;

    if (!selkeys) {
        strcpy(modetext, normtext);
    } else {
        strcpy(modetext, instext);
    }

    if (hidemode) strcpy(modetext, "");

    drawmenu();
}
