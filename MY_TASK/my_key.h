#ifndef __MY_KEY_H
#define __MY_KEY_H


#ifdef __cplusplus
	extern "C" {
#endif




//按键任务函数
void my_key (void *);

//获取按键状态
u8 get_keystate(u8 key_num);


#ifdef __cplusplus
	}
#endif


#endif


