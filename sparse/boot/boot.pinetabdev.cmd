echo Setting bootargs
setenv bootargs console=tty0 console=ttyS0,115200 root=/dev/mmcblk${devnum}p2 no_console_suspend rootwait fbcon=rotate:1 earlycon=uart,mmio32,0x01c28000 panic=10 consoleblank=0
printenv
echo Loading DTB
load mmc 0:1 ${fdt_addr_r} /sun50i-a64-pinetab-dev.dtb
echo Loading kernel Image
load mmc 0:1 ${kernel_addr_r} /Image
echo Booting kernel NOW
booti ${kernel_addr_r} - ${fdt_addr_r}
