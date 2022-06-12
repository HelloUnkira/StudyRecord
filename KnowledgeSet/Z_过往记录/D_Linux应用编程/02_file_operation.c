#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<math.h>

int main()
{
	//实现文件1
	char pathname1[] = "/dev/fb0";
	int fp1 = open(pathname1, O_RDWR);
	if (fp1 == -1)
		printf("open test1 fail: %d",errno);
	
	int i, j ,k;
	int all = 480*800*4; 
	int center_row = 480 / 2;
	int center_col = 800 / 2;
	unsigned int buf[480][800] = {0};
	//装填buf为一个圆
	for (i = 0; i < 480; i++)
		for (j = 0; j < 800; j++)
			if (fabs((double)(i - center_row)) * fabs((double)(i - center_row)) + \
				fabs((double)(j - center_col)) * fabs((double)(j - center_col)) <= \
				200.0 * 200.0)
					buf[i][j] = 0x00ff0000;
			else
					buf[i][j] = 0x000000ff;

	lseek(fp1, 0, SEEK_SET);	
	write(fp1, buf, all);
		
	sleep(2);
			
	//装填buf为一个彩虹	
	int color[7] = {0x00ff0000, 0x00ff7d00, 0x00ffff00, 0x0000ff00, \
							0x0000ffff, 0x000000ff, 0x00ff00ff};
	int length = 450;
	center_row = 480 - 1;
	center_col = 800 / 2;
	//从最大范围开始装填，半径依次缩小50
	for (k = 6; k >= 0; k--, length -= 50)
		for (i = 0; i < 480; i++)
			for (j = 0; j < 800; j++)
				if (fabs((double)(i - center_row)) * fabs((double)(i - center_row)) + \
					fabs((double)(j - center_col)) * fabs((double)(j - center_col)) <= \
					(double)length * (double)length)
					buf[i][j] = color[k];

	lseek(fp1, 0, SEEK_SET);	
	write(fp1, buf, all);

	
	fp1 = close(fp1);
	if (fp1 == -1)
		return -1;	
	
	return 0;
	
	
}

