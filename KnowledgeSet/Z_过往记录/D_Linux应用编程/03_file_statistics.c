#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<fcntl.h>


struct stat buf;

int main()
{
	
	int ret = stat("./a.out", &buf);
	if (ret == -1) {
		printf("option false\n");
	}
	int fp1 = open("/mnt/hgfs/Share/Linux应用编程/1.txt", \
		O_RDWR | O_CREAT | O_TRUNC, 0664);
	if (ret == -1) {
		printf("open false\n");
	}		
		
	char cmd[1024] = {0};
	int num = sprintf(cmd, "设备号：%lu\n Inode号：%lu\n 权限位信息：%lu\n 硬链接数量%lu\n\
				所有者id：%lu\n 组id：%lu\n 设备号：%lu\n 文件内容：%lu\n ", \
				(unsigned long)buf.st_dev, (unsigned long)buf.st_ino, (unsigned long)buf.st_mode, \
				(unsigned long)buf.st_nlink, (unsigned long)buf.st_uid, (unsigned long)buf.st_gid, \
				(unsigned long)buf.st_rdev, (unsigned long)buf.st_size);
	
	printf("%s", cmd);
	
	write(fp1, cmd, num + 1);
	
	close(fp1);
	
	return 0;
}




