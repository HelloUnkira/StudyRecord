#ifndef __TIMO_HPP_
#define __TIMO_HPP_

#include<iostream>
#include"sort.h"
#include"search.h" 

using namespace std;
//��������ҵ���Ŀ

//timo1����ɶ����������˫�������ת��
template <typename T>
class TreeToList
{
	private:
		T data;
		TreeToList* left_last;	//��ʾ��-��ڵ㣬��ʾ����-��һ�ڵ� 
		TreeToList* right_next;	//��ʾ��-�ҽڵ㣬��ʾ����-��һ�ڵ� 
	public:
	//�������ݣ� 
	TreeToList()
	{
		this->data      = T(-1);
		this->left_last  = NULL;
		this->right_next = NULL;
	}
		
	TreeToList(T Data)
	{
		this->data      = Data;
		this->left_last  = NULL;
		this->right_next = NULL;
	}		
			
	~TreeToList()
	{
	}
	
	//��ɶ����������to���б�Ĵ��� 
	//����һ������������飬������� 
	TreeToList* createTreeToList(T* list, int left, int right)
	{
		if (left > right)
			return NULL;
		else if (left == right) {
			TreeToList* pBuf = new TreeToList(list[left]);
			return pBuf;
		}
		else {		
			int center = (left + right) / 2;			
			TreeToList* pBuf = new TreeToList(list[center]);					 
			pBuf->left_last = createTreeToList(list, left, center-1);
			pBuf->right_next = createTreeToList(list, center+1, right);				
			return pBuf;		
		}		
	}
	
	void helpPrintTree(TreeToList* Root)
	{
		if (Root == NULL)
			return;
		else {								
			if (Root->left_last != NULL)
				helpPrintTree(Root->left_last);	
			cout<<Root->data<<" ";	
			if (Root->right_next != NULL)
				helpPrintTree(Root->right_next);						
		}		
	}
	
	//������������ת�� 
	void changeTreeToList(TreeToList* Root, TreeToList* Ancester, int flag)
	{	
		//�����Ҷ�ӽڵ� 
		if (Root->left_last == NULL && Root->right_next == NULL)
			return;					
		else {
			if (Root->left_last != NULL) {	
				changeTreeToList(Root->left_last, Root, 0);									
				TreeToList* pBuf = Root->left_last;
				while (pBuf->right_next != NULL)
					pBuf = pBuf->right_next;
				pBuf->right_next = Root;	
				Root->left_last  = pBuf;								
			}

			if (Root->right_next != NULL) {				
				changeTreeToList(Root->right_next, Root, 1);													
				TreeToList* pBuf = Root->right_next;
				while (pBuf->left_last !=NULL)
					pBuf = pBuf->left_last;
				pBuf->left_last  = Root;	
				Root->right_next = pBuf;			
			}			
		}																							
	}
	
	TreeToList* finishTreeToList(TreeToList* Root)
	{
		changeTreeToList(Root, Root, 100);
		TreeToList* pBuf = Root;
		while (pBuf->left_last != NULL)
			pBuf = pBuf->left_last;		
	return pBuf;	
	}
	
	void helpPrintList(TreeToList* Root)
	{
		for (TreeToList* pBuf = Root; pBuf != NULL; pBuf = pBuf->right_next)
			cout<<pBuf->data<<" ";		
	}
	
	void destoryTreeToList(TreeToList* Root)
	{
		TreeToList* pBuf = Root;
		while (1) {
			if (pBuf == NULL)
				break;
			delete pBuf;				
			Root = Root->right_next;
		 	pBuf = Root;
		}
	}
		
};
 
void testTreeToList()
{
	int i=0;
	int test[30]={0};
	
	for(i=0;i<30;i++)
		test[i]=rand()%100;
	heap_sort(test,29);
	
	for(i=0;i<30;i++)
		cout<<test[i]<<" ";
	cout<<endl;
		
	TreeToList<int>* aHa = NULL;
	TreeToList<int>* treetolist = NULL;
	treetolist = (*aHa).createTreeToList(test,0,29);	
	cout<<endl;
	(*aHa).helpPrintTree(treetolist);
	cout<<endl;	
	treetolist = (*aHa).finishTreeToList(treetolist);
	cout<<endl;	
	(*aHa).helpPrintList(treetolist);
	cout<<endl;
	(*aHa).destoryTreeToList(treetolist);
}

//timo2�����һ��ջ���������ѯջ������Сֵ�Լ���ջ��ջ
//�������ʱ����ΪO(1) 
template <typename T>
class SearchStack
{
	private:
		static int maxNumber;
		int index;
		SearchStack* lastNode;
		SearchStack* nextNode;
		T data;
	public:
	SearchStack()
	{
		this->data     = T(-1);
		this->index    = 0;
		this->lastNode = NULL;
		this->nextNode = NULL; 
	}
	SearchStack(T Data, int Index)
	{
		this->data     = Data;
		this->index	   = Index;
		this->lastNode = NULL;
		this->nextNode = NULL;			
	}
	
	~SearchStack()
	{
	}
	
	T getData()
	{
		return this->data;
	}
	
	SearchStack** createSearchStack()
	{
		SearchStack** AllStack = new SearchStack*[2];
		AllStack[0] = new SearchStack();
		AllStack[1] = new SearchStack();
		AllStack[0]->nextNode = AllStack[1];
		AllStack[1]->lastNode = AllStack[0];	
		maxNumber = 0;			
		return AllStack; 
	}
	
	//��ʱʵ��min��O(1)���� 
	//�޸� buf->data <= start->nextNode->data Ϊ>=���� 
	void popdownSearchStack(SearchStack** AllStack, SearchStack* Node)
	{
		maxNumber++;
		SearchStack* buf = new SearchStack(Node->data,maxNumber);
		SearchStack* start = AllStack[0];
		SearchStack* end = AllStack[1];
		if (start->nextNode == end && end->lastNode == start) {
			buf->nextNode = end;
			buf->lastNode = start;
			start->nextNode = buf;
			end->lastNode = buf;
		} else if (buf->data <= start->nextNode->data) {
			buf->nextNode = start->nextNode;
			buf->lastNode = start;
			start->nextNode->lastNode = buf;
			start->nextNode = buf;
		} else {
			buf->nextNode = end;
			buf->lastNode = end->lastNode;
			end->lastNode->nextNode = buf;
			end->lastNode = buf;
		}							
	}
	
	void popupSearchStack(SearchStack** AllStack, SearchStack* Node) 
	{
		SearchStack* start = AllStack[0];
		SearchStack* end = AllStack[1];
		SearchStack* buf =NULL;
		if (start->nextNode == end && end->lastNode == start)
			return;
		if (start->nextNode->index >= end->lastNode->index) {
			Node->data = start->nextNode->data; 
			buf = start->nextNode;
			start->nextNode = buf->nextNode;
			buf->nextNode->lastNode = start;
			delete buf;
		} else {
			Node->data = end->lastNode->data; 			
			buf = end->lastNode;
			end->lastNode = buf->lastNode;
			end->lastNode->nextNode = end;
			delete buf;
		}
		maxNumber--;
	}
	
	void destorySearchStack(SearchStack** AllStack)
	{
		SearchStack* start = AllStack[0];
		SearchStack* buf = start;
		while (buf != NULL) {
			start = start->nextNode;
			delete buf;
			buf = start;
		}
		delete [] AllStack;
	}
	
	SearchStack* searchSearchStack(SearchStack** AllStack)
	{
		return AllStack[0]->nextNode;
	}
	
};

template <typename T>
	int SearchStack<T>::maxNumber = 0;

void testSearchStack()
{
	SearchStack<int>* aHa;
	SearchStack<int>** all = (*aHa).createSearchStack();
	int i=0;
	int test[30]={0};
	
	for (i=0;i<30;i++) {
		test[i]=rand()%100;
		cout<<test[i]<<" ";			
		SearchStack<int>* pBuf = new SearchStack<int>(test[i], 0);	
		(*aHa).popdownSearchStack(all, pBuf);		
	
	delete pBuf; 
	}
		
	cout<<endl;		
	
	cout<<(*(*aHa).searchSearchStack(all)).getData();
		
	cout<<endl;
	
	for (i=0;i<30;i++) {		
		SearchStack<int>* pBuf = new SearchStack<int>();	
		(*aHa).popupSearchStack(all, pBuf);				
		cout<<(*pBuf).getData()<<" ";	
	delete pBuf; 
	}

		
	(*aHa).destorySearchStack(all);
}

//timo3����̬�滮�������������
void testMinArraySum()
{
	int test[10] = {1,-2,5,4,-6,9,-3,7,-6,13};
	int max=0,sum=0;
	for (int i =0; i < 10; i++){
		sum += test[i];
		if(sum < test[i])
			sum = test[i];
		if(max < sum)
			max = sum;
	}	
	cout<<max<<endl;
} 

//timo4����������·�����������ݺ�
template <typename T> 
class SearchTreeSum
{
	private:
		static int maxLevel;
		T data;
		SearchTreeSum* leftChild;
		SearchTreeSum* rightChild;
	public:
		SearchTreeSum()
		{
			this->data = T(-1);
			this->leftChild = NULL;
			this->rightChild = NULL;
		}
		
		SearchTreeSum(T Data)
		{
			this->data = Data;
			this->leftChild = NULL;
			this->rightChild = NULL;
		}
		
		~SearchTreeSum()
		{
		}
		
		T getData()
		{
			return this->data;
		}
		
		//���ǽ������������ݣ����򹹽� 
		SearchTreeSum* createSearchTreeSum(T* list, int left, int right)
		{
			maxLevel++;
			if (left > right)
				return NULL;
			else if (left == right) {
				SearchTreeSum* pBuf = new SearchTreeSum(list[left]);				
				return pBuf;
			} else {
				int center = (left + right) / 2;
				SearchTreeSum* pBuf = new SearchTreeSum(list[center]);				
				pBuf->leftChild = createSearchTreeSum(list, left, center - 1);
				pBuf->rightChild = createSearchTreeSum(list, center + 1, right);
				return pBuf;
			}
		}
		
		void destorySearchTreeSum(SearchTreeSum* Root)
		{
			if(Root->leftChild == NULL && Root->rightChild == NULL)
				return;
			else {
				if(Root->leftChild != NULL)
					destorySearchTreeSum(Root->leftChild);
				if(Root->rightChild != NULL)
					destorySearchTreeSum(Root->rightChild);
				delete Root;	
				Root = NULL;
			}			
		}
				
		void helpPrintSearchTreeSum(SearchTreeSum* Root)
		{
			if (Root == NULL)
				return;
			else {
				cout<<Root->data<<" ";								
				if (Root->leftChild != NULL)
					helpPrintSearchTreeSum(Root->leftChild);		
				if (Root->rightChild != NULL)
					helpPrintSearchTreeSum(Root->rightChild);						
			}		
		}

		void helpSearchTreeSum(SearchTreeSum* Root, T* array,\
				 T target, int sum = 0, int index = 0)
		{
			//arrayģ��ջ��ֻ�Ǽ򵥻���
			//��ջ
			array[index] = Root->data; 
			sum += Root->data;
			index++;
			if (sum == target){
				for (int i = 0; i < index; i++)
					cout<<array[i]<<" ";
				cout<<endl;
			}
			if (Root->leftChild != NULL)
				helpSearchTreeSum(Root->leftChild,array,target,sum,index);
			if (Root->rightChild != NULL)
				helpSearchTreeSum(Root->rightChild,array,target,sum,index);	
					
			//��ջ 
			 index--; 
			 sum -= Root->data;
		}

		void searchSearchTreeSum(SearchTreeSum* Root, T target)
		{
			T* array = new T[maxLevel];
			for (int i = 0; i < maxLevel; i++)
				array[i] = T(0);
				
			helpSearchTreeSum(Root, array, target);
			
			delete [] array;

			if (Root->leftChild != NULL)
				searchSearchTreeSum(Root->leftChild, target);
			if (Root->rightChild != NULL)
				searchSearchTreeSum(Root->rightChild, target);		
		}	
		
};	

template <typename T>
	int SearchTreeSum<T>::maxLevel = 0;

void testSearchTreeSum()
{
	int i=0;
	int test[15]={0};
	
	for(i=0;i<15;i++)
		test[i]=rand()%20;
	//heap_sort(test,29);
	
	for(i=0;i<15;i++)
		cout<<test[i]<<" ";
	cout<<endl;
		
	SearchTreeSum<int>* aHa = NULL;
	SearchTreeSum<int>* searchtreesum = NULL;	
	searchtreesum = (*aHa).createSearchTreeSum(test, 0, 14);
	(*aHa).helpPrintSearchTreeSum(searchtreesum);
	cout<<endl;
	
	(*aHa).searchSearchTreeSum(searchtreesum, 12);
	
	(*aHa).destorySearchTreeSum(searchtreesum);
}

//timo5��������С��k����
//ʹ��������ɾͿ��ԣ���ѡ����Ż��Ǵ�����ʱ���飬���ʶ����
//ѡ������õ� 
//�ô��ʺ�ʹ�ÿ�������ͨ��ѡȡ����ȥ�ƽ�Ŀ���k���±�
//�ҵ���Ը�������ǰ�������򼴿ɣ�����˳����Ҫ��Ҳ����������

//timo6���������飬���������飬ֵΪԭ�����ڱ������г��ָ��� 
bool isSatisfy(int* list1, int* list2, int n)
{
	for (int i = 0; i < n; i++) {
		//�жϸ�λ�ã��Ƿ���������
		int sum = 0;	
		for (int j = 0; j < n; j++) {
			if(list1[i] == list2[j])
				sum++;
			} 
		if (sum != list2[i])
 			return false;
	}
	return true;
}

void searchArrayNum(int* list, int* listNew,\
	int n, int range, int index = 0)
{
	if (index >= n)
		return;	
	for (int k = 0; k <= range; k++) {
		listNew[index] = k;
		searchArrayNum(list, listNew, n, range - k, index + 1);
		if (index == n - 1 && isSatisfy(list, listNew, n)) {			
			for (int i =0; i < n; i++)
				cout<<listNew[i]<<" ";
			cout<<endl;
		}
	}		
} 

void testsearchArrayNum()
{
	int test[10] = {0,1,2,3,4,5,6,7,8,9};
	int testNew[10] = {0};
	searchArrayNum(test, testNew, 10, 10, 0);

}

//timo7�������������ཻ
//��һ�������У�ָ���ֵַһ�µĻ���
//�ɿ϶������ڵ�Ϊͬһ�ڵ㣬���ɶ϶�
//Ŀǰ�뵽���ǣ�������������������ѭ�� 

//timo9���ж����������ǲ���
//��Ԫ��������������Ľ��
//ֻ��ݹ����ÿһ��ֵ������ֵΪtrue�ͼ��� 
//Ϊfalse��ֱ���˳� 
template <typename T>
class CheckTree{
	private:
		static int index;
		T data;
		CheckTree* leftChild;
		CheckTree* rightChild; 
	public:
		//����
	bool isTree(CheckTree* Root, T* list)
	{
		if (Root->leftChild == NULL&& Root->rightChild == NULL) {
			if (Root->data != list[index])
				return false;
			return true;
		} else {
			bool leftFlag,rightFlag;
			if (Root->leftChild != NULL)
				leftFlag = isTree(Root->leftChild, list);
				index++;
			if (leftFlag == false)
				return false;
			if (Root->rightChild != NULL)
				rightFlag = isTree(Root->rightChild, list);
				index++;
			if (rightFlag == false)
				return false;
			if (Root->data != list[index++])
				return false;
			
			return true;
		}			
	} 
};

template <typename T>
	int CheckTree<T>::index = 0; 

//timo10����ת�ַ������Ե���Ϊ��λ 
void reverseWord(char* str)
{
	int n = 1;
	char* strLength = str;
	while (*strLength++ != '\0')
		n++;
	char* strBuf = new char[n];
	for (int i = 0; i < n; i++)
		strBuf[i] = 0;
	for (int i = 0; i < n; i++)
		strBuf[i]=*(str+i);
	
	for (int buf = 0, i = 0, j = n - 2; i <= j; i++, j--) {
		buf = strBuf[i];
		strBuf[i] = strBuf[j]; 
		strBuf[j] = buf;
	}
	
	for (int k1 = 0, k2 = 0; k1 < n && k2 < n; k2++, k1 = k2) {
		while(strBuf[k2] != ' ' && strBuf[k2] != '\0')
			k2++;
		int buf =k2;		
		for (int buf = 0, i = k1, j = k2-1; i <= j; i++, j--) {
			buf = strBuf[i];
			strBuf[i] = strBuf[j]; 
			strBuf[j] = buf;
		}
		k2 = buf;
		strBuf[k2] = ' ';		
	}
	
	for (int i = 0; i < n; i++)
		*(str + i) = strBuf[i]; 
	*(str + n - 1) = '\0';
	
	delete [] strBuf;
} 

void testReverseWord()
{
	char str[] = "I am a student.";
	cout<<str<<endl;	
	reverseWord(str);
	cout<<str<<endl;
}

//timo12����1��n�ĺ�
//Ҫ����ʹ�ó˳�����for��while��if��else��switch��case��
//�ؼ����Լ������ж����

int sumAbitAndBbit(int ci, int a, int b, int n = 1)
{
	if(n >= 32)
		return 0;
		
	int ai = a & (1 << n);
	int bi = b & (1 << n);

	int sumi = ai ^ bi ^ ci;
	int ciplus = ((ai & bi) | (bi & ci) | (ai & ci)) << 1;
		
	return sumAbitAndBbit(ciplus , a , b , n + 1) | sumi;	
}

int sumAandB(int a, int b)
{
	int ai = a & 1;
	int bi = b & 1;
	int ci =(ai & bi) << 1, sumi = ai ^ bi;
	
	return sumi | sumAbitAndBbit(ci, a, b);
}

int sumN(int n)
{
	if(n <= 1)
		return 1;
	return sumAandB(n, sumN(n - 1));
}

void testSumN()
{
	cout<<sumN(15);
}

//timo14�����Ҹ�Ԫ���Ƿ����������������ϳ�
void testArrayTwoSum()
{
	int left = 0, right = 29, n = right + 1;
	int test[30] = {0};
	
	for(int i = left; i <= right; i++)
		test[i] = rand() % 100;
	
	quick_sort(test,left,right);
	
	for(int i = left; i <= right; i++)
		cout<<test[i]<<" ";
	cout<<endl;
		
	int target = 47;
	
	//�۰���ң���Ѱ����Ŀ��ֵ�ӽ������ֵ
	int center = 0;
	int leftbuf = left, rightbuf =right;
	while (1) {
		if(test[center] <= target &&\
			 test[center + 1] >= target)
			break;
		center = (leftbuf + rightbuf) /2;	
		if (test[center] > target) {
			rightbuf = center;
		} else {
			leftbuf = center;
		}
	}
	
	int index = 0;
	while (center-- > 0) {
		index=binary_search(test, left, right, target-test[center]);
	 	if (index != -1){
			cout<<target<<"="<<test[center]<<"+"<<test[index]<<endl;
		 }
	 }
	 
}

//timo15��ת����Ԫ������Ϊ����
//�ݹ�شӵײ����������������ҽڵ��ָ��

//timo16�������ӡ��Ԫ��
//�˴���Ҫʹ�õ����У�����ʵ�ֹ����������
//ʹ������ʵ�ֶ��У����нڵ������ڵ��ַ
//���캯����ȡ���нڵ㲢��ӡ��Ȼ������� 
//�����ýڵ�����Һ��������

//timo19��쳲��������е�n�� 
//��̬�滮 
int searchFibonacci(int n)
{
	int f1 = 1, f2 = 1;
	int i = 2;
	while (1) {

		f1 = f1 + f2;
		i++;
		if (i >= n)
			return f1;		
		f2 = f1 + f2;
		i++;
		if (i >= n)
			return f2;		
	}	
} 
 
//timo20��һ����ʾ�������ַ���תΪ���� 
int finishStrToInt(char* str)
{
	int buf = 0;
	while (1) {
		if (*str == '\0')
			break;
		buf = buf * 10 + (*str++) - '0';		
	}	
	return buf;
} 

//timo21��������������n��m
//���Ϊm����1��nΪ��Χ��ȫ�����ݼ��� 
//ֱ���Ա�����٣�ȫ���в�ÿ�����ݱȽϳ��� 
void finishSumUnion(int n, int m)
{
	bool* array = new bool[n + 1];
	array[0] = false;
	for (int i = 1; i < n + 1; i++)
		array[i] = false;
	int start = 0, end = int(pow(2, n));
	while (1) {
		if (start >= end)
			break;
			
		int temp = start++;
		for (int i =1; i < n + 1; i++) {
			array[i] = temp % 2;
			temp /= 2;
		}
		
		int sum = 0; 
		for (int i =1; i < n + 1; i++) 
			sum += i * array[i];
		
		if (sum == m) {
			cout<<m<<" : ";
			for (int i = 1; i < n + 1; i++) 
				if(array[i] == true)			
					cout<<i<<" ";
			cout<<endl;	
		}
	}			
	delete [] array;
} 
//ʹ��0-1�������䣬�����ʼ��
//F(n,m) �ɷ�Ϊ n�Ƿ����룬����ΪF(n-1,m)��F(n-1,m-n)
//֮���Կ��Խ�ʡʱ�䣬�ؼ����ڵ� m < n��������£�
//���൱һ���ֵ������ǲ�����Ҫ����������� 
void sumUnion(int n, int m, bool* flag, int all)
{
	if (n < 1 || m < 1)
		return;
	if (n > m) //�κ�>m������û������ 
		n = m;
	if (m == n){
		flag[n] = true;
		cout<<"--: ";
		for (int i = 1; i < all + 1; i++) 
			if(flag[i] == true)			
				cout<<i<<" ";
		cout<<endl;	
		flag[n] = false;				
	}
	
	flag[n] = true;
	sumUnion(n - 1, m - n, flag, all);
		
	flag[n] = false;
	sumUnion(n - 1, m, flag, all);	
}

void finishSumUnionOther(int n, int m) 
{
	bool* array = new bool[n + 1];
	array[0] = false;
	for (int i = 1; i < n + 1; i++)
		array[i] = false;	
	
	sumUnion(n, m, array, n);	
	delete [] array;	
}

//timo24���������
//������͵ص��ã���Ҫ�õ�����ָ�룬
//һ��ָ��ָ��Ͽ��㣬һ��ָ��ϵ�ǰ�����Լ�ת�úõģ�
//һ��ָ��ϵ��������Ҫת�ã�
//������ͷת�ü��ɣ���Լ�ƶ�����

//�ϲ�����
//�������ø�����ָ�����·������ 
//����ͼ�Ϳ��� 

//timo25�����ַ�����������ִ�����
int countMaxNum(char* outputstr,char* inputstr) 
{
	char* pMax = NULL, * pNew = NULL, * pNow = NULL;
	//У׼����һ�����ִ���
	pNow = inputstr;
	while (*pNow !='\0' && !(*pNow - '0' >= 0 && *pNow - '0' <= 9))
		pNow++;
	pMax = pNow;

	int max = 0, count = 0;
	while (*pNow != '\0'){
		pNew = pNow;
		count = 0;		
		while (*pNow !='\0' && *pNow - '0' >= 0 && *pNow - '0' <= 9) {
			count++;			
			pNow++;			
		}

		if (max < count) {
			max = count;
			pMax = pNew;				
		}
		while (*pNow !='\0' && !(*pNow - '0' >= 0 && *pNow - '0' <= 9))
			pNow++;		
	}	
	
	pNow = pMax;
	char* pBuf = outputstr;
	while(*pNow !='\0' && *pNow - '0' >= 0 && *pNow - '0' <= 9)
		*pBuf++ = *pNow++;

	return max;
}
 
//timo26������ת�ַ�������
//����תabcdef��λΪcdefab 
void leftRotateStr(char* str, int n)
{
	char* strend = str;
	while (*strend++ != '\0');
	char buf;
	for (char* pbuf = str, * poffset = str + n - 1;
		pbuf <= poffset; pbuf++, poffset--) {
		buf = *pbuf;
		*pbuf = *poffset;
		*poffset = buf;
	}
	for (char* pbuf = str + n, * poffset = strend - 2;
		pbuf <= poffset; pbuf++, poffset--) {
		buf = *pbuf;
		*pbuf = *poffset;
		*poffset = buf;
	}	
	for (char* pbuf = str, * poffset = strend - 2;
		pbuf <= poffset; pbuf++, poffset--) {
		buf = *pbuf;
		*pbuf = *poffset;
		*poffset = buf;
	}	
		
}

//timo27����̨������
//���ж�������
int howmanyWay(int n)
{
	if (n == 0)
		return -1;
	else if (n == 1)
		return 1;
	else if (n == 2)
		return 2;
	else {
		//������Ϊnʱ��
		//����Ϊn-1����������n-2�������� 
		int i = 3;
		int all = 0, n_1 = 1, n_2 = 2;
		for (i = 3; i <= n; i++) {
			all = n_1 + n_2;
			n_1 = n_2;
			n_2 = all;
		}
		
		return all;
	}	
} 

//timo29����֪push�������ж�pop�Ƿ����
bool assertPop(int* push, int pushN, int* pop, int popN) 
{
	int * ready = new int[pushN + 1];	
	int * used = new int[pushN + 1];
	
	ready[0] = 0;
	used[0] = 0;
	for (int i = 1; i < pushN + 1; i++) {
		ready[i] = push[i - 1];
		used[i] = 0;	
		
		cout<<i<<" - "<<ready[i]<<" "<<used[i]<<endl;			
	}
		
	int usedIndex = 0, readyIndex = 0;
	
	for (int i = 0; i < popN; i++) {
		//����ready
		for (int j = readyIndex; j <= pushN; j++) 
			if (pop[i] == ready[j]) {
				while(readyIndex <= j){
					used[usedIndex++] = ready[readyIndex];					
					ready[readyIndex++] = 0;
				}
				
				cout<<pop[i]<<" "<<endl;									
		for (int k = 0; k < pushN + 1; k++)
			cout<<k<<" - "<<ready[k]<<" "<<used[k]<<endl;			
								
				goto finish; 
			} 
		//����used	
		for (int j = usedIndex; j >= 1; j--) 
			if (pop[i] == used[j]) {				
				while (usedIndex >= j)
					used[usedIndex--] = 0;
			if (usedIndex <= 0)
				usedIndex = 1;
					
				cout<<pop[i]<<" "<<endl;				
		for (int k = 0; k < pushN + 1; k++)
			cout<<k<<" - "<<ready[k]<<" "<<used[k]<<endl;
											
				goto finish; 
			} 			
		goto fail;
		finish:;
		//������ 
		
	}
	
	delete [] used;
	delete [] ready;
	return true;	
			
	fail:
	delete [] used;
	delete [] ready;
	return false;
	
}

//timo30���� 1 - n �� 1���ֵĴ���
int countOneToN(int n)
{
	int count = 0, offset = 1, sum = n;
	while (1) {

		count +=(sum / 10 + (sum % 10 >= 1? 1: 0)) * offset;
		if (sum / 10 == 1)
			count += sum % 10 + 1;				
		offset *= 10;
		sum /= 10;
		if (sum == 1 || sum == 0)
			break;
	}
	return count;
} 

//timo32�� ����������ֵ��֤�����Ǻ͵Ĳ���С 
//������㷨��һ�ִ�� 
int sumOneArray(int* list, int left, int right)
{
	int sum = 0;
	for (int i = left; i <= right; i ++)
		sum += list[i];
	return sum;
}

int searchMaxElement(int* list, int left, int right, int target)
{
	int index = -1;
	for (int i = left, temp = 0; i <= right; i++)
		if(target > temp && target >list[i])
		if (temp < list[i]) {
			index = i;
			temp = list[i];			
		}

	return index;
}

int searchMinElement(int* list, int left, int right)
{
	int index = left;
	for (int i = left; i <= right; i++)
		if (list[index] > list[i])
			index = i;
	return index;
}

void changeTwoArray(int* listA, int leftA, int rightA,\
					int* listB, int leftB, int rightB) 
{
	int listASum    = 0,  listBSum    = 0;	
	int indexAMax   = 0,  indexBMin   = 0;
	int indexAMin   = 0,  indexBMax   = 0;
	int lastRecord1 = 0,  lastRecord2 = 0;
	int thisRecord  = 0,  target      = 0;
	
	while (1) {
		listASum = sumOneArray(listA, leftA, rightA);
		listBSum = sumOneArray(listB, leftB, rightB);	
						
		if (listASum == listBSum)
			break;
		else if (listASum > listBSum) {
			//ȡlistAMax,listBMin
			thisRecord = listASum - listBSum;					
			if (lastRecord1 < lastRecord2 &&  
				(thisRecord == lastRecord1 || thisRecord == lastRecord2))
				break;
   			else{
				lastRecord1 = lastRecord2;
				lastRecord2 = thisRecord;								
			}											
			indexBMin = searchMinElement(listB, leftB, rightB);	
			if (thisRecord < listB[indexBMin])
				target = thisRecord + listB[indexBMin];
			else
				target = thisRecord - listB[indexBMin];
			indexAMax = searchMaxElement(listA, leftA, rightA, target);			
			if (indexAMax != -1) {
				int temp = listA[indexAMax];
				listA[indexAMax] = listB[indexBMin]; 
				listB[indexBMin] = temp;					
			}
			
		} else {																			
			//ȡlistBMax,listAMin
			thisRecord = listBSum - listASum;						
			if (lastRecord1 < lastRecord2 &&  
				(thisRecord == lastRecord1 || thisRecord == lastRecord2))
				break;
			else{         
				lastRecord1 = lastRecord2;
				lastRecord2 = thisRecord;								
			}
			indexAMin = searchMinElement(listA, leftA, rightA);	
			if (thisRecord < listA[indexAMin])
				target = thisRecord + listA[indexAMin];	
			else
				target = thisRecord - listA[indexAMin];				
			indexBMax = searchMaxElement(listB, leftB, rightB, target);
			if (indexBMax != -1) {
				int temp = listB[indexBMax];
				listB[indexBMax] = listA[indexAMin]; 
				listA[indexAMin] = temp;							
			}											
		}	 
	}
		
}

void testChangeTwoArray()
{
	int test1[] = {1, 2, 3, 4, 5, 40};
	int test2[] = {1, 2, 3, 98, 99, 100};
	changeTwoArray(test1, 0, 5, test2, 0, 5);
	for (int i = 0; i < 6; i++)
		cout<<test1[i]<<" ";
	cout<<endl;
	for (int i = 0; i < 6; i++)
		cout<<test2[i]<<" ";
	cout<<endl;	
}

//���Զ�̬�滮������0-1�����������������
void ChangeTwoArrayD(int* list, int left, int right)
{
	int sum = sumOneArray(list, left, right);
	int length = right - left + 1;
	int half = length / 2;
	
	//���붯̬�ڴ棬��ά���� 
	// i->length+1 , j->half+1 , v-> sum/2+1 
	int*** Vbest = new int**[length + 1];	
	for (int i = 0; i < length + 1; i++){
		Vbest[i] = new int*[half + 1];		
		for (int j = 0; j < half + 1; j++)
			Vbest[i][j] = new int[sum / 2 + 1];
	}
	
	for (int i = 0; i < length + 1; i++)
		for (int j = 0; j < half + 1; j++)
			for (int v = 0; v < sum / 2 + 1; v++)
				Vbest[i][j][v] = 0;

	for (int i = 1; i < length + 1; i++)
		for (int j = 1; j < (i < half ? i : half) + 1; j++)
			for (int v = 1; v < sum / 2 + 1; v++) {
		if (v < list[i - 1])	
			Vbest[i][j][v] = Vbest[i - 1][j][v];
		else if(Vbest[i - 1][j][v] < 
			Vbest[i - 1][j - 1][v - list[i - 1]]+list[i - 1])
			Vbest[i][j][v] = Vbest[i - 1][j - 1][v - list[i - 1]]+list[i - 1];
		else
			Vbest[i][j][v] = Vbest[i - 1][j][v];		 
	}
//	//��ΪУ׼����ӡȫ�� 
//	for (int i = 0; i < length + 1; i++) {
//		for (int j = 0; j < half + 1; j++) {
//			for (int v = 0; v < sum / 2 + 1; v++)
//				cout<<Vbest[i][j][v]<<" ";
//			cout<<endl;			
//		}
//		cout<<endl;		
//	}
	//����һ������ 
	int i = length, j = half, v = sum / 2;
	while (1) {
		if (i <= 0)
			break;
		else if (Vbest[i][j][v] == Vbest[i - 1][j][v])
			i = i - 1;
		else if (v - list[i - 1] > 0 &&
				(Vbest[i][j][v] == 
				Vbest[i - 1][j - 1][v - list[i - 1]]+list[i - 1])) {
			cout<<list[i - 1]<<" ";
			i = i - 1;	
			j = j - 1;
			v = v - list[i - 1];
		}			
	} 
	cout<<endl;
	
	//�ͷŶ�̬�ڴ� 
	for (int i = 0; i < length + 1; i++){		
		for (int j = 0; j < half + 1; j++)
			delete [] Vbest[i][j];
		delete [] Vbest[i];
	}
	delete [] Vbest;
		
} 

void testChangeTwoArrayD()
{
	int test[12] = {1, 2, 3, 4, 5, 40, 1, 2, 3, 98, 99, 100}; 
	ChangeTwoArrayD(test, 0, 11);
}

//timo33��ʵ���ַ�*ƥ���㷨,
//������������1**2****3*** ,ƥ��123 
//���� 1**2****3***�� *1*2*3*���� 
bool assertSomeStr(char* str1, char* str2)
{	
	int   length1 = 0	  ,   length2 = 0;
	char* pstr1   = str1  , * pstr2   = str2;
	
	while(*pstr1++ != '\0') 
		length1++;
	while(*pstr2++ != '\0') 
		length2++;
		
	//��-��str2����-��str1 
	bool** dp = new bool*[length2 + 1];
	for (int i = 0; i < length2 + 1; i++)
		dp[i] = new bool[length1 + 1]; 
	
	for (int i = 0; i < length2 + 1; i++)
		for (int j = 0; j < length1 + 1; j++)
			dp[i][j] = false;	
	dp[0][0] = true;
			
	//Ϊ*��ʱ ��dp[i][j] = dp[i - 1][j] || dp[i][j - 1]
	//Ϊ������ĸʱ��dp[i][j] = dp[i - 1][j - 1] && (str2[j] == str1[i]) 		
	for (int i = 1; i < length2 + 1; i++) {
		dp[i][0] = true && str2[i - 1] == '*';
		for (int j = 1; j < length1 + 1; j++) {
			dp[0][j] = true && str1[j - 1] == '*';			
			if (str2[i - 1] == '*' || str1[j - 1] == '*')
				dp[i][j] = dp[i - 1][j] || dp[i][j - 1];
			else
				dp[i][j] = dp[i - 1][j - 1] && (str2[i - 1] == str1[j - 1]
				 || str2[i - 1] == '?' || str2[i - 1] == '?');
	}
	}
	//��ӡ 
	for (int i = 0; i < length2 + 1; i++) {
		for (int j = 0; j < length1 + 1; j++)
			cout<<dp[i][j]<<" ";		
		cout<<endl;	
	}	
	//��������		
	bool answer = dp[length2][length1];
	//�ͷŶ�̬�ڴ� 
	for (int i = 0; i < length2 + 1; i++)
		delete [] dp[i];	
	delete [] dp;
	
	return answer;
} 

//timo35���������е�һ������ά����Ԫ�غ����
void testTwoMatirx()
{
	int all[6][5] = {{3,2,4,1,4},
					 {7,4,6,7,9},
					 {3,2,5,7,5},
					 {6,7,6,8,2},
					 {7,5,4,2,8},
					 {4,5,2,7,5}};
	int max = 0, buf = 0;
	int x = 0,y = 0; 
	for (int i = 0; i < 6 - 1; i++)
		for (int j = 0; j < 5 - 1; j++)
			 if(max < (buf = (all[i][j]+all[i+1][j+1]
			 				+all[i+1][j]+all[i][j+1]))) {
			 	max = buf;
			 	x = i;
			 	y = j;
			 }
			 
	cout << max <<endl;
	cout <<all[x][y]<<" "<<all[x][y+1]<<endl
		<<all[x+1][y]<<" "<<all[x+1][y+1]<<endl;		 
		
} 

//timo37�����ַ��������ӹ�ϵ
//��str1-��str2 ��return 1������ return 2, ʧ��Ϊ 0 
int compareStr(char* str1, char* str2)
{
	char* buf1   = NULL, * buf2   = NULL; 
	//����str1 ǰ ��str2 �� 
	buf1 = str1; 
	buf2 = str2; 
	buf1++;
	while (*buf1 != '\0') 
		 if(*buf1 == *buf2) {
		 	buf1++;
			buf2++;
		 } else 
		 	break;		 
	if (*buf1 == '\0')
		return 1;
	//����str2 ǰ ��str1 �� 
	buf1 = str1; 
	buf2 = str2; 
	buf2++;
	while (*buf2 != '\0') 
		 if(*buf1 == *buf2) {
		 	buf1++;
			buf2++;
		 } else
			break;		 
	if (*buf2 == '\0')
		return 2;	
	//����ʧ�� 
	return 0;
}

int connectCompareStr(char** str, int number, int n) 
{	
	//�������ģ�� 	
	int Infinity =  -100000; 
	int max = Infinity;
	//���붯̬�ڴ� 
	int** dp = new int*[number];
	for (int i = 0; i < number; i++)
		dp[i] = new int[number];
	int** p = new int*[number];
	for (int i = 0; i < number; i++)
		p[i] = new int[number];	
	//��ʼ��	
	for (int i = 0; i < number; i++)
		for (int j = 0; j <number; j++)
			dp[i][j] = 0;
	for (int i = 0; i < number; i++)
		for (int j = 0; j <number; j++)
			p[i][j] = 0;
	//ȷ�����ַ������ӹ�ϵ 
	for (int i = 0; i < number; i++)
		dp[i][i] = Infinity;	
	for (int i = 0; i < number; i++)
		for (int j = 0; j <number; j++)	
			if(compareStr(str[i],str[j]) == 1)
				dp[i][j] = 1;
			else
				dp[i][j] = Infinity;
	//��ѯ���·�� 
	for (int k = 0; k < number; k++) 
		for (int i = 0; i < number; i++)
			for (int j = 0; j <number; j++)	{
					if (dp[i][k] > Infinity && dp[k][j] > Infinity 
					&& dp[i][j] < dp[i][k] + dp[k][j]) {
					dp[i][j] = dp[i][k] + dp[k][j];							
					 p[i][j] = k;	
					}			
	}
	
	bool trueorfalse = true;
	for (int i = 0; i < number; i++)
		if (dp[i][i] != Infinity)
			trueorfalse = false;	
//	//��ӡ 
//	for (int i = 0; i < number; i++) {
//		for (int j = 0; j <number; j++)
//			cout<<dp[i][j]<<"	";		
//		cout<<endl;	
//	}		
//	cout<<endl; 
//	for (int i = 0; i < number; i++) {
//		for (int j = 0; j <number; j++)
//			cout<<p[i][j]<<"	";		
//		cout<<endl;	
//	}	
	//�ͷŶ�̬�ڴ� 
	for (int i = 0; i < number; i++)
		delete [] p[i];	
	delete [] p;	
	for (int i = 0; i < number; i++)
		delete [] dp[i];	
	delete [] dp;
	
	return trueorfalse;
}

void testConnectCompareStr()
{

	char str[10][10] = {
		"qwer","wert","erty","rtyu",
		"asdf","sdfg","dfgh","dfgz",
		"zxcv","xcvb"}; 	
 	char* allStr[10] = {0};
 	int number = 10;
	int n = 4;	
 	for (int i = 0; i < number; i++)
		allStr[i] = str[i];	
// 	for (int i = 0; i < number; i++)
//		cout<<allStr[i]<<endl;		
	connectCompareStr(allStr,number,n);
}

//timo47����һ���������ݼ�������
void maxLengthArray(int * array, int length)
{
	//���붯̬�ڴ� 
	int* dp = new int[length];	
	//��ʼ��
	for (int i = 0; i < length; i++) 
			dp[i] = 0;
							
	for (int i = 0; i < length; ++i) {
		dp[i] = 1;
		for (int j = 0; j < i; ++j) {
			if (array[j] > array[i] && (dp[j]+1) > dp[i])
				dp[i] = dp[j] + 1;											
		}		
	}
				
	//��ӡ	
	for (int i = 0; i < length; i++)
			cout<<dp[i]<<" ";			
	cout<<endl;	

	//�ͷŶ�̬�ڴ� 	
	delete dp;
	
} 

//timo50�����������ڵ�������
//��֤���ڵ���Ψһ�ģ����ɣ�ʵ���������
//ͨ������ֵ true �� false �жϹ��и��ڵ�
//ĳһ�ڵ㣬�������Ӻ��ӷ��ض�Ϊtrue
//��ͨ������Ϊ���ڵ�������룬��ͬʱ��ÿ�η���trueʱ
//ȫ�ֱ����ۼ�·�����ɣ����ȫtrue ʱ�������� ��false�������� 
//���϶����ǲ�ѯ�ڵ�����ڵ㻹���ҽڵ㣬ֻ��Ҫ��true��Ϊ1,2
//�����Ӽ��� 

//timo51����Ϊn�������������� 
void searchAllList(int n)
{
	for (int i = 1; i < n / 2 + 1; i++) {
		int sum = 0;
		int start = i;
		int  offset = i;
		while (1) {
			sum += offset;
			if (sum == n)
				cout<<start<<" "<<offset<<endl;
			if (sum > n)
				break;	
			offset++;				
		}			
	} 
} 

//timo53�����ַ���ȫ����
void sortAllStr(char* str,int left, int right)
{
	if (left == right)
		cout << str<<endl;
	else {
		for (int temp, i = left; i <= right; i++) {
			temp = str[i];
			str[i] = str[left];
			str[left] = temp;	
				
			sortAllStr(str, left + 1, right);
			
			temp = str[i];
			str[i] = str[left];
			str[left] = temp;
		}
			
	}
} 
 
//timo55����ϰ��ֵ���������,�ַ��� 
class CMyString{
	private:
		char* pData;
	public:
		CMyString(char* pStr = NULL) 
		{
			if (pStr == NULL)
				this->pData = NULL;
			else {
				char * pBuf = pStr;
				char * newpBuf;
				int length = 0;
				while (*pBuf++ != '\0')
					length++;
				this->pData = new char[length+1];
				pBuf = pStr;
				newpBuf = this->pData;
				while (*pBuf != '\0')
					*newpBuf++ = *pBuf++;	
				*++newpBuf = '\0';							
			}
		}
		
		CMyString(const CMyString& str)
		{
			if (str.pData == NULL)
				this->pData = NULL;
			else {
				char * pBuf = str.pData;
				char * newpBuf;
				int length = 0;
				while (*pBuf++ != '\0')
					length++;
				this->pData = new char[length + 1];
				pBuf = str.pData;
				newpBuf = this->pData;
				while (*pBuf != '\0')
					*newpBuf++ = *pBuf++;
				*++newpBuf = '\0';							
			}
		}	
	
		~CMyString(void)
		{
			if (this->pData != NULL)
				delete this->pData;
		}
		
		CMyString& operator = (const CMyString& str)
		{
			if (str.pData == NULL)
				this->pData = NULL;
			else {
				if (this->pData != NULL)
					delete this->pData;
				char * pBuf = str.pData;
				char * newpBuf;
				int length = 0;
				while (*pBuf++ != '\0')
					length++;
				this->pData = new char[length + 1];
				pBuf = str.pData;
				newpBuf = this->pData;
				while (*pBuf != '\0')
					*newpBuf++ = *pBuf++;
				*++newpBuf = '\0';								
			}			
		}
		
		void printStr()
		{
			cout<<this->pData<<endl;
		}
	
};

//timo56��������Ӵ�
void maxTwoLittleStr(char* str1, char* str2)
{
	int length1 = 0, length2 = 0;
	char* buf = NULL;
	buf = str1;
	while (*buf++ != '\0')
		length1++;
	buf = str2;
	while (*buf++ != '\0')
		length2++;	
	cout<<length1<<" "<<length2;
	//�ַ����Ļ����Ǵ�0 - length1 ��������length1 + 1 
	//���붯̬�ڴ� 
	int** dp = new int*[length1 + 1];
	for (int i = 0; i <length1 + 1; i++)
		dp[i] = new int[length2 + 1]; 
	//��ʼ��
	for (int i = 0; i <length1 + 1; i++)
		for (int j = 0; j <length2 + 1; j++)
			dp[i][j] = 0;		
	//ʵ��
	for (int i = 1; i <length1 + 1; i++)
		for (int j = 1; j <length2 + 1; j++)
			if (str1[i - 1] == str2[j - 1]) {
				int max = 0;
				for (int p = 1; p < i; p++)
					for (int q = 1; q < j; q++) {
						if (dp[p][q] != 0)
							max = max > dp[p][q] ? max : dp[p][q];						
					}
				dp[i][j] = max + 1;				
			}

	//��ӡ 
	for (int i = 0; i <length1 + 1; i++) {
		for (int j = 0; j <length2 + 1; j++)
			cout<<dp[i][j]<<" ";		
		cout<<endl;
	}	
	//�ͷŶ�̬�ڴ� 
	for (int i = 0; i <length1 + 1; i++)
		delete dp[i];	
	delete dp;
} 

//timo61:
//ȫ�����^ �������x^y
//x^y��ĳһλΪ 1  �� x��y��Ӧ��ĳһλһ��Ϊ1 һ��Ϊ0
//��ʱ�����մ�λ�����飬���Ӧ��˫λ��Ҳ����һ��
//��ʱֻ��Ҫ��������ȫ�����^ �������x �� y 

//timo66���ߵ�ջ
void pushToButtom(int* stack, int n, int data)
{
	if (n < 0) {
		stack[0] = data;
	} else {
		int temp = stack[n];
		pushToButtom(stack, n - 1, data);
		stack[n + 1] = temp;
	}	
}

void reverseStack(int* stack, int n) 
{
	if (n < 0)
		return;
	else {
		
		int temp = stack[n];
		reverseStack(stack, n - 1);	
		pushToButtom(stack,n - 1,temp);
	}
	
	
}

//timo68�������������ų���С����
//ʹ��ǿ����ַ������Ӻ������ַ����ȽϺ������� 

//timo69��������ת�������СԪ��
int searchRotatedArray(int* array, int left, int right) 
{
	//��СԪ�� ���� array[i - 1] > array[i] 
	for (int i = left + 1, j = right; i <= right && j >= left ; i++, j--){
		if (array[i - 1] > array[i])
			return array[i];
		if (array[j - 1] > array[j])
			return array[j];		
	}		
} 

//timo73�����ַ��������Գ����ַ������� 
//��Ϊ�����㷨����ר�Ż����㷨manachar 
void searchMaxLittleStr(char* str)
{
	char* buf = str;
	int length = 0;
	while (*buf++ != '\0')
		length++;
	cout<<length<<endl;	
	//�������ܵ�����ַ�����ǰ������	
	for (int offset = length - 1; offset > 0; offset--)
		for (int index = 0;index + offset < length; index++)		
			for (int i = index, j = index + offset;
					str[i] == str[j]; i++, j--) {				
				if (i >= j) {						
					cout<<index<<" "<<index + offset<<endl;
					goto end;
				}								
			}									
	end:;	
} 
 
//timo74�����������У����ִ�������һ������� 
int searchMaxNumData(int* array,int left, int right) 
{
//�ҵ��е�ֵ����Ŀ�꣬���Կ��Կ������� 
	int data = array[left];
	int number = 1;
	for (int i = left + 1; i <= right; i++) {
		if (data == array[i])
			number ++;
		else 
			number --;		
		
		if (number == 0) {
			data = array[i];
			number = 1;
		}
	} 		
}

//timo75�������ڵ���͹�ͬ���ڵ㣬
//�ȼ���������֮����̾��룬
//�������ӽڵ������Ŀ�꣬��ýڵ�������� 

//timo76����������ĸ��ƣ����������ĸ��� 

//timo77�����������Ƿ��л���δ���� 
template<typename T>
class list{
	private:
		T data;
		list* next;
	public:
	bool haveLoop(list* head)
	{
		bool flag = false;
	
		for (list* p1 = head, * p2 = head;
			p2->next != NULL && p2 != NULL \
			&& p2->next != NULL && p2->next->next != NULL; 
			p1 = p1->next, p2 = p2->next->next)
				if (p1 == p2) 
					flag = true;

		return flag;	
	} 		
	
	int calculateListLength(list* head)
	{
		if (head == NULL)
			return 0;
		else {
			int length = 0;
			while (head != NULL) {
				head = head->next;
				length++;
			}
		return length;			
		}
	}
	
	list* firstLoopNode(list*head)
	{
		list* flag = NULL;
	
		for (list* p1 = head, * p2 = head;
			p2->next != NULL && p2 != NULL \
			&& p2->next != NULL && p2->next->next != NULL; 
			p1 = p1->next, p2 = p2->next->next)
				if (p1 == p2) 
					flag = p1;

		return flag;
	} 
	
	list* haveCrosspoint(list* head1 ,list* head2)
	{
		list* result = NULL;
		if (head1 == NULL || head2 == NULL)
			return NULL;		
		//˫�޻������߼�ͼΪy�� 
		//����һ�ַ��������ӳ�һ�������ɻ����н��� 
		if (haveLoop(head1) == false &&\
			haveLoop(head2) == false) {
			//�Ƿ��ཻ
			list* end1 = head1; 
			while(end1->next != NULL)
				end1 = end1->next;
			list* end2 = head2; 
			while(end2->next != NULL)
				end2 = end2->next;
			if (end1 != end2)
				return NULL;
			//���ཻ 
			int length1 = calculateListLength(head1);
			int length2 = calculateListLength(head2);
			if (length1 > length2)
				for (int i = length1 - length2; i > 0; i--)
					head1 = head1->next;
			if (length1 < length2)
				for (int i = length2 - length1; i > 0; i--)
					head2 = head2->next;		
			//�޸�ָ�룬ʹ�� y ���� Y	
			while (head1 != NULL && head2 != NULL)
				if (head1 == head2)
					return head1;
				else {
					head1 = head1->next;
					head2 = head2->next;
				}
			//û�ҵ���Ӧ�ò����� 
			return NULL;
		} 
		else {		
			//��ѡ�� ���õ�һ������㣬�޻���ΪNULL 
			list* startLoop1 = firstLoopNode(head1);
			list* startLoop2 = firstLoopNode(head2);		 
			//���ڻ������ཻ����һ�����л�
			//��Ϊĳ���޻������NULL�ڵ�
			//���ʹ�û���ĳһ�ڵ�������״̬ 
			if (startLoop1 == NULL || startLoop2 == NULL)
				return NULL;
			else if (startLoop1 == startLoop2) {		
				//˫���ҽ���Ϊ����ͬһ���ڵ� ,�����תΪy�� 
				int length1 = calculateListLength(head1);
				int length2 = calculateListLength(head2);
				if (length1 > length2)
					for (int i = length1 - length2; i > 0; i--)
						head1 = head1->next;
				if (length1 < length2)
					for (int i = length2 - length1; i > 0; i--)
						head2 = head2->next;		
				//�޸�ָ�룬ʹ�� y ���� Y	
				while (head1 != startLoop1 && head2 != startLoop2)
					if (head1 == head2)
						return head1;
					else {
						head1 = head1->next;
						head2 = head2->next;
					}					
				return startLoop1;								
			} else {
				//˫���ҽ��㲻Ϊ����ͬһ���ڵ�				
				//�ཻ��Ϊͬһ����ֻ��黷�Ƿ�ͬһ
				list* buf = startLoop1->next;
				while (buf != startLoop1) {
					if (buf == startLoop2)
						return startLoop1;
					buf = buf->next;
				}				 											
			}						
		}			
	}
	
	void deleteOneNode(list*head, list Node)
	{
		if (head == NULL)
			return;
		else if (!haveLoop(head)) {
			//�޻�ʱ
			list* buf1 = head;
			list* buf2 = head;  
			//�ҵ��ýڵ� 
			while (buf1 != NULL)
				if(buf1->data == Node->data)
					break;
				else {
					buf1 = buf1->next;
					if (buf1!= NULL)
						buf2 = buf1->next;					
				}
			//û�ҵ� 
			if (buf1 == NULL) 
				return;
			//�ҵ��� 
			buf1->data = buf2->data; 
			buf1->next = buf2->next;
			//ɾ��buf2ָ��Ľڵ㼴��
			// delete buf2;
			// buf2 = NULL; 
		} else {
			//�л�ʱ�����һ��ڵ� 
			list* startLoop = firstLoopNode(head); 
			if (startLoop == NULL)
				return; 
			//�����ȫ���Ļ�������ڵ㶼�������ڵ� 
			//����Ƿ�ȫ���Ļ�����ֻ��Ψһһ�����ڵ� 
			//���������Լ�Ϊ ��head - > startLoop 
			//�� startLoop->next ->startLoop

			list* buf1 = head;
			list* buf2 = head; 
			//���Դ�head - > startLoop 			
			buf1 = head;
			buf2 = head;  
			//�ҵ��ýڵ� 
			while (buf1 != startLoop->next)
				if(buf1->data == Node->data)
					break;
				else {
					buf1 = buf1->next;
					if (buf1!= NULL)
						buf2 = buf1->next;					
				}

			if (buf1 != startLoop->next) {
				//�ҵ��� 
				buf1->data = buf2->data; 
				buf1->next = buf2->next;
				//ɾ��buf2ָ��Ľڵ㼴��
				// delete buf2;
				// buf2 = NULL;								
			} 
			//���Դ�startLoop->next ->startLoop			 
			//���Դ�head - > startLoop 			
			buf1 = startLoop->next;
			buf2 = startLoop->next;  
			//�ҵ��ýڵ�
			if (startLoop->data == Node->data) {
				buf1 = startLoop;
				buf2 = buf1->next;
				//�ҵ��� 
				buf1->data = buf2->data; 
				buf1->next = buf2->next;
				//ɾ��buf2ָ��Ľڵ㼴��
				// delete buf2;
				// buf2 = NULL;									
			}else {
				while (buf1 != startLoop)
					if(buf1->data == Node->data)
						break;
					else {
						buf1 = buf1->next;
						if (buf1!= NULL)
							buf2 = buf1->next;					
					}
			if (buf1 != startLoop) {
				//�ҵ��� 
				buf1->data = buf2->data; 
				buf1->next = buf2->next;
				//ɾ��buf2ָ��Ľڵ㼴��
				// delete buf2;
				// buf2 = NULL;								
			} 							
			}			 
										
		}
				
	}
			
};

//timo80��˫���򣬵�һ�Ŷ�ӦλС�ڵڶ��Ŷ�Ӧλ 
//�����ѧ�еĿ������� 

//������� 
void twoSort()
{
	int alldata[12] = {1,2,3,4,5,6,7,8,9,10,11,12}; 	
	int helpdata[12] = {1,1,1,1,1,1,0,0,0,0,0,0};
	//helpdata���� 
	while (1) {
		
		//��ʼ�Ƚ��£��ҵ�һ��1��0�Ƚ�
		//ʧ�ܽ������֣��ɹ��ͱȽ���һ�� 
		int start1 = 0, start0 = 0; 
		int all = 0;
		for (int i = 0; i < 6; i ++) {
			//�ҵ� start - 11 �г��ֵĵ�һ��1
			for (int j = start1; j < 12; j++) 
				if (helpdata[j] == 1) {
					start1 = j;
					break;									
				}
			for (int j = start0; j < 12; j++) 
				if (helpdata[j] == 0) {
					start0 = j;
					break;									
				}
			//ƥ��ʧ�ܣ��˳� 
			if (alldata[start1] <= alldata[start0])
				all++;
			else
				break;
			
			start1++;
			start0++;
		}
		//�ҵ���һ�� 
		if (all == 6) {
			for(int i = 0; i < 12; i++)
				if (helpdata[i] == 1)
					cout<<alldata[i]<<" ";
			cout<<endl;
			for(int i = 0; i < 12; i++)
				if (helpdata[i] == 0)
					cout<<alldata[i]<<" ";					
			cout<<endl;							
		}		
		
		
		int flag = 1;
		for (int i = 6; i < 12; i++)
			flag *= helpdata[i];
		if (flag == 1)
			break;
		
		//�ҵ���һ�� 1  0 �����
		int index = 0;	
		for (int i = 0; i + 1 < 12; i++) 
			if (helpdata[i] == 1 && helpdata[i + 1] == 0) {
				index = i;
				break;
			}
				
		//������һ��1��0 
		helpdata[index] = 0; 
		helpdata[index + 1] = 1;
		
		//���� 0 - index1 �е����� 1 ������ߣ�0�����ұ�
		int count = 0; 
		for (int i = 0; i < index; i++) 
		 	if (helpdata[i] == 1) 
			count++;
		for (int i = 0; i < index; i++, count--) 
		 	if (count > 0) 
				helpdata[i] = 1;
			else
				helpdata[i] = 0;						
	} 
			
}

//˼����ͨ����ٷ��֣��Ե�һ��������ÿ��������һ���޶��ķ�Χ��
//������ҵ�һ�鲻�ظ���ǡ������÷�Χ�������Ƿ���� 

//timo81��������ĳ����������֮ǰ��С��������֮�󶼴�����
//���������˼ά��ֻ��Ҫ��׼ֵ��ͷ��β���ɣ����Ҳ����ڽ�����ʱ 
//�Ļ�׼������
//���Ӵ������������ַ�������hashɢ�У���֤�����һ���ַ�������ͬhashֵ
//�������ݷ��飬֮���ٽ��бȽϣ����͸��Ӷ� 

//timo88���޸��ַ�����*ȫ���Ƶ�ǰ�棬��*ȫ���Ƶ�����
void testMoveSpecialStr() 
{
	char str[] = "!!** hello ***world **";
	char* end = str;
	while (*end != '\0') end++;
	--end;
	
	char* buf = end;
	int count = 0;
	while (buf >= str) {
		
		while (*buf == '*') {
			buf--;
			count++;	
		}			
						
		if (buf >= str)
			*end-- = *buf--;
		
	}
	
	for (int i = 0; i < count; i++)
		*(str + i) = '*';
		
	cout<<str<<"!";	
}
 
//timo89����ת������
struct sList{
	sList* next;
};
 
sList* reversalList(sList* start)
{
	if (start == NULL)
		return NULL;
	else if (start->next == NULL)
		return start;
	else if (start->next->next == NULL) {
		sList* buf = start;
		buf->next->next = buf;
		buf = buf->next;
		buf->next->next = NULL;
		return buf;
	} else {
	sList* PP = NULL;
	sList* P1 = start;
	sList* P2 = P1->next;
	//�����ת��������ָ���ת
	//ǰ����ָ��PP��P1��תǰ�����ڵ㣬�����佻����PP
	//������ָ�뱣������ڵ�
	//֮��ÿ��P2��ȡһ�ڵ�����PP��ǰ�漴�� 		
	}	
}

//�������ַ���������ַ���
void searchMaxStr(char* str1, char* str2) 
{
	int count1 = 0;
	int count2 = 0;
	char* buf1 = str1;
	char* buf2 = str2;
	while (*buf1++ != '\0')
		count1++;
	while (*buf2++ != '\0')
		count2++;
	cout<<str1<<endl<<str2<<endl;	
	cout<<count1<<" "<<count2<<endl;
	//�����ڴ�ռ� 
	char** dp = new char*[count1 + 1];
	for (int i = 0; i < count1 + 1; i++)
		dp[i] = new char[count2 + 1];
	//��ʼ��ȫ�� 
	for (int i = 0; i < count1 + 1; i++) 
		for (int j = 0; j < count2 + 1; j++) 
			dp[i][j] = 0;	
	//��д���� 
	for (int i = 1; i < count1 + 1; i++) 
		for (int j = 1; j < count2 + 1; j++)
			if (str1[i - 1] == str2[j - 1])
				dp[i][j] = 1 + dp[i - 1][j - 1];
	//��ӡ 
	for (int i = 0; i < count1 + 1; i++) {
		for (int j = 0; j < count2 + 1; j++) 
		 	cout<<int(dp[i][j])<<" ";		
		cout<<endl;
	}
	//ֻ�����������������ж�������ַ���
	 
		
	//�ͷ��ڴ�ռ� 
	for (int i = 0; i < count1 + 1; i++)
		delete [] dp[i];	
	delete [] dp;
}












#endif 
