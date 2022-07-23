## makefile基础
makefile用于说明文件之间的依赖，并对应一系列的指令。程序转化可执行文件的过程，首先要把源文件编译成中间代码文件，然后把大量中间代码文件链接成可执行文件。  
编译时，编译器需要检查的是语法的正确，函数与变量的声明是否正确。链接时主要是链接函数和全局变量，中间文件打个包，就是库，也用于链接。

### 赋值
```
 =用于赋值，可以获取变量最后定义的值，后展开
 := 用于赋值，获取变量当前的值，立即展开
 += 可以进行拼接
```

### 伪目标
```
.PHONY用于定义为目标，用于方便make命令的执行。
1.如果文件的更新时间早于目标，command是不会启动的，但是伪目标不会有这个问题，make clean每一次的执行都可以执行clean下的command
2.主要是方便执行一些shell指令，不理解教程上的什么同名问题。
```

### 结构
```
target: prequisites
  command
target就是目标体，可以是Object文件、可执行文件和Label，而prequisites就是依赖文件，而command就是需要的命令，执行任何shell命令。
```

### 路径
```
Makefile提供了目录搜索功能：一般搜索VPATH和选择搜索vpath。
在Makefile中我们可以这样书写
VPATH:= src:car
记得多个路径需要用空格或者“:”隔开，make如果在当前目录下没有找到文件就会到VPATH下进行搜索，从左到右依次搜索src,car。
而vpath是进行选择性的搜索，它可以这样书写
vpath %.c src:car
就会在src和car搜索c的源文件

注意在执行shell命令时并不会自动从路径中获取头文件，仍然需要-I 进行说明。
```
### 基本参数与语法
```
gcc 的参数
-o:用于指定目标文件
-c:只产生目标文件，并不进行链接
-I:指定头文件的搜索目录
-l:如-lpthread，指定静态库

隐晦规则：
$@ 表示目标文件
$^ 全部的依赖文件
$< 表示第一个依赖文件

变量：
1.定义变量var后可以使用$(var)获取它的值。
2.foo := a.o b.o c.o
  使用bar := $(foo:.o=.c)将所有的.o尾的变量化为.c
3.变量还可以进行嵌套可以如下这样书写，结果会输出1
var := foo
foo := 1
.PHONY: print
print:
 -@echo $($(var)
 
 函数的使用：
 $(<function> <arguments>;)参数之间使用“,”进行隔开。
 
 语句的使用：
 $(if <condition>,<then part>,<else part>)
 
 值得注意的点：
 Makefile中的变量其实就是C中的宏，如果定义objects = *.o ,这里objects的值就是*.o并没有展开，如果想展开可以如下的方式书写
 objects:=$(wildcard *.o)
```

### 自动推导和自动生成依赖
```
只要make看到一个.o文件，它自动会把.c文件加到依赖文件中。这就是隐式规则。编译时如果加上-MMD还会生成头文件依赖文件，保证头文件更新目标文件也会被更新
例子如下：
--------------------------------------------------------main.c-------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include "command.h"

int main()
{
        printf("#### time: %s ####\n", __TIME__);
        command();//调用command.c里定义的C函数

        #ifdef __PUB_H__
                puts("##### the pub.h is included #####");
        #else
                puts("##### the pub.h is not included #####");
        #endif

        return 0;
}
-----------------------------------------------------------command.c------------------------------------------------------------------------
#include <stdio.h>
#include "command.h"

void command()
{
        puts("##### in command.c #####");
}
-----------------------------------------------------------command.h--------------------------------------------------------------------
#ifndef COMMAND_H
#define COMMAND_H

#include "pub.h"//复合嵌套

void command();

#endif
-----------------------------------------------------------pub.h---------------------------------------------------------------------------
#ifndef __PUB_H__       
#define __PUB_H__

#endif           
-----------------------------------------------------------Makefile------------------------------------------------------------------------
CC:=gcc
target:=app
source:=$(wildcard *.c)
header:=$(wildcard *.h)
objects:=$(source:.c=.o) 
dependence:=$(objects:.o=.d) $(header:.h=.d) ##*是在shell环境中搜索中用，到makefile中就可以用%

all:$(target)

$(target):$(objects)
    $(CC) $^ -o $@

%.o: %.c
    $(CC) -MMD -c $^

-include $(dependence)

.PHONY:all clean print

clean:
    rm $(objects) $(target) $(dependence) -rf
    
print:
    @echo target=$(target)
    @echo objects=$(objects)
    @echo dependence=$(dependence)
```

### makefile中常用的函数
```
$(foreach var, text, commond) 遍历text中，对每个var执行command
$(patsubst PATTERN,REPLACEMENT,TEXT) 匹配PATTERN进行替换
$(strip TEXT) 去除头尾的空格
$(filter PATTERN, TEXT) 保留符合PATTERN的部分
$(filter-out PATTERN, TEXT) 去除符合PATTERN的部分
$(dir NAMES) 取出路径的部分
$(wildcard PATTERN) 取路径下符合PATTERN的文件
$(eval TEXT) 执行TEXT

define func_name
 body
endef

$(call func_name,arg1,arg2,arg3)
```


