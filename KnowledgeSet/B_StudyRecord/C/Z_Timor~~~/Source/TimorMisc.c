/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* Timor_Misc:<Timo1> 二叉查找树到双向链表的转换(递归)
 * 分析1(非本地修改):
 *     typedef struct {
 *         RBT_Node T_Node; //使用该成员构建树,然后迭代树
 *         DL_Node  D_Node; //在迭代树的过程构建双向链表
 *         int32_t  Key;
 *         uint32_t Data;
 *         // ......
 *     } DataStruct;
 * 分析2(本地修改):
 *     直接分治思想解决即可(递归的解决)
 */
/* Timor_Misc:<Timo1>Start */

typedef struct {
    void *Prev_Left;    //表示树-左节点，表示链表-上一节点
    void *Next_Right;   //表示树-右节点，表示链表-下一节点
    int32_t KeyData;
} TM_001_Node;

/* 构建,接收一个已排序数组,构建树 */
TM_001_Node * TM_001_CreateTree(int32_t *List, uint32_t Left, uint32_t Right)
{
    if (Left > Right)
        return NULL;
    if (Left == Right) {
        TM_001_Node* Node = malloc(sizeof(TM_001_Node));
        Node->KeyData    = List[Left];
        Node->Prev_Left  = NULL;
        Node->Next_Right = NULL;
        return Node;
    }
    
    uint32_t Center = (Left + Right) / 2;
    TM_001_Node* Node = malloc(sizeof(TM_001_Node));
    Node->KeyData    = List[Center];
    Node->Prev_Left  = TM_001_CreateTree(List, Left,       Center - 1);
    Node->Next_Right = TM_001_CreateTree(List, Center + 1, Right);
    return Node;
}

static void TM_001_ToListRecursion(TM_001_Node *Node)
{
    /* 直接以递归的方式实现即可,最简单精简 */
    if (Node->Prev_Left == NULL && Node->Next_Right == NULL)
        return;
    /* 处理左子树,将其转化成双向链表 */
    if (Node->Prev_Left != NULL) {
        /* 递归的将左子树转化成双向链表 */
        TM_001_ToListRecursion(Node->Prev_Left);
        /* 迭代到左子树双向链表的最后一个节点,这是前驱 */
        TM_001_Node *Prev = Node->Prev_Left;
        while (Prev->Next_Right != NULL)
            Prev = Prev->Next_Right;
        /* 关联这个前驱 */
        Prev->Next_Right = Node;
        Node->Prev_Left  = Prev;
    }
    /* 处理右子树,将其转化成双向链表 */
    if (Node->Next_Right != NULL) {
        /* 递归的将右子树转化成双向链表 */
        TM_001_ToListRecursion(Node->Next_Right);
        /* 迭代到左子树双向链表的最前一个节点,这是后继 */
        TM_001_Node *Next = Node->Next_Right;
        while (Next->Prev_Left != NULL)
            Next = Next->Prev_Left;
        /* 关联这个后继 */
        Next->Prev_Left  = Node;
        Node->Next_Right = Next;
    }
}

TM_001_Node * TM_001_ToList(TM_001_Node *Root)
{
    TM_001_ToListRecursion(Root);
    /* 迭代到最开始 */
    while (Root->Prev_Left != NULL)
        Root = Root->Prev_Left;
    return Root;
}

void TM_001_Function(void)
{
    int32_t List[10] = {0};
    for (uint32_t Idx = 0; Idx < 10; List[Idx] = Idx * 10 + rand() % 10, Idx++);
    TM_001_Node *Root = TM_001_CreateTree(List, 0, 10 - 1);
    TM_001_Node *Head = TM_001_ToList(Root);
    TM_001_Node *Tail = NULL;
    for (Tail = Head; Tail->Next_Right != NULL; Tail = Tail->Next_Right);
    printf("\nTM_001:");
    for (TM_001_Node *Curr = Head; Curr != NULL; Curr = Curr->Next_Right)
        printf("%d ", Curr->KeyData);
    printf("\nTM_001:");
    for (TM_001_Node *Curr = Tail; Curr != NULL; Curr = Curr->Prev_Left)
        printf("%d ", Curr->KeyData);
}

/* Timor_Misc:<Timo1>End */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* Timor_Misc:<Timo2> 数组的子数组最大和(时间复杂度为O(N))
 */
/* Timor_Misc:<Timo2>Start */

int32_t TM_002_SubArrayMaxSum(int32_t *List, uint32_t Number)
{
    int32_t  MaxVal = 0, SumVal = 0;
    uint32_t MaxPos = 0, SumPos = 0; //起始点(结束点通过累加偏移算)
    for (uint32_t Idx = 0; Idx < Number; Idx++) {
        if ((SumVal += List[Idx]) < List[Idx]) {
             SumVal  = List[Idx];
             SumPos  = Idx;
        }
        if (MaxVal < SumVal) {
            MaxVal = SumVal;
            MaxPos = SumPos;
        }
    }
    return MaxVal; //MaxPos
}

void TM_002_Function(void)
{
    uint32_t List[10] = {3,7,-2,5,-4,9,6,-7,1,2};
    printf("\nTM_002:%u\n", TM_002_SubArrayMaxSum(List, 10));
}

/* Timor_Misc:<Timo2>End */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* Timor_Misc:<Timo3> 位运算和函数实现1~N的和
 */
/* Timor_Misc:<Timo3>Start */

uint32_t TM_003_SumAB(uint32_t Ci, uint32_t A, uint32_t B, uint32_t N)
{
    if (N >= 32)
        return 0;
    
    uint32_t Ai = A & (1 << N);
    uint32_t Bi = B & (1 << N);
    uint32_t Si = Ai ^ Bi ^ Ci;
    uint32_t Cp = ((Ai & Bi) | (Bi & Ci) | (Ai & Ci)) << 1;
    
    return TM_003_SumAB(Cp, A, B, N + 1) | Si;
}

uint32_t TM_003_SumN(uint32_t N)
{
    if (N <= 1)
        return 1;
    return TM_003_SumAB(0, N, TM_003_SumN(N - 1), 0);
}

void TM_003_Function(void)
{
    uint32_t N = 10;
    printf("TM_003:%u\n", TM_003_SumN(N));
}

/* Timor_Misc:<Timo3>End */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* Timor_Misc:<Timo4> 输入俩个整数N,M,在[1,N]为范围求和为M全部数据集合
 * 分析:
 *     使用0-1背包分配,从最大开始
 *     F(N,M)可分为N是否被载入而分为F(N-1, M)与F(N-1, M-N)
 */
/* Timor_Misc:<Timo4>Start */

void TM_004_Sum(int32_t N, int32_t M, bool *Flag, int32_t Num)
{
    if (N  < 1 || M < 1) return;
    if (N  > M)   N = M; //任何>M的数据没有求和组
    if (N == M) {
        Flag[N] = true;
        printf("(-");
        for (int32_t Idx = 1; Idx <= Num; Idx++)
            if (Flag[Idx])
                printf("%d-", Idx);
        printf(")");
        Flag[N] = false;
    }
    Flag[N] = true;
    TM_004_Sum(N - 1, M - N, Flag, Num);
    Flag[N] = false;
    TM_004_Sum(N - 1, M, Flag, Num);
}

void TM_004_Function(void)
{
    int32_t N = 10, M = 7;
    printf("TM_004:");
    bool *Flag = malloc(sizeof(bool) * (N + 1));
    for (int32_t Idx = 0; Idx < N + 1; Flag[Idx] = false, Idx++);
    TM_004_Sum(N, M, Flag, N);
    printf("\n");
    free(Flag);
}

/* Timor_Misc:<Timo4>End */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* Timor_Misc:<Timo5> 交换俩个数组中的元素使得数组和的差最小
 * 分析:
 *     使用0-1背包分配
 */
/* Timor_Misc:<Timo5>Start */

void TM_005_SwapArrayToHarmony(int32_t *List, int32_t Left, int32_t Right)
{   /* 注意:这是俩个相同长度的数组拼接形成,所以它一定是偶数 */
    /* 求俩个数组总的和 */
    int32_t Sum = 0, Len = Right - Left + 1, Half = Len / 2;
    for (int32_t Idx = Left; Idx <= Right; Sum += List[Idx], Idx++);
    /* 申请动态内存,三维数组:Vbest[L_0][L_1][L_2] */
    int32_t ***Vbest = NULL, L_0 = Len + 1, L_1 = Half + 1, L_2 = Sum / 2 + 1;
    /* (Idx0:Len + 1) (Idx1:Half + 1)(Idx2:Sum / 2 + 1) */
        Vbest = malloc(sizeof(int32_t **) * L_0);
    for (int32_t Idx0 = 0; Idx0 < L_0; Idx0++)
        Vbest[Idx0] = malloc(sizeof(int32_t *) * L_1);
    for (int32_t Idx0 = 0; Idx0 < L_0; Idx0++)
    for (int32_t Idx1 = 0; Idx1 < L_1; Idx1++)
        Vbest[Idx0][Idx1] = malloc(sizeof(int32_t) * L_2);
    /* 清空这个三维数组 */
    for (int32_t Idx0 = 0; Idx0 < L_0; Idx0++)
    for (int32_t Idx1 = 0; Idx1 < L_1; Idx1++)
    for (int32_t Idx2 = 0; Idx2 < L_2; Idx2++)
        Vbest[Idx0][Idx1][Idx2] = 0;
    /* 动态规划推导 */
    for (int32_t Idx0 = 1; Idx0 < L_0; Idx0++) {
         int32_t L_M = (Idx0 < Half ? Idx0 : Half) + 1;
    for (int32_t Idx1 = 1; Idx1 < L_M; Idx1++)
    for (int32_t Idx2 = 1; Idx2 < L_2; Idx2++) {
        /* Condition1: */
        if (Idx2 < List[Idx0 - 1]) {
            Vbest[Idx0][Idx1][Idx2] = Vbest[Idx0 - 1][Idx1][Idx2];
            continue;
        }
        /* Condition2: */
        if (Vbest[Idx0 - 1][Idx1][Idx2] <
            Vbest[Idx0 - 1][Idx1 - 1][Idx2 - List[Idx0 - 1]] + List[Idx0 - 1]) {
            Vbest[Idx0][Idx1][Idx2] = 
            Vbest[Idx0 - 1][Idx1 - 1][Idx2 - List[Idx0 - 1]] + List[Idx0 - 1];
            continue;
        }
        /* Condition3: */
            Vbest[Idx0][Idx1][Idx2] = Vbest[Idx0 - 1][Idx1][Idx2];
    }
    }
    /* 将一半数据交换到前半部 */
    int32_t Idx0 = L_0 - 1, Idx1 = L_1 - 1, Idx2 = L_2 - 1, Idx = 0;
    int32_t *Record = malloc(sizeof(int32_t) * Len);
    while (1) {
        /* Condition1: */
        if (Idx0 <= 0)
            break;
        /* Condition2: */
        if (Vbest[Idx0][Idx1][Idx2] == Vbest[Idx0 - 1][Idx1][Idx2]) {
            Idx0--;
            continue;
        }
        /* Condition3: */
        if (Idx2 > List[Idx0 - 1] &&
            Vbest[Idx0][Idx1][Idx2] ==
            Vbest[Idx0 - 1][Idx1 - 1][Idx2 - List[Idx0 - 1]] + List[Idx0 - 1]) {
            /* 将一半数据交换到前半部 */
            Record[Idx++] = Idx0 - 1;
            Idx0--; Idx1--; Idx2 -= List[Idx0 - 1];
        }
    }
    for (int32_t Idx = 0; Idx < Half; Idx++) {
        int32_t Temp = List[Record[Idx]];
        List[Record[Idx]] = List[Idx];
        List[Idx] = Temp;
    }
    free(Record);
    /* 释放动态内存 */
    for (int32_t Idx0 = 0; Idx0 < L_0; Idx0++)
    for (int32_t Idx1 = 0; Idx1 < L_1; Idx1++)
        free(Vbest[Idx0][Idx1]);
    for (int32_t Idx0 = 0; Idx0 < L_0; Idx0++)
        free(Vbest[Idx0]);
        free(Vbest);
}

void TM_005_Function(void)
{
    int32_t List1[6] = {1, 2, 3, 4, 5, 40};
    int32_t List2[6] = {1, 2, 3, 98, 99, 100};
    int32_t List[12] = {1, 2, 3, 4, 5, 40, 1, 2, 3, 98, 99, 100}; 
    TM_005_SwapArrayToHarmony(List, 0, 11);
    printf("TM_005:( ");
    for (int32_t Idx = 0; Idx < 6; Idx++)
        printf("%d ", List[Idx]);
    printf(") ( ");
    for (int32_t Idx = 0; Idx < 6; Idx++)
        printf("%d ", List[Idx + 6]);
    printf(")\n");
}

/* Timor_Misc:<Timo5>End */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* Timor_Misc:<Timo6> 求字符串之间的连接关系(是否成环)
 * 分析:
 *     动态分配
 */
/* Timor_Misc:<Timo6>Start */

/* Str1为Str2的前半部1,Str2为Str1的前半部2,否则0 */
int8_t TM_006_StrTwoConnect(char *Str1, char *Str2)
{
    char *S1 = NULL, *S2 = NULL;
    /* 测试Str1是否为Str2的前半部 */
    for (S1 = Str1, S2 = Str2, S1++; *S1 != 0 && *S1 == *S2; S1++, S2++);
    if (*S1 == 0) return 1;
    /* 测试Str2是否为Str1的前半部 */
    for (S1 = Str1, S2 = Str2, S2++; *S2 != 0 && *S1 == *S2; S1++, S2++);
    if (*S2 == 0) return 2;
    /* 测试失败 */
    return 0;
}

int8_t TM_006_StrNConnect(char **Str, int32_t Num)
{
    /* 负无穷的模拟 */
    int32_t **DP = NULL, **P = NULL;
    /* 申请动态内存 */
    DP = malloc(sizeof(int32_t *) * Num);
     P = malloc(sizeof(int32_t *) * Num);
    for (int32_t Idx = 0; Idx < Num; Idx++) {
        DP[Idx] = malloc(sizeof(int32_t) * Num);
         P[Idx] = malloc(sizeof(int32_t) * Num);
    }
    /* 初始化 */
    for (int32_t Idx0 = 0; Idx0 < Num; Idx0++)
    for (int32_t Idx1 = 0; Idx1 < Num; Idx1++) {
        DP[Idx0][Idx1] = P[Idx0][Idx1] = 0;
        DP[Idx0][Idx0] = -1;
    }
    /* 确定俩字符串连接关系 */
    for (int32_t Idx0 = 0; Idx0 < Num; Idx0++)
    for (int32_t Idx1 = 0; Idx1 < Num; Idx1++) {
        DP[Idx0][Idx1] = (TM_006_StrTwoConnect(Str[Idx0], Str[Idx1]) == 1) ? 1 : -1;
    }
    /* 查询最大路径 */
    for (int32_t Idx0 = 0; Idx0 < Num; Idx0++)
    for (int32_t Idx1 = 0; Idx1 < Num; Idx1++)
    for (int32_t Idx2 = 0; Idx2 < Num; Idx2++)
        if (DP[Idx1][Idx0] > -1 &&
            DP[Idx0][Idx2] > -1 &&
            DP[Idx1][Idx2] < DP[Idx1][Idx0] + DP[Idx0][Idx2]) {
            DP[Idx1][Idx2] = DP[Idx1][Idx0] + DP[Idx0][Idx2];                            
             P[Idx1][Idx2] = Idx0;    
        }
    /* 返回值 */
    int8_t Ret = true;
    for (int32_t Idx = 0; Idx < Num; Idx++)
        if (DP[Idx][Idx] != -1)
            Ret = false;
    /* 本地输出结果 */
    // for (int32_t Idx1 = 0; Idx1 < Num; Idx1++) {
    // for (int32_t Idx2 = 0; Idx2 < Num; Idx2++)
        // printf("%d\t", DP[Idx1][Idx2]);
        // printf("\n");
    // }
    // for (int32_t Idx1 = 0; Idx1 < Num; Idx1++) {
    // for (int32_t Idx2 = 0; Idx2 < Num; Idx2++)
        // printf("%d\t", P[Idx1][Idx2]);
        // printf("\n");
    // }
    /* 释放动态内存 */
    for (int32_t Idx = 0; Idx < Num; Idx++) {
        free(DP[Idx]);
        free( P[Idx]);
    }
        free(DP);
        free( P);
    return Ret;
}

void TM_006_Function(void)
{
	char *Str[11] = {0},
          StrList[11][11] = {
          "qwer","wert","erty","rtyu", "tyuq", "yuqw", "uqwe",
		  "asdf","sdfg","dfgh","dfgz"};
    for (int32_t Idx = 0; Idx < 11; Str[Idx] = StrList[Idx], Idx++);
    /* 字符串之间未连接成环 */
    printf("TM_006:%d\n", TM_006_StrNConnect(Str, 7));
}

/* Timor_Misc:<Timo6>End */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* Timor_Misc:<Timo7> 求字符串全排列(深度优先搜索)
 */
/* Timor_Misc:<Timo7>Start */

void TM_007_StrAllSort(char *Str, uint32_t Left, uint32_t Right)
{
    if (Left == Right) {
        // for (char *S = Str; *S != NULL; printf("%c", *S), S++);
        // printf("\n");
    } else {
        for (int32_t Idx = Left; Idx <= Right; Idx++) {
            if (1) {
                char Char = Str[Idx];
                Str[Idx]  = Str[Left];
                Str[Left] = Char;
            }
            TM_007_StrAllSort(Str, Left + 1, Right);
            if (1) {
                char Char = Str[Idx];
                Str[Idx]  = Str[Left];
                Str[Left] = Char;
            }
        }
    }
}

void TM_007_Function(void)
{
    char Str[5] = "012";
    printf("TM_007:\n");
    TM_007_StrAllSort(Str, 0, 2);
}

/* Timor_Misc:<Timo7>End */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* Timor_Misc:<Timo8> 颠倒栈(回溯递归)
 */
/* Timor_Misc:<Timo8>Start */

void TM_008_TopToButtom(int32_t *Stack, int32_t Top, int32_t Data)
{
    if (Top < 0)
        Stack[0] = Data;
    else {
        int32_t Temp = Stack[Top];
        TM_008_TopToButtom(Stack, Top - 1, Data);
        Stack[Top + 1] = Temp;
    }
}

void TM_008_ReverseStack(int32_t *Stack, int32_t Top)
{
    if (Top < 0)
        return;
    int32_t Temp = Stack[Top];
    TM_008_ReverseStack(Stack, Top - 1);
    TM_008_TopToButtom( Stack, Top - 1, Temp);
}

void TM_008_Function(void)
{
    int32_t Stack[10] = {0,1,2,3,4,5,6,7,8,9};
    TM_008_ReverseStack(Stack, 9);
    printf("TM_008:");
    for (int32_t Idx = 0; Idx < 10; printf("%d", Stack[Idx]), Idx++);
    printf("\n");
}

/* Timor_Misc:<Timo8>End */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* Timor_Misc:<Timo9> 查找旋转数组的最小元素索引
 */
/* Timor_Misc:<Timo9>Start */

int32_t TM_009_RotatedArraySearchMin(int32_t *Array, int32_t Left, int32_t Right)
{
    /* 最小元素满足Array[Idx - 1] > Array[Idx] */
    for (int32_t Idx1 = Left + 1, Idx2 = Right; Idx1 <= Right && Idx2 > Left; Idx1++, Idx2--) {
        /* 旋转数组要从俩个方向找,谁先碰撞到谁就先找到分界点 */
        if (Array[Idx1 - 1] > Array[Idx1])
            return Idx1;
        if (Array[Idx2 - 1] > Array[Idx2])
            return Idx2;
    }
}

void TM_009_Function(void)
{
    int32_t Array[10] = {4,5,6,7,8,9,0,1,2,3};
    printf("TM_009:%d\n", TM_009_RotatedArraySearchMin(Array, 0, 9));
}

/* Timor_Misc:<Timo9>End */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* Timor_Misc:<Timo10> 查找数组中出现次数超过一半的数字
 * 分析:
 *     该问题里面的超过一半貌似是硬性需求???
 */
/* Timor_Misc:<Timo10>Start */

int32_t TM_010_SearchOverHalfNumIndex(int32_t *Array, int32_t Left, int32_t Right)
{
    int32_t Data = Array[Left++], Num = 1;
    for (int32_t Idx = Left; Idx <= Right; Idx++) {
        if (Data == Array[Idx])
            Num++;
        else
            Num--;
        if (Num == 0) {
            Num  = 1;
            Data = Array[Idx];
        }
    }
    return Data;
}

void TM_010_Function(void)
{
    int32_t Array[10] = {1,0,0,2,0,0,3,0,0,3,};
    printf("TM_010:%d\n", TM_010_SearchOverHalfNumIndex(Array, 0, 9));
}

/* Timor_Misc:<Timo10>End */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* Timor_Misc:<Timo11> 最长公共子串(动态规划)
 */
/* Timor_Misc:<Timo11>Start */

int32_t TM_011_StringMaxSub(char *Str1, char *Str2)
{
    int32_t Len1 = strlen(Str1), Len2 = strlen(Str2), **DP = NULL;
    /* 动态内存 */
        DP = malloc(sizeof(int32_t *) * (Len1 + 1));
    for (int32_t Idx = 0; Idx < Len1 + 1; Idx++)
        DP[Idx] = malloc(sizeof(int32_t) * (Len2 + 1));
    /* 初始化 */
    for (int32_t Idx1 = 0; Idx1 < Len1 + 1; Idx1++)
    for (int32_t Idx2 = 0; Idx2 < Len2 + 1; Idx2++)
        DP[Idx1][Idx2] = 0;
    /* 动态规划推导 */
    for (int32_t Idx1 = 1; Idx1 < Len1 + 1; Idx1++)
    for (int32_t Idx2 = 1; Idx2 < Len2 + 1; Idx2++)
        if (Str1[Idx1 - 1] == Str2[Idx2 - 1]) {
            int32_t Max = 0;
            for (int32_t Idx3 = 1; Idx3 < Idx1; Idx3++)
            for (int32_t Idx4 = 1; Idx4 < Idx2; Idx4++)
                if (DP[Idx3][Idx4] != 0)
                    Max = Max > DP[Idx3][Idx4] ? Max : DP[Idx3][Idx4];
            DP[Idx1][Idx2] = Max + 1;
        }
    /* 计算最大字串长度 */
    int32_t Max = 0;
    for (int32_t Idx1 = 0; Idx1 < Len1 + 1; Idx1++)
    for (int32_t Idx2 = 0; Idx2 < Len2 + 1; Idx2++)
        Max = DP[Idx1][Idx2] > Max ? DP[Idx1][Idx2] : Max;
    /* 本地输出结果 */
    // for (int32_t Idx1 = 0; Idx1 < Len1 + 1; Idx1++) {
    // for (int32_t Idx2 = 0; Idx2 < Len2 + 1; Idx2++)
        // printf("%d ", DP[Idx1][Idx2]);
        // printf("\n");
    // }
    /* 释放动态内存 */
    for (int32_t Idx = 0; Idx < Len1 + 1; Idx++)
        free(DP[Idx]);
        free(DP);
    return Max;
}

void TM_011_Function(void)
{
    char Str1[] = "*Hello***Kitty*";
    char Str2[] = "*World***Hello*";
    printf("TM_011:%d\n", TM_011_StringMaxSub(Str1, Str2));
}

/* Timor_Misc:<Timo11>End */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
