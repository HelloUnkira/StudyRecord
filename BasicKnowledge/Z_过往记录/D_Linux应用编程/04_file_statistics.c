#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>

//��������·��������ֻʵ����Ҫ�Ĳ���
struct list{
	char* data;
	struct list* prev, * next;
};

struct list* insert_node(struct list* list, const char* data)
{
	char *buf_str = (char *)malloc(sizeof(char) * 1024);
	strcpy(buf_str, data);
	struct list* buf_list = (struct list*)malloc(sizeof(struct list));
	buf_list->data = buf_str;
	buf_list->prev = NULL;
	buf_list->next = NULL;
	
	//ͷ�巨
	if (list != NULL) {
		struct list* help = list;
		help->prev = buf_list;
		buf_list->next = help;		
	}
	return buf_list;	
}

void delete_list(struct list* list)
{
	struct list* help = list;
	while (help != NULL) {
		list = list->next;		
		free(help->data);
		free(help);
		help = list;
	}
}

void print_list(struct list* list)
{
	while (list != NULL) {
		printf("%s\n", list->data);
		list = list->next;
	}		
}

//ƥ��str1��β������str2��ͬ�ģ���չ����.����
static int help_compare_extension(const char * str1, const char * str2)
{
	const char * temp1 = str1;
	const char * temp2 = str2;	
	while (*temp1 != '\0')
		temp1++;
	temp1--;
	while (*temp2 != '\0')
		temp2++;
	temp2--;
	
	while (temp1 >= str1 && temp2 >= str2 && \
		*temp2 != '.' && *temp1 != '.')
		if (*temp1 != *temp2)
			return 0;
		else {
			temp1--;
			temp2--;			
		}
		if (*temp1 == *temp2)
			return 1;
		return 0;
}

//flagΪ0��ͳ���ļ�������Ϊ1��ͳ���ļ���С�ܺ�
//flagΪ2��ͳ���ض��ļ��������Ѽ����·������������������·����
//���߾���·��������������Ǿ���·����
//�����·�������뵱ǰ���̵Ĺ���·���йأ����Կ���ʹ��getwd��ǰ��һ������
//ʹ�ô���ʱ�Ĳ���Ϊ����·�����򱣴��Ҳ���Ǿ���·��,���ѡ������ٷ�װһ��
unsigned int statistics_file_infomation(const char *pathname, int flag, struct list** mylist)
{
	struct stat st;
	int ret = stat(pathname,&st);
	//ʧ����
	if (ret == -1) {
		perror("stat fail\n");
		errno = 0;
		return 0;				
	} if (!S_ISDIR(st.st_mode)) {
		//�Ƿ�ΪĿ¼,����Ŀ¼Ĭ������ͨ�ļ�	
		if (flag == 0)
			return 1;				
		if (flag == 1)
			return st.st_size;
		//�޸Ĵ˴�str2�������ж����ȽԿ�
		if (flag == 2) {
			if (help_compare_extension(pathname, ".txt")  || \
				help_compare_extension(pathname, ".out")) {
				*mylist = insert_node(*mylist, pathname);				
				return 1;				
			}
			return 0;
		}
	}
	
	unsigned int all_number = 0;	
	DIR * buf = opendir(pathname);
	//��Ŀ¼ʧ��
	if (buf == NULL && errno != 0) {
		perror("dir open fail\n");
		errno = 0;
		return 0;		
	} else {
		//����������Ŀ¼����ÿ��Ŀ¼����з���
		while (1) {
			struct dirent * help = readdir(buf);
			//����ʧ�ܣ��޷�ʹ�ã�������һ��
			if (help == NULL && errno != 0) {
				perror("entry fail\n");
				errno = 0;				
			} else if (help == NULL && errno == 0)
			//Ŀ¼�Ѱ�����				
				break;
			else {
				//�ų�.��..
				if (!(0 == strcmp(help->d_name, ".") || \
					0 == strcmp(help->d_name, ".."))) {
					//������һ��·��
					char cmd[1024] = {0};
					strcat(cmd, pathname);
					strcat(cmd, "/");
					strcat(cmd, help->d_name);
					all_number += statistics_file_infomation(cmd, flag, mylist);													
				}								
			}						
		}		
	}
	closedir(buf);
	return all_number;
}

int main(int argc, char* argv[])
{
	struct list** help = NULL;
	struct list* help_buf = NULL;
	help = &help_buf;
	
	printf("%d\n", statistics_file_infomation("/mnt/hgfs/Share", 2, help));
	print_list(*help);
	delete_list(*help);
	
	return 0;
}

