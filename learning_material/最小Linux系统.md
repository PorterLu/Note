## 最小Linux系统

```makefile
INIT := init

initramfs:
	@mkdir -p build/initramfs/bin
	sudo bash -c "cp /boot/vmlinuz build/ && chmod 666 build/vmlinuz"
	cp init build/initramfs/
	cp minimal build/initramfs/
	cp $(shell which busybox) build/initramfs/bin/
	
	cd build/initramfs &&
	find . -print0 |
		| cpio --null -ov --format=newc \
		| gzip -9 > ../initramfs.cpio.gz 

run:
	qemu-system-x86_64 \
		-serial mon:stdio \
		-kernel build/vmlinuz \
		-initrd build/initramfs.cpio.gz \
		-machine accel=kvm:tcg \
		-append "console=ttyS0 quiet rdinit=$(INIT)"
		
clean:
	rm -rf build
	
.PHONY: initramfs run clean
```

​	这里`INIT`表示了我们要运行的第一个程序，每次运行时我们都先要执行`initramfs`

### initramfs

​	这里我们创建`initramfs`， 是我们使用的最小`linux`待会使用的根目录。

1. `vmlinuz`是编译出来的`linux`镜像，我们将这个镜像拷贝到`build`目录下。
2. 之后`init`是我们要执行的初始脚本，我们将他放到`Linux`待会能看到的根目录下
3. `busybox`是一个工具集合，我们也从我们当前`Linux`系统中拷贝到根目录下。
4. 之后我们需要告知系统这个根目录的结构，
   1. `find . -print0`将输出当前目录下的所有文件，并且每个文件使用`null`进行间隔。
   2. `cpio`将文件格式转化为`newc`，`--null`说明使用`null`进行间隔，同时`-ov`表示进行`output`和`verify`。
   3. 最后使用`gzip`进行打包

### run

​	这里对于使用`qemu`的参数进行说明，

1. `-serial mon:stdio`， 我们串口的输入输出重定向到`host`
2. `-kernel build/vmlinuz`， 说明使用的`kernel`的镜像
3. `-initrd build/initramfs.cpio.gz`， 说明使用的初始的磁盘镜像。
4. `-machine accel=kvm:tcg`，使用`kvm`在本机上进行虚拟化。
5. `-append "console=ttyS0 quiet rdinit=$(INIT)"`:  传递参数给`kernel`，这里寿命使用`console=ttyS0`作为串口，同时`quiet`表示屏蔽非关键信息的输出，`rdinit=$(INIT)`指明会被第一个执行的程序

## INIT

​	这了我们已经指明了第一个执行的程序为`INIT`, 它需要创建`Linux`基本的对象，如一些驱动，文件系统。

```shell
#!/bin/busybox sh

BB=/bin/busybox

$BB echo -e "\033[31mHello, OS world\033[0m"

$BB sh

for cmd in $($BB --list); do
	$BB ln -s $BB /bin/$cmd
done

mkdir -p /tmp
mkdir -p /proc && mount -t proc none /proc
mkdir -p /sys  && mount -t sysfs none /sys
mknod /dev/tty c 4 1
setsid /bin/sh </dev/tty >/dev/tty  2>&1 
```

​		这里会对`busybox`进行软连接，之后挂载一些文件系统，之后说明`tty`。