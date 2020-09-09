#!/bin/sh

while [ ! -f /run/ofono-ready ] ;
do
        echo "Waiting for ofono..."
        sleep 2
done
echo "Ofono ready!"
