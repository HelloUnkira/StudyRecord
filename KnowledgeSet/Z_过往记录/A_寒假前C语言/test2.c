#include <stdio.h>
int main()
{
	char symbol,aaa;
	int aa[10]={23,12,432,65,34,23,4,5,78,787};
	int i,j ;

	char str[]="aaa *a aqq qqq11 11133  333";
	char * str1=str;
	int c_count=0,d_count=0,other=0;

	//小写字母换成大写字母
	scanf("%c",&aaa);
	printf("%c\n",(char)((int)aaa-32));

	//统计字符串的数据
	while(1)
	{//数字范围在48 - 57
	 //字母范围在65 - 90  97 - 122
	 //
		if((48<=(int)*str1)&&(57>=(int)*str1))
		d_count++;
		else
		if(((65<=(int)*str1)&&(90>=(int)*str1))||((97<=(int)*str1)&&(122>=(int)*str1)))
		c_count++;
		else other++;

		if(*str1 =='\0')break;	
		str1++;
	}
		printf("\n");
		printf("%d\n",d_count);
		printf("%d\n",c_count);
		printf("%d\n",other);

		printf("%d",-1);
		//int型别中包含-1，可直接打印
		printf("%u",-1);
		//unsigned int 型别不包括-1，
		//则-1会被以补码的形式保存，则打印出来为无符号数
		printf("%d",(char)-1);
		//char型别为迷你版int型别，包含-1，可直接打印
		printf("%u",(char)-1);	
		//unsign int型别不包括-1如上
		printf("%d",(unsigned char)-1);
		//-1强行转成其无符号数时，其值会变成模-1，此处为255
		//255在int范围内，直接打印
		printf("%u",(unsigned char)-1);	
		//-1强行转成其无符号数时，其值会变成模-1，此处为255
		//255在unsigne int 范围内，直接打印
		
		char a = 250;//溢出  -128 ~ 127  
		char b = 10;
		char d = a + b;//表达式的计算  自动提升 是整型int 
		printf("%d",d);//
		//char型别用于加减等运算时
		//250 ~ 11111010   10 ~ 1010
		//d = a + b = 11111010 + 00001010 = 1 00000100
		//高位溢出后值为00000100 ~ 4

	//数组排序
	//冒泡法排序
	for (i = 0; i < 10; i++)
	{
		for (j = i+1; j < 10; j++)
		{	int t=0;
			if(aa[i]>aa[j])
			{
				t=aa[i];
				aa[i]=aa[j];
				aa[j]=t;
			}
		}
	}

	printf("\n" );
	for (i = 0; i <10; ++i)
	{
		printf("%d\n",aa[i] );	/* code */
	}


	return 0;
}