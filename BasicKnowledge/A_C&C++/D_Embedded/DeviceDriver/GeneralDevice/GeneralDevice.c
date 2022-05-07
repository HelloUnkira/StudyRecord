/*************************************************************************************************/
#include "GeneralDevice.h"
/*************************************************************************************************/
void GeneralDevice_Init(void)
{
}
/*************************************************************************************************/
void GeneralDevice_DeInit(void)
{
}
/*************************************************************************************************/
void GeneralDevice_Open(void)
{
    /* 如果需要 */
}
/*************************************************************************************************/
void GeneralDevice_Close(void)
{
    /* 如果需要 */
}
/*************************************************************************************************/
/* 传输状态标记: */
/*************************************************************************************************/
static bool TransferFrameTxStatus = true;
static bool TransferFrameRxStatus = true;
/*************************************************************************************************/
/* 中间补充中断回调函数,完成对上述完成状态的检查 */
/*************************************************************************************************/
/* 扩充层级:数据链路传输层数据暂存队列容器 */
/*          也可选择系统的一种较为高效的实现 */
/*          这里默认是使用RingBuffer */
/*************************************************************************************************/
#if 1
/*************************************************************************************************/
#define RING_BUFFER_INDEX_MAX   (0x80000000)
/* 当SIZE==2**K时,MASK==SIZE-1,当SIZE!=2**K时,MASK==0 */
#define RING_BUFFER_TX_TYPE     (0)
#define RING_BUFFER_RX_TYPE     (1)
#define RING_BUFFER_TX_SIZE     (512)
#define RING_BUFFER_RX_SIZE     (512)
#define RING_BUFFER_TX_MASK     (512 - 1)
#define RING_BUFFER_RX_MASK     (512 - 1)
/*************************************************************************************************/
static uint32_t RingBufferTxHead = 0;
static uint32_t RingBufferRxHead = 0;
static uint32_t RingBufferTxTail = 0;
static uint32_t RingBufferRxTail = 0;
static uint8_t  RingBufferTxData[RING_BUFFER_TX_SIZE] = {0};
static uint8_t  RingBufferRxData[RING_BUFFER_RX_SIZE] = {0};
/*************************************************************************************************/
static inline void RingBuffer_Rewind(uint8_t Type)
{
    if (Type == RING_BUFFER_TX_TYPE) {
        if (RingBufferTxHead >= RING_BUFFER_INDEX_MAX) {
            uint32_t Rewind = RING_BUFFER_INDEX_MAX;
            Rewind /= RING_BUFFER_TX_SIZE;
            Rewind *= RING_BUFFER_TX_SIZE;
            RingBufferTxHead -= Rewind;
            RingBufferTxTail -= Rewind;
        }
    }
    if (Type == RING_BUFFER_RX_TYPE) {
        if (RingBufferRxHead >= RING_BUFFER_INDEX_MAX) {
            uint32_t Rewind = RING_BUFFER_INDEX_MAX;
            Rewind /= RING_BUFFER_RX_SIZE;
            Rewind *= RING_BUFFER_RX_SIZE;
            RingBufferRxHead -= Rewind;
            RingBufferRxTail -= Rewind;
        }
    }
}
/*************************************************************************************************/
static bool RingBuffer_GetData(uint8_t Type, uint8_t *Data, uint32_t Length)
{
    if (Type == RING_BUFFER_TX_TYPE) {
        if (RingBufferTxHead == RingBufferTxTail)
            return false;
        if (RingBufferTxTail - RingBufferTxHead < Length)
            return false;
        if (RING_BUFFER_TX_MASK != 0)
            for (uint32_t Index0 = 0; Index0 < Length, Index0++) {
                 uint32_t Index1 = (Index0 + RingBufferTxHead) & RING_BUFFER_TX_MASK;
                 Data[Index0] = RingBufferTxData[Index1];
            }
        if (RING_BUFFER_TX_MASK == 0)
            for (uint32_t Index0 = 0; Index0 < Length, Index0++) {
                 uint32_t Index1 = (Index0 + RingBufferTxHead) % RING_BUFFER_TX_SIZE;
                 Data[Index0] = RingBufferTxData[Index1];
            }
        RingBufferTxHead += Length;
        RingBuffer_Rewind(RING_BUFFER_TX_TYPE);
        return true;
    }
    if (Type == RING_BUFFER_RX_TYPE) {
        if (RingBufferRxHead == RingBufferRxTail)
            return false;
        if (RingBufferRxTail - RingBufferRxHead < Length)
            return false;
        if (RING_BUFFER_RX_MASK != 0)
            for (uint32_t Index0 = 0; Index0 < Length, Index0++) {
                 uint32_t Index1 = (Index0 + RingBufferRxHead) & RING_BUFFER_RX_MASK;
                 Data[Index0] = RingBufferRxData[Index1];
            }
        if (RING_BUFFER_RX_MASK == 0)
            for (uint32_t Index0 = 0; Index0 < Length, Index0++) {
                 uint32_t Index1 = (Index0 + RingBufferRxHead) % RING_BUFFER_RX_SIZE;
                 Data[Index0] = RingBufferRxData[Index1];
            }
        RingBufferRxHead += Length;
        RingBuffer_Rewind(RING_BUFFER_RX_TYPE);
        return true;
    }
    return false;
}
/*************************************************************************************************/
static bool RingBuffer_PutData(uint8_t Type, uint8_t *Data, uint32_t Length)
{
    if (Type == RING_BUFFER_TX_TYPE) {
        if (RING_BUFFER_TX_SIZE - (RingBufferTxTail - RingBufferTxHead) < Length)
            return false;
        if (RING_BUFFER_TX_MASK != 0)
            for (uint32_t Index0 = 0; Index0 < Length, Index0++) {
                 uint32_t Index1 = (Index0 + RingBufferTxTail) & RING_BUFFER_TX_MASK;
                 RingBufferTxData[Index1] = Data[Index0];
            }
        if (RING_BUFFER_TX_MASK == 0)
            for (uint32_t Index0 = 0; Index0 < Length, Index0++) {
                 uint32_t Index1 = (Index0 + RingBufferTxTail) % RING_BUFFER_TX_SIZE;
                 RingBufferTxData[Index1] = Data[Index0];
            }
        RingBufferTxTail += Length;
        RingBuffer_Rewind(RING_BUFFER_TX_TYPE);
        return true;
    }
    if (Type == RING_BUFFER_RX_TYPE) {
        if (RING_BUFFER_RX_SIZE - (RingBufferRxTail - RingBufferRxHead) < Length)
            return false;
        if (RING_BUFFER_RX_MASK != 0)
            for (uint32_t Index0 = 0; Index0 < Length, Index0++) {
                 uint32_t Index1 = (Index0 + RingBufferRxTail) & RING_BUFFER_RX_MASK;
                 RingBufferRxData[Index1] = Data[Index0];
            }
        if (RING_BUFFER_RX_MASK == 0)
            for (uint32_t Index0 = 0; Index0 < Length, Index0++) {
                 uint32_t Index1 = (Index0 + RingBufferRxTail) % RING_BUFFER_RX_SIZE;
                 RingBufferRxData[Index1] = Data[Index0];
            }
        RingBufferRxTail += Length;
        RingBuffer_Rewind(RING_BUFFER_RX_TYPE);
        return true;
    }
    return false;
}
/*************************************************************************************************/
static inline void RingBuffer_Reset(uint8_t Type)
{
    if (Type == RING_BUFFER_TX_TYPE) {
        RingBufferTxHead = 0;
        RingBufferTxTail = 0;
    }
    if (Type == RING_BUFFER_RX_TYPE) {
        RingBufferRxHead = 0;
        RingBufferRxTail = 0;
    }
}
/*************************************************************************************************/
#else
#error "Extend Level: Need Container"
#endif
/*************************************************************************************************/
/* 泛用容器转阶层:容器基本要求只需要满足FIFO式 */
/* 本层级存在的意义:应对高密集TX,RX时,如果传输过快会造成包遗漏 */
/*************************************************************************************************/
static inline bool TransferTx_GetData(uint8_t *Data, uint32_t Length)
{
    return RingBuffer_GetData(RING_BUFFER_TX_TYPE, Data, Length);
}
/*************************************************************************************************/
static inline bool TransferRx_GetData(uint8_t *Data, uint32_t Length)
{
    return RingBuffer_GetData(RING_BUFFER_RX_TYPE, Data, Length);
}
/*************************************************************************************************/
static inline bool TransferTx_PutData(uint8_t *Data, uint32_t Length)
{
    return RingBuffer_PutData(RING_BUFFER_TX_TYPE, Data, Length);
}
/*************************************************************************************************/
static inline bool TransferRx_PutData(uint8_t *Data, uint32_t Length)
{
    return RingBuffer_PutData(RING_BUFFER_RX_TYPE, Data, Length);
}
/*************************************************************************************************/
static inline void TransferTx_Reset(void)
{
    RingBuffer_Reset(RING_BUFFER_TX_TYPE);
}
/*************************************************************************************************/
static inline void TransferRx_Reset(void)
{
    RingBuffer_Reset(RING_BUFFER_RX_TYPE);
}
/*************************************************************************************************/
/* 第一层级:帧数据暂存空间以及链路传输层 */
#define TRANSFER_FRAME_TX_SIZE_MAX     255 + 4 + 1
#define TRANSFER_FRAME_RX_SIZE_MAX     255 + 4 + 1
/*************************************************************************************************/
static uint32_t TransferFrameTxSize = 0;
static uint32_t TransferFrameRxSize = 0;
static uint8_t  TransferFrameTxData[TRANSFER_FRAME_TX_SIZE_MAX] = {0};
static uint8_t  TransferFrameRxData[TRANSFER_FRAME_RX_SIZE_MAX] = {0};
/*************************************************************************************************/
static void GeneralDevice_DelayMs(uint32_t Overtime)
{
#if 0
    /* 将系统的延时函数转接到此处,如果有的话 */
    /* 因为系统的延时函数具备调度,不会空耗CPU占用,推荐使用 */
#else
    uint32_t Factor1 = 0;
    uint32_t Factor2 = 0;
    for (uint32_t K = 0; K < Overtime; K++)
    /* 下面俩级回调控制到1ms附近 */
    for (uint32_t I = 0; I < Factor1, I++)
    for (uint32_t J = 0; J < Factor2, J++)
        __asm__ __volatile__ ("nop");
#endif
}
/*************************************************************************************************/
static bool GeneralDevice_FrameTx(uint32_t UnitTime, uint32_t MaxCount, bool Wait)
{
    TransferFrameTxStatus = false;
 
    /* 将系统接口适配到此处 */
    /* 无论是DMA传输,是TX中断传输,还是TX阻塞传输 */
    /* 一共分为两个流程 */
    /* 1.将数据包从数据中心取出: */
    TransferTx_GetData(&TransferFrameTxSize, sizeof(TransferFrameTxSize));
    TransferTx_GetData( TransferFrameTxData,       (TransferFrameTxSize));
    /* 2.传输:(TransferFrameTxData,TransferFrameTxSize)到TX */
    /* 3.结束后:TransferFrameTxStatus == true,注意需要提供部分保护,如果是非中断环境 */
    
    if (Wait == true) {
        uint32_t Count = 0;
        /* 不使用超时机制(死等) */
        if (MaxCount == 0)
            while (TransferFrameTxStatus == false)
                GeneralDevice_DelayMs(UnitTime);
        /* 使用超时机制 */
        if (MaxCount != 0)
            while (TransferFrameTxStatus == false && Count++ < MaxCount)
                GeneralDevice_DelayMs(UnitTime);
    }
    /* 反馈帧状态 */
    return TransferFrameTxStatus;
}
/*************************************************************************************************/
static bool GeneralDevice_FrameRx(uint32_t UnitTime, uint32_t MaxCount, bool Wait)
{
    TransferFrameRxStatus = false;
    
    /* 将系统接口适配到此处 */
    /* 无论是DMA传输,是RX中断传输,还是RX阻塞传输 */
    /* 一共分为两个流程 */
    /* 1.传输:(TransferFrameRxData,TransferFrameRxSize)从RX */
    /* 2.将数据包向数据中心存入 */
    TransferRx_PutData(&TransferFrameRxSize, sizeof(TransferFrameRxSize));
    TransferRx_PutData( TransferFrameRxData,       (TransferFrameRxSize));
    /* 3.结束后:TransferFrameRxStatus == true */
    
    if (Wait == true) {
        uint32_t Count = 0;
        /* 不使用超时机制(死等) */
        if (MaxCount == 0)
            while (TransferFrameRxStatus == false)
                GeneralDevice_DelayMs(UnitTime);
        /* 使用超时机制 */
        if (MaxCount != 0)
            while (TransferFrameRxStatus == false && Count++ < MaxCount)
                GeneralDevice_DelayMs(UnitTime);
    }
    /* 反馈帧状态 */
    return TransferFrameRxStatus;
}
/*************************************************************************************************/
/* 第二层级:帧数据处理 */
#define FRAME_TX_SIZE_MAX     255
#define FRAME_RX_SIZE_MAX     255
/*************************************************************************************************/
static uint8_t  FrameTxCmd  = 0;
static uint8_t  FrameRxCmd  = 0;
static uint8_t  FrameTxSize = 0;
static uint8_t  FrameRxSize = 0;
static uint8_t *FrameTxData[FRAME_TX_SIZE_MAX] = {0};
static uint8_t *FrameRxData[FRAME_RX_SIZE_MAX] = {0};
/*************************************************************************************************/
/* 生成帧数据:TransferFrameTxData <== (FrameTxCmd,FrameTxSize,FrameTxData) */
/* 生成帧数据:TransferFrameTxSize <== (FrameTxCmd,FrameTxSize,FrameTxData) */
static bool GeneralDevice_FrameMake(void)
{
    /* 将(FrameTxCmd,FrameTxSize,FrameTxData)按照协议的布局方式 */
    /* 转化为等价的(TransferFrameTxData, TransferFrameTxSize) */
    /* 这一步主要是为帧添加包头,校验等固定信息 */
    
    /* 将数据包向数据中心存入 */
    TransferTx_PutData(&TransferFrameTxSize, sizeof(TransferFrameTxSize));
    TransferTx_PutData( TransferFrameTxData,       (TransferFrameTxSize));
}
/*************************************************************************************************/
/* 解析帧数据:TransferFrameRxData ==> (FrameRxCmd,FrameRxSize,FrameRxData) */
/* 解析帧数据:TransferFrameRxSize ==> (FrameRxCmd,FrameRxSize,FrameRxData) */
static bool GeneralDevice_FrameParse(void)
{
    /* 将数据包从数据中心取出: */
    TransferRx_GetData(&TransferFrameRxSize, sizeof(TransferFrameRxSize));
    TransferRx_GetData( TransferFrameRxData,       (TransferFrameRxSize));
 
    /* 将(TransferFrameRxData, TransferFrameRxSize)按照协议的布局方式 */
    /* 转化为等价的(FrameRxCmd,FrameRxSize,FrameRxData) */
    /* 这一步主要是为帧检查包头,校验等固定信息 */
}
/*************************************************************************************************/
/* 第三层级:帧数据转通配事件 */
#define EVENT_TX_SIZE_MAX_LENGTH    255
#define EVENT_RX_SIZE_MAX_LENGTH    255
/*************************************************************************************************/
static uint8_t  EventTxCmd  = 0;
static uint8_t  EventRxCmd  = 0;
static uint8_t  EventTxSize = 0;
static uint8_t  EventRxSize = 0;
static uint8_t *EventTxData[FRAME_TX_SIZE_MAX] = {0};
static uint8_t *EventRxData[FRAME_RX_SIZE_MAX] = {0};
/*************************************************************************************************/
/* (EventTxCmd,EventTxData,EventTxSize) <== (Event,Data,Size) */
static bool GeneralDevice_EventMake(uint32_t Event, uint8_t *Data, uint32_t Size)
{
    /* 将(Event,Data,Size)转化为等价的(EventTxCmd,EventTxData,EventTxSize) */
    /* 这一步实现的较好的话,可以还原协议中"不太重要的"信息,只让上层提供最关心的内容,定制化较高 */
    
    /* 注意:如果不需要定制封装,则可以直接越级跳跃: */
    /*      将将(Event,Data,Size)转化为等价的(FrameTxCmd,FrameTxSize,FrameTxData) */
    
    /* 固有动作:实现(EventTxCmd,EventTxData,EventTxSize)传输到 */
    /*              (FrameTxCmd,FrameTxSize,FrameTxData) */
}
/*************************************************************************************************/
/* (EventRxCmd,EventRxData,EventRxSize) ==> (Event,Data,Size) */
static bool GeneralDevice_EventParse(uint32_t *Event, uint8_t *Data, uint32_t *Size)
{
    /* 将(EventRxCmd, EventRxData, EventRxSize)转化为等价的(Event, Data, Size) */
    /* 这一步实现的较好的话,可以去除协议中"不太重要的"信息,只让上层得到最关心的内容,定制化较高 */
    
    /* 注意:如果不需要定制封装,则可以直接越级跳跃: */
    /*      将(FrameRxCmd,FrameRxSize,FrameRxData)转化为等价的(Event,Data,Size) */
    
    /* 固有动作:实现(FrameRxCmd,FrameRxSize,FrameRxData)传输到 */
    /*              (EventRxCmd,EventRxData,EventRxSize) */
}
/*************************************************************************************************/
bool GeneralDevice_Read(GeneralDevice_Event *Instance, uint32_t UnitTime, uint32_t MaxCount, bool Wait)
{
    /* 接收帧数据,并拆解帧数据 */
    if (GeneralDevice_FrameRx(UnitTime, MaxCount, Wait) == false)
        return false;
    if (GeneralDevice_FrameParse() == false)
        return false;
    if (GeneralDevice_EventParse(&(Instance->Event), (Instance->Data), &(Instance->Size)) == false)
        return false;
    return true;
}
/*************************************************************************************************/
bool GeneralDevice_Write(GeneralDevice_Event *Instance, uint32_t UnitTime, uint32_t MaxCount, bool Wait)
{
    if (GeneralDevice_EventMake((Instance->Event), (Instance->Data), (Instance->Size)) == false)
        return false;
    if (GeneralDevice_FrameMake() == false)
        return false;
    if (GeneralDevice_FrameTx(UnitTime, MaxCount, Wait) == false)
        return false;
    return true;
}
/*************************************************************************************************/
