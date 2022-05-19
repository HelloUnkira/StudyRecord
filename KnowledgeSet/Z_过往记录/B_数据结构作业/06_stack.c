#include<stdio.h>

//自定义头文件
typedef int elemtype;

typedef struct stack 
{
	int length;
	int num;
	int point;
	elemtype* stack_p;
}stack_t;

struct stack* create_stack(int n);
void clear_stack(struct stack* stk);
void destory_stack(struct stack* stk);
bool pop_from_stack(struct stack* stk, elemtype* data);
bool push_to_stack(struct stack* stk, elemtype data);

void conversion(int num_10, int k);

//main.c函数
int main()
{
	
	 conversion(108, 15);
	return 0;
}


//自定义源文件
//#include<……>
struct stack* create_stack(int n)
{
	struct stack* buf = \
		(struct stack*)malloc(sizeof(struct stack));
	buf->stack_p = \
		(elemtype*)malloc(sizeof(elemtype) * n);
	buf->num = 0;
	buf->point = -1;
	buf->length = n;
	return buf;
}

void clear_stack(struct stack* stk)
{
	stk->point = -1;
	stk->num = 0;
}

void destory_stack(struct stack* stk)
{
	free(stk->stack_p);
	free(stk);
}

bool pop_from_stack(struct stack* stk, elemtype* data)
{
	if (stk->point > -1) {
		*data = stk->stack_p[stk->point];		
		stk->point--;
		stk->num--;
		return 1;
	}
	return 0;
}

bool push_to_stack(struct stack* stk, elemtype data)
{
	if (stk->num < stk->length) {
		stk->point++;
		stk->num++;
		stk->stack_p[stk->point] = data; 
		return 1;
	}
	return 0;
}

void conversion(int num_10, int k)
{
	int stack[100] = {-1, 0};
	int point = 0;
	int buf = 0;
	/* 模拟栈 */
	while (num_10 != 0) {
		buf = num_10 % k;
		num_10 = num_10 / k;
		/* 入栈 */
		point++;
		stack[point] = buf;
	}
	
	while (point > 0) {
		/* 出栈 */
		buf = stack[point];
		point--;
		printf("%d", buf);
	}
	printf("\n");
}











