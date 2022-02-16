static FASTLZ_INLINE int FASTLZ_COMPRESSOR(const void* input, int length, void* output)
{
  const flzuint8* ip = (const flzuint8*) input; //字节流获取数据 ip = in put
  const flzuint8* ip_bound = ip + length - 2;	//压缩边界到最后俩字节前
  const flzuint8* ip_limit = ip + length - 12;	//压缩限制到最后12字节前
  flzuint8* op = (flzuint8*) output;			//输出地址重定向为字节流
  
  const flzuint8** hslot;						//简写hash slot
  flzuint32 hval;								//简写hash value

  flzuint32 copy;
												//动态创建哈希表，大小为8bit * 0x2000
  const flzuint8** htab = (const flzuint8**)malloc(sizeof(flzuint8*) * HASH_SIZE);
  if (htab == NULL) {							//它被用作滑动窗口缓冲区
    return -1;
  }

  if(FASTLZ_UNEXPECT_CONDITIONAL(length < 4)) { //length<4太短了，别压缩了
    if(length) {								//所以这里就是一个直接复制，没做压缩
      *op++ = length-1;							//第一字节保存 长度-1
      ip_bound++;								
      while(ip <= ip_bound)
        *op++ = *ip++;
      free(htab);
      return length+1;
    } else {
      free(htab);
      return 0;
    }
  }

  for (hslot = htab; hslot < htab + HASH_SIZE; hslot++)
    *hslot = ip;								//初始化哈希表，使其都指向输入input开始位置		
		
  copy = 2;										//已经直接拷贝俩个字节了（匹配失败次数）
  *op++ = MAX_COPY-1;							//输出的第一位保留之后的直接复制（匹配失败）信息
  *op++ = *ip++;								//前俩个字节不压缩，用来为后续做参考
  *op++ = *ip++;								//所以首俩个字母没有预留参考，无法压缩

												//开始主循环ip < ip_limit
  while(FASTLZ_EXPECT_CONDITIONAL(ip < ip_limit)) {
    const flzuint8* ref;						//缓冲区起始位置，当次有效
    flzuint32 distance;							//距离，位置
    flzuint32 len = 3;							//潜在的最小的匹配长度（一级匹配是3）
												//压缩本质是字符串匹配
												//将之前出现的重复字符串复用，以实现压缩
    const flzuint8* anchor = ip;				//实际定位到匹配字符串的首位置

	//备注1;
#if FASTLZ_LEVEL==2								//高压缩等级意味着匹配的字符串长度限制扩大
    if(ip[0] == ip[-1] && FASTLZ_READU16(ip-1)==FASTLZ_READU16(ip+1)) {
	  distance = 1;								//这里是为下一步做处理时做准备所用
	  
												//注意的是当前的ip的位置与前一字符相等
												//且FASTLZ_READU16是一次取俩个字符比较
												//那么当前之前的字符与现在的字符相等
												//那么当前之后的俩个字符如果也与自己相等
												//意思是连续四个字符已经相等(从-1开始四个字符)
												
      ip += 3;									//那么现在ip定位到这连续相等的四个字符之后
      ref = anchor - 1 + 3;						//成功比较的起始位置重定位到
												//刚才匹配到的最后一个字符之后
      goto match;								//进行匹配处理
    }
#endif

    HASH_FUNCTION(hval,ip);						//求当前位置以及之后俩个字符对应的哈希值
												//连续三个字符合在一起哈希值一样，绝大部分时间
												//可以保证这俩个前三字符相等，方便筛选绝大多数无用匹配
    hslot = htab + hval;						//单位到哈希表的对应位置
    ref = htab[hval];							//缓冲区重定位到哈希表的对应位置
    distance = anchor - ref;					//确认当前字符与之前匹配到的字符串之间的距离
												//这很重要，解压缩通过距离偏移才能找到相同的字符串
												//然后完成精准复制的动作
    *hslot = anchor;							//现在将哈希表对应位置的指针更新到ip位置

												//尝试查看潜在的匹配是否有效
    if(distance==0 || 							//字符串匹配长度不够长
#if FASTLZ_LEVEL==1
    (distance >= MAX_DISTANCE) ||				//字符串匹配程度太长	
#else
    (distance >= MAX_FARDISTANCE) ||			//字符串匹配程度太长
#endif											//一级匹配，至少前三个字符匹配到（做最后的确认）
    *ref++ != *ip++ || *ref++!=*ip++ || *ref++!=*ip++)	
      goto literal;								//潜在的匹配失败

#if FASTLZ_LEVEL==2
    if(distance >= MAX_DISTANCE) {				//二级匹配，至少五个字符匹配到
      if(*ip++ != *ref++ || *ip++!= *ref++) 	//从上面下来至少匹配了三个字符，再匹配俩个
        goto literal;							//潜在的匹配失败
      len += 2;
    }
												//上面进行最小值匹配可以快速筛选无效的匹配
    match:
#endif
												//目前至少已经满足最小值匹配的结果了
    ip = anchor + len;							//ip重定位到最小匹配之后，将来继续从这里匹配下去
	
    distance--;									//修正距离，因为俩个位置的距离本来还要+1补足的
												//上面不仅没做补足+1而且这里又要--
												//那么实际出现了一个2的差值，如果distance==0
												//所以0无效的情况下，又补足了一个差值，总计为3

    if(!distance) {								//注意，这里的处理是从备注1中的goto跳过来的
												//跳到上面是distance==1，然后做--动作之后
												//所以意味着这里的distance==0，取非为真
												
      flzuint8 x = ip[-1];						//因为备注1连续匹配到多个相同字符，所以这里
      while(ip < ip_bound)						//继续做匹配尝试，寻找最大的相同字符匹配长度
        if(*ref++ != x) break; 
			else ip++;
    } else for(;;) {
      if(*ref++ != *ip++) break;				//这里是从第二种情况下来的，即正常匹配
      if(*ref++ != *ip++) break;				//此时出现的一堆的同字母类似的，并不会太优
      if(*ref++ != *ip++) break;				//所以挨个比较
      if(*ref++ != *ip++) break;				//只有二级匹配时，考虑到字符重复的情况
      if(*ref++ != *ip++) break;				//因为一级匹配时，即使出现字符重复，那么下一次
      if(*ref++ != *ip++) break;				//的匹配利用率会更高，因为短的连续字符出现概率
      if(*ref++ != *ip++) break;				//会比长的连续字符出现概率更高
      if(*ref++ != *ip++) break;
      while(ip < ip_bound)
        if(*ref++ != *ip++) 
			break;
      break;
    }

    if(copy)									//直接复制数量，该字符串出现了1次复制
      *(op-copy-1) = copy-1;					//0代表1位复制，所以修正一下
    else
      op--;										//回退，因为直到现在op都没写（匹配成功）
												//那么op依然存在上一次的位置
												
    copy = 0;									//重置直接拷贝字符次数为0，因为到这里以后
												//一定是产生了匹配的了，不需要做直接拷贝

    ip -= 3;									//偏差修订，从数值1开始代表有3个字符匹配
	
    len = ip - anchor;							//长度修订，当前位置与之前匹配开始的位置差
												//（因为是字节流所以直接做差）

#if FASTLZ_LEVEL==2								//二级匹配
    if(distance < MAX_DISTANCE) {				//限制检查
      if(len < 7) {								//长度小于10个时？(上面有偏差修订)
        *op++ = (len << 5) + (distance >> 8);	//输出一个len与distance组合在一起的位的数
        *op++ = (distance & 255);				//len占据前3位，distance占据后5位
												//distance的后8位写到下一字节
												
      } else {									//第一字节写入一个111的标记表明len很长
        *op++ = (7 << 5) + (distance >> 8);		//第一字节为3位len标记和5位distance
        for(len-=7; len >= 255; len-= 255)		//第后面字节保留len实际长度
          *op++ = 255;							//最后字节写入distance后8位
        *op++ = len;
        *op++ = (distance & 255);
      }
	} else {									//因为是二级匹配，目前还算不上真正的溢出			
      if(len < 7) {								//参照上述的方式写入重复部分的长度(记录)
        distance -= MAX_DISTANCE;
        *op++ = (len << 5) + 31;
        *op++ = 255;
        *op++ = distance >> 8;
        *op++ = distance & 255;
      } else {
        distance -= MAX_DISTANCE;
        *op++ = (7 << 5) + 31;
        for(len-=7; len >= 255; len-= 255)
          *op++ = 255;
        *op++ = len;
        *op++ = 255;
        *op++ = distance >> 8;
        *op++ = distance & 255;
      }
    }
#else											//如果是一级匹配的话
    if(FASTLZ_UNEXPECT_CONDITIONAL(len > MAX_LEN-2))
      while(len > MAX_LEN-2) {					//如果len>MAX_LEN-2
        *op++ = (7 << 5) + (distance >> 8);		//前三位做len标记，但是也同时做部分长度
												//后面存实际len-7
        *op++ = MAX_LEN - 2 - 7 - 2; 			//len被限制在264以内，存储时做个小偏移
        *op++ = (distance & 255);				//存入distance后八位
        len -= MAX_LEN-2;						//循环只会进行一次
      }
    if(len < 7) {								//这里要跟着上面继续下来，继续存剩下的情况
      *op++ = (len << 5) + (distance >> 8);		//这里多次存储distance是因为
      *op++ = (distance & 255);					//解压缩时会把这个分成多次解压缩
    } else {									//而distance永远是以解压缩当前位置做的distance
      *op++ = (7 << 5) + (distance >> 8);		//所以保持了同步（精密！）
      *op++ = len - 7;							//偏移量扣除
      *op++ = (distance & 255);
    }
#endif
    HASH_FUNCTION(hval,ip);						//更新哈希表，输入需要进行重定位
    htab[hval] = ip++;							//因为之前已经匹配过了
    HASH_FUNCTION(hval,ip);						//所以下一次开始的三个字符
    htab[hval] = ip++;							//这里重新定位前俩个字符

    *op++ = MAX_COPY-1;							//复制成功
    continue;									//开启下一次匹配

    literal:									//字符串匹配失败
      *op++ = *anchor++;						//将缓冲区的首位移入到输出，表明以该字符为开始
      ip = anchor;								//最大可能是匹配不到对应的字符串的
      copy++;									//copy溢出时重置
      if(FASTLZ_UNEXPECT_CONDITIONAL(copy == MAX_COPY)){
        copy = 0;								//所以copy表示直接复制了多少个字符（匹配失败）
        *op++ = MAX_COPY-1;
      }
  }

  ip_bound++;
  while(ip <= ip_bound) {						//最后的几个字节直接拷贝即可
    *op++ = *ip++;								//直接拷贝
    copy++;
    if(copy == MAX_COPY) {						//一个单位字符串长度被控制为MAX_COPY
      copy = 0;									
      *op++ = MAX_COPY-1;						//每次读取这个标记就明白是直接COPY还是匹配压缩
    }
  }

  if(copy)										//做最后检查，上一次循环结束后还未
    *(op-copy-1) = copy-1;						//更新copy，将它写到之前，去替换MAX_COPY-1
  else											
    op--;										//忽略最后一个MAX_COPY-1，见上
												//if(copy == MAX_COPY)

#if FASTLZ_LEVEL==2
  *(flzuint8*)output |= (1 << 5);				//给最开始的第6位设置标签表明是二级匹配
#endif
  free(htab);									//哈希表释放
  return op - (flzuint8*)output;
}
