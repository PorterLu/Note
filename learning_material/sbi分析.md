## sbi分析

### SBI标准接口

​	所有的`sbi`函数都使用一样的二进制接口，它的调用规则如下：

* 一次`ecall`用于转移控制从`supervisor`到`machine mode`。`a7`中是`sbi extension id`, 而`a6`中是`sbi function id`。
* 一次调用中，除了`a0`和`a1`其他寄存器都应该保留，`a0`中返回`error_code`, `a1`中返回是`value`，它可以用如下一个结构体表示

```c
struct sbiret {
    long error;
    long value;
}
```

### ecall服务调用过程

​	当`Linux`发起`ecall`后，`opensbi`会进行响应。具体原理如下：

​	在`firmware/fw_base.S`中注册了`Machine Mode`的`trap handler`，它的函数名为`sbi_trap_handler`，`sbi_trap_handler`会处理各种的`mcause`，如`illegal instruction`、`misaligned load`、`supervisor & machine ecall`。如果发现了是`ecall`，在这个函数会调用`sbi_ecall_handler`, 它会遍历`ecall_exts_list`中注册的各种服务。
