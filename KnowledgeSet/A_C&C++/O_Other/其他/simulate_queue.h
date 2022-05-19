#ifndef __SIMULATE_QUEUE_H_
#define __SIMULATE_QUEUE_H_

#ifdef __cplusplus
	extern "C" {
#endif	

/*
*ģ�����ʵ�ֹ���������� 
*/

#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>

#define MAX_ROW 7
#define MAX_COL 7

int simulate_map[MAX_ROW][MAX_COL] = \
{
	{0,1,0,0,1,0,1},
	{0,0,1,0,0,0,0},
	{1,0,1,0,1,1,0},
	{0,0,0,0,1,0,1},
	{1,1,0,1,0,0,0},
	{0,0,0,0,0,1,0},
	{1,0,1,1,0,1,0}	
};

struct queue
{
	int row;
	int col;
};

int high = 0;
int low  = 0;
int flag = 0;

struct queue simulate_queue[MAX_ROW * MAX_COL] \
= {{0, 0}, };

void enqueue(int row,int col)
{
	simulate_queue[high].row = row;
	simulate_queue[high].col = col;
	simulate_map[row][col] = -1;
	if(high+1 > MAX_ROW * MAX_COL-1){
		high=0;
		flag++; 
	} else 
		high++;
}

void dequeue(struct queue* node)
{
	node->row = simulate_queue[low].row;
	node->col = simulate_queue[low].col;
	if(low+1 > MAX_ROW * MAX_COL-1){
		low=0;
		flag++; 
	} else 
		low++;
}

int is_empty()
{
	return (flag%2 == 0) && (low == high);
}


void simulate_queue_option()
{
	int i, j;
	struct queue* bufqueue = \
		(struct queue*)malloc(sizeof(struct queue)*1);
	bufqueue->col = 0;
	bufqueue->row = 0;
	
	enqueue(bufqueue->row, bufqueue->col);
	while (!is_empty()) {			
		
		dequeue(bufqueue);										

		/*
		if (bufqueue->row == MAX_ROW - 1 && bufstack->col == MAX_COL - 1)
			break;
		*/
		 
		/* �����ұ߽� */ 
		if (bufqueue->col + 1 <= MAX_COL - 1 \
			&& simulate_map[bufqueue->row][bufqueue->col + 1] == 0)
			enqueue(bufqueue->row,bufqueue->col+1);			
		/* �����±߽� */ 
		if (bufqueue->row + 1 <= MAX_ROW - 1 \
			&& simulate_map[bufqueue->row + 1][bufqueue->col] == 0)
			enqueue(bufqueue->row+1,bufqueue->col);				
		/* ������߽� */
		if (bufqueue->col - 1 >= 0 \
			&& simulate_map[bufqueue->row][bufqueue->col - 1] == 0)
			enqueue(bufqueue->row,bufqueue->col-1);
		/* �����ϱ߽� */
		if (bufqueue->row - 1 >= 0 \
			&& simulate_map[bufqueue->row - 1][bufqueue->col] == 0)
			enqueue(bufqueue->row-1,bufqueue->col);								

	for (i = 0; i < MAX_ROW; i++) {
		for (j = 0; j < MAX_COL; j++)
			printf("%d\t",simulate_map[i][j]);		
		printf("\n");
	}
	printf("\n");	
	}
			
	free(bufqueue);
}


#ifdef __cplusplus
	}
#endif	
#endif
