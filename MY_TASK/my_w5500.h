
#ifndef __MY_W5500_H
#define __MY_W5500_H



#ifdef __cplusplus
	extern "C" {
#endif






void my_w5500 (void * t);

//调试线程
void my_debug_task ( void *t);


				//温控客户端函数接口
void wk_client(void);



#ifdef __cplusplus
	}
#endif




#endif














