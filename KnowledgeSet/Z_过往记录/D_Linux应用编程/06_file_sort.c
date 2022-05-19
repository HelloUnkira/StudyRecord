
#include <stdio.h>
#include <string.h>
#include "sort.h"

//尝试文件排序，但是算法容易陷入死区

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

//仅判断是否有序以及数据个数
static int test_file_data_have_fault(FILE*fp)
{
	//遍历fp，保证每一个前小于后
	int x = 0 , y = 0, data = 0, all_file_number = 0;
	fseek(fp, 0, SEEK_SET);
	while (!feof(fp)) {
		if (fscanf(fp, "%d ", &data) > 0) {
			x = y;
			y = data;
			if (x > y)
				printf("sort fail about: %d - %d\n", x, y);
			all_file_number++;
		}
	}
	return all_file_number;
}

//整理文件至每行10个
static int tidy_file(FILE* fp)
{
	char cmd[1024] = {0};
	int i = 0, data, ret, count = 1;	
	FILE* fpbuf = fopen("help.txt", "w+");
	int all_data = test_file_data_have_fault(fp);
	fseek(fp, 0, SEEK_SET);	
	while (!feof(fp)) {
		for (i = 0; i < 10; i++) 
			if (!feof(fp)) {
				if (fscanf(fp, "%d ", &data) > 0) 
					fprintf(fpbuf, "%d ", data);				
			} else
				break;
		fprintf(fpbuf, "\n");
	}
	
	fseek(fp, 0, SEEK_SET);
	fseek(fpbuf, 0, SEEK_SET);
	while((count = fread(cmd, sizeof(char), 1024, fpbuf)) > 0) 
		fwrite(cmd, sizeof(char), count, fp);
	
	ret = fclose(fpbuf);
	if (ret == -1) {
		perror("file close fail\n");
		return 0;				
	}				
	remove("help.txt");
	return all_data;		
}

static void patition_sort(FILE* fp, int offset_start, int offset_end, \
	int* buffer, int zone_size, int flag)
{
	//开始主体，此时保证文件为.txt，若文件小于一个范围
	//可考虑采用读取到内存进行排序，过多的分治
	//会导致产生大量的文件
	if ((offset_end - offset_start) <= zone_size) {
		//定位到offset_start,从offset_start - offset_end提取数据
		//排序,保存排序后的数据
		int start = 0, end = 0, data = 0, i;
		fseek(fp, offset_start, SEEK_SET);
		for (i = 0; (!feof(fp)) && (offset_start + i <= offset_end); i++)
			fscanf(fp, "%d ", &buffer[end++]);
		
		quick_sort(buffer, start, end - 1);	
		
	single_bucket_sort(buffer, start, end - 1);	
		fseek(fp, offset_start, SEEK_SET);	
		while ((start < end))
			fprintf(fp, "%d ", buffer[start++]);
	} else {
		//printf("出现了分治\n");
		//现在的文件很大,需要分治
		//采用二分治的方式治理可以考虑,创建俩个新的文件
		int split_pot, split_pot_count = 0,data, i;
		int offset_center = offset_start;
		int ret_fp1,ret_fp2;
		FILE* fp1 = fopen("help1.txt", "w+");
		FILE* fp2 = fopen("help2.txt", "w+");
		//则取三个数作为分割点,寻找三位数中的中位数
		int offset = rand() % (offset_end - offset_start);
		fseek(fp, offset_start + offset, SEEK_SET);
		while (fgetc(fp) != ' ');
		fscanf(fp, "%d ", &split_pot);								
		//提取数据,从offset_start - offset_end
		fseek(fp, offset_start, SEEK_SET);			
		for (i = 0; (!feof(fp)) && (offset_start + i <= offset_end); i++) {
			while(!feof(fp)) {
				fscanf(fp, "%d ", &data);
				if (data == split_pot)
					split_pot_count++;
				else if (data < split_pot) 
					fprintf(fp1, "%d ", data);
				else
					fprintf(fp2, "%d ", data);									

			}			
		}	
					
		fseek(fp, offset_start, SEEK_SET);			
		rewind(fp1);			
		rewind(fp2);		
		while (!feof(fp1))
			if (fscanf(fp1, "%d ", &data) > 0) 
				fprintf(fp, "%d ", data);					
		offset_center = ftell(fp);
		while (split_pot_count-- > 0)	
			fprintf(fp, "%d ", split_pot);
		while (!feof(fp2)) 
			if (fscanf(fp2, "%d ", &data) > 0) 
				fprintf(fp, "%d ", data);													

		//关闭并删除新建的临时文件				
		ret_fp1 = fclose(fp1);
		ret_fp2 = fclose(fp2);
		if (ret_fp1 == -1 || ret_fp2 == -1) {
			perror("file close fail\n");
			return;				
		}
		remove("help1.txt");
		remove("help2.txt");

		printf("--%d\n", split_pot);	
		printf("%d %d %d--\n", offset_start, offset_center, offset_end);
		patition_sort(fp, offset_start, offset_center, \
			buffer, zone_size, flag);
		patition_sort(fp, offset_center, offset_end, \
			buffer, zone_size, flag);
	}
}

void file_sort(char * pathname)
{
	if (!help_compare_extension(pathname, ".txt"))
		printf("can't accept this file type");	
	FILE* fp = fopen(pathname, "r+");
	int buf[1024] = {0};	
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	//目标文件fp，辅助数组buf[1024]
	//排序范围为0 --- size - 1
	patition_sort(fp, 0, size - 1, buf, 1024, 0);	
	int ret = fclose(fp);
	if (ret == -1) {
		perror("file close fail\n");
		return;				
	}		
}


int main(int argc, char* argv[])
{
	int i = 0, k = 1;
	
	FILE* fp = fopen(argv[1], "w+");
	scanf("%d", &k);
	for (i = 0; i < k; i++)
		fprintf(fp, "%d ", rand()%10000);
	int ret = fclose(fp);
	if (ret == -1) 
		perror("file close fail\n");		
									
	file_sort(argv[1]);
	
	fp = fopen(argv[1], "r+");	
	printf("%d\n", tidy_file(fp));	

			

	return 0;
}


