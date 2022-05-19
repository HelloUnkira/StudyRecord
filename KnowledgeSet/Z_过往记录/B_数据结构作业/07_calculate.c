#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

//自定义头文件部分
float calculate_simple_expression(char* str);

//main.c部分
int main()
{
	printf("%f\n",calculate_simple_expression(" -3.4+(2.23 - (2* 8 ) )+3*5"));
	return 0;
}

//自定义源文件部分

/* 关于逆波兰式需要俩个不同的数据类型栈，为了简便使用数组*/

/* 从未来的字符串中提取出前一段数组合成小数*/
static float compound_data(char* str)
{
	float data;
	int offset, flag;
	for (offset = 0, flag = 0, data = 0.0; *str != '\0'; str++) {
		if (*str == '(' || *str == ')' || \
			*str == '*' || *str == '/' || *str == '+' || *str == '-')
			break;
		if (*str >='0' && *str <= '9') {
			data *= 10;	
			data += *str - '0';
		}
		if (flag == 1)
			offset++;	
		if (*str == '.')
			flag = 1;		
	}	
	for (; offset > 0; data /= 10.0, offset--) ;	 
	return data;
}

/* 从str的point位置插入一个字符c，则后面的字符都需向后移动一位即可 */
static void insert_char_to_str(const char c, char* point)
{
	char temp = *point;
	char temp_buf;
	*point = c;		
	 while (*point != '\0') {
		point++;
		/* 交换temp 与 point */
		temp_buf = temp;
		temp = *point;
		*point = temp_buf;
	}
	/* 走到了第一个‘\0' */
	*point = temp;
	point++;
	*point = '\0';
	
}

/* 会动态分配一个字符数组，需要释放，处理负数的情况*/
static char* alloc_and_change_str(char* str)
{
	/* 负数的情况：开头出现负号， 中间负号的左边首次出现的是操作数 */
	char* buf = str;
	char* buf_new = NULL;
	char* new_str = NULL;
	int num = 0,all = 0;
	/**/
	for ( buf = str; *buf != '\0' ; buf++) {
		if (*buf == '-')
			num++;		
		if (*buf != ' ')
			all++;
	}

	/* 创建新的字符数组，所有-号都能变成-号，所以需要（0 ）至少多增加三个字符 */		
	new_str = (char*)malloc(sizeof(char)*(all + num * 3 + 3));
	/* 拷贝字符串，去掉空白字符*/
	for (buf = str, buf_new = new_str; *buf != '\0'; buf++) 
		if (*buf == ' ')
			continue;
		else {
			*buf_new = *buf;
			buf_new++;		
		}
	
	/* 找到负号，去判断它是负号还是减号，从而确定是修正还是跳过 */
	/* 其中只有俩种情况，因为空白全部去掉了所以很好比较，第一种就是开头 即*buf_new为负号*/
	/* 第二种就是在负号紧贴的左边是六个操作符之一*/
	/* 其中需要做的就是，往它的左边插入（0   然后判断右边是否紧贴（*、
	/* 如果有需要往后腾移到合适位置，没有的话紧贴一个）  */
	for (buf = new_str, buf_new = new_str; *buf_new != '\0'; buf_new++){
		if (*buf_new == '-') {
			/* 扫描字符串，检测到了一个目标负号*/
			buf = buf_new;
			--buf;			
				/* 为减号的话，前面紧接一个一定为数字或）,则直接跳过就可以 */			
			if  ((buf_new != new_str) && ((*buf == ')') || (*buf >= '0' && *buf <= '9')))
				continue; /* 无需做什么 */
			/* 如果是起始负号 ,或者中间括号，它们的处理都是一样的*/
			else if ((buf_new == new_str) || (*buf == '(' || \
				*buf == '*' || *buf == '/' || *buf == '+' || *buf == '-')) {
					/* 找到了位置，在buf_new处依次插入（ 0 */
					insert_char_to_str('(', buf_new);
					buf_new++;
					insert_char_to_str('0', buf_new);
					buf_new++;
				/* 现在buf_new指向插入(0后的 -  ，buf指向-后一个，  还需插入一个)*/
				buf = buf_new;
				buf++;
				if (*buf >= '0' && *buf <= '9') {
					/*仅一个数字，则找到该操作数后面的那个位置，即下一个运算符 */
					while (*buf != '\0') 
						if (*buf == '(' || *buf == ')' || *buf == '*' || \
							*buf== '/' || *buf == '+' || *buf == '-')
							break;
						else
							buf++;
						/* 在此插入）即可 */
						insert_char_to_str(')', buf);															
				} else  if (*buf == '(') {
					int left = 1;
					int right = 0;
					buf++;
					/* 需要移动到下一个与此（对应的），其中中间可能还会出现（）对 */
					while (*buf != '\0') {
						if (*buf == '(')
							left++;
						if (*buf == ')')
							right++;
						if (left == right)
							break;
						buf++;			
					}
					/* 插入 */
					insert_char_to_str(')', buf);				
				}				
			}		
		}
	}
	return new_str;
}

static void free_change_str(char* str)
{
	free(str);
}

float calculate_simple_expression(char* old_str)
{
	char* str = alloc_and_change_str(old_str);
	float result_result;
	char* buf = str;
	int n = 1;		
	for (n = 1; *buf != '\0'; n++, buf++) ;	
	/* 辅助数组，存放表达式*/
	char* buf_help = (char*)malloc(sizeof(char) * n);
	int buf_buf = -1;
	/* 逆波兰式集合，expression1为操作数，expression2为运算符 */
	float* expression1 = (float*)malloc(sizeof(float) * n);
	char* expression2 = (char*)malloc(sizeof(char) * n);
	/* 且需要一个标志位专门去判别，在某一位置上，栈是操作数还是运算符*/
	bool* expression_help = (bool*)malloc(sizeof(bool) * n);
	/* 实际的游标是跟着expression_help走的*/
	int exp = -1;
	float* result = (float*)malloc(sizeof(float) * n);
	int result_index = -1;
	
	//先转化成逆波兰表达式
	for (buf = str; *buf != '\0'; ) {
		if (*buf == ' ') { /* 处理旁白，空格键 */
			buf++;
			continue;
		}
		/* 接下来有一个操作数, 拿到它*/
		if (*buf >='0' && *buf <= '9') {
			/* 辅助函数，获取从buf开始未来的数据，变成浮点数直到遇到下一个操作符*/
			float data = compound_data(buf);					
			/* 直接压入主栈*/
			exp++;
			/* 设置符号位为0，表示操作数*/
			expression_help[exp] = 0;
			/* 将操作数写入*/
			expression1[exp] = data;		
		}
		/* 但是此时buf并没有移动，还是指向该操作数，通过移动去跳过该操作数*/
		while (*buf != '\0')
			if (*buf == '(' || *buf == ')' || \
				*buf == '*' || *buf == '/' || *buf == '+' || *buf == '-')
				break;
			else
				buf++;
		if (*buf == '\0')
			break;
		/* 现在buf指向了操作数后面的运算符*/
		
		/* 操作数拿到后就处理了，就是运算符了 */		
		/* 如果是左括号，直接压入辅助栈 */
		if (*buf == '(') {
			buf_buf++;
			buf_help[buf_buf] = *buf;				
			buf++;
		} else if (*buf == ')') {
			/* 如果是右括号 */
			/* 从辅助栈中取数据，放到主栈之中，直到取到第一个左括号 */
			while (buf_buf >= 0) {
				if (buf_help[buf_buf] == '(') {
					buf_buf--;/* 左括号直接丢弃 */
					break;
				} else {
					/* 取出来 */
					char qa = buf_help[buf_buf];
					buf_buf--;
					/* 将符号丢到主栈之中 */
					exp++;
					/* 设置标记为符号*/
					expression_help[exp] = 1;
					expression2[exp] = qa;
				}				
			}	
			buf++;
		} else {
			/* 如果辅助栈的顶为（，则直接压入*/
			if (buf_help[buf_buf] == '(') {
				buf_buf++;
				buf_help[buf_buf] = * buf;
				buf++;
			} else {
				/* 判断其级别，buf优先级大于辅助栈顶元素优先级 */
				if ((buf_buf < 0) || ((*buf == '*' || *buf == '/') && \
					(buf_help[buf_buf] == '+' || buf_help[buf_buf] == '-'))) {
						/* 直接入辅助栈 */
						buf_buf++;
						buf_help[buf_buf] = *buf;
						buf++;
					} else {
						while (buf_buf >= 0) {
							/* 直到辅助栈顶运算符优先级别低于（不包括等于）x的优先级 */
							if ((buf_help[buf_buf] == '+' || buf_help[buf_buf] == '-')\
								&& (*buf == '*' || *buf == '/')) {
									buf_buf++;
									buf_help[buf_buf] = *buf;				
									buf++;
									break;
							} else if (expression_help[exp] == 1 && expression2[exp] == '(') {
								/* 或栈s2的栈顶运算符为'('，此时再则将x压入栈s1 */
									buf_buf++;
									buf_help[buf_buf] = *buf;				
									buf++;				
									break;
							}								
							/* 将栈s1的栈顶运算符弹出，压入栈s2中 */
							/* 取出来 */
							char qa = buf_help[buf_buf];
							buf_buf--;
							/* 将符号丢到主栈之中 */
							exp++;
							/* 设置标记为符号*/
							expression_help[exp] = 1;
							expression2[exp] = qa;							
					}							
				}				
			}			
		}			
	}
	free_change_str(str);
	
	for (; buf_buf >= 0; buf_buf--) {
		/* 将栈s1的栈顶运算符弹出，压入栈s2中 */
		/* 取出来 */
		char qa = buf_help[buf_buf];
		/* 将符号丢到主栈之中 */
		exp++;
		/* 设置标记为符号*/
		expression_help[exp] = 1;
		expression2[exp] = qa;
	}
	free(buf_help);		
	
	/* 前面利用数组模拟栈，实现了中序表达式转化成后续表达式 */
	/* 为了节约我们无需逆序堆栈，可直接从最开始操作即从 0 - exp */
	for (n = 0; n <= exp; n++) {
		if (expression_help[n] == 0) {
			/* 遇到了操作数 */
			result_index++;
			result[result_index] = expression1[n];
		} else if (expression_help[n] == 1) {
			/* 遇到了运算符 */
			float buf = 0;
			float b = result[result_index];
			result_index--;
			float a = result[result_index];
			result_index--;
			switch (expression2[n]) {
				case '+':
					buf = a + b;
					break;
				case '-':
					buf = a - b;
					break;
				case '*':
					buf = a * b;
					break;
				case '/':
					buf = a / b;
					break;
			}
			result_index++;
			result[result_index] = buf;				
		}
	}
	result_result = result[result_index];
	
	free(expression1);
	free(expression2);
	free(expression_help);
	free(result);
	
	return result_result;
}

