void
switchmode(const Arg *arg)
{
    curMode = !curMode;
    allowkeys = !curMode;

    strncpy(modetext, curMode ? instext : normtext, 15);

    drawmenu();
}
