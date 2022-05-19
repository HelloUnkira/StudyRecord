#ifndef __SORT_H_
#define __SORT_H_

#include<stdio.h>
#include<math.h>
#include<malloc.h>
#include<stdlib.h>

//模拟数组列表
typedef struct bucket{
	struct bucket * bk_next;
	int bk_number;
}bucket;

typedef struct radixBucket{
	struct radixBucket* next;
	int number;	
}radixBucket;

//使用规则
//bubble_sort(test,10,29);
//counting_sort(test,5,29);
//heap_sort(test,29);
//insertion_sort(test,5,29);
//quick_sort(test,5,29);
//radix_sort(test,7,29,2);
//selection_sort(test,6,29);
//shell_sort(test,7,29);
//bucket_sort(test,4,29);

void bubble_sort(int* list,int left,int right);
void selection_sort(int* list,int left,int right);	
void insertion_sort(int* list,int left,int right);
void shell_sort(int* list,int left,int right);
void merge_sort(int* list,int*temp,int left,int right);
void quick_sort(int* list,int left,int right)	;
void heap_sort(int* list,int n);
void counting_sort(int* list,int left,int right);
void bucket_sort(int* list,int left,int right);
int radix_sort(int* list,int left,int right,int index);

#endif
