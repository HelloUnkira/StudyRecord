/* 例: */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "Calculator.h"

void Print(const char *ErrorString)
{
    printf("\n");
    printf(ErrorString);
    printf("\n");
}

int32_t main(int32_t argc, uint8_t *argv[]) {

    while (1) {
        char Expression[1024] = {0};
        /* 终端输入: */
        scanf("%s", Expression);
        /* 终端输入对照 */
        printf("%s\n", Expression);
        
        double Result = 0.0;
        bool Ret = false;
        Ret = CalculatorMathExpression(Expression, &Result, Print);

        printf("\nSuccess:%d, Result: %7f\n",Ret, Result);
    }
    return 0;
}


