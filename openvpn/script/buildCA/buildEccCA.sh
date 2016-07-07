#!/bin/bash

# build selfsigned CA
#1 keyname
#2 certname


openssl ecparam -out $1 -name prime256v1 -genkey
openssl req  -key $1 -new -out EccCA.req
openssl x509 -req -in EccCA.req -signkey $1 -out $2
rm EccCA.req
