#ifndef EXCEPTION_H
#define EXCEPTION_H
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#include <assert.h>
#include <stdint.h>
#include <setjmp.h>
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define ExceptionLabel  uint32_t
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define ExceptionStart                  \
    do {                                \
        jmp_buf  __ExceptionStack__;    \
        uint32_t __ExceptionLabel__     \
        = setjmp(__ExceptionStack__);   \
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define ExceptionEnd        \
    } while (0);            \
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define Try             if (__ExceptionLabel__ == 0)
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define Throw(Label)    if (Label != 0) longjmp(__ExceptionStack__, Label)
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define Catch(Lable)    else if (__ExceptionLabel__ == Lable)
/*****************************************************************************/
#define CatchUnknown    else if (__ExceptionLabel__ != 0)
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define Finally
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
