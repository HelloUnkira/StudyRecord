#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char* argv[])
{
	int on;
	struct  sockaddr_in sockaddr_server, sockaddr_client;
	int addrlen = sizeof(sockaddr_client);
	//1.创建套接字
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1) {
		perror("socket failed\n");
		return -1;
	}

	int ret_setsockopt = setsockopt(sockfd, \
		SOL_SOCKET, SO_BROADCAST, (void *)&on, sizeof(on));
	if (sockfd == -1) {
		perror("setsockopt failed\n");
		return -1;
	}
	
	//2.创建服务器端 或 客户端
	bzero(&sockaddr_server, sizeof(sockaddr_server));
	//初始化IPV4协议族
	sockaddr_server.sin_family = AF_INET;
	//初始化端口号
	sockaddr_server.sin_port = htons(2000);
	//初始化IP地址（字符串格式为标准ip格式）
	inet_aton(argv[1], &sockaddr_server.sin_addr);
	sockaddr_server.sin_addr.s_addr = inet_addr(argv[1]);
	
	//绑定地址
	bind(sockfd, (struct  sockaddr *)&sockaddr_server, sizeof(sockaddr_server));

	while (1) {
		char buf[1024] = {0};
		ssize_t ret_recvfrom = recvfrom(sockfd, buf, 1024, 0, \
			(struct  sockaddr *)&sockaddr_client, &addrlen);		
		printf("%s", buf);
		
		}
		sleep(1);
	
	close(sockfd);
	return 0;
}
