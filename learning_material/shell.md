## bash -c "echo ..."
  如果要使用root权限写入一个文件时，容易写出下列的命令
```shell
sudo echo "some text" >> /path/file
```

但是这种情况下，`>> /path/file`会被优先执行，这样实际上我们可能没有权限写入file。所以正确的写法时
```shell
sudo bash -c "echo data >> /path/file"
```
