## virtualbox扩容方法

### Windows端

​	进入`virtualbox`的安装目录，使用`VBoxManage modifyhd C:****** --resize ***`

### 虚拟机端

​	`sudo fdisk /dev/sda`可以进行分区查看，输入`n`创建新分区，输入`p`输出分区表，之后全部默认配置，最后`w`保存。最后输入`fdisk -l`可以显示分区的结果。

​	我们重启虚拟机，之后`sudo mkfs -t ext4 /dev/sda2`,将新增的分区格式化。挂载分区

```shell
sudo mkdir sda2
sudo mount /dev/sda2 ./sda2
```

​	最后进入`/etc`文件夹，在`fstab`增加一个项

```
/dev/sda2 /home/ubuntu/sda2 ext4 defaults 0 1
```

​	`sudo chomod 666 fstab`，重启虚拟机完成。