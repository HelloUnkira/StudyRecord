#ifndef __DATASTRUCT_TRIE_HPP_
#define __DATASTRUCT_TRIE_HPP_

#include<iostream>

//���ڲ������ṹ 

using namespace std;

template<typename T>
class trie{
	private:
		char currentChar;
		int currentIndexNumber;
		trie<T>** nextIndex;
		static trie<T>* trieRoot;
		static int Max;
		
	public:
		trie()
		{
			this->currentChar = '\0';
			this->currentIndexNumber = 0;
			this->nextIndex = new trie<T>*[Max];
			for (int i = 0; i < Max; i++)
				this->nextIndex[i] = NULL;
		}
	
		trie(char data)
		{
			this->currentChar = data;
			this->currentIndexNumber = 0;
			this->nextIndex = new trie<T>*[Max];
			for (int i = 0; i < Max; i++)
				this->nextIndex[i] = NULL;			
		}
		
		~trie()
		{
		}
		
		void creatTrie()
		{
			trieRoot = new trie<T>('/');		
		}
		
		void insertString(char* str)
		{
			trie<T>* buf = trieRoot;
			while (*str != '\0') {
				if (buf->nextIndex[*str] == NULL) {
					buf->nextIndex[*str] = new trie<T>(*str);
					buf->currentIndexNumber++;
				}
				buf = buf->nextIndex[*str++];
			}
		}
		
		bool searchString(char* str)
		{
			trie<T>* buf = trieRoot;
			while (*str != '\0') {
				if (buf->nextIndex[*str] == NULL)
					return false;
				buf = buf->nextIndex[*str++];
			}
			return true;			
		}
		
		int helpDeleteString(char* str, trie<T>* buf = trieRoot)
		{
			//ɾ�� str ����� ��ɾ����ǰ�ַ�
			if (*str == '\0')
				return 2;
			else if (buf == NULL)	//û�ߵ�ĩβ�Ͳ鲻����
				return 0;
			else {				
				switch(helpDeleteString(str + 1, buf->nextIndex[*str])) {
				case 0:		//ѹ��û�ҵ�Ҫɾ�ģ�ֱ����
					return 0;
				case 1:
					buf->nextIndex[*str] = NULL;
					buf->currentIndexNumber--;
				case 2:
					;//�Ӵ�β���˵ģ������赱ǰ�ڵ��������Ϊ��������һ�ڵ�
				default :
					;
				}
		
				//�������ݣ����ڵ㲻�������������ڵ���ɾ����Ϊ�ﵽƥ����
				if (buf->currentIndexNumber == 0) {
					delete buf;
					return 1;
				}					
			}					
		}		
		
		void deleteString(char* str)
		{
			helpDeleteString(str);
		}
		
		void deleteTrie(trie<T>* buf = trieRoot)
		{
			if (buf == NULL)
				return;
			else {
				for (int i = 0; i < Max; i++)
					if (buf->nextIndex[i] != NULL)
						deleteTrie(buf->nextIndex[i]);
				delete buf;
			}	
		}
		
};

template<typename T>
	trie<T>* trie<T>::trieRoot = NULL;
template<typename T>
	int trie<T>::Max = (unsigned char)(~0) + 1;

void testTrie()
{
	trie<char>* root;
	(*root).creatTrie();
	
	char str1[] = "Radd";
	char str2[] = "Ram";
	char str3[] = "Rthe";
	char str4[] = "Rthink";
	char str5[] = "Rgood";
	char str6[] = "Tch";
	char str7[] = "Ramew";
	
	(*root).insertString(str1);
	(*root).insertString(str2);
	(*root).insertString(str3);
	(*root).insertString(str4);
	(*root).insertString(str5);
	(*root).insertString(str6);
	(*root).insertString(str1);
	
	cout<<(*root).searchString(str3);
	cout<<(*root).searchString(str5);
	cout<<(*root).searchString(str7);
	
	(*root).deleteString(str3);
	(*root).deleteString(str5);
	(*root).deleteString(str7);
	
	cout<<(*root).searchString(str3);
	cout<<(*root).searchString(str5);
	cout<<(*root).searchString(str7);
	
	(*root).deleteTrie();
}



#endif