## loop设备

​	在类`Unix`系统中，`loop`设备是一种伪设备。在使用前，一个`loop`设备必须和一个文件进行连接。如果这个文件包含一个完整的文件系统，那么这个文件就可以像磁盘设备一样被`mount`起来。于是我们就可以像访问一个文件系统一样读写这个文件。

## losetup命令

​	此命令用来设置`loop`设备：

* `-e` 表示加密方式
* `-o`设置数据偏移
* `-d`卸载设备

​	例子如下：

```shell
dd if=/dev/zero of=floppy.img bs=512 count=2880
losetup /dev/loop1 floppy.img
mount /dev/loop0 /tmp
```

​	之后我们就可以通过`tmp`目录访问：

```shell
umount /tmp
losetup -d /dev/loop1
```

