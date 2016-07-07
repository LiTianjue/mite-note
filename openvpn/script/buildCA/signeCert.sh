#!/bin/bash

# 1 client key
# 2 client pem
# 3 CA key
# 4 CA pem

 openssl ecparam -out $1 -name prime256v1 -genkey
 openssl req -key $1 -new -out EccTMP.req
 openssl x509 -req -in EccTMP.req -CA $4 -CAkey $3 -out $2 -CAcreateserial
 rm *.req
