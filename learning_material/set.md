## set

### 简介

​	`set` 是`bash`脚本的重要环节，下面将对这个命令进行介绍。

​	在执行`Bash`脚本的时候，会创建一个新的`Shell`，这个`Shell`就是这个`Bash`脚本的执行环境，这个环境中默认了很多参数。我们可以使用`set`来改变这个环境中的参数。

### set -u

​	`Bash`在遇到不存在的变量后，就会忽略它。如下面的代码:

```shell
#!/bin/bash

echo $a
echo bar
```

​	`a`是不存在的，则`Bash`直接忽略它。但是一旦设置`set -u`，遇到不存在的变量，会直接报错。

### set -x

​	默认的情况下，脚本执行后，屏幕只会显示运行的结果，没有其他内容。如果多个命令连续执行，那么它们的运行结果就会连续输出。但是如果加上了`set -x`，就会在输出结果前，先输出命令本身。

```shell
#!/bin/bash
set -x
echo bar
```

​	输出结果如下：

```shell
+ echo bar
bar
```

### set -e

​	`set -e`使得脚本一旦发生错误，就会终止执行。