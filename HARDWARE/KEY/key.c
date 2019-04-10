#include "includes.h"
#include "timer.h"
#include "hard_irq.h"
#include "key.h"




/**************************************************
		�ļ�˵������������
		����ʹ�ã�PC0��PC1��PC2��PC3
		�ⲿ���ã�10ms�Ķ�ʱ���ж�
		ʵ�ַ�����ʹ����������ɨ���ⰴ����ʹ�ö�ʱ���ж�ʵ�֣�
		ʹ�÷�������������Ҫ�ⲿ����һ��10ms�Ķ�ʱ���жϷ���������洢���һ�ΰ��µİ���ֵ��

****************************************************/





#ifndef JZQ_V2_3




//�������飬֧�ֶ������ͬʱ����
static u8 KEYS[6];

void Key_Init (void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	
	//��ʼ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOC, &GPIO_InitStructure); //�°��Ӳ�ʹ����ǰ�İ����ƹ�2018.11.5����4��IO��������������

	addTimerIrq10ms(KEY_IRQHandler);//�ڶ�ʱ���������жϷ�����
}



//���ذ���״̬
u8 Get_Key(u8 keynum)
{
	u8 key_ret;
	key_ret=KEYS[keynum-1];
	KEYS[keynum-1]=PRESS_NONE;
	return key_ret;
}


//������Ч
#define KEY_Effective (KEYS[0]||KEYS[1]||KEYS[2]||KEYS[3]||KEYS[4]||KEYS[5])



//�����жϷ������
void KEY_IRQHandler(void)   //
{
	static u8 key_time[6];
	static u8 key_valid[6];
	u8 keys=0xff;//������ֵ��0xffΪ�ް�������
	keys=GPIO_ReadInputData(GPIOC)&0x0f;//��ȡ������Ϣ
//KEY1																																				
		if (keys==0)
		{
			if (key_valid[0]==0)//������Ч
			{
				KEYS[0]=PRESS_DOWN;
				key_time[0]++;
				if (key_time[0]>=80)
				{
					KEYS[0]=PRESS_LONG;
					key_valid[0]=1;//����������������Ч
				}
			}
		}
		else
		{
			if (key_time[0]>1&&key_time[0]<80)
			{
				KEYS[0]=PRESS_SHORT;
			}
			key_time[0]=0;
			key_valid[0]=0;//������Ϊ��Ч
		}
//KEY2																																				
		if (keys==1)
		{
			if (key_valid[1]==0)//������Ч
			{
				KEYS[1]=PRESS_DOWN;
				key_time[1]++;
				if (key_time[1]>=80)
				{
					KEYS[1]=PRESS_LONG;
					key_valid[1]=1;//����������������Ч
				}
			}
		}
		else
		{
		if (key_time[1]>1&&key_time[1]<80)
			{
				KEYS[1]=PRESS_SHORT;
			}
			key_time[1]=0;
			key_valid[1]=0;
		}
//KEY3																																				
		if (keys==2)
		{
			if (key_valid[2]==0)//������Ч
			{
				KEYS[2]=PRESS_DOWN;
				key_time[2]++;
				if (key_time[2]>=80)
				{
					KEYS[2]=PRESS_LONG;
					key_valid[2]=1;//����������������Ч
				}
			}
		}
		else
		{
		if (key_time[2]>1&&key_time[2]<80)
			{
				KEYS[2]=PRESS_SHORT;
			}
			key_time[2]=0;
			key_valid[2]=0;
		}
//KEY4																																				
		if (keys==3)
		{
			if (key_valid[3]==0)//������Ч
			{
				KEYS[3]=PRESS_DOWN;
				key_time[3]++;
				if (key_time[3]>=80)
				{
					KEYS[3]=PRESS_LONG;
					key_valid[3]=1;//����������������Ч
				}
			}
		}
		else
		{
			if (key_time[3]>1&&key_time[3]<80)
			{
				KEYS[3]=PRESS_SHORT;
			}
			key_time[3]=0;
			key_valid[3]=0;
		}
//KEY5																																				
		if (keys==4)
		{
			if (key_valid[4]==0)//������Ч
			{
				KEYS[4]=PRESS_DOWN;
				key_time[4]++;
				if (key_time[4]>=80)
				{
					KEYS[4]=PRESS_LONG;
					key_valid[4]=1;//����������������Ч
				}
			}
		}
		else
		{
			if (key_time[4]>1&&key_time[4]<80)
			{
				KEYS[4]=PRESS_SHORT;
			}
			key_time[4]=0;
			key_valid[4]=0;
		}
//KEY6																																				
		if (keys==5)
		{
			if (key_valid[5]==0)//������Ч
			{
				KEYS[5]=PRESS_DOWN;
				key_time[5]++;
				if (key_time[5]>=80)
				{
					KEYS[5]=PRESS_LONG;
					key_valid[5]=1;//����������������Ч
				}
			}
		}
		else
		{
			if (key_time[5]>1&&key_time[5]<80)
			{
				KEYS[5]=PRESS_SHORT;
			}
			key_time[5]=0;
			key_valid[5]=0;//������Ϊ��Ч
		}
		if (KEY_Effective)//�а�������ʱ���Ѵ�������
			TaskSendMsg(0,SYS_MSG_KEY); 

}



#else /*ifdef JZQ_V2_3*/



//�������Ŷ���
#define KEY1  PCin(7)
#define KEY2  PAin(0)
#define KEY3  PCin(8)
#define KEY4  PBin(9)
#define KEY5  PCin(9)
#define KEY6  PBin(8)


//�������飬֧�ֶ������ͬʱ����
static u8 KEYS[6];

void Key_Init (void)
{
#if USE_EXIT==1
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
#endif

	GPIO_InitTypeDef GPIO_InitStructure;
	//��ʼ��KEY0-->GPIOA.13,KEY1-->GPIOA.15  ��������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTA,PORTEʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  
  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��
	addTimerIrq10ms(KEY_IRQHandler);//�ڶ�ʱ���������жϷ�����

}


//���ذ���״̬
u8 Get_Key(u8 keynum)
{
	u8 key_ret;
	key_ret=KEYS[keynum-1];
	KEYS[keynum-1]=PRESS_NONE;
	return key_ret;
}



//������Ч
#define KEY_Effective (KEYS[0]||KEYS[1]||KEYS[2]||KEYS[3]||KEYS[4]||KEYS[5])


//�жϷ�����														

//�����жϷ������
void KEY_IRQHandler(void)   //
{
	static u8 key_time[6];
	static u8 key_valid[6];
	u8 keys=0xff;//������ֵ��0xffΪ�ް�������
//	keys=GPIO_ReadInputData(GPIOC)&0x0f;//��ȡ������Ϣ
//KEY1																																				
		if (KEY1==0)
		{
			if (key_valid[0]==0)//������Ч
			{
				KEYS[0]=PRESS_DOWN;
				key_time[0]++;
				if (key_time[0]>=80)
				{
					KEYS[0]=PRESS_LONG;
					key_valid[0]=1;//����������������Ч
				}
			}
		}
		else
		{
			if (key_time[0]>1&&key_time[0]<80)
			{
				KEYS[0]=PRESS_SHORT;
			}
			key_time[0]=0;
			key_valid[0]=0;//������Ϊ��Ч
		}
//KEY2																																				
		if (KEY2==0)
		{
			if (key_valid[1]==0)//������Ч
			{
				KEYS[1]=PRESS_DOWN;
				key_time[1]++;
				if (key_time[1]>=80)
				{
					KEYS[1]=PRESS_LONG;
					key_valid[1]=1;//����������������Ч
				}
			}
		}
		else
		{
		if (key_time[1]>1&&key_time[1]<80)
			{
				KEYS[1]=PRESS_SHORT;
			}
			key_time[1]=0;
			key_valid[1]=0;
		}
//KEY3																																				
		if (KEY3==0)
		{
			if (key_valid[2]==0)//������Ч
			{
				KEYS[2]=PRESS_DOWN;
				key_time[2]++;
				if (key_time[2]>=80)
				{
					KEYS[2]=PRESS_LONG;
					key_valid[2]=1;//����������������Ч
				}
			}
		}
		else
		{
		if (key_time[2]>1&&key_time[2]<80)
			{
				KEYS[2]=PRESS_SHORT;
			}
			key_time[2]=0;
			key_valid[2]=0;
		}
//KEY4																																				
		if (KEY4==0)
		{
			if (key_valid[3]==0)//������Ч
			{
				KEYS[3]=PRESS_DOWN;
				key_time[3]++;
				if (key_time[3]>=80)
				{
					KEYS[3]=PRESS_LONG;
					key_valid[3]=1;//����������������Ч
				}
			}
		}
		else
		{
			if (key_time[3]>1&&key_time[3]<80)
			{
				KEYS[3]=PRESS_SHORT;
			}
			key_time[3]=0;
			key_valid[3]=0;
		}
//KEY5																																				
		if (KEY5==0)
		{
			if (key_valid[4]==0)//������Ч
			{
				KEYS[4]=PRESS_DOWN;
				key_time[4]++;
				if (key_time[4]>=80)
				{
					KEYS[4]=PRESS_LONG;
					key_valid[4]=1;//����������������Ч
				}
			}
		}
		else
		{
			if (key_time[4]>1&&key_time[4]<80)
			{
				KEYS[4]=PRESS_SHORT;
			}
			key_time[4]=0;
			key_valid[4]=0;
		}
//KEY6																																				
		if (KEY6==0)
		{
			if (key_valid[5]==0)//������Ч
			{
				KEYS[5]=PRESS_DOWN;
				key_time[5]++;
				if (key_time[5]>=80)
				{
					KEYS[5]=PRESS_LONG;
					key_valid[5]=1;//����������������Ч
				}
			}
		}
		else
		{
			if (key_time[5]>1&&key_time[5]<80)
			{
				KEYS[5]=PRESS_SHORT;
			}
			key_time[5]=0;
			key_valid[5]=0;//������Ϊ��Ч
		}
		if (KEY_Effective)//�а�������ʱ���Ѵ�������
			TaskSendMsg(0,SYS_MSG_KEY); 

}







#endif




//���ð���״̬���������ⰴ��,0,�ɹ���1,ʧ��
u8 Set_Key(u8 keynum,u8 action)
{
	if (keynum==0||keynum>6) return 1;
	if (action>3) return 1;
	if (KEYS[keynum-1]==PRESS_NONE)
	{
		KEYS[keynum-1]=action;
		TaskSendMsg(0,SYS_MSG_KEY); 
		return 0;
	}
	else
	{
		return 1;
	}
}




















