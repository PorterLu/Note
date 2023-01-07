## la
```Assembly
la t0, t0, 0x80000fff
```
  这行代码会出问题，因为`la`使用`lui`和`addi`（medany的情况）实现的，但是`addi`的实数是一个符号位扩展不能达到0xfff大小。如果要实现0xfff的效果，可以使用`lui`扩大常数值，再`addi`做一个减法