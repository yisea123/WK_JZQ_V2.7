#include "includes.h"
#include "iwdg.h"
#include "timer.h"
#include "beep.h"
#include "file.h"
#include "power.h"


#define SHCSR    (u32 *)0xE000ED24

void Sys_Init(void);






//系统开机
void SysPowerOn (void)
{
	Sys_Init();
	if (os_init()!=0) {// Load_up();
	}//不为0初始化失败，一般是文件系统失败
	RTC_Init();
	SPI_Flash_Init(); 
	
}

 

			
void SysPowerOff (void)
{
	delay_us(1000);
	NVIC_SystemReset();
}









void Sys_Init(void)
{
	*SHCSR|=7<<16;
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x2800);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	TimersInit();
	BEEP_Init();
	
	
}	
 










