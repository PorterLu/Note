# OPENSBI

## 编译

```c
make all PLATFORM=generic LLVM=1 PLATFORM_RISCV_XLEN=64
```

​	其中`PLATFORM`指定为`generic`平台，同时开启`LLVM`编译器，最后指定为`RV64`。

## Firmware说明

为了兼容不同的运行要求，`OpenSBI`支持三种类型的`Firmware`，分别为：

* `dynamic`: 从上一级`Boot Stage`获取下一级的`Boot Stage`的入口地址，以`struct fw_dynamic_info`结构体通过`a2`寄存器传递。
* `jump`：假设下一级`Boot Stage Entry`为固定地址，直接跳转过去。
* `payload`: 在`jump`的基础上，直接打包进来下一级`Boot Stage`的`Binary`

## Opensbi domain

​	`opensbi domain` 是一个底层硬件的系统级分区。 

### 属性

* `order`, 一个内存区域的大小为$ 2^{order}$ 的大小。
* `base`，基地址必须是$2^{order}$ 对齐。
* `flags`，用于标记这个内存区域的内存类型(`RAM`或者`MMIO`) 并且紧跟着访问类型(`READ`、`WRITE`、`EXECUTE`等)。
