## Visionfive2

### U-Boot

克隆官方目录：

```shell
git clone https://github.com/starfive-tech/u-boot.git
```

切换到对应的分支下

```shell
git checkout -b JH7110_VisionFive2_devel origin/JH7110_VisionFive2_dev
git pull
```

编译visionfive2提供的uboot

```shell
make starfive_visionfive2_defconfig ARCH=riscv CROSS_COMPILE=riscv64-linux-gnu
make ARCH=riscv CROSS_COMPILE=riscv64-linux-gnu-
```

生成3个文件

1. u-boot.bin (本体)
2. arch/riscv/dts/starfive_visionfive2.dtb （设备树文件）
3. spl/u-boot-spl.bin (一阶段引导文件)

## OPENSBI

克隆官方目录：

```shell
git clone https://github.com/starfive-tech/opensbi.git
```

在opensbi的目录进行编译

```shell
make ARCH=riscv CROSS_COMPILE=riscv64-linux-gnu- PLATFORM=generic FW_PAYLOAD_PATH={U-BOOT_PATH}/u-boot.bin FW_FDT_PATH={U-BOOT_PATH}/arch/riscv/dts/starfive_visionfive2.dtb FW_TEXT_START=0x40000000
```

会生成对应的固件到opensbi/build/platform/generic/firmware

## 创建SPL

克隆官方工具

```shell
git clone https://github.com/starfive-tech/Tools
```

编译工具

```shell
cd spl_tool
make
```

生成SPL	

```shell
./spl_tool -c -f {U-BOOT_PATH}/spl/u-boot-spl.bin
```

## 创建fw_payload

```shell
cd Tools/uboot_its
cp {OPENSBI_PATH}/build/platform/generic/firmware/fw_payload.bin ./
{U-BOOT_PATH}/tools/mkimage -f visionfive2-uboot-fit-image.its -A riscv -O u-boot -T firmware visionfive2_fw_payload.img
```

## 编译内核

克隆官方目录：

```shell
git clone https://github.com/starfive-tech/linux.git
```

编译visionfive2的分支

```shell
cd linux
git checkout -b JH7110_VisionFive2_devel origin/JH7110_VisionFive2_devel
git pull
```

之后进行编译

```shell
make starfive_visionfive2_defconfig
make CROSS_COMPILE=riscv64-linux-gnu- ARCH=riscv menuconfig
make CROSS_COMPILE=riscv64-linux-gnu- ARCH=riscv -j4
```

## 创建根文件系统

创建目录结构：

```shell
mkdir rootfs
cd rootfs
mkdir dev usr bin sbin lib etc proc tmp sys var root mnt
```

编译busybox

```shell
git clone https://git.busybox.net/busybox
cd busybox
make CROSS_COMPILE=riscv64-linux-gnu- ARCH=riscv menuconfig
```

选择静态编译

选择编译器前缀

选择生成目录

退出编译`make ARCH=riscv` ，并且`make install`

在rootfs/etc创建如下的文件

inittab文件

```shell
::sysinit:/etc/init.d/rcS
::respawn:-/bin/login
::restart:/sbin/init
::ctrlaltdel:/sbin/reboot
::shutdown:/bin/umount -a -r
::shutdown:/sbin/swapoff -a
```

在rootfs/profile

```shell
# /etc/profile: system-wide .profile file for the Bourne shells
echo
# echo -n "Processing /etc/profile..."
# no-op
# Set search library path
# echo "Set search library path in /etc/profile"
export LD_LIBRARY_PATH=/lib:/usr/lib
# Set user path
# echo "Set user path in /etc/profile"
PATH=/bin:/sbin:/usr/bin:/usr/sbin
export PATH
# Set PS1
# Note: In addition to the SHELL variable, ash supports \u, \h, \W, \$, \!, \n, \w, \nnn (octal numbers corresponding to ASCII characters)
# And \e[xx;xxm (color effects), etc.
# Also add an extra '\' in front of it! 
# echo "Set PS1 in /etc/profile"
export PS1="\\e[00;32m[$USER@\\w\\a]\\$\\e[00;34m"
# echo "Done"
```

在rootfs/fstab

```shell
proc /proc proc defaults 0 0
none /tmp tmpfs defaults 0 0
mdev /dev tmpfs defaults 0 0
sysfs /sys sysfs defaults 0 0
```

在rootfs/passwd

```shell
root:x:0:0:root:/root:/bin/sh
```

在rootfs/group

```shell
root:x:0:root
```

在rootfs/shadow

```shell
root:BAy5qvelNWKns:1:0:99999:7:::
```

在rootfs/etc

```shell
mkdir init.d
cd init.d
```

创建rcS

```shell
#! /bin/sh
#echo "----------mount all"
/bin/mount -a
#echo "----------Starting mdev......"
#/bin/echo /sbin/mdev > /proc/sys/kernel/hotplug
mdev -s
echo "********************************************************"
echo " starfive mini RISC-V Rootfs"
echo "********************************************************"
```

在rootfs/dev文件下

```shell
sudo mknod -m 666 console c 5 1
sudo mknod -m 666 null c 1 3
```

之后

```shell
ln -s bin/busybox init
```

最后

```shell
sudo chmod 777 -R *
find .| cpio -o -H newc | gzip > /home/user/Desktop/rootfs.cpio.gz
```

