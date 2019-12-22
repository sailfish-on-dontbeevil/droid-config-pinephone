#!/bin/bash

# Enable EG25 modem
# GPIO35 is PWRKEY
# GPIO68 is RESET_N
# GPIO232 is W_DISABLE

# Setup GPIO pins
for i in 35 68 232
do
        echo $i > /sys/class/gpio/export
        echo out > /sys/class/gpio/gpio$i/direction
done

# Turn RESET_N off
echo 0 > /sys/class/gpio/gpio68/value

# Turn W_DISABLE on
echo 1 > /sys/class/gpio/gpio232/value

# Trigger PWRKEY
echo 1 > /sys/class/gpio/gpio35/value && sleep 2 && echo 0 > /sys/class/gpio/gpio35/value
