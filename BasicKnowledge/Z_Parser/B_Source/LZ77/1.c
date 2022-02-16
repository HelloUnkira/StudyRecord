#include <stdio.h>
#include <stdlib.h>


//安全检查，绑定
#define FASTLZ_SAFE

//编译器预测优化，无优化这里
#define FASTLZ_EXPECT_CONDITIONAL(c)    (c)
#define FASTLZ_UNEXPECT_CONDITIONAL(c)  (c)


//内联函数，提升程序运行效率
#define FASTLZ_INLINE __inline //内联符号

//8位限制取消
#define FASTLZ_STRICT_ALIGN
#undef FASTLZ_STRICT_ALIGN


//重声明本文件使用的字节对齐规则为无符号位
typedef unsigned char  flzuint8;
typedef unsigned short flzuint16;
typedef unsigned int   flzuint32;

//函数原型
int fastlz_compress(const void* input, int length, void* output);
int fastlz_compress_level(int level, const void* input, int length, void* output);
int fastlz_decompress(const void* input, int length, void* output, int maxout);

#define MAX_COPY       32
#define MAX_LEN       264  /* 256 + 8 */
#define MAX_DISTANCE 8192

//因为上面取消限制，所以可以使用多8位的数据单元，这里是16位地址
#define FASTLZ_READU16(p) *((const flzuint16*)(p)) 


#define HASH_LOG  13
#define HASH_SIZE (1<< HASH_LOG) //0x2000
#define HASH_MASK  (HASH_SIZE-1) //0x1FFF
#define HASH_FUNCTION(v,p) \
{\
	v = FASTLZ_READU16(p);\
	v ^= FASTLZ_READU16(p+1)^(v>>(16-HASH_LOG));\
	v &= HASH_MASK;\
	// v  = *(const flzuint16*)(p);
	// v ^= *(const flzuint16*)(p+1)^(v>>3);
	// v &= 0x1FFF;	
}//这是一个散列函数，散列函数的意味是尽可能的均匀到下标

#undef FASTLZ_LEVEL
#define FASTLZ_LEVEL 1

#undef FASTLZ_COMPRESSOR
#undef FASTLZ_DECOMPRESSOR
#define FASTLZ_COMPRESSOR fastlz1_compress
#define FASTLZ_DECOMPRESSOR fastlz1_decompress
static FASTLZ_INLINE int FASTLZ_COMPRESSOR(const void* input, int length, void* output);
static FASTLZ_INLINE int FASTLZ_DECOMPRESSOR(const void* input, int length, void* output, int maxout);
#include "fastlz.c"	//这里重包含自己，因为恰好上面的俩个宏定义，这部分内容被替换为2.c的内容
//那么替换的规则会重定向为对应的动作，参照现在之前的来

//那么下面又重新更变了规则，所以最终下面的重包含替换就按下面的规则来
#undef FASTLZ_LEVEL
#define FASTLZ_LEVEL 2

#undef MAX_DISTANCE
#define MAX_DISTANCE 8191
#define MAX_FARDISTANCE (65535+MAX_DISTANCE-1)

#undef FASTLZ_COMPRESSOR
#undef FASTLZ_DECOMPRESSOR
#define FASTLZ_COMPRESSOR fastlz2_compress
#define FASTLZ_DECOMPRESSOR fastlz2_decompress
static FASTLZ_INLINE int FASTLZ_COMPRESSOR(const void* input, int length, void* output);
static FASTLZ_INLINE int FASTLZ_DECOMPRESSOR(const void* input, int length, void* output, int maxout);
#include "fastlz.c"//这里重包含自己，因为恰好上面的俩个宏定义，这部分内容被替换为2.c的内容

//最终文件预处理后会变成0.c的内容
//但那是整理后的，仅仅适用于当前工程环境下


int fastlz_compress(const void* input, int length, void* output)
{
  /* for short block, choose fastlz1 */
  if(length < 65536)
    return fastlz1_compress(input, length, output);

  /* else... */
  return fastlz2_compress(input, length, output);
}

int fastlz_decompress(const void* input, int length, void* output, int maxout)
{
  /* magic identifier for compression level */
  int level = ((*(const flzuint8*)input) >> 5) + 1;

  if(level == 1)
    return fastlz1_decompress(input, length, output, maxout);
  if(level == 2)
    return fastlz2_decompress(input, length, output, maxout);

  /* unknown level, trigger error */
  return 0;
}

int fastlz_compress_level(int level, const void* input, int length, void* output)
{
  if(level == 1)
    return fastlz1_compress(input, length, output);
  if(level == 2)
    return fastlz2_compress(input, length, output);

  return 0;
}