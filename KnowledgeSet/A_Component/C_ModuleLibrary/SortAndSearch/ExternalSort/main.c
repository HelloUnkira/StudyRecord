#include <stdio.h>
#include <limits.h>
#include "ExternalSort.h"

typedef struct DataStructForExternalSort {
    int Data4;
} DataStruct;

int DataStructCompare(const void *Data1, const void *Data2)
{
    int D1 = ((DataStruct *)Data1)->Data4;
    int D2 = ((DataStruct *)Data2)->Data4;
    
    return D1 > D2 ? 1 : D1 == D2 ? 0 : -1;
}

DataStruct DataMin = {
    .Data4 = INT_MIN,
};

DataStruct DataMax = {
    .Data4 = INT_MAX,
};

#define DataNumber  1000000
#define DataSplit   '\n'
#define DataDomain  10000
#define DataUnit    100000

/* 注意:一个进程不支持打开过多的流 */
/*      多路流计算:DataNumber / DataUnit */

int main(int argc, char* argv[]) {

    char *FileName = "SampleFile";
    char *ReadFileName = "ReadFile.txt";
    FILE *File = fopen(FileName, "wb+");
    FILE *ReadFile = fopen(ReadFileName, "wb+");
    for (long I = 0; I < DataNumber; I++) {
        char Split = DataSplit;
        DataStruct TempData = {0};
        TempData.Data4 = ((rand() % 2) == 0) ?
                          (rand() % DataDomain) :
                         -(rand() % DataDomain);
        fwrite(&TempData, sizeof(DataStruct), 1, File);
        fwrite(&Split, 1, 1, File);
    }
    fclose(File);
    long Result = MulticannelSort(FileName, (sizeof(DataStruct) + 1) * DataUnit,
                                  &DataMin, &DataMax,
                                  sizeof(DataStruct), DataStructCompare, DataSplit, 1);
    printf("\r\nMulticannelSort:Check:%ld\r\n", Result);
    File = fopen(FileName, "rb+");
    for (long I = 0; I < DataNumber; I++) {
        char Split = 0;
        DataStruct TempData = {0};
        fread(&TempData, sizeof(DataStruct), 1, File);
        fread(&Split, 1, 1, File);
        fprintf(ReadFile, "%d%c", TempData.Data4, Split);
    }
    fclose(ReadFile);
    fclose(File);
    
    return 0;
}


