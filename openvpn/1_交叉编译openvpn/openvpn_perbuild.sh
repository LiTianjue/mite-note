#!/bin/bash
./configure	\
	--host=arm-none-linux-gnueabi \
	--enable-password-save \
	CC=arm-none-linux-gnueabi-gcc \
	--target=arm-none-linux-gnueabi \
	LZO_CFLAGS="-I/root/WORK/kedaVPN/arm-vpn/lib/lzo-arm-lib/include/" \
	LZO_LIBS="-L/root/WORK/kedaVPN/arm-vpn/lib/lzo-arm-lib/lib -llzo2" \
	--enable-small \
	--disable-plugins \
	--disable-debug	\
	OPENSSL_CRYPTO_CFLAGS="-I/root/WORK/kedaVPN/arm-vpn/lib/arm-openssl-lib/include" \
	OPENSSL_CRYPTO_LIBS="-L/root/WORK/kedaVPN/arm-vpn/lib/arm-openssl-lib/lib -lcrypto" \
	OPENSSL_SSL_CFLAGS="-I/root/WORK/kedaVPN/arm-vpn/lib/arm-openssl-lib/include" \
	OPENSSL_SSL_LIBS="-L/root/WORK/kedaVPN/arm-vpn/lib/arm-openssl-lib/lib -lssl" \
	
