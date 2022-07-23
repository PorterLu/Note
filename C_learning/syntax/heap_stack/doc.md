数据库的使用说明
数据库的命令为database 根据文件名 action进行数据库打开
这样的使用是低效的，但是这个数据库只用于联系c语言的语法
Database_open(),用于生成一个链接类型，链接类型的一个成员是database，一个file，在内存数据结构和文件之间建立联系，并返回这个链接.
action输入为create时是只写打开，其他都是可读可写,将文件的内容读入到数据结构中.

Database_write 用于写数据库，即将内存的内容写入文件
Database_set 设置内存内数据结构的值
Database_get 得到对应id的数据库内容
Database_delete 删去一条记录
Database_list 显示数据库的非空内容


action 
c create
g get I<id>
s set <id> <name> <email>
d delete <id>
l list
