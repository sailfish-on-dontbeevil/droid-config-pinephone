#!/bin/sh
#Delay until sure the imei has been read
/bin/sleep 20

#Signal to start lipstick
touch /run/ofono-ready
