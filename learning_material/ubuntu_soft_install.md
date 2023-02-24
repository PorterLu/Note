## visual studio code
以 sudo 用户身份运行下面的命令，更新软件包索引，并且安装依赖软件：
```
sudo apt update
sudo apt install software-properties-common apt-transport-https wget
```
使用 wget 命令插入 Microsoft GPG key ：
```
wget -q https://packages.microsoft.com/keys/microsoft.asc -O- | sudo apt-key add -
```
启用 Visual Studio Code 源仓库，输入：
```
sudo add-apt-repository "deb [arch=amd64] https://packages.microsoft.com/repos/vscode stable main"
```
一旦 apt 软件源被启用，安装 Visual Studio Code 软件包：
```
sudo apt install code
```
安装完成后，在所需的文件夹下 输入 code .即可启动。

版权声明：本文为CSDN博主「ASDDAG」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/qq_50840738/article/details/126060563
