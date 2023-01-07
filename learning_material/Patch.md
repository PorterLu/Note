# Patch

## 准备文件

1. cat testfile1

```shell
Hello,This is the firstfile!
```

2. cat testfile2

```shell
Hello,This is the secondfile!
```

## 生成Patch文件

​	使用`diff`命令生成补丁

```shell
diff testfile1 testfile2 > testfile.patch
```

## 使用补丁升级

```shell
patch -p0 testfile1 testfile.patch
```

​	使用完这条命令后，`testfile1`中的内容就会和`testfile2`一致了。

## 选项

* `-p0`，`-p`意味着忽略目录的层数，这里是0，意味着就是当前目录下
* `--forward`，可以保证过时的补丁不起作用。