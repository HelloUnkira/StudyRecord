#ifndef __A_OTHER_H_
#define __A_OTHER_H_

#ifdef __cplusplus
	extern "C" {
#endif	

#include<stdio.h>
#include<stdbool.h>
#include<math.h>
#include<string.h>
#include<stdarg.h>

unsigned char dashu1[1000];
unsigned char cs[1000];
unsigned char thebuf[1000];
unsigned char bufdashu[1000];
//函数之间存在相互调用，存在向上依赖性

int dashucheng(int y,int x)	//计算X*Y 允许溢出，数据存储在dashu1[]中
{
	int buf;
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

void alldashucheng(int xt)//计算xt！允许溢出，数据存储在dashu1中
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

long xima(int x)//计算x！后面有多少个0 ，不会溢出
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

void moce(int x)//计算x全部合数对，可能没有 10=1+2+3+4
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

bool runnian(int x)//断言一年是否闰年
{
	if((x%4==0)&&(x%100!=0))
		return true;
	else if(x%400==0)
		return true;
	else
		return false;
}

int tianshu(int year,int month,int day)//断言这一年过了多少天
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

int week(int year,int month,int day)//辅助
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

void wannianli(int year,int month,int day)//万年历
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

//你活了多久
int houlong(int birth_year,int birth_month,int birth_day,\
			int now_year,int now_month,int now_day)
{
	//如果来自未来的话
	if(birth_year>now_year)
	return -1;
	//如果都是当年的话
	if(birth_year==now_year)
	return tianshu(now_year,now_month,now_day)-\
		   tianshu(birth_year,birth_month,birth_day);
	
	//出生年与当前年额外算
	int birth_days=tianshu(birth_year,12,31)-\
				   tianshu(birth_year,birth_month,birth_day);
	int now_days=tianshu(now_year,now_month,now_day);
	int mid_days=0;	
	//中间年份单独算
	int i=birth_year+1;
	while(1)
	{
		if(i>=now_year) break;
		mid_days+=tianshu(i,12,31);
		i++;
	}
	return birth_days+now_days+mid_days;
}

void var_parameter(int x,...)//变参函数实现，打印指定x个字符串
{
	int i;
	va_list ap;
	va_start(ap,x);
	for(i=0;i<x;i++)
        printf("%s \n",va_arg(ap,const char*));
	va_end(ap);
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

void fenjie(char * str,int x)//求解一个数完备的质因数，保存在str字符串中
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


#ifdef __cplusplus
	}
#endif	
#endif
