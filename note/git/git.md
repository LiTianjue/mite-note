1.系统环境设置
	一般只用设置用户信息和简单语法高亮
	git config --global user.name "LiTianjue"
	git config --global user.email "mhpmii@126.com"
	git config --global color.ui true
	git config --global core.editor vim
	
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

	//分支切换
	git branch -b bname
	//创建并切换分支
	git checkout -b newbranch

	//在分支中完成代码的修改，切换回主分支进行合并和删除分支
	git checkout master
	git merge bname
	git branch -d bname
	git branch

5.服务器
	git clone 之后，可以用origin 来代替服务器地址
	用git remote 来查看服务器相关信息
	git remote show origin

	修改
	git remote rm origin 
	git remote add origin

	创建新的remote 设置 ,这样就可以使用project1来代替远程地址
	git remote add project1 git@git.com

	在提交代码之前，首先要确认相关的更新已经合并到主分支master
	还要确认master的代码是服务器的最新代码，确保提交不冲突
	git pull origin master
	git push origin master

	提交标签到服务器
	git push origin master
	git push origin --tags

6.管理
	//检查损坏情况
	git fsck
	//清理无用的数据
	git gc


7.创建分支版本
	git branch NewBranch
	git checkout NewBranch
	//发布新分支到github上
	git push origin NewBranch
	//删除本地分支
	git branch -d NewBranch
	//删除远程的分支
	git push origin :NewBranch


8. git pull 的时候本地和服务器有冲突
	方法1：先将本地内容暂存，pull成功之后，在查看冲突文件，进行合并
		git pull		(失败，提示可以快进)
		git stash	(暂存，用git stash list 可以查看)
		git pull  	(重新pull)
		git stash pop stash@{0}	(还原暂存的内容)

		快速找回 git stash apply
		解决冲突，再次提交
		git stash clear

		内容冲突
		树冲突：一般是由于修改文件名造成的
			通过
				git rm 冲突文件
				git rm 原始文件
				git add 以后要使用的文件名
				git commit 来解决
			树冲突可以用 git mergetool来交互解决

		暴力解决，手动存储好修改过的文件
		然后使用 git rebase remote-branch-name 恢复到服务器的某个版本
		然后在在此基础上提交更新



