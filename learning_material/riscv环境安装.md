# riscv环境安装

# gcc安装

1. `git clone --recursive https://github.com/riscv/riscv-gnu-toolchain ` 使用这个github地址安装gcc等一些编译工具。

2. `riscv64-unknown-elf-gcc`， 该工具，针对于riscv64架构的编译器，使用的C运行库为newlib。执行下列命令`./configure --prefix=/opt/riscv64 --with-arch=rv64imc --with-abi=lp64 `。其中`prefix`指定安装的路径，`--with-arch=rv64imc` 指定需要的riscv指令集，`--with-abi` 指定所支持的abi

3. 最后设置`/opt/riscv64/bin`到`PATH`路径下

## QEMU

我们从源码下载qemu。

```shell
wget https://download.qemu.org/qemu-4.1.1.tar.xz
tar xf qemu-4.1.1.tar.xz
cd qemu-4.1.1
./configure --target-list=riscv32-softmmu,riscv64-softmmu
make -j
```

安装依赖：

```shell
sudo apt install libpixman-1-dev 
```

在主目录下

```
QEMU_PATH=~/qemu-4.1.1
export PATH=$PATH:$QEMU_PATH/riscv32-softmmu:$QEMU_PATH/riscv64-softmmu
```

```shell
qemu-system-riscv64 --version
```



 