#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/types.h>
#include <dirent.h>

struct help_pathname{
	char* cmd1;
	char* cmd2;
};

void copy_file(char* pathname1, char* pathname2)
{
	//��pathname1·�����ļ����Ƶ�·��pathname2��
	int fd1 = open(pathname1, O_RDWR);
	int fd2 = open(pathname2, O_CREAT | O_RDWR | O_TRUNC, 0664);
	if (fd1 == -1 || fd2 == -1) {
		perror("open file failed\n");
		errno = 0;
		return;
	}
	
	char buf1[1024] = {0};
	int count = 0;
	while((count = read(fd1, buf1, 1024)) > 0)
		write(fd2, buf1, count);
	
	if (close(fd1) == -1 || close(fd2) == -1) {
		perror("close file failed\n");
		errno = 0;
	}
	
	//Ϊ�˸��߳�ʹ�ã����δ˾�
	free(pathname1);
	free(pathname2);
}

void* thread_copy_file(void* arg)
{
	copy_file(((struct help_pathname*)arg)->cmd1, \
		((struct help_pathname*) arg)->cmd2);
	free(arg);
	int data;
	pthread_exit(&data);
}

void copy_dir(char* pathname1, char* pathname2)
{
	DIR* fd1 = opendir(pathname1);	
	//�����ļ���,����ʧ�����,�����Դ���
	mkdir(pathname2, 0664);
	DIR* fd2 = opendir(pathname2);	
	//������Ŀ���ļ��ж��ܴ򿪲���
	if (fd1 == NULL || fd2 == NULL) {
		perror("open dir failed\n");
		printf("open dir %d - %d failed\n", fd1 == NULL , fd2 == NULL);
		errno = 0;
		return;
	}		
	
	//��ȡĿ¼��1 ���俽����Ŀ¼��2��
	struct dirent * rd1 = NULL;
	while (1) {
		rd1 = readdir(fd1);
		if (rd1 == NULL && errno == 0)
			break;
		else if (rd1 == NULL && errno != 0) {
			perror("read dir failed\n");
			errno = 0;		
			continue;
		} else if(strcmp(rd1->d_name, ".") == 0 || \
			strcmp(rd1->d_name, "..") == 0)
			continue;
		else{
			int count = 0;
			char* buf = pathname1;
			while(*buf++ != '\0')
				count++;
			buf = rd1->d_name;
			while(*buf++ != '\0')
				count++;
			//�����㹻��С��·������Ϊ����Ҫ����һ��pathname1/filename�ļ�
			char* cmd1 = (char*)calloc((count + 5) , sizeof(char));
			cmd1[0] = '\0';
			strcat(cmd1, pathname1);
			strcat(cmd1, "/");
			strcat(cmd1, rd1->d_name);			
			
			count = 0;
			buf = pathname2;
			while(*buf++ != '\0')
				count++;
			buf = rd1->d_name;
			while(*buf++ != '\0')
				count++;					
			//�����㹻��С��·������Ϊ����Ҫ����һ��pathname2/filename�ļ�
			char* cmd2 = (char*)calloc((count + 5) , sizeof(char));	
			cmd2[0] = '\0';			
			strcat(cmd2, pathname2);
			strcat(cmd2, "/");
			strcat(cmd2, rd1->d_name);						
			
			struct stat st;
			stat(cmd1,&st);			
			
			if (!S_ISDIR(st.st_mode)) {
				//���Ŀ¼�����ļ��Ļ�					
				//copy_file(cmd1, cmd2);			
				
				//����һ���߳�ȥ��
				pthread_t pid;
				struct help_pathname* arg = \
					(struct help_pathname*)malloc(sizeof(struct help_pathname) * 2);
				arg->cmd1 = cmd1;
				arg->cmd2 = cmd2;
				if (pthread_create(&pid, NULL, \
					thread_copy_file, (void *) arg) == -1) {
						perror("create thread failed\n");
						errno = 0;	
						copy_dir(cmd1, cmd2);
						free(arg);
				}							
				if (pthread_join(pid, NULL) != 0) {
					errno = 0;	
				}	
				
				
			} else {
				//�����Ŀ¼����Ŀ¼�Ļ����ݹ���ô�
				copy_dir(cmd1, cmd2);
				free(cmd1);
				free(cmd2);								
			}			
		}	
	}
	if (closedir(fd1) == -1 || closedir(fd2) == -1) {
		perror("close dir failed\n");
		errno = 0;
		return;
	}
}

int main(int argc, char* argv[])
{
	//�趨argv[1]ΪԴ�ص�
	//�趨argv[2]ΪĿ��ص�
	
	/*
	DIR* fd1 = opendir(argv[1]);	
	//�����ļ���,����ʧ�����,�����Դ���
	mkdir(argv[2], 0664);
	DIR* fd2 = opendir(argv[2]);	
	//������Ŀ���ļ��ж��ܴ򿪲���
	if (fd1 == -1 || fd2 == -1) {
		perror("open dir failed\n");
		return -1;
	}	
	*/
	
	copy_dir(argv[1], argv[2]);
	//copy_file(argv[1], argv[2]);
	
	return 0;
}



















