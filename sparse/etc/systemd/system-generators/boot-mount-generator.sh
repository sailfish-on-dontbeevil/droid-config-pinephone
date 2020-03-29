#!/bin/bash

SERVICE_NAME=boot.mount
GENERATOR_DIR=$3
UNIT_FILE=$GENERATOR_DIR/$SERVICE_NAME
WHAT=`mount | grep " / " | awk '{print $1}' | sed 's/p2/p1/g'`

# Create new unit and overwrite if needed
function new_unit () {
        rm -f $UNIT_FILE
        touch $UNIT_FILE
}

# Append line to unit
function append_unit () {
        echo $1 | dd of=$UNIT_FILE oflag=append conv=notrunc
}

# Create unit
new_unit
append_unit "[Unit]"
append_unit "Description=Mount boot partition"
append_unit "Before=local-fs.target"
append_unit ""
append_unit "[Mount]"
append_unit "What=$WHAT"
append_unit "Where=/boot"
append_unit "Type=ext4"
append_unit "Options=noatime"
append_unit ""
append_unit "[Install]"
append_unit "WantedBy=local-fs.target"

# Enable unit
mkdir -p "$GENERATOR_DIR/local-fs.target.wants" 2>/dev/null
ln -sf "$UNIT_FILE" "$GENERATOR_DIR/local-fs.target.wants/$serviceName"
