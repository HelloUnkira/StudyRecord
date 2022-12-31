/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include "SingleList.h"
#include "SingleList.c"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* Timor_Misc:<Timo1> 单链表成环与相交(案例参考)
 * 分析:
 *     扩展:如果要删除环中的某一节点
 *          先像单链表一样删除一次,然后再遍历
 *          如果还能找到该节点,则为交叉点,更新该点到下一点即可
 */
/* Timor_Misc:<Timo1>Start */

uint32_t SL_List_Length(SL_List *List)
{
    uint32_t Length = 0;
    SL_List_Traserve(List, Node)
        Length++;
    return Length;
}

bool SL_List_LoopCheck(SL_List *List, SL_Node **Node)
{
    SL_Node *Node1 = SL_List_GetHead(List);
    SL_Node *Node2 = SL_List_GetHead(List);
    
    while (Node1 != NULL && Node2 != NULL &&
           SL_Node_GetNear(Node2) != NULL) {
        if (Node1 == Node2) {
            if (Node != NULL)
               *Node  = Node2;
            return true;
        }
        Node1 = SL_Node_GetNear(Node1);
        Node2 = SL_Node_GetNear(Node2);
        Node2 = SL_Node_GetNear(Node2);
    }
    if (Node != NULL)
       *Node  = NULL;
    return false;
}

bool SL_List_CrossPoint(SL_List *List1, SL_List *List2, SL_Node **Node)
{
    if (Node != NULL)
       *Node  = NULL;
    SL_Node *Node1 = NULL, *Node2 = NULL, *NodeT = NULL;
    bool Loop1 = SL_List_LoopCheck(List1, &Node1);
    bool Loop2 = SL_List_LoopCheck(List2, &Node2);
    /* Condition1:俩个链表均无环,检查是否有归一点,无归一点一定不相交 */
    if (!Loop1 && !Loop2 && SL_List_GetTail(List1) != SL_List_GetTail(List2))
        return false;
    /* Condition2:俩个链表中存在环,俩个链表必需都成环,否则一定不相交 */
    if ((Loop1 || Loop2) && (Node1 == NULL || Node2 == NULL))
        return false;
    /* Condition3: 俩个链表均有环,此环必为同一环,否则一定不相交 */
    if ((Loop1 || Loop2) && (Node1 != Node2)) {
        /* 如果相交一定是同一环,同一环则节点一定能相遇 */
        NodeT = Node1;
        for (Node1  = SL_Node_GetNear(Node1);
             Node1 != NodeT && Node1 != Node2;
             Node1  = SL_Node_GetNear(Node1));
        if (Node1 != Node2)
            return false;
    }
    /* 自此为止,环存在归一点,一定相交,从最开始同步寻找相遇点 */
    Node1 = SL_List_GetHead(List1);
    Node2 = SL_List_GetHead(List2);
    uint32_t Len1 = SL_List_Length(List1);
    uint32_t Len2 = SL_List_Length(List2);
    for (uint32_t Num = 0; Len1 > Len2 && Num < Len1 - Len2; Num++)
        Node1 = SL_Node_GetNear(Node1);
    for (uint32_t Num = 0; Len2 > Len1 && Num < Len2 - Len1; Num++)
        Node2 = SL_Node_GetNear(Node2);
    /* 同步迭代 */
    while (Node1 != NULL && Node2 != NULL) {
        if (Node1 == Node2) {
           *Node   = Node2;
           return true;
        }
        Node1 = SL_Node_GetNear(Node1);
        Node2 = SL_Node_GetNear(Node2);
    }
    /* 理论不存在未找到 */
    return false;
}

/* Timor_Misc:<Timo1>End */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
