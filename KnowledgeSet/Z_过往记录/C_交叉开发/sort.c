#include"sort.h"
#include<stdio.h>
#include<math.h>
#include<malloc.h>
#include<stdlib.h>

static void swap(int*list,int i,int j){//��������ָ���±��������
	int buf=0;
	buf=list[i];
	list[i]=list[j];
	list[j]=buf;
}

void bubble_sort(int* list,int left,int right){//ð��O(n^2)
	int i=0,j=0;
	for(i=left;i<=right;i++)
		for (j =0; i+j<=right; ++j)
			if(list[i]>list[i+j])
					swap(list,i,i+j);
}

void selection_sort(int* list,int left,int right){//ѡ��o(n^2)	
	int i=0,j=0,temp=0,buf=0;
	for(i=left;i<=right;i++){
		temp=list[i];
		for (j=0;i+j<=right;++j){
			if(temp>list[i+j]){
				buf=temp;
				temp=list[i+j];
				list[i+j]=buf;
			}
		}
		list[i]=temp;
	}	
}

void insertion_sort(int* list,int left,int right){//����
	int n=right+1;
	int i=left,j=0,temp;
	for(i=left;i<=right;i++){
		temp=list[i+1];	
		for(j=i;j>=left;j--){
			if(temp<list[j])
				list[j+1]=list[j];
			else break;
		}
		list[j+1]=temp;
	}
}

static int Hibbard(int x){//ϣ���������е�һ��
	return (x+1)/2;
}

void shell_sort(int* list,int left,int right){//ϣ��
	int i,j,s,temp;
	int k=right-left;
	while(1){		   
	k=Hibbard(k);
	for(s=left;s<left+k;s++){	
	   	for(i=s;i+k<=right;i+=k){
		    temp=list[i+k];	
	    	for(j=i;j>=left;j-=k){
			    if(temp<list[j])
			    	list[j+k]=list[j];
			    else break;
		    }
	    	list[j+k]=temp;
		}
	}	
	if(k<=1)break;
	}
}

//���η�:
//��ʱ�Եݹ�ﵽ��Χ��С������,
//���Ƕ����ⷶΧ��������,�ڻ���ʱ��С��Ϊ������׼��
//����������ϲ�Ϊһ������
//���η��ؼ��Ƕ��ε����
static void merge(int* list,int*temp,int left,int center,int right){
	//��������left��center�У��������������
	//��������center+1��right�У��������������
	int i=left,j=center+1,k=left;//
	while(1){
		//ȡ����ֵ���ȽϺ�д��temp����������
		if(i>center){
			temp[k]=list[j];
			j++;
		}
		else if(j>right){	
			temp[k]=list[i];
			i++;
		}
		else if(list[i]<list[j]){
			temp[k]=list[i];
			i++;
		}
		else{
			temp[k]=list[j];
			j++;
		}		
		k++;
		if(k>right) 
			break;
	}
	k=0;
	while(1){
		list[k]=temp[k];
		k++;
		if(k>right) 
			break;
	}
}

void merge_sort(int* list,int*temp,int left,int right){//�鲢
	int center=(left+right)/2;		
	if(left>=right) 
		return;
	merge_sort(list,temp,left,center);
	merge_sort(list,temp,center+1,right);	
	merge(list,temp,left,center,right);	
}

static int partition(int* list,int left,int right){
	int bufleft=left,bufright=right;
	int middle=0,index=0;//middle=list[index];
	int center=(left+right)/2;
	if(left>=right) 
		return 0;
	//��׼ѡȡ	
	if((list[left]<=list[center]&&list[center]<=list[right])||\
		(list[right]<=list[center]&&list[center]<=list[left])){
		middle=list[center];
		index=center;
	}
	else if((list[center]<=list[left]&&list[left]<=list[right])||\
		(list[right]<=list[left]&&list[left]<=list[center])){
		middle=list[left];	
		index=left;
	}
	else if((list[center]<=list[right]&&list[right]<=list[left])||\
		(list[left]<=list[right]&&list[right]<=list[center])){
		middle=list[right];
		index=right;
	}
	//��������
	while(1){			
		while(list[bufleft]<=middle)
			bufleft++;		
		while(list[bufright]>=middle)
			bufright--;	
		if(bufleft>=bufright)
			break;
		if(list[bufright]!=middle&&list[bufleft]!=middle)
			swap(list,bufleft,bufright);
	}
	do{	
		if (bufleft<bufright){
			if(bufleft>=index){
				swap(list,bufleft,index);
				index=bufleft;
			}
			if(bufright<=index){
				swap(list,bufright,index);
				index=bufright;
			}
		}				
		else{
			if(bufright>=index){
				swap(list,bufright,index);
				index=bufright;
			}
			if(bufleft<=index){
				swap(list,bufleft,index);
				index=bufleft;
			}
		}
		
	}while(0);
	return index;
}

void quick_sort(int* list,int left,int right){//����
	if(left<right){
		int n=right-left+1;	
		int index=partition(list,left,right);
		if(index==0)
			return;	
		quick_sort(list,left,index-1);
		quick_sort(list,index+1,right);
	}
	else
		return;
}

void heap_sort(int* list,int n){//������
	int i=0,iMax=0;
	int scale=n-1;
	int father,left_child,right_child;
	//��ʼλ�ô�i=0��ʼ,�����Һ���Ϊ2i+1,2i+2;
	//��������Ҷ�ӽڵ�Ĳ�iMax
	while(1){	
		if(2*i+2<n) 
			i++;
		else{
			iMax=i;
			break;
		}
	}
	//��������Ϊ�ѣ������¶��ϵ�����	
	for (i = iMax; i>=0; --i){	
		father=i;
		while(1){		
			left_child=2*father+1;
			right_child=2*father+2;
			if(right_child>scale&&left_child>scale)
				break;//�˽ڵ㲻�������Һ���
			else if(right_child>scale&&left_child<=scale){
				//�˽ڵ����Һ���
				if(list[left_child]<list[father])
					break;//���ڵ�����������
				else{
					swap(list,left_child,father);
					father=left_child;
				}
			}
			else if(right_child<=scale){
				if(list[father]>list[right_child]&&\
					list[father]>list[left_child])
					break;//���ڵ�����������					
				else if(list[right_child]>list[left_child]){
					//���ӽڵ�һ����һ����
					swap(list,father,right_child);
					father=right_child;
				}
				else{
					swap(list,father,left_child);
					father=left_child;
					}
			}
		}		
	}	
	i=n-1;
	while(1){
		if(i<=0) 
			break;
	//������ͷ���β���ѷ�Χ��С1��	
		swap(list,0,i);
		i--;
		father=0;
	//���µ�������Ϊ�ѣ������϶��µ�����
	//����һ�κ������Ҫ�����ӽڵ�������ڵ��������
	//�������Ҫ��������ѭ���˳�������־��ѭ���������
		while(1){		
			left_child=2*father+1;
			right_child=2*father+2;
			if(right_child>i&&left_child>i)
				break;//�˽ڵ㲻�������Һ���
			else if(right_child>i&&left_child<=i){
				//�˽ڵ����Һ���
				if(list[left_child]<list[father])
					break;//���ڵ�����������
				else{
					swap(list,left_child,father);
					father=left_child;
					}
			}
			else if(right_child<=i){
				if(list[father]>list[right_child]&&\
				  	list[father]>list[left_child])
					break;//���ڵ�����������				
				else if(list[right_child]>list[left_child]){
					//���ӽڵ�һ����һ����
					swap(list,father,right_child);
					father=right_child;
				}
				else{
					swap(list,father,left_child);
					father=left_child;
				}
			}
		}		
	}
}

void counting_sort(int* list,int left,int right){//��������
	int i=0,j=0;
	unsigned char*count=NULL;
	int max=list[left],min=list[left];
	for(i=left+1;i<=right;i++){
		if(list[i]>max) max=list[i];
		if(list[i]<min) min=list[i];
	}
	count=(unsigned char*)malloc(\
		   sizeof(unsigned char)*(max-min+1));
	//����������ƫ����Ҫ��min
	for(i=0;i<max-min+1;i++)
		count[i]=0;
	for(i=left;i<=right;i++)
		count[list[i]-min]++;
	for(i=0,j=left;i<max-min+1;i++){
		while(1){
			if(count[i]>0){
				list[j]=i+min;
				count[i]--;
				j++;
			}
			else 
				break;
		}
	}
	free(count);
}

//����������ʼ��
static bucket* createNode(bucket bk){
	struct bucket*node=(bucket*)malloc(sizeof(bucket));
	node->bk_next=NULL;
	node->bk_number=bk.bk_number;
	return node;
}
//�ͷ��б�,δ����
static void freeNode(bucket*bk){
	if(bk->bk_next==NULL){
		free(bk);
		bk=NULL;
	}
}
//˳�����Ԫ��
static void insertNode(bucket** bk_p,int n,bucket* bk){		
	//��������ָ�룬ָ����ʱ���ĵ����ݽṹ��	
	struct bucket* pMove=NULL;
	pMove=bk_p[n];
	if(pMove==NULL)//�����ͷ�����ݲ嵽��ͷ����
		bk_p[n]=bk;
	else if(pMove->bk_number>bk->bk_number){
		//���һ����С��Ҫ��������������������һ��Ԫ��
		bk->bk_next=pMove;
		bk_p[n]=bk;
	}
	else
		while(1){
			if(pMove->bk_next==NULL && pMove->bk_number<=bk->bk_number){
				//���һ����С��Ҫ��������������������һ��Ԫ��
				pMove->bk_next=bk;
				break;
			}
			else if(pMove->bk_next!=NULL&&pMove->bk_number<=bk->bk_number&&\
				pMove->bk_next->bk_number<=bk->bk_number){
				//���һ�����Լ������������С�ڸ�Ŀ��
				//�ƶ�����һ����
				pMove=pMove->bk_next;
			}
			else{	
				bk->bk_next=pMove->bk_next;
				pMove->bk_next=bk;
				break;
			}	
		}
}
//˳�򵯳�Ԫ��
static int removeNode(bucket**bk_p,int n){
	int buf=bk_p[n]->bk_number;
	struct bucket* bkBuf=bk_p[n];
	//������ֻ��һ��Ԫ����
	if(bk_p[n]->bk_next==NULL)
		bk_p[n]=NULL;
	else
		bk_p[n]=bk_p[n]->bk_next;
	free(bkBuf);
	return buf;
}

static void helpPrintf(bucket** bk_p,int n){//������ӡ��ά����
	struct bucket* pBucket;
	int i=0;
	for(i=0;i<=n;i++){
		pBucket=bk_p[i];
		while(1){
			if(pBucket==NULL) 
				break;
			printf("%d ",pBucket->bk_number);
			pBucket=pBucket->bk_next;
		}
	}
	printf("\n");
}

//Ͱ����һ���������ݷ�Χ��֪�����
//��Ȼһ������»���ڳ��ڴ棬����������
static void single_bucket_sort(int* list,int left,int right){//����Ͱ����
	int n=right-left+1;
	int i,t,max=list[left],min=list[left];
	struct bucket** bk_pointer=NULL;
	struct bucket* bk_buf=NULL;
	for(i=left;i<=right;i++){
		if(max<list[i]) max=list[i];
		if(min>list[i]) min=list[i];
	}
	//���䣺 ӳ�亯��һ���� f = array[i] / k; 
	//k^2 = n; n������Ԫ�ظ��� 
	if(max==min)
		return;
	//k��ȷ������Щ����
	//����Ͱ��
	bk_pointer=(bucket**)malloc(sizeof(bucket*)*(n+1));
	//��ʼ��Ͱ
	for(i=0;i<n+1;i++)
		bk_pointer[i]=NULL;
	//������Ҫ����Ĵ�����ɾ�������룬���	
	for(i=left;i<=right;i++){
		int Ibuf=(int)((list[i]-min)*n/(max-min));
		struct bucket buf;
		struct bucket* pBuf;
		buf.bk_number=list[i];
		buf.bk_next=NULL;
		pBuf=createNode(buf);	
		while(Ibuf+1<n&&bk_pointer[Ibuf+1]!=NULL&&\
			bk_pointer[Ibuf+1]->bk_number<buf.bk_number)
			Ibuf++;
		insertNode(bk_pointer,Ibuf,pBuf);
	}
	//helpPrintf(bk_pointer,n);
	i=0;
	t=left;
	while(1){
		if(i>n) 
			break; 
		while(1){	
			if(bk_pointer[i]!=NULL){	
				list[t]=removeNode(bk_pointer,i);
				t++;
			}
			else
				break;
		}
	i++;
	}
	free(bk_pointer);
}
//�ݹ�Ͱ����δʵ��
void bucket_sort(int* list,int left,int right){	
	single_bucket_sort(list,left,right);
}

static void helpPrintfRadix(radixBucket** bk_p,int n){//������ӡ��ά����
	struct radixBucket* pBucket;
	int i=0;
	for(i=0;i<=n;i++){
		pBucket=bk_p[i];
		while(1){
			if(pBucket==NULL) 
				break;
			printf("%d ",pBucket->number);
			pBucket=pBucket->next;
		}
	}
	printf("\n");
}

static radixBucket* createRadixNode(radixBucket Node){
    struct radixBucket*node=(radixBucket*)malloc(sizeof(radixBucket));
	node->next=NULL;
	node->number=Node.number;
	return node;
}

static void insertRadixNode(radixBucket**radix,int n,radixBucket* Node){
	struct radixBucket*pradix=radix[n];
	if(radix[n]==NULL)
 	    radix[n]=Node;
	else{
		while(pradix->next!=NULL)
   			pradix=pradix->next;
		pradix->next=Node;
	}	
}

static int removeRadixNode(radixBucket**radix,int n){
 	int buf=0;
 	struct radixBucket*pradix=radix[n];
	if(radix[n]==NULL)
		return -1;
	else{
		radix[n]=radix[n]->next;
		buf=pradix->number;
		free(pradix);
	}
	return buf;
}

int radix_sort(int* list,int left,int right,int index){	
	int bufNumber=1,bufNumber1=1; 
	int cindex[13]={2,8,10,16,32,50,64,100,128,200,256,400,512};
	int i,j,t=0,length=1,max=list[left];
	//????t? 
	struct radixBucket**thisradix=(radixBucket**)malloc(sizeof(radixBucket*)*cindex[index]);
	for(i=0;i<cindex[index];i++)
		thisradix[i]=NULL;
	for(i=left;i<=right;i++)
		if(max<list[i])max=list[i];
	while((max/=cindex[index])!=0)
        length++;
	while(length--){
		bufNumber*=cindex[index];
	   	for(i=left;i<=right;i++){
			int x=list[i]%bufNumber/bufNumber1;						
			struct radixBucket* Pbufradix;
			struct radixBucket bufradix;
			bufradix.next=NULL;
			bufradix.number=list[i];
			Pbufradix=createRadixNode(bufradix);
			insertRadixNode(thisradix,x,Pbufradix);
		}	
		for(t=0,i=left;i<=right;i++){	
			int x=removeRadixNode(thisradix,t);
			if(t>=cindex[index])break;
			if(x==-1){
				t++;
				i--;
			}			  
		    else
 				list[i]=x;
		}			
		bufNumber1*=cindex[index];
	}	
	free(thisradix);
}


