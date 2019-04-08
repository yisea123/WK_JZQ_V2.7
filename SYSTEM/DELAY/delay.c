#include "includes.h"
#include "delay.h"






static u8  fac_us=0;//us��ʱ������
static u16 fac_ms=0;//ms��ʱ������

#define  MICRO_MS  2//5�����ж�һ��


//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void delay_init()	 
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
	fac_us=SystemCoreClock/8000000;	//Ϊϵͳʱ�ӵ�1/8  
	fac_ms=(u16)fac_us*1000;//��ucos��,����ÿ��ms��Ҫ��systickʱ����   
	
	SysTick->LOAD=(u32)MICRO_MS*fac_ms;//ʱ�����(SysTick->LOADΪ24bit)

	SysTick->CTRL|=3;//ʹ�ܼ������������ж�
}								    
	

//����MICRO_MS����ʱ����������γ���ѭ��������MICRO_MS����ʱ��ʹ��delay_ms
//
void delay_us(u32 nus)
{
	u32 old,now;
	old=SysTick->VAL;
	while (1)
	{
		now=SysTick->VAL;
		if (old>now)
		{
			if (((old-now)/fac_us)>=nus)//�ݼ����������ʾ�ֵ����ֵ
				break;
		}
		else 
		{
			if (((old+(MICRO_MS*fac_ms)-now)/fac_us)>=nus)
				break;
		}
	}
}

u32 delay_ms(u16 nms)
{	 
 #if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		 OS_CPU_SR  cpu_sr;
 #endif
	OS_ENTER_CRITICAL(); 
	TCB_Table[OSPrioHighRdy].MYDelay_ms=nms/MICRO_MS+1;//������ʱ���������ý���
	OS_EXIT_CRITICAL();
	return TaskGetMsg();
} 


//ǿ��˯�ߣ�����ǰ���ܱ�����
u32 sleep_ms(u16 nms)
{	 
 #if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		 OS_CPU_SR  cpu_sr;
 #endif
	u32 msg;
	OS_ENTER_CRITICAL(); 
	TCB_Table[OSPrioHighRdy].MYDelay_ms=nms/MICRO_MS+1;//������ʱ���������ý���
	OS_EXIT_CRITICAL();
	do{
		msg|=TaskGetMsg();
	}while((msg&DELAY_END)==0);
} 






//ϵͳ����ʱ�䣬��λ��
static u32 SYS_RUNTIME=0;

void RunTime_IRQHandler (void)
{
	static u16 micro_ms_t=0;//��Сʱ��Ƭ����
	micro_ms_t++;
	if (MICRO_MS*micro_ms_t>=1000)
	{
		micro_ms_t=0;
		SYS_RUNTIME++;
	}
}

//��ȡϵͳ����ʱ��
u32 getSysRunTime(void)
{
	return SYS_RUNTIME;
}



//	��ֹ���ֳ�ʱ��ռ��CPU����������²�
void cpuBreakIRQ (void)
{
	static u16 cpu=0;
	extern u8 OS_ONLYME;
	if (OS_ONLYME)
	{
		cpu++;
		if (cpu>=500)
		{
			cpu=0;
			OS_EXIT_ONLYME();
		}
	}
	else
	{
		cpu=0;
	}	
}






void SysTick_Handler (void)
{
	u8 i=0;
	for(i=0;i<32;i++)
	{
		if (TCB_Table[i].MYDelay_ms>0) 
		{
			TCB_Table[i].MYDelay_ms--;
			if (TCB_Table[i].MYDelay_ms==0)
			{ 
				TaskIntSendMsg(i,DELAY_END);//�ڼ�����0ʱ����һ��������
			}
		}
	}
	RunTime_IRQHandler();
	CheckTaskUsege();
	//cpuBreakIRQ();
}




























