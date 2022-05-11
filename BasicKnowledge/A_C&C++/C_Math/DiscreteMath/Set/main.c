#include <stdio.h> 
#include "FiniteSet.h"

int main(int argc, char *argv[]) {
    
    FiniteFullSet FullSet = {0};
    FiniteSubSet  Sub1 	  = {0};
    FiniteSubSet  Sub2    = {0};
    FiniteSubSet  Sub3    = {0};
    
    #define LENGTH  10
    uint32_t Set0[LENGTH] = {1,2,3,4,5,6,7,8,9,10};
    uint8_t  BitMap1[LENGTH / FINITE_SUBSET_BITS + 1] = {0};
    uint8_t  BitMap2[LENGTH / FINITE_SUBSET_BITS + 1] = {0};
    uint8_t  BitMap3[LENGTH / FINITE_SUBSET_BITS + 1] = {0};
        
    FullSet.Set    = Set0;
    FullSet.Length = LENGTH;
    FullSet.Number = 10;
    
    Sub1.BitMap = BitMap1;
    Sub2.BitMap = BitMap2;
    Sub3.BitMap = BitMap3;
    Sub1.Length = LENGTH / FINITE_SUBSET_BITS + 1;
    Sub2.Length = LENGTH / FINITE_SUBSET_BITS + 1;
    Sub3.Length = LENGTH / FINITE_SUBSET_BITS + 1;
    Sub1.Full = &FullSet;
    Sub2.Full = &FullSet;
    Sub3.Full = &FullSet;
    
    {
        for (uint32_t Index = 1; Index <= LENGTH; Index++)
            if(FiniteSubSet_ElementSet(&Sub1, Index) == false)
                printf("FiniteSubSet_ElementSet Fail");
        for (uint32_t Index = 1; Index <= LENGTH; Index++)
            if(FiniteSubSet_ElementCheck(&Sub1, Index) == false)
                printf("FiniteSubSet_ElementCheck Fail");
        for (uint32_t Index = 1; Index <= LENGTH; Index++)
            if(FiniteSubSet_ElementClear(&Sub1, Index) == false)
                printf("FiniteSubSet_ElementClear Fail");
        for (uint32_t Index = 1; Index <= LENGTH; Index++)
            if(FiniteSubSet_ElementCheck(&Sub1, Index) == true)
                printf("FiniteSubSet_ElementCheck Fail");
    }
    
    {
        uint32_t Length = 0;
        uint32_t Temp[LENGTH] = {0};
        
        for (uint32_t Index = 1; Index <= LENGTH; Index++)
            if(FiniteSubSet_ElementClear(&Sub1, Index) == false)
                printf("FiniteSubSet_ElementClear Fail");
        for (uint32_t Index = 1; Index <= LENGTH; Index++)
            if(FiniteSubSet_ElementClear(&Sub2, Index) == false)
                printf("FiniteSubSet_ElementClear Fail");
        
        FiniteSubSet_ElementSet(&Sub2, 1);
        FiniteSubSet_ElementSet(&Sub2, 3);
        FiniteSubSet_ElementSet(&Sub2, 5);
        FiniteSubSet_ElementSet(&Sub2, 7);
        FiniteSubSet_ElementSet(&Sub2, 9);
        
        FiniteSubSet_Not(&Sub1, &Sub2);

        printf("\nSub1 = ~Sub2:");
        Length = FiniteSubSet_ElementGets(&Sub2, Temp);
        printf("\nSub2:");
        for (uint32_t Index = 0; Index < Length; Index++)
            printf("%d ", Temp[Index]);
        Length = FiniteSubSet_ElementGets(&Sub1, Temp);
        printf("\nSub1:");
        for (uint32_t Index = 0; Index < Length; Index++)
            printf("%d ", Temp[Index]);
    }
    
    {
        uint32_t Length = 0;
        uint32_t Temp[LENGTH] = {0};
        
        for (uint32_t Index = 1; Index <= LENGTH; Index++)
            if(FiniteSubSet_ElementClear(&Sub1, Index) == false)
                printf("FiniteSubSet_ElementClear Fail");
        for (uint32_t Index = 1; Index <= LENGTH; Index++)
            if(FiniteSubSet_ElementClear(&Sub2, Index) == false)
                printf("FiniteSubSet_ElementClear Fail");
        for (uint32_t Index = 1; Index <= LENGTH; Index++)
            if(FiniteSubSet_ElementClear(&Sub3, Index) == false)
                printf("FiniteSubSet_ElementClear Fail");
        
        FiniteSubSet_ElementSet(&Sub2, 1);
        FiniteSubSet_ElementSet(&Sub2, 3);
        FiniteSubSet_ElementSet(&Sub2, 5);
        FiniteSubSet_ElementSet(&Sub2, 7);
        FiniteSubSet_ElementSet(&Sub2, 9);
        FiniteSubSet_ElementSet(&Sub3, 1);
        FiniteSubSet_ElementSet(&Sub3, 4);
        FiniteSubSet_ElementSet(&Sub3, 7);
        FiniteSubSet_ElementSet(&Sub3, 10);
        
        
        printf("\nSub2:");
        Length = FiniteSubSet_ElementGets(&Sub2, Temp);
        for (uint32_t Index = 0; Index < Length; Index++)
            printf("%d ", Temp[Index]);
        printf("\nSub3:");
        Length = FiniteSubSet_ElementGets(&Sub3, Temp);
        for (uint32_t Index = 0; Index < Length; Index++)
            printf("%d ", Temp[Index]);
        
        
        printf("\nSub1 = Sub2 & Sub3:");
        FiniteSubSet_And(&Sub1, &Sub2, &Sub3);
        Length = FiniteSubSet_ElementGets(&Sub1, Temp);
        printf("\nSub1:");
        for (uint32_t Index = 0; Index < Length; Index++)
            printf("%d ", Temp[Index]);
        
        printf("\nSub1 = Sub2 | Sub3:");
        FiniteSubSet_Or(&Sub1, &Sub2, &Sub3);
        Length = FiniteSubSet_ElementGets(&Sub1, Temp);
        printf("\nSub1:");
        for (uint32_t Index = 0; Index < Length; Index++)
            printf("%d ", Temp[Index]);
        
        printf("\nSub1 = Sub2 - Sub3:");
        FiniteSubSet_Sub(&Sub1, &Sub2, &Sub3);
        Length = FiniteSubSet_ElementGets(&Sub1, Temp);
        printf("\nSub1:");
        for (uint32_t Index = 0; Index < Length; Index++)
            printf("%d ", Temp[Index]);
    }
    
    return 0;
}

