#include<stdio.h>
#include<stdbool.h>
#include<stdarg.h>

unsigned char dashu1[1000];
unsigned char cs[1000];
unsigned char thebuf[1000];
unsigned char bufdashu[1000];
unsigned long long jiecheng(int x)
{
int i=1;
unsigned long long result=1;
for(i=1;i<=x;i++)
{
	result*=i;
}
return result;
}

int dashucheng(int y,int x)
{	int buf;
	int t=0,i=0,j,t1=0;
		for(j=0;j<1000;j++)
		{
		bufdashu[j]=0;
		thebuf[j]=0;
	}

	while(1)
		{

		if(x>10)
			{
			buf = y % 10;
			y/=10;	
		}
		else
			{
			buf = y;
			y=0;
		}
		if(t1<1000)
		dashu1[t1]=buf;
		t1++;	
		if((y/10==0)&&(y%10==0)) break;
	}
	while(1)
		{
		if(x>10)
			{
			buf = x % 10;
			x/=10;	
		}
		else
			{
			buf = x;
			x=0;
		}
		if(t<1000)
		cs[t]=buf;
		t++;
		if((x/10==0)&&(x%10==0)) break;
	}
	
	
	while (1)
		{
		if(i>=t)break;
		//清空缓冲区
		for(j=0;j<1000;j++)
			{
			thebuf[j]=0;
		}
		
		for(j=0;j<t1;j++)
			{
			//第一位对数值全乘并存入缓冲区
			buf=cs[i]*dashu1[j];
			//对齐写入缓冲区
			thebuf[i+j]=buf;			
		}	
		/*
		//对缓冲区的进位完成进位操作
		for(j=0;j<100;j++)
			{
			if(thebuf[j]>10)
				{
				buf=thebuf[j]/10;
				thebuf[j]%=10;
				if(j+1<100)
					thebuf[j+1]+=buf;
			}			
		}
		*/
			//将该位对齐累加入累加器中
		for(j=0;j<1000;j++)
			{
			bufdashu[j]+=thebuf[j];
		}	
		
		//对累加器中的进位完成进位操作
		for(j=0;j<1000;j++)
			{
			if(bufdashu[j]>=10)
				{
				buf=bufdashu[j]/10;
				bufdashu[j]%=10;
				if(j+1<1000)
					bufdashu[j+1]+=buf;
			}			
		}
	
		i++;
	}
	
	return 0;
}

void alldashucheng(int xt)
{
	int buf,flag=1;
	int t=0,i=0,j=0,t1=0,xt1=1,bufx=1;
	for(j=0;j<1000;j++)
		{
		bufdashu[j]=0;
		thebuf[j]=0;
	}
	bufdashu[0]=1;
	
	while (1)
		{
	
	 	if(bufx>xt+1)break;
 
		for (j= 0; j< 1000; ++j)
		{
		dashu1[j]=bufdashu[j];
	}
		
		for (j = 999; j >= 0; --j)
		{
		if(flag==1&&dashu1[j]==0)continue;
		else{flag=0;t1++;}
	}
		
		for(j=0;j<1000;j++)
		{
		bufdashu[j]=0;
		thebuf[j]=0;
	}

	while(1)
		{
		if(xt1>10)
			{
			buf = xt1 % 10;
			xt1/=10;	
		}
		else
			{
			buf = xt1;
			xt1=0;
		}
		if(t<100)
		cs[t]=buf;
		t++;
		if((xt1/10==0)&&(xt1%10==0)) break;
	}
		
	while (1)
		{
		if(i>=t){i=0;t=0;t1=0;break;}
			//清空缓冲区
		for(j=0;j<1000;j++)
			{
			thebuf[j]=0;
		}
			
		for(j=0;j<t1;j++)
			{
			//第一位对数值全乘并存入缓冲区
			buf=cs[i]*dashu1[j];
			//对齐写入缓冲区
			thebuf[i+j]=buf;			
		}	
		
			//将该位对齐累加入累加器中
		for(j=0;j<1000;j++)
			{
			bufdashu[j]+=thebuf[j];
		}	
			
		//对累加器中的进位完成进位操作
		for(j=0;j<1000;j++)
			{
			if(bufdashu[j]>=10)
				{
				buf=bufdashu[j]/10;
				bufdashu[j]%=10;
				if(j+1<1000)
					bufdashu[j+1]+=buf;
			}			
		}
		
		i++;
	}
	bufx++;
	xt1=bufx;
	flag=1;
	}


}

long xima(int x)
{
	int i=0;
	long two_count=0,five_count=0;
	for(i=0;i<=x;i++)
	{
	int buf;
	buf=i;
	while(1)
	{
		
		if((buf%2==0)&&(buf!=0))
		{
		buf/=2;
		two_count++;
		}
		else break;
	}
	//printf("%d \n",two_count);
	buf=i;
	while(1)
	{
		if((buf%5==0)&&(buf!=0))
		{
		buf/=5;
		five_count++;
		}
		else break;
	}
	//printf("%d \n",five_count);
	}
	if(two_count<=five_count) return two_count;
	else return five_count;
}

void moce(int x)
{
	int i=0;
	for(i=1;i<x/2+1;i++)
	{	int buf=0,aa,j=0;
		aa=i;
		while(1)
			{
				if(buf<=x)
				{
					if(buf==x)
						{
						for(j=i;j<aa;j++)
						printf("%d ",j);
						printf("\n");
						break;
					}
					else
					{
						buf+=aa;
						aa++;
					}
				}
				else break;
		}
	}

}

bool runnian(int x)
{
	if((x%4==0)&&(x%100!=0))
		return true;
	else if(x%400==0)
		return true;
	else
		return false;
}

int tianshu(int year,int month,int day)
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

int week(int year,int month,int day)
{
	//以1900,1,1为参考，这一天为星期1
	int alldays=0;
	if(year>=1900)
		{
		int t=1900;
		while(1)
			{
			if(t>=year)break;
			alldays+=(365+runnian(t));
			t++;		
		}
	alldays+=tianshu(year,month,1);
	}
	else
		{
		int t=1900;
		while(1)
			{
			if(t<=year)break;
			alldays+=(365+runnian(t));
			t--;		
		}
	alldays+=tianshu(year,month,1);
	}
	
	return alldays%7;//为0则当月第一天为星期日
}

void wannianli(int year,int month,int day)
{
	int bufmonth[12]={31,28,31,30,31,30,31,31,30,31,30,31};
	int offset=week(year,month,day);
	int i=0,j=offset;
	printf("0\t1\t2\t3\t4\t5\t6\t\n");
	for(i=0;i<42;i++)
	{
	if(i%7==0)printf("\n");
	if(i>(bufmonth[month-1]+((month==2)?runnian(year):0)+offset-1))
		printf("--\t");
	else
	if(j>0)
		{
		printf("--\t");
		j--;
	}
	else 
		printf("%d\t",i-offset+1);
	
	}
	

}

/*
void var_parameter(int x,...)
{
	int i;
	va_list ap;
	va_start(ap,x);
	for(i=0;i<x;i++)
        printf("%s \n",va_arg(ap,const char*));
	va_end(ap);
}
*/

/*

//作业
//1.  a&0x xxx 即可
int makexxx(int p,int n)
{
	int i=0;
	int xxx=0;
	for(i=0;i<n;i++)
		{
			xxx<<1;
			xxx=xxx|0x01;
	}
	for(i=0;i<p;i++)
		{
			xxx<<1;
	}
	return xxx;
}
//1.  第一题取出了 0x xxx
//2.  只需修改     0x xxx即可
void changexxx(int p,int n,int oldxxx,int y)
{
	int newone,i;
		for(i=0;i<n;i++)
		{
			newone<<1;
			y=newone|0x01;
	}
	newone=newone&y;

		for(i=0;i<p;i++)
		{
			newone<<1;
	}

	oldxxx=oldxxx&0;
	oldxxx=oldxxx|newone;
}

//3.只需按照第一步构造一个 ~0x xxx即可

//4.循环左移和右移
//
//循环右移
void movebitright()
{
int t;
unsigned char x=240;//11110000

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
}

*/

int main()
{	
	//验证
	int flag=1,q,e,allzero=0;
	for(e=5;e<200;e+=5)
		{
		flag=1;
		allzero=0;
		alldashucheng(e);
	for (q = 0; q < 1000; ++q)
		{
		if (dashu1[q]==0)allzero++;
		else break;
	}
	for (q = 999; q >= 0;--q)
		{
		if (flag==1&&dashu1[q]==0)continue;
		else flag=0;
		if(flag==0)printf("%d",dashu1[q]);
	}
	printf("\n");
	printf("%d",allzero);
	printf("\n");
	printf("%ld",xima(e));
	printf("\n");	
	}
	return 0;
}

