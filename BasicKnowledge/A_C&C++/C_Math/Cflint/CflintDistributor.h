#ifndef CFLINT_DISTRIBUTOR_H
#define CFLINT_DISTRIBUTOR_H
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#if 1
static inline void * Cflint_Allocate(uint8_t UnitSize, uint32_t Length)
{
    return malloc(UnitSize * Length);
}
static inline void Cflint_Free(void * Pointer)
{
    free(Pointer);
}
#else
static inline void * Cflint_Allocate(uint8_t UnitSize, uint32_t Length)
{
    return NULL;
}
static inline void Cflint_Free(void * Pointer)
{
}
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
