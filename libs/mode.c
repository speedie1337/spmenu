void
switchmode(const Arg *arg)
{
    curMode = !curMode;
    allowkeys = !curMode;

    strcpy(modetext, curMode ? instext : normtext);

    drawmenu();
}
