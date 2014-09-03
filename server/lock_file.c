#include <sys/file.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *f = fopen("test2", "r");
	if(NULL == f)
	{
		fprintf(stderr, "test2 filename error\n");
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
