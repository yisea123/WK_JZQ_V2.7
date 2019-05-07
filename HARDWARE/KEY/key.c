#include "includes.h"
#include "timer.h"
#include "hard_irq.h"
#include "key.h"




/**************************************************
		文件说明：按键驱动
		引脚使用：PC0，PC1，PC2，PC3
		外部引用：10ms的定时器中断
		实现方法：使用软件周期扫描检测按键，使用定时器中断实现，
		使用方法：本驱动需要外部引用一个10ms的定时器中断服务，驱动会存储最后一次按下的按键值。
		中断优先级：未使用中断
****************************************************/





#ifndef JZQ_V2_3




//按键数组，支持多个按键同时按下
static u8 KEYS[6];

void Key_Init (void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	
	//初始化时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOC, &GPIO_InitStructure); //新板子不使用以前的按键灯光2018.11.5，这4个IO用来做触摸按键

	addTimerIrq10ms(KEY_IRQHandler);//在定时器中添加中断服务函数
}



//返回按键状态
u8 Get_Key(u8 keynum)
{
	u8 key_ret;
	key_ret=KEYS[keynum-1];
	KEYS[keynum-1]=PRESS_NONE;
	return key_ret;
}


//按键有效
#define KEY_Effective (KEYS[0]||KEYS[1]||KEYS[2]||KEYS[3]||KEYS[4]||KEYS[5])



//按键中断服务程序
void KEY_IRQHandler(void)   //
{
	static u8 key_time[6];
	static u8 key_valid[6];
	u8 keys=0xff;//按键键值，0xff为无按键按下
	keys=GPIO_ReadInputData(GPIOC)&0x0f;//读取按键信息
//KEY1																																				
		if (keys==0)
		{
			if (key_valid[0]==0)//按键有效
			{
				KEYS[0]=PRESS_DOWN;
				key_time[0]++;
				if (key_time[0]>=80)
				{
					KEYS[0]=PRESS_LONG;
					key_valid[0]=1;//触发长按，按键无效
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
			key_valid[0]=0;//按键设为有效
		}
//KEY2																																				
		if (keys==1)
		{
			if (key_valid[1]==0)//按键有效
			{
				KEYS[1]=PRESS_DOWN;
				key_time[1]++;
				if (key_time[1]>=80)
				{
					KEYS[1]=PRESS_LONG;
					key_valid[1]=1;//触发长按，按键无效
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
			if (key_valid[2]==0)//按键有效
			{
				KEYS[2]=PRESS_DOWN;
				key_time[2]++;
				if (key_time[2]>=80)
				{
					KEYS[2]=PRESS_LONG;
					key_valid[2]=1;//触发长按，按键无效
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
			if (key_valid[3]==0)//按键有效
			{
				KEYS[3]=PRESS_DOWN;
				key_time[3]++;
				if (key_time[3]>=80)
				{
					KEYS[3]=PRESS_LONG;
					key_valid[3]=1;//触发长按，按键无效
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
			if (key_valid[4]==0)//按键有效
			{
				KEYS[4]=PRESS_DOWN;
				key_time[4]++;
				if (key_time[4]>=80)
				{
					KEYS[4]=PRESS_LONG;
					key_valid[4]=1;//触发长按，按键无效
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
			if (key_valid[5]==0)//按键有效
			{
				KEYS[5]=PRESS_DOWN;
				key_time[5]++;
				if (key_time[5]>=80)
				{
					KEYS[5]=PRESS_LONG;
					key_valid[5]=1;//触发长按，按键无效
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
			key_valid[5]=0;//按键设为有效
		}
		if (KEY_Effective)//有按键按下时唤醒处理任务
			TaskSendMsg(0,SYS_MSG_KEY); 

}



#else /*ifdef JZQ_V2_3*/



//按键引脚定义
#define KEY1  PCin(7)
#define KEY2  PAin(0)
#define KEY3  PCin(8)
#define KEY4  PBin(9)
#define KEY5  PCin(9)
#define KEY6  PBin(8)


//按键数组，支持多个按键同时按下
static u8 KEYS[6];

void Key_Init (void)
{

	//初始化KEY0-->GPIOA.13,KEY1-->GPIOA.15  上拉输入
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA,PORTE时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  
  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟
	addTimerIrq10ms(KEY_IRQHandler);//在定时器中添加中断服务函数

}


//返回按键状态
u8 Get_Key(u8 keynum)
{
	u8 key_ret;
	key_ret=KEYS[keynum-1];
	KEYS[keynum-1]=PRESS_NONE;
	return key_ret;
}



//按键有效
#define KEY_Effective (KEYS[0]||KEYS[1]||KEYS[2]||KEYS[3]||KEYS[4]||KEYS[5])


//中断服务函数														

//按键中断服务程序
void KEY_IRQHandler(void)   //
{
	static u8 key_time[6];
	static u8 key_valid[6];
//	u8 keys=0xff;//按键键值，0xff为无按键按下
//	keys=GPIO_ReadInputData(GPIOC)&0x0f;//读取按键信息
//KEY1																																				
		if (KEY1==0)
		{
			if (key_valid[0]==0)//按键有效
			{
				KEYS[0]=PRESS_DOWN;
				key_time[0]++;
				if (key_time[0]>=80)
				{
					KEYS[0]=PRESS_LONG;
					key_valid[0]=1;//触发长按，按键无效
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
			key_valid[0]=0;//按键设为有效
		}
//KEY2																																				
		if (KEY2==0)
		{
			if (key_valid[1]==0)//按键有效
			{
				KEYS[1]=PRESS_DOWN;
				key_time[1]++;
				if (key_time[1]>=80)
				{
					KEYS[1]=PRESS_LONG;
					key_valid[1]=1;//触发长按，按键无效
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
			if (key_valid[2]==0)//按键有效
			{
				KEYS[2]=PRESS_DOWN;
				key_time[2]++;
				if (key_time[2]>=80)
				{
					KEYS[2]=PRESS_LONG;
					key_valid[2]=1;//触发长按，按键无效
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
			if (key_valid[3]==0)//按键有效
			{
				KEYS[3]=PRESS_DOWN;
				key_time[3]++;
				if (key_time[3]>=80)
				{
					KEYS[3]=PRESS_LONG;
					key_valid[3]=1;//触发长按，按键无效
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
			if (key_valid[4]==0)//按键有效
			{
				KEYS[4]=PRESS_DOWN;
				key_time[4]++;
				if (key_time[4]>=80)
				{
					KEYS[4]=PRESS_LONG;
					key_valid[4]=1;//触发长按，按键无效
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
			if (key_valid[5]==0)//按键有效
			{
				KEYS[5]=PRESS_DOWN;
				key_time[5]++;
				if (key_time[5]>=80)
				{
					KEYS[5]=PRESS_LONG;
					key_valid[5]=1;//触发长按，按键无效
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
			key_valid[5]=0;//按键设为有效
		}
		if (KEY_Effective)//有按键按下时唤醒处理任务
			TaskSendMsg(0,SYS_MSG_KEY); 

}







#endif




//设置按键状态，用于虚拟按键,0,成功，1,失败
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





















