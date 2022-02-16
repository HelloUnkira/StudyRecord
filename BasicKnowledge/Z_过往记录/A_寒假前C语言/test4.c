#include<stdio.h>
#include<stdbool.h>
#include<math.h>
#include<string.h>

int test[12]={543,34,543,34,54,65,54,43,5,78,5,56};

bool runnian(int x)//断言是不是闰年
{
	if((x%4==0)&&(x%100!=0))
		return true;
	else if(x%400==0)
		return true;
	else
		return false;
}

int tianshu(int year,int month,int day)//当日于此年总共日子
{
	//断言今年是否大年?
	//31*7+30*4+28
	int bufmonth[12]={31,28,31,30,31,30,31,31,30,31,30,31};
	bool year_flag=runnian(year);
	int allday=0,i;
	for(i=1;i<month;i++)
		allday+=bufmonth[i-1];//累加此月前面的日期
	if(year_flag&&month>=3) 
		allday+=1;//大年补偿
	allday+=day;
	return allday;
}

float yourTable(int x)
{
	switch (x)
		{
		case 0:return 1;
		case 1:return 1.5;
		case 2:return 3;
		case 3:return 5;
		case 4:return 7.5;
		case 5:return 10;
		}
}

float yourTable1(int x)
{
	switch (x)
		{
		case 0:
		case 1:return 1000000;
		case 2:return 600000;
		case 3:return 400000;
		case 4:return 200000;
		case 5:return 100000;
		}

}

float yourPrize(int profit)
{
	float allmoney=0;
	int i=0,flag=1;
	int table[6]={0,0,0,0,0,0};
	
	while(1)
		{
	if(profit>1000000)
		{table[0]=1;break;}
	if(profit>=600000&&profit<=1000000)
		{table[1]=1;break;}
	if(profit>=400000&&profit<=600000)
		{table[2]=1;break;}
	if(profit>=200000&&profit<=400000)
		{table[3]=1;break;}
	if(profit>=100000&&profit<=200000)
		{table[4]=1;break;}
	if(profit<=100000)
		{table[5]=1;break;}

	
	}
	//做不出来，太限制范围了
	
	 for(i=0;i<6;i++)
		 {

		 if (flag==1&&table[i]==0)
		 	continue;
		 else 
		 	flag=0;
		 if(flag==0) 
		 	allmoney+=(float)(profit-yourTable1(i-1))*yourTable(i); 			
	}	
	return allmoney;
}

void erjinzhi(int x)//打印二进制数，实际装入temp中，一个char装一个
{
	unsigned char temp[100]={0};
	int i=0,flag=1;
	for(i=0;i<100;i++)
		{
		if (x==0) break;
		temp[i]=x%2;
		x/=2;
	}
	for (i = 31; i>= 0;--i)
		{
			printf("%d",temp[i]);
	}
}

bool iszhishu(int x)//断言数是不是质数
{
	int buf=(int)(sqrt((double)x));
	int t=2;
	if(x==0||x==1) return 0;
	if(x==2||x==3) return 1;
	while(1)
	{	
		if(x%t==0) return 0;
		if(t>=buf) return 1;
		while(1)
		{
		t++;
		if(t==3||(t+1)%6==0||(t-1)%6==0) break;
		if(t>=buf) break;
		}
	}
}

void fenjie(char * str,int x)//求解一个数完备的质因数，保存在str中
{
	int i=0,buf=x;		
	if(x==0) {sprintf(str,"cannot found! about 0");return;}
	if(x==1) {sprintf(str,"1=1*1");return;}
	do
	{
	char p[10]="";
	sprintf(p,"%d=%d",x,1);
	strcat(str,p);
	}while(0);
	
	while(1)
	{
	char t1[10]="";
	i++;
	if(iszhishu(i))
	while(1)
	{
		if(buf%i==0){
		char t[10]="";
		sprintf(t,"*%d",i);
		strcat(str,t);buf/=i;
		}
		else break;
	}
	if(buf<i)
	if(buf!=1){
		char t1[10]="";
		sprintf(t1,"*%d",buf);
		strcat(str,t1);
		printf("%s",str);
		return;
	}
	else return;		
	}
}

void chazhao(int * temp,int n)//未完成
{
	int * move=temp;
	int * start=temp;
	int * end=temp+n;
 	int i=0;
	while(1)
	{
	move=move+i;
	if(*temp==*move){temp++;move=start;i=0;i--;}
	if(i>n) {printf("%d \n",*temp);i=0;}
	if(temp>end) printf(" NOT FOUND \n");
	i++;
	}
}

void pipei(const char * p)//匹配数据
{
	int count=0;
	while(*p++!='\0')
		{
		if(*p>'1'&&*p<'9') count++;
	}

	if(count>0)printf("YES");
	else	   printf("NO");
	
}

int main()
{

	//chazhao(test, 12);
	
	
	/*
	int i=0;
	for(i=0;i<100;i++)
	//{if(iszhishu(i))printf("%d\n",i);}
	{
	do
	{
	char str[100]="";
	fenjie(str,i);
	printf("%s\n",str);
	}while(0);
	}
	*/
	
	
	/*
	int a=5,b=7,c=4,temp;
	int maxsize1,maxsize;
	char * p;
	*/
	/*
	switch (temp)
		{
		case 100:
			printf("你是A+");break;
		case 99:case 98:case 97:case 96:case 95:
		case 54:case 93:case 92:case 91:case 90:
			printf("你是A");break;
		case 89:case 88:case 87:case 86:case 85:
		case 84:case 83:case 82:case 81:case 80:
			printf("你是B");break;
		case 79:case 78:case 77:case 76:case 75:
		case 74:case 73:case 72:case 71:case 70:
			printf("你是C");break;
		case 69:case 68:case 67:case 66:case 65:
		case 64:case 63:case 62:case 61:case 60:
			printf("你是D");break;
		default:
			printf("你是E");
		}
	*/

	/*
	
	scanf("%d %d %d",&a,&b,&c);
	printf("%d %d %d",a,b,c);

	if((a*a>b*b+c*c)||(b*b>a*a+c*c)||(c*c>a*a+b*b))
		printf("不为三角形");
	else
	if((a==b)&&(b==c)&&(c==a))
			printf("为等边三角形");
	else
	if (((a==b)||(b==c)||(c==a))&&((a*a==b*b+c*c)||(b*b==a*a+c*c)||(c*c==a*a+b*b)))
			printf("为等腰直角三角形");
	else
	if((a==b)||(b==c)||(c==a))
			printf("为等腰三角形");	
	else
	if((a*a==b*b+c*c)||(b*b==a*a+c*c)||(c*c==a*a+b*b))	
			printf("为直角三角形");
	else	
			printf("非规则三角形");
		
	*/

	/*
	maxsize1= a>b? a>c? a : c  : b>c? b : c ;

	while(1)
	if(a<b)
		if(a<c)
			if(b<c)
				break;
			else
				{temp=b;b=c;c=temp;}
		else
			{temp=a;a=c;c=temp;}
	else
		{temp=a;a=b;b=temp;}


	
	printf("%d %d %d\n",a,b,c);
	printf("%d",maxsize1);

	scanf("%s",p);

	while (1)
		{
		if(*p=='\0'){printf("no");break;}
		if(*p>'0'&&*p<'9')
			printf("yes");
		p++;
		}

	*/

}





