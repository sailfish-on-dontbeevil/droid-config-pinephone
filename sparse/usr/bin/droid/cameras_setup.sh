#!/bin/sh
media-ctl -d /dev/media0 --set-v4l2 '"ov5640 3-004c":0[fmt:YUYV8_2X8/1280x720]'
media-ctl -d /dev/media0 --set-v4l2 '"gc2145 3-003c":0[fmt:YUYV8_2X8/1280x720]'
