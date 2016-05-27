1.将本地的部分文件或文件件删除后，同步服务器上的相应文件或文件夹
	git checkout -- filename
	git checkout -- dirname

2.撤销git add
	git rm --cached <add file>

3.撤销git commit
	git reset --hard HEAD

4.从主分支中拉取数据到其他分支
	git chekout origin/master -- filename
	git chekout origin/master -- dirname
