 #include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void* thread_read(void* arg)
{
	
}

void* thread_write(void* arg)
{
	
}

int main(int argc, char* argv[])
{
	 struct  sockaddr_in sockaddr_server, sockaddr_client;
	 int addrlen = sizeof(sockaddr_client);
	//1.�����׽���
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
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
	
	//�󶨵�ַ
	bind(sockfd, (struct  sockaddr *)&sockaddr_server, sizeof(sockaddr_server));

	char buf[1024] = {0};
	strcat(buf, "hello server\n");
	while (1) {
		ssize_t ret_sendto = sendto(sockfd, buf, 1024, 0, \
			(struct  sockaddr *)&sockaddr_server, sizeof(sockaddr_server));
		sleep(5);
	}
	
	close(sockfd);
	return 0;
}