/*
 * config.c
 *
 *  Created on: 2014-8-19
 *      Author: wzb
 *
 */
#include "config.h"

const int MAX_SIZE = 1024;
const int MAX_NUM = 10;
const char *config_filename = "key.conf";
const char *temp_config_filename = "temp_key.conf";
const char *open_mode[] =
{ "rt+", "wt+" };

int is_comment(const char *content)
{
	int i = 0;
	int len = strlen(content);
	while (i < len && isspace(content[i]) != 0)
	{
		++i;
	}
	if (i >= len || content[i] == '#') // empty line or comment line
		return 0;
	else
		return 1;
}

char *get_first_column(const char *line)
{
	char *ptr = NULL;
	char content[MAX_SIZE];
	strcpy(content, line);
	ptr = strtok(content, " \n\t\f\r\v");
	return ptr;
}

char *get_second_column(const char *line)
{
	char *ptr = NULL;
	char content[MAX_SIZE];
	strcpy(content, line);
	ptr = strtok(content, " \n\t\f\r\v");
	if (NULL != ptr)
	{
		ptr = strtok(NULL, " \n\t\f\r\v");
	}

	return ptr;
}

char *get_third_column(const char *line)
{
	char *ptr = NULL;
	char content[MAX_SIZE];
	strcpy(content, line);
	ptr = strtok(content, " \n\t\f\r\v");
	if (NULL != ptr)
	{
		ptr = strtok(NULL, " \n\t\f\r\v");
		if (NULL != ptr)
		{
			ptr = strtok(NULL, " \n\t\f\r\v");
		}
	}

	return ptr;
}

int get_uuid_number(const char *line)
{
	char *ptr = NULL;
	int num = 0;

	ptr = get_third_column(line);
	if (NULL == ptr)
	{
		return 0;
	}

	ptr = strtok(ptr, ","); //split uuid
	while (ptr != NULL )
	{
		num++;
		ptr = strtok(NULL, ",");
	}
	return num;
}

int print_a_by_b(const char *key, const int type,
		const struct encrypt_operations *en)
{
	char content[MAX_SIZE];
	char *ptr = NULL;
	FILE *f = fopen(config_filename, open_mode[0]);
	if (NULL == f)
	{
		printf("error: fail to open the configure file!\n");
		return -1;
	}

	while (!feof(f))
	{
		if (NULL == fgets(content, MAX_SIZE, f))
			continue;

		content[strlen(content) - 2] = '\0'; // remove the newline character
		if (0 != is_comment(content) && strstr(content, key) != NULL )
		{
			switch (type)
			{
			case 0:
				ptr = get_first_column(content);
				if (NULL != ptr)
					printf("%s\n", ptr);
				break;
			case 1:
				if (NULL == en)
				{
					printf("error:key error!\n");
					fclose(f);
					return -1;
				}

				ptr = get_second_column(content); // print key
				if (NULL != ptr)
				{
					char *temp = NULL;
					char result[200];
					char tt[250];
					strcpy(tt, ptr);
					if (NULL
							!= (temp = (*(en->decrypt))(tt, result, 200,
									en->sk_filename)))
					{
						printf("%s\n", result);
					}
					else
					{
						printf("error:can not get key!\n");
					}
				}
				break;
			case 2:
				ptr = get_third_column(content);
				if (NULL != ptr)
					printf("%s\n", ptr);
				break;
			case 3:
				ptr = get_first_column(content);
				if (NULL != ptr)
					printf("%s ", ptr);

				ptr = get_third_column(content);
				if (NULL != ptr)
					printf("%s", ptr);
				printf("\n");
				break;
			}
		}
	}

	return fclose(f);
}

int print_uuid_by_id(const char *id)
{
	return print_a_by_b(id, 2, NULL );
}

int print_id_by_uuid(const char *uuid)
{
	return print_a_by_b(uuid, 0, NULL );
}

int print_key_by_id(const char *id, const struct encrypt_operations *en)
{
	return print_a_by_b(id, 1, en);
}

int print_key_by_uuid(const char *uuid, const struct encrypt_operations *en)
{
	return print_a_by_b(uuid, 1, en);
}

int print_id_and_uuid_by_id_or_uuid(const char *key)
{
	return print_a_by_b(key, 3, NULL );
}

int print_all_id_and_uuid()
{
	return print_a_by_b(" ", 3, NULL );
}

char *remove_sub_string(const char *line, const char *uuid)
{
	char *ptr = strstr(line, uuid);
	int length = 0;
	char *ans = NULL;
	int tag = 0;
	if (ptr != NULL )
	{
		ans = (char *) malloc(sizeof(char) * (strlen(line) - strlen(uuid) + 1));
		if (ans == NULL )
		{
			return NULL ;
		}

		length = ptr - line;
		if (line[length - 1] == ',')
		{
			tag = 1;
			length -= 1;
		}
		strncpy(ans, line, length);
		if (ptr[strlen(uuid)] == ',' && tag == 0) // only remove one ','
			ptr++;
		strcat(ans, ptr + strlen(uuid));
		return ans;
	}

	return NULL ;
}

int remove_uuid(const char *uuid)
{
	FILE *f = NULL;
	FILE *f2 = NULL;
	char line[MAX_SIZE];
	char *result;

	if (NULL == (f = fopen(config_filename, open_mode[0])) || NULL == (f2 =
			fopen(temp_config_filename, open_mode[1])))
	{
		printf("error: fail to open the configure file!\n");
		return -1;
	}

	while (!feof(f))
	{
		if (NULL != fgets(line, MAX_SIZE, f))
		{
			if ((result = remove_sub_string(line, uuid)) != NULL )
			{
				//delete old uuid
				fputs(result, f2);
				free(result);
				continue;
			}
			else
			{
				fputs(line, f2);
			}
		}
	}
	fclose(f);
	fclose(f2);

	return rename(temp_config_filename, config_filename);
}

int remove_id(const char *id)
{
	FILE *f = NULL;
	FILE *f2 = NULL;
	char line[MAX_SIZE];

	if (NULL == (f = fopen(config_filename, open_mode[0])) || NULL == (f2 =
			fopen(temp_config_filename, open_mode[1])))
	{
		printf("error: fail to open the configure file!\n");
		return -1;
	}

	while (!feof(f))
	{
		if (NULL != fgets(line, MAX_SIZE, f))
		{
			if (NULL == strstr(line, id))
			{
				fputs(line, f2);
			}
		}
	}
	fclose(f);
	fclose(f2);
	return rename(temp_config_filename, config_filename);
}

/*
 * judge if id is in the file
 */
int judge_id(const char *id)
{
	FILE *f = NULL;
	char line[MAX_SIZE];
	if (NULL == (f = fopen(config_filename, open_mode[0])))
	{
		printf("error: fail to open the configure file!\n");
		return -1;
	}

	while (!feof(f))
	{
		if (NULL != fgets(line, MAX_SIZE, f))
		{
			if (NULL != strstr(line, id))
			{
				fclose(f);
				return 0;
			}
		}
	}

	fclose(f);
	return 1; // no this id
}

int update_uuid(const char *id, const char *uuid)
{
	FILE *f = NULL;
	FILE *f2 = NULL;
	char line[MAX_SIZE];
	char *result;

	switch (judge_id(id))
	{
	case 1:
		printf("error: select invalid volume key\n");
		return -1;
	case -1:
		printf("error: fail to open the configure file!\n");
		return -1;
		break;
	default:
		break;
	}

	if (NULL == (f = fopen(config_filename, open_mode[0])) || NULL == (f2 =
			fopen(temp_config_filename, open_mode[1])))
	{
		printf("error: fail to open the configure file!\n");
		return -1;
	}

	while (!feof(f))
	{
		if (NULL != fgets(line, MAX_SIZE, f))
		{

			if ((result = remove_sub_string(line, uuid)) != NULL )
			{
				//delete old uuid
				fputs(result, f2);
				free(result);
				continue;
			}

			if (NULL != strstr(line, id))
			{
				//insert uuid
				char *ptr = strstr(line, "\r\n");
				if (NULL != ptr)
					line[ptr - line] = 0;

				int number = get_uuid_number(line);
				if (number >= 10)
				{
					printf(
							"error:number exceed, please select another volume key\n");
					return -1;
				}
				if (number >= 0)
				{
					if (number > 0)
						strcat(line, ",");
					else if (isspace(line[strlen(line) - 1]) == 0)
						strcat(line, " ");
					strcat(line, uuid);
					strcat(line, "\r\n");
					fputs(line, f2);
				}
			}
			else
			{
				fputs(line, f2);
			}
		}
	}
	fclose(f);
	fclose(f2);

	return rename(temp_config_filename, config_filename);
}

