 #include <stdio.h>
#include <string.h>


#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct all_data{
	struct  sockaddr_in sockaddr_client;
	char* data;
};

void* thread_read(void* arg)
{
	printf("client ip addr is :%s\n", \
		inet_ntoa((((struct all_data*)arg)->sockaddr_client).sin_addr));
	printf("%s\n", ((struct all_data*)arg)->data);
}

void* thread_write(void* arg)
{
	//scanf("%s", ((struct all_data*)arg)->data);
}

int main(int argc, char* argv[])
{
	 struct  sockaddr_in sockaddr_server, sockaddr_client;
	 int addrlen = sizeof(sockaddr_client);
	//1.创建套接字
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
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
	
	//绑定地址
	bind(sockfd, (struct  sockaddr *)&sockaddr_server, sizeof(sockaddr_server));

	while (1) {
		char buf[1024] = {0};
		ssize_t ret_recvfrom = recvfrom(sockfd, buf, 1024, 0, \
			(struct  sockaddr *)&sockaddr_client, &addrlen);
			
		if (ret_recvfrom != -1) {	
			int pid = fork(); 		
            if(pid == 0) {
				char pthread_buf[1024] = {0};
				strcat(pthread_buf, buf);				
				struct all_data my_all_data;
				
				memcpy((void*)(&my_all_data.sockaddr_client), \
					(const void*) &sockaddr_client, sizeof(sockaddr_client));
				my_all_data.data = pthread_buf;
				pthread_t pth_read, pth_write;
				pthread_create(&pth_read, NULL, thread_read, (void*)&my_all_data);              
				pthread_create(&pth_write,NULL, thread_write, (void*)&my_all_data);       
				pthread_join(pth_read, NULL);
				pthread_join(pth_write, NULL);
			} 
		}
		sleep(1);
	}
	
	close(sockfd);
	return 0;
}