## GPU

### GPU虚拟化方案[from 兰新宇知乎]

#### 概述

![](https://raw.githubusercontent.com/PorterLu/picgo/main/gpu_virtio_passthrough.png)

​	GPU也可以像其他的外设一样，可以借助virtio来实现guest VM对GPU的共享。因为Hypervisor的参与导致了性能不佳，可以将整个GPU passthrough给VM。

#### Passthrough

​	将一整个GPU给VM使用，多个GPU的情况可以一个VM一个，但是在只有GPU的情况也可以带来如下的好处。

1. 在VM内使用GPU驱动导致内核crash时，可以知识虚拟机本身出问题
2. 可以实现热插拔

#### 分享

![](https://raw.githubusercontent.com/PorterLu/picgo/main/SR_IOV.png)	

​		如果设备本身支持SR-IOV的话，可以从硬件上对于GPU资源进行划分，可以使用VF最后在GPU内部转化为PF使用。

[[GPU 虚拟化 [一\] - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/610441011)]