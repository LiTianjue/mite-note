# 和 c 语言的不同
头文件用包代替
第一个大括号不允许换行
分号可有可无

内置函数
println
len
iota
string
new
切片
make
append
copy

map
delete
range


有指针但是不支持指针运算及 -> 直接使用点 . 
使用 & 取地址，*号取值  空值是nil

++，-- 是语句而不能作为表达式
b := a++	是错误的，作为表达式放在等号右边

a++			作为语句运算 ++a 是错误的
b := a

go 语言的数组是数值而不是引用，传递给函数的时候是一份数组的拷贝
go 语言对相同类型的数组实现了 == != 的操作





1. 安装好之后，除了GOPATH其他环境变量一般都是一键配置

go get 		通过git下载远程包
go run 		直接运行程序（临时文件）
go build	测试编译，检查是否有编译错误，生成可执行程序
go fmt		格式化源码,一般不用手动调用
go install	编译包文件，然后编译主文件
go test 	运行测试文件 _test.go
go doc          查看一个函数或者文件


2. 配置好GOPATH 下的文件结构
	bin	//可以
	pkg
	src
   目录结构就是Makefile
   在 src 中新建目录 hello,创建源码hello.go
   执行 go install hello/
   在bin 目录中就得到二进制文件 hello
   如果源文件是非main包则生成.a放到 pkg目录下

3. 关键字
	package 组织程序 必须放在非注释第一行 可执行程序有且仅有一个main包
		可执行程序必须是 main 包，main 函数
	import  关键字来导入其他包
	const   来定义常量
	var     定义全局变量
	type	一般类型的声明
	//type 变量名 类型名
	type newname oldname

	type 结构名 struct {
		//...
	}

	type 接口名 interface {
		//...
	}
	func 函数名 {
		//
	}
	

4. 导入包
	import "fmt"
	import "os"
	import {
		import "fmt"
		import "os"
	}

   包别名
	import std "fmt"
	import . "fmt"	//省略调用

   通过 包名.方法名 调用
   最好不使用省略调用

5. 可见性规则
	通过大小写规定，首字母是大写的函数和变量才可以被外部调用

6. 基本类型
	bool   		只有true false 不可以和数值之间做隐式但转换
	int/uint 	平台相关 32/64位
	int/uint8	8位整形
	int16/uint16    16字节整型
	int32/uint32	32字节整型
	int64/uint64	64字节整型
	float32/float64 4/8 字节但浮点型，有7/15但精度
	complex64/copmlex128 复数类型
	
	[别名] 用于增强代码但可读性
	byte		字节型 uint8 的别名
	rune		int32 的别名,这个别名为处理unicode
	uintptr		uint 的别名，32/64位，指示用来表示指针

	[其他类型]
	array struct string
	[引用类型]
	slice map chan
	[接口类型]
	interface
	[函数类型]
	func

	[数组]
	var ar []int
	
7. 类型的其他说明
	数值	0
	bool	false
	string	空字串
	其他	nil
	
	类型的最大最小值
	import math
	math.Max_
	math.Min_

	类型但别名(类似于typedef)
	type{
		byte 		int8
		rune 		uint
		ByteSize	int64   
	}

8. 变量的声明与赋值
	声明  var 变量名 变量类型
	赋值  变量名 = 值
	声明的时候赋值 { var 变量名 类型 =  值 }
	
	[类型推断] (省略 类型名)
	go 是强类型，不允许隐式但转换，所以可以通过赋值推断类型
	如 声明 int 型变量 {  var num = 1 }
	
	[省略 var 关键字] (全局变量用 var 声明，不能用：= 声明赋值)
	用 := 
	{ num := 1 }

	[多个变量的同时声明]
	方法组内声明与赋值 不能用：,: 本身就是替代var 关键字的，但是全局变量不能用：
	var (
		a = "string"
		b = 1
		c = 1.2
	) 
	并行的声明与赋值
	var a,b,c,d int = 1,2,3,4
	var a,b,c,d = 1,2,3,4		// 推断
	a,b,c,d := 1,2,3,4		// 省略var关键字

	[空白符号] 忽略某个值
	a,_,c,d = 1,2,3,4
	
9. 变量的转换
	数值之间的转换 显式的转换
	bool 类型不能和数值转换
	a := 1.1
	b := int(a)

	错误的写法 1
	var s string = "65"
	var s2 int = int(s)
	错误的写法 2
	var b bool = false
 	var b2 int = int(b)

	可以运行但不能获得期望的转换
	var x int =65
	s:= string(x)
	fmt.Println(s)	'A'	//e newname oldname
	想获得 字符串 "65"
	import strconv
	s := strconv.Itoa(x)
	b,_ := strconv.Atoi(s)



10. 常量与运算符
	常量的值在编译的时候已经确定
	常量的右值必须是常量和内置函数
	const c int = 1
	const c  = 1
	const a,b,c = 1,"2",3
	const (
		a = 1
		b = "2"
		c = 3
	)
	方法组中常量不赋初始值则使用上一个赋值的值
	const (
		a = 1
		b
	)
	const (
		a,b = 1,"2"
		c,d			// c =1,b = "2"
	)

	错误的写法1 右值不是常量
	var a := "string"
	const  b = len(a)
	错误的写法2 自动赋值格式不一致
	const (
		a,b = 1,2
		c
	)

	[实现枚举效果] 从 0 开始
	const (
		a = iota	//0
		b			//1
		c			//2
		d			//3
	)

	const (
		a = 2		//2
		b = iota	//1
		c			//2
		d			//3
	)
	const (
		a = 'A'		// 'A'
		b = iota	// 1
		c = 66		// 66
		d			// 66
		e			// 66
	)
	const (
		a = 'A'		// 'A'
		b = iota	// 1
		c = 66		// 66
		d = iota	// 3
		e			// 4
	)

	如果上一个常量的赋值是个表达式样，则自动赋值会套用上一个常量赋值的表达式
	const (
		B float = 1 << (iota * 10)
		KB
		MB
		GB
		TB
	)
	
11. 运算符 从左到右的
	一元运算符	^ !
				* / % << >> 
	位运算符	& | ^  &^
	二元运算符	+ - | ^
				== != < <= >= >
	专用运算符 <-
	逻辑运算符	&& ||


12. 控制语句
	[if 表达式]
	条件表达式没有括号
	if 1<2
	支持初始化表达式,作用域是这个表达式块，包括else
	if a:=1;a > 1 {
	
		fmt.Println(a)
	}else{
		a++
	}
	
	[循环 for]
	只有 for
	死循环	// c while(1)
	for {
	
	}
	
	带条件	// c while(a<3)
	for a<=3 {
	}

	带步进
	for i:=0;i<3;i++ {
	
	}

	[选择语句 switch]
	不需要写break
	需要继续检查关键字 fallthrough
	支持初始化表达式 及条件表达式
	通常方式 （C语言格式）
	switch a {
		case 1:
			xxx
		case 2:
			xxx
		default:
			xxx
	}
	带条件表达式
	switch {
		case a > 0:
			xxx
			fallthrough
		case a > 1:
			xxx
		default
	}
	带初始化表达式 作用于语句块
	switch a:= 9{
		case a > 0:
			xxx
		case a > 1:
			xxx
		default
	}

	
	[跳转语句]
	goto break continue
	常规用法和C一致
	配合标签可以多层跳转
	LABEL1:
	for {
		for i:=0;i<10;i++ {
			if i > 3 {
			break LABEL1	//break 和标签同级的循环，退出死循环
			goto LABEL1		//跳转到标签，还是一个死循环	
			continue LABEL1	//continue 和标签同级的循环，还是一个死循环
							//和 goto不一样的是，如果最外层的循环含有变量的话，变量的值不会重新修改
			}
		}

	}


13. 数组
	var a [2]int	// [2]int 是类型
	var b [1]int
	a = b			//会报错，不同类型的数据不能相互赋值
	普通顺序赋值
	a := [10]int{1,2,3,4}	//未初始化的值为0
	索引赋值
	a:= [10]int{9:99}
	自动计算长度
	a:= [...]{1,2,3,4}	//长度为4的数组
	a:= [...]{19:1}		//长度为20的数组

	数组的指针
	a:=[20]int{19:9}
	var p *[100]int = &a

	指针数组
	x,y := 1,2
	a:=[...]*int{&x,&y}

	go语言中，数组不是引用类型，而是值类型
	go语言实现了数组的比较 == ，!= 
	不同长度的数组是不同类型,不能进行比较


	指向数组的指针
	p:= new([10]int)
	无论是指向数组的本身还是数组本身都可以用索引的方式赋值操作
	p[1] = 10

	多维数组 结尾的括号一定不换行
	a := [3][3]int{
		{1,1,1},
		{2,2,2}}

	a := [...][3]int{	//可以省略行数
		{1,1,1},
		{2,2,2}}

	
	

14. 切片Slice
	 底层是数组，是引用类型 ,变长数组的替代方案
	 直接创建或者从数组获得
	 len()获取长度，cap() 获取容量
	 使用 make() 创建，不是单纯的指向指针的指针
	 
	 var a1 [10]int		//数组
	 var s1 []int		//切片
	 
	 从数组创建(包含起始值不包含结尾值) 指向同一个数组切片会相互影响
	 var s2 = a[2：9]
	 var s3 = a[2:len(a)]
	 var s4 = a[2:]
	 var s5 = a[:5]
	 //这几个切片底层指向数组a,一个被修改，其他都会被修改

	 使用make
	 s1 := make([]int,3,10)	//最后一个参数是容量,重新分配或增大1倍


	 从另一个切片生成 reslice
	 索引以原来的切片对齐 ,这时候切片会相互影响
	 a := [10]int
	 sa := a[0:8]		//容量为10
	 sa1 := a[2:8]		//容量为8
	 sa2 := a[0:2]		//容量为2
	 sa3 := sa1[1:]		//容量为7

	 追加 Append
	 sa append(sa,1,2,3)
	 这时候如果超过cap,则会重新分配内存，重新分配后切片就不会相互影响了

	 拷贝 Copy
	 s1 := []int{1, 2, 3, 4,5,6}
	 s2 := []int{7,8,9}
	 copy(s1,s2)
	 以短的切片为准
	 copy(s2,s1[2:4])
	 copy(s2[1:],s1)
	 两个数组之间相互拷贝在重新赋值不会相互影响，因为他们指向的不是同一个数组



15. map
	map[key]value
	key 必须支持= != , value 可以是任何类型
	声明
	var m map[int]string
	m  = map[int]string{}

	var m map[int]string = make(map[int]string)

	m:=make(map[int]string)

	赋值
	m[1] = "OK"

	删除
	delete(m,1)

	复杂的map 每一级的map都需要分别初始化
	var m map[int]map[int]string
	m = make(map[int]map[int]string)
	m[1] = make(map[int]string)
	m[1][1] = "OK"

	map取值的多返回
	a,ok = map[2][1]  ok 是bool型

	迭代操作
	for i,v := range slice {	//i 是索引，v是值的拷贝
		
	}
	
	for k,v := range map {		// K 是键值，v 也是拷贝
	
	}

	对map的排序间接排序
	map 的迭代操作是无需的
	m := map[int]string{1:"A",2:"B",3:"C",4:"D"}
	s := make([]int,len(m))

	i:=0
	for k,v range m {
		s[i] = k;
		i++
	}
	//每次取到的切片顺序都是不一样的
	import "sort" //进行排序
	
	sort.Ints(s)

	
	

16. 函数
	不支持嵌套，重载，默认参数
	无需声明原型，参数不定长，多返回值，命名返回值参数，匿名函数，闭包
	函数是一种数据类型
	大括号不可以另起一行

	func 函数名(参数名称 参数类型,...)(返回类型,...) {
	}
	func A(a int,b string)(int,string)
	{
	}
	func A(a int,b int)string{
	}
	
	同参数类型简写
	func A(a,b,c int)(int ,int ,int){
	
	}
	命名返回值的写法
	func A()(a,b,c int){
		//已经声明了 a,b,c
	}
	不定长可变参数 (其实是一个slice,但是是值拷贝的slice)
	func A(b string, a ...int){
		//这串 slice
	}
	如果参数是一个真实的slice,那他就是引用

	函数类型的使用
	a := A		//类型声明赋值
	a()			//调用函数

	匿名函数
	a := func(){	//a是指向匿名函数的变量
		//
	}

	闭包函数 ,外层函数返回匿名函数
	func B(x int) func(int) int{
		return func(y int) int {
			return x + y
		}
	}

	defer
	类似于其他语言的析构函数
	在函数体执行结束后会按相反的顺序执行 即使严重错误也执行
	在return 之后还可以修改数据
	
	fori :=0;i < 3;i++{
		defer func(){
			fmt.Println(i)
		}()
	}

	这样写在函数体退出的时候才执行 defer 的函数，这时候i的值已经被修改了,derfer的参数都是引用
	闭包的原理
	所以会打印3个3

	用于资源清理，文件关闭，解锁，记录时间

	GO没有异常机制 有 panic/recover ,recover 只能在 defer中调用
	defer 语句中检查
	defer func(){
		err := recover()
		if(err != nil) {
			fmt.Println("Recover")
		}
	}()

	
	
17. 结构

	//结构定义
	type 结构名称 struct {
	
	}

	//结构声明
	a := 结构名{}
	
	操作
	. 

	//简便的初始化方式
	type persion struct{
		Name string
		Age	 int
	}


	a:= persion{
		Name: "andy",
		Age:=25,
	  }
	//注意要有逗号，可以只初始化几个值
	a.Name = "joe"
	a.Age  =18"

	struct 是一种类型，所以数据传递的时候值拷贝而不是引用

	如果要修改，就要使用指针传递,都只用.操作
	func A(per *persion){
		
	}
	
	//对结构的初始话使用指针，而不用特意取地址操作，完全兼容
	a:= &persion{}
	a.Name = "Andy"
	a.Age = 25

	这时候传入函数的就是指针变量

	[匿名结构]
	a:= &struct {
		Name string
		Age int
	}{
		Name:"andy"
		Age int
	}

	// 嵌套的匿名结构
	type persion{
		Name string
		Age int
		Contact struct{
			Phone ,City string
		}
	}

	a:= persion{Name:"andy",Age:16,}
	a.Contact.Phone = "1868017734"
	a.Contact.City	= "Beijing"

	[匿名字段]
	初始化数据的时候可以按顺序初始化值
	a := persion{"Andy",35}
	
	[结构的组合]
	type human struct{
		Sex int
	}
	//human 在这里是一个匿名字段
	//并不是C语言那种结构里面套结构的概念
	type student struct{
		human
		Name string
		Age	int
	}
	a:= student(Name:"Andy",Age:25,.human:human(Sex:0))
	a.Sex
	a.human.Sex //都是正确的写法,有两种方法是为了避免重名变量

	//如果不是匿名字段，就和C语言的嵌套相似
	//就只有一种访问方式了
	type teacher struct{
		basic human
		Name string
		Age int
	}
	a:= student(Name:"Andy",Age:25,.human{Sex:1})
	a.human.Sex
  

18. 方法method
	//为结构写方法，类似于面向对象的类方法
	type persion struct {
			Name string
	}

	func (a persion) Say_hello() {
			fmt.Println(a.Name, ": hello !")
	}
	//方法也支持值传递和指针传递，go 会自动判断是值还是指针
	func (a *persion) SetName(name string) {
		a.Name = name
	}

	b:=persion{"Andy"}
	b.Say_hello()
	b.SetName("Sandy")
	
	[类型方法]
	为int 类型添加类型方法
	方法绑定只能在同一个包中，所以不能为int 型直接绑定
	type TZ int
	func (a *TZ) Print() {
		fmt.Println("TZ")
	}

	var a TZ
	TZ.Print()

	[对比]
	通过类型调用方法
	var a TZ
	a.Print()
	(*TZ).Print(&a)

	[方法访问]
	首字母大写，才可以在用 结构.方法 的形式访问
	但是结构中的小写变量在包中都是可以见的
	结构.小写变量 是可以访问成功的

19. 接口interface
	是多个方法签名接口的集合
	创建一个接口
	type USB interface {
		Name() string
		Connect()
	}

	//创建一个结构 手机
	type Phone struct{
		name string
	}
	
	为这个结构编写接口的方法
	func (ph Phone) Name()string {
		return name
	}

	func (ph Phone) Connect(){
		fmt.Prinln("Connect :",ph.Name())
	}

	//声明一个USB的接口
	var xiaomi USB
	//用结构初始化接口
	xiaomi = Phone{"Red Note2"}
	//调用接口方法
	fmt.Println(xiaomi.Name())
	xiaomi.Connect()


	//创建另外一个结构平板
	type Pad {
		name string
		size int32
	}

	//再为这个结构编写接口方法
	
	//然后就可以向下面这么写
	var xiaomi USB
	var ipad USB

	xiaomi = Phone{"Red Note"}
	ipad   = Pad{"ipad air 3",12}

	xiaomi.Connect()
	ipad.Connect()

	//[1]这时候会发现
	ipad.name
	ipad.size	取不到，只能在初始化的时候从子字面值设置(有没有想C++的私有变量)

	//[2]如果只某个类型的部分接口函数，则编译不通过

	//[3]如果采用如下的方式声明一个结构
		 那么就可以取到结构中的字段
		那他有没有这个接口就不确定
		xiaomi4 := Phone{"xiaomi 4"}
		我们实现的函数就只是方法
		xiaomi4.Connect()

	实现参数为接口的函数
	func DisConnect(usb USB){
		fmt.Println("Disconnectd")
	}

	DisConnect(xiaomi)


	// [嵌入的接口]
	// 将上面的接口定义改为如下，效果是一样的
	type USB interface {
		Name() string
		//Connect()
		Connecter
	}
	type Connecter interface {
		Connect()
	}

	接口的结构的类型判断
	func Disconnect(usb USB) {
	 	if dev,ok := usb.(Phone);ok {
			fmt.Println("Phone Disconnect")
			return 
		}
		fmt.Println("Unknow Dev")
	}

	//重点
	只要你实现了接口的方法,你这个结构就实现了这个接口
	所有的结构都实现了没有任何方法的接口
	type emtpy interface {
	}
	那我们实现一个可以传入任何类型的方法
	//那么我们就可以实现一个函数传入多重类型 多态！！！
	func Disconnect(usb interface{}){
		switch v:=usb.(type) {
		case Phone:
			fmt.Println("Phone Disconnect ",v.name)
		case Pad:
			fmt.Println("Pad Disconnect ",v.name)
		case int32:
			fmt.Println("This is a int32 Data Not Devices")
		default:
			fmt.Println("Unknow Device")
		}
	}

	//接口的转换
	如上嵌入接口中
	USB 可以转换为 Connecter
	xiaomi := Phone("Red Note3")
	var a Connecter
	a = Connecter(xiaomi)
	但是反过来就不行，因为Connecter 没有实现 Name()方法


	//接口使用的注意事项
	对象赋值给接口的时候 发生了拷贝，接口内部获得的是对象拷贝
	xiaomi := Phone("Red Note3")
	var xiaomiusb USB
	xiaomiusb = USB(xiaomi)

	fmt.Println(xiaomiusb.Name())
	xiaomi.name = "Xiaomi 4"
	fmt.Println(xiaomiusb.Name())
	//两次打印的都是 Red Note3

	//只有当接口存储的类型和对象都为nil时,接口才为nil (接口的零值)
	a := interface{}	//空接口 nil
	var p *int =nil   
	a = p				//a存储的类型是 *int,对象是nil ,所以这个接口不为 nil


	
	
20.反射 reflection
	TypeOf() Valueof() 中获取接口目标的信息


