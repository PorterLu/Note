# Kconfig
&emsp;什么是Kconfig？Kconfig用于配置内核，内核的配置工具读取各个Kconfig文件，生成配置界面供开发人员
配置内核，最后生成配置文件`.config`。
## Kconfig的基本要素
1. config条目
   ```
   config TMPFS_POSIX_ACL
   bool "Tmpfs POSIX Access Control Lists"
   depends on TMPFS
   select GENERIC_ACL
   help
      ......
   ```
   解析：config是关键字，表示一个配置选项的开始，紧跟的TMPFS_POSIX_ACL是配置选项的名称，省略了前缀
   “CONFIG_”。bool表示变量类型，即“CONIG_TMPFS_POSIX_ACL”的类型，有5中类型：bool，tristate，string，
   hex和int，这里bool之后的字符串是提示信息，在配置界面中上下移动选中它时，就可以通过空格和回车键来
   设置。depends on，表示依赖于XXX，这里表示只有TMPFS配置选项选中时，当前配置信息才会出现。select表示
   当CONFIG_TMPFS_POSIX_ACL被选中时，GENERIC_ACL也会被选中。
2. menu条目
   ```
   menu "Floating point emulation"
   config FPE_NWFPE
   ......
   
   config FPE_NWFPE_XP
   ......
   
   endmenu
   ```
   解析：menu之后的Floating point emulation是菜单名，menu和endmenu间会有很多config条目
3. menuconfig
   ```
   menuconfig MODULES
      bool "Enable loadable module support"config
   if MODULES
   xx
   
   endif
   ```
   menuconfig和menu有点相似，但区别在于menu之后多了一个config，即这个menu是可以配置的，前面比menu
   类型多了一个方框，通过空格可以修改这个配置项的选中状态，如果选中，就会出现子目录，也就是说配置项    是位于if和endif中的内容。如果没有之间没有定义选项，就只能进入一个空目录。
4. choice条目
   ```
   choice
      prompt "ARM system type"
      default ARCH_VERSATILE
      config ARCH_AAEC2000
      ......
      config ARCH_REALVIEW
      ......
   endchoice
   ```
   choice条目将多个类似的配置选项组合在一起，供用户单选或者多选，给出提示信息“ARM system type”，光
   标选中回车后就可以看到多个config条目定义的配置选项，choice条目中定义的变量只有bool和tristate。
5. comment条目
   ```
   menu "Floating point emulation"
   comment "At least one emulation must be selected"
   config FPE_NWFPE
   ......
   config FPE_NWFPE_XP
   ```
   comment条目用于给出一些帮助信息，出现在界面的第一行。
6. source条目
   source条目用于读取另一个Kconfig文件，如：
   ```
      source "net/Kconfig"
   ```
## nemu如何使用Kconfig
&emsp; 键入 `make menuconfig` 后会执行如下的操作:  
1. 检查 `nemu/tools/kconfig/build/mconf`是否存在，若不存在，则编译生成mconf，nemu中
   的Kconfig进行了简化，源文件放在tools/kconfig下。
2. 检查 `nemu/tools/kconfig/build/conf`程序是否存在，不存在则编译生成。
3. 运行`mconf nemu/Kconfig`，解析Kconfig中的选项，并且根据用户的选项，生成nemu/.config。
4. 运行 `conf --syncconfig nemu/Kconfig`，conf解析nemu/Kconfig和nemu/.config，结合
   两者生成nemu/include/generated/autoconf.h,用于C代码中的宏定义；nemu/include/config/auto.conf，用于Makefile中的变量定义；nemu/include/config/auto.conf.md，用于Makefile中的依赖规则的生成；通过时间戳来维护配
   置选项变化的目录树nemu/include/config。
