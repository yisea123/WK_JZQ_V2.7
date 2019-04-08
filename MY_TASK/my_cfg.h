
#ifndef __MY_CFG_H
#define __MY_CFG_H


#ifdef __cplusplus
	extern "C" {
#endif


//配置功能任务函数
void my_cfg (void * t);

//进入配置模式
void into_cfg(void);
		
//退出配置模式
void exit_cfg(void);

//控制配置模式的消息
void cfg_msg (u8 *msg);
		
//无线配置
void rf_cfg (u8 *rf_recv);
		
u8 cfg_getstate (void);

		
#ifdef __cplusplus
	}
#endif
		
		

#endif









