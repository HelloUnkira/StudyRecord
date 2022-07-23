#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char* argv[])
{
	 struct  sockaddr_in sockaddr_server, sockaddr_client;
	 int addrlen = sizeof(sockaddr_client);
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
	
	//�󶨵�ַ
	bind(sockfd, (struct  sockaddr *)&sockaddr_server, sizeof(sockaddr_server));
	//��������������Ŀ
	listen(sockfd, 5);
	
	int ret_accept = accept(sockfd, (struct  sockaddr *)&sockaddr_client, &addrlen);
	if (ret_accept == -1) {
		perror("accept failed\n");
		return -1;			
	}	
	
	char buf[1024] = {0};
	while (1) {
		int count = read(ret_accept, buf, 1024);	
		printf("%s", buf);	
		sleep(5);
	}
	
	close(ret_accept);
	close(sockfd);
	return 0;
}