

Hello World
docker run hub.c.163.com/library/busybox echo hello world


1.Dockerfile

FORM		基础镜像，父镜像
MAINTARINER	维护者,邮件列表

EVN			环境变量

#　拷贝文件
ADD			[ADD]比起COPY　有解压功能和远程添加功能
COYP

#执行命令
RUN			执行命令

RUN			pip install supervisor	//安装进程管理工具，管理多个进程
ADD			supervisord.conf /etc/supervisord.conf

RUN			makedir  -p /etc/supervisor.conf.d 
			makdir   -p /var/log/supervisor

#虚拟机和宿主机的端口映射
EXPOSE		22

#只有一条，容器启动时候的命令
ENTRYPOINE ["/usr/bin/supervisord","-n","-c","/ect/supervisord.conf"] //开启进程管理服务

#　构建镜像的时候不生效，在下一个镜像生效
ONBUILD


2.通过Dockerfile构建镜像
	docker build -t [名称]　.
	docker build -t [名称]  [绝对路径]

	查看创建的镜像
	docker images

3.执行镜像
	docker run
	docker help run
	-it	交换模式启动，前端启动
	-d	后端启动
	-p	22			// 2222:22	制定端口映射
	-P  22			//随机端口映射
	--name 　base	//给容器起名
	镜像名

4.查看容器
	docker ps
	查看启动和未启动的容器
	docker ps -a

# 基础镜像
# 中间件镜像
# 应用镜像




