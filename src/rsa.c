/*
 * aes.c
 *
 *  Created on: 2014-8-21
 *      Author: pc
 */

#include "rsa.h"

const char *RSA_SK = "test.key";
const char *RSA_PK = "test_pub.key";

char *base64(const unsigned char *a, int length)
{
	EVP_ENCODE_CTX ectx;
	int size = length * 2;
	size = size > 64 ? size : 64;
	unsigned char* out = NULL;
	int outlen = 0;
	int tlen = 0;

	if (NULL == (out = (unsigned char*) malloc(sizeof(char) * size)))
	{
		printf("error: base64 error, can not encrypt the volume key\n");
		return NULL ;
	}

	EVP_EncodeInit(&ectx);
	EVP_EncodeUpdate(&ectx, out, &outlen, a, length);
	tlen += outlen;
	EVP_EncodeFinal(&ectx, out + tlen, &outlen);
	tlen += outlen;

	return (char *) out;
}

unsigned char *debase64(const char *a, int length)
{
	EVP_ENCODE_CTX ectx;
	unsigned char* out = NULL;
	int outlen = 0;
	int tlen = 0;

	if (NULL == (out = (unsigned char*) malloc(sizeof(char) * length)))
	{
		printf("error:debase64 error, can not decrypt the volume key\n");
		return NULL ;
	}

	EVP_DecodeInit(&ectx);
	EVP_DecodeUpdate(&ectx, out, &outlen, (const unsigned char*) a, length);
	tlen += outlen;
	EVP_DecodeFinal(&ectx, out + tlen, &outlen);
	tlen += outlen;
	out[tlen] = 0;
	return out;
}

char *rsa_encrypt(const char *plain_text, const char *pk_filename)
{
	unsigned char *cipher;
	int len;
	RSA *rsa;
	FILE *file;

	if (NULL == (file = fopen(pk_filename, "rb")))
	{
		printf("error:open key file error\n");
		return NULL ;
	}
	if (NULL == (rsa = PEM_read_RSA_PUBKEY(file, NULL, NULL, NULL )))
	{
		ERR_print_errors_fp(stdout);
		return NULL ;
	}
	fclose(file);

	len = RSA_size(rsa);
	if (NULL == (cipher = (unsigned char *) malloc(len + 1)))
	{
		RSA_free(rsa);
		return NULL ;
	}
	memset(cipher, 0, len + 1);

	if (0
			> RSA_public_encrypt(len, (unsigned char *) plain_text,
					(unsigned char*) cipher, rsa, RSA_NO_PADDING))
	{
		RSA_free(rsa);
		free(cipher);
		return NULL ;
	}

	RSA_free(rsa);
	char *result = base64(cipher, strlen((char *) cipher));
	free(cipher);
	return result;
}

char *rsa_decrypt(const char *cipher, const char *sk_filename)
{
	unsigned char *plain_text;
	FILE *file = NULL;
	RSA *rsa;
	int len;

	if (NULL == (file = fopen(sk_filename, "rb")))
	{
		printf("error:open key file error\n");
		return NULL ;
	}
	if ((rsa = PEM_read_RSAPrivateKey(file, NULL, NULL, NULL )) == NULL )
	{
		ERR_print_errors_fp(stdout);
		return NULL ;
	}
	fclose(file);

	len = RSA_size(rsa);
	if (NULL == (plain_text = (unsigned char *) malloc(len + 1)))
	{
		RSA_free(rsa);
		return NULL;
	}
	memset(plain_text, 0, len + 1);

	char *temp = (char *) debase64(cipher, strlen(cipher));
	if (0
			> RSA_private_decrypt(len, (unsigned char *) temp,
					(unsigned char*) plain_text, rsa, RSA_NO_PADDING))
	{
		RSA_free(rsa);
		free(temp);
		return NULL ;
	}

	RSA_free(rsa);
	free(temp);
	return (char *) plain_text;
}

char *rsa_sign(const char *text, const char *sk_filename)
{
	RSA *rsa;
	FILE *file;
	unsigned char *sig;
	unsigned int sig_len;

	if (NULL == (file = fopen(sk_filename, "rb")))
	{
		printf("error:open key file error\n");
		return NULL ;
	}
	if ((rsa = PEM_read_RSAPrivateKey(file, NULL, NULL, NULL )) == NULL )
	{
		ERR_print_errors_fp(stdout);
		return NULL ;
	}
	fclose(file);

	if (NULL == (sig = (unsigned char*) malloc(RSA_size(rsa))))
	{
		RSA_free(rsa);
		return NULL ;
	}

	unsigned char temp[50];
	SHA((const unsigned char *) text, strlen(text), temp);
	if (1 != RSA_sign(NID_sha1, temp, 16, sig, &sig_len, rsa))
	{
		printf("error:fail to sign the message!\n");
		free(sig);
		RSA_free(rsa);
		return NULL ;
	}
	char *buf = base64(sig, strlen((char *) sig));
	free(sig);
	RSA_free(rsa);
	return buf;
}

int rsa_verify(const char *text, const char *sig, const char *pk_filename)
{
	unsigned char *sig_temp;
	RSA *rsa;
	FILE *file;

	if (NULL == (file = fopen(pk_filename, "rb")))
	{
		printf("error:open key file error\n");
		return -1;
	}
	if ((rsa = PEM_read_RSA_PUBKEY(file, NULL, NULL, NULL )) == NULL )
	{
		ERR_print_errors_fp(stdout);
		return -1;
	}
	fclose(file);

	sig_temp = debase64(sig, strlen((char *) sig));
	unsigned char temp[50];
	SHA((const unsigned char *) text, strlen(text), temp);
	int ret = RSA_verify(NID_sha1, temp, 16, sig_temp, 128, rsa);
	free(sig_temp);
	RSA_free(rsa);

	return (ret == 1) ? 0 : -1;
}
