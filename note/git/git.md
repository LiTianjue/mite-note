1.系统环境设置
	一般只用设置用户信息和简单语法高亮
	git config --global user.name "LiTianjue"
	git config --global user.email "mhpmii@126.com"
	git config --global color.ui true
	
	git config --list

	
2.创建软件仓库
	git init
	touch .gitignore

	git clone

3.基本操作
	//添加
	git add .
	git add file
	git add *.c
	//提交
	git commit -m "message"
	//提交的同时移除被删除的文件
	git commit -am "commit and delete file"
	
	//查看文件差异
	git diff [file]
	git diff --cached [file] //查看暂存区和代码仓库的差异

	//查看当前所有未提交的差异
	git diff HEAD

	//撤销
	git reset HEAD <file>
	git checkout -- <filename>
	
	// git reset + 
	HEAD  表示上一个版本
	HEAD^ 是倒数第二个版本
	HEAD^^ 是更老的版本
	--hard commit_id 指定版本

	//回退之后比回退版本新的版本将会看不到
	使用 git reflog 查看所有commit 记录

	//记录日志
	git log -p	//详细信息
	git log -2  //查看最新两条
	git log --stat //查看统计摘要

	//重做 最后一次提交
	git commit -amend

	//查看某一个文件的变更信息
	git show readme
	git show HEAD^ readme

	//使用tag 做版本的里程碑标记
	//在提交完代码之后，为这次提交打上tag
	git tag v0.9
	然后可以查看tag,等于用一个容易记住的tag号代替commit_id
	git show v0.9
	
	//补丁
	git diff > patch.txt
	git diff HEAD HEAD~ > patch.txt
	//打补丁
	git apply < patch.txt


4.使用分支来工作
	git branch <bname>
	//创建不以当前版本为起点的分支
	git branch <name> HEAD^^


