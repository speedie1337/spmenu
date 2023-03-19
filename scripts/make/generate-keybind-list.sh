#!/bin/sh
grep "{ " "$1" | grep -v "[*]" | sed "s/{.i = //g; s/{.f = //g; s/{0} }/0/g; s/ } }//g; s/,//g; s/{ //g; s/^\s*//g; s/XK_//g"
