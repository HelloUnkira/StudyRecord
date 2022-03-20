//C std lib
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define   CALCULATOR_C
#include "Calculator.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
static inline void * CalculatorMalloc(uint32_t size)
{
#ifndef CALCULATE_MALLOC(size)
#error  "CalculatorMalloc: Dynamic memory policies must be supported"
#endif
    return CALCULATE_MALLOC(size);
}
static inline void CalculatorFree(void *pointer)
{
#ifndef CALCULATE_FREE(pointer)
#error  "CalculatorFree: Dynamic memory policies must be supported"
#endif
    CALCULATE_FREE(pointer);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 符号记录(表明支持的基本运算规则): */
enum {
    DEFAULT         = 0,
    SUCCESS         = 1,
    ERROR           = 2,
    TYPEDATA        ,
    TYPEFLAG        ,
    /* 内置解析的括号 */
    I_BracketLeft   ,
    I_BracketRight  ,
    I_Comma         ,
    /* 默认几个数学符号: */
    D_E             ,   // E
    D_PAI           ,   // PAI
    /* 基础运算类 */
    B_Add           ,   // +
    B_Subtract      ,   // -
    B_Multiply      ,   // *
    B_Devide        ,   // /
    /* 扩展运算1类 */
    E_FMOD          ,   // double fmod(double x, double y)
    E_POW           ,   // double pow(double x, double y)
    E_FABS          ,   // double fabs(double x)
    /* 扩展运算2类 */
    E_CEIL          ,   // double ceil(double x)
    E_FLOOR         ,   // double floor(double x)
    /* 扩展运算3类 */
    E_SQRT          ,   // double sqrt(double x)
    /* 扩展运算4.1类 */
    E_COS           ,   // double cos(double x)
    E_SIN           ,   // double sin(double x)
    E_TAN           ,   // double tan(double x)
    /* 扩展运算4.2类 */
    E_COSH          ,   // double cosh(double x)
    E_SINH          ,   // double sinh(double x)
    E_TANH          ,   // double tanh(double x)
    /* 扩展运算4.3类 */
    E_ACOS          ,   // double acos(double x)
    E_ASIN          ,   // double asin(double x)
    E_ATAN          ,   // double atan(double x)
    E_ATAN2         ,   // double atan2(double y, double x)
    /* 扩展运算5类 */
    E_LOG           ,   // double log(double x)
    E_LOG10         ,   // double log10(double x)
    /* 扩展运算6类 */
    E_EXP           ,   // double exp(double x)
    /* 其余扩展后续支持 */
    E_FREXP         ,   // double frexp(double x, int *exponent)
    E_LDEXP         ,   // double ldexp(double x, int exponent)
    E_MODF          ,   // double modf(double x, double *integer)
    /*  */
} Expression_Flag;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 通过基础配置和伸缩因子生成初始栈或者扩展栈 */
static void CalculatorStack(uint8_t Type, void **Stack, uint32_t *Length)
{
    /* 初始化时的生成栈 */
    if (*Stack == NULL) {
        *Stack  = CalculatorMalloc(Type * CALCULATE_LENGTH_BASE);
        *Length = CALCULATE_LENGTH_BASE;
        return;
    }
    /* 结束时的回收栈 */
    if (*Length == 0) {
        CalculatorFree(*Stack);
        *Stack  = NULL;
        *Length = 0;
        return;
    }
    /* 长度至少为基础栈长 */
    if (*Length < CALCULATE_LENGTH_BASE)
        *Length = CALCULATE_LENGTH_BASE;
    /* 计算新栈长 */
    uint32_t LengthNew = *Length + CALCULATE_LENGTH_FACTOR;
    /* 生成新栈 */
    void *StackNew = CalculatorMalloc(Type * LengthNew);
    /* 将原生栈信息拷贝到新栈中 */
    uint32_t Index = 0;
    if (Type == sizeof(double))
        for (Index = 0; Index < *Length; Index++)
            ((double *)StackNew)[Index] = ((double *)(*Stack))[Index];
    if (Type == sizeof(uint8_t))
        for (Index = 0; Index < *Length; Index++)
            ((uint8_t *)StackNew)[Index] = ((uint8_t *)(*Stack))[Index];
    /* 反馈新栈 */
    CalculatorFree(*Stack);
    *Stack  = StackNew;
    *Length = LengthNew;
    return StackNew;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 操作符所需操作数解析运算: */
static inline uint8_t CalculatorFlagNumber(uint8_t Type)
{   /* 如此强行写出的代码有种离经叛道的感觉,纯属个人爱好 */
    switch (Type) {
    /* 基本数解析 */
    case D_E:           return 0;
    case D_PAI:         return 0;
    /* 基本运算解析 */
    case B_Add:         return 2;
    case B_Subtract:    return 2;
    case B_Multiply:    return 2;
    case B_Devide:      return 2;
    /* 扩展运算1类 */
    case E_FMOD:        return 2;
    case E_POW:         return 2;
    case E_FABS:        return 1;
    /* 扩展运算2类 */
    case E_CEIL:        return 1;
    case E_FLOOR:       return 1;
    /* 扩展运算3类 */
    case E_SQRT:        return 1;
    /* 扩展运算4.1类 */
    case E_COS:         return 1;
    case E_SIN:         return 1;
    case E_TAN:         return 1;
    /* 扩展运算4.2类 */
    case E_COSH:        return 1;
    case E_SINH:        return 1;
    case E_TANH:        return 1;
    /* 扩展运算4.3类 */
    case E_ACOS:        return 1;
    case E_ASIN:        return 1;
    case E_ATAN:        return 1;
    case E_ATAN2:       return 2;
    /* 扩展运算5类 */
    case E_LOG:         return 1;
    case E_LOG10:       return 1;
    /* 扩展运算6类 */
    case E_EXP:         return 1;
    /* 其余扩展后续支持 */
    }
    ERROR_PRINT(true, "CalculatorFlagNumber");
    return -1;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 接收上述指定类型参数完成结果计算反馈 */
/* 这里调用系统数学库,需要库作为基础外界支持 */
static inline bool CalculatorRun(uint8_t Type, double *Result, double x, double y)
{   /* 如此强行写出的代码有种离经叛道的感觉,纯属个人爱好 */
    switch (Type) {
    /* 基本数解析 */
    case D_E:           *Result = exp(1);           return true;
    case D_PAI:         *Result = acos(-1);         return true;
    /* 基本运算解析 */
    case B_Add:         *Result = x + y;            return true;
    case B_Subtract:    *Result = x - y;            return true;
    case B_Multiply:    *Result = x * y;            return true;
    case B_Devide:      *Result = x / y;            return true;
    /* 扩展运算1类 */
    case E_FMOD:        *Result = fmod(x, y);       return true;
    case E_POW:         *Result = pow(x, y);        return true;
    case E_FABS:        *Result = fabs(x);          return true;
    /* 扩展运算2类 */
    case E_CEIL:        *Result = ceil(x);          return true;
    case E_FLOOR:       *Result = floor(x);         return true;
    /* 扩展运算3类 */
    case E_SQRT:        *Result = sqrt(x);          return true;
    /* 扩展运算4.1类 */
    case E_COS:         *Result = cos(x);           return true;
    case E_SIN:         *Result = sin(x);           return true;
    case E_TAN:         *Result = tan(x);           return true;
    /* 扩展运算4.2类 */
    case E_COSH:        *Result = cosh(x);          return true;
    case E_SINH:        *Result = sinh(x);          return true;
    case E_TANH:        *Result = tanh(x);          return true;
    /* 扩展运算4.3类 */
    case E_ACOS:        *Result = acos(x);          return true;
    case E_ASIN:        *Result = asin(x);          return true;
    case E_ATAN:        *Result = atan(x);          return true;
    case E_ATAN2:       *Result = atan2(x, y);      return true;
    /* 扩展运算5类 */
    case E_LOG:         *Result = log(x);           return true;
    case E_LOG10:       *Result = log10(x);         return true;
    /* 扩展运算6类 */
    case E_EXP:         *Result = exp(x);           return true;
    /* 其余扩展后续支持 */
    }
    ERROR_PRINT(1, "CalculatorRun:Type");
    return false;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 将一个可能的字母起始的字符串解析为操作符 */
static inline uint8_t CalculatorParseFlag(char *String, uint32_t Length)
{   /* 为了简化代码,这里的Length是一个相对较大的数,它至少是最大操作符的长度 */
    /*
    if (Length <= 5) {
        ERROR_PRINT(Length <= 5, "CalculatorParseFlag:Length")
        return ERROR;
    }
    */
    /*
     *这里没有太多加速比较的方法,只能一个个比较
     *当然,可以采用switch嵌套加速过滤做优化
     *这里做了分类,最优需要融合在一起
     */
    
    /* 默认几个数学符号: */
    switch (String[0]) {
    case 'E':
        if (String[1] == '\0')
            return D_E;
        break;
    case 'P':
        if (String[1] == 'A' && String[2] == 'I' && String[3] == '\0')
            return D_PAI;
        break;
    case 'e':
        if (String[1] == '\0')
            return D_E;
        break;
    case 'p':
        if (String[1] == 'a' && String[2] == 'i' && String[3] == '\0')
            return D_PAI;
        break;
    }
    
    /* 扩展运算1类 */
    switch (String[0]) {
    case 'f':
        if (String[1] == 'm' &&  String[2] == 'o' && String[3] == 'd' &&
            String[4] == '\0')
            return E_FMOD;
        if (String[1] == 'a' &&  String[2] == 'b' && String[3] == 's' &&
            String[4] == '\0')
            return E_FABS;
        break;
    case 'p':
        if (String[1] == 'o' &&  String[2] == 'w' && String[3] == '\0')
            return E_POW;
        break;
    }
    /* 扩展运算2类 */
    switch (String[0]) {
    case 'c':
        if (String[1] == 'e' &&  String[2] == 'i' && String[3] == 'l' &&
            String[4] == '\0')
            return E_CEIL;
        break;
    case 'f':
        if (String[1] == 'l' &&  String[2] == 'o' && String[3] == 'o' &&
            String[4] == 'r' &&  String[5] == '\0')
            return E_FLOOR;
        break;
    }
    /* 扩展运算3类 */
    switch (String[0]) {
    case 's':
        if (String[1] == 'q' &&  String[2] == 'r' && String[3] == 't' &&
            String[4] == '\0')
            return E_SQRT;
        break;
    }
    /* 扩展运算4类 */
    switch (String[0]) {
    case 'a':
        if (String[1] == 'c' && String[2] == 'o' &&
            String[3] == 's' && String[4] == '\0')
            return E_ACOS;
        if (String[1] == 's' && String[2] == 'i' &&
            String[3] == 'n' && String[4] == '\0')
            return E_ASIN;
        if (String[1] == 't' && String[2] == 'a' &&
            String[3] == 'n') {
            if (String[4] == '\0')
                return E_ATAN;
            if (String[4] == '2' && String[5] == '\0')
                return E_ATAN2;
        }
        break;
    case 'c':
        if (String[1] == 'o' &&  String[2] == 's') {
            if (String[3] == '\0')
                return E_COS;
            if (String[3] == 'h' &&String[4] == '\0')
                return E_COSH;
        }
        break;
    case 's':
        if (String[1] == 'i' &&  String[2] == 'n') {
            if (String[3] == '\0')
                return E_SIN;
            if (String[3] == 'h' &&String[4] == '\0')
                return E_SINH;
        }
        break;
    case 't':
        if (String[1] == 'a' &&  String[2] == 'n') {
            if (String[3] == '\0')
                return E_TAN;
            if (String[3] == 'h' &&String[4] == '\0')
                return E_TANH;
        }
        break;
    }
    /* 扩展运算5类 */
    switch (String[0]) {
    case 'l':
        if (String[1] == 'o' && String[2] == 'g') {
            if (String[3] == '\0')
                return E_LOG;
            if (String[3] == '1' && String[4] == '0' && String[5] == '\0')
                return E_LOG10;
        }
        break;
    }
    /* 扩展运算6类 */
    switch (String[0]) {
    case 'e':
        if (String[1] == 'x' && String[2] == 'p' && String[3] == '\0')
            return E_EXP;
        break;
    }
    /* 错误检查 */
    ERROR_PRINT(true, "CalculatorParseFlag:");
    ERROR_PRINT(true, String);
    return ERROR;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 将一个可能的.或数字起始的字符串解析为数据 */
static inline bool CalculatorParseData(char *String, uint32_t Length, double *Result)
{
    uint32_t Index = 0;
    
    double PartInteger          = 0.0;
    double PartDecimal          = 0.0;
    double PartDecimalFactor    = 1.0;
    
    /* 先解析整数部分 */
    while (Index < Length) {
        if (String[Index] >= '0' && String[Index] <= '9') {
            PartInteger *= 10.0;
            PartInteger += String[Index] - '0';
            Index++;
            continue;
        }
        if (String[Index] == '.')
            break;
        ERROR_PRINT(true, "CalculatorParseData:PartInteger");
        return false;
    }
    /* 再解析小数点 */
    if (String[Index] == '.')
        Index++;
    /* 再解析小数部分 */
    while (Index < Length) {
        if (String[Index] >= '0' && String[Index] <= '9') {
            PartDecimalFactor *= 0.1;
            PartDecimal += (String[Index] - '0') * PartDecimalFactor;
            Index++;
            continue;
        }
        ERROR_PRINT(true, "CalculatorParseData:PartDecimal");
        return false;
    }
    /* 拼接整数部分和小数部分 */
    *Result = PartInteger + PartDecimal;
    return true;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 过滤部分留白字符或忽略字符 */
static inline bool CalculatorIsIgnore(char character)
{
    if (character == ' ' || character == '\t' || character == '\n')
        return true;
    return false;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 指定除()括号外其余运算符优先级规则 */
static inline uint8_t CalculatorGetPriority(uint8_t Type)
{
    /* ()的优先级最高 */
    if (Type == I_BracketLeft ||
        Type == I_BracketRight)
        return 5;
    /* +-号优先级最低 */
    if (Type == B_Add ||
        Type == B_Subtract)
        return 1;
    /* /*号优先级仅次 */
    if (Type == B_Multiply ||
        Type == B_Devide)
        return 2;
    /* 其他的优先级一致居中 */
    return 3;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 解析一个数据或符号并反馈为对应的类型 */
static inline uint8_t CalculatorParse(char *Pointer, uint32_t *Index, uint8_t TypeLast,
                                      double *Data, uint8_t *Flag)
{
    if (Pointer[*Index] == '\0')
        return SUCCESS;
    /* 1.先过滤一次留白 */
    while (CalculatorIsIgnore(Pointer[*Index]) == true)
        (*Index)++;
    /* 额外添加,如果出现-为单目运算的场景时 */
    if ((Pointer[*Index] == '-'     ||
         Pointer[*Index] == '+')        &&
        (TypeLast == DEFAULT        ||
         TypeLast == I_BracketLeft  ||
         TypeLast == B_Add          ||
         TypeLast == B_Subtract)) {
        /* 这里就做一下简单处理即可,索引不要去动 */
        *Data = 0.0;
        return TYPEDATA;
    }
    /* 额外添加,如果出现**时转为pow */
    if (Pointer[*Index] == '*' && Pointer[*Index + 1] == '*') {
        *Flag = E_POW;
        (*Index) += 2;
        return TYPEFLAG;
    }
    /* 基础内置符号本地检测并反馈 */
    switch (Pointer[*Index]) {
    case '(':
        *Flag = I_BracketLeft;
        (*Index)++;
        return TYPEFLAG;
    case ')':
        *Flag = I_BracketRight;
        (*Index)++;
        return TYPEFLAG;
    case '+':
        *Flag = B_Add;
        (*Index)++;
        return TYPEFLAG;
    case '-':
        *Flag = B_Subtract;
        (*Index)++;
        return TYPEFLAG;
    case '*':
        *Flag = B_Multiply;
        (*Index)++;
        return TYPEFLAG;
    case '/':
        *Flag =  B_Devide;
        (*Index)++;
        return TYPEFLAG;
    case '%':
        *Flag = E_FMOD;
        (*Index)++;
        return TYPEFLAG;
    case ',':
        *Flag = I_Comma;
        (*Index)++;
        return TYPEFLAG;
    }
    /* 准备进一步检测 */
    char *String = NULL;
    uint32_t IndexNew = *Index;
    /* 检测是出现了符号? */
    if ((Pointer[IndexNew] >= 'a' && Pointer[IndexNew] <= 'z') ||
        (Pointer[IndexNew] >= 'A' && Pointer[IndexNew] <= 'Z')) {
        /* 确认符号的范围 */
        while ((Pointer[IndexNew] >= 'a' && Pointer[IndexNew] <= 'z') ||
               (Pointer[IndexNew] >= 'A' && Pointer[IndexNew] <= 'Z') ||
               (Pointer[IndexNew] >= '0' && Pointer[IndexNew] <= '9') ||
                /* 这里留白字符一并计算 */
               (CalculatorIsIgnore(Pointer[IndexNew]) == true))
                IndexNew++;
        /* 生成空间保留该片段,用于下一步的解析 */
        uint32_t Length = 0;
        String = CalculatorMalloc(sizeof(char) * (IndexNew - *Index + 1));
        uint32_t IndexTemp = 0;
        for (IndexTemp = *Index; IndexTemp < IndexNew; IndexTemp++) {
            if (CalculatorIsIgnore(Pointer[IndexTemp]) == true)
                continue;
            String[Length++] = Pointer[IndexTemp];
        }
        /* 最后填充字符串尾缀 */
        String[Length] = '\0';
        /* 解析符号 */
        *Flag = CalculatorParseFlag(String, Length);
        /* 错误检查 */
        if (*Flag == ERROR) {
            ERROR_PRINT(true, "CalculatorParse:Flag");
            ERROR_PRINT(true, String);
            CalculatorFree(String);
            return ERROR;
        }
        /* 指针重定位 */
        *Index = IndexNew;
        /* 结束解析 */
        CalculatorFree(String);
        return TYPEFLAG;
    }
    /* 检测是出现了数字 */
    if ((Pointer[IndexNew] >= '0' && Pointer[IndexNew] <= '9') ||
        (Pointer[IndexNew] == '.')) {
        /* 确认数字的范围 */
        while ((Pointer[IndexNew] >= '0' && Pointer[IndexNew] <= '9') ||
               (Pointer[IndexNew] == '.') ||
                /* 这里留白字符一并计算 */
               (CalculatorIsIgnore(Pointer[IndexNew]) == true))
                IndexNew++;
                
        /* 生成空间保留该片段,用于下一步的解析 */
        uint32_t Length = 0;
        String = CalculatorMalloc(IndexNew - *Index + 1);
        uint32_t IndexTemp = 0;
        for (IndexTemp = *Index; IndexTemp < IndexNew; IndexTemp++) {
            if (CalculatorIsIgnore(Pointer[IndexTemp]) == true)
                continue;
            String[Length++] = Pointer[IndexTemp];
        }
        /* 最后填充字符串尾缀 */
        String[Length] = '\0';
        /* 解析符号 */
        if (CalculatorParseData(String, Length, Data) == false) {
            ERROR_PRINT(true, "CalculatorParse:Number");
            ERROR_PRINT(true, String);
            CalculatorFree(String);
            return ERROR;
        }
        /* 指针重定位 */
        *Index = IndexNew;
        /* 结束解析 */
        CalculatorFree(String);
        return TYPEDATA;
    }
    ERROR_PRINT(true, "CalculatorParse:Unkonwn");
    return ERROR;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 生成新的字符串,去除留白,捕获不支持的字符错误 */
static inline char * CalculatorStringUnimportant(char *Expression)
{
    uint32_t Index      = 0;
    uint32_t IndexNew   = 0;
    /* 获得旧有字符串长度 */
    uint32_t Length = strlen(Expression);
    /* 生成一个新字符串,用于装载数据 */
    char *ExpressionNew = CalculatorMalloc(sizeof(char) * (Length + 1));
    /* 忽略掉数学表达式不关心的部分 */
    for (Index = 0; Index < Length; Index++) {
        /* 将关键的字符保留 */
        if (Expression[Index] == '(' ||
            Expression[Index] == ')' ||
            Expression[Index] == '+' ||
            Expression[Index] == '-' ||
            Expression[Index] == '*' ||
            Expression[Index] == '/' ||
            Expression[Index] == '%' ||
            Expression[Index] == ',' ||
            Expression[Index] == '.') {
            ExpressionNew[IndexNew++] = Expression[Index];
            continue;
        }
        if (Expression[Index] >= '0' &&
            Expression[Index] <= '9') {
            ExpressionNew[IndexNew++] = Expression[Index];
            continue;
        }
        if (Expression[Index] >= 'A' &&
            Expression[Index] <= 'Z') {
            ExpressionNew[IndexNew++] = Expression[Index];
            continue;
        }
        if (Expression[Index] >= 'a' &&
            Expression[Index] <= 'z') {
            ExpressionNew[IndexNew++] = Expression[Index];
            continue;
        }
        /* 过滤留白 */
        if (Expression[Index] == ' '  ||
            Expression[Index] == '\t' ||
            Expression[Index] == '\n')
            continue;
        /* 将不支持的字符输出 */
        char ErrorChar[10] = {0};
        sprintf(ErrorChar, "%c ", Expression[Index]);
        ERROR_PRINT(true, ErrorChar);
        break;
    }
    /*  */
    if (IndexNew == 0) {
        CalculatorFree(ExpressionNew);
        return NULL;
    }
    return ExpressionNew;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 表达式解析策略(堆栈模式) */
bool CalculatorMathExpression(char *Expression, double *Result, ErrorPrint Print)
{
    /* 重解析字符串,去除留白并更新字符串 */
    /* 注意:这一步并不是必备的,主要为了流程化 */
    //char *ExpressionNew = CalculatorStringUnimportant(Expression);
    /* 空运算流检查 */
    //if (ExpressionNew == NULL) {
    //    ERROR_PRINT(true, "Error Expression:");
    //    ERROR_PRINT(true, Expression);
    //    return false;
    //}
    /* 将简要过滤前后的字符串输出 */
    //ERROR_PRINT(true, Expression);
    //ERROR_PRINT(true, ExpressionNew);
    
    /* 生成数据暂存栈 */
    double  *NumberStack = NULL;
    uint32_t NumberMax   = 0;
    uint32_t NumberTop   = 0;
    /* 生成符号暂存栈 */
    uint8_t *FlagStack = NULL;
    uint32_t FlagMax   = 0;
    uint32_t FlagTop   = 0;
    /* 生成栈空间,初始化 */
    CalculatorStack(sizeof(double),  &NumberStack, &NumberMax);
    CalculatorStack(sizeof(uint8_t), &FlagStack,   &FlagMax);
    ERROR_PRINT(NumberStack == NULL || NumberMax == 0,
                "CalculatorMathExpression: NumberXxx");
    ERROR_PRINT(FlagStack == NULL   || FlagMax == 0,
                "CalculatorMathExpression: FlagXxx");
    /* 后缀表达式计算 */
    uint8_t  Type       = DEFAULT;  //当前状态
    uint8_t  TypeLast   = DEFAULT;  //上次状态暂存记录
    uint8_t  State      = DEFAULT;  //程序运行状态
    uint32_t Index      = 0;        //索引进动情况
    double   Data       = 0;        //数据暂存记录
    uint8_t  Flag       = DEFAULT;  //符号暂存记录
    
    /* 后缀表达式解析 */
    /* 先到符号栈压入一个左括号 */
    FlagStack[++FlagTop] = I_BracketLeft;
    do {
        /* 计算到了末尾,假压入一个右括号,用于消除堆栈全部操作符 */
        if (Expression[Index] == '\0') {
            Flag  = I_BracketRight;
            Type  = TYPEFLAG;
            State = SUCCESS;
        }
        /* 解析一个数据或符号 */
        if (Expression[Index] != '\0')
            Type = CalculatorParse(Expression, &Index, TypeLast, &Data, &Flag);
        /* 错误检查 */
        if (Type == ERROR) {
            char ErrorString[30] = {0};
            sprintf(ErrorString, "Error Index:%d", Index);
            ERROR_PRINT(true, ErrorString);
            Print(ErrorString);
            State == ERROR;
            break;
        }
        /* 如果解析到了一个数据 */
        if (Type == TYPEDATA) {
            /* 如果栈顶空间不够,扩展它 */
            if (++NumberTop > NumberMax)
                CalculatorStack(sizeof(double), &NumberStack, &NumberMax);
            /* 将数据加入到缓存栈的栈顶 */
            NumberStack[NumberTop] = Data;
        }
        /* 如果解析到了一个符号 */
        /* 运算规则1:左括号直接入栈 */
        /* 运算规则2:右括号入栈,弹出运算符到左括号为止 */
        /* 运算规则3:将当前优先级高的和相等的操作符弹出,直到遇见(或最低 */
        if (Type == TYPEFLAG) {
            /* 其他情况都要涉及操作符的弹出过程 */
            while (FlagTop > 0) {
                /* 左括号直接入栈 */
                if (Flag == I_BracketLeft) {
                    /* 如果栈顶空间不够,扩展它 */
                    if (++FlagTop > FlagMax)
                        CalculatorStack(sizeof(uint8_t), &FlagStack, &FlagMax);
                    /* 将符号压入到缓存栈的栈顶 */
                    FlagStack[FlagTop] = Flag;
                    break;
                }
                /* 当前优先级高于栈顶优先级直接入栈 */
                if (Flag != I_BracketRight &&
                   (CalculatorGetPriority(Flag) >
                    CalculatorGetPriority(FlagStack[FlagTop]))) {
                    /* 如果栈顶空间不够,扩展它 */
                    if (++FlagTop >= FlagMax)
                        CalculatorStack(sizeof(uint8_t), &FlagStack, &FlagMax);
                    /* 将符号压入到缓存栈的栈顶 */
                    FlagStack[FlagTop] = Flag;
                    break;
                }
                /* 逗号表达式可直接丢弃 */
                if (FlagStack[FlagTop] == I_Comma) {
                    FlagTop--;
                    continue;
                }
                /* 遇到左括号时 */
                if (FlagStack[FlagTop] == I_BracketLeft) {
                    /* 右括号遇到第一个左括号停下,弹出左括号 */
                    if (Flag == I_BracketRight) {
                        FlagTop--;
                        break;
                    }
                    /* 其他符号遇到左括号停下,压入符号 */
                    /* 如果栈顶空间不够,扩展它 */
                    if (++FlagTop >= FlagMax)
                        CalculatorStack(sizeof(uint8_t), &FlagStack, &FlagMax);
                    /* 将符号压入到缓存栈的栈顶 */
                    FlagStack[FlagTop] = Flag;
                    break;
                }
                /* 其他符号遇到低优先级的符号停下 */
                if ((Flag != I_BracketRight) &&
                    (CalculatorGetPriority(Flag) >
                     CalculatorGetPriority(FlagStack[FlagTop]))) {
                    /* 如果栈顶空间不够,扩展它 */
                    if (++FlagTop >= FlagMax)
                        CalculatorStack(sizeof(uint8_t), &FlagStack, &FlagMax);
                    /* 将符号压入到缓存栈的栈顶 */
                    FlagStack[FlagTop] = Flag;
                    break;
                }

                /* 获取栈顶操作符,解析 */
                uint8_t Number = CalculatorFlagNumber(FlagStack[FlagTop]);
                /* 栈顶操作符错误 */
                if (Number == -1) {
                    char ErrorString[30] = {0};
                    sprintf(ErrorString, "Error Index:%d", Index);
                    ERROR_PRINT(true, ErrorString);
                    Print(ErrorString);
                    State == ERROR;
                    break;
                }
                /* 使用解析后的参数数量计算栈顶结果 */
                if (Number > NumberTop) {
                    char ErrorString[30] = {0};
                    sprintf(ErrorString, "Error Index:%d", Index);
                    ERROR_PRINT(true, ErrorString);
                    Print(ErrorString);
                    State == ERROR;
                    break;
                }
                /* 获取运算操作数 */
                double X = 0;
                double Y = 0;
                /* 无操作数 */
                if (Number >= 2)
                    Y = NumberStack[NumberTop--];
                if (Number >= 1)
                    X = NumberStack[NumberTop--];
                if (Number == 0);
                /* 计算栈顶结果 */
                double ResultTemp = 0;
                if (CalculatorRun(FlagStack[FlagTop], &ResultTemp, X, Y) == false) {
                    char ErrorString[30] = {0};
                    sprintf(ErrorString, "Error Index:%d", Index);
                    ERROR_PRINT(true, ErrorString);
                    Print(ErrorString);
                    State == ERROR;
                    break;
                }
                /* 栈顶结果重新入栈 */
                /* 如果栈顶空间不够,扩展它 */
                if (++NumberTop >= NumberMax)
                    CalculatorStack(sizeof(double), &NumberStack, &NumberMax);
                /* 将数据加入到缓存栈的栈顶 */
                NumberStack[NumberTop] = ResultTemp;
                /* 栈顶符号弹出 */
                FlagTop--;
            }
        }
        if (Type == TYPEDATA)
            TypeLast = TYPEDATA;
        if (Type == TYPEFLAG)
            TypeLast = Flag;
    } while (State != SUCCESS && State != ERROR);
    /* 错误检查: */
    if (FlagTop != 0) {
        ERROR_PRINT(true, "ERROR: Flag Too Much\n");
        Print("ERROR: Flag Too Much\n");
        State = ERROR;
    }
    if (NumberTop != 1) {
        ERROR_PRINT(true, "ERROR: Number Too Much\n");
        Print("ERROR: Number Too Much\n");
        State = ERROR;
    }
    
    if (State == SUCCESS)
        *Result = NumberStack[NumberTop];
    /* 结束运行 */
    NumberMax = 0;
    FlagMax   = 0;
    /* 回收栈空间 */
    CalculatorStack(sizeof(double),  &NumberStack, &NumberMax);
    CalculatorStack(sizeof(uint8_t), &FlagStack,   &FlagMax);
    //CalculatorFree(ExpressionNew);
    /* 计算结果反馈 */
    if (State != SUCCESS)
        return false;
    if (State == SUCCESS)
        return true;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
