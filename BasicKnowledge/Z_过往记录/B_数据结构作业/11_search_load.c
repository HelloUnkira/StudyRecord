#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>

#define MAX_ROW 7
#define MAX_COL 7

//深度广度优先搜索完成迷宫图的路径查找

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

//队列使用数据结构如下

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

//堆栈使用数据结构如下

struct stack
{
	int row;
	int col;	
};

//int high = 0;

struct stack simulate_stack[MAX_ROW * MAX_COL] \
= {{0, 0}, };



/*
*模拟队列实现广度优先搜索 
*/

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

int queue_is_empty()
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
	while (!queue_is_empty()) {			
		
		dequeue(bufqueue);										

		/*
		if (bufqueue->row == MAX_ROW - 1 && bufstack->col == MAX_COL - 1)
			break;
		*/
		 
		/* 到达右边界 */ 
		if (bufqueue->col + 1 <= MAX_COL - 1 \
			&& simulate_map[bufqueue->row][bufqueue->col + 1] == 0)
			enqueue(bufqueue->row,bufqueue->col+1);			
		/* 到达下边界 */ 
		if (bufqueue->row + 1 <= MAX_ROW - 1 \
			&& simulate_map[bufqueue->row + 1][bufqueue->col] == 0)
			enqueue(bufqueue->row+1,bufqueue->col);				
		/* 到达左边界 */
		if (bufqueue->col - 1 >= 0 \
			&& simulate_map[bufqueue->row][bufqueue->col - 1] == 0)
			enqueue(bufqueue->row,bufqueue->col-1);
		/* 到达上边界 */
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

/*
*模拟堆栈实现深度优先搜索 
*/

/* 压栈 */
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

int stack_is_empty()
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
	while (!stack_is_empty()) {			
		
		popup(bufstack);										

		/*
		if (bufstack->row == MAX_ROW - 1 && bufstack->col == MAX_COL - 1)
			break;
		*/
		 
		/* 到达右边界 */ 
		if (bufstack->col + 1 <= MAX_COL - 1 \
			&& simulate_map[bufstack->row][bufstack->col + 1] == 0)
			popdown(bufstack->row,bufstack->col+1);			
		/* 到达下边界 */ 
		if (bufstack->row + 1 <= MAX_ROW - 1 \
			&& simulate_map[bufstack->row + 1][bufstack->col] == 0)
			popdown(bufstack->row+1,bufstack->col);				
		/* 到达左边界 */
		if (bufstack->col - 1 >= 0 \
			&& simulate_map[bufstack->row][bufstack->col - 1] == 0)
			popdown(bufstack->row,bufstack->col-1);
		/* 到达上边界 */
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

int main()
{
	//无法同时运行
	simulate_queue_option();
	//simulate_stack_option();
	return 0;
}








