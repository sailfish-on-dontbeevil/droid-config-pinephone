echo Setting bootargs
setenv rootfs_part 2
part uuid ${devtype} ${devnum}:${rootfs_part} uuid
setenv bootargs console=tty0 console=ttyS0,115200 root=PARTUUID=${uuid} no_console_suspend rootwait fbcon=rotate:1 earlycon=uart,mmio32,0x01c28000 panic=10 consoleblank=0
echo Environment:
printenv
echo Loading DTB
load ${devtype} ${devnum}:${distro_bootpart} ${fdt_addr_r} /sun50i-a64-pinetab.dtb
echo Loading kernel Image
load ${devtype} ${devnum}:${distro_bootpart} ${kernel_addr_r} /Image
echo Booting kernel NOW
booti ${kernel_addr_r} - ${fdt_addr_r}
