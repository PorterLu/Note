## likely 和 unlikely 实现原理
  在内核的代码中常常使用likely和unlikely来进行优化，它们实际上是对gcc中`__builtin_expect`的封装:
```c
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
```
  `__builtin_expect`用于告诉编译器，对于第一个变量它取取值为1或者0的可能性更大，从而编译器将可能性更大的代码直接放在前面代码的后面，从而减小性能的损耗。  
## 实验
```c
if(likely(n==10)){
  n = n + 2
} else {
  n = n - 2
}
```
  我们对这段代码进行反汇编，代码如下：
```assembly
 40049f:  cmp $0xa, %esi
 4004a2:  jne 404bc
 4004a4:  mov $0xc, %eax
 4004a9:  mov $0x400640, %edi
 4004ae:  xor %eax, %eax
 4004b0:  callq 400450
 4004b5:  xor %eax, %eax
 4004b7:  add $0x8, %rsp
 4004bb:  retq
 4004bc:  lea -0x2(%rax), %esi
 4004bf:  jmp 4004a9
```
  我们可以发现是jne即不相等的时候进行跳转，所以默认将`n = n + 2`的代码紧邻着条件语句放（我们编译时要加上`-O2`）
## 分析
  这么做，为什么能进行优化，这是因为现代处理器都是使用了缓存进行访存优化的，比如每一级的cacheline都是64bytes，那么如果我们现在`cmp`后确实没有跳转，那么很大概率可以直接从L1中直接读到对应的值。不过由于动态分支预测器的存在这种静态分支预测技术作用不大，但是如果你的CPU没有动态分支预测，且默认不进行跳转，那么这个技术仍然有帮助。
