#include <sys/types.h>
#include <unistd.h>	
#include <sys/wait.h>	   
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
	key_t kt_shm = ftok(argv[1], 1000);
	if (kt_shm == -1) {
		perror("ftok fail\n");
		return -1;
	}	
	
	int ret_shmget = shmget(kt_shm, 1024, IPC_CREAT | 0664);
	if (ret_shmget == -1) {
		perror("shmget fail\n");
		return -1;
	}
	
	int * ret_shmat = (int *)shmat(ret_shmget, NULL, 0);
	if (ret_shmat == NULL) {
		perror("shmat fail\n");
		return -1;				
	}	
	(*ret_shmat) = 0;	
	
	key_t kt_shm1= ftok(argv[2], 1000);
	if (kt_shm1 == -1) {
		perror("ftok fail\n");
		return -1;
	}	
	
	int ret_shmget1 = shmget(kt_shm1,4096,IPC_CREAT | 0664);
	if (ret_shmget1 == -1) {
		perror("shmget fail\n");
		return -1;
	} 
	
	sem_t * ret_shmat1 = (sem_t *)shmat(ret_shmget1, NULL, 0);
	if (ret_shmat1 == NULL) {
		perror("shmat fail\n");
		return -1;				
	}	
	
	int ret_sem_init = sem_init(ret_shmat1, 1, 1);
	if (ret_sem_init == -1) {
		perror("sem_init fail\n");
		return -1;
	}
	
	int ret_fork = fork();
	if (ret_fork == -1) {
		perror("fork fail\n");
		return -1;
	}
	
	if (ret_fork > 0) {
		//父进程					
		int ret_sem_wait = sem_wait(ret_shmat1);
		if (ret_sem_wait == -1)  {
			perror("sem_wait fail\n");
			return -1;
		}
		
		printf("I am the old process\n");
		printf("now,let's start!\n");
		int i = 1000000;
		while (i-- > 0)			
			(*ret_shmat)++;	
		printf("oh well, I finish the job my need do\n");
		
		int ret_sem_post = sem_post(ret_shmat1);
		if (ret_sem_post == -1) {
			perror("sem_post fail\n");
			return -1;					
		}
		
		int ret_waitpid = waitpid(ret_fork, NULL, 0);
		if (ret_waitpid == -1) {
			perror("waitpid fail\n");
			return -1;
		}			
		
		int ret_sem_destory = sem_destroy(ret_shmat1);
		if (ret_sem_destory == -1) {
			perror("sem_destroy fail\n");
			return -1;
		}
		
		int ret_shmctl = shmctl(ret_shmget, IPC_RMID, NULL);
		if (ret_shmctl == -1) {
			perror("shmctl fail\n");
			return -1;
		}		
		
		int ret_shmdt = shmdt(ret_shmat);
		if (ret_shmdt == -1) {
			perror("shmdt fail\n");
			return -1;
		}

		int ret_shmctl1 = shmctl(ret_shmget1, IPC_RMID, NULL);
		if (ret_shmctl1 == -1) {
			perror("shmctl fail\n");
			return -1;
		}		
		
		int ret_shmdt1 = shmdt(ret_shmat1);
		if (ret_shmdt1 == -1) {
			perror("shmdt fail\n");
			return -1;
		}
		
	} else if (ret_fork == 0) {
		//子进程		
		int ret_sem_wait = sem_wait(ret_shmat1);
		if (ret_sem_wait == -1)  {
			perror("sem_wait fail\n");
			return -1;
		}		
		
		printf("I am the new process\n");
		printf("now,let's start!\n");
		int i = 1000000;		
		while (i-- > 0)
			(*ret_shmat)++;		
		printf("oh well, I finish the job my need do\n");
		
		int ret_sem_post = sem_post(ret_shmat1);
		if (ret_sem_post == -1) {
			perror("sem_post fail\n");
			return -1;					
		}
		
		printf("%d\n", *ret_shmat);		
		
	}		
			
	return 0;
}

/*
//POSIX有名信号量
#include <sys/types.h>
#include <unistd.h>	
#include <sys/wait.h>	   
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#include <stdio.h>
#include <string.h>


int main(int argc, char* argv[])
{
	key_t kt_shm = ftok(argv[1], 1000);
	if (kt_shm == -1) {
		perror("ftok fail\n");
		return -1;
	}	
	
	int ret_shmget = shmget(kt_shm, 1024, IPC_CREAT | 0664);
	if (ret_shmget == -1) {
		perror("shmget fail\n");
		return -1;
	}
	
	int * ret_shmat = (int *)shmat(ret_shmget, NULL, 0);
	if (ret_shmat == NULL) {
		perror("shmat fail\n");
		return -1;				
	}	
	(*ret_shmat) = 0;	

	sem_t * ret_sem_open = sem_open(argv[2], O_CREAT, 0664, 1);
	if (ret_sem_open == SEM_FAILED) {
		perror("sem_open fail\n");
		return -1;
	}
	
	int ret_fork = fork();
	if (ret_fork == -1) {
		perror("fork fail\n");
		return -1;
	}
	
	if (ret_fork > 0) {
		//父进程					
		int ret_sem_wait = sem_wait(ret_sem_open);
		if (ret_sem_wait == -1)  {
			perror("sem_wait fail\n");
			return -1;
		}
		
		printf("I am the old process\n");
		printf("now,let's start!\n");
		int i = 1000000;
		while (i-- > 0)			
			(*ret_shmat)++;	
		printf("oh well, I finish the job my need do\n");
		
		int ret_sem_post = sem_post(ret_sem_open);
		if (ret_sem_post == -1) {
			perror("sem_post fail\n");
			return -1;					
		}
		
		int ret_waitpid = waitpid(ret_fork, NULL, 0);
		if (ret_waitpid == -1) {
			perror("waitpid fail\n");
			return -1;
		}			
		
		int ret_sem_close = sem_close(ret_sem_open);
		if (ret_sem_close == -1) {
			perror("sem_close fail\n");
			return -1;						
		}
		
		int ret_sem_unlink = sem_unlink(argv[2]);
		if (ret_sem_unlink == -1) {
			perror("sem_unlink fail\n");
			return -1;
		}
		
		int ret_shmctl = shmctl(ret_shmget, IPC_RMID, NULL);
		if (ret_shmctl == -1) {
			perror("shmctl fail\n");
			return -1;
		}		
		
		int ret_shmdt = shmdt(ret_shmat);
		if (ret_shmdt == -1) {
			perror("shmdt fail\n");
			return -1;
		}
			
	} else if (ret_fork == 0) {
		//子进程		
		int ret_sem_wait = sem_wait(ret_sem_open);
		if (ret_sem_wait == -1)  {
			perror("sem_wait fail\n");
			return -1;
		}		
		
		printf("I am the new process\n");
		printf("now,let's start!\n");
		int i = 1000000;		
		while (i-- > 0)
			(*ret_shmat)++;		
		printf("oh well, I finish the job my need do\n");
		
		int ret_sem_post = sem_post(ret_sem_open);
		if (ret_sem_post == -1) {
			perror("sem_post fail\n");
			return -1;					
		}		
		
		printf("%d\n", *ret_shmat);		
		
	}		
			
	return 0;
}

*/

