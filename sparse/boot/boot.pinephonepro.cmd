# This is the description of the GPIO lines used in this boot script:
#
# GPIO #105 is GPIO3_B1, or RK3399 ball C27, which controls the vibrator motor
# GPIO #154 is GPIO4_D2, or RK3399 ball AH3, which controls the red part of the multicolor LED
# GPIO #157 is GPIO4_D5, or RK3399 ball AJ3, which controls the green part of the multicolor LED
# GPIO #158 is GPIO4_D6, or RK3399 ball AG4, which controls the blue part of the multicolor LED
#

gpio set 105
gpio set 154

# Set root partition to the second partition of boot device
part uuid ${devtype} ${devnum}:1 uuid_boot
part uuid ${devtype} ${devnum}:2 uuid_root

setenv bootargs loglevel=4 console=tty0 console=ttyS2,1500000 console=${console} earlycon=uart8250,mmio32,0xff1a0000 consoleblank=0 boot=PARTUUID=${uuid_boot} root=PARTUUID=${uuid_root} quiet rw rootwait audit=0

if load ${devtype} ${devnum}:${distro_bootpart} ${kernel_addr_r} /Image; then
  gpio clear 105
  if load ${devtype} ${devnum}:${distro_bootpart} ${fdt_addr_r} ${fdtfile}; then
	gpio set 158
	booti ${kernel_addr_r} - ${fdt_addr_r};
  fi;
fi

# EOF
