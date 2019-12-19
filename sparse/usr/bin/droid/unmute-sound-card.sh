#!/bin/bash
# Unmute the A64 sound card at boot

amixer -c 0 set 'AIF1 Slot 0 Digital DAC' unmute
amixer -c 0 set 'Line Out' unmute
amixer -c 0 set 'Line Out' 100%
