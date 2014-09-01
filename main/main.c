/*
 * main.c
 *
 *  Created on: 2014-8-19
 *      Author: pc
 */

#include <getopt.h>
#include <time.h>
#include "config.h"

struct option const long_options[] =
{
{ "set", no_argument, NULL, 's' },
{ "list", no_argument, NULL, 'l' },
{ "remove", no_argument, NULL, 'r' },
{ "id", optional_argument, NULL, 'i' },
{ "uuid", optional_argument, NULL, 'u' },
{ "plain_key", no_argument, NULL, 'k' },
{ "config_pathname", required_argument, NULL, 'c' },
{ "sk_pathname", required_argument, NULL, 'S' },
{ "pk_pathname", required_argument, NULL, 'P' },
{ "help", no_argument, NULL, 'h' },
{ NULL, 0, NULL, 0 } };

#define CONFIG_FILENAME "key.conf"
#define SK_FILENAME "rsa_priv.key"
#define PK_FILENAME "rsa_pub.key"

struct kmc_option
{
	short mode;
	short plain_key;
	bool id;
	bool uuid;

	char sk_pathname[PATH_MAX];
	char pk_pathname[PATH_MAX];
	char config_pathname[PATH_MAX];
	char uuid_content[70];
	char id_content[10];
};

#define LIST_CMD 1
#define SET_CMD 2
#define REMOVE_CMD 3
#define HELP_CMD 4

void kmc_option_init(struct kmc_option *x)
{
	x->mode = 0;
	x->plain_key = false;
	x->id = false;
	x->uuid = false;

	strcpy(x->id_content, "");
	strcpy(x->uuid_content, "");
	strcpy(x->sk_pathname, SK_FILENAME);
	strcpy(x->pk_pathname, PK_FILENAME);
	strcpy(x->config_pathname, CONFIG_FILENAME);
}

void do_help()
{
	printf(("Usage: kmc [OPTION]... \n"));
	fputs(("\
volume key and volume key relation operation interface.\n\
\n\
"),
			stdout);
	fputs(
			("\
Mandatory arguments to long options are mandatory for short options too.\n\
"),
			stdout);
	fputs(
			("\
  -l, --list        list the information of the volume .\n\
                    egg:\n\
                      kmc -l \n\
                      kmc -l -i=10000004 -u \n\
                      kmc -l -u=550E8400-E29B-11D4-A716-44665544asdf \n\
                      kmc -l -i=10000004 -k\n\
                      kmc -l -u=550E8400-E29B-11D4-A716-44665544asdf -k \n\
"),
			stdout);
	fputs(
			("\
  -s, --set         set the volume key for the volume ~\n\
                    egg:\n\
                      kmc -s -i=10000004 -u=550E8400-E29B-11D4-A716-44665544asdf\n\
"),
			stdout);
	fputs(
			("\
  -r, --remove      delete the volume key or volume key relation\n\
                    egg:\n\
                     delete the volume key:\n\
                      kmc -r -i=10000004\n\
                     delete the volume key relation:\n\
                      kmc -r -u=550E8400-E29B-11D4-A716-44665544asdf\n\
"),
			stdout);
	fputs(
			("\
  -c, --config_pathname key file pathname\n\
                    egg:\n\
                     delete the volume key relation:\n\
                      kmc -l -i=10000001 -c key.conf\n\
"),
			stdout);
	fputs(
			("\
  -P, --pk_pathname public key pathname\n\
                    egg:\n\
                     print the volume key:\n\
                      kmc -l -i=10000004 -k -P rsa_pub.key\n\
"),
			stdout);

	fputs(
			("\
  -S, --sk_pathname secrite key pathname\n\
                    egg:\n\
                     print the volume key:\n\
                     kmc -l -i=10000004 -k -S rsa_priv.key\n\
"),
			stdout);

	fputs(
			("\
  -r, --remove      delete the volume key or volume key relation\n\
                    egg:\n\
                     delete the volume key:\n\
                      kmc -r -i=10000004\n\
                     delete the volume key relation:\n\
                      kmc -r -u=550E8400-E29B-11D4-A716-44665544asdf\n\
"),
			stdout);
	fputs(
			("\
  -i, --id[=ID]     volume key id\n\
  -u, --uuid[=uuid] volume uuid\n\
  -k, --plain_key   get the plan_key\n\
"),
			stdout);
	fputs(("\n\
Exit status:\n\
   0  if OK,\n\
  -1  if error\n\
  \n\
"),
			stdout);
}

inline int show_command_error()
{
	fprintf(stderr, "selected one command in -l -r -s\n!");
	exit(-1);
}

int decode_switch(int argc, char **argv, struct kmc_option *x)
{
	int c;
	while (1)
	{
		c = getopt_long_only(argc, argv, "lsrkhiuS:P:c:", long_options, NULL );
		if (c == -1)
			break;

		switch (c)
		{
		case 'l':
			if (x->mode > 0)
				show_command_error();
			x->mode = LIST_CMD;
			break;

		case 's':
			if (x->mode > 0)
				show_command_error();
			x->mode = SET_CMD;
			break;

		case 'r':
			if (x->mode > 0)
				show_command_error();
			x->mode = REMOVE_CMD;
			break;

		case 'h':
			if (x->mode > 0)
				show_command_error();
			x->mode = HELP_CMD;
			break;

		case 'k':
			x->plain_key = true;
			break;

		case 'i':
			x->id = true;
			if (optarg && strlen(optarg) < sizeof(x->id_content))
				strcpy(x->id_content, optarg);
			break;

		case 'u':
			x->uuid = true;
			if (optarg && strlen(optarg) < sizeof(x->uuid_content))
				strcpy(x->uuid_content, optarg);
			break;

		case 'c':
			if (strlen(optarg) < PATH_MAX)
				strcpy(x->config_pathname, optarg);
			break;

		case 'P':
			if (*optarg == '=')
				++optarg;
			if (strlen(optarg) < PATH_MAX)
				strcpy(x->pk_pathname, optarg);
			break;
		case 'S':
			if (*optarg == '=')
				++optarg;
			if (strlen(optarg) < PATH_MAX)
				strcpy(x->sk_pathname, optarg);
			break;

		case '?':
			break;

		default:
			printf("?? getopt returned character code 0%o ??\n", c);
			exit(-1);
		}
	}

	return optind;
}

int do_list(const struct kmc_option *x)
{
	if (x->plain_key)
	{
		struct encrypt_operations *en = set_encryption_method("rsa",
				x->sk_pathname, x->pk_pathname);
		if(en == NULL)
		{
			fprintf(stderr, "key error\n");
			exit(1);
		}

		if (x->id && strlen(x->id_content) != 0)
			return do_list_key(x->config_pathname, x->id_content, en);
		if (x->uuid && strlen(x->uuid_content) != 0)
			return do_list_key(x->config_pathname, x->uuid_content, en);

		fprintf(stderr, "input correct id or uuid\n");
		return -1;
	}

	if (strlen(x->id_content) != 0)
	{
		if (!x->uuid)
			return do_list_id_uuid(x->config_pathname, x->id_content);
		else if (strlen(x->uuid_content) == 0)
			return do_list_uuid(x->config_pathname, x->id_content);
	}
	else if (strlen(x->uuid_content) != 0)
		return do_list_id(x->config_pathname, x->uuid_content);
	else
		return do_list_line(x->config_pathname);

	fprintf(stderr, "invalid command, read more by help\n");
	return -1;
}

void rand_temp_pathname(const char *old_pathname, char *pathname, size_t len)
{

	if (strlen(old_pathname) + 4 >= len)
	{
		fprintf(stderr, "%s pathname too long!\n", old_pathname);
		exit(-1);
	}

	srand((int) time(0));
	char buf[5];
	sprintf(buf, "%d", rand() % 10000);
	strcpy(pathname, old_pathname);
	strcat(pathname, buf);
	if(access(pathname, "r") == 0) // if temp file exist, generate another temp file pathname	
		rand_temp_pathname(old_pathname, pathname, len);
}

int do_set(const struct kmc_option *x)
{
	char temp_pathname[200];
	rand_temp_pathname(x->config_pathname, temp_pathname, 200);

	if (strlen(x->id_content) != 0 && strlen(x->uuid_content) != 0)
		return do_update_uuid(x->config_pathname, temp_pathname, x->id_content,
				x->uuid_content);

	fprintf(stderr, "invalid command, read more by help\n");
	return -1;
}

int do_remove(const struct kmc_option *x)
{
	char temp_pathname[200];
	rand_temp_pathname(x->config_pathname, temp_pathname, 200);

	if (strlen(x->id_content) != 0 && !x->uuid)
		return do_remove_id(x->config_pathname, temp_pathname, x->id_content);

	if (!x->id && strlen(x->uuid_content) != 0)
		return do_remove_uuid(x->config_pathname, temp_pathname,
				x->uuid_content);

	fprintf(stderr, "invalid command, read more by help\n");
	return -1;
}

int command(const struct kmc_option *x)
{
	set_encryption_method("rsa", x->sk_pathname, x->pk_pathname);

	switch (x->mode)
	{
	case LIST_CMD:
		do_list(x);
		break;
	case SET_CMD:
		do_set(x);
		break;
	case REMOVE_CMD:
		do_remove(x);
		break;
	case HELP_CMD:
		do_help();
		break;
	default:
		fprintf(stderr, "command error\n");
		exit(-1);
	}

	return 0;
}

int main(int argc, char ** argv)
{
	int status;
	struct kmc_option *x = (struct kmc_option *) malloc(
			sizeof(struct kmc_option));
	if (x != NULL )
	{
		kmc_option_init(x);
		decode_switch(argc, argv, x);
		status = command(x);
		free(x);
		return status;
	}
	return 0;
}
