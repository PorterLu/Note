## 寻找包含特定关键字文件
`grep csr ./ -r | cat | grep ".*scala" | uniq`
## 浏览markdown文件
使用`markdown_py -o html4 README.md > README.html` 生成对应的html文件，使用firefox进行浏览
## pushd记录路径
`pushd .`,将当前目录移入shell维护的目录栈，栈顶永远时当前目录 
`push +n`,将第n个目录移动到栈顶，并cd到该目录  
`dirs`, 可以显示栈里的所有路径，`-c`可以清除所有路径  
`popd`,可以弹出0号目录，并cd到新的栈顶  
`popd +n`:可以弹出具体的路径  
## 二进制生成
`riscv64-linux-objdump -D -b binary -m riscv xxx.bin > xxx.asm` 可以反汇编bin文件

## Ubuntu 图标隐藏
```shell
gsettings set org.gnome.shell.extensions.desktop-icons show-home false
gsettings set org.gnome.shell.extensions.desktop-icons show-trash false
```
