#ifndef __DATASTRUCT_MAP_HPP_
#define __DATASTRUCT_MAP_HPP_

#include<iostream>

//为了直观表示最小生成树
//直接采用邻接矩阵，即使透支过量空间
//虽然临接表也可达到更好的效果，但这里出于简化的目的
//关键的体现是，关于最小生成树算法的实现 

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
				//构建最小权值邻接矩阵
				if (nodeSet == NULL)
					nodeSet = new bool[nodeNum];				
				if (matrixPrim == NULL) {
					matrixPrim = new map<T>*[nodeNum];
					for (int i = 1; i <= nodeNum; i++) {
						matrixPrim[i - 1] = new map<T>[nodeNum];
						nodeSet[i - 1] = false;
					}					
				}
					
				//以下就是Prim算法
				
				//随机选择一个顶点
				nodeSet[abs(rand()) % nodeNum] = true;
				int currentNode = 1; 
				while (currentNode < nodeNum) {
					int minEdge = 0x7fffffff;
					int nodeI = -1, nodeJ = -1;
					
					//从已有集合中寻找一个节点，集合外寻找一个节点
					for (int i = 1; i <= nodeNum; i++) {
						//对所有集合内节点都要查找
						if (nodeSet[i - 1] == false)
							continue;
						for (int j = 1; j <= nodeNum; j++) {
							//集合外一个节点
							if (nodeSet[j - 1] == true)
								continue;
							
							//俩个节点不存在关系
							if (matrix[i - 1][j - 1] == -1)
								continue;								
							//现在已经找到了一个集合内节点与与一个集合外节点
							//为这俩个节点确定是否可使用

							if (matrix[i - 1][j - 1] < minEdge) {
								minEdge = matrix[i - 1][j - 1];
								nodeI = i;
								nodeJ = j;
							}
						}						
					}
						
					//找到集合内与集合外最小权值边，关系导入
					if ((1 <= nodeI && nodeI <= nodeNum) && (1 <= nodeJ && nodeJ <= nodeNum)) {
						matrixPrim[nodeI - 1][nodeJ - 1] = minEdge;	
						matrixPrim[nodeJ - 1][nodeI - 1] = minEdge;
						nodeSet[nodeJ - 1] = true;
						currentNode++;
					}
				}						
			}	
		}
		
		//这个算法更加好采用，优先队列的临接表
		//一般是使用 堆+并查集 实现
		void finishKruskalMapToMinTree1()
		{
			if (nodeNum == -1)
				return;
			else {
				//构建最小权值邻接矩阵				
				if (matrixKruskal == NULL) {
					matrixKruskal = new map<T>*[nodeNum];
					for (int i = 1; i <= nodeNum; i++) 
						matrixKruskal[i - 1] = new map<T>[nodeNum];
								
				}			
			
				//涉及到修改，生成临时矩阵代替
				map<T>** matrixTemp = new map<T>*[nodeNum];
				for (int i = 1; i <= nodeNum; i++) 
					matrixTemp[i - 1] = new map<T>[nodeNum];
				//初始化
				for (int i = 1; i <= nodeNum; i++) 
					for (int j = 1; j <= nodeNum; j++) 
						matrixTemp[i - 1][j - 1] = matrix[i - 1][j - 1];
				
				
				//用简单数据结构代替并查集，直接矩阵搜索代替堆，问题简化
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
					
					//先找一个权值最小的边
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
					//找到一条最小边,现在开始检查	
					if (set[nodeI - 1][nodeJ - 1] == false ) {
						
						set[nodeI - 1][nodeJ - 1] = true;
						set[nodeJ - 1][nodeI - 1] = true;
						
						bool* temp = new bool[nodeNum];
						for (int i = 1; i <= nodeNum; i++) {
							temp[i - 1] |= set[nodeI - 1][i - 1];
							temp[i - 1] |= set[nodeJ - 1][i - 1];
						}
						
						//对nodeI-1中存在的全部节点都添加
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
				//构建最小权值邻接矩阵				
				if (matrixKruskal == NULL) {
					matrixKruskal = new map<T>*[nodeNum];
					for (int i = 1; i <= nodeNum; i++) 
						matrixKruskal[i - 1] = new map<T>[nodeNum];
								
				}			
			
				//涉及到修改，生成临时矩阵代替
				map<T>** matrixTemp = new map<T>*[nodeNum];
				for (int i = 1; i <= nodeNum; i++) 
					matrixTemp[i - 1] = new map<T>[nodeNum];
				//初始化
				for (int i = 1; i <= nodeNum; i++) 
					for (int j = 1; j <= nodeNum; j++) 
						matrixTemp[i - 1][j - 1] = matrix[i - 1][j - 1];
				
				
				//并查集,
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
					
					//先找一个权值最小的边
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
					//找到一条最小边,现在开始检查其集合组是否一致
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
					
					//不同集合，可建立联系
					if (nodeISelf != nodeJSelf) {				
						//集合合并，让后一个节点所有父节点都指向前一个节点祖先节点
						int nodeJSelf = element[nodeJ - 1],
							nodeJFather = itsFather[nodeJ - 1];
						
						while (nodeJSelf != nodeJFather) {
							nodeJSelf = element[nodeJFather - 1];
							nodeJFather = itsFather[nodeJFather - 1];
							//本节点指向祖父节点
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