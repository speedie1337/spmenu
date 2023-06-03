#!/bin/sh
# generate-keybind-list.sh
# Generates a keybind list in Markdown column format
#
# See LICENSE file for copyright and license details.
grep "{ " "$1" | \
    grep -v "[*]" | \
    sed "s/{.i = //g; s/{.c = //g; s/{.f = //g; s/{0} }/0/g; s/ } }//g; s/,//g; s/{ //g; s/^\s*//g; s/XK_//g; s/|/+/g" \
    > /tmp/data

cat << EOF > /tmp/wipdata
| Mode               | Modifier           | Key                | Function           | Argument           |
| :----------------- | :----------------- | :----------------- | :----------------- | :----------------- |
EOF

i=1

while true; do
    len="$(wc -l < /tmp/data)"
    mode="$(printf '%-18s' "$(sed -n ${i},${i}p /tmp/data | awk '{ print $1 }')")"
    mod="$(printf '%-18s' "$(sed -n ${i},${i}p /tmp/data | awk '{ print $2 }')")"
    key="$(printf '%-18s' "$(sed -n ${i},${i}p /tmp/data | awk '{ print $3 }')")"
    func="$(printf '%-18s' "$(sed -n ${i},${i}p /tmp/data | awk '{ print $4 }')")"
    arg="$(printf '%-18s' "$(sed -n ${i},${i}p /tmp/data | awk '{ print $5 }')")"
cat << EOF >> /tmp/wipdata
| $mode | $mod | $key | $func | $arg |
EOF
    [ "$i" = "$len" ] && break
    i=$((i+1))
done
cat /tmp/wipdata
rm -f /tmp/wipdata /tmp/data
