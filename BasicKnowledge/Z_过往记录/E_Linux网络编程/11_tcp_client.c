#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/param.h>

#include <netinet/in.h>

#include <arpa/inet.h>

//客户端

int main(int argc, char* argv[])
{
	 struct  sockaddr_in sockaddr_server, sockaddr_client;
	 int addr_len = sizeof(sockaddr_client);
	//1.创建套接字
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
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
	
	
	int ret_connect = \
		connect(sockfd,(struct  sockaddr *)&sockaddr_server, addr_len);
	if (ret_connect == -1) {
		perror("connect failed\n");
		return -1;
	}
	
	//键盘网络数据多路选择
	fd_set rd_set;	
	char buf_send[2048];
	char buf_recv[2048];	
	int maxfd = MAX(sockfd, 0);
	
	while (1) {
		//初始化select结构体
		FD_ZERO(&rd_set);
		//设置其为选择数据+网络的模式
		FD_SET(0,&rd_set);
		FD_SET(sockfd, &rd_set);
		//多路选择开始
		int ret = select(maxfd+1, &rd_set, NULL, NULL, NULL);
		//如果是键盘有数据可读
		if(FD_ISSET(0, &rd_set))
		{
			bzero(buf_send, sizeof(buf_send));
			fgets(buf_send, sizeof(buf_send),stdin);
			write(sockfd, buf_send, strlen(buf_send));
		}
		//如果是网络有数据可读
		if(FD_ISSET(sockfd, &rd_set))
		{
			bzero(buf_recv, sizeof(buf_recv));
			int count = 0;
			recvfrom(sockfd, buf_recv, sizeof(buf_recv), 0, \
				(struct sockaddr *)&sockaddr_client, &addr_len);
			if (0 == strcmp(buf_recv, "quit now"))
				break;
			printf("%s:%s", inet_ntoa(sockaddr_client.sin_addr),buf_recv);
		}
	}	


	
	close(ret_connect);
	close(sockfd);
	return 0;
}