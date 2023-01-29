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
/* TC1:<Timo1> 俩数之和
 * 分析:
 *     散列表
 */
/* TC1:<Timo1>Start */

typedef struct {
    void    *Near;
    uint32_t Index;
} TC1_001_Node;

void TC1_001_TwoNumSum(int32_t *List, uint32_t Num, int32_t Target)
{
    bool **Used = malloc(sizeof(bool *) * Num);
    for (uint32_t Idx = 0; Idx < Num; Idx++)
        Used[Idx] = malloc(sizeof(bool) * Num);
    for (uint32_t Idx1 = 0; Idx1 < Num; Idx1++)
    for (uint32_t Idx2 = 0; Idx2 < Num; Idx2++)
        Used[Idx1][Idx2] = 0;
    TC1_001_Node **Table = malloc(sizeof(TC1_001_Node *) * Num);
    for (uint32_t Idx = 0; Idx < Num; Table[Idx] = NULL, Idx++);
    /* 散列函数: abs(Target - List[Idx]) % Num */
    for (uint32_t Idx = 0; Idx < Num; Idx++) {
        TC1_001_Node *Node = malloc(sizeof(TC1_001_Node));
        Node->Near = Table[abs(Target - List[Idx]) % Num];
        Table[abs(Target - List[Idx]) % Num] = Node;
        Node->Index = Idx;
    }
    /* 二次遍历检查 */
    for (uint32_t Idx = 0; Idx < Num; Idx++) {
        if (Table[abs(List[Idx]) % Num] == NULL)
            continue;
        TC1_001_Node *Node = Table[abs(List[Idx]) % Num];
        /* 输出这些数据对: */
        while (Node != NULL) {
            if (Node->Index != Idx &&
               !Used[Idx][Node->Index] &&
               !Used[Node->Index][Idx] &&
               (List[Node->Index] + List[Idx] == Target)) {
                printf("(%d-%d)\t", Idx, Node->Index);
                Used[Idx][Node->Index] = true;
                Used[Node->Index][Idx] = true;
            }
            Node = Node->Near;
        }
    }
    /* 释放空间 */
    for (uint32_t Idx = 0; Idx < Num; Idx++)
        free(Used[Idx]);
    for (uint32_t Idx = 0; Idx < Num; Idx++) {
        if (Table[Idx] == NULL)
            continue;
        TC1_001_Node *Node = Table[Idx];
        TC1_001_Node *Free = NULL;
        while (Node != NULL) {
            Free = Node;
            Node = Node->Near;
            free(Free);
        }
    }
    free(Table);
    free(Used);
}

void TC1_001_Function(void)
{
    int32_t List[10] = {11,2,3,3,14,6,8,9,0,2};
    printf("TC1_001:%d:\t", 17);
    TC1_001_TwoNumSum(List, 10, 17);
    printf("\n");
}

/* TC1:<Timo1>End */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* TC1:<Timo2> 最长不重复字串
 * 分析:
 *     滑动窗口
 */
/* TC1:<Timo2>Start */

void TC1_002_StrMaxSubPatition(char *Str)
{
    bool Used[256] = {0};
    char *Window = Str;
    int32_t Max = 0; char *MaxIdx = Str;
    int32_t Cur = 0; char *CurIdx = Str;

    while (*Window != 0) {
        Cur++;
        if (Used[*Window] == 0) {
            Used[*Window] = 1;
            if (Max < Cur) {
                Max = Cur;
                MaxIdx = CurIdx;
            }
        } else {
            while (*CurIdx != *Window) {
                Used[*CurIdx] = 0;
                CurIdx++; Cur--;
            }
            CurIdx++; Cur--;
        }
        Window++;
    }
    printf("(%d-%d)(", Max, MaxIdx - Str);
    while (Max-- > 0)
        printf("%c", *(MaxIdx++));
    printf(")");
}

void TC1_002_Function(void)
{
    printf("TC1_002:");
    TC1_002_StrMaxSubPatition("asdfgstdsaqwerytuott");
    printf("\n");
}

/* TC1:<Timo2>End */ 
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* TC1:<Timo3> 俩个正序数组的中位数, 偶数则求平均(时间复杂度O(log(M + N)))
 * 分析:
 *     二分查找
 */
/* TC1:<Timo3>Start */

float TC1_003_SMid(int32_t *List1, int32_t Left1, int32_t Right1,
                   int32_t *List2, int32_t Left2, int32_t Right2, int32_t K)
{
    //printf("%d - %d - %d - %d\n", Left1, Right1, Left2, Right2);
    if (Left1 > Right1) 
        return (float)(List2[Left2 + K - (Left1 - Right1)] +
                       List2[Left2 + K - (Left1 - Right1) + 1]) / 2;        
    if (Left2 > Right2)
        return (float)(List1[Left1 + K - (Left2 - Right2)] +
                       List1[Left1 + K - (Left2 - Right2) + 1]) / 2;        
    if (K == 1) 
        return (float)(List1[Left1] + List2[Left2]) / 2;    
    /* 从List1和List2中淘汰掉K/2个数据 */
    if (Left1 + K / 2 > Right1 ||
        List1[Left1 + K / 2] >= List2[Left2 + K / 2]) {
        int32_t L1 = Left1;
        int32_t R1 = Right1;
        int32_t L2 = Left2 + K / 2;
        int32_t R2 = Right2;
        return TC1_003_SMid(List1, L1, R1, List2, L2, R2, K - K / 2);
    }
    if (Left2 + K / 2 > Right2 ||
        List2[Left2 + K / 2] >= List1[Left1 + K / 2]) {
        int32_t L1 = Left1 + K / 2;
        int32_t R1 = Right1;
        int32_t L2 = Left2;
        int32_t R2 = Right2;
        return TC1_003_SMid(List1, L1, R1, List2, L2, R2, K - K / 2);
    }
}

float TC1_003_SearchMiddle(int32_t *List1, int32_t Num1, int32_t *List2, int32_t Num2)
{
    return TC1_003_SMid(List1, 0, Num1 - 1, List2, 0, Num2 - 1, (Num1 + Num2) / 2);
}

void TC1_003_Function(void)
{
	int32_t List1[10] = {2,3,5,7,8,10,13,15,17,18};
	int32_t List2[10] = {1,2,3,4,5,7,9,11,38,40};
	printf("TC1_003:%f\n", TC1_003_SearchMiddle(List1, 10, List2, 10));
}

/* TC1:<Timo3>End */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* TC1:<Timo4> Manachar计算最长回文字串
 * 分析:
 *     1.Manachar计算Palindrome
 *     2.
 *       dp[i][i] true 
 *       dp[i][i + 1] true if s[i] == [i + 1]
 *       dp[i][j] true if dp[i +1][j - 1] && s[i] == s[j]
 */
/* TC1:<Timo4>Start */

void TC1_004_ManacharPalindrome(char *Str)
{
    int32_t  StrLen = strlen(Str) * 2 + 1;
	char    *StrNew = malloc(sizeof(char) *    StrLen);
	int32_t *LenNew = malloc(sizeof(int32_t) * StrLen);
    /* 填充# */
    bool Rev = false;
	for (int32_t Idx = 0; Idx < StrLen; Idx++) {
        if (Rev) {
            StrNew[Idx] = *Str++;
            Rev = false;
        } else {
            StrNew[Idx] = '#';
            Rev = true;
        }
		LenNew[Idx] = 1;
    }
    /* Manachar */
    int32_t MaxPos = -1, MaxIdx = -1;
    for (int32_t Idx = 0; Idx < StrLen; Idx++) {
        #if 1
        /* 定点 */
        LenNew[Idx] = Idx < MaxIdx ?
                      LenNew[2 * MaxPos - Idx] < MaxIdx - Idx ?
                      LenNew[2 * MaxPos - Idx] : MaxIdx - Idx : 1;
        /* 扩散 */
        while (StrNew[Idx - LenNew[Idx]] == StrNew[Idx + LenNew[Idx]])
               LenNew[Idx]++;
        /* 更新 */
        if (MaxIdx < Idx + LenNew[Idx]) {
            MaxIdx = Idx + LenNew[Idx];
            MaxPos = Idx;
        }
        #else
        if (Idx < MaxIdx) {
            /* Idx相对MaxPos的对称点 */
            if ((LenNew[Idx] = LenNew[2 * MaxPos - Idx]) >= MaxIdx - Idx)
            /* 从Idx + MaxIdx开始扩散 */
            for (int32_t IdxT = LenNew[Idx];
                (Idx - IdxT >= 0) && (Idx + IdxT < StrLen); IdxT++)
                if (StrNew[Idx - IdxT] == StrNew[Idx + IdxT])
                    LenNew[Idx]++;
                else
                    break;
        } else {
            for (int32_t IdxT = 1;
                (Idx - IdxT >= 0) && (Idx + IdxT < StrLen); IdxT++)
                if (StrNew[Idx - IdxT] == StrNew[Idx + IdxT])
                    LenNew[Idx]++;
                else
                    break;
        }
        /* 更新 */
        if (MaxIdx < Idx + LenNew[Idx]) {
            MaxIdx = Idx + LenNew[Idx];
            MaxPos = Idx;
        }
        #endif
    }
    /* 输出回文串: */
    printf("\tStrNew:");
    for (int32_t Idx = 0; Idx < StrLen; Idx++)
        printf("%c ", StrNew[Idx]);
	printf("\n");
    printf("\tLenNew:");
    for (int32_t Idx = 0; Idx < StrLen; Idx++)
        printf("%d ", LenNew[Idx]);
	printf("\n");
    /*  */
    free(StrNew);
    free(LenNew);
}

void TC1_004_Function(void)
{
    printf("TC1_004:\n");
    TC1_004_ManacharPalindrome("abacdadca");
}

/* TC1:<Timo4>End */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* TC1:<Timo5> 装水最多的容器
 * 分析:
 *     不等式原理
 *     MaxSum = (Right - Left) * Min(List[Left], List[Right])
 */
/* TC1:<Timo5>Start */

void TC1_005_MostWater(int32_t *List, int32_t Num)
{
    int32_t MaxSum = 0, Sum = 0;
    int32_t CurLeft = 0, CurRight = Num - 1;
    int32_t MaxLeft = 0, MaxRight = Num - 1;
    
    while (CurLeft < CurRight) {
        
        Sum  = CurRight - CurLeft;
        Sum *= List[CurLeft] < List[CurRight] ? List[CurLeft] : List[CurRight];
        
        if (MaxSum < Sum) {
            MaxSum = Sum;
            MaxLeft  = CurLeft;
            MaxRight = CurRight;
        }
        
        if (List[CurLeft] < List[CurRight])
            CurLeft++;
        else
            CurRight--;
    }
    
    printf("((%d, %d) : %d)", MaxLeft, MaxRight, MaxSum);
}

void TC1_005_Function(void)
{
	int32_t List[9] = {1,8,6,2,5,4,8,3,7};
	printf("TC1_005:");
    TC1_005_MostWater(List, 9);
    printf("\n");
}

/* TC1:<Timo5>End */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* TC1:<Timo6> 三数和最相近,三数和,四叔和
 * 分析:
 *     排序后滑动查找O(N^2)
 */
/* TC1:<Timo6>Start */

int TC1_006_QSort(const void *Data1, const void *Data2)
{
    if (*(int32_t *)Data1 < *(int32_t *)Data2)
        return -1;
    if (*(int32_t *)Data1 > *(int32_t *)Data2)
        return +1;
    return 0;
}

void TC1_006_ThreeNumSum(int32_t *List, uint32_t Num, int32_t Target)
{
    /* 先排序 */
    qsort(List, Num, sizeof(int32_t), TC1_006_QSort);
    /* 然后一个个数剔除 */
	for (int32_t Idx = 0; List[Idx] < Target; Idx++) {
        /* 跳过重复数据 */
        if (Idx - 1 >= 0 && List[Idx - 1] == List[Idx])
            continue;
        
        int32_t SubTarget = Target - List[Idx];
        int32_t CurLeft   = Idx + 1;
        int32_t CurRight  = Num - 1;
        
        while (CurLeft < CurRight) {
            if (CurLeft - 1 >= 0 && List[CurLeft - 1] == List[CurLeft])
                CurLeft++;
            if (CurRight + 1 < Num && List[CurRight + 1] == List[CurRight])
                CurRight--;
            
            if (List[CurLeft] + List[CurRight] < SubTarget)
                CurLeft++;
            else
            if (List[CurLeft] + List[CurRight] > SubTarget)
                CurRight--;
            else {
                printf("(%d,%d,%d)\t", List[Idx], List[CurLeft] , List[CurRight]);
                CurLeft++;
                CurRight--;
            }
        }
    }
}

void TC1_006_ThreeNumNear(int32_t *List, uint32_t Num, int32_t Target)
{
    int32_t CurSum = 0, CurLast = 0;
    int32_t NearIdx1 = 0, NearIdx2 = 0, NearIdx3 = 0, NearSum = ~(1 << 31);
    /* 先排序 */
    qsort(List, Num, sizeof(int32_t), TC1_006_QSort);
    /* 然后一个个数剔除 */
	for (int32_t Idx = 0; Idx < Num; Idx++) {
        /* 跳过重复数据 */
        if (Idx - 1 >= 0 && List[Idx - 1] == List[Idx])
            continue;
        
        int32_t CurLeft   = Idx + 1;
        int32_t CurRight  = Num - 1;
        
        while (CurLeft < CurRight) {
            if (CurLeft - 1 >= 0 && List[CurLeft - 1] == List[CurLeft])
                CurLeft++;
            if (CurRight + 1 < Num && List[CurRight + 1] == List[CurRight])
                CurRight--;
            
            CurLast = CurSum;
            CurSum  = List[Idx] + List[CurLeft] + List[CurRight];
            
            if (abs(Target - NearSum) > abs(Target - CurSum)) {
                NearSum = CurSum;
				NearIdx1 = Idx;
				NearIdx2 = CurLeft;
				NearIdx3 = CurRight;
				CurLeft++;
				CurRight--;
            } else {
                if (CurSum < Target)
                    CurLeft++;
                else
                    CurRight--;
            }
        }
    }
    
    printf("(%d, %d, %d)", List[NearIdx1], List[NearIdx2], List[NearIdx3]);
}

void TC1_006_FourNumSum(int32_t *List, uint32_t Num, int32_t Target)
{
    /* 先排序 */
    qsort(List, Num, sizeof(int32_t), TC1_006_QSort);
    /* 然后一个个数剔除 */
	for (int32_t Idx1 = 0; List[Idx1] < Target; Idx1++) {
        /* 跳过重复数据 */
        if (Idx1 - 1 >= 0 && List[Idx1 - 1] == List[Idx1])
            continue;
        /* 然后一个个数剔除 */
        for (int32_t Idx2 = Idx1; List[Idx2] < Target; Idx2++) {
            /* 跳过重复数据 */
            if (Idx2 - 1 >= 0 && List[Idx2 - 1] == List[Idx2])
                continue;
            
            int32_t SubTarget = Target - List[Idx1] - List[Idx2];
            int32_t CurLeft   = Idx2 + 1;
            int32_t CurRight  = Num - 1;
            
            while (CurLeft < CurRight) {
                if (CurLeft - 1 >= 0 && List[CurLeft - 1] == List[CurLeft])
                    CurLeft++;
                if (CurRight + 1 < Num && List[CurRight + 1] == List[CurRight])
                    CurRight--;
                
                if (List[CurLeft] + List[CurRight] < SubTarget)
                    CurLeft++;
                else
                if (List[CurLeft] + List[CurRight] > SubTarget)
                    CurRight--;
                else {
                    printf("(%d,%d,%d,%d)\t", List[Idx1], List[Idx2], List[CurLeft] , List[CurRight]);
                    CurLeft++;
                    CurRight--;
                }
            }
        }
    }
}

void TC1_006_Function(void)
{
    int32_t List[10] = {-7, -3, -5, -6, 0, 2, 2, 3, 4, 6};
    printf("TC1_006:\n\t");
    TC1_006_ThreeNumNear(List, 10, 1);
    printf("\n\t");
    TC1_006_ThreeNumSum(List, 10, 0);
    printf("\n\t");
    TC1_006_FourNumSum(List, 10, 0);
    printf("\n");
}

/* TC1:<Timo6>End */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
