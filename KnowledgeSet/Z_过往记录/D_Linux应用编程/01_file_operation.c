#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main()
{
	//实现文件1到文件2的复制
	char pathname1[] = "/mnt/hgfs/Share/Linux应用编程/test1.txt";
	char pathname2[] = "/mnt/hgfs/Share/Linux应用编程/test2.txt";
	int fp1 = open("test1.txt", O_RDONLY);
	if (fp1 == -1)
		printf("open test1 fail: %d",errno);	
	
	int fp2 = open("test2.txt", O_RDWR  | O_CREAT | O_TRUNC, 0664);
	if (fp2 == -1)
		printf("open test2 fail: %d",errno);	
	
	if (fp1 == -1 || fp2 == -1) {			
		//return -1;	
	}

	
	int cur = 0;	
	unsigned char buf[100] = {0};
	do {
		cur = read(fp1, buf, 100);
		write(fp2, (const void *)buf, cur);
	} while (cur == 100);
		
	fp1 = close(fp1);
	fp2 = close(fp2);
	if (fp1 == -1 || fp2 == -1)
		return -1;	
	
	return 0;
	
	
}

