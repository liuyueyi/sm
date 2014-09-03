/*
 * lock_file.c
 *  对文件上锁，用于测试server中接受client发送的文件时，重改名的问题
 *		若文件上锁，则server接受到文件并保存为临时文件(rsa_key.conf.tm)，并等待文件解锁；当文件解锁后，将临时文件重命名为所需的文件名(rsa_key.conf)
 *  Created on: 2014-8-29
 *      Author: wzb
 */
#include <sys/file.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *f = fopen("rsa_key.conf", "r");
	if(NULL == f)
	{
		fprintf(stderr, "filename error\n");
		return 0;
	}
	
	if(0!=flock(fileno(f), LOCK_EX)) // if lock then wait; you can use LOCK_EX | LOCK_NB to break if file lock
	{
		printf("lock failed\n");
		return 0;
	}
	
	printf("input some character\n");
	getchar();
	flock(fileno(f), LOCK_UN);
	fclose(f);
	printf("close--\n");
	return 0;
}
