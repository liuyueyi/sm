/*
 * encrypt.c
 *
 *  Created on: 2014-8-25
 *      Author: pc
 */

#include "encrypt.h"

struct encrypt_operations * set_encryption_method(const char *method,
		const char *sk_filename, const char *pk_filename)
{
	struct encrypt_operations *en = (struct encrypt_operations *) malloc(sizeof(struct encrypt_operations));
	if (NULL == en)
		return NULL ;

	if (strcmp(method, "rsa") == 0)
	{
		en->encrypt = rsa_encrypt;
		en->decrypt = rsa_decrypt;
		en->sign = rsa_sign;
		en->verify = rsa_verify;
		strcpy(en->sk_filename, sk_filename);
		strcpy(en->pk_filename, pk_filename);
	}

	return en;
}
