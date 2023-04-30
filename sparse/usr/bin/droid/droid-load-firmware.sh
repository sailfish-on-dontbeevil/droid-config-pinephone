#!/bin/sh

FIRMWARE_FOLDERS="/lib/firmware/ /usr/lib/firmware/"

log() {
    logger -p daemon.info -t firmware "$@"
}

log "Attempting to load firmware $FIRMWARE for $DEVPATH"

if [ -e /sys$DEVPATH/loading ]; then
    for folder in $FIRMWARE_FOLDERS; do
        if [ -e "$folder/$FIRMWARE" ]; then
            log "Loading firmware $folder/$FIRMWARE"

            echo 1 > /sys$DEVPATH/loading
            cat "$folder/$FIRMWARE" > /sys$DEVPATH/data
            echo 0 > /sys$DEVPATH/loading

            log "Loaded firmware $FIRMWARE"
            exit 0
        fi
    done

    log "Failed to find firmware $FIRMWARE for $DEVPATH"
    echo "\-1" > /sys$DEVPATH/loading
    exit 1
else
    log "Failed to find /sys$DEVPATH/loading, could not load $FIRMWARE."
    exit 1
fi
