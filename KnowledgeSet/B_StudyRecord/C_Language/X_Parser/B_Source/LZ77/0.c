#include <stdio.h>
#include <stdlib.h>

//安全检查，绑定
#define FASTLZ_SAFE

//重声明本文件使用的字节对齐规则为无符号位
typedef unsigned char  flzuint8;
typedef unsigned short flzuint16;
typedef unsigned int   flzuint32;

//函数原型
int fastlz_compress(const void* input, int length, void* output);
int fastlz_compress_level(int level, const void* input, int length, void* output);
int fastlz_decompress(const void* input, int length, void* output, int maxout);

//实际的实现接口
static __inline int fastlz1_compress(const void* input, int length, void* output);
static __inline int fastlz1_decompress(const void* input, int length, void* output, int maxout);
static __inline int fastlz2_compress(const void* input, int length, void* output);
static __inline int fastlz2_decompress(const void* input, int length, void* output, int maxout);

int fastlz_compress(const void* input, int length, void* output)
{
  return (length < 65536 ? \
	fastlz1_compress(input, length, output):fastlz2_compress(input, length, output));
}

int fastlz_decompress(const void* input, int length, void* output, int maxout)
{//查看输入提供的level信息
  int level = ((*(const flzuint8*)input) >> 5) + 1;
  switch (level) {//这样子也挺好看的，
    case  1: return fastlz1_decompress(input, length, output, maxout);
	case  2: return fastlz2_decompress(input, length, output, maxout);
	default: return 0;
  }
}

int fastlz_compress_level(int level, const void* input, int length, void* output)
{
  switch (level) {
    case  1: return fastlz1_decompress(input, length, output, maxout);
	case  2: return fastlz2_decompress(input, length, output, maxout);
	default: return 0;
  }
}

//因为上面取消限制，所以可以使用多8位的数据单元，这里是16位地址
#define FASTLZ_READU16(p) *((const flzuint16*)(p)) 
//这是一个散列函数，散列函数的意味是尽可能的均匀到下标
#define HASH_LOG  13
#define HASH_SIZE (1<< HASH_LOG) //0x2000
#define HASH_MASK  (HASH_SIZE-1) //0x1FFF
#define HASH_FUNCTION(v,p) \
{/*[v = *(const flzuint16*)(p);] */\
	v = FASTLZ_READU16(p);\
 /*[v ^= *(const flzuint16*)(p+1)^(v>>3);]*/\
	v ^= FASTLZ_READU16(p+1)^(v>>(16-HASH_LOG));\
	v &= HASH_MASK;\
 /*[v &= 0x1FFF;]*/}

//数据的限制情况
#define MAX_COPY       32
#define MAX_LEN       264  /* 256 + 8 */


#define MAX_DISTANCE 8192

static __inline int fastlz1_compress(const void* input, int length, void* output)
{
	//……
}

static __inline int fastlz1_decompress(const void* input, int length, void* output, int maxout)
{
	//……
}


#undef MAX_DISTANCE
#define MAX_DISTANCE 8191
#define MAX_FARDISTANCE (65535+MAX_DISTANCE-1)

static __inline int fastlz2_compress(const void* input, int length, void* output)
{
	//……
}

static __inline int fastlz2_decompress(const void* input, int length, void* output, int maxout)
{
	//……
}

