#ifndef SGUI_BASIC_GRAPH_H
#define SGUI_BASIC_GRAPH_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 目标:定义简洁通配的基本图元 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 基本图元描述:坐标点 */
typedef struct SimpleGui_Dot {
    SGui_Coord X;    //横坐标
    SGui_Coord Y;    //纵坐标
} SGui_Dot;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 基本图元描述:区域 */
typedef union SimpleGui_Area {
    struct { /* 面向边界 */
        SGui_Coord BL_X;   //左边界X坐标(Boundary Left)
        SGui_Coord BU_Y;   //上边界Y坐标(Boundary Up)
        SGui_Coord BR_X;   //右边界X坐标(Boundary Right)
        SGui_Coord BB_Y;   //下边界Y坐标(Boundary Bottom)
    };
    struct { /* 面向坐标点 */
        SGui_Coord LU_X;    //左上角X坐标(Left,  Up)
        SGui_Coord LU_Y;    //左上角Y坐标(Left,  Up)
        SGui_Coord RB_X;    //右下角X坐标(Right, Down)
        SGui_Coord RB_Y;    //右下角Y坐标(Right, Down)
    };
    struct { /* 面向起点 */
        SGui_Coord Off_X;   //区域X偏移(Offset)
        SGui_Coord Off_Y;   //区域Y偏移(Offset)
        SGui_Coord Off_W;   //区域宽度(Offset Width)
        SGui_Coord Off_H;   //区域高度(Offset Height)
    };
} SGui_Area;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 基本图元描述:像素流类型 */
typedef enum SimpleGui_StreamType {
    SGui_StreamIsDefault = 0,   /* 占位符:不使用,设定初始状态,作为纠错 */
    SGui_StreamIsPure,          /* 纯色:此时只需要Alpha通道即可完成数据融合 */
    SGui_StreamIsGrey,          /* 灰度:此时只需要Color通道即可完成数据融合 */
    SGui_StreamIsNormal,        /* 基础:此时为完整的像素点数据 */
} SGui_StreamType;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 基本图元描述:像素流 */
typedef struct SimpleGui_Stream {
    SGui_StreamType Type;       /* 像素类型 */
    SGui_Alpha      Alpha;      /* 属性扩充:如果是纯色图,额外携带Alpha通道数据 */
    SGui_Color      Color;      /* 属性扩充:如果是灰度图,额外携带Color通道数据 */
} SGui_Stream;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 基本图元描述:画布 */
typedef struct SimpleGui_Canvas {
    SGui_Pixel *Pixel;  /* 像素流(矩阵或点) */
    uint32_t    Length; /* 像素流空间大小 */
    SGui_Area   Offset; /* 像素流偏移区域(面向起点) */
    SGui_Area   Clip;   /* 画布空间剪切域(面向坐标点) */
} SGui_Canvas;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 基本图元内嵌功能: */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 坐标重置 */
static inline void SGui_DotReset(SGui_Dot *Dot)
{
    Dot->X = SGUI_COORD_INVALID;
    Dot->Y = SGUI_COORD_INVALID;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 区域重置 */
static inline void SGui_AreaReset(SGui_Area *Area)
{
    Area->BL_X = SGUI_COORD_INVALID;
    Area->BU_Y = SGUI_COORD_INVALID;
    Area->BR_X = SGUI_COORD_INVALID;
    Area->BB_Y = SGUI_COORD_INVALID;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 区域类型转化(坐标点转为偏移量) */
static inline void SGui_AreaPosToOff(SGui_Area *Region)
{
    SGui_Area Offset = {0};
    SGui_AreaReset(&Offset);
    
    Offset.Off_X = Region->LU_X;
    Offset.Off_Y = Region->LU_Y;
    Offset.Off_W = Region->RB_X - Region->LU_X + 1;
    Offset.Off_H = Region->RB_Y - Region->LU_Y + 1;

    *Region = Offset;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 区域类型转化(偏移量转为坐标点) */
static inline void SGui_AreaOffToPos(SGui_Area *Offset)
{
    SGui_Area Region = {0};
    SGui_AreaReset(&Region);
    
    Region.LU_X = Offset->Off_X;
    Region.LU_Y = Offset->Off_Y;
    Region.RB_X = Offset->Off_W - 1 + Offset->Off_X;
    Region.RB_Y = Offset->Off_H - 1 + Offset->Off_Y;

    *Offset = Region;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 画布数据流缓冲区设置 */
static inline void SGui_CanvasSetPixel(SGui_Canvas *Canvas, SGui_Pixel *Pixel, uint32_t Length)
{
    Canvas->Pixel  = Pixel;
    Canvas->Length = Length;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 画布数据流清空 */
static inline void SGui_CanvasCleanPixel(SGui_Canvas *Canvas)
{
    SGui_Pixel Pixel = {0};
    for (uint32_t Offset = 0; Offset < Canvas->Length; Offset++)
        Canvas->Pixel[Offset] = Pixel;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 画布设置剪切域(面向坐标点) */
static inline bool SGui_CanvasSetClip(SGui_Canvas *Canvas, SGui_Area *Clip)
{
    uint32_t Width  = Clip->RB_X - Clip->LU_X + 1;
    uint32_t Height = Clip->RB_X - Clip->LU_X + 1;
    /* 无效的剪切域不予设置 */
    if (Width * Height > Canvas->Length)
        return false;
    /* 设置区域:剪切域 */
    Canvas->Clip = *Clip;
    /* 设置区域:剪切域到像素矩阵的映射 */
    Canvas->Offset.Off_X = Clip->LU_X;
    Canvas->Offset.Off_Y = Clip->LU_Y;
    Canvas->Offset.Off_W = Width;
    Canvas->Offset.Off_H = Height;
    return true;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 画布获取指定像素点位置(Dot取值范围需要是Canvas->Offset内) */
static inline SGui_Pixel * SGui_CanvasOffset(SGui_Canvas *Canvas, SGui_Dot *Dot)
{
    /* 这是一个参考模板,批量处理像素点时不应该直接调用本接口 */
    /* 绘制空间布局配置:高宽布局:1;宽高布局:2;(通常不会用到后者) */
    return &Canvas->Pixel[Dot->X + Dot->Y * Canvas->Offset.Off_W];
    return &Canvas->Pixel[Dot->Y + Dot->X * Canvas->Offset.Off_H];
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
