
#ifndef __MY_W5500_H
#define __MY_W5500_H



#ifdef __cplusplus
	extern "C" {
#endif






void my_w5500 (void * t);

					//处理网口中断的任务函数
void my_w5500interrupt (void * t);


				//温控客户端函数接口
void wk_client(void);



#ifdef __cplusplus
	}
#endif




#endif














