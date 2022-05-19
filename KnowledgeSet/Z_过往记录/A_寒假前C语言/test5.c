#include<stdio.h>
#include<stdbool.h>
#include<math.h>

unsigned char dashu1[1000];
unsigned char cs[1000];
unsigned char thebuf[1000];
unsigned char bufdashu[1000];

int weishu(int x) //数的位数
{
	int count=0;
	int buf=x;
	while(1)
	{
		if(buf/10==0&&buf%10!=0){count++;break;}
		else {buf/=10;count++;}
	}
	return count;
}

int nixiang(int x)//数反向
{
	int buf=x;
	int result=0;
	while(1)
	{
		if(buf/10==0&&buf%10!=0){result+=buf%10;break;}
		else {result+=buf%10;result*=10; buf/=10;}
	}
	return result;
}

unsigned long long jiecheng(int x) //普通阶乘
{
int i=1;
unsigned long long result=1;
for(i=1;i<=x;i++)
{
	result*=i;
}
return result;
}

void alldashucheng(int xt) //大数阶乘 ，为xt！ 结果按位存在dashu[1000]中
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

int gongyueshu(int x,int y)//辗转相除法,求最大公约数,会崩溃当公约数不存在时
{		
	int min,max,buf;	
	min=x<y?x:y;
	max=x<y?y:x;
	while(1)
	{
		if(max%min==0) return min;
		else
			{
				buf=max/min;
				max=min;
				min=buf;
			}
	}
}

int gongbeishu(int x,int y)//求最小公倍数，x*y=最大公约数*最小公倍数
{
	return x*y/gongyueshu(x,y);
}

bool runnian(int x)//求闰年
{
	if((x%4==0)&&(x%100!=0))
		return true;
	else if(x%400==0)
		return true;
	else
		return false;
}

int tianshu(int year,int month,int day)//求天数
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

void shuixianhua()//水仙花数
{
	int x=100,bai=0,shi=0,ge=0;
	while(1)
	{
		bai=x/100;
		shi=x%100/10;
		ge=x%10;
		
		if(x==bai*bai*bai+shi*shi*shi+ge*ge*ge)
			printf("%d = %d^3+%d^3+%d^3\n",x,bai,shi,ge);
		x++;
		if(x>=1000) return;
	}

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

int aaa(int number,int count)//计算S=a+aa+aa+……
{								
	int buf=0;
	int Sum=0;
	int i=0;
	while(1)
	{
		if(i>=count) return Sum; 
		buf+=number;
		Sum+=buf;
		buf*=10;
		i++;		
	}
}

int main()
{

	//printf("%d \n",weishu(1234));
	//printf("%d \n",nixiang(1234));
	//printf("%d \n",gongyueshu(15,34));
	//printf("%d \n",gongbeishu(15,34));
	//printf("%d \n",houlong(2000,1,14,2020,1,16));
	//shuixianhua();
	//printf("%u\n",aaa(1,3));
	//printf("%u\n",aaa(2,2));

return 0;
}