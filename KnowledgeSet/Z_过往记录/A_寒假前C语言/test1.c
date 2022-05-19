#include<stdio.h>
int main(void)
{
	int number1,number2,result;
	char symbol;

	int t;
	unsigned char x=240;//11110000

	int i,number3=0;

	// //简单计算器
	// scanf("%d %c %d",&number1,&symbol,&number2);
	// switch(symbol)
	// {
	// 	case '+':result=number1+number2;break;
	// 	case '-':result=number1-number2;break;
	// 	case '*':result=number1*number2;break;
	// 	case '/':result=number1/number2;break;
	// 	default : 
	// 	 printf("您的输入有误，程序已崩溃\n");
	// }
	// printf("the result is %d \n",result);

	//循环右移
	printf("(您要做几位的循环右移：)\n");
	scanf("%d",&t);
	t%=8;
	while(t!=0)
	{
		int flag=0;
		if(x%2!=0) flag=1;
		else flag = 0;
 		x>>=1;
 		if(flag==1) x|=0x80;
 		else x|=0x00;
 	t--;
	}
	printf("%d \n",x);


	// //1到100奇数做和
	// for(i=0;i<101;i++)
	// 	if(i%2!=0)
	// 		number3=number3+i;
	// printf("%d\n" ,number3);
	

}