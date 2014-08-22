/*
 * aes.h
 *
 *  Created on: 2014-8-21
 *      Author: wzb
 */

#ifndef RSA_H_
#define RSA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

char *base64(const unsigned char *a, int length);

unsigned char *debase64(const char *a, int length);

char *rsa_encrypt(const char *clear_text, const char *sk_filename);

char *rsa_decrypt(const char *cipher, const char *pk_filename);

char *rsa_sign(const char *text, const char *sk_filename);

int rsa_verify(const char *text, const char *sig, const char *pk_filename);

#endif