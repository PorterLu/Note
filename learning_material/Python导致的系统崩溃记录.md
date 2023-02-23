# Python导致的系统崩溃记录

## 简述

​	`Ubuntu`的一些系统功能依赖`python`，如果删除了系统的`python`文件，可能导致整个系统部分功能的崩溃，如网络、图形界面等等。本文档记录如何修复这样一次崩溃的修复过程。

## 崩溃

​	删库的第一瞬间，`python3`已经无法重新工作，退出`terminal`就无法再次启动了，关机后就无法进行图形界面了。如果再次启动会发现，网络的配置出了一些问题，同时无法正常使用`python`。

## 解决方案

​	我们在网络出现故障时，可以采用源码安装同时达到精确的版本控制的目的。

```shell
wget https://www.python.org/ftp/python/3.8.6/Python-3.8.6.tgz
```

​	这个网址大部分的情况都是连接不上的，可以换成阿里的源。

```shell
wget https://npm.taobao.org/mirrors/python/3.8.5/Python-3.8.6.tgz
```

​	这里指定安装目录，之后进行对系统`python`的替换。

```shell
./configure --enable-optimizations --prefix=/usr/python3
make -j4 && sudo make install
```

​	之后将所有`/usr/bin`和`bin`建立软连接到我们的安装目录，并且还可以设置我们的安装目录到`PATH`下，这里需要注意`home/用户`下是否有更高优先级的`.local`路径，注意除去。

## 重启

​	这个时候进行重启可能会失败，我们进行一次`recovery`启动，我们重新安装`ubuntu-desktop`。

​	我们现在`/etc/resolv.conf`中设定一个`nameserver` 并且重新设置网卡`sudo dhclient ens33`, 之后重新安装`sudo apt install ubuntu-desktop`重新启动。我们可以再次进行图形界面，但是`terminal`出错

## 修复terminal

​	我们还使用到了python的`openssl`，需要自行安装

```shell
wget https://github.com/openssl/openssl/archive/OpenSSL_1_1_1d.tar.gz
```

​	解压之后，

```shell
./config --prefix=/usr/local/openssl // 指定安装路径
make && sudo make install
```

​	重新安装我们的`python`

```shell
./configure --enable-optimizations --prefix=/usr/python3 --with-openssl=/usr/local/openssl
```

​	再次安装，我们需要安装启动`terminal`的一系列依赖。这里我的`pip`也出现了问题，我们同时要将`bin`和`usr/bin`下的`pip`软连接到我们的安装目录，同时设置号源。

```shell
//~/.pip/pip.conf
[global]
index-url = http://pypi.douban.com/simple
[install]
trusted-host = pypi.douban.com
```

```shell
sudo apt install libcairo2-dev libxt-dev libgirepository1.0-dev
pip install pycairo PyGObject
```

​	安装解决完这些依赖就可以启动`terminal`

