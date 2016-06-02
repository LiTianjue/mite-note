如何用openssl 的C API 解析x509 证书信息

1.获取证书
    1.从 SSL 或者 TLS 回话中获取证书

```c
#include <openssl/x509.h>
#include <openssl/x509v3.h>

X509 *cert = SSL_get_peer_certificate(ssl);
STACK_OF(X509) *sk = SSL_get_peer_cert_chain(ssl);
```

```c
X509 *cert = SSL_get_peer_certificate(ssl);
STACK_OF(X509) *sk = sk_X509_new_null();
sk_X509_push(sk, cert);
```

2.从磁盘读取证书
    对于PEM的证书
```c
#include <stdio.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>

FILE *fp = fopen(path, "r");
if (!fp) {
    fprintf(stderr, "unable to open: %s\n", path);
    return EXIT_FAILURE;
}

X509 *cert = PEM_read_X509(fp, NULL, NULL, NULL);
if (!cert) {
    fprintf(stderr, "unable to parse certificate in: %s\n", path);
    fclose(fp);
    return EXIT_FAILURE;
}

// any additional processing would go here..

X509_free(cert);
fclose(fp);
```
对于DER格式的证书
```c
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/bio.h>

const unsigned char *data = ... ; 
size_t len = ... ;

X509 *cert = d2i_X509(NULL, &data, len);
if (!cert) {
    fprintf(stderr, "unable to parse certificate in memory\n");
    return EXIT_FAILURE;
}

// any additional processing would go here..

X509_free(cert);
```

3.内存中的证书
内存中的PEM证书
```c
char* pemCertString = ..... (includes "-----BEGIN/END CERTIFICATE-----")
size_t certLen = strlen(pemCertString);

BIO* certBio = BIO_new(BIO_s_mem());
BIO_write(certBio, pemCertString, certLen);
X509* certX509 = PEM_read_bio_X509(certBio, NULL, NULL, NULL);
if (!certX509) {
    fprintf(stderr, "unable to parse certificate in memory\n");
    return EXIT_FAILURE;
}

// do stuff

BIO_free(certBio);
X509_free(certX509);
```

3.其他地方的证书
    比如usbkey中的，可以先读取到内存或者文件再做处理



2. 解析证书
在下面每个小块中不再对头文件分别说明，一般你都要包含以下内容，编译的时候加上-ssl
```c
#include <openssl/x509v3.h>
#include <openssl/bn.h>
#include <openssl/asn1.h>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>
#include <openssl/pem.h>
#include <openssl/bio.h>

OpenSSL_add_all_algorithms();
```

2.1 主题和证书发行者
直接解析为文本
```c
char *subj = X509_NAME_oneline(X509_get_subject_name(cert), NULL, 0);
char *issuer = X509_NAME_oneline(X509_get_issuer_name(cert), NULL, 0);
```
显示效果应该是
```c
/C=US/ST=California/L=Mountain View/O=Google Inc/CN=*.google.com
```
但是你可能想要的是
```c
C=US, ST=Texas, L=Austin, O=Polycom Inc., OU=Video Division, CN=a.digitalnetbr.net
```
你可以自己写代码解析一下字符串
当然openssl也提供了接口解析subject中的内容
这样
```c
X509_NAME *subj = X509_get_subject_name(cert);

for (int i = 0; i < X509_NAME_entry_count(subj); i++) {
    X509_NAME_ENTRY *e = X509_NAME_get_entry(subj, i);
    ASN1_STRING *d = X509_NAME_ENTRY_get_data(e);
    char *str = ASN1_STRING_data(d);    
}
```
或者这样
```c
for (;;) {
    int lastpos = X509_NAME_get_index_by_NID(subj, NID_commonName, lastpos);
    if (lastpos == -1)
        break;
    X509_NAME_ENTRY *e = X509_NAME_get_entry(subj, lastpos);
    /* Do something with e */
}
```

2.2 解析指纹信息(是翻译为指纹信息吗？)
Cryptographic (e.g. SHA-1) Fingerprint
```c
#define SHA1LEN 20
char buf[SHA1LEN];

const EVP_MD *digest = EVP_sha1();
unsigned len;

int rc = X509_digest(cert, digest, (unsigned char*) buf, &len);
if (rc == 0 || len != SHA1LEN) {
    return EXIT_FAILURE;
}
return EXIT_SUCCESS;
```
当然除了sha1还可以是其他算法的
解析出来的数据是二进制的，要答打印成可读的，就16进制转换一下

2.3 解析证书序列号
```c
#define SERIAL_NUM_LEN 1000;
char serial_number[SERIAL_NUM_LEN+1];

ASN1_INTEGER *serial = X509_get_serialNumber(cert);

BIGNUM *bn = ASN1_INTEGER_to_BN(serial, NULL);
if (!bn) {
    fprintf(stderr, "unable to convert ASN1INTEGER to BN\n");
    return EXIT_FAILURE;
}

char *tmp = BN_bn2dec(bn);
if (!tmp) {
    fprintf(stderr, "unable to convert BN to decimal string.\n");
    BN_free(bn);
    return EXIT_FAILURE;
}

if (strlen(tmp) >= len) {
    fprintf(stderr, "buffer length shorter than serial number\n");
    BN_free(bn);
    OPENSSL_free(tmp);
    return EXIT_FAILURE;
}

strncpy(buf, tmp, len);
BN_free(bn);
OPENSSL_free(tmp);
```
或者
```c
ASN1_INTEGER *Serial = NULL;
Serial = X509_get_serialNumber(cert);
int i = 0;
char sn[SERIAL_NUM_LEN] = {0};
for(i = 0 ;i < Serial->length;i++)
{
        sprintf(sn+2*i,"%02x",Serial->data[i]);
}
```

2.4解析版本号
```c
int version = ((int) X509_get_version(cert)) + 1;
```

2.5 解析签名算法
通过NID
```c
int pkey_nid = OBJ_obj2nid(cert->cert_info->key->algor->algorithm);

if (pkey_nid == NID_undef) {
    fprintf(stderr, "unable to find specified signature algorithm name.\n");
    return EXIT_FAILURE;
}
```
转换成人话
```c
char sigalgo_name[SIG_ALGO_LEN+1];
const char* sslbuf = OBJ_nid2ln(pkey_nid);

if (strlen(sslbuf) > PUBKEY_ALGO_LEN) {
    fprintf(stderr, "public key algorithm name longer than allocated buffer.\n");
    return EXIT_FAILURE;
}

strncpy(buf, sslbuf, PUBKEY_ALGO_LEN);
```
返回的字符串会是 sha1WithRSAEncryption 或者  md5WithRSAEncryption.

2.6 公钥信息
证书的公钥信息是类型相关的，这里给出怎么解析RSA和DSA 的 密钥
```c
char pubkey_algoname[PUBKEY_ALGO_LEN];

int pubkey_algonid = OBJ_obj2nid(cert->cert_info->key->algor->algorithm);

if (pubkey_algonid == NID_undef) {
    fprintf(stderr, "unable to find specified public key algorithm name.\n");
    return EXIT_FAILURE;
}

const char* sslbuf = OBJ_nid2ln(pubkey_algonid);
assert(strlen(sslbuf) < PUBKEY_ALGO_LEN);
strncpy(buf, sslbuf, PUBKEY_ALGO_LEN);

if (pubkey_algonid == NID_rsaEncryption || pubkey_algonid == NID_dsa) {
    
    EVP_PKEY *pkey = X509_get_pubkey(cert);
    IFNULL_FAIL(pkey, "unable to extract public key from certificate");
    
    RSA *rsa_key;
    DSA *dsa_key;
    char *rsa_e_dec, *rsa_n_hex, *dsa_p_hex, *dsa_p_hex,
             *dsa_q_hex, *dsa_g_hex, *dsa_y_hex;
    
    switch(pubkey_algonid) {
        
        case NID_rsaEncryption:
        
            rsa_key = pkey->pkey.rsa;
            IFNULL_FAIL(rsa_key, "unable to extract RSA public key");
            
            rsa_e_dec = BN_bn2dec(rsa_key->e);
            IFNULL_FAIL(rsa_e_dec,  "unable to extract rsa exponent");
            
            rsa_n_hex = BN_bn2hex(rsa_key->n);
            IFNULL_FAIL(rsa_n_hex,  "unable to extract rsa modulus");
            
            break;
            
        case NID_dsa:
        
            dsa_key = pkey->pkey.dsa;
            IFNULL_FAIL(dsa_key, "unable to extract DSA pkey");
            
            dsa_p_hex = BN_bn2hex(dsa_key->p);
            IFNULL_FAIL(dsa_p_hex, "unable to extract DSA p");
            
            dsa_q_hex = BN_bn2hex(dsa_key->q);
            IFNULL_FAIL(dsa_q_hex, "unable to extract DSA q");
            
            dsa_g_hex = BN_bn2hex(dsa_key->g);
            IFNULL_FAIL(dsa_g_hex, "unable to extract DSA g");
            
            dsa_y_hex = BN_bn2hex(dsa_key->pub_key);
            IFNULL_FAIL(dsa_y_hex, "unable to extract DSA y");
            
            break;
            
        default:
            break;
    }
    
    EVP_PKEY_free(pkey);
}
```

2.7 解析有效期(Validity Period)
```c
//ASN1_TIME  object
ASN1_TIME *not_before = X509_get_notBefore(cert);
ASN1_TIME *not_after = X509_get_notAfter(cert);
```
我们 也需要说人话
```c
#define DATE_LEN 128

int convert_ASN1TIME(ASN1_TIME *t, char* buf, size_t len)
{
    int rc;
    BIO *b = BIO_new(BIO_s_mem());
    rc = ASN1_TIME_print(b, t);
    if (rc <= 0) {
        log_error("fetchdaemon", "ASN1_TIME_print failed or wrote no data.\n");
        BIO_free(b);
        return EXIT_FAILURE;
    }
    rc = BIO_gets(b, buf, len);
    if (rc <= 0) {
        log_error("fetchdaemon", "BIO_gets call failed to transfer contents to buf");
        BIO_free(b);
        return EXIT_FAILURE;
    }
    BIO_free(b);
    return EXIT_SUCCESS;
}

char not_after_str[DATE_LEN];
convert_ASN1TIME(not_after, not_after_str, DATE_LEN);

char not_before_str[DATE_LEN];
convert_ASN1TIME(not_before, not_before_str, DATE_LEN);
```

2.8 查看是不是CA证书
```c
int raw = X509_check_ca(cert);
```
如果raw >=1 就是CA证书，如果0 或者其他就不是CA证书

2.9 其他的x509 扩展信息
通过下面的代码可以获取和打印扩展信息
```c
STACK_OF(X509_EXTENSION) *exts = cert->cert_info->extensions;

int num_of_exts;
if (exts) {       
    num_of_exts = sk_X509_EXTENSION_num(exts);
} else {
    num_of_exts = 0
}

IFNEG_FAIL(num_of_exts, "error parsing number of X509v3 extensions.");

for (int i=0; i < num_of_exts; i++) {

    X509_EXTENSION *ex = sk_X509_EXTENSION_value(exts, i);
    IFNULL_FAIL(ex, "unable to extract extension from stack");
    ASN1_OBJECT *obj = X509_EXTENSION_get_object(ex);
    IFNULL_FAIL(obj, "unable to extract ASN1 object from extension");

    BIO *ext_bio = BIO_new(BIO_s_mem());
    IFNULL_FAIL(ext_bio, "unable to allocate memory for extension value BIO");
    if (!X509V3_EXT_print(ext_bio, ex, 0, 0)) {
        M_ASN1_OCTET_STRING_print(ext_bio, ex->value);
    }

    BUF_MEM *bptr;
    BIO_get_mem_ptr(ext_bio, &bptr);
    BIO_set_close(ext_bio, BIO_NOCLOSE);

    // remove newlines
    int lastchar = bptr->length;
    if (lastchar > 1 && (bptr->data[lastchar-1] == '\n' || bptr->data[lastchar-1] == '\r')) {
        bptr->data[lastchar-1] = (char) 0;
    }
    if (lastchar > 0 && (bptr->data[lastchar] == '\n' || bptr->data[lastchar] == '\r')) {
        bptr->data[lastchar] = (char) 0;
    }

    BIO_free(ext_bio);

    unsigned nid = OBJ_obj2nid(obj);    
    if (nid == NID_undef) {
        // no lookup found for the provided OID so nid came back as undefined.
        char extname[EXTNAME_LEN];
        OBJ_obj2txt(extname, EXTNAME_LEN, (const ASN1_OBJECT *) obj, 1);
        printf("extension name is %s\n", extname);
    } else {
        // the OID translated to a NID which implies that the OID has a known sn/ln
        const char *c_ext_name = OBJ_nid2ln(nid);
        IFNULL_FAIL(c_ext_name, "invalid X509v3 extension name");
        printf("extension name is %s\n", c_ext_name);
    }
    
    printf("extension length is %u\n", bptr->length)
    printf("extension value is %s\n", bptr->data)
}
```


3 乱序的证书链排序
我们一些时候会收到乱序的证书链，我们就基于subject和issuer将他们排序为合理的证书链
这个排序的时间复杂度为O(n^2)，但是实际中，证书链只会有2级或者3级
```c
STACK_OF(X509) *r_sk = sk_X509_new_null();
    sk_X509_push(r_sk, sk_X509_value(st, 0));
    
    for (int i=1; i < sk_X509_num(st); i++) {
        X509 *prev = sk_X509_value(r_sk, i-1);
        X509 *next = NULL;
        for (int j=1; j < sk_X509_num(st); j++) {
            X509 *cand = sk_X509_value(st, j);
            if (!X509_NAME_cmp(cand->cert_info->subject, prev->cert_info->issuer)
                    || j == sk_X509_num(st) - 1) {
                next = cand;
                break;
            }
        }
        if (next) {
            sk_X509_push(r_sk, next);
        } else {
            // we're unable to figure out the correct stack so just use the original one provided.
            sk_X509_free(r_sk);
            r_sk = sk_X509_dup(st);
            break;
        }
}
```

4  证书验证
创建一个基于文件的证书store
```c
X509_STORE *s = X509_STORE_new();
if (s == NULL) {
    fprintf(stderr, "unable to create new X509 store.\n");
    return NULL;
}
int rc = X509_STORE_load_locations(s, store_path, NULL);
if (rc != 1) {
    fprintf(stderr, "unable to load certificates at %s to store\n", store_path);
    X509_STORE_free(s);
    return NULL;
}
return s;
```
验证证书
```c
X509_STORE_CTX *ctx = X509_STORE_CTX_new();
if (!ctx) {
    fprintf(stderr, "unable to create STORE CTX\n");
    return -1;
}
if (X509_STORE_CTX_init(ctx, store, cert, st) != 1) {
    fprintf(stderr, "unable to initialize STORE CTX.\n");
    X509_STORE_CTX_free(ctx);
    return -1;
}
int rc = X509_verify_cert(ctx);
X509_STORE_CTX_free(ctx);
return rc;
```
上面的代码不能验证自签名的证书
要验证自签名证书
```c
X509_STORE *s = X509_STORE_new();
int num = sk_X509_num(sk);
X509 *top = sk_X509_value(st, num-1);
X509_STORE_add_cert(s, top);
X509_STORE_CTX *ctx = X509_STORE_CTX_new();
X509_STORE_CTX_init(ctx, s, cert, st);
int rc = X509_verify_cert(ctx);
if (rc == 1) {
    // validated OK. either trusted or self signed.
} else {
    // validation failed
    int err = X509_STORE_CTX_get_error(ctx);
}

// any additional processing..

X509_STORE_CTX_free(ctx);
X509_STORE_free(s);
```
或者干脆
```c
if (X509_check_issued(cert, cert) == X509_V_OK) {
    is_self_signed = 1;
} else {
    is_self_signed = 0;
}
```


5.一些有用的函数
可以直接Copy了去用
打印证书的基本信息
```c
#define MAX_LENGTH 1024
void print_certificate(X509* cert) {    
    char subj[MAX_LENGTH+1];
    char issuer[MAX_LENGTH+1];
    X509_NAME_oneline(X509_get_subject_name(cert), subj, MAX_LENGTH);
    X509_NAME_oneline(X509_get_issuer_name(cert), issuer, MAX_LENGTH);
    printf("certificate: %s\n", subj);
    printf("\tissuer: %s\n\n", issuer);
}
```
打印STACK函数
```c
void print_stack(STACK_OF(X509)* sk)
{
    unsigned len = sk_num(sk);
    unsigned i;
    X509 *cert;
    printf("Begin Certificate Stack:\n");
    for(i=0; i<len; i++) {
        cert = (X509*) sk_value(sk, i);
        print_certificate(cert);
    }
    printf("End Certificate Stack\n");
}
```
检查两个证书的stack 是否一致
```c
int certparse_sk_X509_cmp(STACK_OF(X509) *a, STACK_OF(X509) *b)
{
    int a_len = sk_X509_num(a); 
    int b_len = sk_X509_num(b);
    if (a_len != b_len) {
        return 1;
    }
    for (int i=0; i < a_len; i++) {
        if (X509_cmp(sk_X509_value(a, i), sk_X509_value(b, i))) {
            return 1;
        }   
    }
    return 0;
}
```
将OPENSLL的错误宏转换成人话
```c
const char* get_validation_errstr(long e) {
    switch ((int) e) {
        case X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT:
            return "ERR_UNABLE_TO_GET_ISSUER_CERT";
        case X509_V_ERR_UNABLE_TO_GET_CRL:
            return "ERR_UNABLE_TO_GET_CRL";
        case X509_V_ERR_UNABLE_TO_DECRYPT_CERT_SIGNATURE:
            return "ERR_UNABLE_TO_DECRYPT_CERT_SIGNATURE";
        case X509_V_ERR_UNABLE_TO_DECRYPT_CRL_SIGNATURE:
            return "ERR_UNABLE_TO_DECRYPT_CRL_SIGNATURE";
        case X509_V_ERR_UNABLE_TO_DECODE_ISSUER_PUBLIC_KEY:
            return "ERR_UNABLE_TO_DECODE_ISSUER_PUBLIC_KEY";
        case X509_V_ERR_CERT_SIGNATURE_FAILURE:
            return "ERR_CERT_SIGNATURE_FAILURE";
        case X509_V_ERR_CRL_SIGNATURE_FAILURE:
            return "ERR_CRL_SIGNATURE_FAILURE";
        case X509_V_ERR_CERT_NOT_YET_VALID:
            return "ERR_CERT_NOT_YET_VALID";
        case X509_V_ERR_CERT_HAS_EXPIRED:
            return "ERR_CERT_HAS_EXPIRED";
        case X509_V_ERR_CRL_NOT_YET_VALID:
            return "ERR_CRL_NOT_YET_VALID";
        case X509_V_ERR_CRL_HAS_EXPIRED:
            return "ERR_CRL_HAS_EXPIRED";
        case X509_V_ERR_ERROR_IN_CERT_NOT_BEFORE_FIELD:
            return "ERR_ERROR_IN_CERT_NOT_BEFORE_FIELD";
        case X509_V_ERR_ERROR_IN_CERT_NOT_AFTER_FIELD:
            return "ERR_ERROR_IN_CERT_NOT_AFTER_FIELD";
        case X509_V_ERR_ERROR_IN_CRL_LAST_UPDATE_FIELD:
            return "ERR_ERROR_IN_CRL_LAST_UPDATE_FIELD";
        case X509_V_ERR_ERROR_IN_CRL_NEXT_UPDATE_FIELD:
            return "ERR_ERROR_IN_CRL_NEXT_UPDATE_FIELD";
        case X509_V_ERR_OUT_OF_MEM:
            return "ERR_OUT_OF_MEM";
        case X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT:
            return "ERR_DEPTH_ZERO_SELF_SIGNED_CERT";
        case X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN:
            return "ERR_SELF_SIGNED_CERT_IN_CHAIN";
        case X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY:
            return "ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY";
        case X509_V_ERR_UNABLE_TO_VERIFY_LEAF_SIGNATURE:
            return "ERR_UNABLE_TO_VERIFY_LEAF_SIGNATURE";
        case X509_V_ERR_CERT_CHAIN_TOO_LONG:
            return "ERR_CERT_CHAIN_TOO_LONG";
        case X509_V_ERR_CERT_REVOKED:
            return "ERR_CERT_REVOKED";
        case X509_V_ERR_INVALID_CA:
            return "ERR_INVALID_CA";
        case X509_V_ERR_PATH_LENGTH_EXCEEDED:
            return "ERR_PATH_LENGTH_EXCEEDED";
        case X509_V_ERR_INVALID_PURPOSE:
            return "ERR_INVALID_PURPOSE";
        case X509_V_ERR_CERT_UNTRUSTED:
            return "ERR_CERT_UNTRUSTED";
        case X509_V_ERR_CERT_REJECTED:
            return "ERR_CERT_REJECTED";
        case X509_V_ERR_SUBJECT_ISSUER_MISMATCH:
            return "ERR_SUBJECT_ISSUER_MISMATCH";
        case X509_V_ERR_AKID_SKID_MISMATCH:
            return "ERR_AKID_SKID_MISMATCH";
        case X509_V_ERR_AKID_ISSUER_SERIAL_MISMATCH:
            return "ERR_AKID_ISSUER_SERIAL_MISMATCH";
        case X509_V_ERR_KEYUSAGE_NO_CERTSIGN:
            return "ERR_KEYUSAGE_NO_CERTSIGN";
        case X509_V_ERR_INVALID_EXTENSION:
            return "ERR_INVALID_EXTENSION";
        case X509_V_ERR_INVALID_POLICY_EXTENSION:
            return "ERR_INVALID_POLICY_EXTENSION";
        case X509_V_ERR_NO_EXPLICIT_POLICY:
            return "ERR_NO_EXPLICIT_POLICY";
        case X509_V_ERR_APPLICATION_VERIFICATION:
            return "ERR_APPLICATION_VERIFICATION";
        default:
            return "ERR_UNKNOWN";
    }
}
```





