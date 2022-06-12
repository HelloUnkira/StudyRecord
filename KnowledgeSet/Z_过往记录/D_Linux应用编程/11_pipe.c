
/*
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void haha(int x)
{
	printf("I still alive\n");
	signal(2, SIG_DFL);
}

int main()
{
	signal(2, haha);
	return 0;
}

*/

/*

#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
	//创建PIPE管道实现通信、
	int pipefd[2] = {0};
	int ret = pipe(pipefd);
	if (ret == -1) {
		perror("pipe fail\n");	
		return -1;
	}
	//分裂，父进程向管道发送信息，子进程向管道接收信息
	ret = fork();
	if (ret == -1) {
		perror("fork fail\n");
		return -1;
	}
	//通过ret去判断是父进程还是子进程
	if (ret != 0) {
		//父进程				
		char cmd[100] = {0};
		while (1) {
			//read是阻塞的读取
			int count = read(pipefd[0], cmd, 100);
			printf("I am a old process\n");
			printf ("count = %d;str = %s", count, cmd);							
		}
		close(pipefd[0]);							
	} else {
		//子进程
		time_t tp;		
		while (1) {
			time(&tp);
			char* str = ctime(&tp);
			int count = write(pipefd[1], str, 100);
			printf("I am a new process\n");
			printf ("count = %d;str = %s", count, str);
			sleep(2);
		}
		close(pipefd[1]);
	}
	return 0;
}

*/





