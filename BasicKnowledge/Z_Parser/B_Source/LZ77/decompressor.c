static FASTLZ_INLINE int FASTLZ_DECOMPRESSOR(const void* input, int length, void* output, int maxout)
{												//输入的已压缩的字节流包
  const flzuint8* ip = (const flzuint8*) input;
  const flzuint8* ip_limit  = ip + length;		//压缩包大小，从头到尾，不像之前的配置
  flzuint8* op = (flzuint8*) output;			//重定向输出流
  flzuint8* op_limit = op + maxout;				//输出限制到这个位置
  flzuint32 ctrl = (*ip++) & 31;				//取压缩的第一个字节，它的低5位保留了MAX_COPY信息
												//注意这里，将上面的LEVEL标签给屏蔽掉了
												//LEVEL与解压缩无关，因为宏定义在预处理中已经结束
												//它只用于函数调用的选择时使用
  int loop = 1;

  do {
    const flzuint8* ref = op;					//滑动缓冲区被定为到当前输出位置
												//跟compress相反，通过当前位置以及偏移量
												//我可以确定拷贝了之前从哪部分开始的字符串

    flzuint32 len = ctrl >> 5;					//取出潜在成功匹配的匹配长度，的前一部分
    flzuint32 ofs = (ctrl & 31) << 8;			//获取偏移量distance的，存在潜在成功匹配组合中
												//注意这里的细节，压缩时用到的字节方式存储
												//而这里解压时一次性读取了四个字节

    if(ctrl >= 32) {							//这里表明不是直接COPY
												//因为在compress中确定直接COPY一次只允许
												//MAX_COPY-1长度的字符串
												
#if FASTLZ_LEVEL==2
      flzuint8 code;							//
#endif

      len--;									//偏移校准
      ref -= ofs;								//缓冲区定位到压缩时匹配到的那个范围
	  
      if (len == 7-1)							//len取出来如果是111，那么现在是110==6
												//标记位，说明后面的字节是len实际长度
												//len以255（8个1）为中间界限
												//遇到的第一个非255的数为len的结尾
												//后面紧接着distance的后八位
												
#if FASTLZ_LEVEL==1								//如果是一级匹配的话
        len += *ip++;							//那么len就只有一个字节的大小
      ref -= *ip++;								//后面的distance做最后修订
#else
        do {									//如果是二级匹配的话
          code = *ip++;							//循环取出len的长度
          len += code;
        } while (code==255);
      code = *ip++;
      ref -= code;								//缓冲区起始回退到对应位置（distance后8位）

												//这里是FarDistance的情况
      if(FASTLZ_UNEXPECT_CONDITIONAL(code==255))//因为FarDistance存储完len后不是紧接着存储
												//distance后八位，而是存储一个255标记
												//而且原有的distance高5位是直接替换为11111
												//的一个标记，而不是distance的一部分
      if(FASTLZ_EXPECT_CONDITIONAL(ofs==(31 << 8)))
      {											//这里判断高5位是不是为0x1f00（标记）
        ofs = (*ip++) << 8;						//取16位distance
        ofs += *ip++;							
        ref = op - ofs - MAX_DISTANCE;			//重定向缓冲区的起始位置
      }
#endif
      
#ifdef FASTLZ_SAFE
												//安全检查，输出长度超出限制
      if (FASTLZ_UNEXPECT_CONDITIONAL(op + len + 3 > op_limit))
        return 0;
												//错误的前项偏移，缓冲区滑到外太空去了
      if (FASTLZ_UNEXPECT_CONDITIONAL(ref-1 < (flzuint8 *)output))
        return 0;
#endif
												//满足正常的情况下
      if(FASTLZ_EXPECT_CONDITIONAL(ip < ip_limit))
        ctrl = *ip++;							//取下一个控制命令，因为是匹配成功
												//所以后面不存在实际数据，实际数据
												//在之前缓冲区里面，后面会拷贝出来
												//即使是直接COPY，那也是控制命令在前（COPY的字节数）
      else							
        loop = 0;								//出现错误，终止下一次循环
												

      if(ref == op){							//这里对应的是压缩的第一种情况(压缩中的备注1)        
        flzuint8 b = ref[-1];					//表明后续的字符都是相同的连续字符
        *op++ = b;								//先拷贝三个，保证四个都相同
        *op++ = b;
        *op++ = b;
        for(; len; --len)						//再拷贝剩下的
          *op++ = b;
      } else {									//这里是第二种压缩情况，是滑动窗口内容的重复
#if !defined(FASTLZ_STRICT_ALIGN)				//地址8位限制不存在
        const flzuint16* p;
        flzuint16* q;
#endif
        /* copy from reference */
        ref--;									//滑动窗口偏移，因为之前是挪到首地址之后
												//为了做第一种情况的检查
        *op++ = *ref++;							//滑动窗口先拷贝三个字符
        *op++ = *ref++;
        *op++ = *ref++;

#if !defined(FASTLZ_STRICT_ALIGN)				//限制不存在
        if(len & 1) {							//先拷贝一个字符，做一个四字节对齐（精准）
          *op++ = *ref++;
          len--;
        }

        q = (flzuint16*) op;
        op += len;								//输出位置提前偏移到拷贝后的位置
        p = (const flzuint16*) ref;				//从滑动窗口做拷贝
        for(len>>=1; len > 4; len-=4) {       				
          *q++ = *p++;							//所以这里就是一次性拷贝4字节的大小
          *q++ = *p++;							//如果是直接unit32的话一次赋值拷贝就行了
          *q++ = *p++;							//但事实上这种动作取决于计算机硬件总线
          *q++ = *p++;							//一次性能承载的数据量是多少位
        }										//编译器在这部分也会对其优化到上述情况
												//同样的，字节对齐也会提升Cache命中率
												
        for(; len; --len)						//再拷贝剩下的非对齐的部分
          *q++ = *p++;
#else
        for(; len; --len)						//出现8位限制，所以以字节流拷贝
          *op++ = *ref++;
#endif
      }
    } else {									//直接复制的情况下
      ctrl++;
#ifdef FASTLZ_SAFE								//安全检查，输出长度超出限制
      if (FASTLZ_UNEXPECT_CONDITIONAL(op + ctrl > op_limit))
        return 0;
      if (FASTLZ_UNEXPECT_CONDITIONAL(ip + ctrl > ip_limit))
        return 0;
#endif
      *op++ = *ip++; 							//直接拷贝
      for(--ctrl; ctrl; ctrl--)
        *op++ = *ip++;
												//安全检查，出错终止下一次循环
      loop = FASTLZ_EXPECT_CONDITIONAL(ip < ip_limit);
      if(loop)									//取下一个命令字
        ctrl = *ip++;
    }													
  } while(FASTLZ_EXPECT_CONDITIONAL(loop));		//继续循环解压剩余部分

  return op - (flzuint8*)output;
}

