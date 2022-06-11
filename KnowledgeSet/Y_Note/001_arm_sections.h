/*
 * linker段配置
 */

//简易了解
//各种链接器节的定义
//链接器脚本,C文件和汇编文件所使用的Section声明

#ifndef ZEPHYR_INCLUDE_LINKER_SECTIONS_H_
#define ZEPHYR_INCLUDE_LINKER_SECTIONS_H_

//一个可运行的二进制程序基本上都是由三个段组成
//文本段存放程序指令(对应的机器码)(只读)
//数据段存放赋值全局变量,常量,静态变量
//未初始化数据段,仅纪录大小,执行时被分配出
//那么除去文本段后其他俩个段的组合便形成了数据区

//文本段
#define _TEXT_SECTION_NAME text
//只读段
#define _RODATA_SECTION_NAME rodata
//控制段
#define _CTOR_SECTION_NAME ctors
//数据段
//XIP的链接器问题,名称“data”不能使用
#define _DATA_SECTION_NAME datas
//状态段
#define _BSS_SECTION_NAME bss
//未初始化段
#define _NOINIT_SECTION_NAME noinit

//APP层所需的数据区空间
#define _APP_SMEM_SECTION_NAME		app_smem
#define _APP_DATA_SECTION_NAME		app_datas
#define _APP_BSS_SECTION_NAME		app_bss
#define _APP_NOINIT_SECTION_NAME	app_noinit

//未定义段名
#define _UNDEFINED_SECTION_NAME undefined

//中断关联段
//中断向量表段
#define _IRQ_VECTOR_TABLE_SECTION_NAME	.gnu.linkonce.irq_vector_table*
#define _SW_ISR_TABLE_SECTION_NAME	.gnu.linkonce.sw_isr_table*

//特定于体系结构的部分
#if defined(CONFIG_ARM)
//Kinetis_Flash配置段
#define _KINETIS_FLASH_CONFIG_SECTION_NAME  kinetis_flash_config
//TI_Control配置段
#define _TI_CCFG_SECTION_NAME		.ti_ccfg
//CPU专用CCM内存配置段
#define _CCM_DATA_SECTION_NAME		.ccm_data
#define _CCM_BSS_SECTION_NAME		.ccm_bss
#define _CCM_NOINIT_SECTION_NAME	.ccm_noinit
//TCM(直接高速内存)
//ITCM指令高速缓冲区配置段
#define _ITCM_SECTION_NAME			.itcm
//DTCM数据高速缓冲区配置段
#define _DTCM_DATA_SECTION_NAME		.dtcm_data
#define _DTCM_BSS_SECTION_NAME		.dtcm_bss
#define _DTCM_NOINIT_SECTION_NAME	.dtcm_noinit
#endif

//IMX 芯片驱动特殊段
#define _IMX_BOOT_CONF_SECTION_NAME	.boot_hdr.conf
#define _IMX_BOOT_DATA_SECTION_NAME	.boot_hdr.data
#define _IMX_BOOT_IVT_SECTION_NAME	.boot_hdr.ivt
#define _IMX_BOOT_DCD_SECTION_NAME	.boot_hdr.dcd_data
//STM32 芯片静态内存区配置段
#define _STM32_SDRAM1_SECTION_NAME	.stm32_sdram1
#define _STM32_SDRAM2_SECTION_NAME	.stm32_sdram2

//无缓冲内存配置
#ifdef CONFIG_NOCACHE_MEMORY
#define _NOCACHE_SECTION_NAME nocache
#endif

//在ASM文件中使用的短段引用
#if defined(_ASMLANGUAGE)
#define TEXT text//不同的文本段名
#if defined(CONFIG_X86)
#define TEXT_START text_start//X86文本段起始名
#else
#define TEXT_START text//文本段起始名
#endif

//各种数据类型的段名
#define BSS bss
#define RODATA rodata
#define DATA data
#define NOINIT noinit
#endif

#include <linker/section_tags.h>

#endif
