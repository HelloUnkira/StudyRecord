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
	//1.�����׽���
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
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
	
	//���ļ������ն��ļ���־
	int fd = open(argv[2], O_RDONLY);
	if (fd == -1) {
		perror("read failed\n");
		return -1;
	}
	
	char buf[1024] = {0}, count;
	while ((count = read(fd, buf, 1024)) > 0) {	
		printf("%s", buf);
		write(sockfd, buf, count);
	}
	
	int ret_close = close(sockfd);
	ret_close |= close(fd);
	if (ret_close == -1) {
		perror("close failed\n");
		return -1;
	}
	
	return 0;
}

