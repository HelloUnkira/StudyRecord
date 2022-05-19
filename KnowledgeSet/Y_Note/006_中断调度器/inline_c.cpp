//cpp中内联c的方法

//在c头文件中
//将需要以c编译的代码用以下段包含

#ifdef __cplusplus
	extern "C" {
#endif

/* 你的将要使用c编写的代码 */

#ifdef __cplusplus
	}
#endif