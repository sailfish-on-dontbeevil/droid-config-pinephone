#!/bin/bash
# Unmute the A64 sound card at boot

# Configure A64 sound card in ALSA
CARD=1 # A64 sound card
amixer -c $CARD set 'AIF1 Slot 0 Digital DAC' unmute
amixer -c $CARD set 'Line Out' unmute
amixer -c $CARD set 'Line Out' 100%

# Set A64 sound card as default sink for Pulseaudio
pacmd set-default-sink alsa_output.platform-sound.stereo-fallback
