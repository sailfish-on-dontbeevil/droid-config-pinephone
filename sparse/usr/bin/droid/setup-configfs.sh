#!/bin/sh

#Set up configfs as we are not using droid-boot init-script

USB_FUNCTIONS=rndis
GADGET_DIR=/sys/kernel/config/usb_gadget

# Sugar for accessing usb config
write() {
  echo -n "$2" > "$1"
}

mkdir $GADGET_DIR/g1
write $GADGET_DIR/g1/idVendor                   "0x18D1"
write $GADGET_DIR/g1/idProduct                  "0xD001"
mkdir $GADGET_DIR/g1/strings/0x409
write $GADGET_DIR/g1/strings/0x409/serialnumber "$1"
write $GADGET_DIR/g1/strings/0x409/manufacturer "Mer Boat Loader"
write $GADGET_DIR/g1/strings/0x409/product      "Pine64 Pinephone"

if echo $USB_FUNCTIONS | grep -q "rndis"; then
    mkdir $GADGET_DIR/g1/functions/rndis.usb0
fi
echo $USB_FUNCTIONS | grep -q "mass_storage" && mkdir $GADGET_DIR/g1/functions/storage.0

mkdir $GADGET_DIR/g1/configs/b.1
mkdir $GADGET_DIR/g1/configs/b.1/strings/0x409
write $GADGET_DIR/g1/configs/b.1/strings/0x409/configuration "$USB_FUNCTIONS"

if echo $USB_FUNCTIONS | grep -q "rndis"; then
    ln -s $GADGET_DIR/g1/functions/rndis.usb0 $GADGET_DIR/g1/configs/b.1
fi
echo $USB_FUNCTIONS | grep -q "mass_storage" && ln -s $GADGET_DIR/g1/functions/storage.0 $GADGET_DIR/g1/configs/b.1

echo "$(ls /sys/class/udc)" > $GADGET_DIR/g1/UDC
 
