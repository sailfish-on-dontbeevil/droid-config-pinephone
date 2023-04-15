echo "========== Setting up bootargs ==========="
gpio set 98 # Enable vibrator
gpio set 114 # Turn LED green on
part uuid ${devtype} ${devnum}:2 uuid
setenv bootargs console=tty0 console=ttyS0,115200 root=PARTUUID=${uuid} no_console_suspend rootwait quiet earlycon=uart,mmio32,0x01c28000 panic=10 consoleblank=0 loglevel=0

echo "========= Loading DTB and kernel ========="
gpio set 115 # Turn LED red on

if load ${devtype} ${devnum}:${distro_bootpart} ${fdt_addr_r} /${fdtfile}; then
	echo "Loaded hinted fdtfile (${fdtfile})"
else
	echo "Hinted fdtfile not found, fallbacking to /sun50i-a64-pinephone-1.1.dtb"
	load ${devtype} ${devnum}:${distro_bootpart} ${fdt_addr_r} /sun50i-a64-pinephone-1.1.dtb
fi

load ${devtype} ${devnum}:${distro_bootpart} ${kernel_addr_r} /Image

echo "============= Booting kernel ============="
gpio set 116 # Turn LED blue on
gpio clear 98 # Disable vibrator
booti ${kernel_addr_r} - ${fdt_addr_r}

