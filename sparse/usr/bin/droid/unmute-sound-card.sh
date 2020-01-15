#!/bin/bash
# Unmute the A64 sound card at boot
CARD=1 # A64 sound card

amixer -c $CARD set 'AIF1 Slot 0 Digital DAC' unmute
amixer -c $CARD set 'Line Out' unmute
amixer -c $CARD set 'Line Out' 100%
