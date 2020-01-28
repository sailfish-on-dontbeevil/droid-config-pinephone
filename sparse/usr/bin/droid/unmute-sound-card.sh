#!/bin/bash
# Configure A64 sound card in ALSA
CARD=1 # A64 sound card
amixer -c $CARD set 'AIF1 Slot 0 Digital DAC' unmute
amixer -c $CARD set 'Line Out' unmute
amixer -c $CARD set 'Line Out' 100%

#Modem intut set to mono
amixer -c $CARD set 'Line Out Source' 'Mono Differential'

# Set A64 sound card as default sink for Pulseaudio
pacmd set-default-sink alsa_output.platform-sound.stereo-fallback

#Setup voicecall route
pactl load-module module-loopback source=alsa_input.platform-sound.stereo-fallback sink=alsa_output.platform-sound-modem.analog-mono
pactl load-module module-loopback source=alsa_input.platform-sound-modem.analog-mono sink=alsa_output.platform-sound.stereo-fallback
