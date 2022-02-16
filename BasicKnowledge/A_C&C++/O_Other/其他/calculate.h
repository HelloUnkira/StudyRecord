#ifndef __CALCULATE_H_
#define __CALCULATE_H_

#ifdef __cplusplus
	extern "C" {
#endif	

/* ʹ�� */
/* printf("%f\n",calculate_simple_expression(" -3.4+(2.23 - (2* 8 ) )+3*5")); */

/* �����沨��ʽ��Ҫ������ͬ����������ջ��Ϊ�˼��ʹ������*/
/* ��δ�����ַ�������ȡ��ǰһ������ϳ�С��*/
static float compound_data(char* str)
{
	float data;
	int offset, flag;
	for (offset = 0, flag = 0, data = 0.0; *str != '\0'; str++) {
		if (*str == ' ')
			continue;
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

/* ��str��pointλ�ò���һ���ַ�c���������ַ���������ƶ�һλ���� */
static void insert_char_to_str(const char c, char* point)
{
	char temp = *point;
	char temp_buf;
	*point = c;		
	 while (*point != '\0') {
		point++;
		/* ����temp �� point */
		temp_buf = temp;
		temp = *point;
		*point = temp_buf;
	}
	/* �ߵ��˵�һ����\0' */
	*point = temp;
	point++;
	*point = '\0';
	
}

/* �ᶯ̬����һ���ַ����飬��Ҫ�ͷţ��������������*/
static char* alloc_and_change_str(char* str)
{
	/* �������������ͷ���ָ��ţ� �м为�ŵ�����״γ��ֵ��ǲ����� */
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

	/* �����µ��ַ����飬����-�Ŷ��ܱ��-�ţ�������Ҫ��0 �����ٶ����������ַ� */		
	new_str = (char*)malloc(sizeof(char)*(all + num * 3 + 3));
	/* �����ַ�����ȥ���հ��ַ�*/
	for (buf = str, buf_new = new_str; *buf != '\0'; buf++) 
		if (*buf == ' ')
			continue;
		else {
			*buf_new = *buf;
			buf_new++;		
		}
	
	/* �ҵ����ţ�ȥ�ж����Ǹ��Ż��Ǽ��ţ��Ӷ�ȷ���������������� */
	/* ����ֻ�������������Ϊ�հ�ȫ��ȥ�������ԺܺñȽϣ���һ�־��ǿ�ͷ ��*buf_newΪ����*/
	/* �ڶ��־����ڸ��Ž��������������������֮һ*/
	/* ������Ҫ���ľ��ǣ���������߲��루0   Ȼ���ж��ұ��Ƿ������*��
	/* �������Ҫ�������Ƶ�����λ�ã�û�еĻ�����һ����  */
	for (buf = new_str, buf_new = new_str; *buf_new != '\0'; buf_new++){
		if (*buf_new == '-') {
			/* ɨ���ַ�������⵽��һ��Ŀ�긺��*/
			buf = buf_new;
			--buf;			
				/* Ϊ���ŵĻ���ǰ�����һ��һ��Ϊ���ֻ�,��ֱ�������Ϳ��� */			
			if  ((buf_new != new_str) && ((*buf == ')') || (*buf >= '0' && *buf <= '9')))
				continue; /* ������ʲô */
			/* �������ʼ���� ,�����м����ţ����ǵĴ�������һ����*/
			else if ((buf_new == new_str) || (*buf == '(' || \
				*buf == '*' || *buf == '/' || *buf == '+' || *buf == '-')) {
					/* �ҵ���λ�ã���buf_new�����β��루 0 */
					insert_char_to_str('(', buf_new);
					buf_new++;
					insert_char_to_str('0', buf_new);
					buf_new++;
				/* ����buf_newָ�����(0��� -  ��bufָ��-��һ����  �������һ��)*/
				buf = buf_new;
				buf++;
				if (*buf >= '0' && *buf <= '9') {
					/*��һ�����֣����ҵ��ò�����������Ǹ�λ�ã�����һ������� */
					while (*buf != '\0') 
						if (*buf == '(' || *buf == ')' || *buf == '*' || \
							*buf== '/' || *buf == '+' || *buf == '-')
							break;
						else
							buf++;
						/* �ڴ˲��룩���� */
						insert_char_to_str(')', buf);															
				} else  if (*buf == '(') {
					int left = 1;
					int right = 0;
					buf++;
					/* ��Ҫ�ƶ�����һ����ˣ���Ӧ�ģ��������м���ܻ�����֣����� */
					while (*buf != '\0') {
						if (*buf == '(')
							left++;
						if (*buf == ')')
							right++;
						if (left == right)
							break;
						buf++;			
					}
					/* ���� */
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
	/* �������飬��ű���ʽ*/
	char* buf_help = (char*)malloc(sizeof(char) * n);
	int buf_buf = -1;
	/* �沨��ʽ���ϣ�expression1Ϊ��������expression2Ϊ����� */
	float* expression1 = (float*)malloc(sizeof(float) * n);
	char* expression2 = (char*)malloc(sizeof(char) * n);
	/* ����Ҫһ����־λר��ȥ�б���ĳһλ���ϣ�ջ�ǲ��������������*/
	bool* expression_help = (bool*)malloc(sizeof(bool) * n);
	/* ʵ�ʵ��α��Ǹ���expression_help�ߵ�*/
	int exp = -1;
	float* result = (float*)malloc(sizeof(float) * n);
	int result_index = -1;
	
	//��ת�����沨������ʽ
	for (buf = str; *buf != '\0'; ) {
		if (*buf == ' ') { /* �����԰ף��ո�� */
			buf++;
			continue;
		}
		/* ��������һ��������, �õ���*/
		if (*buf >='0' && *buf <= '9') {
			/* ������������ȡ��buf��ʼδ�������ݣ���ɸ�����ֱ��������һ��������*/
			float data = compound_data(buf);					
			/* ֱ��ѹ����ջ*/
			exp++;
			/* ���÷���λΪ0����ʾ������*/
			expression_help[exp] = 0;
			/* ��������д��*/
			expression1[exp] = data;		
		}
		/* ���Ǵ�ʱbuf��û���ƶ�������ָ��ò�������ͨ���ƶ�ȥ�����ò�����*/
		while (*buf != '\0')
			if (*buf == '(' || *buf == ')' || \
				*buf == '*' || *buf == '/' || *buf == '+' || *buf == '-')
				break;
			else
				buf++;
		if (*buf == '\0')
			break;
		/* ����bufָ���˲���������������*/
		
		/* �������õ���ʹ����ˣ������������ */		
		/* ����������ţ�ֱ��ѹ�븨��ջ */
		if (*buf == '(') {
			buf_buf++;
			buf_help[buf_buf] = *buf;				
			buf++;
		} else if (*buf == ')') {
			/* ����������� */
			/* �Ӹ���ջ��ȡ���ݣ��ŵ���ջ֮�У�ֱ��ȡ����һ�������� */
			while (buf_buf >= 0) {
				if (buf_help[buf_buf] == '(') {
					buf_buf--;/* ������ֱ�Ӷ��� */
					break;
				} else {
					/* ȡ���� */
					char qa = buf_help[buf_buf];
					buf_buf--;
					/* �����Ŷ�����ջ֮�� */
					exp++;
					/* ���ñ��Ϊ����*/
					expression_help[exp] = 1;
					expression2[exp] = qa;
				}				
			}	
			buf++;
		} else {
			/* �������ջ�Ķ�Ϊ������ֱ��ѹ��*/
			if (buf_help[buf_buf] == '(') {
				buf_buf++;
				buf_help[buf_buf] = * buf;
				buf++;
			} else {
				/* �ж��伶��buf���ȼ����ڸ���ջ��Ԫ�����ȼ� */
				if ((buf_buf < 0) || ((*buf == '*' || *buf == '/') && \
					(buf_help[buf_buf] == '+' || buf_help[buf_buf] == '-'))) {
						/* ֱ���븨��ջ */
						buf_buf++;
						buf_help[buf_buf] = *buf;
						buf++;
					} else {
						while (buf_buf >= 0) {
							/* ֱ������ջ����������ȼ�����ڣ����������ڣ�x�����ȼ� */
							if ((buf_help[buf_buf] == '+' || buf_help[buf_buf] == '-')\
								&& (*buf == '*' || *buf == '/')) {
									buf_buf++;
									buf_help[buf_buf] = *buf;				
									buf++;
									break;
							} else if (expression_help[exp] == 1 && expression2[exp] == '(') {
								/* ��ջs2��ջ�������Ϊ'('����ʱ����xѹ��ջs1 */
									buf_buf++;
									buf_help[buf_buf] = *buf;				
									buf++;				
									break;
							}								
							/* ��ջs1��ջ�������������ѹ��ջs2�� */
							/* ȡ���� */
							char qa = buf_help[buf_buf];
							buf_buf--;
							/* �����Ŷ�����ջ֮�� */
							exp++;
							/* ���ñ��Ϊ����*/
							expression_help[exp] = 1;
							expression2[exp] = qa;							
					}							
				}				
			}			
		}			
	}
	free_change_str(str);
	
	for (; buf_buf >= 0; buf_buf--) {
		/* ��ջs1��ջ�������������ѹ��ջs2�� */
		/* ȡ���� */
		char qa = buf_help[buf_buf];
		/* �����Ŷ�����ջ֮�� */
		exp++;
		/* ���ñ��Ϊ����*/
		expression_help[exp] = 1;
		expression2[exp] = qa;
	}
	free(buf_help);		
	
	/* ǰ����������ģ��ջ��ʵ�����������ʽת���ɺ�������ʽ */
	/* Ϊ�˽�Լ�������������ջ����ֱ�Ӵ��ʼ�������� 0 - exp */
	for (n = 0; n <= exp; n++) {
		if (expression_help[n] == 0) {
			/* �����˲����� */
			result_index++;
			result[result_index] = expression1[n];
		} else if (expression_help[n] == 1) {
			/* ����������� */
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

#ifdef __cplusplus
	}
#endif	
#endif
