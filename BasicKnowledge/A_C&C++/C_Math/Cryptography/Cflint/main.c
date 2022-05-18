/* 例: */
#include <stdio.h>
#include "Cflint.h"
#include "CflintTest.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#if 0
#elif 1  /* Windows */
#include <windows.h>
#include <wincrypt.h>
int32_t Calculate_RNG(uint8_t *dest, uint32_t size)
{
    HCRYPTPROV prov;
    
    if (!CryptAcquireContext(&prov, NULL, NULL, PROV_RSA_FULL,
                                                CRYPT_VERIFYCONTEXT))
        return 0;

    CryptGenRandom(prov, size, (BYTE *)dest);
    CryptReleaseContext(prov, 0);
    return 1;
}
#else
int32_t Calculate_RNG(uint8_t *dest, uint32_t size)
{
    return 0;
}
#endif


int main(int argc, char *argv[]) {
    /* 这里强转函数指针类型 */
    Cflint_PortInfoCheck((Cflint_PortInfoPrint)printf);

    // Test_CflintFunctionSet1();
    // Test_CflintFunctionSet2();
    // Test_CflintFunctionSet3();
    // Test_CflintFunctionSet4();
    // Test_CflintFunctionSet5();
    // Test_CflintFunctionSet6();
    // Test_CflintFunctionSet7();
    // Test_CflintFunctionSet8();
    //未完成,测试不通过
    // Test_Mentgomery();
    


    return 0;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
