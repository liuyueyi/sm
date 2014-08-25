/*
 * main.c
 *
 *  Created on: 2014-8-19
 *      Author: pc
 */

#include <getopt.h>
#include "config.h"

struct option const long_options[] =
{
{ "set", no_argument, NULL, 's' },
{ "list", no_argument, NULL, 'l' },
{ "remove", no_argument, NULL, 'r' },
{ "id", optional_argument, NULL, 'i' },
{ "uuid", optional_argument, NULL, 'u' },
{ "plain_key", no_argument, NULL, 'k' },
{ "help", no_argument, NULL, 'h' },
{ NULL, 0, NULL, 0 } };

struct kmc_option
{
	short mode;
	short list_mode;
	short set_mode;
	short help_mode;
	short remove_mode;
	short plain_key_mode;
	short id_mode;
	short uuid_mode;

	char *id;
	char *uuid;
};

void kmc_option_init(struct kmc_option *x)
{
	x->mode = 0;
	x->list_mode = 0;
	x->set_mode = 0;
	x->help_mode = 0;
	x->remove_mode = 0;
	x->plain_key_mode = 0;
	x->id_mode = 0;
	x->uuid_mode = 0;

	x->id = NULL;
	x->uuid = NULL;
}

void help()
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

int decode_switch(int argc, char **argv, struct kmc_option *x)
{
	int c;
	int option_index

	= 0;
	while (1)
	{
		c = getopt_long_only(argc, argv, "lsrkhiu", long_options,
				&option_index);

		if (c == -1)
			break;
		switch (c)
		{
		case 'l':
			x->mode += 1;
			x->list_mode = 1;
			break;

		case 's':
			x->mode += 1;
			x->set_mode = 1;
			break;

		case 'r':
			x->mode += 1;
			x->remove_mode = 1;
			break;

		case 'k':
			x->plain_key_mode = 1;
			break;

		case 'h':
			x->mode += 1;
			x->help_mode = 1;
			break;

		case 'i':
			x->id_mode = 1;
			x->id = optarg;
			//strcpy(x->id, optarg);
			break;

		case 'u':
			x->uuid_mode = 1;
			x->uuid = optarg;
			//strcpy(x->uuid, optarg);
			break;

		case '?':
			break;

		default:
			printf("?? getopt returned character code 0%o ??\n", c);
			break;
		}
	}

	return optind;
}

int command(const struct kmc_option *x, const struct ENCRYPT *en)
{
	if (x->mode != 1)
	{
		return -1;
	}

	if (x->list_mode == 1)
	{
		if (x->id == NULL && x->uuid == NULL )
		{
			if (x->plain_key_mode == 1)
			{
				return -1;
			}
			else
			{
				return print_all_id_and_uuid();
			}
		}
		else if (x->id == NULL )
		{
			if (x->plain_key_mode == 1)
			{
				return print_key_by_uuid(x->uuid, en);
			}
			else if (x->id_mode == 1)
			{
				return print_id_by_uuid(x->uuid);
			}
			else
			{
				return print_id_and_uuid_by_id_or_uuid(x->uuid);
			}
		}
		else if (x->uuid == NULL )
		{
			if (x->plain_key_mode == 1)
			{
				return print_key_by_id(x->id, en);
			}
			else if (x->uuid_mode == 1)
			{
				return print_uuid_by_id(x->id);
			}
			else
			{
				return print_id_and_uuid_by_id_or_uuid(x->id);
			}
		}
	}

	if (x->set_mode == 1)
	{
		if (x->id == NULL || x->uuid == NULL || x->plain_key_mode == 1)
		{
			return -1;
		}
		else
		{
			return update_uuid(x->id, x->uuid);
		}
	}

	if (x->remove_mode == 1)
	{
		if ((x->id_mode == 1 && x->uuid_mode == 1)
				|| (x->id_mode == 0 && x->uuid_mode == 0))
		{
			return -1;
		}

		if (x->id != NULL )
		{
			return remove_id(x->id);
		}
		else if (x->uuid != NULL )
		{
			return remove_uuid(x->uuid);
		}
		else
		{
			return -1;
		}
	}

	if (x->help_mode == 1)
	{
		help();
	}

	return 0;
}

int main(int argc, char ** argv)
{
	int status;
	struct ENCRYPT *en = set_encryption_method("rsa", "test.key", "test_pub.key");
	struct kmc_option *x = (struct kmc_option *) malloc(
			sizeof(struct kmc_option));
	if (x != NULL )
	{
		kmc_option_init(x);
		decode_switch(argc, argv, x);
		status = command(x, en);
		free(en);
		free(x);
		return status;
	}
	return 0;
}
