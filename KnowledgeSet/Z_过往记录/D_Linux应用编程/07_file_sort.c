#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sort.h"

//�����ļ����򣬵��㷨Ч�ʵ���
//���㷨�߼��Ͽ��У�����Ϊ��ȡ�ָ����ѡȡ����������ѭ��
//�����ʹ�ÿ��ŷ��β��ɿ�

static int search_middle_number(int num1, int num2, int num3)
{
	if (num1 == num2)
			return num3;
	else if (num1 == num3)
			return num2;
	else if (num2 == num3)
			return num1;		
	else if ((num1 > num2 && num1 < num3) || \
		(num1 > num3 && num1 < num2))
		return num1;
	else if ((num2 > num1 && num2 < num3) || \
		(num2 > num3 && num2 < num1))
		return num2;
	else	
		return num3;
}

static void insert_char_to_str(char* pathname, char x)
{
	char* p = pathname;
	while(*p != '\0')
		p++;
	*(p + 1) = '\0';
	*(p) = *(p - 1);
	*(p - 1) = *(p - 2);
	*(p - 2) = *(p - 3);
	*(p - 3) = *(p - 4);
	*(p - 4) = x;
}

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

static void patition_sort(char * pathname, int offset_start, int offset_end, int flag, int min_size)
{
	if (!help_compare_extension(pathname, ".txt"))
		printf("can't accept this file type");
	else {
		int size = offset_end - offset_start;
		//��ʼ���壬��ʱ��֤�ļ�Ϊ.txt
		if (size <= min_size) {
			//���ļ�С�ڴˣ��ɿ��ǲ��ö�ȡ���ڴ�������򣬹���ķ���
			//�ᵼ�²����������ļ�
			int start = 0, end = 0, data = 0, count = 0, i;
			int ret_fp, ret_before, ret_after;
			int* buffer = (int*)malloc(sizeof(int) * min_size);
			//ֻ���򿪣���λ��offset_start
			FILE* fp = fopen(pathname, "r");
			FILE* fp_before = fopen("help1.txt", "w+");
			FILE* fp_after = fopen("help2.txt", "w+");	
			//��������
			while ((!feof(fp)) && (ftell(fp) < offset_start)) {
				fscanf(fp, "%d ", &data);				
				fprintf(fp_before, "%d ", data);
			}							
			//��ȡ����,��offset_start - offset_end
			for (i = 0; (!feof(fp)) && (offset_start + i <= offset_end); i++) {
				fscanf(fp, "%d ", &data);
				buffer[end++] = data;
			}
			//����	
			quick_sort(buffer, start, end - 1);			
			//��������
			while (!feof(fp)) {
				fscanf(fp, "%d ", &data);				
				fprintf(fp_after, "%d ", data);
			}			

			ret_fp = fclose(fp);
			if (ret_fp == -1) {
				perror("file close fail\n");
				return;
			}
			
			fp = fopen(pathname, "w");			
			fseek(fp_before, 0 , SEEK_SET);
			fseek(fp_after, 0 , SEEK_SET);
			while (!feof(fp_before)) {
				int count = fscanf(fp_before, "%d ", &data);	
				if (count > 0)
					fprintf(fp, "%d ", data);
			}					
			while ((start < end))
				fprintf(fp, "%d ", buffer[start++]);							
			while (!feof(fp_after)) {			
				int count = fscanf(fp_after, "%d ", &data);
				if (count > 0)
					fprintf(fp, "%d ", data);
			}		
			//�ر��ļ�
			ret_fp = fclose(fp);
			ret_before = fclose(fp_before);
			ret_after = fclose(fp_after);
			if (ret_fp == -1 || ret_before == -1 || ret_after == -1) {
				perror("file close fail\n");
				return;				
			}	
			free(buffer);
			remove("help1.txt");
			remove("help2.txt");
		} else {
			//printf("�����˷���\n");
			//���ڵ��ļ��ܴ���Ҫ����
			//���ö����εķ�ʽ������Կ��ǣ�Ĭ�����ݾ��������
			//��ȡǰ��������Ϊ�ָ��
			//���������µ��ļ��������ϸ����ļ���СӦ����ԭ�ļ�һ��
			//����������Χ�ļ��������ļ�֮ǰ��֮���ֵ
			
			//������Ҫ��offset_start - offset_end ���н�һ����С
			FILE* fp = fopen(pathname, "r");
			FILE* fp1 = fopen("fp1.txt", "w+");
			FILE* fp2 = fopen("fp2.txt", "w+");
			FILE* fp_before = fopen("help1.txt", "w+");
			FILE* fp_after = fopen("help2.txt", "w+");	
			int num1, num2, num3, split_pot, data, i;
			int ret_fp, ret_fp1, ret_fp2, ret_before, ret_after;	
			int offset_center = offset_start;
			
			//Ѱ����λ���е���λ��
			int offset = 5;
			char x = 0;
			if (flag == 1) {
				offset += rand()%50;				
				flag = 0;
			}			
			fseek(fp, offset_start, SEEK_SET);
			fscanf(fp, "%d ", &num1);	
			fseek(fp, (offset_start + offset_end) / 2 + offset, SEEK_SET);

			while ((x = fgetc(fp)) != ' ');
			fscanf(fp, "%d ", &num2);
			fseek(fp, offset_end - offset, SEEK_SET);
			while ((x = fgetc(fp)) != ' ');
			fscanf(fp, "%d ", &num3);		
			split_pot = search_middle_number(num1, num2, num3);
			//printf("--%d %d %d --%d\n", num1, num2, num3, split_pot);		
			
			//��������
			rewind(fp);
			while ((!feof(fp)) && (ftell(fp) < offset_start)) {
				fscanf(fp, "%d ", &data);				
				fprintf(fp_before, "%d ", data);
			}									
			//��ȡ����,��offset_start - offset_end			
			for (i = 0; (!feof(fp)) && (offset_start + i <= offset_end); i++) {
				//��ʼ�֣�ÿ�ζ�ȡһ����д��
				while(!feof(fp)) {
					fscanf(fp, "%d ", &data);
					if (data < split_pot) 
						fprintf(fp1, "%d ", data);
					else 
						fprintf(fp2, "%d ", data);									
				}			
			}	
			//��������
			while (!feof(fp)) {
				fscanf(fp, "%d ", &data);				
				fprintf(fp_after, "%d ", data);
			}			
			ret_fp = fclose(fp);
			if (ret_fp == -1) {
				perror("file close fail\n");
				return;				
			}			
			fp = fopen(pathname, "w");	
			//�ض�fp��������ƴ��fp
			rewind(fp_before);
			while (!feof(fp_before)) {
				int count = fscanf(fp_before, "%d ", &data);	
				if (count > 0)
					fprintf(fp, "%d ", data);
			}			
			rewind(fp1);			
			while (!feof(fp1)) {
				int count = fscanf(fp1, "%d ", &data);	
				if (count > 0) {
					fprintf(fp, "%d ", data);					
					if (data == split_pot)
						offset_center = ftell(fp);
				}
			}

			rewind(fp2);			
			while (!feof(fp2)) {
				int count = fscanf(fp2, "%d ", &data);	
				if (count > 0) {
					fprintf(fp, "%d ", data);					
					if (data == split_pot)
						offset_center = ftell(fp);					
				}
			}			
			rewind(fp_after);			
			while (!feof(fp_after)) {			
				int count = fscanf(fp_after, "%d ", &data);
				if (count > 0)
					fprintf(fp, "%d ", data);
			}						
			ret_fp = fclose(fp);
			ret_fp1 = fclose(fp1);
			ret_fp2 = fclose(fp2);
			ret_before = fclose(fp_before);
			ret_after = fclose(fp_after);
			if (ret_fp == -1 ||ret_fp1 == -1 || ret_fp2 == -1 || \
				ret_before == -1 ||ret_after == -1) {
				perror("file close fail\n");
				return;				
			}
			//ɾ���½�����ʱ�ļ�
			remove("fp1.txt");
			remove("fp2.txt");	
			remove("help1.txt");
			remove("help2.txt");
			
			printf("%d %d %d--\n", offset_start, offset_center, offset_end);
			if (offset_start == offset_center)
				flag = 1;
			if (offset_center == offset_end)
				flag = 2;
			patition_sort(pathname, offset_start, offset_center, flag, min_size);
			patition_sort(pathname, offset_center, offset_end, flag, min_size);
		}	
	}						
}

void file_sort(char * pathname)
{
	FILE * fp = fopen(pathname, "r");
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	int ret = fclose(fp);
	if (ret == -1) {
		perror("file close fail\n");
		return;				
	}
	
	patition_sort(pathname, 0, size - 1, 0, 1024);	
	
}

int tidy_file(char* pathname)
{
	if (!help_compare_extension(pathname, ".txt")) {
		printf("can't accept this file type");		
		return 0;
	} else {
		int ret, ret1, count, all_data = 0;
		char buf[1024] = {0};		
		FILE* fp = fopen(pathname, "r");
		FILE* fpbuf = fopen("help.txt", "w+");
		//
		while (!feof(fp)) {
			int i = 0, data;
			for (i = 0; i < 10; i++) {
				if (!feof(fp)) {
					fscanf(fp, "%d ", &data);
					fprintf(fpbuf, "%d ", data);	
					all_data++;					
				}						
			}
			fprintf(fpbuf, "\n");
		}
		ret = fclose(fp);
		if (ret == -1) {
			perror("file close fail\n");
			return;				
		}	
		fp = fopen(pathname, "w");
		rewind(fpbuf);
		while(count = fread(buf,sizeof(char), 1024, fpbuf)) 
			fwrite(buf,sizeof(char), count, fp);
		
		ret = fclose(fp);
		ret1 = fclose(fpbuf);
		if (ret == -1 || ret1 == -1) {
			perror("file close fail\n");
			return;				
		}				
		remove("help.txt");
		return all_data;		
	}
}


int main(int argc, char* argv[])
{
	FILE* fp = fopen(argv[1], "w");
	int i = 0;
	for (i = 0; i < 1000; i++)
		fprintf(fp, "%d ", rand()%1000000);
	int ret = fclose(fp);
	if (ret == -1) 
		perror("file close fail\n");		
							
	file_sort(argv[1]);
	printf("%d", tidy_file(argv[1]));
	
	return 0;
}

