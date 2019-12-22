#!/bin/bash

# Disable EG25 modem
# GPIO export is done by enable-modem.sh
# GPIO35 is PWRKEY
# GPIO68 is RESET_N
# GPIO232 is W_DISABLE

# Turn RESET_N on
echo 1 > /sys/class/gpio/gpio68/value

# Turn W_DISABLE on
echo 1 > /sys/class/gpio/gpio232/value

# Trigger PWRKEY and wait until the modem has been turned off
echo 1 > /sys/class/gpio/gpio35/value && sleep 2 && echo 0 > /sys/class/gpio/gpio35/value
sleep 30
