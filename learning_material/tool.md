## 常用工具
`xdg-open`: 打开pdf文件  
`curl`: `curl -o 本地名称 文件url` 下载文件

## 壁纸网站
wallhaven.cc

## tmux
在`./tmux.conf`设置`set -g mouse on`, 之后使用`tmux source`将其生效。这样就可以在`tmux`中进行鼠标滑动

## convert
图片格式转化，`sudo apt-get install imagemagick`，之后使用`convert mandelbrot.ppm mandelbrot.jpg`可以将`raw`格式的文件转化为`jpg`文件

## tee
使用`tee`可以输出信息的同时将输出的信息存到文件中，`./test 2>&1 | tee out.txt`  
其中`2>&1`意味着将错误也做为标准输出

## tool
在vim中使用`:%!xxd`将打开的二进制转化为16进制形式，修改后使用`:%!xxd -r`进行恢复，最后保存就可以完成二进制文件修改

## sgdisk

### -n

​	意思为`--new`，创建一个新的分区，之后的参数为`分区号|起始地址|终止地址`。

​	如果起始地址或者终止地址为0则分别表示第一个可用地址和最后一个可用地址。

### -i

​	意思是`--info`，可以用于显示一个分区的详细信息：

```shell
sgdisk -i 1 /dev/sdb
```

​	而`-p`可以查看所有的分区

### -c

​	意思是`--change-name`，可以修改一个分区的名字

```shell
sgdisk -c 1:"test" /dev/sdb
```
## cloudflare
To connect for the very first time you must call register first:

    Register the client warp-cli register.
    Connect warp-cli connect.
    Run curl https://www.cloudflare.com/cdn-cgi/trace/ and verify that warp=on.

## qqmusic
sudo vim /usr/share/applications/qqmusic.desktop  
在Exec后面添加--no-sandbox

## cpio
使用cpio -o -H newc进行创建一个Linux启动用的根目录  
1. cpio是一个创建、提取和处理cpio存档的命令行工具
2. -o 这是cpio的一个选项，表示将文件打包到存档中
3. -H newc，其中-H用于指定格式，而newc表示Linux启动过程中特定的cpio格式

## scp
secure copy: scp local_file user_name@ip_address:remote_file。这样就可以将文件拷贝到远程。

## asciidocFX
可以编辑adoc文件，并同时进行渲染，windows和linux平台皆可傻瓜式下载。

## webp
WebP 格式是一种旨在加快图片加载速度的图片格式。图片压缩体积大约只有 JPEG 的2/3，并能节省大量的服务器宽带资源和数据空间。在ubuntu上要对webp格式和jpg格式进行转化可以下载webp工具：
```shell
sudo apt install webp

## from jpg,png to webp
cwebp -q 80 image.png -o image.webp

## from webp to jpg,png
dwebp image.webp -o image.png

```
