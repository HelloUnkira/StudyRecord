/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-11-19     RT-Thread    first version
 */

#include <rtthread.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define ELF32_0BYTE1   signed char
#define ELF32_1BYTE4 unsigned int

typedef struct elf32_ksym {
    ELF32_1BYTE4 addr;  //
    ELF32_1BYTE4 name;  //
} Elf32_KSym;

//
#define SYMTAB_NAME  ".rodata.name"
#define SYMTAB_SYM   ".elf32symtab"

#define KSYM_EXPORT(symbol)                                               \
const ELF32_0BYTE1 ksym_##symbol##_name[] SECTION(SYMTAB_NAME) = #symbol; \
const Elf32_KSym   ksym_##symbol          SECTION(SYMTAB_SYM)  = {        \
    .addr = (ELF32_1BYTE4)(&symbol),                                      \
    .name = (ELF32_1BYTE4)(ksym_##symbol##_name)                          \
};


void test_ksymtab_1(void)
{
    LOG_D("test_ksymtab_1\n");
}

void test_ksymtab_2(void)
{
    LOG_D("test_ksymtab_2\n");
}

void ksymtab_test_0(void)
{
    LOG_D("ksymtab_test_2\n");
}

KSYM_EXPORT(test_ksymtab_1);
KSYM_EXPORT(test_ksymtab_2);
KSYM_EXPORT(ksymtab_test_0);

int main(int argc, char *argv[])
{
    LOG_D("Hello, world\n");

    return 0;
}



