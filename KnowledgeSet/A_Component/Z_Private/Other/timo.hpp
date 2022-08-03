#ifndef __TIMO_HPP_
#define __TIMO_HPP_

#include<iostream>
#include"sort.h"
#include"search.h" 

using namespace std;
//网上随便找的题目

//timo1：完成二叉查找树到双向链表的转换
template <typename T>
class TreeToList
{
	private:
		T data;
		TreeToList* left_last;	//表示树-左节点，表示链表-上一节点 
		TreeToList* right_next;	//表示树-右节点，表示链表-下一节点 
	public:
	//创建数据， 
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
	
	//完成二叉查找树—to—列表的创建 
	//接收一个已排序好数组，中序插入 
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
	
	//完成树到链表的转换 
	void changeTreeToList(TreeToList* Root, TreeToList* Ancester, int flag)
	{	
		//如果是叶子节点 
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

//timo2：完成一个栈，能满足查询栈最大或最小值以及出栈入栈
//其操作的时间量为O(1) 
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
	
	//此时实现min的O(1)查找 
	//修改 buf->data <= start->nextNode->data 为>=即可 
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

//timo3：动态规划完成子数组最大和
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

//timo4：查找数据路径，满足数据和
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
		
		//这是将利用数组数据，中序构建 
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
			//array模拟栈，只是简单化点
			//入栈
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
					
			//出栈 
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

//timo5：查找最小的k个数
//使用排序完成就可以，可选择的优化是创建临时数组，与标识数组
//选择排序得到 
//该处适合使用快速排序，通过选取基点去逼近目标的k的下标
//找到后对该数及以前的数排序即可，若对顺序无要求也可无需排序

//timo6：给定数组，创建新数组，值为原数组在本数组中出现个数 
bool isSatisfy(int* list1, int* list2, int n)
{
	for (int i = 0; i < n; i++) {
		//判断该位置，是否满足条件
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

//timo7：俩个单链表相交
//若一个程序中，指针地址值一致的话，
//可肯定俩个节点为同一节点，即可断定
//目前想到的是，暴力搜索，内外俩个循环 

//timo9：判断整数数组是不是
//二元查找树后序遍历的结果
//只需递归查找每一个值，返回值为true就继续 
//为false就直接退出 
template <typename T>
class CheckTree{
	private:
		static int index;
		T data;
		CheckTree* leftChild;
		CheckTree* rightChild; 
	public:
		//……
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

//timo10：翻转字符串，以单词为单位 
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

//timo12：求1到n的和
//要求不能使用乘除法、for、while、if、else、switch、case等
//关键字以及条件判断语句

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

//timo14：查找该元素是否由数组中俩个数合成
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
	
	//折半查找，先寻找与目标值接近的最大值
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

//timo15：转换二元查找树为镜像
//递归地从底部到顶部，交换左右节点的指针

//timo16：按层打印二元树
//此处需要使用到队列，用以实现广度优先搜索
//使用链表实现队列，队列节点存放树节点地址
//构造函数，取队列节点并打印，然后出队列 
//并将该节点的左右孩子入队列

//timo19：斐波那契数列第n项 
//动态规划 
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
 
//timo20：一个表示整数的字符串转为整数 
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

//timo21：输入俩个整数n，m
//求和为m的以1到n为范围的全部数据集合 
//直接以暴力穷举，全排列并每个数据比较尝试 
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
//使用0-1背包分配，从最大开始：
//F(n,m) 可分为 n是否被载入，而分为F(n-1,m)与F(n-1,m-n)
//之所以可以节省时间，关键在于当 m < n这种情况下，
//有相当一部分的数据是不符合要求而被舍弃的 
void sumUnion(int n, int m, bool* flag, int all)
{
	if (n < 1 || m < 1)
		return;
	if (n > m) //任何>m的数据没有意义 
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

//timo24：链表操作
//单链表就地倒置，需要用到三个指针，
//一个指针指向断开点，一个指向断点前链表（以及转置好的）
//一个指向断点后链表（将要转置）
//从链表头转置即可，节约移动次数

//合并链表
//合理利用辅助的指针调整路径即可 
//画个图就可以 

//timo25：求字符串中最长的数字串长度
int countMaxNum(char* outputstr,char* inputstr) 
{
	char* pMax = NULL, * pNew = NULL, * pNow = NULL;
	//校准到第一个数字串上
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
 
//timo26：左旋转字符串，即
//左旋转abcdef俩位为cdefab 
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

//timo27：跳台阶问题
//求有多少种跳
int howmanyWay(int n)
{
	if (n == 0)
		return -1;
	else if (n == 1)
		return 1;
	else if (n == 2)
		return 2;
	else {
		//当阶梯为n时，
		//种数为n-1级的种数与n-2级的种数 
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

//timo29：已知push操作，判断pop是否存在
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
		//搜索ready
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
		//搜索used	
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
		//出队列 
		
	}
	
	delete [] used;
	delete [] ready;
	return true;	
			
	fail:
	delete [] used;
	delete [] ready;
	return false;
	
}

//timo30：求 1 - n 中 1出现的次数
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

//timo32： 交换俩数组值保证，它们和的差最小 
//下面的算法是一种错的 
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
			//取listAMax,listBMin
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
			//取listBMax,listAMin
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

//尝试动态规划，例子0-1背包，依据它解此题
void ChangeTwoArrayD(int* list, int left, int right)
{
	int sum = sumOneArray(list, left, right);
	int length = right - left + 1;
	int half = length / 2;
	
	//申请动态内存，三维数组 
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
//	//作为校准，打印全表 
//	for (int i = 0; i < length + 1; i++) {
//		for (int j = 0; j < half + 1; j++) {
//			for (int v = 0; v < sum / 2 + 1; v++)
//				cout<<Vbest[i][j][v]<<" ";
//			cout<<endl;			
//		}
//		cout<<endl;		
//	}
	//给出一半数据 
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
	
	//释放动态内存 
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

//timo33：实现字符*匹配算法,
//例：查找诸如1**2****3*** ,匹配123 
//输入 1**2****3***和 *1*2*3*即可 
bool assertSomeStr(char* str1, char* str2)
{	
	int   length1 = 0	  ,   length2 = 0;
	char* pstr1   = str1  , * pstr2   = str2;
	
	while(*pstr1++ != '\0') 
		length1++;
	while(*pstr2++ != '\0') 
		length2++;
		
	//行-》str2；列-》str1 
	bool** dp = new bool*[length2 + 1];
	for (int i = 0; i < length2 + 1; i++)
		dp[i] = new bool[length1 + 1]; 
	
	for (int i = 0; i < length2 + 1; i++)
		for (int j = 0; j < length1 + 1; j++)
			dp[i][j] = false;	
	dp[0][0] = true;
			
	//为*号时 ，dp[i][j] = dp[i - 1][j] || dp[i][j - 1]
	//为其他字母时，dp[i][j] = dp[i - 1][j - 1] && (str2[j] == str1[i]) 		
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
	//打印 
	for (int i = 0; i < length2 + 1; i++) {
		for (int j = 0; j < length1 + 1; j++)
			cout<<dp[i][j]<<" ";		
		cout<<endl;	
	}	
	//保留数据		
	bool answer = dp[length2][length1];
	//释放动态内存 
	for (int i = 0; i < length2 + 1; i++)
		delete [] dp[i];	
	delete [] dp;
	
	return answer;
} 

//timo35：求大矩阵中的一个最大二维矩阵，元素和最大
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

//timo37：求字符串的连接关系
//若str1-》str2 则return 1，否则 return 2, 失败为 0 
int compareStr(char* str1, char* str2)
{
	char* buf1   = NULL, * buf2   = NULL; 
	//尝试str1 前 ，str2 后 
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
	//尝试str2 前 ，str1 后 
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
	//尝试失败 
	return 0;
}

int connectCompareStr(char** str, int number, int n) 
{	
	//负无穷的模拟 	
	int Infinity =  -100000; 
	int max = Infinity;
	//申请动态内存 
	int** dp = new int*[number];
	for (int i = 0; i < number; i++)
		dp[i] = new int[number];
	int** p = new int*[number];
	for (int i = 0; i < number; i++)
		p[i] = new int[number];	
	//初始化	
	for (int i = 0; i < number; i++)
		for (int j = 0; j <number; j++)
			dp[i][j] = 0;
	for (int i = 0; i < number; i++)
		for (int j = 0; j <number; j++)
			p[i][j] = 0;
	//确定俩字符串连接关系 
	for (int i = 0; i < number; i++)
		dp[i][i] = Infinity;	
	for (int i = 0; i < number; i++)
		for (int j = 0; j <number; j++)	
			if(compareStr(str[i],str[j]) == 1)
				dp[i][j] = 1;
			else
				dp[i][j] = Infinity;
	//查询最大路径 
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
//	//打印 
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
	//释放动态内存 
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

//timo47：求一个数组的最长递减子序列
void maxLengthArray(int * array, int length)
{
	//申请动态内存 
	int* dp = new int[length];	
	//初始化
	for (int i = 0; i < length; i++) 
			dp[i] = 0;
							
	for (int i = 0; i < length; ++i) {
		dp[i] = 1;
		for (int j = 0; j < i; ++j) {
			if (array[j] > array[i] && (dp[j]+1) > dp[i])
				dp[i] = dp[j] + 1;											
		}		
	}
				
	//打印	
	for (int i = 0; i < length; i++)
			cout<<dp[i]<<" ";			
	cout<<endl;	

	//释放动态内存 	
	delete dp;
	
} 

//timo50：查任意俩节点最大举例
//保证俩节点是唯一的，即可，实现中序遍历
//通过返回值 true 与 false 判断共有父节点
//某一节点，当且俩子孩子返回都为true
//则通过它即为俩节点间最大距离，则同时在每次返回true时
//全局变量累加路径即可，最后全true 时汇总数据 ，false不做操作 
//而断定，是查询节点中左节点还是右节点，只需要把true分为1,2
//这样子即可 

//timo51：和为n的连续正数序列 
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

//timo53：求字符串全排列
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
 
//timo55：复习赋值运算符重载,字符串 
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

//timo56：最长公共子串
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
	//字符串的话，是从0 - length1 ，长度有length1 + 1 
	//申请动态内存 
	int** dp = new int*[length1 + 1];
	for (int i = 0; i <length1 + 1; i++)
		dp[i] = new int[length2 + 1]; 
	//初始化
	for (int i = 0; i <length1 + 1; i++)
		for (int j = 0; j <length2 + 1; j++)
			dp[i][j] = 0;		
	//实现
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

	//打印 
	for (int i = 0; i <length1 + 1; i++) {
		for (int j = 0; j <length2 + 1; j++)
			cout<<dp[i][j]<<" ";		
		cout<<endl;
	}	
	//释放动态内存 
	for (int i = 0; i <length1 + 1; i++)
		delete dp[i];	
	delete dp;
} 

//timo61:
//全数异或^ 即可求出x^y
//x^y中某一位为 1  则 x与y对应的某一位一个为1 一个为0
//此时若按照此位分俩组，则对应的双位数也会在一边
//此时只需要对子数组全数异或^ 即可求出x 和 y 

//timo66：颠倒栈
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

//timo68：把整数数组排成最小的数
//使用强大的字符串连接函数，字符串比较函数即可 

//timo69：查找旋转数组的最小元素
int searchRotatedArray(int* array, int left, int right) 
{
	//最小元素 满足 array[i - 1] > array[i] 
	for (int i = left + 1, j = right; i <= right && j >= left ; i++, j--){
		if (array[i - 1] > array[i])
			return array[i];
		if (array[j - 1] > array[j])
			return array[j];		
	}		
} 

//timo73：求字符串中最大对称子字符串长度 
//此为朴素算法，有专门回文算法manachar 
void searchMaxLittleStr(char* str)
{
	char* buf = str;
	int length = 0;
	while (*buf++ != '\0')
		length++;
	cout<<length<<endl;	
	//保留可能的最大字符串的前后索引	
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
 
//timo74：查找数组中，出现次数超过一半的数字 
int searchMaxNumData(int* array,int left, int right) 
{
//找到中点值就是目标，所以可以考虑排序 
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

//timo75：求俩节点最低共同父节点，
//等价于求，它们之间最短距离，
//若左右子节点均返回目标，则该节点就是所求 

//timo76：复杂链表的复制，类似于树的复制 

//timo77：测试链表是否有环，未测试 
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
		//双无环，则逻辑图为y型 
		//还有一种方法，连接成一个链表，成环则有交点 
		if (haveLoop(head1) == false &&\
			haveLoop(head2) == false) {
			//是否不相交
			list* end1 = head1; 
			while(end1->next != NULL)
				end1 = end1->next;
			list* end2 = head2; 
			while(end2->next != NULL)
				end2 = end2->next;
			if (end1 != end2)
				return NULL;
			//会相交 
			int length1 = calculateListLength(head1);
			int length2 = calculateListLength(head2);
			if (length1 > length2)
				for (int i = length1 - length2; i > 0; i--)
					head1 = head1->next;
			if (length1 < length2)
				for (int i = length2 - length1; i > 0; i--)
					head2 = head2->next;		
			//修改指针，使得 y 换成 Y	
			while (head1 != NULL && head2 != NULL)
				if (head1 == head2)
					return head1;
				else {
					head1 = head1->next;
					head2 = head2->next;
				}
			//没找到，应该不存在 
			return NULL;
		} 
		else {		
			//先选择环 ，得到一个环起点，无环则为NULL 
			list* startLoop1 = firstLoopNode(head1);
			list* startLoop2 = firstLoopNode(head2);		 
			//存在环，若相交俩个一定都有环
			//因为某个无环会出现NULL节点
			//这会使得环上某一节点有俩种状态 
			if (startLoop1 == NULL || startLoop2 == NULL)
				return NULL;
			else if (startLoop1 == startLoop2) {		
				//双环且交点为环上同一个节点 ,则情况转为y型 
				int length1 = calculateListLength(head1);
				int length2 = calculateListLength(head2);
				if (length1 > length2)
					for (int i = length1 - length2; i > 0; i--)
						head1 = head1->next;
				if (length1 < length2)
					for (int i = length2 - length1; i > 0; i--)
						head2 = head2->next;		
				//修改指针，使得 y 换成 Y	
				while (head1 != startLoop1 && head2 != startLoop2)
					if (head1 == head2)
						return head1;
					else {
						head1 = head1->next;
						head2 = head2->next;
					}					
				return startLoop1;								
			} else {
				//双环且交点不为环上同一个节点				
				//相交则为同一环，只需查环是否同一
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
			//无环时
			list* buf1 = head;
			list* buf2 = head;  
			//找到该节点 
			while (buf1 != NULL)
				if(buf1->data == Node->data)
					break;
				else {
					buf1 = buf1->next;
					if (buf1!= NULL)
						buf2 = buf1->next;					
				}
			//没找到 
			if (buf1 == NULL) 
				return;
			//找到了 
			buf1->data = buf2->data; 
			buf1->next = buf2->next;
			//删除buf2指向的节点即可
			// delete buf2;
			// buf2 = NULL; 
		} else {
			//有环时，查找环节点 
			list* startLoop = firstLoopNode(head); 
			if (startLoop == NULL)
				return; 
			//如果是全环的话，任意节点都可做环节点 
			//如果是非全环的话，则只有唯一一个环节点 
			//但情况则可以简化为 从head - > startLoop 
			//和 startLoop->next ->startLoop

			list* buf1 = head;
			list* buf2 = head; 
			//尝试从head - > startLoop 			
			buf1 = head;
			buf2 = head;  
			//找到该节点 
			while (buf1 != startLoop->next)
				if(buf1->data == Node->data)
					break;
				else {
					buf1 = buf1->next;
					if (buf1!= NULL)
						buf2 = buf1->next;					
				}

			if (buf1 != startLoop->next) {
				//找到了 
				buf1->data = buf2->data; 
				buf1->next = buf2->next;
				//删除buf2指向的节点即可
				// delete buf2;
				// buf2 = NULL;								
			} 
			//尝试从startLoop->next ->startLoop			 
			//尝试从head - > startLoop 			
			buf1 = startLoop->next;
			buf2 = startLoop->next;  
			//找到该节点
			if (startLoop->data == Node->data) {
				buf1 = startLoop;
				buf2 = buf1->next;
				//找到了 
				buf1->data = buf2->data; 
				buf1->next = buf2->next;
				//删除buf2指向的节点即可
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
				//找到了 
				buf1->data = buf2->data; 
				buf1->next = buf2->next;
				//删除buf2指向的节点即可
				// delete buf2;
				// buf2 = NULL;								
			} 							
			}			 
										
		}
				
	}
			
};

//timo80：双排序，第一排对应位小于第二排对应位 
//组合数学中的卡特兰数 

//暴力穷举 
void twoSort()
{
	int alldata[12] = {1,2,3,4,5,6,7,8,9,10,11,12}; 	
	int helpdata[12] = {1,1,1,1,1,1,0,0,0,0,0,0};
	//helpdata代表 
	while (1) {
		
		//开始比较呗，找到一个1与0比较
		//失败结束此轮，成功就比较下一个 
		int start1 = 0, start0 = 0; 
		int all = 0;
		for (int i = 0; i < 6; i ++) {
			//找到 start - 11 中出现的第一个1
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
			//匹配失败，退出 
			if (alldata[start1] <= alldata[start0])
				all++;
			else
				break;
			
			start1++;
			start0++;
		}
		//找到了一组 
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
		
		//找到第一个 1  0 的组合
		int index = 0;	
		for (int i = 0; i + 1 < 12; i++) 
			if (helpdata[i] == 1 && helpdata[i + 1] == 0) {
				index = i;
				break;
			}
				
		//交换第一个1和0 
		helpdata[index] = 0; 
		helpdata[index + 1] = 1;
		
		//调整 0 - index1 中的数据 1 在最左边，0在最右边
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

//思考：通过穷举发现，对第一组数，其每个数都是一个限定的范围，
//如果能找到一组不重复且恰好满足该范围的数，是否可行 

//timo81：求数组某数，满足它之前都小于它，它之后都大于它
//快速排序的思维，只需要基准值从头到尾即可，查找不存在交换对时 
//的基准数即可
//求庞大数据下相逆字符串，先hash散列，保证互逆的一对字符串有相同hash值
//进行数据分组，之后再进行比较，降低复杂度 

//timo88：修改字符串，*全部移到前面，非*全部移到后面
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
 
//timo89：逆转单链表
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
	//完成逆转，有三个指针调转
	//前俩个指针PP，P1逆转前俩个节点，并将其交付与PP
	//第三个指针保存后续节点
	//之后每从P2中取一节点便插入PP的前面即可 		
	}	
}

//求俩个字符串的最大字符串
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
	//申请内存空间 
	char** dp = new char*[count1 + 1];
	for (int i = 0; i < count1 + 1; i++)
		dp[i] = new char[count2 + 1];
	//初始化全部 
	for (int i = 0; i < count1 + 1; i++) 
		for (int j = 0; j < count2 + 1; j++) 
			dp[i][j] = 0;	
	//查写数据 
	for (int i = 1; i < count1 + 1; i++) 
		for (int j = 1; j < count2 + 1; j++)
			if (str1[i - 1] == str2[j - 1])
				dp[i][j] = 1 + dp[i - 1][j - 1];
	//打印 
	for (int i = 0; i < count1 + 1; i++) {
		for (int j = 0; j < count2 + 1; j++) 
		 	cout<<int(dp[i][j])<<" ";		
		cout<<endl;
	}
	//只需查找最大数，即可判定最大子字符串
	 
		
	//释放内存空间 
	for (int i = 0; i < count1 + 1; i++)
		delete [] dp[i];	
	delete [] dp;
}












#endif 
