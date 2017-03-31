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
	int listenfd, connfd;
	struct sockaddr_in  serveraddr, clientaddr;
	socklen_t len = sizeof(struct sockaddr_in);
	char buf[64] = {0};
	ssize_t bytes;

	memset(&serveraddr, 0,  sizeof(struct sockaddr_in));
	memset(&clientaddr, 0,  sizeof(struct sockaddr_in));

	if (argc < 3)
	{
		puts("please input correct argument!");
		exit(1);
	}

	//填充结构体
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(atoi(argv[2]));//转换成网络字节序
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);//将点分10进制的字符串转换成u32类型


	//创建监听套接字
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0)
	{
		perror("fail to create socket");
		exit(1);
	}

	//完成监听套接字和本地主机的绑定
	if (bind(listenfd, (struct sockaddr*)&serveraddr, len) < 0)
	{
		perror("Fail to bind ");
		close(listenfd);
		exit(1);
	}

	//监听
	if (listen(listenfd, 10) < 0)
	{
		perror("fail to listen");
		close(listenfd);
		exit(1);
	}

	while (1)
	{
		//阻塞等待客户端的连接请求
		connfd = accept(listenfd, (struct sockaddr*)&clientaddr, &len);
		if (connfd < 0)
		{
			perror("fail to accept ..");
			continue;
		}

		//输出以连接的客户端的ip地址和端口号
		printf("client ip: %s,  client port: %u\n", inet_ntoa(clientaddr.sin_addr), 
				ntohs(clientaddr.sin_port));

		while (1)
		{
			//阻塞接收
			bytes = recv(connfd, buf, sizeof(buf), 0);
			if (bytes <= 0)
				break;

			printf("recv %d B,  is : %s\n", bytes, buf);
		}
		
		//关闭通信的套机字
		close(connfd);
	}

	return 0;
}
