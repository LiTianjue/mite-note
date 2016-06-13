ssl.h  主要数据结构
SSL_METHOD  里面是一大堆方法函数的集合，包括创建SSL, 加解密等等，和协议版本有直接关系
SSL_CTX              握手前的环境准备，设置CA，证书，私钥，协议版本，其他握手选项，缓存了SESSION
SSL                          用于握手及传送应用数据
SSL_SESSION    用于保存连接信息，主密钥，session id, 读写加解密密钥，读写MAC密钥等信息  

加密套件
由服务器选择
openssl 的加密套件在s3_lib.c 的 ssl3_ciphers数组中定义

设置自己支持的算法套件
int SSL_set_cipher_list(SSL *s,const char *str);
int SSL_CTX_set_cipher_list(SSL_CTX *ctx,const char *str);

只设置一种加密套件
int ret = SSL_set_cipher_list(ssl,"RC4+MD5");

密钥信息
1.预主密钥
    由客户端生成，采用服务端的公钥加密发送给服务端
    预主密钥的生成在 s3_clnt.c 中的
    ssl3_send_client_key_exchange 函数中

2.主密钥
    更具预主密钥，客户端随机数和服务端随机数来生成，是相同的
    存放在SSL_SESSION结构中
        ssl    :  ssl3_genrate_master_secret 
        tls     :  tls1_generate_master_secret

3.对称密钥和MAC密钥
    对称密钥（IV）和MAC读写密钥通过主密钥，客户端随机数，服务端随机数来生成
    在sslv3源码中，
    ssl3_generate_eky_block 中生成
    ssl3_change_cipher_state 中分配


SESSION
    服务端生成一个session ID (一个随机数)， 通过哈希表缓存
    通过server hello 消息发送给客户端
    SSL v2 版本是16字节
    TLSv1   是 32字节，与安全无关，但对于服务器来说必须是唯一的

    session 重用，客户端发送包含session id 的 cllientHello 消息给服务端
    session 重用 可以减少握手的开销，session 的默认超时时间为60*5+4 秒，5分钟

    相关函数
    1.查询哈希表中是否有匹配的session id
    int SSL_has_matching_session()
    2.生成一个新到session id，服务端和客户端都可以调用,客户端生成长度为0 的session id
        ssl_get_new_session(),
    3.获取以前用过的session id,用于服务端重用，服务端调用
        ssl_get_prev_session()
    4.设置session,客户端调用
        ssl_set_session
    5. 清除超时的session
        void SSL_CTX_flush_session()
    6.清除无效的session
        int ssl_clear_bad_session()
        