#include"sort.h"
#include<stdio.h>

int main()
{
	int test[10] = {0};
	int i = 0;
	for (i = 0; i < 10; i++) 
		test[i] = rand()%100; 

	//bubble_sort(test,0,9);
	//counting_sort(test,5,29);
	//heap_sort(test,29);
	//insertion_sort(test,5,29);
	//quick_sort(test,5,29);
	//radix_sort(test,7,29,2);
	//selection_sort(test,6,29);
	//shell_sort(test,7,29);
	bucket_sort(test,0,9);
	
	for (i = 0; i < 10; i++) 
		printf("%d ", test[i]);  
	
	return 0;
}







