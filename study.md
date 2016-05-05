1.所有tcp/ip 调优参数位于 /proc/sys/net/下

2.设置系统缓冲
    sysctl -a | grep net.core
	/etc/sysctl.conf 中修改相应项目
	sysctl -p	//生效
	或者使用
	sysctl -w key=value 直接设置，但这样修改的开机会恢复

3.链路层MTU 1500字节
  Internet 标准MTU 576字节

4.命令split可以将文件分割
	split -b 20M BigFile Pack_
	使用cat命令可以将文件重组

5./run/shm 内存目录
	从/etc/fstab 中mount

6.忽略大小写进行字符串比较
	strcasecmp

7.获取程序的路径
	readlink("/proc/self/exe",buf,bufsize)

8.虚函数必须是类的非静态成员函数

9.容器中使用类，类必须有无参构造函数及可选的拷贝赋值函数

10.监控网卡流量
	iftop -i 网卡名
		  -B bytes显示
		  -n host显示为IP
		  -N 显示端口号
		  -F 指定网段
		  -m 设置刻度

11.查看内存使用 free top

12.解决文件中有windows风格的换行
	dos2unix工具
	vim  :set ff=unix

13.gdb 带参数调试
	gdb --args ./a.out arg1 arg2 ...
	adb ./a.out
	set args arg1 arg2 ...

14.编译内核/内核模块时缺少头文件
	apt-get install linux-headers-$(uname -r)

15. shell 脚本 “期待一元表达式”
	尝试用 [[ ]]解决

16. 检查挂载的存储设备
	/etc/mtab  --> /proc/mounts

17. 修改./configure 生成的Makefiel
	修改文件Makefile.in

18. 注意套接字被关闭时，read write 返回的是0而非负数

19. TCP 1448问题是由MTU 造成的，
	cat /sys/class/net/eth0/mtu
	大于1448的tcp数据其实上是会被分包的

20.添加默认网关的方法
	route add default gw 192.168.1.1

21. 定义char 的时候要如果要区分符号，一定要写上unsigned 
	arm-linux-gcc 中char 是无符号的
	x86-gcc       中char 是有符号的

22.#program once 宏写在头文件中和
	#ifndef 
	#define
	#endif
	的效果一样,需要编译器支持

23.查看系统限制的函数API
	sysconf
	内存，CPU,空闲内存...

24. 创建文件或者目录软连接
	ln -s 源文件 目标文件

25. 设置系统时间
	date -s "2016-05-03 16:15:00"

26. 段错误可能的原因
	1.交叉编译的时候使用了非交叉编译获得的头文件
	2.当不用标准的方式（*）比如用int表示一个指针，
	  32位和64位地址长度不同会造成段错误

27. glibc版本过低导致程序不能运行
	#!/bin/sh
	echo "deb http://ftp.debian.org/debian sid main" >> /etc/apt/sources.list
	apt-get update
	apt-get -t sid install libc6 libc6-dev libc6-dbg
	echo "Please remember to hash out sid main from your sources list. /etc/apt/sources.list"""""
