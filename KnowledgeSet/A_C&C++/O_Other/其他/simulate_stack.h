#ifndef __SIMULATE_STACK_H_
#define __SIMULATE_STACK_H_

#ifdef __cplusplus
	extern "C" {
#endif	

/*
*ģ���ջʵ������������� 
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

struct stack
{
	int row;
	int col;	
};

int high = 0;

struct stack simulate_stack[MAX_ROW * MAX_COL] \
= {{0, 0}, };

/* ѹջ */
void popdown(int row, int col)
{
	simulate_stack[high].row = row;
	simulate_stack[high].col = col;
	simulate_map[row][col] = -1;
	high++;
}

void popup(struct stack* node)
{
	high--;
	node->row = simulate_stack[high].row;
	node->col = simulate_stack[high].col;
}

int is_empty()
{
	return high == 0; 
}

void simulate_stack_option()
{
	int i, j;
	struct stack* bufstack = \
		(struct stack*)malloc(sizeof(struct stack)*1);
	bufstack->col = 0;
	bufstack->row = 0;
	
	popdown(bufstack->row, bufstack->col);
	while (!is_empty()) {			
		
		popup(bufstack);										

		/*
		if (bufstack->row == MAX_ROW - 1 && bufstack->col == MAX_COL - 1)
			break;
		*/
		 
		/* �����ұ߽� */ 
		if (bufstack->col + 1 <= MAX_COL - 1 \
			&& simulate_map[bufstack->row][bufstack->col + 1] == 0)
			popdown(bufstack->row,bufstack->col+1);			
		/* �����±߽� */ 
		if (bufstack->row + 1 <= MAX_ROW - 1 \
			&& simulate_map[bufstack->row + 1][bufstack->col] == 0)
			popdown(bufstack->row+1,bufstack->col);				
		/* ������߽� */
		if (bufstack->col - 1 >= 0 \
			&& simulate_map[bufstack->row][bufstack->col - 1] == 0)
			popdown(bufstack->row,bufstack->col-1);
		/* �����ϱ߽� */
		if (bufstack->row - 1 >= 0 \
			&& simulate_map[bufstack->row - 1][bufstack->col] == 0)
			popdown(bufstack->row-1,bufstack->col);								

	for (i = 0; i < MAX_ROW; i++) {
		for (j = 0; j < MAX_COL; j++)
			printf("%d\t",simulate_map[i][j]);		
		printf("\n");
	} 	
	printf("\n");
	}
			
	free(bufstack);
}

#ifdef __cplusplus
	}
#endif	
#endif
