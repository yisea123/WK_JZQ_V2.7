#include "includes.h"
#include "iwdg.h"
#include "timer.h"
#include "beep.h"
#include "file.h"
#include "power.h"


#define SHCSR    (u32 *)0xE000ED24

void Sys_Init(void);






//ϵͳ����
void SysPowerOn (void)
{
	Sys_Init();
	if (os_init()!=0) {// Load_up();
	}//��Ϊ0��ʼ��ʧ�ܣ�һ�����ļ�ϵͳʧ��
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	TimersInit();
	BEEP_Init();
	
	
}	
 










