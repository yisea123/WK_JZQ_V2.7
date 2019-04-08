#include "includes.h"
#include "timer.h"


//10ms��ʱ��
void TIME4_Init(void);

//��ʱ��3��������PWM
void TIME3_Init(void);

//10us��ʱ��
void TIME2_Init(void);



static void (*IrqBy10ms[10])(void)={0};
static void (*IrqBy10us[1])(void)={0};



void TimersInit (void)
{
	TIME2_Init();
	TIME3_Init();
	TIME4_Init();
}


//���10ms�Ķ�ʱ���жϷ�����
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

//���10ms��ʱ���ն˷�����
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




//���10us��ʱ���жϷ�����
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

//���10us��ʱ���ն˷�����
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








void TIME2_Init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM4��ʼ��
	TIM_TimeBaseStructure.TIM_Period = 10-1; //10us
	TIM_TimeBaseStructure.TIM_Prescaler =72-1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx		
}









#define hz 10000
void TIME3_Init(void)
{
	u16 arr=256-1;//����������ز�Ƶ�ʼ������Ƶϵ��
	u16 psc=72000000/(hz)/256-1;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	
	
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //10us
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���



	//ͨ��4 ��ɫ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //�������ģʽ
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //�Զ�����

	//ͨ��3 ��ɫ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //�������ģʽ
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //�Զ�����


	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx		
}






void TIME4_Init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM4��ʼ��
	TIM_TimeBaseStructure.TIM_Period = 100-1; //10ms
	TIM_TimeBaseStructure.TIM_Prescaler =7200-1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx		
}













void TIM2_IRQHandler(void)   //TIM2�ж�
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		BX_VOID_FUN(IrqBy10us[0]);

	}
}





void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 

	}
}





void TIM4_IRQHandler(void)   //TIM4�ж�
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
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









