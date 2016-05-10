package main

import (
	"fmt"
	"strings"
	//"unicode/utf8"
)

func main() {
	fmt.Println("strings 中 Contains 函数的用法")
	fmt.Println(strings.Contains("seafoo", "foo")) // true
	fmt.Println(strings.Contains("seafoo", "faa")) // false
	fmt.Println(strings.Contains("seafoo", ""))    // true
	fmt.Println("")
	fmt.Println("ContainsAny 的用法")
	fmt.Println(strings.ContainsAny("team", "i"))   //false
	fmt.Println(strings.ContainsAny("team", "t"))   //true
	fmt.Println(strings.ContainsAny("team", ""))    //false
	fmt.Println(strings.ContainsAny("team", "m&e")) //true
	fmt.Println(strings.ContainsAny("team", "m&f")) //true
	fmt.Println(strings.ContainsAny("team", "d&f")) //false
	fmt.Println("")

	fmt.Println("ContainsRune 的用法")
	fmt.Println(strings.ContainsRune("hello", 'h')) // ture

	fmt.Println("Count 的用法")
	fmt.Println(strings.Count("hello world ", "l")) //3
	fmt.Println(strings.Count("hello world ", ""))  //13
	fmt.Println("")

	fmt.Println("EqualFold 的用法")
	fmt.Println(strings.EqualFold("hello world ", "hello")) //false
	fmt.Println(strings.EqualFold("Go", "go"))              //true
	fmt.Println("")

	fmt.Println("Fields 的用法")
	fmt.Println("Fields are:", strings.Fields(" foo bar baz "))
	//函数作为参数，匿名函数
	for _, record := range []string{"aaa*1892*122", "aaa\taaa\t", "123|456|789"} {
		fmt.Println(strings.FieldsFunc(record, func(ch rune) bool {
			switch {
			case ch > '5':
				return true
			}
			return false
		}))
	}
	fmt.Println("")

	fmt.Println("HasPrefix 函数的用法")
	fmt.Println(strings.HasPrefix("NLT_T_abc", "NLT_"))

	fmt.Println("")
	fmt.Println("HasSuffix函数的用法")
	fmt.Println(strings.HasSuffix("NLT_T_abc", "_abc"))

	fmt.Println("")
	fmt.Println("index 函数的用法")
	fmt.Println(strings.Index("NLT_T_abc", "_abc"))
	fmt.Println(strings.Index("NLT_T_abc", "_l")) //-1

	fmt.Println("")
	fmt.Println("indexAny 函数的用法")
	fmt.Println(strings.IndexAny("NLT_T_abc", "_abc")) //3
	fmt.Println(strings.IndexAny("NLT_T_abc", "l_"))   //	//3
	fmt.Println(strings.IndexAny("NLT_T_abc", "l"))    //	-1

	fmt.Println("")
	fmt.Println("indexRune 函数的用法")
	fmt.Println(strings.IndexRune("NLT_T_abc", '1')) //-1
	fmt.Println(strings.IndexRune("NLT_T_abc", 'a')) //	//6

	fmt.Println("")
	fmt.Println("join 的用法")
	s := []string{"foo", "bar", "bza"}
	fmt.Println(strings.Join(s, "_"))

	//strings.LastIndext	//右边索引
	//strings.LastIndex	//右边索引匹配任意

	fmt.Println("Map 函数的用法")
	rot13 := func(r rune) rune {
		switch {
		case r >= 'A' && r <= 'Z':
			return 'A' + (r-'A'+13)%26
		case r >= 'a' && r <= 'z':
			return 'a' + (r-'a'+13)%26
		}
		return r
	}
	fmt.Println(strings.Map(rot13, "hello world ...."))

	fmt.Println("")
	fmt.Println("Repeat 的用法")
	fmt.Println(strings.Repeat("say three times ", 3))

	fmt.Println("")
	fmt.Println("Replease 的用法")
	fmt.Println(strings.Replace("Hello world, world,world ", "world", "andy", 1))
	fmt.Println(strings.Replace("Hello world, world,world ", "world", "andy", 2))
	fmt.Println(strings.Replace("Hello world, world,world ", "world", "andy", -1))

	fmt.Println("")
	fmt.Println("Split 的用法")
	fmt.Printf("%q\n", strings.Split("Hello World !!!", " "))
	fmt.Printf("%q\n", strings.Split("Hello World !!!", ""))
	fmt.Printf("%q\n", strings.Split("", "Beijing huanyingn ni "))

	fmt.Println("")
	fmt.Println("SplitAfter 的用法")
	fmt.Printf("%q\n", strings.SplitAfter("/home/andy/go/src_test", "/"))

	//还有 SplitN	切分为N短
	// SplitAfterN
	// ToLower 大小写

	fmt.Println(strings.ToTitle("how to title")) //貌似也是大小写

	fmt.Printf("%q\n", strings.Trim("!!! hahah !!! ", "!"))
	fmt.Printf("%q\n", strings.TrimLeft("!!! hahah !!! ", "!"))

	fmt.Println(strings.TrimSpace("\t\n a long story \t\n say it againt \n"))
}
