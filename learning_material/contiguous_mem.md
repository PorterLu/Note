# __get_free_pages
```c
  vaddr_t addr;
  int order;
  for(i = 0; i < 12; i++) {
    addr = __get_free_pages(GFP_KERNEL, order);
    if (addr == NULL) {
      printk("fail at %d\n", i);
      break;
    } else {
      free_pages(addr, i);
    }
  }
```
  这里在11的时候，分配失败，说明最大连续也为4MB, `GFP_KERNEL`为`get free page`，得不到页会进入休眠。
  
# dma_alloc_coherent
```c
vaddr = (vaddr_t) dma_alloc_coherent(device.this_device,
          num << PAGE_SHIFT,
          &device_phys_addr,
          GFP_KERNEL | __GFP_DMA32);
```
  经过测试`num`的最大值为1024，同样为`4MB`
