#include <stdio.h>
#include "Exception.h"

void TestException(ExceptionLabel Label)
{
    /* 异常投递及捕获类型为非0值 */
#define ExceptionType1      1
#define ExceptionType2      2
    
    printf("---------------------------\n");
    ExceptionStart
        Try {
            /* TODO... UNTIL... */
            printf("Do Something\n");
            printf("Try-Throw-Catch Execute\n");
            /* C的体系不支持完整的异常捕获能力 */
            /* 所以只能手动使用条件语句确定异常类型 */
            /* 因为这是一个样本,所以异常类型直接传参确定 */
            /* 实际上异常类型应该在此次经过条件语句过滤生成 */
            /* 如果投递值为0会直接过滤而不执行投递 */
            Throw(Label);
            /* 因为投递值为0默认是无异常的 */
            /* 无需做任何额外的处理 */            
        } Catch(ExceptionType1) {
            printf("Catch Exception Type 1\n");
        } Catch(ExceptionType2) {
            printf("Catch Exception Type 2\n");
        } CatchUnknown {
            printf("Catch Unknown Exception\n");
        } Finally {
            printf("Try-Throw-Catch-Finally\n");
        }
    ExceptionEnd
    printf("---------------------------\n");
}

int main(int argc, char *argv[]) {
    TestException(0);
    TestException(1);
    TestException(2);
    TestException(3);
};
