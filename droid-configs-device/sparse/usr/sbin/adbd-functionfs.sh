#!/bin/sh
set -e
mkdir -p /dev/usb-ffs
chmod 0770 /dev/usb-ffs
chown shell:shell /dev/usb-ffs
mkdir -p /dev/usb-ffs/adb
chmod 0770 /dev/usb-ffs/adb
chown shell:shell /dev/usb-ffs/adb
/bin/mount -t functionfs adb /dev/usb-ffs/adb -o uid=shell,gid=shell
exit 0
