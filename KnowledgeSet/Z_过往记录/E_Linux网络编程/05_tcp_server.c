#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	 struct  sockaddr_in sockaddr_server, sockaddr_client;
	 int addrlen = sizeof(sockaddr_client);
	//1.创建套接字
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("socket failed\n");
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
	//监听，最大监听数目
	listen(sockfd, 5);

	int ret_accept = accept(sockfd, (struct  sockaddr *)&sockaddr_client, &addrlen);
	if (ret_accept == -1) {
		perror("accept failed\n");
		return -1;			
	}	
	
	//写文件，接收写文件标志
	int fd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0777);
	
	char buf[1024] = {0}, count = 1;
	while ((count = read(ret_accept, buf, 1024)) > 0) {		
		printf("%s", buf);
		write(fd, buf, count);	
	}
	
	int ret_close = close(ret_accept);
	ret_close |= close(sockfd);
	if (ret_close == -1) {
		perror("close failed\n");
		return -1;
	}
	return 0;
}