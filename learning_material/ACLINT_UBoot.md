## ACLINT_UBoot

### MSWI

​	`ipi`初始化，这里试图获取`TIIMER`的`uclass`， 这样就可以获取`MTIMER`中的`offset`数据，如果是`ACLINT`这个偏移值就是0，如果是`CLINT`这个偏移不为0。

​	这里可以这个可以看到判断如果这个值为0，这直接从设备树中获取。否则获取`mswi`的基地址。

```c
int riscv_init_ipi(void)
{
    int ret;
    struct udevice *dev;
    
    ret = uclass_get_device_by_driver(UCLASS_TIMER, DM_DRIVER_GET(sifive_clint), &dev);
    if(ret)
        return reg;
    /*
    * dev_get_driver_data() - get the driver data used to bind a device
    */
    if (dev_get_driver_data(dev) != 0)
        gd->arch.clint = dev_read_addr_ptr(dev);
    else
        gd->arch.clint = syscon_get_first_range(RISCV_SYSCON_CLINT);
    
    if (!gd->arch.clint)
        return -EINVAL;
    
    return 0;
}
```

​	`U_BOOT_DRIVER` 声明了如下这样`u device`，它名字叫做`riscv_aclint_swi`，它的`id`被分配为`UCLASS_SYSCON`，在结构体`riscv_aclint_swi_ids`声明了这个`udevice`所匹配的设备名字，这里目前只有`mswi`。

```c
static const struct udevice_id riscv_aclint_swi_ids[] = {
	{ .compatible = "riscv,aclint-mswi", .data = RISCV_SYSCON_CLINT },
	{ }
};

U_BOOT_DRIVER(andes_plicsw) = {
	.name		= "riscv_aclint_swi",
	.id		= UCLASS_SYSCON,
	.of_match	= riscv_aclint_swi_ids,
	.flags		= DM_FLAG_PRE_RELOC,
};
```

### MTIMER

`mtimer`的初始化根据第一个地址参数设置`mtime`寄存器的地址，`sifive_clint_get_count`可以获取值

```c
static u64 notrace sifive_clint_get_count(struct udevice *dev)
{
    return readq((void __iomem*)MTIME_REG(dev_get_priv(dev), dev_get_driver_data(dev)));
}

u64 notrace timer_early_get_count(void)
{
    return readq((void __iomem*)MTIME_REG(RISCV_MMODE_TIMERBASE, RISCV_MMODE_TIMEROFF));
}

static const struct timer_ops sifive_clint_ops = {
    .get_count = sifive_clint_get_count,
}

static int sifive_clint_probe(struct udevice *dev)
{
    dev_set_priv(dev, dev_read_addr_ptr(dev));
    if (!dev_get_priv(dev))
        return -EINVAL;
 	return timer_timebase_fallback(dev);   
}

static const struct udevice_id sifive_clint_ids[] = {
    { .compatible = "riscv,clint0", .data = CLINT_MTIME_OFFSET },
    { .compatible = "sifive,clint0", .data = CLINT_MTIME_OFFSET },
    { .compatible = "riscv,aclint-mtimer", .data = ACLINT_MTIME_OFFSET },
    {}
};

U_BOOT_DRIVER(sifive_clint) = {
	.name		= "sifive_clint",
	.id		= UCLASS_TIMER,
	.of_match	= sifive_clint_ids,
	.probe		= sifive_clint_probe,
	.ops		= &sifive_clint_ops,
	.flags		= DM_FLAG_PRE_RELOC,
};
```

