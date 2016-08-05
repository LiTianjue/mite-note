一个代码仓库管理多个代码仓库
 git submodule的使用

 1. 创建一个新的工程
 
 2. 设置项目文件夹

3.添加子模块
git submodule add 仓库地址 路径
注：路径不能用 / 结束

4.当使用git clone下来的工程中带有submodule时，初始的时候，submodule的内容并不会自动下载下来的，此时，只需执行如下命令：
git submodule update --init --recursive

5.submodule的删除
在.gitmodules 文件中删除相应配置信息。
然后，执行git rm cached 命令将子模块所在的文件从git中删除。


6. 子模块更新后，管理工程版本停留在该工程的位置
    cd 到子模块
    git checkout master
    git pull

    回到父级模块，更新版本号