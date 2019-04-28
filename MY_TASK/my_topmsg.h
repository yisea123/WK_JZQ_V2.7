#ifndef MY_TOPMSG_H__
#define MY_TOPMSG_H__


#ifdef __cplusplus
	extern "C" {
#endif



		
		
		
		
		

//顶级消息分发任务
void my_topmsg (void *);


/*****************供外部任务调用的API********************/
		
//添加软件定时器
u8 addSoftTimerIrq10ms (void (*irq)(void));
		
//移除指定软件定时器
u8 delSoftTimerIrq10ms (void (*irq)(void));

//加载默认配置信息
void load_test_cfg(void);



/***************供外部任务调用的API End******************/
		
		
		
		
#ifdef __cplusplus
	}
#endif


#endif











