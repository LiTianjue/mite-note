
## 1.安装软件
	windows exe,linux apt-get ...
---
## 2.安装Package control
#### 1. Ctrl + ` 调出控制台
#### 2. 复制以下内容并回车
`import urllib.request,os; pf = 'Package Control.sublime-package'; ipp = sublime.installed_packages_path(); urllib.request.install_opener( urllib.request.build_opener( urllib.request.ProxyHandler()) ); open(os.path.join(ipp, pf), 'wb').write(urllib.request.urlopen( 'http://sublime.wbond.net/' + pf.replace(' ','%20')).read())`
#### 3. 重启 Perferences->package settings中看到 package control
#### 4. Ctrl + Shift + p ,输入可以找到Install Package,回车之后就可以安装插件了
---
## 3.安装常用插件
#### 1.MarkDown
- 在浏览器中实时浏览 OmniMarkupPreviwer
- 语法高亮 MarkDown Editing
#### 2.中文支持 ConvertToUTF8
---
## 4.常用配置
- Preferences -> Settings - User
``` json
        {
            "line_numbers": true,      //是否显示行号
             "gutter": true,            //是否显示边列
            "draw_centered": false,      //是否居中显示
            //"wrap_width": 170,           //换行宽度(单位：字符)
            "word_wrap": true,          //是否自动换行
            "scroll_past_end": true,     //滚动能否超过结尾
            "font_face": "Consolas YaHei",          //字体
            "font_size": 11,         //字体大小
            "line_padding_top": 1,         //行高
            "line_padding_bottom": 1, 
            "tab_size": 4,         //TAB缩进宽度
            "translate_tabs_to_spaces": false,         //自动转换TAB为空格
            "update_check": false      //禁用自动更新
            //"theme": "Soda Dark 3.sublime-theme"         //皮肤
        }
```
- 使用VIM模式
    配置文件中加入"ignored_packages":[],

