void
switchmode(const Arg *arg)
{
    curMode = !curMode;
    allowkeys = !curMode;

    if (!curMode) {
        strcpy(modetext, normtext);
    } else {
        strcpy(modetext, instext);
    }

    if (hidemode) strcpy(modetext, "");

    drawmenu();
}
