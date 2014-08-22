/*
 * config.h
 *
 *  Created on: 2014-8-19
 *      Author: pc
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int is_comment(const char *content);

char *get_first_column(const char *line);

char *get_second_column(const char *line);

char *get_third_column(const char *line);

int print_a_by_b(const char *key, const int type);

int print_uuid_by_id(const char *id);

int print_id_by_uuid(const char *uuid);

int print_key_by_id(const char *id);

int print_key_by_uuid(const char *uuid);

int print_id_and_uuid_by_id_or_uuid(const char *key);

int print_all_id_and_uuid();

char *remove_sub_string(const char *line, const char *uuid);

int remove_id(const char *id);

int remove_uuid(const char *uuid);

int judge_id(const char *id);

int update_uuid(const char *id, const char *uuid);

#endif /* CONFIG_H_ */
