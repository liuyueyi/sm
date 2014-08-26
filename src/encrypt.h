/*
 * encrypt.h
 *
 *  Created on: 2014-8-21
 *      Author: pc
 */

#ifndef ENCRYPT_H_
#define ENCRYPT_H_

#include "rsa.h"

struct encrypt_operations
{
	char *(*encrypt)(const char *plain_text, char *result, int size, const char *pk_filename);
	char *(*decrypt)(const char *cipher, char *result, int size, const char *sk_filename);
	char *(*sign)(const char *text, char *signature, int size, const char *sk_filename);
	int (*verify)(const char *text, const char *signature,
			const char *pk_filename);

	char sk_filename[100];
	char pk_filename[100];
};

struct encrypt_operations *set_encryption_method(const char *method,
		const char *sk_filename, const char *pk_filename);

#endif /* ENCRYPT_H_ */
