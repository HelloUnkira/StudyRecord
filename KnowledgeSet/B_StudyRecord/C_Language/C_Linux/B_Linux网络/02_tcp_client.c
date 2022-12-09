#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char* argv[])
{
	 struct  sockaddr_in sockaddr_server, sockaddr_client;
	 int addrlen = sizeof(sockaddr_server);
	//1.�����׽���
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket failed\n");
		return -1;
	}
	//2.������������ �� �ͻ���
	bzero(&sockaddr_server, sizeof(sockaddr_server));
	//��ʼ��IPV4Э����
	sockaddr_server.sin_family = AF_INET;
	//��ʼ���˿ں�
	sockaddr_server.sin_port = htons(2000);
	//��ʼ��IP��ַ���ַ�����ʽΪ��׼ip��ʽ��
	inet_aton(argv[1], &sockaddr_server.sin_addr);
	sockaddr_server.sin_addr.s_addr = inet_addr(argv[1]);
	
	int ret_connect = \
		connect(sockfd,(struct  sockaddr *)&sockaddr_server, addrlen);
	if (ret_connect == -1) {
		perror("connect failed\n");
		return -1;
	}
		
	char buf[1024] = {0};
	strcat(buf,"hello ! in the other sides,my friend\n");
	while (1) {
		write(ret_connect, buf, 1024);	
		sleep(5);
	}	
	
	close(ret_connect);
	close(sockfd);
	return 0;
}