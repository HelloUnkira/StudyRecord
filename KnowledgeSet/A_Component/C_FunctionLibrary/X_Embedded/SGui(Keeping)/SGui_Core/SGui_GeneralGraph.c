#include "SGui_Core.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* �����߶��ཻ�ж� */
bool SGui_LineIsAnd(SGui_Dot *S1, SGui_Dot *E1, SGui_Dot *S2, SGui_Dot *E2)
{
    /* ���1:�����ų��� */
    if (SGui_Min(S1->X, E1->X) > SGui_Max(S2->X, E2->X) ||
        SGui_Min(S2->X, E2->X) > SGui_Max(S1->X, E1->X) ||
        SGui_Min(S1->Y, E1->Y) > SGui_Max(S2->Y, E2->Y) ||
        SGui_Min(S2->Y, E2->Y) > SGui_Max(S1->Y, E1->Y))
        return false;
    /* ���2:������� */
    if (((S1->X - S2->X) * (E2->Y - S2->Y) - (S1->Y - S2->Y) * (E2->X - S2->X)) *
        ((E1->X - S2->X) * (E2->Y - S2->Y) - (E1->Y - S2->Y) * (E2->X - S2->X)) > 0 ||
        ((S2->X - S1->X) * (E1->Y - S1->Y) - (S2->Y - S1->Y) * (E1->X - S1->X)) *
        ((E2->X - S1->X) * (E1->Y - S1->Y) - (E2->Y - S1->Y) * (E1->X - S1->X)) > 0)
        return false;
    
    return true;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* ��������ת��(�����תΪƫ����) */
void SGui_AreaPosToOff(SGui_Area *Region)
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
/* ��������ת��(ƫ����תΪ�����) */
void SGui_AreaOffToPos(SGui_Area *Offset)
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
/* ����ת������(������תΪ�����) */
void SGui_AreaFromDot2(SGui_Area *Region, SGui_Dot *Dot1, SGui_Dot *Dot2)
{
    Region->LU_X = SGui_Min(Dot1->X, Dot2->X);
    Region->LU_Y = SGui_Min(Dot1->Y, Dot2->Y);
    Region->RB_X = SGui_Max(Dot1->X, Dot2->X);
    Region->RB_Y = SGui_Max(Dot1->Y, Dot2->Y);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* ������������� */
void SGui_CanvasCleanPixel(SGui_Canvas *Canvas)
{
    SGui_Pixel Pixel = {0};
    for (uint32_t Offset = 0; Offset < Canvas->Length; Offset++)
        Canvas->Pixel[Offset] = Pixel;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* �������ü�����(���������) */
bool SGui_CanvasSetClip(SGui_Canvas *Canvas, SGui_Area *Clip)
{
    uint32_t Width  = Clip->RB_X - Clip->LU_X + 1;
    uint32_t Height = Clip->RB_X - Clip->LU_X + 1;
    /* ��Ч�ļ����������� */
    if (Width * Height > Canvas->Length)
        return false;
    /* ��������:������ */
    Canvas->Clip = *Clip;
    /* ��������:���������ؾ����ӳ�� */
    Canvas->Offset.Off_X = Clip->LU_X;
    Canvas->Offset.Off_Y = Clip->LU_Y;
    Canvas->Offset.Off_W = Width;
    Canvas->Offset.Off_H = Height;
    return true;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* ����ƫ���������ת�� */
bool SGui_CanvasPosToOff(SGui_Canvas *Canvas, SGui_Area *Region)
{
    SGui_Area Offset = {0};
    SGui_ClipRegionAnd(&Offset, &Canvas->Clip, Region);
    /* ���û�����ڵ�ǰ��������,�����м��� */
    if (SGui_ClipRegionIsVaild(&Offset) == false)
        return false;
    SGui_AreaPosToOff(&Offset);
    /* ��ƫ����ƫ�Ƶ����� */
    Offset.Off_Y -= Canvas->Offset.Off_Y;
    Offset.Off_X -= Canvas->Offset.Off_X;
    *Region = Offset;
    return true;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/