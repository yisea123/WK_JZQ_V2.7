#include "includes.h"
#include "timer.h"



/***********************************************************************

				文件说明：定时器初始化
				文件功能：向外部提供PWM输出，周期为10us的定时器中断，周期为10ms的定时器中断
				中断优先级：0,0；3,2

***********************************************************************/

/**************************局部函数定义******************************/

//10ms计时器
static void TIME4_Init(void);

//定时器3输出调光的PWM
static void TIME3_Init(void);

//10us定时器
static void TIME2_Init(void);

/*************************局部函数定义End*****************************/

static void (*IrqBy10ms[10])(void)={0};
static void (*IrqBy10us[1])(void)={0};



void TimersInit (void)
{
	TIME2_Init();
	TIME3_Init();
	TIME4_Init();
}


//添加10ms的定时器中断服务函数
u8 addTimerIrq10ms (void (*irq)(void))
{
	for (u8 i=0;i<10;i++)
	{
		if (IrqBy10ms[i]==0)
		{
			IrqBy10ms[i]=irq;
			return 0;
		}
	}
	return 1;
}

//清除10ms定时器终端服务函数
u8 delTimerIrq10ms (void (*irq)(void))
{
	for (u8 i=0;i<10;i++)
	{
		if (IrqBy10ms[i]==irq)
		{
			IrqBy10ms[i]=0;
			return 0;
		}
	}
	return 1;
}




//添加10us定时器中断服务函数
u8 addTimerIrq10us (void (*irq)(void))
{
	for (u8 i=0;i<1;i++)
	{
		if (IrqBy10us[i]==0)
		{
			IrqBy10us[i]=irq;
			return 0;
		}
	}
	return 1;
}

//清除10us定时器终端服务函数
u8 delTimerIrq10us (void (*irq)(void))
{
	for (u8 i=0;i<1;i++)
	{
		if (IrqBy10us[i]==irq)
		{
			IrqBy10us[i]=0;
			return 0;
		}
	}
	return 1;
}




/*****************************************************

			10us的定时器中断，优先级设置为最高，抢占优先级为0，从优先级也为0

*****************************************************/
void TIME2_Init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	//定时器TIM4初始化
	TIM_TimeBaseStructure.TIM_Period = 20-1; //10us
	TIM_TimeBaseStructure.TIM_Prescaler =72-1; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM2, ENABLE);  //使能TIMx		
}






/************************************************

	定时器3用于产生PWM脉冲，不分配中断

*************************************************/
#define hz 10000
void TIME3_Init(void)
{
	u16 arr=256-1;//根据需求的载波频率计算出分频系数
	u16 psc=72000000/(hz)/256-1;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	
	
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //10us
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级0级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
//	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器



	//通道4 红色
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //中央对齐模式
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //自动重载

	//通道3 绿色
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //中央对齐模式
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //自动重载


	TIM_Cmd(TIM3, ENABLE);  //使能TIMx		
}




/*************************************************

				定时器4用于产生10ms的周期定时，分配较低的优先级

***********************************************/

void TIME4_Init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	
	//定时器TIM4初始化
	TIM_TimeBaseStructure.TIM_Period = 100-1; //10ms
	TIM_TimeBaseStructure.TIM_Prescaler =7200-1; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM4, ENABLE);  //使能TIMx		
}













void TIM2_IRQHandler(void)   //TIM2中断
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx更新中断标志 
		BX_VOID_FUN(IrqBy10us[0]);

	}
}





void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 

	}
}





void TIM4_IRQHandler(void)   //TIM4中断
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx更新中断标志 
		BX_VOID_FUN(IrqBy10ms[0]);
		BX_VOID_FUN(IrqBy10ms[1]);
		BX_VOID_FUN(IrqBy10ms[2]);
		BX_VOID_FUN(IrqBy10ms[3]);
		BX_VOID_FUN(IrqBy10ms[4]);
		BX_VOID_FUN(IrqBy10ms[5]);
		BX_VOID_FUN(IrqBy10ms[6]);
		BX_VOID_FUN(IrqBy10ms[7]);
		BX_VOID_FUN(IrqBy10ms[8]);
		BX_VOID_FUN(IrqBy10ms[9]);
	}
}









