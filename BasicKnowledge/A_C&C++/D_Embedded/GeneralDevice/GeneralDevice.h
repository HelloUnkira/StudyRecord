#ifndef GENERAL_DEVICE_H
#define GENERAL_DEVICE_H
/*************************************************************************************************/
//C std lib
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/* 类Linux驱动抽象模板仿写 */
/*************************************************************************************************/
void GeneralDevice_Init(void);
void GeneralDevice_DeInit(void);
void GeneralDevice_Open(void);
void GeneralDevice_Close(void);
/*************************************************************************************************/
/* 通配事件类型: */
typedef struct GeneralDevice_EventBody {
    uint32_t Event;
    uint32_t Size;
    uint8_t *Data;
} GeneralDevice_Event;
/*************************************************************************************************/
/* 事件解析: */
typedef enum GeneralDevice_EventTypeBody {
    GENERAL_DEVICE_DEFAULT = 0x00,
    GENERAL_DEVICE_EVENT1  = 0x00,
    GENERAL_DEVICE_EVENT2  = 0x00,
} GeneralDevice_Type;
/*************************************************************************************************/
bool GeneralDevice_Read(GeneralDevice_Event *Instance, uint32_t UnitTime, uint32_t MaxCount);
bool GeneralDevice_Write(GeneralDevice_Event *Instance, uint32_t UnitTime, uint32_t MaxCount);
/*************************************************************************************************/
#endif
