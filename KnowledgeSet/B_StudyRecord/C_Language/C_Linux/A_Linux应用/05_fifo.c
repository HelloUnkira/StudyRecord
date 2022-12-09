
#define _GNU_SOURCE

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>	  
#include <stdio.h> 

void charge_way(int sig)
{
	printf("now is time to say goodbay\n");
	signal(sig, SIG_DFL);
}

int main(int argc, char* argv[])
{
	//创建管道文件入口（尝试）
	//system("mkfifo /home/my_fifo");
	//创建新进程并替换成其他进程
	int ret = fork();
	//让子进程播放信息
	if (ret == -1) {
		perror("fork fail\n");
		return -1;
	}	
	if (ret > 0) {
		//父进程
		printf("I am a old process\n");
		int ret1 = waitpid(ret, NULL, 0);
		if (ret1 == -1) {
			perror("wait fail\n");
			return -1;
		} else if (ret == ret1) {
			printf("all is done");			
		}
	} else {
		sleep(2);
		//子进程	
		printf("I am a new process\n");		
		printf("now , I wanna do something new!\n");
		signal(SIGINT, charge_way);	
		//int ret1 = execlp("mplayer","mplayer","-slave","-input","file=/mnt/hgfs/my_fifo", \
		//	"-quiet", "-zoom", "-x", "800", "-y", "480", "/mnt/hgfs/Share/1.mp4", NULL);	
		if (ret1 == -1) {
			perror("exec fail\n");
			return -1;
		}
	}			
	return 0;
}







