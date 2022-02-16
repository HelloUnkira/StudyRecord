#ifndef __DATASTRUCT_MAP_HPP_
#define __DATASTRUCT_MAP_HPP_

#include<iostream>

//Ϊ��ֱ�۱�ʾ��С������
//ֱ�Ӳ����ڽӾ��󣬼�ʹ͸֧�����ռ�
//��Ȼ�ٽӱ�Ҳ�ɴﵽ���õ�Ч������������ڼ򻯵�Ŀ��
//�ؼ��������ǣ�������С�������㷨��ʵ�� 

using namespace std;

template<typename T>
class map{
	private:
		static map<T>** matrix;
		static int nodeNum;		
		
		static bool* nodeSet;
		static map<T>** matrixPrim;
		
		static map<T>** matrixKruskal;
		
		int weight;		
	public:
		map(int weight = -1)
		{
			this->weight = weight;
		}
		
		~map()
		{
		}
		
		void createMap(int n)
		{
			if (matrix != NULL)
				return;
			nodeNum = n;
			matrix = new map<T>*[nodeNum];
			for (int i = 1; i <= nodeNum; i++)
				matrix[i - 1] = new map<T>[nodeNum];
		}
		
		void destroyMap()
		{
			if (matrix == NULL)
				return;
			for (int i = 1; i <= nodeNum; i++)
				delete [] matrix[i - 1];
			delete [] matrix;
			matrix = NULL;
			nodeNum = -1;
		}
		
		void addEdge(int V1, int V2, int weight)
		{
			if (matrix == NULL)
				return;			
			if ((1 <= V1 && V1 <= nodeNum) && (1 <= V2 && V2 <= nodeNum) && weight > 0) {
				matrix[V1 - 1][V2 - 1].weight = weight;
				matrix[V2 - 1][V1 - 1].weight = weight;
			}
		}
		
		void deleteEdge(int V1, int V2)
		{
			if (matrix == NULL)
				return;
			if ((1 <= V1 && V1 <= nodeNum) && (1 <= V2 && V2 <= nodeNum)) {
				matrix[V1 - 1][V2 - 1].weight = -1;			
				matrix[V2 - 1][V1 - 1].weight = -1;	
			}
		}
		
		friend ostream& operator<<(ostream& out, map<T> matrix)
		{
			return out<<matrix.weight;
		}
		
		operator int()
		{
			return this->weight;
		}
		
		void printMap()
		{
			if (matrix != NULL)
				for (int i = 1; i <= nodeNum; i++) {
					for (int j = 1; j <= nodeNum; j++)
						cout<<int(matrix[i - 1][j - 1])<<"\t";
					cout<<endl;
				}
			if (matrixPrim != NULL) {
				cout<<endl<<"Prim:"<<endl;
				for (int i = 1; i <= nodeNum; i++)
					cout<<nodeSet[i - 1];
				cout<<endl;
				for (int i = 1; i <= nodeNum; i++) {
					for (int j = 1; j <= nodeNum; j++)
						cout<<int(matrixPrim[i - 1][j - 1])<<"\t";
					cout<<endl;
				}						
			}
			if (matrixKruskal != NULL) {
				cout<<endl<<"Kruskal:"<<endl;
				for (int i = 1; i <= nodeNum; i++) {
					for (int j = 1; j <= nodeNum; j++)
						cout<<int(matrixKruskal[i - 1][j - 1])<<"\t";
					cout<<endl;
				}					
				
			}
		}
	
		void finishPrimMapToMinTree()
		{
			if (nodeNum == -1)
				return;
			else {
				//������СȨֵ�ڽӾ���
				if (nodeSet == NULL)
					nodeSet = new bool[nodeNum];				
				if (matrixPrim == NULL) {
					matrixPrim = new map<T>*[nodeNum];
					for (int i = 1; i <= nodeNum; i++) {
						matrixPrim[i - 1] = new map<T>[nodeNum];
						nodeSet[i - 1] = false;
					}					
				}
					
				//���¾���Prim�㷨
				
				//���ѡ��һ������
				nodeSet[abs(rand()) % nodeNum] = true;
				int currentNode = 1; 
				while (currentNode < nodeNum) {
					int minEdge = 0x7fffffff;
					int nodeI = -1, nodeJ = -1;
					
					//�����м�����Ѱ��һ���ڵ㣬������Ѱ��һ���ڵ�
					for (int i = 1; i <= nodeNum; i++) {
						//�����м����ڽڵ㶼Ҫ����
						if (nodeSet[i - 1] == false)
							continue;
						for (int j = 1; j <= nodeNum; j++) {
							//������һ���ڵ�
							if (nodeSet[j - 1] == true)
								continue;
							
							//�����ڵ㲻���ڹ�ϵ
							if (matrix[i - 1][j - 1] == -1)
								continue;								
							//�����Ѿ��ҵ���һ�������ڽڵ�����һ��������ڵ�
							//Ϊ�������ڵ�ȷ���Ƿ��ʹ��

							if (matrix[i - 1][j - 1] < minEdge) {
								minEdge = matrix[i - 1][j - 1];
								nodeI = i;
								nodeJ = j;
							}
						}						
					}
						
					//�ҵ��������뼯������СȨֵ�ߣ���ϵ����
					if ((1 <= nodeI && nodeI <= nodeNum) && (1 <= nodeJ && nodeJ <= nodeNum)) {
						matrixPrim[nodeI - 1][nodeJ - 1] = minEdge;	
						matrixPrim[nodeJ - 1][nodeI - 1] = minEdge;
						nodeSet[nodeJ - 1] = true;
						currentNode++;
					}
				}						
			}	
		}
		
		//����㷨���Ӻò��ã����ȶ��е��ٽӱ�
		//һ����ʹ�� ��+���鼯 ʵ��
		void finishKruskalMapToMinTree1()
		{
			if (nodeNum == -1)
				return;
			else {
				//������СȨֵ�ڽӾ���				
				if (matrixKruskal == NULL) {
					matrixKruskal = new map<T>*[nodeNum];
					for (int i = 1; i <= nodeNum; i++) 
						matrixKruskal[i - 1] = new map<T>[nodeNum];
								
				}			
			
				//�漰���޸ģ�������ʱ�������
				map<T>** matrixTemp = new map<T>*[nodeNum];
				for (int i = 1; i <= nodeNum; i++) 
					matrixTemp[i - 1] = new map<T>[nodeNum];
				//��ʼ��
				for (int i = 1; i <= nodeNum; i++) 
					for (int j = 1; j <= nodeNum; j++) 
						matrixTemp[i - 1][j - 1] = matrix[i - 1][j - 1];
				
				
				//�ü����ݽṹ���沢�鼯��ֱ�Ӿ�����������ѣ������
				bool** set = new bool*[nodeNum];
				for (int i = 1; i <= nodeNum; i++) 
					set[i - 1] = new bool[nodeNum];
				for (int i = 1; i <= nodeNum; i++) 
					for (int j = 1; j <= nodeNum; j++) 
						if (i == j)
							set[i - 1][j - 1] = true;
						else
							set[i - 1][j - 1] = false;
				
				int currentLink = nodeNum;
				while (currentLink > 1) {
					int weight = 0x7fffffff;
					int nodeI = -1;
					int nodeJ = -1;
					
					//����һ��Ȩֵ��С�ı�
					for (int i = 1; i <= nodeNum; i++)
						for (int j = 1; j <= nodeNum; j++)
							if (matrixTemp[i - 1][j - 1] == -1)
								continue;
							else if (matrixTemp[i - 1][j - 1] < weight) {
								weight = matrixTemp[i - 1][j - 1];
								nodeI = i;
								nodeJ = j;
							}
							
					if (weight == 0x7fffffff)
						break;								
					//�ҵ�һ����С��,���ڿ�ʼ���	
					if (set[nodeI - 1][nodeJ - 1] == false ) {
						
						set[nodeI - 1][nodeJ - 1] = true;
						set[nodeJ - 1][nodeI - 1] = true;
						
						bool* temp = new bool[nodeNum];
						for (int i = 1; i <= nodeNum; i++) {
							temp[i - 1] |= set[nodeI - 1][i - 1];
							temp[i - 1] |= set[nodeJ - 1][i - 1];
						}
						
						//��nodeI-1�д��ڵ�ȫ���ڵ㶼���
						for (int i = 1; i <= nodeNum; i++) {
							if (i == nodeI || i == nodeJ)
								continue;
							if (set[i - 1][nodeI - 1] == true || \
								set[i - 1][nodeJ - 1] == true) {
									
								for (int j = 1; j <= nodeNum; j++)
									set[i - 1][j - 1] |= temp[j - 1];
							}
						}		
							
						matrixKruskal[nodeI - 1][nodeJ - 1] = weight;
						matrixKruskal[nodeJ - 1][nodeI - 1] = weight;
						currentLink--;
						
					}
					
					matrixTemp[nodeI - 1][nodeJ - 1] = -1;
				}				
				
				for (int i = 1; i <= nodeNum; i++) 
					delete [] set[i - 1];
				delete [] set;
				
				for (int i = 1; i <= nodeNum; i++)
					delete [] matrixTemp[i - 1];
				delete [] matrixTemp;		
			}
		}

		void finishKruskalMapToMinTree()
		{
			if (nodeNum == -1)
				return;
			else {
				//������СȨֵ�ڽӾ���				
				if (matrixKruskal == NULL) {
					matrixKruskal = new map<T>*[nodeNum];
					for (int i = 1; i <= nodeNum; i++) 
						matrixKruskal[i - 1] = new map<T>[nodeNum];
								
				}			
			
				//�漰���޸ģ�������ʱ�������
				map<T>** matrixTemp = new map<T>*[nodeNum];
				for (int i = 1; i <= nodeNum; i++) 
					matrixTemp[i - 1] = new map<T>[nodeNum];
				//��ʼ��
				for (int i = 1; i <= nodeNum; i++) 
					for (int j = 1; j <= nodeNum; j++) 
						matrixTemp[i - 1][j - 1] = matrix[i - 1][j - 1];
				
				
				//���鼯,
				int* element = new int[nodeNum];
				int* itsFather = new int[nodeNum];
				for (int i = 1; i <= nodeNum; i++) {
					element[i - 1] = i;
					itsFather[i - 1] = i;			
				}
				
				int currentLink = nodeNum;
				while (currentLink > 1) {
					int weight = 0x7fffffff;
					int nodeI = -1;
					int nodeJ = -1;
					
					//����һ��Ȩֵ��С�ı�
					for (int i = 1; i <= nodeNum; i++)
						for (int j = 1; j <= nodeNum; j++)
							if (matrixTemp[i - 1][j - 1] == -1)
								continue;
							else if (matrixTemp[i - 1][j - 1] < weight) {
								weight = matrixTemp[i - 1][j - 1];
								nodeI = i;
								nodeJ = j;
							}
							
					if (weight == 0x7fffffff)
						break;								
					//�ҵ�һ����С��,���ڿ�ʼ����伯�����Ƿ�һ��
					int nodeISelf = element[nodeI - 1], 
						nodeJSelf = element[nodeJ - 1],
						nodeIFather = itsFather[nodeI - 1], 
						nodeJFather = itsFather[nodeJ - 1];
					
					while (nodeISelf != nodeIFather) {
						nodeISelf = element[nodeIFather - 1];
						nodeIFather = itsFather[nodeIFather - 1];
					}
					
					while (nodeJSelf != nodeJFather) {
						nodeJSelf = element[nodeJFather - 1];
						nodeJFather = itsFather[nodeJFather - 1];
					}					
					
					//��ͬ���ϣ��ɽ�����ϵ
					if (nodeISelf != nodeJSelf) {				
						//���Ϻϲ����ú�һ���ڵ����и��ڵ㶼ָ��ǰһ���ڵ����Ƚڵ�
						int nodeJSelf = element[nodeJ - 1],
							nodeJFather = itsFather[nodeJ - 1];
						
						while (nodeJSelf != nodeJFather) {
							nodeJSelf = element[nodeJFather - 1];
							nodeJFather = itsFather[nodeJFather - 1];
							//���ڵ�ָ���游�ڵ�
							itsFather[nodeJSelf - 1] = nodeISelf;
						}
						itsFather[nodeJSelf - 1] = nodeISelf;

							
						matrixKruskal[nodeI - 1][nodeJ - 1] = weight;
						matrixKruskal[nodeJ - 1][nodeI - 1] = weight;
						currentLink--;
						
					}
					matrixTemp[nodeI - 1][nodeJ - 1] = -1;
				}
				
				
				delete [] element;
				delete [] itsFather;
				
				for (int i = 1; i <= nodeNum; i++)
					delete [] matrixTemp[i - 1];
				delete [] matrixTemp;			
			}
		}	
};

template<typename T>
map<T>** map<T>::matrix = NULL;
template<typename T>
int map<T>::nodeNum = -1;

template<typename T>
map<T>** map<T>::matrixPrim = NULL;
template<typename T>
bool* map<T>::nodeSet = NULL;

template<typename T>
map<T>** map<T>::matrixKruskal = NULL;


void testMapToTree()
{
	map<int>* aHa;
	(*aHa).createMap(6);
	
	(*aHa).addEdge(1,2,6);
	(*aHa).addEdge(1,3,1);
	(*aHa).addEdge(1,4,5);
	(*aHa).addEdge(2,3,5);
	(*aHa).addEdge(2,5,3);
	(*aHa).addEdge(5,3,6);
	(*aHa).addEdge(5,6,6);
	(*aHa).addEdge(3,6,4);
	(*aHa).addEdge(6,4,2);
	(*aHa).addEdge(3,4,5);
	
	
	(*aHa).finishPrimMapToMinTree();
	
	(*aHa).finishKruskalMapToMinTree();
	
	//(*aHa).finishKruskalMapToMinTree1();
	
	(*aHa).printMap();
	
	(*aHa).destroyMap();
}









#endif