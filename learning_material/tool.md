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

