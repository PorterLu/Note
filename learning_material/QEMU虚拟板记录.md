## QEMU虚拟板记录

### Qemu硬件SoC

![](https://raw.githubusercontent.com/PorterLu/picgo/main/qemu_virt_board.png)

​	这里需要定义`Qemu`初始化虚拟板的代码，我们需要注册一系列的函数来完成这些操作，这里通过一个结构体告诉`Qemu`定义的`SoC`的相关信息：

```c
static const TypeInfo quard_star_machine_typeInfo = {
    .name	= MACHINE_TYPE_NAME("quard-star"),
    .parent = TYPE_MACHINE,
    .class_init		= quard_star_machine_class_init,
    .instance_init	= quard_star_machine_instance_init,
    .instance_size 	= sizeof(RISCVVirtState),
}

static void quard_star_machine_init_register_types(void){
    type_register_static(&quard_star_machine_typeinfo);
}

type_init(quard_star_machine_init_register_types)
```

​	在`quard_star_machine_class_init`函数中对`SoC`进行了初始化，下面进行进一步的说明。初始化的过程主要是对地址空间进行说明，同时对支持的设备进行初始化。首先是物理核心数目的检查。

​	在`qemu`中：

* `socket`: 意味着物理`CPU`插槽数目
* `core`: 意味着每个物理`CPU`支持的核心数目
* `thread`:每个核心支持的线程数目

​	我们需要检查`socket num`、`hardid base`、`hart num`是否有问题。之后每一个`hart`都要对应`clint`中的寄存器，但是可以发现这里内存映射中只有一段的`clint`单元，这里是通过不同的`offset`的方式被不同的`hart`共享这段内存区间。

​	同时还要初始化`PLIC`中每个`socket`上的配置。

​	接下来要将`memory`的部分注册到`qemu`的地址空间上，包括`rom`,`sram`,`dram`, `flash`。一开始执行的代码是`rom`中的代码。

```assembly
auipc	t0, 0
addi	a2,	t0, 0x28
csrr	a0, mhartid
ld		a1, 32(t0)
ld		t0,	24(t0)
jr		t0
start: .dword
start_addr,
start_addr_hi,
fdt_laddr: .dword
fdt_load_addr,
```

​	执行`rom`代码后，`t0: jump_addr, a1:fdt_addr, a2:fw_dyn`，这里会跳转到`flash`中。当然还会有串口、`virtio`的说明。
