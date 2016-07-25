1. 3个内部组件
	docker images		//只读模板，用来创建容器
	docker registries	//dokcer 仓库,共有私有两种，用来上传下载image
	docker containers   //从image创建

2.Docker容器的启动
	 docker run -i -t ubuntu /bin/bash
	 1). 如果本地有ubuntu 这个image就从本地创建，否则从公有仓库下载
	 2). 从image 创建容器
	 3). 分配一个文件系统，并在只读的image层外挂载一层可读写的层
	 4). 从宿主主机配置的网桥接口中桥接一个虚拟接口到容器中去
	 5). 从地址池配置一个ip地址给容器
	 6). 执行你指定的程序，这里启动了/bin/bash
	 7). -it 指定标准输入输出(前台启动)

3.Docker 的底层技术
	2个核心技术 Namespaces 和 Control groups
	1) pid namsespaces
		不同的用户进程通过pid namespace 隔离开的，且不同的namespace中可以有个相同的pid
	2） net namespace
		每个net namespace 有独立的network devices ,IP address,IP routing tables./proc/net 这样的目录，将每个容器的网络隔离开来
	
	3) ipc namespace
		容器之间进程间交互还是host上具有相同pid namespace 中的进程间交互

	4) mnt namespace
		类似与chroot,将一个进程放到一个特定的目录执行
	
	5) uts namespace
		允许每个容器拥有独立的hostname 和 domain name,在网络上可以被视为一个独立的节点而非host上的一个进程

	6) user namespace
		每个容器都可以有不同的user和group id

	Control groups 可以用来隔离容器和宿主机之间的资源利用


/************************************************************/
	Docker image
/************************************************************/
	使用docker images显示本机上的images
	来自哪个仓库 mage标记	id
	一个仓库有一个image的多个发行版
	运行的时候通过tag 来指定版本
	docker run -it ubuntu:14:04 /bin/bash
	不指定的话默认会是 ubuntu:latest 这样子

1.获取image
	启动的时候本机上不存在，会自动下载，但是这样会很耗时
	可以通过docker pull 命令来预先下载我们需要的image
	下载一个centos镜像
	docker pull centos

2.查找image
	sudo docker search sinatra

3.下载 images
	docker pull training/sinatra


4.创建我们自己的images
	(1) 使用 docker commit 来扩展一个image
		先使用下载的image来启动容器，更新后提交结果到新的image
		docker run -it training/sinatra 
		//记住启动容器后的ID
		在容器里添加json gem
		gem install json
		运行结束后，使用exit来退出，现在容器已经被我们改变了，使用docker commit来提交副本
		docker commit -m="Aded gson gem" -a="Kate Smith" [源容器ID] ouruser/sinatra:v2
		-m 指定提交信息
		-a 指定跟新的用户信息
		指定从哪个image创建副本
		最后指定image的名字:版本标记

	(2) 从dockerfile 来创建一个image
		创建一个目录
		mkdir sinara
		cd sinatra
		touch Dockerfile
		示例：
		/* -------- */
		FORM ubuntu:15.04		//image的源
		MAINTAINER Andy Lee<mhpmii@126.com>	//维护者的信息
		#RUN指令在image中执行命令，安装一些软件包 
		RUN	apt-get -qq update
		RUN apt-get -qqy install ruby ruby-dev
		RUN gem install sinatra
		/* -------- */
		通过docker build 来通过Dockerfile 创建image
		docker build -t="ouruser/sinatra:v2" .
		-t 指定新的image 的用户信息和版本
		使用.来指定Dockerfile 的路径
		每一个操作都会创建一个新的image层，这些中间层在最后会被删除，只留下最后一层，最多支持127层

5.使用docker push 上传image
	docker push ouruser/sina

6.使用docker rmi 移除本地images
	docker rmi trainning/sinatra
	//这删除这个image之前要先删除(docker rm)依赖于这个image的容器


/************************************************************/
	Docker 中的网络映射
/************************************************************/
1.端口映射
	-P 随机映射一个49000到49900的端口到容器内的端口
	docker -d -P traiuning/webapp python app.py

	-p 指定映射的端口，在一个指定端口上只可以绑定一个容器
	docker -d -p 5000:5000 traiuning/webapp python app.py
	docker -d -p 127.0.0.1:5000:5000 traiuning/webapp python app.py
	绑定本地lo的任意端口到5000
	docker -d -p 127.0.0.1::5000 traiuning/webapp python app.py
	指定udp协议
	docker -d -p 127.0.0.1:5000:5000/udp traiuning/webapp python app.py
	
	使用docker port 来查看端口的绑定信息
	docker port nostalgic_morse 5000

	容器有自己的内部网络和ip地址，使用docker inspect 可以获取所有变量
	-p可以标记多个端口

2.docker 中的容器互联-linking系统
	docker 中的 linking系统可以连接多个容器
	他会创建一对父子关系
	父容器可以看到所选择的子容器的信息
	(1) 容器的命名系统
		--name 标记可以为容器命名
		docker run -d -P --name web training/webapp python app.py
		使用docker ps -l 查看命名结果
		使用docker inspect来查看容器的名字
		docker inspect -f "{{.Name}}" [ID]

	(2) 容器互联
		links 可以让容器之间安全的交互
		--link name:alias 
		name 名称
		alias 别名
		docker run -d --name db training/postgres
		删除之前的web容器
		docker rm -f web
		创建一个新的web容器，并将他link到db容器
		docker run -d -P --name web --link db:db training/webapp python app.py

		查看
		docker ps
		可以看到我们命名的容器，db和web,db容器的names里有db 也有web/db,表示web容器连接到db容器，他们是一个父子关系
		他们之间建立了安全的连接，不用映射端口到宿主主机
		在启动db的时候也不用-p和-P标记
		使用link之后我们就可以不用暴露数据库端口到网络上

		docker 通过2种方式为父子关系的容器公开连接信息
		* 环境变量
		* 更新/etc/hosts文件
		
		docker run --rm --name web2 --link db:db training/webapp env

		除了环境变量，docker 还添加host 信息到父容器的/etc/hosts的文件，
		docker run -t -i --rm --link db:db training/webapp /bin/bash
		cat /etc/hosts
		...
		172.17.0.7 aed84ee21bd3
		...
		172.17.0.5 db
		...
		#这里有2个hosts
		#第一个是web容器，web容器使用id作为他的主机名
		#第二个是db容器的ip和主机名
		apt-get install -yqq inetutils-ping	//官方ubuntu镜像没有安装ping
		ping db
		用ping 来ping db容器，他会被解析成172.17.0.5

		你可以连接多个子容器到父容器


/************************************************************/
	Docker 中的网络映射
/************************************************************/
