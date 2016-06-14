1. 算法套件
2.压缩算法
3.其他协议


// [1]  首先要显式的加载库和算法

//[2] 定义SSL_METHOD 结构
SSL_METHOD  *meth = NULL	//用于hold 各种协议的ssl方法，ssl_accept,，ssl_read 等等
//[3] 初始化meth
meth = TLSv1_1_method();
/*
 我们并没有找到TLSv1_1_method 这个函数的定义，
 我们找到文件 ssl/t1_meth.c
 看到 IMPLEMENT tls_meth_func 宏，尝试把这个宏展开
所以其实这个函数是有定义的
//  注释含有猜测的陈分，如果错了不怪
  const SSL_METHOD *TLSv1_1_method(void) {
        static const SSL_METHOD TLSv1_1_method_data={
            0x0302,		//协议版本号	TLSv1.1
            tls1_new,	// new 一个 SSL
            tls1_clear,	// 清除	SSL
            tls1_free,	// 释放
            ssl3_accept,	// accept
            ssl3_connect,	// 连接
            ssl3_read,	// 读
            ssl3_peek,	// 试读
            ssl3_write,	// 写
            ssl3_shutdown,	// 关闭连接
            ssl3_renegotiate,	//重新协商
            ssl3_renegotiate_check,	//重新协商检查
            ssl3_get_message,		// 获得message
            ssl3_read_bytes,		// 读 bytes
            ssl3_write_bytes,		// 写 bytes
            ssl3_dispatch_alert,		// 报警
            ssl3_ctrl,				//控制？
            ssl3_ctx_ctrl,			// ctx 控制？
            ssl3_get_cipher_by_char,	// 获取加密套件
            ssl3_put_cipher_by_char,	// 设置加密套件
            ssl3_pending,			// pending
            ssl3_num_ciphers,		// 加密套件个数
            ssl3_get_cipher,		// 获取加密套件
            tls1_get_method,		//获取method ，获取自己？
            tls1_default_timeout,		// 默认超时时间
            &TLSv1_1_enc_data,		// 一系列的加密运算
            ssl_undefined_void_function,	// 未定义的函数？
            ssl3_callback_ctrl,		// 回调个啥？
            ssl3_ctx_callback_ctrl,		//回调个啥？
        };
        return &TLSv1_1_method_data;
    }


    这里面的主要函数实现在t1_lib里面
    就比如	tsv1_1 的method  中有 tls1_new 是一个复用的函数，在进去发现其实是 ssl3_new(s) 
    openssl 这样设计虽然阅读起来很累，但是有效的提高了代码的复用

   
   再看t1_meth.c 中的 这个宏
    IMPLEMENT_tls_meth_func(TLS1_1_VERSION, TLSv1_1_method,
                         ssl3_accept,
                         ssl3_connect, tls1_get_method, TLSv1_1_enc_data)

     通过宏修改掉的内容包括
     TLS1_1_VERSION		协议版本号
     TLSv1_1_method 		函数名 
     ssl3_accept			accept函数，直接使用ssl3_accept;
     ssl3_connect		connect 方法
     tls1_get_method		协议选择函数
     TLSv1_1_enc_data		定义在t1_lib.c 中，加密实现
     				以后也是我们主要关注的，可以看到里面有
     				加密，mac ,主密钥的生成，协议套件的转换，错误报警码



*/

   // 然后是通过这个meth 函数合集去创建一个CTX
    SSL _CTX 	*s_ctx = NULL;
    s_ctx =  SSL_CTX_new(meth);

    //设置算法套件
    static char *cipher = NULL;		//通过字符串的组合就能设置算法套件
    SSL_CTX_set_cipher_list(s_ctx,cipher);
    //两种设置接口
    //int SSL_set_cipher_list(SSL *s,const char *str)；
    //int SSL_set_cipher_list(SSL *s,const char *str)；

    // 4中连接字符串的方式 + (增加)  -(删除)  | (永久删除) @ (排序)，多个描述可以用：，; 来分开
    /*ALL:!ADH:RC4+RSA:+SSLv2:@STRENGTH
    表示的意义是：首先选择所有的加密套件（不包含eNULL，即空对称加密算法），然后在得到的双向链表之中去掉身份验证采用DH的加密套件；加入包含RC4算法并将包含RSA的加密套件放在双向链表的尾部；再将支持SSLV2的加密套件放在尾部；最后得到的结果按照安全强度进行排序。
*/

    /*
	算法套件在并没有定义在 t1_lib.c 中而是定义在 s3_lib.c 中
	可能是因为 tls 只是更新了协议，和算法套件没关系
    */

// 一些callback函数，暂时不知道干嘛用

//设置证书，私钥
// 服务器是一定要有证书的，
// 客户端验证是可选的

//载入CA
// 设置客户端认证的回调函数
// 

//创建SSL 回话
SSL *s_ssl;
s_ssl = SSL_new(c_ctx);

//设置session 
SSL_set_session(s_ssl,NULL);

SSL_set_fd(s_ssl,client_fd);

//对于服务器来讲
//客户端连接成功后第一个调用的就是
SSL_accept(s_ssl);
//看看这里面做什么什么
// s->method->ssl_accept(s);
//明显是调用了meth 里面的ssl_accept
// 源码 s32_srvr.c
//首先产生一个4字节的时间戳
// unsigned long TIME = time(NULL)
//然后开始一个状态机循环（握手过程）
/*
 for(;;)
{
        switch{
            握手前的准备：
                分配一块buff
                buf = BUF_MEM_new()
                然后让这块buf增大到 16384的大小
                BUF_MEM_grow(buf,SSL3_RT_MAX_PLAIN_LENGTH)
                然后清除ssl中原有的数据块，,跳出循环，更新状态机转到HELLOA
            HELLOA:
                获取客户端的hello 数据
                检查协议版本号
                查看需要什么版本的加密套件去适配客户端版本
            HELLOB:
                取出加密套件，压缩算法，随机数
                取出session ID， 看有没有重用
                没有new　一个session
                选择一个算法套件
            SSL3_ST_SW_SRVR_HELLO_A:
                
            

        }
}
*/