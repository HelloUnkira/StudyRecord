#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>

//辅助保存路径，所以只实现需要的部分
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
	
	//头插法
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

//匹配str1的尾部中与str2相同的，扩展名以.结束
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

//flag为0，统计文件数量，为1，统计文件大小总和
//flag为2，统计特定文件数量，已及相对路径（若传入参数是相对路径）
//或者绝对路径（若传入参数是绝对路径）
//而相对路径本身与当前进程的工作路径有关，所以可以使用getwd提前做一步处理
//使得传入时的参数为绝对路径，则保存的也会是绝对路径,最好选择外层再封装一次
unsigned int statistics_file_infomation(const char *pathname, int flag, struct list** mylist)
{
	struct stat st;
	int ret = stat(pathname,&st);
	//失败了
	if (ret == -1) {
		perror("stat fail\n");
		errno = 0;
		return 0;				
	} if (!S_ISDIR(st.st_mode)) {
		//是否为目录,不是目录默认是普通文件	
		if (flag == 0)
			return 1;				
		if (flag == 1)
			return st.st_size;
		//修改此处str2，增添判断语句等皆可
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
	//打开目录失败
	if (buf == NULL && errno != 0) {
		perror("dir open fail\n");
		errno = 0;
		return 0;		
	} else {
		//遍历整个该目录，对每个目录项进行分析
		while (1) {
			struct dirent * help = readdir(buf);
			//该项失败，无法使用，跳到下一个
			if (help == NULL && errno != 0) {
				perror("entry fail\n");
				errno = 0;				
			} else if (help == NULL && errno == 0)
			//目录项被寻找完成				
				break;
			else {
				//排除.和..
				if (!(0 == strcmp(help->d_name, ".") || \
					0 == strcmp(help->d_name, ".."))) {
					//构建下一级路径
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

