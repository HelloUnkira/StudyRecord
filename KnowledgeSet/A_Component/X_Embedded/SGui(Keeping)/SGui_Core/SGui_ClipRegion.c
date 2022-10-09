#include "SGui_Kernel.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 实现目标:
 * 1.设计简要的剪切域运算
 * 剪切域统一相对父窗口的偏移做计算
 * 2.设计简要的剪切域管理阵列
 * 剪切域管理阵列服务于多剪切域控件
 * 剪切域统一相对父窗口的偏移做计算
 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#if SGUI_CLIP_REGION1
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 俩个剪切域交集 */
SGui_Area SGui_ClipRectAnd(SGui_Area Clip1, SGui_Area Clip2)
{
    SGui_Area Clip = {0};
    SGui_AreaReset(&Clip);
    
    Clip.LU.X = SGUI_MAX(Clip1.LU.X, Clip2.LU.X);
    Clip.LU.Y = SGUI_MAX(Clip1.LU.Y, Clip2.LU.Y);
    Clip.RB.X = SGUI_MIN(Clip1.RB.X, Clip2.RB.X);
    Clip.RB.Y = SGUI_MIN(Clip1.RB.Y, Clip2.RB.Y);

    return Clip;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 俩个剪切域并集 */
SGui_Area SGui_ClipRectOr(SGui_Area Clip1, SGui_Area Clip2)
{
    SGui_Area Clip = {0};
    SGui_AreaReset(&Clip);

    Clip.LU.X = SGUI_MIN(Clip1.LU.X, Clip2.LU.X);
    Clip.LU.Y = SGUI_MIN(Clip1.LU.Y, Clip2.LU.Y);
    Clip.RB.X = SGUI_MAX(Clip1.RB.X, Clip2.RB.X);
    Clip.RB.Y = SGUI_MAX(Clip1.RB.Y, Clip2.RB.Y);

    return Clip;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 俩个剪切域联合 */
SGui_Area SGui_ClipRectUnion(SGui_Area Clip1, SGui_Area Clip2)
{
    SGui_Area Clip = {0};
    SGui_AreaReset(&Clip);

    /* 剪切域求和要求:剪切域互相平行或垂直 */

    /* 剪切域垂直: */
    if (Clip1.LU.X == Clip2.LU.X && Clip1.RB.X == Clip2.RB.X) {
        /* 场景1:非相交 */
        if (Clip1.RB.Y < Clip2.LU.Y || Clip2.RB.Y < Clip1.LU.Y)
            return Clip;
        /* 场景2:相交 */
        Clip.LU.X = (Clip1.LU.X, Clip2.LU.X);
        Clip.RB.X = (Clip1.RB.X, Clip2.RB.X);
        Clip.LU.Y = SGUI_MIN(Clip1.LU.Y, Clip2.LU.Y);
        Clip.RB.Y = SGUI_MAX(Clip1.RB.Y, Clip2.RB.Y);
        return Clip;
    }
    /* 剪切域平行: */
    if (Clip1.LU.Y == Clip2.LU.Y && Clip1.RB.Y == Clip2.RB.Y) {
        /* 场景1:非相交 */
        if (Clip1.RB.X < Clip2.LU.X || Clip2.RB.X < Clip1.LU.X)
            return Clip;
        /* 场景2:相交 */
        Clip.LU.Y = (Clip1.LU.Y, Clip2.LU.Y);
        Clip.RB.Y = (Clip1.RB.Y, Clip2.RB.Y);
        Clip.LU.X = SGUI_MIN(Clip1.LU.X, Clip2.LU.X);
        Clip.RB.X = SGUI_MAX(Clip1.RB.X, Clip2.RB.X);
        return Clip;
    }
    /*  */
    return Clip;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 俩个剪切域求差(要求:必须存在交集) */
uint32_t SGui_ClipRectSub(SGui_Area *Result, SGui_Area Master, SGui_Area Slave)
{
    /* 最高生成4个剪切域 */
    /* 裁剪主剪切域(四种情况) */
    uint32_t Count = 0;

    /* 裁剪主域多余上部 */
    if (Master.LU.Y < Slave.LU.Y) {
        Result[Count].LU.X = Master.LU.X;
        Result[Count].RB.X = Master.RB.X;
        Result[Count].LU.Y = Master.LU.Y;
        Result[Count].RB.Y = Slave.LU.Y;
        
        Master.LU.Y = Slave.LU.Y;
        Count++;
    }
    /* 裁剪主域多余下部 */
    if (Master.RB.Y > Slave.RB.Y) {
        Result[Count].LU.X = Master.LU.X;
        Result[Count].RB.X = Master.RB.X;
        Result[Count].LU.Y = Slave.RB.Y;
        Result[Count].RB.Y = Master.RB.Y;

        Master.RB.Y = Slave.RB.Y;
        Count++;
    }
    /* 裁剪主域多余左部 */
    if (Master.LU.X < Slave.LU.X) {
        Result[Count].LU.Y = Master.LU.Y;
        Result[Count].RB.Y = Master.RB.Y;
        Result[Count].LU.X = Master.LU.X;
        Result[Count].RB.X = Slave.LU.X;
        
        //Master.LU.X = Slave.LU.X;
        Count++;
    }
    /* 裁剪主域多余右部 */
    if (Master.RB.X > Slave.RB.X) {
        Result[Count].LU.Y = Master.LU.Y;
        Result[Count].RB.Y = Master.RB.Y;
        Result[Count].LU.X = Slave.RB.X;
        Result[Count].RB.X = Master.RB.X;

        //Master.RB.X = Slave.RB.X;
        Count++;
    }

    return Count;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 剪切域存在检查 */
bool SGui_ClipRectIsVaild(SGui_Area Clip)
{
    bool Result = true;
    /* 本集与补集运算 */
    if (Clip.LU.X == SGUI_COORD_INVALID || Clip.LU.Y == SGUI_COORD_INVALID ||
        Clip.RB.X == SGUI_COORD_INVALID || Clip.RB.Y == SGUI_COORD_INVALID)
        Result = false;
    
    if (Clip.LU.X >= Clip.RB.X  || Clip.LU.Y >= Clip.RB.Y)
        Result = false;

    return Result;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 剪切域包含关系检查1:(域包含) */
bool SGui_ClipRectCheck1(SGui_Area Child, SGui_Area Parent)
{
    bool Result = false;
    /* 本集与补集运算 */
    if (Child.LU.X >= Parent.LU.X && Child.RB.X <= Parent.RB.X &&
        Child.LU.Y >= Parent.LU.Y && Child.RB.Y <= Parent.RB.Y)
        Result = true;

    return Result;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 剪切域包含关系检查2:(点包含) */
bool SGui_ClipRectCheck2(SGui_Dot Dot, SGui_Area Clip)
{
    bool Result = false;
    /* 本集与补集运算 */
    if (Dot.X >= Clip.LU.X && Dot.X <= Clip.RB.X &&
        Dot.Y >= Clip.LU.Y && Dot.Y <= Clip.RB.Y)
        Result = true;

    return Result;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#if SGUI_CLIP_REGION2
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 资源单元剪切域集合扩张缩减因子 */
/* 数量越大,额外维护空间越小 */
/* 数量越小,内存需求弹性越高 */
#define SGUI_CLIP_REGION_FACTOR    5
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 资源管理单元 */
typedef struct SimpleGui_ClipRectsUnit {
    uint32_t    Index;  //当前单元编号
    uint32_t    Number; //可使用剪切域数量
    uint32_t    Length; //总剪切域数量
    SGui_Area   Master; //剪切域主域
    SGui_Area  *Slave;  //剪切域资源集合
} SGui_ClipRectsUnit;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static SGui_SList List = {0};
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 简化1: 剪切域阵列递归添加剪切域 */
static void SGui_ClipRectsUnitRecurveMerge(SGui_ClipRectsUnit *Unit, SGui_Area Clip)
{
    /* 如果剪切域可以合并 */
    /* 合并后的剪切域要继续检查 */
    /* 如果走到末尾,则无法继续合并 */
    int32_t Recover = Unit->Length;
    /* 合并剪切域 */
    for (int32_t Index = 0; Index < Unit->Length; Index++) {
        /* 出现一个有效的剪切域 */
        if (SGui_ClipRectIsVaild((Unit->Slave)[Index]) == true) {
            /* 创建一个临时剪切域 */
            SGui_Area Temp = {0};
            SGui_AreaReset(&Temp);
            /* 尝试联合剪切域 */
            Temp = SGui_ClipRectUnion(Clip, (Unit->Slave)[Index]);
            /* 联合剪切域成功时 */
            if (SGui_ClipRectIsVaild(Temp) == true) {
                /* 移除原生剪切域 */
                SGui_AreaReset(&(Unit->Slave)[Index]);
                /* 扣除记录 */
                Unit->Number++;
                /* 获得合并后剪切域 */
                Clip = Temp;
                /* 这个位点的剪切域已经失效,考虑纪录索引 */
                Recover = Recover < Index ? Recover : Index;
                /* 将索引拉到最开始,重制整个流程 */
                Index = -1;
            }
            continue;
        }
        /* 这个位点的剪切域已经失效,考虑纪录索引 */
        Recover = Recover < Index ? Recover : Index;
    }
    /* 2.将最终剪切域载入到剪切域阵列单元中 */
    if (Recover == Unit->Length);
    if (Recover != Unit->Length) {
        /* 载入合并后的剪切域(如果可以合并) */
        (Unit->Slave)[Recover] = Clip;
        /* 添加记录 */
        Unit->Number--;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 简化2: 合并生成剪切域阵列 */
static void SGui_ClipRectsUnitMerge(SGui_ClipRectsUnit *Unit1, SGui_ClipRectsUnit *Unit2)
{
    /* 1.生成剪切域阵列单元 */
    SGui_ClipRectsUnit *Unit = SGUI_ALLOC(sizeof(SGui_ClipRectsUnit));
    /* 2.计算合并空间 */
    Unit->Number = Unit1->Number + Unit2->Number;
    Unit->Length = Unit1->Length + Unit2->Length;
    /* 如果合并空间存在大量空闲,则取就近值(此循环应稳定执行0~1次) */
    while (Unit->Number >= SGUI_CLIP_REGION_FACTOR) {
           Unit->Number -= SGUI_CLIP_REGION_FACTOR;
           Unit->Length -= SGUI_CLIP_REGION_FACTOR;
    }

    /* 3.生成合并空间 */
    Unit->Slave = SGUI_ALLOC(sizeof(SGui_Area) * (Unit->Length));
    /*  */
    uint32_t Count = 0;
    /* 4.拷贝剪切域资源 */
    for (uint32_t Index = 0; Index < Unit1->Length; Index++)
        if (SGui_ClipRectIsVaild((Unit1->Slave)[Index]) == true)
            (Unit->Slave)[Count++] = (Unit1->Slave)[Index];
    for (uint32_t Index = 0; Index < Unit2->Length; Index++)
        if (SGui_ClipRectIsVaild((Unit2->Slave)[Index]) == true)
            (Unit->Slave)[Count++] = (Unit2->Slave)[Index];
    /* 5.检查失败:销毁剪切域阵列单元,返回空 */
    if (Count + Unit->Number != Unit->Length) {
        SGUI_FREE(Unit->Slave);
    }
    /* 5.检查成功:返回剪切域阵列单元 */
    if (Count + Unit->Number == Unit->Length) {
        /* 先释放原生剪切域 */
        SGUI_FREE(Unit1->Slave);
        /* 新剪切域覆盖原生剪切域 */
        Unit1->Number = Unit->Number;
        Unit1->Length = Unit->Length;
        Unit1->Slave  = Unit->Slave;
    }
    SGUI_FREE(Unit);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 获取一个剪切域集合 */
uint32_t SGui_ClipRectsTake(void)
{
    SGui_Area Master = {0};
    SGui_AreaReset(&Master);
    /* 1.生成一个剪切域阵列单元 */
    SGui_ClipRectsUnit *Unit = SGUI_ALLOC(sizeof(SGui_ClipRectsUnit));
    /* 2.初始化生成空间 */
    Unit->Number = SGUI_CLIP_REGION_FACTOR;
    Unit->Length = SGUI_CLIP_REGION_FACTOR;
    Unit->Master = Master;
    Unit->Slave  = SGUI_ALLOC(sizeof(SGui_Area) * (Unit->Length));
    /*  */
    uint32_t Index = 0;
    /* 3.重置全部剪切域 */
    for (Index = 0; Index < Unit->Length; Index++)
        SGui_AreaReset(&(Unit->Slave)[Index]);
    /* 4.剪切域阵列单元加入到阵列中 */
    SGui_SNodeUnitAdd(&List, Unit);
    /* 5.返回剪切域阵列单元标号 */
    Index = Unit->Index;
    return Index;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 释放一个剪切域集合 */
void SGui_ClipRectsGive(uint32_t Clips)
{
    /* 1.遍历链表,确认是哪一个管理单元 */
    SGui_ClipRectsUnit *Unit = SGui_SNodeUnitSearch(&List, Clips);
    /* 2.剪切域阵列单元移除出阵列中 */
    if (Unit == NULL);
    if (Unit != NULL) {
        /* 释放一个空闲单元 */
        /* 生成与释放顺序相反 */
        /* 虽然Unit会变成野指针,但我们不会拿它干坏事 */
        SGUI_FREE(Unit->Slave);
        SGUI_FREE(Unit);
        /* 将空闲单元移除出以最小化索引链表 */
        SGui_SNodeUnitRemove(&List, Unit);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 为剪切域集合设置主域 */
void SGui_ClipRectsMasterSet(uint32_t Clips, SGui_Area Clip)
{
    /* 1.遍历链表,确认是哪一个管理单元 */
    SGui_ClipRectsUnit *Unit = SGui_SNodeUnitSearch(&List, Clips);
    /* 2.剪切域阵列单元设置主域 */
    if (Unit == NULL);
    if (Unit != NULL) {
        /* 设置主域 */
        Unit->Master = Clip;

        /* 重置剪切域空间 */
        SGUI_FREE(Unit->Slave);

        Unit->Number = SGUI_CLIP_REGION_FACTOR;
        Unit->Length = SGUI_CLIP_REGION_FACTOR;
        Unit->Slave  = SGUI_ALLOC(sizeof(SGui_Area) * (Unit->Length));
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 为剪切域集合获取主域 */
SGui_Area SGui_ClipRectsMasterGet(uint32_t Clips)
{
    SGui_Area Clip = {0};
    SGui_AreaReset(&Clip);
    /* 1.遍历链表,确认是哪一个管理单元 */
    SGui_ClipRectsUnit *Unit = SGui_SNodeUnitSearch(&List, Clips);
    /* 2.剪切域阵列单元设置主域 */
    if (Unit == NULL);
    if (Unit != NULL) {
        /* 获取主域 */
        Clip = Unit->Master;
    }
    return Clip;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 剪切域集合添加剪切域 */
void SGui_ClipRectsSlaveAdd(uint32_t Clips, SGui_Area Clip)
{
    /* 1.遍历链表,确认是哪一个管理单元 */
    SGui_ClipRectsUnit *Unit = SGui_SNodeUnitSearch(&List, Clips);
    /* 2.剪切域阵列单元添加剪切域 */
    if (Unit == NULL);
    if (Unit != NULL) {
        /* 先确认有效剪切域 */
        Clip = SGui_ClipRectAnd(Unit->Master, Clip);
        /* 剪切域递归迭代到剪切域阵列中 */
        if (SGui_ClipRectIsVaild(Clip) == true)
            SGui_ClipRectsUnitRecurveMerge(Unit, Clip);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 剪切域集合移除剪切域 */
void SGui_ClipRectsSlaveRemove(uint32_t Clips, SGui_Area Clip)
{
    uint32_t Count4    =  0;
    SGui_Area Rects4[4] = {0};
    int32_t Index = 0, Index1 = 0;
    /* 1.遍历链表,确认是哪一个管理单元 */
    SGui_ClipRectsUnit *Unit  = SGui_SNodeUnitSearch(&List, Clips);
    SGui_ClipRectsUnit *Unit1 = NULL;
    /* 2.先确认剪切域的有效性 */
    if (Unit == NULL);
    if (Unit != NULL) {
        /* 计算剪切域是否与主域相交 */
        Clip = SGui_ClipRectAnd(Unit->Master, Clip);
        /* 如果剪切域与主域不相交,无法移除 */
        if (SGui_ClipRectIsVaild(Clip) == false)
            return;
    }
    /* 3.生成一个临时的剪切域集合,进行过程记录 */
    if (Unit != NULL) {
        Unit1         = SGUI_ALLOC(sizeof(SGui_ClipRectsUnit));
        Unit1->Number = SGUI_CLIP_REGION_FACTOR;
        Unit1->Length = SGUI_CLIP_REGION_FACTOR;
        Unit1->Slave  = SGUI_ALLOC(sizeof(SGui_Area) * (Unit1->Length));
        /*  */
        for (Index = 0; Index < Unit1->Length; Index++)
            SGui_AreaReset(&(Unit1->Slave)[Index]);
    }
    /* 4.剪切域阵列单元移除剪切域 */
    if (Unit != NULL) {
        /* 迭代所有剪切域 */
        for (Index = 0; Index < Unit->Length; Index++) {
            /* 1.跳过一个无效的剪切域 */
            if (SGui_ClipRectIsVaild((Unit->Slave)[Index]) == false)
                continue;
            /* 2.计算与有效剪切域的交集 */
            SGui_Area Rect_sub = SGui_ClipRectAnd(Clip, (Unit->Slave)[Index]);
            /* 3.跳过不相交的有效剪切域 */
            if (SGui_ClipRectIsVaild(Rect_sub) == false)
                continue;
            /* 4.计算剪切域差集 */
            Count4 = SGui_ClipRectSub(Rects4, (Unit->Slave)[Index], Rect_sub);
            /* 5.将剪切域差集保留到本地剪切域集合中 */
            /* 5.1.空间不够记录时,扩充记录 */
            while (Unit1->Number < Count4) {
                /* 创建扩充记录 */
                uint32_t Length = Unit1->Length + SGUI_CLIP_REGION_FACTOR;
                SGui_Area  *Temp = SGUI_ALLOC(sizeof(SGui_Area) * Length);
                /* 拷贝原生记录 */
                for (Index1 = 0; Index1 < Unit1->Length; Index1++)
                    Temp[Index1] = (Unit1->Slave)[Index1];
                /* 重置新增空间 */
                for (Index1 = Index1; Index1 < Length; Index1++)
                    SGui_AreaReset(&(Unit1->Slave)[Index1]);

                SGUI_FREE(Unit1->Slave);

                Unit1->Number += SGUI_CLIP_REGION_FACTOR;
                Unit1->Length += SGUI_CLIP_REGION_FACTOR;
                Unit1->Slave   = Temp;
            }
            /* 5.2.空间足够记录时,直接记录 */
            if (Unit1->Number >= Count4)
                for (Index1 = 0; Index1 < Count4; Index1++)
                    SGui_ClipRectsUnitRecurveMerge(Unit1, Rects4[Index1]);
            /* 6.移除原生剪切域 */
            SGui_AreaReset(&(Unit->Slave)[Index]);
            Unit->Number++;
        }
    }
    /* 5.新生成剪切域集合归并到剪切域集中 */
    if (Unit != NULL) {
        SGui_ClipRectsUnitMerge(Unit, Unit1);
        /* 销毁临时空间 */
        SGUI_FREE(Unit1->Slave);
        SGUI_FREE(Unit1);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 获取剪切域集合长度 */
void SGui_ClipRectsSlaveLengthGet(uint32_t Clips, uint32_t *Length)
{
    uint32_t Index = 0;
    /* 1.遍历链表,确认是哪一个管理单元 */
    SGui_ClipRectsUnit *Unit  = SGui_SNodeUnitSearch(&List, Clips);
    /* 2.确认剪切域的有效性 */
    if (Unit == NULL)
        *Length = 0;
    if (Unit != NULL)
        *Length = Unit->Length;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 获取剪切域集合 */
void SGui_ClipRectsSlaveSetGet(uint32_t Clips, SGui_Area *Slave)
{
    uint32_t Index = 0;
    /* 1.遍历链表,确认是哪一个管理单元 */
    SGui_ClipRectsUnit *Unit  = SGui_SNodeUnitSearch(&List, Clips);
    /* 2.确认剪切域的有效性 */
    if (Unit != NULL)
        for (Index = 0; Index < Unit->Length; Index++)
            Slave[Index] = (Unit->Slave)[Index];
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#if SGUI_INTERNAL_TEST
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define SGuiClipRegionTest(Clip)                 \
    SGUI_LOGMESSAGE("Left:%d\n",   (Clip).LU.X); \
    SGUI_LOGMESSAGE("Up:%d\n",     (Clip).LU.Y); \
    SGUI_LOGMESSAGE("Right:%d\n",  (Clip).RB.X); \
    SGUI_LOGMESSAGE("Bottom:%d\n", (Clip).RB.Y); \
    SGUI_LOGMESSAGE("\n");
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#if SGUI_CLIP_REGION1
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_ClipRegion1_Test(void)
{
    /* 剪切域基本运算 */
    SGui_Area Clip0 = {0};
    SGui_AreaReset(&Clip0);
    SGui_Area Clip1 = {0};
    SGui_AreaReset(&Clip1);
    SGui_Area Clip2 = {0};
    SGui_AreaReset(&Clip2);
    SGui_Area Clip3 = {0};
    SGui_AreaReset(&Clip3);

    SGUI_AreaSet(&Clip1, 100, 100, 300, 300);
    SGUI_AreaSet(&Clip2, 150, 150, 250, 250);
    SGUI_AreaSet(&Clip3, 150, 100, 350, 300);

    SGuiClipRegionTest(Clip1);
    SGuiClipRegionTest(Clip2);
    SGuiClipRegionTest(Clip3);

    /* 俩个剪切域交集 */
    SGUI_LOGMESSAGE("Rect1 & Rect2:\n");
    Clip0 = SGui_ClipRectAnd(Clip1, Clip2);
    SGuiClipRegionTest(Clip0);

    /* 俩个剪切域并集 */
    SGUI_LOGMESSAGE("Rect1 | Rect2:\n");
    Clip0 = SGui_ClipRectOr(Clip1, Clip2);
    SGuiClipRegionTest(Clip0);

    /* 俩个剪切域联合 */
    SGUI_LOGMESSAGE("Rect1 U Rect2:\n");
    Clip0 = SGui_ClipRectUnion(Clip1, Clip3);
    SGuiClipRegionTest(Clip0);

    /* 剪切域包含关系检查1:(域包含) */
    SGUI_LOGMESSAGE("Rect1 have Rect2:%d\n", SGui_ClipRectCheck1(Clip2, Clip1));

    /* 俩个剪切域求差(要求:必须存在交集) */
    SGui_Area Result[4] = {0};
    int Index = 0;
    int Count = SGui_ClipRectSub(Result, Clip1, Clip2);
    
    SGUI_LOGMESSAGE("Rect1 - Rect2:\n");
    for (int Index = 0; Index < Count; Index++) {
        SGuiClipRegionTest(Result[Index]);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#if SGUI_CLIP_REGION2
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_ClipRegion2_Test(void)
{
    SGui_Area *Slave = NULL;
    uint32_t Length = 0;
    uint32_t Index  = 0;
    
    SGui_Area Master = {0};
    SGui_AreaReset(&Master);
    SGui_Area Slave1 = {0};
    SGui_AreaReset(&Slave1);
    SGui_Area Slave2 = {0};
    SGui_AreaReset(&Slave2);
    
    SGUI_AreaSet(&Master, 0, 0, 400, 400);
    SGUI_AreaSet(&Slave1, 150, 150, 250, 250);
    SGUI_AreaSet(&Slave2, 50, 50 , 100, 100);
    /* 1.获得一个剪切域集合 */
    uint32_t Rects = SGui_ClipRectsTake();
    /* 2.为剪切域集合设置主域 */
    SGui_ClipRectsMasterSet(Rects, Master);
    /* 3.剪切域集合添加剪切域 */
    SGui_ClipRectsSlaveAdd(Rects, Master);
    /* 4.剪切域集合移除剪切域 */
    SGui_ClipRectsSlaveRemove(Rects, Slave1);
    /* 获取剪切域集合 */
    SGUI_LOGMESSAGE("Current Rects:\n");
    SGui_ClipRectsSlaveLengthGet(Rects, &Length);
    Slave = SGUI_ALLOC(sizeof(SGui_Area) * Length);
    SGui_ClipRectsSlaveSetGet(Rects, Slave);
    if (Slave != NULL)
        for (Index = 0; Index < Length; Index++)
            if (SGui_ClipRectIsVaild(Slave[Index]) == true) {
                SGUI_LOGMESSAGE("Index:%d\n", Index);
                SGuiClipRegionTest(Slave[Index]);
            }
    SGUI_FREE(Slave);
    /* 4.剪切域集合移除剪切域 */
    SGui_ClipRectsSlaveRemove(Rects, Slave2);
    /* 获取剪切域集合 */
    SGUI_LOGMESSAGE("Current Rects:\n");
    SGui_ClipRectsSlaveLengthGet(Rects, &Length);
    Slave = SGUI_ALLOC(sizeof(SGui_Area) * Length);
    SGui_ClipRectsSlaveSetGet(Rects, Slave);
    if (Slave != NULL)
        for (Index = 0; Index < Length; Index++)
            if (SGui_ClipRectIsVaild(Slave[Index]) == true) {
                SGUI_LOGMESSAGE("Index:%d\n", Index);
                SGuiClipRegionTest(Slave[Index]);
            }
    SGUI_FREE(Slave);
    /* 3.剪切域集合添加剪切域 */
    SGui_ClipRectsSlaveAdd(Rects, Slave1);
    /* 获取剪切域集合 */
    SGUI_LOGMESSAGE("Current Rects:\n");
    SGui_ClipRectsSlaveLengthGet(Rects, &Length);
    Slave = SGUI_ALLOC(sizeof(SGui_Area) * Length);
    SGui_ClipRectsSlaveSetGet(Rects, Slave);
    if (Slave != NULL)
        for (Index = 0; Index < Length; Index++)
            if (SGui_ClipRectIsVaild(Slave[Index]) == true) {
                SGUI_LOGMESSAGE("Index:%d\n", Index);
                SGuiClipRegionTest(Slave[Index]);
            }
    SGUI_FREE(Slave);
    /* 3.剪切域集合添加剪切域 */
    SGui_ClipRectsSlaveAdd(Rects, Slave2);
    /* 获取剪切域集合 */
    SGUI_LOGMESSAGE("Current Rects:\n");
    SGui_ClipRectsSlaveLengthGet(Rects, &Length);
    Slave = SGUI_ALLOC(sizeof(SGui_Area) * Length);
    SGui_ClipRectsSlaveSetGet(Rects, Slave);
    if (Slave != NULL)
        for (Index = 0; Index < Length; Index++)
            if (SGui_ClipRectIsVaild(Slave[Index]) == true) {
                SGUI_LOGMESSAGE("Index:%d\n", Index);
                SGuiClipRegionTest(Slave[Index]);
            }
    SGUI_FREE(Slave);
    /* 释放一个剪切域集合 */
    SGui_ClipRectsGive(Rects);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
