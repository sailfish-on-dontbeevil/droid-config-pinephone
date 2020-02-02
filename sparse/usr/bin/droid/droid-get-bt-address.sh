#!/bin/sh
echo "droid-get-bt-address: Setting up bluetooth address"

addresspath="/var/lib/bluetooth/"
addressfile="$addresspath/board-address"

mac=$(hciconfig | grep Address | grep -o "[[:xdigit:]:]\{11,17\}")
if [ ! -f "$addressfile" ] && [ ! -z "$mac" ]; then
    echo "File not found, saving address"
    mkdir -p "$addresspath"
    chmod 0755 "$addresspath"
    echo $mac > "$addressfile"
fi
