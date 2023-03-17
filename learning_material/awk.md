## awk命令
 awk 可以通过指定分隔符，将一行分为多个字段进行输出
```shell
awk '{print $1, $7}' tmp.txt
``` 

* 我们还可以通过`-F ':'指定分隔符 
* 我们还以使用守卫条件，`awk $1 < 2 {print $7} tmp tmp.txt'
