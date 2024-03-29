#include "SGui_Driver.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#if 0
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#elif SGUI_USE_LINUX
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
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
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
int fp1 = 0;
int xres = 800;
int yres = 480;
uint32_t *plcd = NULL;
uint32_t  plcd_buffer[800 * 480] = {0};
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_ScreenInit(void)
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
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_ScreenDeinit(void)
{
    close(fp1);
    munmap(plcd,  xres * yres * 4);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 我们这里只使用屏幕的一小部分即可 */
#define ScreenWidth     320
#define ScreenHeight    360
#define PixelWidth      70
#define PixelHeight     70
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static SGui_Pixel PixelMap[PixelWidth * PixelHeight] = {0};
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
uint32_t SGui_ScreenWidth(void)
{
    return ScreenWidth;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
uint32_t SGui_ScreenHeight(void)
{
    return ScreenHeight;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_ScreenTakeMap(SGui_Pixel **Pixel, uint32_t *Length, uint32_t *Width, uint32_t *Height)
{
    *Pixel  = PixelMap;
    *Length = PixelWidth * PixelHeight;
    *Width  = PixelWidth;
    *Height = PixelHeight;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_ScreenGiveMap(SGui_Pixel *Pixel)
{
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_ScreenFrameLock(void)
{
    sleep(1);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_ScreenFrameUnlock(void)
{
    sleep(1);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_ScreenCanvas(SGui_Canvas *Canvas)
{
    /* 屏幕的域 */
    SGui_Area ScreenClip = {.LU_X = 0, .RB_X = 0 + ScreenWidth  - 1,
                            .LU_Y = 0, .RB_Y = 0 + ScreenHeight - 1,};
    /* 转为偏移量 */
    SGui_Area ScreenRegion = {0};
    SGui_ClipRegionAnd(&ScreenRegion, &Canvas->Clip, &ScreenClip);
    SGui_AreaPosToOff(&ScreenRegion);
    
    for (SGui_Coord Y = 0; Y < ScreenRegion.Off_H; Y++)
    for (SGui_Coord X = 0; X < ScreenRegion.Off_W; X++) {
        
        int y = Y + ScreenRegion.Off_Y;
        int x = X + ScreenRegion.Off_X;
        
        /* 有些屏幕需要将Alpha和Color合并到一起,有些需要分开 */
        SGui_PixelBlendAlphatoColor(&Canvas->Pixel[X + Y * Canvas->Offset.Off_W]);
        
        plcd_buffer[x + y * xres] = 
        
        *(uint32_t *)&Canvas->Pixel[X + Y * Canvas->Offset.Off_W].Color;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_ScreenCanvasSync(void)
{
    int x = 0, y = 0;
    for (x = 0; x < xres; x++)
        for (y = 0; y < yres; y++)
            plcd[x + y * xres] = plcd_buffer[x + y * xres];
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
//继续添加其他平台
//#elif SGUI_USE_UNKONWN
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#else
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_ScreenInit(void)
{
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_ScreenDeinit(void)
{
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
uint32_t SGui_ScreenWidth(void)
{
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
uint32_t SGui_ScreenHeight(void)
{
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_ScreenTakeMap(SGui_Pixel **Pixel, uint32_t *Length, uint32_t *Width, uint32_t *Height)
{
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_ScreenGiveMap(SGui_Pixel *Pixel)
{
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_ScreenFrameLock(void)
{
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_ScreenFrameUnlock(void)
{
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_ScreenCanvas(SGui_Canvas *Canvas)
{
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_ScreenCanvasSync(void)
{
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
