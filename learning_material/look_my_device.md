## How
  今天需要查询自己的设备信息：
1. 通过fdisk -l可以查询自己的硬盘序列号。
2. 通过ifconfig可以看到自己的ip地址，在ether后是MAC地址。
3. 通过dmidecode | grep Serial可以看到自己机器的序列号。
