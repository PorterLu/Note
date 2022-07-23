## 如何更新ubuntu
1. sudo apt update && sudo apt dist-upgrade
2. 之后启用sudo update-manager进行更新

因为 dist-upgrade 只会检查新的 LTS 版本，如果要检查所有稳定版本应执行：sudo do-release-upgrade，  
跟新系统需要其他程序，之前的sudo apt update或者sudo apt upgrade没检测到程序，可以sudo apt list --upgradable，之后用sudo upgrade one_program 进行更新
最后再进行系统的更新。

## 本次执行过程中遇到了ubuntu的bug，记录如下
csdn等论坛无果，用google输入出错文件处，搜索到如下的结果：  
```
(ubuntu forums)
 sudo mkdir /tmp/gccdox ##暂存出错的文件gcc-doc的安装信息
 sudo mv /var/lib/dkpg/info/gcc* /tmp/gccdox
 sudo dkpg --remove --force-remove-reinstreq gcc-doc #强制卸载包
 sudo apt-get remove gcc-doc ##告诉系统gcc已经被卸载
 sudo apt-get autoremove
 sudo apt-get autoclean #卸载依赖等信息
 最后可以重新进行安装
```

## dpkg的使用方法
本次学习中查阅了dpkg的使用方法  
1. dpkg -i package.deb进行安装
2. dpkg -r package.deb对包进行删除
3. dpkg -l 列出已经删除的包
4. dpkg -c package.deb显示包的内容
5. dpkg --remove --forece-remove-reinstreq 包名，进行强制卸载

## apt指令
```
apt-get update 更新包的来源，获取包的最新信息
apt-get upgrade 更新软件
apt-get install softwore 安装成熟版软件，虽然比较老
apt-get remove software 进行卸载
apt-get autoremove 同时将软件依赖的包也卸载
apt-get purge 软件卸载时也删除配置文件

```


