
#ifndef __TIMER_H
#define __TIMER_H



#ifdef  __cplusplus
	extern "C" {
#endif




//初始化定时器
void TimersInit (void);

//添加1ms的定时器中断服务函数
u8 addTimerIrq1ms (void (*irq)(void));

//清除1ms定时器终端服务函数
u8 delTimerIrq1ms (void (*irq)(void));

//添加10ms的定时器中断服务函数
u8 addTimerIrq10ms (void (*irq)(void));

//清除10ms定时器终端服务函数
u8 delTimerIrq10ms (void (*irq)(void));

//添加10us定时器中断服务函数
u8 addTimerIrq10us (void (*irq)(void));

//清除10us定时器终端服务函数
u8 delTimerIrq10us (void (*irq)(void));

//设置定时器2的中断频率
void timer2_SetPeriod (u16 us);


#ifdef  __cplusplus
	}
#endif


#endif



