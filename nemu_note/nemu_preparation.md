# linux较为生僻的基本命令  
1. tar指令，如tar zcvf this.tar.gz ./\*.txt命令是将当前目录的所有文件打包压缩到this.tar.gz中；而tar xzvf this.tar.gz ./是将this.tar.gz解压到当前目录
2. file指令用于显示文件类型
3. tee用于重定向时既输出内容又定向输入文件
4. xargs用于重定向时输入的参数又多个的情况
5. uniq删去重复元素
6. time ./myprog用于记录程序运行的时间
# Makefile
```
hello:hello.c
  gcc hello.c -o hello
  
.PHONY: clean

clean:
  rm hello
```
.PHONY是伪目标，使得make clean总是能被执行
