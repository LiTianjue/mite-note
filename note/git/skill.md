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

5.删除已经加入到仓库的文件
	git rm -r --cached .idea/
	git rm --cached local.properites

	//如果不加 --cached ,那本地文件也会被删除掉


6. 当不小心提交了大文件
    提交了大文件，本地数据库被跟新，但是当你想向服务器提交的时候，会出现100M限制的问题

You need to:

use BFG Repo Cleaner, as suggested above.

bfg --strip-blobs-bigger-than 1M  my-repo.git
use 
git gc --agrressive --prune=now (after BFG), as detailed in "Reduce git repository size"

git push -f to force the new history on your remote repo.
