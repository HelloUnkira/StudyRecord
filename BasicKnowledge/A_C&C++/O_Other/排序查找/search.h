#ifndef __SEARCH_H_
#define __SEARCH_H_

#ifdef __cplusplus
	extern "C" {
#endif	

#include<stdio.h>
#include<malloc.h>
//˳����� 
int sequence_search(int* list,int left,int right,int target){
	int i=left;
	while(i++<=right)
		if(list[i]==target)
			return i;
	return -1;
}

//���ֲ��ң����ض�Ӧ�����±��������Ҳ���Ϊ-1 
//Ҫ���������� 
int binary_search(int* list,int left,int right,int target){
	if(left>=right&&list[left]!=target) 
		return -1;
	else{
		int center=(left+right)/2;
		if(target==list[center]) 
			return center;
		else if(target<list[center])
			return binary_search(list,left,center,target);
		else
			return binary_search(list,center+1,right,target);
	}
}

//��ֵ����
/*
*����ֲ������ƣ�ֻ���޸��е�ֵΪ��������ֵ����
* center=left+(target-list[left])/(list[right]-list[left])*(right-left);
*/

//쳲���������
/*����ֲ������� 
*Ҫ�����������������ĳһ��쳲���������С1 F(k)-1
* center=left+F(k-1)-1 
* center==target ��Ϊ����ֵ
* center<target  ����left,center-1�У�k-=1
* center>target  ����center+1��right�У�k-=2 
*/ 

//��������
/*����Ҫ���� 
*����һ�Ŷ�������� 
*���м�λ�õݹ鹹�����ɣ����б���Ԫ��ֵ�����±�
*�ݹ�Ĵ��м乹������֪��left==right���ɹ������ 
*֮��ʹ����������ķ������� 
*/ 
//����������Ļ���������������ȫ�������ݲſ� 
//����Ч�������ĸ߶��йأ���Ϊ���Լ�С�ݹ��� 
struct tree{
	int data;
	int index;
	
	tree* leftChild;
	tree* rightChild;
}; 
//�������������ظ��ڵ�ָ�� 
tree* createTree(int*list,int left,int right){
	struct tree* treeBuf=(tree*)malloc(sizeof(tree));
	if(left>=right){
		treeBuf->data=list[left];
		treeBuf->index=left;
		treeBuf->leftChild=NULL;
		treeBuf->rightChild=NULL;
		return treeBuf;
	}
	else{
		int center=(left+right)/2;
		treeBuf->data=list[center];
		treeBuf->index=center;
		treeBuf->leftChild=createTree(list,left,center-1);
		treeBuf->rightChild=createTree(list,center+1,right);
		return treeBuf;
	}
}
//ʹ�ø��ڵ�ɾ���� 
void destoryTree(tree* Root){
	if(Root->leftChild==NULL&&Root->rightChild==NULL){
		delete Root;
		Root=NULL;		
	}
	else if(Root->leftChild==NULL){
		destoryTree(Root->rightChild);
		delete Root;
		Root=NULL;
	}
	else if(Root->rightChild==NULL){
		destoryTree(Root->leftChild);
		delete Root;
		Root=NULL;		
	}
	else{
		destoryTree(Root->leftChild);
		destoryTree(Root->rightChild);
		delete Root;
		Root=NULL;		
	}
}
//ʹ�ø��ڵ�������� 
int searchTree(tree* Root,int target){
	if(Root->leftChild==NULL&&Root->rightChild==NULL){
		if(Root->data==target)
			return Root->index;
		else
			return -1;
	}
	else if(Root->rightChild==NULL){
		if(Root->data<target)
			return -1;
		else
			return searchTree(Root->leftChild,target);
	}
	else if(Root->leftChild==NULL){
		if(Root->data>target)
			return -1;
		else
			return searchTree(Root->rightChild,target);
	}
	else{
		if(Root->data>target)
			return searchTree(Root->leftChild,target);
		else
			return searchTree(Root->rightChild,target);
	}
}

//�ֿ����
/*
*��Χ�ķֿ飬С��Χ�ڲ��ң����տ������ֵ������������ 
*/ 

struct block{
	int maxdata;
	int indexstart;
	int indexend;
};

int block_search(int* list,int left,int right,int target){
	int max=0,i=0,j=0,index,length=5,offset=(right-left+1)/length;
	block* all=(block*)malloc(sizeof(block)*length);
	block buf={0,0,0};
	//�ݶ������ ���������Ӧ�ø������ݳ����ж� 
	for(i=0,max=0;i<length;i++){
		for(j=left+i*offset;j<=right&&j<(left+(i+1)*offset);j++)
			if(max<list[j]) max=list[j];
			all[i].maxdata=max;
			all[i].indexstart=left+i*offset;			
			all[i].indexend=j;
	} 
	
	//����������
	for(i=0;i<length;i++)
		for(j=i;i+j<length;j++)
			if(all[i].maxdata>all[i+j].maxdata){
				buf.maxdata=all[i].maxdata;
				buf.indexstart=all[i].indexstart;
				buf.indexend=all[i].indexend;
				all[i].maxdata=all[i+j].maxdata;
				all[i].indexstart=all[i+j].indexstart;
				all[i].indexend=all[i+j].indexend;
				all[i+j].maxdata=buf.maxdata;
				all[i+j].indexstart=buf.indexstart;
				all[i+j].indexend=buf.indexend;
			} 
	if(all[0].maxdata>=target)
		index=0;
	else
		for(i=0;i<length;i++){
			if(i+1<length)
				if(all[i].maxdata<=target&&all[i+1].maxdata>target){
					index=i;
					break;
				}
			else
				index=i;
		}
	i=sequence_search(list,all[index].indexstart,\
					all[index].indexend,target);

	free(all);	
	return i;
} 

//��ϣ����
/*
*ֻ��Ҫ�������ݹ���ɢ�б�����
*�ɲ���������ʽ��������Լ�ռ� 
*/ 

//int test[10]={2,7,13,37,45,56,67,72,84,93};
/*
tree* root=createTree(test,0,9);
int index=searchTree(root,317);
destoryTree(root);
*/


#ifdef __cplusplus
	}
#endif	
#endif
