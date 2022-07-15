#include <sys/types.h>
#include <unistd.h>	
#include <sys/wait.h>	   
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>

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

	key_t kt_sem = ftok(argv[2], 1000);
	if (kt_sem == -1) {
		perror("ftok fail\n");
		return -1;
	}
	
	int ret_semget = semget(kt_sem, 5, IPC_CREAT | 0664);
	if (ret_semget == -1) {
		perror("semget fail\n");
		return -1;
	}
	
	int ret_semctl = semctl(ret_semget, 0, SETVAL, 1);
	if (ret_semctl == -1) {
		perror("semctl fail\n");
		return -1;		
	}
	
	int ret_fork = fork();
	if (ret_fork == -1) {
		perror("fork fail\n");
		return -1;
	}
	
	if (ret_fork > 0) {
		//������				
		struct sembuf sb = { 0, 1, IPC_NOWAIT};
		int ret_semop = semop(ret_semget, &sb, 1);
		if (ret_semop == -1) {
			perror("semop1 fail\n");
			return -1;		
		}
		
		printf("I am the old process\n");
		printf("now,let's start!\n");
		int i = 1000000;
		while (i-- > 0)			
			(*ret_shmat)++;	
		printf("oh well, I finish the job my need do\n");
		
		struct sembuf sb1 = { 0, -1, IPC_NOWAIT};
		ret_semop = semop(ret_semget, &sb1, 1);
		if (ret_semop == -1) {
			perror("semop2 fail\n");
			return -1;		
		}		
		
		int ret_waitpid = waitpid(ret_fork, NULL, 0);
		if (ret_waitpid == -1) {
			perror("waitpid fail\n");
			return -1;
		}			
			
		int ret_semctl = semctl(ret_semget, 0, IPC_RMID);
		if (ret_semctl == -1) {
			perror("semctl fail\n");
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
		//�ӽ���		
		struct sembuf sb = { 0, 1, IPC_NOWAIT};
		int ret_semop = semop(ret_semget, &sb, 1);
		if (ret_semop == -1) {
			perror("semop1 fail\n");
			return -1;		
		}		
		
		printf("I am the new process\n");
		printf("now,let's start!\n");
		int i = 1000000;		
		while (i-- > 0)
			(*ret_shmat)++;		
		printf("oh well, I finish the job my need do\n");
		
		struct sembuf sb1 = { 0, -1, IPC_NOWAIT};
		ret_semop = semop(ret_semget, &sb1, 1);
		if (ret_semop == -1) {
			perror("semop2 fail\n");
			return -1;		
		}			
		
		printf("%d\n", *ret_shmat);		
		
	}		
	
	return 0;
}




/*
//��Ϣ����
#include <sys/types.h>
#include <unistd.h>	
#include <sys/wait.h>	   
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>

struct msgbuf {
   long mtype;       // message type, must be > 0 
   char mtext[1024];    // message data 
};

int main(int argc, char* argv[])
{
	key_t kt = ftok(argv[1], 1000);
	if (kt == -1) {
		perror("ftok fail\n");
		return -1;
	}
	
	int ret_msgget = msgget(kt, IPC_CREAT | 0664);
	if (ret_msgget == -1) {
		perror("msgget fail\n");
		return -1;
	}
	
	//msgctl(msqid,IPC_STAT,&buf);
	//����buf�е�ĳЩ����
	//msgctl(msqid,IPC_SET,&buf);
	//IPC_STAT 	������ȡ��Ϣ���е�����(ͷ�����Ϣ),�洢��bufָ��Ŀռ���ȥ
	//IPC_SET 	����������Ϣ���е�����(ͷ���),Ҫ���õ����Դ洢��bufָ��Ľṹ����
	
	int ret_fork = fork();
	if (ret_fork == -1) {
		perror("fork fail\n");
		return -1;
	}
	
	if (ret_fork > 0) {
		//������
		struct msgbuf buf;
		buf.mtype = 100;
		strncpy(buf.mtext,argv[2],strlen(argv[2]) + 1);
		int ret_msgsnd = msgsnd(ret_msgget, (const void *)&buf, strlen(argv[2]) + 1, 0);	
		if (ret_msgsnd == -1) {
			perror("msgsnd fail\n");
			return -1;
		}		
		int ret_waitpid = waitpid(ret_fork, NULL, 0);
		if (ret_waitpid == -1) {
			perror("waitpid fail\n");
			return -1;
		}		
	} else if (ret_fork == 0) {
		//�ӽ���
		struct msgbuf buf;
		memset(&buf, 0, sizeof(buf));		
		ssize_t ret_msgrcv = msgrcv(ret_msgget, (void *)&buf, 1024, 0, 0);	
		if (ret_msgrcv == -1) {
			perror("msgrcv error");
			return -1;
		}
		printf("how many bytes?\n : = %d\n",ret_msgrcv);
		printf("message type:%ld\n",buf.mtype);
		printf("message data:%s\n",buf.mtext);
	}	
	return 0;
}

*/
