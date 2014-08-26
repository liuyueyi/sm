/*
 * aes.c
 *
 *  Created on: 2014-8-21
 *      Author: pc
 */

#include "rsa.h"

const char *RSA_SK = "test.key";
const char *RSA_PK = "test_pub.key";

char *base64(const char *input, int length)
{
	BIO * bmem = NULL;
	BIO * b64 = NULL;
	BUF_MEM * bptr = NULL;

	b64 = BIO_new(BIO_f_base64());
	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
	bmem = BIO_new(BIO_s_mem());
	b64 = BIO_push(b64, bmem);
	BIO_write(b64, input, length);
	BIO_flush(b64);
	BIO_get_mem_ptr(b64, &bptr);

	char * buff = (char *) malloc(bptr->length + 1);
	memcpy(buff, bptr->data, bptr->length);
	buff[bptr->length] = 0;

	BIO_free_all(b64);

	return buff;
}

char *debase64(char *input, int length)
{
	BIO * b64 = NULL;
	BIO * bmem = NULL;
	char * buffer = (char *) malloc(length);
	memset(buffer, 0, length);

	b64 = BIO_new(BIO_f_base64());
	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
	bmem = BIO_new_mem_buf(input, length);
	bmem = BIO_push(b64, bmem);
	BIO_read(bmem, buffer, length);

	BIO_free_all(bmem);

	return buffer;
}

char *rsa_encrypt(const char *plain_text, char *result, int size, const char *pk_filename)
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

	char *temp = base64((char *) cipher, strlen((char *) cipher));
	strcpy(result, temp);
	free(cipher);
	free(temp);
	return result;
}

char *rsa_decrypt(const char *cipher, char *plain_text, int size, const char *sk_filename)
{
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
	if(len +1 > size)
	{
		RSA_free(rsa);
		return NULL;
	}
	memset(plain_text, 0, size);

	char *temp = (char *)debase64(cipher, strlen(cipher));
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
	return plain_text;
}

char *rsa_sign(const char *text, char *signature, int size, const char *sk_filename)
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
	char *buf = base64((char *) sig, strlen((char *) sig));
	free(sig);
	RSA_free(rsa);
	strcpy(signature, buf);
	free(buf);
	return signature;
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
