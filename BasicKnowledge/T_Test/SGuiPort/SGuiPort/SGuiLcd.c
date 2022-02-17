/* ����Ψһ������ */
#include "SGuiDraw.h"

//����:
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
    //��ȡ��Ļ�ļ�
    fp1 = open("/dev/ubuntu_lcd", O_RDWR);
    if (fp1 == -1) {
        perror("open fail \n");
        return;
    }

    //ӳ�䣬�ϸ�����Ļ��С����ӳ�伴��
    plcd = (uint32_t *)mmap(NULL, xres * yres * 4, 
                            PROT_READ|PROT_WRITE, MAP_SHARED, fp1, 0);
    if (plcd == MAP_FAILED) {
        perror("mmap fail \n");
        return; 
    }

    //��Ļ��Ч�Բ���
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

/* �����������ƽ̨
#elif SGUI_USE_UNKNOWN
*/
#else

#endif
