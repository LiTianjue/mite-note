客户端
openssl s_client
服务端
openssl s_server

常用选项
指定文件
-cert            指定证书
-key            指定私钥
-CAfile        指定CA文件

指定证书文件的格式，默认是pem
-keyform  (PEM/DER)

指定协议
-ssl3
-ssl2
-tls1_1
-tls1

指定ssl连接的端口
-connect host:port          (默认是localhost:4433)

验证客户端证书 
-Verify 1


服务端
-accept         指定监听端口，默认是4433


指定加密套件(套件通过 openssl ciphers 命令获取)
-cipher AES256-SHA

指定私钥加密口令
-pass pass:123456
-pass file:password.txt
-pass env:envname VPNPASS
-pass fd:fdname;
-pass stdin

