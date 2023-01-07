# fdisk

## 简介

​	一个硬盘可以被划分为一个或多个逻辑磁盘，我们称之为分区，我们对磁盘进行的划分信息被存在扇区0的分区表中。`Linux`需要至少一个分区来当作根文件系统，当我们创建一个分区时，我们必须将它格式化为一个合适的文件系统。要完成分区的工作，我们需要一些工具，这里介绍`fdisk`:

## Linux内核如何理解硬盘

​	在`Linux`操作系统中，外围设备都位于`/dev`挂载点，内核通过以下的方式理解硬盘：

* `/dev/hdX[a-z]`,  `IDE`硬盘
* `/dev/sdX[a-z]`, `SCSI`硬盘
* `/dev/vdX[a-z]`, 虚拟硬盘

## fdisk指令

​	`fdisk` 意味着 `format disk`, 它允许我们在每块硬盘上创建最多四个主分区，它们其中之一可以作为扩展分区，并下设多个逻辑分区。

![](https://raw.githubusercontent.com/PorterLu/picgo/main/disk_mbr.png)

## 使用fdisk

​	在执行操作之前，我们必须知道的是哪些磁盘已经加入操作系统。我们可以执行命令`sudo fdisk -l`，这样命令就可以列出磁盘名称、分区数量、分区表类型、磁盘识别代号、分区ID和分区类型。也可以在命令后面添加要查询的分区，指定一个分区。

```shell
$ sudo fdisk /dev/sdc

Welcome to fdisk (util-linux 2.30.1).
Changes will remain in memory only, until you decide to write them.
Be careful before using the write command.

Device does not contain a recognized partition table.
Created a new DOS disklabel with disk identifier 0xe944b373.

Command (m for help): m

Help:

  DOS (MBR)
   a   toggle a bootable flag
   b   edit nested BSD disklabel
   c   toggle the dos compatibility flag

  Generic
   d   delete a partition
   F   list free unpartitioned space
   l   list known partition types
   n   add a new partition
   p   print the partition table
   t   change a partition type
   v   verify the partition table
   i   print information about a partition

  Misc
   m   print this menu
   u   change display/entry units
   x   extra functionality (experts only)

  Script
   I   load disk layout from sfdisk script file
   O   dump disk layout to sfdisk script file

  Save & Exit
   w   write table to disk and exit
   q   quit without saving changes

  Create a new label
   g   create a new empty GPT partition table
   G   create a new empty SGI (IRIX) partition table
   o   create a new empty DOS partition table
   s   create a new empty Sun partition table

```

