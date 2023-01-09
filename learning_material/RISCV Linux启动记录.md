[TOC]

# RISCV Linux启动记录

## 确认版本

​	我们在`ubuntu20.04`进行我们的实验

```shell
lsb_release -a
```

## 下载依赖

​	一些固定的依赖进行下载安装

```shell
sudo apt install autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev \
                 gawk build-essential bison flex texinfo gperf libtool patchutils bc \
                 zlib1g-dev libexpat-dev git \
                 libglib2.0-dev libfdt-dev libpixman-1-dev \
                 libncurses5-dev libncursesw5-dev
```

## RISC-V 工具链

​	我们使用`git`获取工具链的源码，之后就可以进行配置安装。

```shell
git clone https://gitee.com/mirrors/riscv-gnu-toolchain
```

​	进入源码的目录中，删除`qemu`目录，之后下载所有子仓库。

```shell
git submodule update --init --recursive
```

​	对工具进行配置，设置安装的目录，这里对于`RISC-V`的工具链具体的配置采用默认配置。

```shell
./configure --prefix=/opt/riscv64
sudo make linux -j $(nproc)
```

​	然后设置环境变量

```shell
export PATH="$PATH:/opt/riscv64/bin"
```

## Qemu模拟器

​	首先仍旧是获取源码，步骤是解压，配置，编译，安装。

```shell
wget https://download.qemu.org/qemu-5.1.0.tar.xz
tar xvJf qemu-5.1.0.tar.xz
cd qemu-5.1.0
./configure --target-list=riscv64-softmmu,riscv64-linux-user --prefix=/opt/qemu
make -j $(nproc)
sudo make install
```

​	之后进行环境变量的设置

```shell
export PATH="$PATH:/opt/qemu/bin"
```

## Linux内核

​	仍旧是先获取源码。

```shell
git clone https://gitee.com/mirrors/linux.git
```

​	之后编译内核。

```shell
make ARCH=riscv CROSS_COMPILE=riscv64-unknown-linux-gnu- defconfig
make ARCH=riscv CROSS_COMPILE=riscv64-unknown-linux-gnu- -j $(nproc)
```

## 根目录

​	首先获取`busybox`源码。

```shell
git clone https://gitee.com/mirrors/busyboxsource.git
```

​	进入目录后，进行配置编译，`menuconfig`选择`Build Option`中的静态库。

```shell
CROSS_COMPILE=riscv64-unknown-linux-gnu- make menuconfig
CROSS_COMPILE=riscv64-unknown-linux-gnu- make -j $(nproc)
CROSS_COMPILE=riscv64-unknown-linux-gnu- make install
```

## 最小文件系统

​	回到我们实验的目录下，制作镜像。

```shell
qemu-img create rootfs.img 1g
mkfs.ext4 rootfs.img
```

​	将我们的之前的`busybox`生成的文件拷贝进入

```shell
mkdir rootfs
sudo mount -o loop rootfs.img rootfs
cd rootfs
sudo cp -r ../busyboxsource/_install/* .
sudo mkdir proc sys dev etc etc/init.d
```

​	创建`RC`文件

```shell
cd etc/init.d/
sudo touch rcS
sudo vi rcS

#rcS文件内容如下

#!/bin/sh
mount -t proc none /proc
mount -t sysfs none /sys
/sbin/mdev -s

sudo chmod +x rcS
```

其中`mount -t proc none /proc`意味着将`none`挂载到`/proc`上，类型为`proc`伪文件系统。`mdev`是`busybox`提供的一个工具，用在嵌入式系统中，相当于简化版的udev，作用是在系统启动和热插拔或动态加载驱动程序时，自动创建设备节点。文件系统中的/dev目录下的设备节点都是由mdev创建的。

​	之后`sudo umount rootfs`

## 启动

```shell
qemu-system-riscv64 -M virt -m 256M -nographic -kernel linux/arch/riscv/boot/Image -drive file=rootfs.img,format=raw,id=hd0  -device virtio-blk-device,drive=hd0 -append "root=/dev/vda rw console=ttyS0"
```

