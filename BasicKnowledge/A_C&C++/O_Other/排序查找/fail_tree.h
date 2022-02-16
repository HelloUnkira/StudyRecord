#ifndef __FAIL_TREE_H_
#define __FAIL_TREE_H_

#ifdef __cplusplus
	extern "C" {
#endif	

#include <stdio.h>

//leave������е�Ҷ�ӽ�㣬��ʵ�ʵ�����
//node��ŵ�ȫ����Ҷ�ӽ����±꣬�����������
//�������ݼ䣬С����һ��

//��һ��ʼ�ʹ�node�����Ȼ����ǰ��ȷ��
//���node[i]û�����ҽ��
//��ô�����䱾��node[i]

//������
static void adjust_node(int* leave, int* node, int n, int i)
{
	int parent=(i + n - 1) / 2;      /* ������ڵ���±� */
	while (parent > 0) {
		if (leave[i] > leave[node[parent]]) {
			int temp = node[parent];
			node[parent]=i;
			/* iָ�������ʤ�� */
			i= temp;
		}
		parent = parent / 2;
	}
	node[0] = i;
}

static void init_min_data(int* leave, int* node, int n)
{
	//��ʼ��node������leave
	//����leave�ķ�Χ��n��node�ķ�Χ��n - 1
	int i = 0;
	 for(i = n; i > 0; i--)
		adjust_node(leave, node, n, i);
}


int main()
{
	int i, j;
	int leave[11] = {0};
	int node[10] = {0};
	
	for (i = 1; i < 11; i++) 
		leave[i] = rand()%100;
	printf(" \n");
	
	init_min_data(leave, node, 10);
	
	for (j = 1; j < 11; j++) {
		leave[node[0]] = rand()%100;
		adjust_node(leave, node, 10, node[0]);
		
		for (i = 0; i < 11; i++) 
			printf("%d ", leave[i]);	
		printf(" \n");
		for (i = 0; i < 10; i++) 
			printf("%d ", node[i]);		
		printf(" \n\n");		
		
	}	
		
	for (i = 0; i < 11; i++) 
		printf("%d ", leave[i]);	
	printf(" \n");
	for (i = 0; i < 10; i++) 
		printf("%d ", node[i]);		
	printf(" \n");
	printf("%d ", leave[node[0]]);	
	return;
}

#ifdef __cplusplus
	}
#endif	
#endif
