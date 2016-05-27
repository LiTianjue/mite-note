1.Ubuntu 中 Qt creator 关联,pro文件

创建一个文件 Qt-Creator.desktop
//我的装好后有 目录下已经有一个文件了，没有这个文件的自己创建一下
DigiaQt-qtcreator-community.desktop
输入一下内容 
[Desktop Entry]
Version=1.0
Encoding=UTF-8
Type=Application
Name=QtCreator
Comment=QtCreator
NoDsiplay=true
Exec=(Install folder of QT)/Tools/QtCreator/bin/qtcreator %f
Icon=(Install folder of QT)/5.4/Src/qtdoc/doc/images/landing/icon_QtCreator_78x78px.png
Name[en_US]=Qt-Creator

将文件放入目录
/usr/share/applications

编辑文件dafaults.list 加入
text/qtcreator=Qt-Creator.desktop;

编辑文件  mimeinfo.cache 加入
application/vnd.nokia.qt.qmakeprofile=qtcreator.desktop

sudo update-mime-database /usr/share/mime

这时候双击文件就能在qtcreator中打开了
参考链接
http://wiki.qt.io/Install_Qt_5_on_Ubuntu


