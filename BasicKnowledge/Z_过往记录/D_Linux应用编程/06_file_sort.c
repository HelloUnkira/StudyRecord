
#include <stdio.h>
#include <string.h>
#include "sort.h"

//�����ļ����򣬵����㷨������������

//ƥ��str1��β������str2��ͬ�ģ���չ����.����
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

//���ж��Ƿ������Լ����ݸ���
static int test_file_data_have_fault(FILE*fp)
{
	//����fp����֤ÿһ��ǰС�ں�
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

//�����ļ���ÿ��10��
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
	//��ʼ���壬��ʱ��֤�ļ�Ϊ.txt�����ļ�С��һ����Χ
	//�ɿ��ǲ��ö�ȡ���ڴ�������򣬹���ķ���
	//�ᵼ�²����������ļ�
	if ((offset_end - offset_start) <= zone_size) {
		//��λ��offset_start,��offset_start - offset_end��ȡ����
		//����,��������������
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
		//printf("�����˷���\n");
		//���ڵ��ļ��ܴ�,��Ҫ����
		//���ö����εķ�ʽ������Կ���,���������µ��ļ�
		int split_pot, split_pot_count = 0,data, i;
		int offset_center = offset_start;
		int ret_fp1,ret_fp2;
		FILE* fp1 = fopen("help1.txt", "w+");
		FILE* fp2 = fopen("help2.txt", "w+");
		//��ȡ��������Ϊ�ָ��,Ѱ����λ���е���λ��
		int offset = rand() % (offset_end - offset_start);
		fseek(fp, offset_start + offset, SEEK_SET);
		while (fgetc(fp) != ' ');
		fscanf(fp, "%d ", &split_pot);								
		//��ȡ����,��offset_start - offset_end
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

		//�رղ�ɾ���½�����ʱ�ļ�				
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
	//Ŀ���ļ�fp����������buf[1024]
	//����ΧΪ0 --- size - 1
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


