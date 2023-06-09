## 常用工具
`xdg-open`: 打开pdf文件  
`curl`: `curl -o 本地名称 文件url` 下载文件

## 壁纸网站
wallhaven.cc

## tmux
在`./tmux.conf`设置`set -g mouse on`, 之后使用`tmux source`将其生效。这样就可以在`tmux`中进行鼠标滑动

## convert
图片格式转化，`sudo apt-get install imagemagick`，之后使用`convert mandelbrot.ppm mandelbrot.jpg`可以将`raw`格式的文件转化为`jpg`文件

## tee
使用`tee`可以输出信息的同时将输出的信息存到文件中，`./test 2>&1 | tee out.txt`  
其中`2>&1`意味着将错误也做为标准输出
