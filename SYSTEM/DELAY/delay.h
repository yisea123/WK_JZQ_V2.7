#ifndef __DELAY_H
#define __DELAY_H 			   


#ifdef __cplusplus
	extern "C" {
#endif



void delay_init(void);
u32 delay_ms(u16 nms);
u32 sleep_ms(u16 nms);

void delay_us(u32 nus);


//获取系统运行时间
u32 getSysRunTime(void);



#ifdef __cplusplus
	}
#endif




#endif





























