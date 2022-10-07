#include "DateTime.h"

int main(int argc, char *argv[])
{
    printf("\nDateTime_IsLeapYear\n");
    printf("(%u,%u)\n", 2020, DateTime_IsLeapYear(2020));
    printf("(%u,%u)\n", 2022, DateTime_IsLeapYear(2022));
    
    printf("\nDateTime_InMonth\n");
    for (uint32_t Month = 1; Month < 13; Month++)
        printf("(%u,%u)\n", Month, DateTime_InMonth(false, Month));
    
    printf("\nDateTime_PassBy\n");
    printf("(%u,%u,%u,%u)\n", 2022, 1, 1, DateTime_PassBy(2022, 1, 1));
    printf("(%u,%u,%u,%u)\n", 2022, 1, 31, DateTime_PassBy(2022, 1, 31));
    printf("(%u,%u,%u,%u)\n", 2022, 2, 1, DateTime_PassBy(2022, 2, 1));
    printf("(%u,%u,%u,%u)\n", 2022, 10, 7, DateTime_PassBy(2022, 10, 7));
    
    printf("\nDateTime_ToWeek\n");
    printf("(%u,%u,%u,%u)\n", 2022, 10, 1, DateTime_ToWeek(2022, 10, 1));
    printf("(%u,%u,%u,%u)\n", 2022, 10, 2, DateTime_ToWeek(2022, 10, 2));
    printf("(%u,%u,%u,%u)\n", 2022, 10, 3, DateTime_ToWeek(2022, 10, 3));
    printf("(%u,%u,%u,%u)\n", 2022, 10, 4, DateTime_ToWeek(2022, 10, 4));
    printf("(%u,%u,%u,%u)\n", 2022, 10, 5, DateTime_ToWeek(2022, 10, 5));
    printf("(%u,%u,%u,%u)\n", 2022, 10, 6, DateTime_ToWeek(2022, 10, 6));
    printf("(%u,%u,%u,%u)\n", 2022, 10, 7, DateTime_ToWeek(2022, 10, 7));
    
    printf("\nDateTime_HowManyDays\n");
    printf("(%u,%u,%u-%u,%u,%u-%u)\n", 2022, 10, 1, 2022, 10, 7,
           DateTime_HowManyDays(2022, 10, 1, 2022, 10, 7));
    printf("(%u,%u,%u-%u,%u,%u-%u)\n", 2022, 10, 1, 2022, 10, 31,
           DateTime_HowManyDays(2022, 10, 1, 2022, 10, 31));
    printf("(%u,%u,%u-%u,%u,%u-%u)\n", 2022, 10, 1, 2022, 11, 1,
           DateTime_HowManyDays(2022, 10, 1, 2022, 11, 1));
    printf("(%u,%u,%u-%u,%u,%u-%u)\n", 2022, 10, 1, 2023, 1, 1,
           DateTime_HowManyDays(2022, 10, 1, 2023, 1, 1));
    
    printf("\nDateTime_ToUTC\n");
    printf("(%u,%u,%u,%u,%u,%u-%llu)\n", 2022, 10, 1, 19, 54, 3,
           DateTime_ToUTC(2022, 10, 1, 19, 54, 3));
    
    return 0;
}
