/* 核心唯一依赖项 */
#include "SGuiDraw.h"

//测试:
#if SGUI_USE_LINUX

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <linux/fb.h>

int fp1 = 0;
int xres = 800;
int yres = 480;
uint32_t *plcd = NULL;

void SGuiLcdInit(void)
{
    //获取屏幕文件
    fp1 = open("/dev/ubuntu_lcd", O_RDWR);
    if (fp1 == -1) {
        perror("open fail \n");
        return;
    }

    //映射，严格按照屏幕大小对其映射即可
    plcd = (uint32_t *)mmap(NULL, xres * yres * 4, 
                            PROT_READ|PROT_WRITE, MAP_SHARED, fp1, 0);
    if (plcd == MAP_FAILED) {
        perror("mmap fail \n");
        return; 
    }

    //屏幕有效性测试
    int x = 0, y = 0;
    for (x = 0; x < xres; x++)
        for (y = 0; y < yres; y++)
            plcd[x + y * xres] = 0x000000;
}

void SGuiLcdDeinit(void)
{
    close(fp1);
    munmap(plcd,  xres * yres * 4);
}

uint32_t * SGuiLcdInstance(void)
{
    return plcd;
}

uint32_t SGuiLcdWidth(void)
{
    return xres;
}

uint32_t SGuiLcdHeight(void)
{
    return yres;
}

/* 继续添加其余平台
#elif SGUI_USE_UNKNOWN
*/
#else

#endif
