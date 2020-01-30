#!/bin/bash
# Configure A64 sound card in ALSA
CARD=1 # A64 sound card
amixer -c $CARD set 'AIF1 Slot 0 Digital DAC' unmute
amixer -c $CARD set 'Line Out' unmute
amixer -c $CARD set 'Line Out' 100%

# Set A64 sound card as default sink for Pulseaudio
# Wait until the device appears
SOURCE="alsa_output.platform-sound.stereo-fallback"
while ! pactl list sources | grep "$SOURCE"
do 
        echo "Waiting for: $SOURCE"
        sleep 1
done
pacmd set-default-sink $SOURCE

echo "Done"
