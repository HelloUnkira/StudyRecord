#ifndef __DATASTRUCT_HASHMAP_HPP_
#define __DATASTRUCT_HASHMAP_HPP_

#include<iostream>
#include<string>

using namespace std;

template <typename T>
class hashmap
{
	private:
		string str;
		T data;
		hashmap* next;
	public:
	//构造函数与析构函数 
	hashmap()
	{
		this->data = T(0);
		this->next = NULL;
		this->str  = "";
	}
	
	hashmap(T Data, hashmap* Node, string Name)
	{
		this->data = Data;
		this->next = Node;
		this->str  = Name;
	}
			
	~hashmap()
	{
	}
	
	//更新函数与获取函数
	string getName()
	{
		return this->str;
	} 
	
	hashmap* getNext()
	{
		return this->next;
	}
	
	T getData()
	{
		return this->data;
	}
	
	void putName(string Name)
	{
		this->str = Name;
	}
	
	void putNext(hashmap* Node)
	{
		this->next = Node;
	}
	
	void putData(T N)
	{
		this->data = N; 
	}
	
	//哈希函数，选取部分哈希函数 
	int elf_hash(const char *name)
	{
   		unsigned long h = 0, g;
		while (*name) {
			h = (h << 4) + *name++;
			if (g = h & 0xf0000000)
				h ^= g >> 24;
			h &= ~g;
		}
		return int(h);
  	}
  	
	int elfHash(string str, int mod)
	{
		return (mod - (elf_hash(str.c_str()) % mod)) % mod;
	}
	
	int other_hash(const char *str)
	{
		register unsigned int h;
		register unsigned char *p;
		for (h = 0, p = (unsigned char *)str; *p; p++)
			h = 31 * h + *p;
		return int(h);
	}
	
	int otherHash(string str, int mod)
	{
		return (mod - (other_hash(str.c_str()) % mod)) % mod;
	}
	 
	//构造散列表
	hashmap** createHashMap(int Mod)
	{
		hashmap<T>** list = new hashmap<T>*[Mod];
		for (int i = 0; i < Mod; i++)
			list[i] = NULL;
		return list;
	}
	
	void destoryHashMap(hashmap** List, int Mod)
	{
		hashmap<T>* plist = NULL;
		hashmap<T>* pbuf  = NULL;	
		for (int i = 0; i < Mod; i++) {
			plist = List[i];
			pbuf  = NULL;
			if (plist == NULL) 
				continue;
			else {
				while (1) {
					if (plist->next == NULL) {
						delete plist;
						break;	
					} else {
						pbuf  = plist->next;
						delete plist;
						plist = pbuf;
					}
				}		
			}
		}
		delete [] List;
		List = NULL;		
	}
			 
	void insertNode(hashmap** List, int Mod, hashmap Node)
	{
		string name = Node.str;
		hashmap<T>* newNode = new hashmap(Node.data, Node.next, Node.str);
		int index = elfHash(name, Mod);
		hashmap<T>* plist = List[index];
		if (plist == NULL)
			List[index] = newNode;
		else {
			while (plist->next != NULL)
				plist = plist->next;
			plist->next = newNode;	
		}
	}
	
	void deleteNode(hashmap** List, int Mod, hashmap Node)
	{
		string name = Node.str;
		int index = elfHash(name, Mod);
		hashmap<T>* plist = List[index];
		hashmap<T>* pbuf = NULL;	
		if (plist == NULL)
			return;
		else if (plist->str == name) {
			List[index] = plist->next;
			delete plist;
		} else while (1) {
			if (plist->next == NULL)
				return;
			else if (plist->next->str == name) {
				pbuf = plist->next;
				plist->next = plist->next->next;
				delete pbuf;
			}					
		}					
	} 
	
	void printHashMap(hashmap**List, int Mod)
	{
		hashmap<T>*phash = NULL;
		for (int i = 0; i < Mod; i++) {
			phash = List[i];
			while (1) {
				if (phash == NULL)
					break;
				else {
					cout << phash->str << endl;
					phash = phash->next;
				}				
			}
			cout << endl;
		}
	}
	
	hashmap*  searchNode(hashmap** List, int Mod, string name)
	{
		int index = elfHash(name, Mod);
		hashmap<T>* plist = List[index];		
		if (List[index] == NULL)
			return NULL;
		else {
			if (plist == NULL)
				return NULL;
			else if (plist->str == name)			
				return plist;				
			else
				plist = plist->next;			
		}
		return NULL;				
	}
				
};

#endif
