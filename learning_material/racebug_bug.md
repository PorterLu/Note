## 防御性编程
### 什么叫Bug？
Bug可以定义为对于对于实现需求的代码违例
### 防御手段
 总是假设自己的代码是错误的，通过Assert去检查自己的代码错误。由于Bug的引入往往是程序员在编写代码时对于实际需求的语义缺失，所以检查本省可以赋予意义
```c
#define CHECK_INT(x, cond) panic!(concat(x, cond), "%s", cond)
