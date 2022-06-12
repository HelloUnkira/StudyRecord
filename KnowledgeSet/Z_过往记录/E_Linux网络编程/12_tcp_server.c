#include <stdio.h>
#include <stdlib.h>
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

typedef enum client_type
{
	NEWS_TYPE,
	ENTERTAINMENT_TYPE,
	SUPPLY_TYPE
} client_type;

typedef struct client_node
{
 int socket_fd;
 enum client_type type;
 char* logname; // 登录名
 struct client_node* prev;
 struct client_node* next;
} client_node;

typedef struct all_data 
{
	int socket_fd;
	struct  sockaddr_in sockaddr_client;
	char* pathname;
} all_data;

sem_t sem;
client_node* list_all = NULL;

//添加客户
void add_client(int socket_fd, char* client_name, int type)
{
	 struct client_node* buf = 
		calloc(1, sizeof( struct client_node));
	buf->socket_fd = socket_fd;
	//此处是否需要询问客户？
	buf->type = type;
	buf->logname = calloc(strlen(client_name) + 1, sizeof(char));
	strcpy(buf->logname, client_name);
	buf->prev = NULL;
	buf->next = NULL;
	
	//将其链入总链表中，头插法
	sem_wait(&sem);
	
	if (list_all == NULL)
		list_all = buf;
	else {
		list_all->prev = buf;
		buf->next = list_all;
		list_all = buf;
	}
	
	sem_post(&sem);
}

//删除客户，关闭通讯
remove_client(char* buf_receive_clientname)
{
	//查找那个结点
	struct client_node* buf = list_all;
	sem_wait(&sem);
	for (buf = list_all; buf != NULL; buf = buf->next) {
		if (0 == strcat(buf->logname, buf_receive_clientname)) {
			//要么找到要么找不到，不管找不找不到
			if (buf->prev == NULL) {
				//删除头节点
				if (buf->next != NULL) 
					buf->next->prev = NULL;
				list_all = buf->next;
			} else if (buf->next == NULL) {
				//删除尾结点
				if (buf->prev != NULL)
					buf->prev->next = NULL;
				else
					list_all = NULL;
			} else {
				//删除中间节点
				buf->prev->next = buf->next;
				buf->next->prev = buf->prev;
			}
				free(buf);
				break;					
		}		
	}
	sem_post(&sem);
}

//发送历史信息
void send_history(int sockfd, int fd)
{
	lseek(fd, 0, SEEK_SET);
	char buf[1024] = {0};
	int count = 0;
	while ((count = read(fd, buf, sizeof(buf))) > 0)
		write(sockfd, buf, count);
	lseek(fd, 0, SEEK_SET);
}	

void multicast_msg(int sockfd, client_type type, char *data)
{
	//查找某个结点
	struct client_node* buf = list_all;	
	sem_wait(&sem);	
	
	for (buf = list_all; buf != NULL; buf = buf->next) {
		if(buf->socket_fd != sockfd && buf->type == type)
			write(buf->socket_fd, data, strlen(data));
	}
	
	sem_post(&sem);
}

char buf_send [2048] = \
"hailo, bbs1.0 is started now!\n \
maybe you wanna have some choices.\n \
now please choose you action for now:\n \
当前默认就是正常进入后直接进入数据追加过程\n \
1, wai bi ba bo one:正常进入\n \
2, wai bi ba bo two:\n \
3, wai bi ba bo three:\n \
4, wai bi ba bo four:\n \
5, wai bi ba bo five:\n \
0, wai bi ba bo quit:退出\n ";
char buf_send1 [2048] = \
"hailo, your service is started now!\n \
maybe you wanna have some choices.\n \
now please choose you action for now:\n \
当前默认是追加完数据就彻底结束\n \
1, wai bi ba bo one:记录追加\n \
2, wai bi ba bo two:查看历史记录\n \
3, wai bi ba bo three:\n \
4, wai bi ba bo four:\n \
5, wai bi ba bo five:\n \
0, wai bi ba bo quit:退出\n ";

//新进程仅开了一个线程，则。。
//线程完全管理一切，
void* thread_read_write(void* arg)
{
	char buf_receive[2048] = {0};
			
	struct sockaddr_in sockaddr_client = \
		((struct all_data*)arg)->sockaddr_client;
	int sockfd = ((struct all_data*)arg)->socket_fd;
	int addrlen = sizeof(sockaddr_client);
	char* pathname = ((struct all_data*)arg)->pathname;
	
	while (1) {
		//发送选择菜单	
		write(sockfd, buf_send, strlen(buf_send));
		//等待用户回传的信息	
		read(sockfd, buf_receive, 2048);		
		
		//正常退出
		if ('0' == buf_receive[0])
			break;		
		//正常进入
		if ('1' == buf_receive[0]) {
			int type = buf_receive[0] - '1';
			//获取客户名字，问问客户的名字
			char buf_receive_clientname[2048] = {0};
			char buf_send2[] = "what's your name?\n";
			write(sockfd, buf_send2, strlen(buf_send2));
			read(sockfd, buf_receive_clientname, 2048);
			
			//创建新客户
			add_client(sockfd, buf_receive_clientname, type);
			//传过来的名字尾带回车，需要处理掉
			//因为即使名字不可能有回车字符
			char* buf = buf_receive_clientname;
			for (buf = buf_receive_clientname; *buf != '\0'; buf++)
				if (*buf == '\n') {
					*buf = '\0';
					break;
				}
			//创建或者打开客户专用文件，依赖程序当前路径			
			char filepath[2048] = {0};
			strcat(filepath, pathname);
			strcat(filepath, "/");
			strcat(filepath, buf_receive_clientname);
			strcat(filepath, ".txt\0");
				
			int fd = open(filepath, O_CREAT | O_RDWR | O_APPEND, 0666);	
			if (fd == -1) {
				perror("file open failed\n");
				return;				
			}	
			
			while (1) {
				//发送选择菜单
				write(sockfd, buf_send1, strlen(buf_send1));
				//等待用户回传的信息	
				read(sockfd, buf_receive, 2048);				
				
				//追加记录
				if ('1' == buf_receive[0]) {	
					char buf[1024] = {0};
					strcat(buf, "you wanna add record now\n \
we need sure about the end off adding option is input 0\n");
					write(sockfd, buf, strlen(buf));
					while (1) {								
						//等待用户发布信息
						ssize_t ret_read = read(sockfd, buf_receive, 2048);					
						//如果是退出
						if ('0' == buf_receive[0]) 
							break;
						//信息保存到文件
						write(fd, buf_receive, ret_read);					
						//信息转发到相同类型其他客户	
						multicast_msg(sockfd, type, buf_receive);					
					}										
				}
				
				//查看历史记录
				if ('2' == buf_receive[0]) {			
					//发送历史信息
					send_history(sockfd, fd);
					char buf[10] = {'\n', '\n', 0};
					write(fd, buf, strlen(buf));					
				}

				//正常退出
				if ('0' == buf_receive[0]) {
					//删除客户，关闭通讯
					remove_client(buf_receive_clientname);					
					close(fd);
					buf_receive[0] = 'q';
					//退出线程
					break;										
				}											
			}
		}
	}	
}


int main(int argc, char* argv[])
{
	sem_init(&sem, 0, 1);
	
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
	
	while (1) {		
		int ret_accept = accept(sockfd, (struct  sockaddr *)&sockaddr_client, &addrlen);
		if (ret_accept == -1) {
			perror("accept failed\n");
			return -1;			
		}
		
		if (ret_accept != -1) {	
			int pid = fork(); 		
            if(pid == 0) {					
				pthread_t pth_read_write;			
				struct all_data my_all_data;		
				char quit[1024] = "quit now";
				char pathname[1024] = {0};
				strcat(pathname, argv[2]);		
				
				my_all_data.socket_fd = ret_accept;		
				my_all_data.pathname = pathname;
				memcpy((void*)(&my_all_data.sockaddr_client), \
					(const void*) &sockaddr_client, sizeof(sockaddr_client));
						
				pthread_create(&pth_read_write, NULL, thread_read_write, \
					(void*)&my_all_data);  				
				pthread_join(pth_read_write, NULL);
				
				write(ret_accept, quit, sizeof(quit));
			} 
		}
		sleep(1);
	}
	
	sem_destroy(&sem);
	close(sockfd);
	return 0;
}