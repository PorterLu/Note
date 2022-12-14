## CSAPP 打桩

### 编译时打桩

```c
#include <stdio.h>
#include <malloc.h>

int main(){
    int *p =malloc(32);
    free(p);
    return(0);
}
```

```c
#define malloc(size) mymalloc(size)
#define free(ptr) myfree(ptr)

void *mymalloc(size_t size);
void myfree(void *ptr);
```

```c
#ifdef COMPILETIME
#include <stdio.h>
#include <malloc.h>

void *mymalloc(size_t size){
    void *ptr = malloc(size);
    printf("malloc(%d)=%p\n", (int)size, ptr);
    return ptr;
}

void myfree(void *ptr){
	free(ptr);
    printf("free(%p)\n",ptr);
}
```

​	编译时打桩的原理是利用宏编译，添加新的代码。对于上面的代码我们使用如下的语句进行编译：

```shell
gcc -DCOMPILETIME -c mymalloc.c
gcc -I. -o intc int.c mymalloc.o
```

​	一旦使用`-DCOMPILETIME`，就相当于我们定义了宏`COMPILETIME`, 于是`malloc`和`free`就会被翻译为`mymalloc`和`myfree`。这样我们就会通过我们自己的函数调用`malloc`和`free`，我们就可以进行一些额外的操作。

### 链接时编译

```c
#ifdef LINKTIME
#include <stdio.h>

void *__real_malloc(size_t size);
void __real_free(void *ptr);

void *__wrap_malloc(size_t size){
    void *ptr = __real_malloc(size);
    printf("malloc(%d) = %p\n", (int)size, ptr);
    return ptr;
}

void __wrap_free(void *ptr){
    __real_free(ptr);
    printf("free(%p)\n", ptr);
}
#endif
```

​	为了实现编译时插桩，我们必须在编译时加入一些参数。`Linux`静态链接器支持`--wrap f`进行插桩。这个标志可以告诉链接器，把对`f`的引用解释为`__wrap_f`，同时把`__real_f`解析为`f`。我们可以使用如下的代码进行编译。

```shell
gcc -DLINKTIME -c mymalloc.c
gcc -c int.c
gcc -W1,--wrap,malloc -W1,--wrap,free -o int1 int.o mymalloc.o
```

​	我们首先分别编译`mymalloc.c`和`int.c`生成对应的目标文件，之后由于编译选项`-W1,--wrap,malloc`和`-W1,--wrap,malloc` 。于是`int.c`中的代码就被解释为调用`__wrap_malloc`和`__wrap_free`。最后我们对于`__wrap_malloc`的调用和`__wrap_free`又会转化对于`malloc`和`free`的调用。

### 运行时打桩

​	运行时打桩，我们需要依赖基于动态连接机制的`LD_PRELOAD`环境变量。如果`LD_PRELOAD`被设置为一个共享路径名的列表（以逗号和空格进行隔开）。在解析未定义的引用时，动态链接库首先会搜索`LD_PRELOAD`库，然后再搜索任何其他的库。

```c
#ifdef RUNTIME
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

void *malloc(size_t size){
    void *(*mallocp)(size_t size);
    char *error;
    
    mallocp = dlsym(RTLD_NEXT, "malloc");
    if((error = dlerror()) != NULL){
        fputs(error, stderr);
        exit(1);
    }
    char *ptr = mallocp(size);
    printf("malloc(%d) = %p\n", (int)size, ptr);
    return ptr;
}

void free(void *ptr){
    void (*freep)(void *) = NULL;
    char *error;
    
    if(!ptr)
        return;
    
    freep = dlsym(RTLD_NEXT, "free");
    if((error = dlerror()) != NULL){
        fputs(error, stderror);
        exit(1);
    }
    freep(ptr);
    printf("free(%p)\n". ptr);
}
```

​	我们可以看大这里包含了一个库`dlfcn.h` 用于动态链接函数。于是如果我们在编译时指定连接这个我们自己的函数，就可以在执行时先执行我们自己的`malloc`和`free`，再执行标准库中的函数。

```shell
gcc -DRUNTIME -shared -fpic -o mymalloc.so mymalloc.c -ldl
LD_PRELOAD="./mymalloc.so" ./intr
```

