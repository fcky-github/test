#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main(int argc, const char *argv[])
{
	int sockfd;
	struct sockaddr_in serveraddr;
	socklen_t len = sizeof(struct sockaddr_in);
	char buf[64] = {0};

	if (argc < 3)
	{
		puts("please input correct argument!");
		exit(1);
	}


	//填充服务器端的地址信息
	memset(&serveraddr, 0, sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(atoi(argv[2]));
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		perror("fail to create socket");
		exit(1);
	}

	//请求连接指定的服务器程序
	if (connect(sockfd, (struct sockaddr*)&serveraddr, len) < 0)
	{
		perror("fail to connect");
		close(sockfd);
		exit(1);
	}

	while (1)
	{
		fgets(buf, sizeof(buf), stdin);
		buf[strlen(buf) - 1] = '\0';


		//发送给服务器程序
		send(sockfd, buf, sizeof(buf), 0);
	}

	return 0;
}
