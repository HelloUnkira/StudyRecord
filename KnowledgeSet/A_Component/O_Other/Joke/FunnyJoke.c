

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
