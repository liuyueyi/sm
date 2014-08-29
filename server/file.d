hello world\n
do you know there is a logt
suanle afasgasjd zcv
sdgankjdgnagjeiorgjkvnvkjasdnf\n
asfdhasjdhf fadfasdf
gasdgasdgjwjegiojf213578\n489789235789231ur
/*
 * server.c
 *
 *  Created on: 2014-8-29
 *      Author: wzb
 */
 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#define temp_pathname "temp.d"
void server_process(int sockfd)
{
	char buffer[1024];
	int data_len = 0;
	bool start = true;
	FILE *f;
	if((f = fopen(temp_pathname, "w")) == NULL)
	{
		fprintf(stderr, "crete temp file %s error\n", temp_pathname);
		exit(1);
	}
	
	while(data_len = recv(sockfd, buffer, 1024, 0))
	{
		if(start)
		{
			printf("start to receive the data\n");
			printf("receive size = %d\n", data_len);
			start = false;
		}	
		
		if(data_len < 0)
		{
			fprintf(stderr, "receive error!\n");
			return;
		}
		
		fprintf(f, "%s", buffer);
		printf("%s\n-----------------------\n", buffer);
	}
	fclose(f);
	printf("--------------------------------------\n");
}

void init_server()
{
	int sockfd;
	int clientfd;
	int pid;
	uint16_t port = 10033;
	
	struct sockaddr_in server_addr, client_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr  = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "open data stream socket failed!\n");
		exit(1);
	}
	
	if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		fprintf(stderr, "bind data socket failed!\n");
		exit(1);
	}
	
	if(listen(sockfd, SOMAXCONN) < 0)
	{
		fprintf(stderr, "listen data stream failed\n");
		exit(1);
	}
	
	printf("-----------start------------\n");
	while(1)
	{
		int len = sizeof(client_addr);
		clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &len);
		if(clientfd < 0)
		{
			fprintf(stderr, "accept error\n");
			continue;
		}
		
		int pid = fork();
		if(pid == 0)
		{
			server_process(clientfd);
		}
		close(clientfd);
	}
}

int main()
{
	init_server();
	return 0;
}
/*
 * client.c
 *
 *  Created on: 2014-8-29
 *      Author: wzb
 */
 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define filename "file.d"

int main()
{
	int sockfd;
	struct sockaddr_in server_addr;
	uint16_t port = 10033;
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "socket eerror\n");
		exit(1);
	}
	
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");;
	
	if(connect(sockfd, (struct sockaddr*)(&server_addr), sizeof(struct sockaddr))<0)
	{
		fprintf(stderr, "connect eerror\n");
		return -1;
	}	
	
	FILE *f = NULL;
	if((f = fopen(filename, "r")) == NULL)
	{
		fprintf(stderr, "%s file error\n", filename);
		close(sockfd);
		exit(1);
	}
	
	char buf[LINE_MAX];
	while(fgets(buf, LINE_MAX, f))
	{
		send(sockfd, buf, strlen(buf), 0);
	}
	fclose(f);
	printf("-----------send over------------\n");
	close(sockfd);
	return 0;
}
hello world\n
do you know there is a logt
suanle afasgasjd zcv
sdgankjdgnagjeiorgjkvnvkjasdnf\n
asfdhasjdhf fadfasdf
gasdgasdgjwjegiojf213578\n489789235789231ur
/*
 * server.c
 *
 *  Created on: 2014-8-29
 *      Author: wzb
 */
 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#define temp_pathname "temp.d"
void server_process(int sockfd)
{
	char buffer[1024];
	int data_len = 0;
	bool start = true;
	FILE *f;
	if((f = fopen(temp_pathname, "w")) == NULL)
	{
		fprintf(stderr, "crete temp file %s error\n", temp_pathname);
		exit(1);
	}
	
	while(data_len = recv(sockfd, buffer, 1024, 0))
	{
		if(start)
		{
			printf("start to receive the data\n");
			printf("receive size = %d\n", data_len);
			start = false;
		}	
		
		if(data_len < 0)
		{
			fprintf(stderr, "receive error!\n");
			return;
		}
		
		fprintf(f, "%s", buffer);
		printf("%s\n-----------------------\n", buffer);
	}
	fclose(f);
	printf("--------------------------------------\n");
}

void init_server()
{
	int sockfd;
	int clientfd;
	int pid;
	uint16_t port = 10033;
	
	struct sockaddr_in server_addr, client_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr  = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "open data stream socket failed!\n");
		exit(1);
	}
	
	if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		fprintf(stderr, "bind data socket failed!\n");
		exit(1);
	}
	
	if(listen(sockfd, SOMAXCONN) < 0)
	{
		fprintf(stderr, "listen data stream failed\n");
		exit(1);
	}
	
	printf("-----------start------------\n");
	while(1)
	{
		int len = sizeof(client_addr);
		clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &len);
		if(clientfd < 0)
		{
			fprintf(stderr, "accept error\n");
			continue;
		}
		
		int pid = fork();
		if(pid == 0)
		{
			server_process(clientfd);
		}
		close(clientfd);
	}
}

int main()
{
	init_server();
	return 0;
}
/*
 * client.c
 *
 *  Created on: 2014-8-29
 *      Author: wzb
 */
 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define filename "file.d"

int main()
{
	int sockfd;
	struct sockaddr_in server_addr;
	uint16_t port = 10033;
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "socket eerror\n");
		exit(1);
	}
	
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");;
	
	if(connect(sockfd, (struct sockaddr*)(&server_addr), sizeof(struct sockaddr))<0)
	{
		fprintf(stderr, "connect eerror\n");
		return -1;
	}	
	
	FILE *f = NULL;
	if((f = fopen(filename, "r")) == NULL)
	{
		fprintf(stderr, "%s file error\n", filename);
		close(sockfd);
		exit(1);
	}
	
	char buf[LINE_MAX];
	while(fgets(buf, LINE_MAX, f))
	{
		send(sockfd, buf, strlen(buf), 0);
	}
	fclose(f);
	printf("-----------send over------------\n");
	close(sockfd);
	return 0;
}
hello world\n
do you know there is a logt
suanle afasgasjd zcv
sdgankjdgnagjeiorgjkvnvkjasdnf\n
asfdhasjdhf fadfasdf
gasdgasdgjwjegiojf213578\n489789235789231ur
/*
 * server.c
 *
 *  Created on: 2014-8-29
 *      Author: wzb
 */
 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#define temp_pathname "temp.d"
void server_process(int sockfd)
{
	char buffer[1024];
	int data_len = 0;
	bool start = true;
	FILE *f;
	if((f = fopen(temp_pathname, "w")) == NULL)
	{
		fprintf(stderr, "crete temp file %s error\n", temp_pathname);
		exit(1);
	}
	
	while(data_len = recv(sockfd, buffer, 1024, 0))
	{
		if(start)
		{
			printf("start to receive the data\n");
			printf("receive size = %d\n", data_len);
			start = false;
		}	
		
		if(data_len < 0)
		{
			fprintf(stderr, "receive error!\n");
			return;
		}
		
		fprintf(f, "%s", buffer);
		printf("%s\n-----------------------\n", buffer);
	}
	fclose(f);
	printf("--------------------------------------\n");
}

void init_server()
{
	int sockfd;
	int clientfd;
	int pid;
	uint16_t port = 10033;
	
	struct sockaddr_in server_addr, client_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr  = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "open data stream socket failed!\n");
		exit(1);
	}
	
	if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		fprintf(stderr, "bind data socket failed!\n");
		exit(1);
	}
	
	if(listen(sockfd, SOMAXCONN) < 0)
	{
		fprintf(stderr, "listen data stream failed\n");
		exit(1);
	}
	
	printf("-----------start------------\n");
	while(1)
	{
		int len = sizeof(client_addr);
		clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &len);
		if(clientfd < 0)
		{
			fprintf(stderr, "accept error\n");
			continue;
		}
		
		int pid = fork();
		if(pid == 0)
		{
			server_process(clientfd);
		}
		close(clientfd);
	}
}

int main()
{
	init_server();
	return 0;
}
/*
 * client.c
 *
 *  Created on: 2014-8-29
 *      Author: wzb
 */
 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define filename "file.d"

int main()
{
	int sockfd;
	struct sockaddr_in server_addr;
	uint16_t port = 10033;
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "socket eerror\n");
		exit(1);
	}
	
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");;
	
	if(connect(sockfd, (struct sockaddr*)(&server_addr), sizeof(struct sockaddr))<0)
	{
		fprintf(stderr, "connect eerror\n");
		return -1;
	}	
	
	FILE *f = NULL;
	if((f = fopen(filename, "r")) == NULL)
	{
		fprintf(stderr, "%s file error\n", filename);
		close(sockfd);
		exit(1);
	}
	
	char buf[LINE_MAX];
	while(fgets(buf, LINE_MAX, f))
	{
		send(sockfd, buf, strlen(buf), 0);
	}
	fclose(f);
	printf("-----------send over------------\n");
	close(sockfd);
	return 0;
}
hello world\n
do you know there is a logt
suanle afasgasjd zcv
sdgankjdgnagjeiorgjkvnvkjasdnf\n
asfdhasjdhf fadfasdf
gasdgasdgjwjegiojf213578\n489789235789231ur
/*
 * server.c
 *
 *  Created on: 2014-8-29
 *      Author: wzb
 */
 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#define temp_pathname "temp.d"
void server_process(int sockfd)
{
	char buffer[1024];
	int data_len = 0;
	bool start = true;
	FILE *f;
	if((f = fopen(temp_pathname, "w")) == NULL)
	{
		fprintf(stderr, "crete temp file %s error\n", temp_pathname);
		exit(1);
	}
	
	while(data_len = recv(sockfd, buffer, 1024, 0))
	{
		if(start)
		{
			printf("start to receive the data\n");
			printf("receive size = %d\n", data_len);
			start = false;
		}	
		
		if(data_len < 0)
		{
			fprintf(stderr, "receive error!\n");
			return;
		}
		
		fprintf(f, "%s", buffer);
		printf("%s\n-----------------------\n", buffer);
	}
	fclose(f);
	printf("--------------------------------------\n");
}

void init_server()
{
	int sockfd;
	int clientfd;
	int pid;
	uint16_t port = 10033;
	
	struct sockaddr_in server_addr, client_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr  = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "open data stream socket failed!\n");
		exit(1);
	}
	
	if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		fprintf(stderr, "bind data socket failed!\n");
		exit(1);
	}
	
	if(listen(sockfd, SOMAXCONN) < 0)
	{
		fprintf(stderr, "listen data stream failed\n");
		exit(1);
	}
	
	printf("-----------start------------\n");
	while(1)
	{
		int len = sizeof(client_addr);
		clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &len);
		if(clientfd < 0)
		{
			fprintf(stderr, "accept error\n");
			continue;
		}
		
		int pid = fork();
		if(pid == 0)
		{
			server_process(clientfd);
		}
		close(clientfd);
	}
}

int main()
{
	init_server();
	return 0;
}
/*
 * client.c
 *
 *  Created on: 2014-8-29
 *      Author: wzb
 */
 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define filename "file.d"

int main()
{
	int sockfd;
	struct sockaddr_in server_addr;
	uint16_t port = 10033;
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "socket eerror\n");
		exit(1);
	}
	
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");;
	
	if(connect(sockfd, (struct sockaddr*)(&server_addr), sizeof(struct sockaddr))<0)
	{
		fprintf(stderr, "connect eerror\n");
		return -1;
	}	
	
	FILE *f = NULL;
	if((f = fopen(filename, "r")) == NULL)
	{
		fprintf(stderr, "%s file error\n", filename);
		close(sockfd);
		exit(1);
	}
	
	char buf[LINE_MAX];
	while(fgets(buf, LINE_MAX, f))
	{
		send(sockfd, buf, strlen(buf), 0);
	}
	fclose(f);
	printf("-----------send over------------\n");
	close(sockfd);
	return 0;
}
hello world\n
do you know there is a logt
suanle afasgasjd zcv
sdgankjdgnagjeiorgjkvnvkjasdnf\n
asfdhasjdhf fadfasdf
gasdgasdgjwjegiojf213578\n489789235789231ur
/*
 * server.c
 *
 *  Created on: 2014-8-29
 *      Author: wzb
 */
 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#define temp_pathname "temp.d"
void server_process(int sockfd)
{
	char buffer[1024];
	int data_len = 0;
	bool start = true;
	FILE *f;
	if((f = fopen(temp_pathname, "w")) == NULL)
	{
		fprintf(stderr, "crete temp file %s error\n", temp_pathname);
		exit(1);
	}
	
	while(data_len = recv(sockfd, buffer, 1024, 0))
	{
		if(start)
		{
			printf("start to receive the data\n");
			printf("receive size = %d\n", data_len);
			start = false;
		}	
		
		if(data_len < 0)
		{
			fprintf(stderr, "receive error!\n");
			return;
		}
		
		fprintf(f, "%s", buffer);
		printf("%s\n-----------------------\n", buffer);
	}
	fclose(f);
	printf("--------------------------------------\n");
}

void init_server()
{
	int sockfd;
	int clientfd;
	int pid;
	uint16_t port = 10033;
	
	struct sockaddr_in server_addr, client_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr  = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "open data stream socket failed!\n");
		exit(1);
	}
	
	if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		fprintf(stderr, "bind data socket failed!\n");
		exit(1);
	}
	
	if(listen(sockfd, SOMAXCONN) < 0)
	{
		fprintf(stderr, "listen data stream failed\n");
		exit(1);
	}
	
	printf("-----------start------------\n");
	while(1)
	{
		int len = sizeof(client_addr);
		clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &len);
		if(clientfd < 0)
		{
			fprintf(stderr, "accept error\n");
			continue;
		}
		
		int pid = fork();
		if(pid == 0)
		{
			server_process(clientfd);
		}
		close(clientfd);
	}
}

int main()
{
	init_server();
	return 0;
}
/*
 * client.c
 *
 *  Created on: 2014-8-29
 *      Author: wzb
 */
 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define filename "file.d"

int main()
{
	int sockfd;
	struct sockaddr_in server_addr;
	uint16_t port = 10033;
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "socket eerror\n");
		exit(1);
	}
	
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");;
	
	if(connect(sockfd, (struct sockaddr*)(&server_addr), sizeof(struct sockaddr))<0)
	{
		fprintf(stderr, "connect eerror\n");
		return -1;
	}	
	
	FILE *f = NULL;
	if((f = fopen(filename, "r")) == NULL)
	{
		fprintf(stderr, "%s file error\n", filename);
		close(sockfd);
		exit(1);
	}
	
	char buf[LINE_MAX];
	while(fgets(buf, LINE_MAX, f))
	{
		send(sockfd, buf, strlen(buf), 0);
	}
	fclose(f);
	printf("-----------send over------------\n");
	close(sockfd);
	return 0;
}
hello world\n
do you know there is a logt
suanle afasgasjd zcv
sdgankjdgnagjeiorgjkvnvkjasdnf\n
asfdhasjdhf fadfasdf
gasdgasdgjwjegiojf213578\n489789235789231ur
/*
 * server.c
 *
 *  Created on: 2014-8-29
 *      Author: wzb
 */
 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#define temp_pathname "temp.d"
void server_process(int sockfd)
{
	char buffer[1024];
	int data_len = 0;
	bool start = true;
	FILE *f;
	if((f = fopen(temp_pathname, "w")) == NULL)
	{
		fprintf(stderr, "crete temp file %s error\n", temp_pathname);
		exit(1);
	}
	
	while(data_len = recv(sockfd, buffer, 1024, 0))
	{
		if(start)
		{
			printf("start to receive the data\n");
			printf("receive size = %d\n", data_len);
			start = false;
		}	
		
		if(data_len < 0)
		{
			fprintf(stderr, "receive error!\n");
			return;
		}
		
		fprintf(f, "%s", buffer);
		printf("%s\n-----------------------\n", buffer);
	}
	fclose(f);
	printf("--------------------------------------\n");
}

void init_server()
{
	int sockfd;
	int clientfd;
	int pid;
	uint16_t port = 10033;
	
	struct sockaddr_in server_addr, client_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr  = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "open data stream socket failed!\n");
		exit(1);
	}
	
	if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		fprintf(stderr, "bind data socket failed!\n");
		exit(1);
	}
	
	if(listen(sockfd, SOMAXCONN) < 0)
	{
		fprintf(stderr, "listen data stream failed\n");
		exit(1);
	}
	
	printf("-----------start------------\n");
	while(1)
	{
		int len = sizeof(client_addr);
		clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &len);
		if(clientfd < 0)
		{
			fprintf(stderr, "accept error\n");
			continue;
		}
		
		int pid = fork();
		if(pid == 0)
		{
			server_process(clientfd);
		}
		close(clientfd);
	}
}

int main()
{
	init_server();
	return 0;
}
/*
 * client.c
 *
 *  Created on: 2014-8-29
 *      Author: wzb
 */
 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define filename "file.d"

int main()
{
	int sockfd;
	struct sockaddr_in server_addr;
	uint16_t port = 10033;
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "socket eerror\n");
		exit(1);
	}
	
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");;
	
	if(connect(sockfd, (struct sockaddr*)(&server_addr), sizeof(struct sockaddr))<0)
	{
		fprintf(stderr, "connect eerror\n");
		return -1;
	}	
	
	FILE *f = NULL;
	if((f = fopen(filename, "r")) == NULL)
	{
		fprintf(stderr, "%s file error\n", filename);
		close(sockfd);
		exit(1);
	}
	
	char buf[LINE_MAX];
	while(fgets(buf, LINE_MAX, f))
	{
		send(sockfd, buf, strlen(buf), 0);
	}
	fclose(f);
	printf("-----------send over------------\n");
	close(sockfd);
	return 0;
}
hello world\n
do you know there is a logt
suanle afasgasjd zcv
sdgankjdgnagjeiorgjkvnvkjasdnf\n
asfdhasjdhf fadfasdf
gasdgasdgjwjegiojf213578\n489789235789231ur
/*
 * server.c
 *
 *  Created on: 2014-8-29
 *      Author: wzb
 */
 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#define temp_pathname "temp.d"
void server_process(int sockfd)
{
	char buffer[1024];
	int data_len = 0;
	bool start = true;
	FILE *f;
	if((f = fopen(temp_pathname, "w")) == NULL)
	{
		fprintf(stderr, "crete temp file %s error\n", temp_pathname);
		exit(1);
	}
	
	while(data_len = recv(sockfd, buffer, 1024, 0))
	{
		if(start)
		{
			printf("start to receive the data\n");
			printf("receive size = %d\n", data_len);
			start = false;
		}	
		
		if(data_len < 0)
		{
			fprintf(stderr, "receive error!\n");
			return;
		}
		
		fprintf(f, "%s", buffer);
		printf("%s\n-----------------------\n", buffer);
	}
	fclose(f);
	printf("--------------------------------------\n");
}

void init_server()
{
	int sockfd;
	int clientfd;
	int pid;
	uint16_t port = 10033;
	
	struct sockaddr_in server_addr, client_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr  = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "open data stream socket failed!\n");
		exit(1);
	}
	
	if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		fprintf(stderr, "bind data socket failed!\n");
		exit(1);
	}
	
	if(listen(sockfd, SOMAXCONN) < 0)
	{
		fprintf(stderr, "listen data stream failed\n");
		exit(1);
	}
	
	printf("-----------start------------\n");
	while(1)
	{
		int len = sizeof(client_addr);
		clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &len);
		if(clientfd < 0)
		{
			fprintf(stderr, "accept error\n");
			continue;
		}
		
		int pid = fork();
		if(pid == 0)
		{
			server_process(clientfd);
		}
		close(clientfd);
	}
}

int main()
{
	init_server();
	return 0;
}
/*
 * client.c
 *
 *  Created on: 2014-8-29
 *      Author: wzb
 */
 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define filename "file.d"

int main()
{
	int sockfd;
	struct sockaddr_in server_addr;
	uint16_t port = 10033;
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "socket eerror\n");
		exit(1);
	}
	
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");;
	
	if(connect(sockfd, (struct sockaddr*)(&server_addr), sizeof(struct sockaddr))<0)
	{
		fprintf(stderr, "connect eerror\n");
		return -1;
	}	
	
	FILE *f = NULL;
	if((f = fopen(filename, "r")) == NULL)
	{
		fprintf(stderr, "%s file error\n", filename);
		close(sockfd);
		exit(1);
	}
	
	char buf[LINE_MAX];
	while(fgets(buf, LINE_MAX, f))
	{
		send(sockfd, buf, strlen(buf), 0);
	}
	fclose(f);
	printf("-----------send over------------\n");
	close(sockfd);
	return 0;
}
hello world\n
do you know there is a logt
suanle afasgasjd zcv
sdgankjdgnagjeiorgjkvnvkjasdnf\n
asfdhasjdhf fadfasdf
gasdgasdgjwjegiojf213578\n489789235789231ur
/*
 * server.c
 *
 *  Created on: 2014-8-29
 *      Author: wzb
 */
 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#define temp_pathname "temp.d"
void server_process(int sockfd)
{
	char buffer[1024];
	int data_len = 0;
	bool start = true;
	FILE *f;
	if((f = fopen(temp_pathname, "w")) == NULL)
	{
		fprintf(stderr, "crete temp file %s error\n", temp_pathname);
		exit(1);
	}
	
	while(data_len = recv(sockfd, buffer, 1024, 0))
	{
		if(start)
		{
			printf("start to receive the data\n");
			printf("receive size = %d\n", data_len);
			start = false;
		}	
		
		if(data_len < 0)
		{
			fprintf(stderr, "receive error!\n");
			return;
		}
		
		fprintf(f, "%s", buffer);
		printf("%s\n-----------------------\n", buffer);
	}
	fclose(f);
	printf("--------------------------------------\n");
}

void init_server()
{
	int sockfd;
	int clientfd;
	int pid;
	uint16_t port = 10033;
	
	struct sockaddr_in server_addr, client_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr  = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "open data stream socket failed!\n");
		exit(1);
	}
	
	if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		fprintf(stderr, "bind data socket failed!\n");
		exit(1);
	}
	
	if(listen(sockfd, SOMAXCONN) < 0)
	{
		fprintf(stderr, "listen data stream failed\n");
		exit(1);
	}
	
	printf("-----------start------------\n");
	while(1)
	{
		int len = sizeof(client_addr);
		clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &len);
		if(clientfd < 0)
		{
			fprintf(stderr, "accept error\n");
			continue;
		}
		
		int pid = fork();
		if(pid == 0)
		{
			server_process(clientfd);
		}
		close(clientfd);
	}
}

int main()
{
	init_server();
	return 0;
}
/*
 * client.c
 *
 *  Created on: 2014-8-29
 *      Author: wzb
 */
 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define filename "file.d"

int main()
{
	int sockfd;
	struct sockaddr_in server_addr;
	uint16_t port = 10033;
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "socket eerror\n");
		exit(1);
	}
	
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");;
	
	if(connect(sockfd, (struct sockaddr*)(&server_addr), sizeof(struct sockaddr))<0)
	{
		fprintf(stderr, "connect eerror\n");
		return -1;
	}	
	
	FILE *f = NULL;
	if((f = fopen(filename, "r")) == NULL)
	{
		fprintf(stderr, "%s file error\n", filename);
		close(sockfd);
		exit(1);
	}
	
	char buf[LINE_MAX];
	while(fgets(buf, LINE_MAX, f))
	{
		send(sockfd, buf, strlen(buf), 0);
	}
	fclose(f);
	printf("-----------send over------------\n");
	close(sockfd);
	return 0;
}
+++++++++++++++++++++++++++++++++++++++++++++++++
