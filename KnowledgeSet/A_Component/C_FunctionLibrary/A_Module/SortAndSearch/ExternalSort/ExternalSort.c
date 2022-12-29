/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include "ExternalSort.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 检查数据有序情况,返回有序数据数量 */
static long CheckFileData(FILE *File, int Length, SortCompare Compare, char Split)
{
    long  Number = 0;
    char  SplitChar = 0;
    char *Data1 = malloc(Length);
    char *Data2 = malloc(Length);
    fseek(File, 0, SEEK_SET);
    /* Ready */
    memset(Data1, 0, Length);
    memset(Data2, 0, Length);
    /* Loop */
    do {
        if (fread(Data2, Length, 1, File) != 1)
            break;
        memcpy(Data1, Data2, Length);
        if (Compare(Data1, Data2) > 0) {
            printf("CheckFileData:Sort Fail:Error Sequence\r\n");
            break;
        }
        Number++;
        if (fread(&SplitChar, 1, 1, File) != 1)
            break;
        if (SplitChar != Split) {
            printf("CheckFileData:Sort Fail:Error Split\r\n");
            break;
        }
    } while (!feof(File));
    return Number;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 后缀名匹配检查,以.为后缀名起始,成功返回0,失败返回-1 */
static int CheckFileExtension(char *Name1, char *Name2)
{
    /* 迭代到第一个.字符 */
    char *String1 = strchr(Name1, '.');
    char *String2 = strchr(Name2, '.');
    if (String1 == NULL || String2 == NULL)
        return -1;
    /* 比较后缀 */
    if (strcmp(String1, String2) != 0)
        return -1;
    return 0;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 内排序一个小文件 */
static void QuickSortLittleFile(FILE *File, int Length, SortCompare Compare, char Split)
{
    long  Number = 0;
    char  SplitChar = 0;
    fseek(File, 0, SEEK_SET);
    fseek(File, 0, SEEK_END);
    long Size = ftell(File);
    /* 生成文件大小 */
    char *Buffer = malloc(Size);
    fseek(File, 0, SEEK_SET);
    for (Number = 0; !feof(File); Number++) {
        if (fread(Buffer + Number * Length, Length, 1, File) != 1)
            break;
        if (fread(&SplitChar, 1, 1, File) != 1)
            break;
        if (SplitChar != Split)
            break;
    }
    qsort(Buffer, Number, Length, Compare);
    fseek(File, 0, SEEK_SET);
    for (long Index = 0; Index < Number; Index++) {
        if (fwrite(Buffer + Index * Length, Length, 1, File) != 1)
            break;
        if (fwrite(&Split, 1, 1, File) != 1)
            break;
    }
    free(Buffer);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 败者树调整 */
static void FailTreeAdjust(char *Leave, long *Node, long N, long I, int Length, SortCompare Compare)
{
    long Parent = (N + I - 1) / 2;
    while (Parent > 0) {
        if (Compare(Leave + Length * I, Leave + Length * Node[Parent]) > 0) {
            long Temp = Node[Parent];
            Node[Parent] = I;
            I = Temp;
        }
        Parent /= 2;
    }
    Node[0] = I;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 败者树重置 */
static void FailTreeReset(char *Leave, long *Node, long N, int Length, SortCompare Compare)
{
    for (long I = N; I > 0; I--)
        FailTreeAdjust(Leave, Node, N, I, Length, Compare);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 多路已排序文件合并 */
static void MulticannelMergeFile(FILE *FileOut, FILE **FileList, int Number,
                                 char *DataMin, char *DataMax,
                                 int Length, SortCompare Compare, char Split)
{
    char  SplitChar = 0;
    char *Data  = malloc(Length + 1);
    char *Leave = malloc(Length * (Number + 1));
    long *Node  = malloc(sizeof(long) * (Number));
    memcpy(Leave + Length * 0, DataMin, Length);
    for (int Index = 1; Index < Number + 1; Index++) {
        fseek(FileList[Index - 1], 0, SEEK_SET);
        if (fread(Data, Length + 1, 1, FileList[Index - 1]) != 1) {
            memcpy(Data, DataMax, Length);
            Data[Length] = Split;
        }
        if (Data[Length] != Split)
            break;
        memcpy(Leave + Length * Index, Data, Length);
        Node[Index - 1] = 0;
    }
    FailTreeReset(Leave, Node, Number, Length, Compare);
    fseek(FileOut, 0, SEEK_SET);
    while(memcmp(Leave + Length * Node[0], DataMax, Length) != 0) {
        long Index = Node[0];
        memcpy(Data, Leave + Length * Index, Length);
        Data[Length] = Split;
        fwrite(Data, Length + 1, 1, FileOut);
        if (fread(Data, Length + 1, 1, FileList[Index - 1]) != 1) {
            memcpy(Data, DataMax, Length);
            Data[Length] = Split;
        }
        if (Data[Length] != Split)
            break;
        memcpy(Leave + Length * Index, Data, Length);
        FailTreeAdjust(Leave, Node, Number, Index, Length, Compare);
    }
    free(Node);
    free(Leave);
    free(Data);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void DevideAndConquerFile(FILE *File, long ZoneMax,
                                 char *DataMin, char *DataMax,
                                 int Length, SortCompare Compare, char Split)
{
    char SplitChar = 0;
    char FileName[1024] = {0};
    char *Data = malloc(Length + 1);
    fseek(File, 0, SEEK_SET);
    fseek(File, 0, SEEK_END);
    long Size = ftell(File);
    fseek(File, 0, SEEK_SET);
    long FileNumber = 0;
    //生成小文件并排好序
    while (!feof(File)) {
        sprintf(FileName, "TempFile_%d", FileNumber++);
        FILE *TempFile = fopen(FileName, "wb+");
        long TempFileLength = 0; 
        while (!feof(File)) {
            if (fread(Data, Length + 1, 1, File) != 1)
                break;
            if (Data[Length] != Split)
                break;
            fwrite(Data, Length + 1, 1, TempFile);
            if ((TempFileLength += Length + 1) >= ZoneMax)
                break;
        }
        QuickSortLittleFile(TempFile, Length, Compare, Split);
        fclose(TempFile);
    }
    FILE **FileList = malloc(sizeof(FILE *) * FileNumber);
    for (long Index = 0; Index < FileNumber; Index++) {
        sprintf(FileName, "TempFile_%d", Index);
        FileList[Index] = fopen(FileName, "rb+");
    }
    //批量归并
    MulticannelMergeFile(File, FileList, FileNumber, DataMin, DataMax, Length, Compare, Split);
    for (long Index = 0; Index < FileNumber; Index++) {
        fclose(FileList[Index]);
        sprintf(FileName, "TempFile_%d", Index);
        remove(FileName);
    }
    free(FileList);
    free(Data);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
long MulticannelSort(char *PathFile, long ZoneMax, void *DataMin, void *DataMax,
                     int Length, SortCompare Compare, char Split, int NeedCheck)
{
    FILE *File = fopen(PathFile, "rb+");
    DevideAndConquerFile(File, ZoneMax, DataMin, DataMax, Length, Compare, Split);
    long Result = (NeedCheck == 0) ? 0 : CheckFileData(File, Length, Compare, Split);
    fclose(File);
    return Result;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
