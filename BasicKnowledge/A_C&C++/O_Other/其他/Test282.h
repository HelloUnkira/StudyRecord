#ifndef __MYDONE_H_
#define __MYDONE_H_

#include<iostream>
#include<cmath>

using namespace std;

/*
**C���Ծ�����282�� 
*���� 
*/

#ifdef __cplusplus
	extern "C" {
#endif	

/*ex009:��x��*/
unsigned long long ex009(int x){
	int i=1;
	unsigned long long result=1;
	for(i=1;i<=x;i++){
		result*=i;
	}
	return result;
}

/*ex013:����η������λ��*/ 
int  ex013(int x,int y){
	if(y==1) {
		return x%100;
	}
	else if(y%2==0){
		return (ex013(x,y/2)%100)*(ex013(x,y/2)%100)%100;
	}
	else{
		return (ex013(x,y/2)%100)*(ex013(x,y/2+1)%100)%100;
	}
}
 
//�Ƿ�Ϊ���� 
bool help_ex014(int x){
	if((x%4==0)&&(x%100!=0))
		return true;
	else if(x%400==0)
		return true;
	else
		return false;
}
/*ex014:��ǰʱ��Ϊ����ڼ���*/ 
int ex014(int year,int month,int day){
	//31*7+30*4+28
	int bufmonth[12]={31,28,31,30,31,30,31,31,30,31,30,31};
	bool year_flag=help_ex014(year);
	int allday=0,i;
	for(i=1;i<month;i++)
		allday+=bufmonth[i-1];
	if(year_flag&&month>=3) 
		allday+=1;
	allday+=day;
	return allday;
}

/*ex016:��Ԫ��ټ�*/
void ex016(){
	/*������飬ĸ�����飬С��һ��*/
	int x=0,y=0,z=0;
	while(1){
		if(x*5+y*3>100){
			break;
		}
		while(1){
			if(x*5+y*3<=100){
				cout<<"�壺"<<x<<"\t"\
					<<"����"<<y<<"\t"\
					<<"һ��"<<100-5*x-3*y<<endl;
				y++;
			}
			else
				break; 
		}
		y=0;
		x++;
	 } 
	 
	
} 

//�Ƿ�Ϊ���� 
bool help_ex029(int x){
	int buf=(int)(sqrt((double)x));
	int t=2;
	if(x==0||x==1) 
		return 0;
	if(x==2||x==3) 
		return 1;
	while(1){	
		if(x%t==0) 
			return 0;
		if(t>=buf) 
			return 1;
		while(1){
			t++;
			if(t==3||(t+1)%6==0||(t-1)%6==0) 
				break;
			if(t>=buf) 
				break;
		}
	}
}
/*ex029:��°ͺղ���*/
void ex029(){
	/*һ��������ż�������ڶ���ʼ*/
	int number=4;
	int i=2;
	while(1){
		if(number>=100)
			break;
		i=2;
		while(1){
			if(i>=number)
				break;
			if(help_ex029(i)&&help_ex029(number-i))
				cout<<number<<"\t"<<i<<"\t"<<number-i<<endl;
			i++;
		}	
		number+=2;
	} 
} 

/*ex030:��Ƴ�˹����*/
bool ex030(int x){
	int number=x*x*x;
	int i=1,j=1,t=0;
	while(1){
		if(i>number/2+1)
			break;
		j=i;
		t=0;
		while(1){
			if(t>number)
				break;		
			if(t==number)
				cout<<number<<"\t"<<i<<endl;
			t+=j;
			j+=2;
		}
		i+=2;
	}
} 

//�Ƿ�Ϊ���� 
bool help_ex033(int x){
	int buf=(int)(sqrt((double)x));
	int t=2;
	if(x==0||x==1) 
		return 0;
	if(x==2||x==3) 
		return 1;
	while(1){	
		if(x%t==0) 
			return 0;
		if(t>=buf) 
			return 1;
		while(1){
			t++;
			if(t==3||(t+1)%6==0||(t-1)%6==0) 
				break;
			if(t>=buf) 
				break;
		}
	}
}
/*ex033:��x��y֮���ȫ������*/
void ex033(int x,int y){
	cout<<x<<"��"<<y<<"֮��ȫ������Ϊ��"<<endl; 
	int i=x;
	while(i++<=y)
		if(help_ex033(i))
			cout<<i<<"\t"<<endl; 
} 

/*ex035:ʮ����תΪ������*/
void ex035(unsigned int x){
	unsigned char temp[32]={0};
	int i=0,flag=1;
	for(i=0;i<32;i++){
		if (x==0) break;
		temp[i]=x%2;
		x/=2;
	}
	for (i = 31; i>= 0;--i){
		if(temp[i]==1)
			cout<<'1';
		if(temp[i]==0)
			cout<<'0';
	}
	cout<<endl;
}

/*ex036:N����תΪʮ����*/
int ex036(unsigned long long x,int n){
	int index=0;
	int number=0;
	int t=0;
	while(1){
		if(x==0)
			return number;
		number+=int(pow(n,t))*(x%10);
		x/=10;	
		t++;	
	}
	return number;	
} 

/*ex037:С����������*/
void ex037(int start){
	int high=start;
	int all_meter=0;
	while(1){
		if(high==0) 
			break;
		all_meter+=high;
		high/=2;

		cout<<"���أ��´ε����߶ȣ�"<<high<<endl; 
		cout<<"���أ��Ѿ����˶�Զ��"<<all_meter<<endl; 
		all_meter+=high;				
	}
	
	
} 

/*ex040:IP��ַת��*/
unsigned char ip[4]={0,0,0,0};
unsigned char ipchar[32]={0};
void ex040(){	
	for(int i=0,j=0;j<4&&i<32;i++){
		ip[j]+=ipchar[i]*int(pow(2,i%8));		
		if((i+1)%8==0)
			j++;
	}
}
 
/*ex044:������*/
int help_ex044(int x){
	int sum=0;
	int half=x/2+1;
	for(int i=1;i<=half;i++){
		if(x%i==0)
			sum+=i;			
	}
	return sum;
} 
void ex044(int x){
	for(int i=1;i<x;i++){
		int t=help_ex044(i);
		if(i==help_ex044(t))
			cout<<i<<"\t"<<t<<endl;	
	}		
}

/*ex045:������*/
void ex045(int x){
	int number=1,buf=0;
	for(int i=1;i<=x;i++){
		int length=1,max1=1;
		buf=i;
		while(!(buf/10==0&&x%10==0)){
			length++;
			buf/=10;
		}		
		while(length--) 
			max1*=10; 		
		if(i*i%max1==i)
			cout<<i<<endl;
	
	
	}
	
}

//���Լ������С������ 
int help_ex046_0(int x,int y){		
	int min,max,buf;	
	min=x<y?x:y;
	max=x<y?y:x;
	while(1){
		if(max%min==0) 
			return min;
		else{
			buf=max/min;
			max=min;
			min=buf;
			}
	}
}
int help_ex046_1(int x,int y){ 
	return x*y/help_ex046_0(x,y);
}
/*ex046:һԪ���β���ʽ����*/
//δ��ɣ��������㷨 
void ex046(int a,int b,int c){
	//ax+by=c;���н� a,b�����Լ������c
	//��(x0,y0)�ɽ⣬����(x0+bK,y0-aK)
	//�κ�����K 
	//a>=b,a>0,b>0
	if(help_ex046_0(a,b)%c==0){
		cout<<"�޽⣡"<<endl;
		return; 
	}
	else{
		
	}
 
} 

//�Ƿ�Ϊ���� 
bool help_ex047(int x){
	int buf=(int)(sqrt((double)x));
	int t=2;
	if(x==0||x==1) 
		return 0;
	if(x==2||x==3) 
		return 1;
	while(1){	
		if(x%t==0) 
			return 0;
		if(t>=buf) 
			return 1;
		while(1){
			t++;
			if(t==3||(t+1)%6==0||(t-1)%6==0) 
				break;
			if(t>=buf) 
				break;
		}
	}
}
/*ex047:��������*/ 
bool ex047(int number){
	int buf=number;
	if(help_ex047(number)==0)
		return false;
	else
	{
		int t=0;
		while(1){
			if(buf<=0)
				break;
			else{
				t*=10;
				t+=buf%10;
				buf/=10;	
			}
		}
		if(help_ex047(t)==0){
			cout<<number<<"\t"<<t<<endl;
		return true;
	}
	}
	return false;
} 

/*ex048:�ж�����*/
bool ex048(int x){
	if((x%4==0)&&(x%100!=0))
		return true;
	else if(x%400==0)
		return true;
	else
		return false;
}

/*ex051:���Լ����С������*/
int ex051_0(int x,int y){		
	int min,max,buf;	
	min=x<y?x:y;
	max=x<y?y:x;
	while(1){
		if(max%min==0) 
			return min;
		else{
			buf=max/min;
			max=min;
			min=buf;
			}
	}
}
int ex051_1(int x,int y){ 
	return x*y/ex051_0(x,y);
}

/*ex052:��һԪ���η��̵ĸ�*/
void ex052(double a,double b,double c){
	double det,x1=0,x2=0;
	if(b*b-4*a*c<0){
		cout<<"�޽⣡"<<endl;
		return; 
	}
	else{
		det=sqrt(b*b-4*a*c);
		x1=(-b+det)/(2*a);
		x2=(-b-det)/(2*a);
		if(x1==x2)
			cout<<"��Ϊ��"<<x1<<endl; 
		else
			cout<<"��Ϊ��"<<x1<<"\t"<<x2<<endl; 	
	}		
}

//��x�� 
unsigned long long help_ex053(int x){
	int i=1;
	unsigned long long result=1;
	for(i=1;i<=x;i++){
		result*=i;
	}
	return result;
}
/*ex053:����Ȼ����e�ļ���*/
double ex053(int x){
	double e=1.0;
	for(int i=1;i<x;i++)
		e+=1/double(help_ex053(i));
	return e;
} 

/*ex054:��abcd=(ab+cd)^2*/
void ex054(){
	for(int abcd=1000;abcd<10000;abcd++){
		int ab=abcd/100,cd=abcd%100;		
		if(abcd==(ab+cd)*(ab+cd))
			cout<<abcd<<endl;
	}
}

/*ex107:ת�þ���*/
int list[4][4]={{1,2,3,4},
{5,6,7,8},{9,10,11,12},{13,14,15,16}};
int x=3,y=3;
void ex107(){	
	//��ά���鲻�����⴫�Σ������ͨ 	
	if(x!=y)
		return;	
	for(int i=0,j=0;i<=x&&j<=y;i++,j++){
		for(int k=0;k+i<=x&&k+j<=y;k++){
			int temp;
			temp=list[i+k][j];
			list[i+k][j]=list[i][k+j];
			list[i][k+j]=temp;
		}							
	}
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++)
			cout<<list[i][j]<<"\t";
		cout<<endl;
	}
} 
	
/*ex108:ħ����*/
//δ��ɣ����� д��

/*ex110:���ƶ�λ�õĲ����ַ�*/ 
//δ��ɣ��ַ������Ѿ��̶�������һ���ַ��������� 
//�����Խ�����⣬�Ƿ���Դ�����ʱ�ռ䣬Ȼ���������
//��󷵻ش����ݸ���ǰ�� 
void ex110(char* str,int n,char c){
	int length=0;
	char* s=str;
	while(*s++!='\0')
		length++;
	if(n>length-1)
		return;	
	str[length+1]='\0';
	while((--length)>=n)
		str[length]=str[length-1];
	str[n]=c;		
} 

/*ex118:�ж�����*/ 
bool ex118(int x){
	int buf=(int)(sqrt((double)x));
	int t=2;
	if(x==0||x==1) 
		return 0;
	if(x==2||x==3) 
		return 1;
	while(1){	
		if(x%t==0) 
			return 0;
		if(t>=buf) 
			return 1;
		while(1){
			t++;
			if(t==3||(t+1)%6==0||(t-1)%6==0) 
				break;
			if(t>=buf) 
				break;
		}
	}
}

/*ex171:�����ַ���*/
//δ���
//˼·����д������������������ַ���˳��Ƚ�
//��˳��ð�ݱȽϼ��� 
void help_ex171(char* str1,char* str2){
	char* buf;char*s1=str1;char*s2=str2;
	while(*s1!='\0'&&*s2!='\0')
		while(*(s1++)>*(s2++)){
			buf=str1;
			str1=str2;
			str2=buf;
			cout<<str1<<endl<<str2<<endl;
		}
}
void ex171(char** list,int left,int right,int n){
	for(int i=left;i<=right;i++){
		char* str=list[i];
		
	
	
		
	}
}

//λ���� 

/*ex215:�ض���Χλ��ת*/
//���ض���λ��1���������λ��0��򣬼��ɷ�ת 
unsigned ex215(unsigned x,int low,int high){
	unsigned temp=0;
	temp=~temp;
	temp=(temp<<low)&(temp>>32-high-1);
	return x^temp;
} 

/*ex217:ѭ����λ*/
unsigned ex217(unsigned x,int n,bool flag){
//��������λֻ�� ��x|y  
//����ǰ��xλ���������yλ��Ȼ�󽻲���λ
//���������������
//Flag==0,ѭ�����ƣ�Flag==1��ѭ������  	
	if(flag==1)
		n=32-n;
	return x>>32-n|x<<n;	
} 

/*ex220:��һ�����Ĳ���*/
int ex220(int x){
	if(x>=0)
		return x;
	else
		return (~x)|(1<<31)+1;
} 

/*ex259:RSA�����㷨*/
int ex259(int x){
	//1.����ѡ�������pq������n=p*q
	//2.����f=(p-1)*(q-1)
	//3.e��f���ʣ�e<f 
	//4.d=e%f
	//���ܣ�c=��m^e)%n	//p,q,e��Կ 
	//���ܣ�m=��c^d)%n  //d,n˽Կ 
} 

/*ex261:DES�����㷨*/
int ex261(int x){
	//1.ȡ����Կ��ȡ��һ��64λ������key
	//key=k1k2����k64��ȥ����8,16,24,32,40,48,56,64
	//��Ϊ��żУ��λ��ʣ��56λ��Ϊ��Ч������Կ
	
	//2.�ȷ���ԿΪAB�����֣�ÿ����28λ 
	//����λ�����Ӧ���ж�Ӧλ���γ�
	//key=k1k2k3����k55k56
	//A	 =k57k49k41����k44k36
	//B	 =k63k55k47����k12k4
	
	//3.��Կ��λ
	//��AB����i�ε���ѭ�����ƣ����ݱ�ȷ����λλ��
	
	//4.ѡȡ��Կ
	//�ϲ�AB C=AB(��˳������) 
	//�α�k=C14C17C11����C29C32 
	//	   =K1K2K3����K47K48 
	
	//5.������DES�㷨��Կ������Ҫ16�ε�����
	//���ǰѭ��3,4�� �������16�׼�����Կ 
	//key[0]����key[15] 
	 
} 


#ifdef __cplusplus
	}
#endif	
#endif
