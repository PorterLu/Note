## strtok(char* str, char* delim)
```
#include <stdio.h>
#include <string.h>

int main(void)
{
  char str[20] = "hello world porter";
  char *p = strtok(str," ");
  char *p2 = strtok(NULL," ");
  char *p3 = strtok(NULL," ");
  printf("%s %s %s",p,p2,p3);
}

```
这个程序会输出hello world porter,首先strtok可以将str根据delim进行分割，第一次使用strtok必须传入一个字符数组（注意不可以是一个指针）,之后调用可以将第一个参数传入NULL，
默认当作上一次使用后指针指向的位置，如这里执行完第一次后将str中的hello后的空格替换为\0之后，指针指向了"w"的位置，之后p2获取的就是world并且"d"后的空格也变为\0，最后没有
找到空格，p3的指针就是指向"p"。

## readline(char*)
readline的参数是一个字符串，readline会先输出这个字符串再进行读取一行的数据，注意返回的字符指针是动态分配的，不再使用的话记得free()，这时我们实现的读取也可以像bash一样按
tab自动补齐了，编译时要记得加上 -lreadline。
add_history后按上下可以提示命令历史。
