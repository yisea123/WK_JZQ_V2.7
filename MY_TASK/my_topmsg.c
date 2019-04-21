#include "includes.h"
#include "key.h"
#include "my_messeg.h"
#include "beep.h"
#include "hard_irq.h"
#include "w5500.h"
#include "timer.h"
#include "my_topmsg.h"


/*********************************************

		顶级消息分发任务：
		分发按键消息，
		各个端口的网络消息
		10ms的软件定时器


***********************************************/



//本文件宏定义
#define SOFT_TIMER_IRQ  (0x2000)


//本文件局域函数
void key_deal (void);
void taskmsg_deal (u32 msg);
void soft_timer_irq (void);
void soft_timer_run (void);
void load_test_cfg(void);//加载默认配置信息

//本文件局域变量
void (**soft_timer_10ms) (void)=0;



void my_topmsg (void *t)
{
	u32 irq_msg=0;
	Key_Init();
	Load_Config();
	load_test_cfg(); 
	//write_config(); //写入配置到文件

	soft_timer_10ms=mymalloc(4*10);
	mymemset(soft_timer_10ms,0,4*10);
	addTimerIrq10ms(soft_timer_irq);
	
	

	
	
	while(1)
	{
		irq_msg=TaskGetMsg();
		
		if (irq_msg&SYS_MSG_KEY)//按键处理
		{
			irq_msg&=~SYS_MSG_KEY;
			key_deal();
		}
		if (irq_msg&SYS_MSG_USART2)//串口2，LCD
		{
			irq_msg&=~SYS_MSG_USART2;
			TaskSendMsg(USART2_GetFocus(),SYS_MSG_USART2);
		}
		if (irq_msg&SYS_MSG_USART1)//串口1，RF
		{
			irq_msg&=~SYS_MSG_USART1;
			TaskSendMsg(USART2_GetFocus(),SYS_MSG_USART1);
		}
		if (irq_msg&SYS_MSG_W5500)//网口消息
		{
			irq_msg&=~SYS_MSG_W5500;
			W5500_IRQ();
		}
		if (irq_msg&SOFT_TIMER_IRQ)//激活软件定时器
		{
			irq_msg&=~SOFT_TIMER_IRQ;
			soft_timer_run();
		}
		if (irq_msg)
		{
			taskmsg_deal (irq_msg);//其他消息处理，
		}
	}
}


static u8 key[6]={0};
static u8 state[6]={0,0,0,0,0x3f,0};//短按状态
static u8 statel[6]={0,0,0,0,1,0};//长安状态

u8 key_color[3][6]={0};


void key_deal (void)
{
	u8 meg[MESSEG_DATA]={0};
	u8 light[MESSEG_DATA]={0};
	
	for (u8 i=0;i<6;i++)//获取按键值并实时反馈
	{
		key[i]=PRESS_NONE;
		key[i]=Get_Key (i+1);
		if (key[i]==PRESS_SHORT)
		{
			light[0]=5;//屏幕蜂鸣器叫
			light[1]=1;
			send_messeg(LCD_MESSEG,light);
			
			light[0]=LIGHT_ROUND_LIGHT;
			light[2]=LIGHT_LIGHT_STOP;
			light[1]=LIGHT_LIGHT_RUNTO;
			send_messeg(LIT_MESSEG,light);
		}
		else if (key[i]==PRESS_LONG)
		{
			light[0]=5;//屏幕蜂鸣器叫
			light[1]=2;
			send_messeg(LCD_MESSEG,light);
			
			light[0]=LIGHT_ROUND_LIGHT;
			light[2]=LIGHT_LIGHT_STOP;
			light[1]=LIGHT_LIGHT_RUNTO;
			send_messeg(LIT_MESSEG,light);
		}
		else if (key[i]==PRESS_DOWN)
		{
			light[0]=LIGHT_ROUND_LIGHT;
			light[2]=LIGHT_LIGHT_STCOR;
			light[1]=LIGHT_LIGHT_RUNTO;
			light[3]=255;
			light[4]=255;
			light[5]=255;
			send_messeg(LIT_MESSEG,light);
		}
		else
		{
		}
	}
		
		if (key[0]==PRESS_SHORT) //按键1短按
		{
			meg[0]=2;//空调
			meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//按键通用参数
			meg[5]=1;//升按键
			send_messeg(RF_MESSEG,meg);
		}
		else if (key[0]==PRESS_LONG)//按键1长安
		{
			if (statel[0]==0)
			{
				TaskPend(8);//挂起自动控制
				Load_down();
			}
			else
			{
				TaskRepend(8);//自动控制
				Load_up();
			}
			statel[0]=!statel[0];
		}

		if (key[1]==PRESS_SHORT) //按键2短按
		{
			meg[0]=2;//空调
			meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//按键通用参数
			meg[5]=2;//降按键
			send_messeg(RF_MESSEG,meg);
		}
		else if (key[1]==PRESS_LONG)//按键2长安
		{
			if (statel[1]==0)
			{
				meg[0]=3;meg[1]=4;//彩灯
				meg[2]=2;//闪烁
				meg[3]=200;meg[4]=100;meg[5]=50;//色彩值
				send_messeg(LIT_MESSEG,meg);//
				statel[1]=1;
				Lcd_SetHandstate(1);
			}
			else if (statel[1]==1)
			{
				meg[0]=3;meg[1]=3;//彩灯
				meg[2]=2;//闪烁
				meg[3]=200;meg[4]=100;meg[5]=50;//色彩值
				send_messeg(LIT_MESSEG,meg);//
				statel[1]=2;
				Lcd_SetHandstate(1);
			}
			else if (statel[1]==2)
			{
				meg[0]=3;meg[1]=4;//彩灯
				meg[2]=0;//停止
				meg[3]=200;meg[4]=100;meg[5]=50;//色彩值
				send_messeg(LIT_MESSEG,meg);//
				statel[1]=0;
				Lcd_SetHandstate(0);
			}
		}


		if (key[2]==PRESS_SHORT) //按键3短按
		{
			meg[0]=5;//加湿机
			meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//按键通用参数
			meg[5]=1;//升按键
			send_messeg(RF_MESSEG,meg);
		}
		else if (key[2]==PRESS_LONG)//按键3长安
		{
			if (statel[2]==0)
			{
				Load_song3();//一首歌会占用很大的存储空间
			}
			else if (statel[2]==1)
			{
				Load_song1();
			}
			else if (statel[2]==2)
			{
				Load_song();
			}
			else if (statel[2]==3)
			{
				Load_song2();
			}
			statel[2]++;
			if (statel[2]>4) statel[2]=0;
		}

		if (key[3]==PRESS_SHORT) //按键4短按
		{
			meg[0]=3;
			meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//按键通用参数
			meg[5]=2;
			send_messeg(RF_MESSEG,meg);
		}
		else if (key[3]==PRESS_LONG)//按键4长安
		{
			meg[0]=2;
			send_messeg(CFG_MESSEG,meg);//切换配置模式
		}


		if (key[4]==PRESS_SHORT) //按键5短按，屏幕控制
		{			
			meg[0]=3;meg[1]=1;
			meg[2]=0xff;meg[3]=0;
			meg[4]=1;meg[5]=0;
			send_messeg(LCD_MESSEG,meg);//
		}
		else if (key[4]==PRESS_LONG)//按键5长安
		{
			meg[0]=3;meg[1]=0;
			meg[2]=0xff;meg[3]=0;
			meg[4]=1;meg[5]=0;
			send_messeg(LCD_MESSEG,meg);//

		}



		
		if (key[5]==PRESS_SHORT) //按键6短按
		{
			meg[0]=4;
			meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//按键通用参数
			meg[5]=2;
			send_messeg(RF_MESSEG,meg);
		}
		else if (key[5]==PRESS_LONG)//按键6长安
		{
		}

}




void taskmsg_deal (u32 msg)
{
	if(msg& W5500_SOCKET0)
	{
		msg&=~W5500_SOCKET0;
		TaskSendMsg(SOCKET0_GetFocus(),W5500_SOCKET0);
	}
	if(msg& W5500_SOCKET1)
	{
		msg&=~W5500_SOCKET1;
		TaskSendMsg(SOCKET1_GetFocus(),W5500_SOCKET1);
	}
	if(msg& W5500_SOCKET2)
	{
		msg&=~W5500_SOCKET2;
		TaskSendMsg(SOCKET2_GetFocus(),W5500_SOCKET2);
	}
	if(msg& W5500_SOCKET3)
	{
		msg&=~W5500_SOCKET3;
		TaskSendMsg(SOCKET3_GetFocus(),W5500_SOCKET3);
	}
	if(msg& W5500_SOCKET4)
	{
		msg&=~W5500_SOCKET4;
		TaskSendMsg(SOCKET4_GetFocus(),W5500_SOCKET4);
	}
	if(msg& W5500_SOCKET5)
	{
		msg&=~W5500_SOCKET5;
		TaskSendMsg(SOCKET5_GetFocus(),W5500_SOCKET5);
	}
	if(msg& W5500_SOCKET6)
	{
		msg&=~W5500_SOCKET6;
		TaskSendMsg(SOCKET6_GetFocus(),W5500_SOCKET6);
	}
	if(msg& W5500_SOCKET7)
	{
		msg&=~W5500_SOCKET7;
		TaskSendMsg(SOCKET7_GetFocus(),W5500_SOCKET7);
	}
	if(msg)//如果还有消息
	{
	}
}







		//加载默认配置信息
void load_test_cfg(void)
{
	
	//如果没有配置，加载默认的
	if (EN_CONFIG[0]==0)
	{
		EN_CONFIG[0]=1;
		EN_CONFIG[1]=0;
		EN_CONFIG[2]=2;
		EN_CONFIG[3]=1;
		EN_CONFIG[4]=3;
		EN_CONFIG[5]=3;
		EN_CONFIG[6]=33;
		EN_CONFIG[7]=1;
		EN_CONFIG[8]=34;
		EN_CONFIG[9]=1;
		EN_CONFIG[10]=35;
		EN_CONFIG[11]=1;
		EN_CONFIG[12]=36;
		EN_CONFIG[13]=1;
		
		//本机地址
		Get_MyIP()[0]=192;
		Get_MyIP()[1]=168;
		Get_MyIP()[2]=2;
		Get_MyIP()[3]=13;
		
		//服务器地址
		Get_MyIP()[6]=192;
		Get_MyIP()[7]=168;
		Get_MyIP()[8]=2;
		Get_MyIP()[9]=56;

		Get_MyIP()[10]=6000>>8;
		Get_MyIP()[11]=6000&0x00ff;
		
		//网关IP
		Get_MyIP()[12]=192;
		Get_MyIP()[13]=168;
		Get_MyIP()[14]=2;
		Get_MyIP()[15]=1;

		Save_Config();
	}
	
	//没有本机名称
	if (*getMyName()==0)
	{
		char *txtbuff=mymalloc(512);
		sprintf(txtbuff,"%.6s-%02X%02X%02X","WK_JZQ",MCU_SN[9],MCU_SN[10],MCU_SN[11]);
		setMyName (txtbuff);
		myfree (txtbuff);
	}
	//默认一个超调量
	if (getAutoCtrlAmount()==0)
	{
		setAutoCtrlAmount(2);
	}
	if (getWarnTolerance()==0)
	{
		setWarnTolerance(2);
	}
	//默认一个控制频率
	if (getAutoCtrlFrequency()==0)
	{
		setAutoCtrlFrequency(30);
	}
}













//软件定时器中断，
void soft_timer_irq (void)
{
	TaskIntSendMsg(0,SOFT_TIMER_IRQ);
}


void soft_timer_run (void)
{
	if (soft_timer_10ms)
	{
		for(u8 i=0;*(soft_timer_10ms+i);i++)
		{
			(*(soft_timer_10ms+i))();
		}
	}
}


//外部调用的函数

//0成功，1失败
u8 addSoftTimerIrq10ms (void (*irq)(void))
{
	for (u8 i=0;i<10;i++)
	{
		if (soft_timer_10ms[i]==0)
		{
			soft_timer_10ms[i]=irq;
			return 0;
		}
	}
	return 1;
	
}


//清除10ms定时器终端服务函数
u8 delSoftTimerIrq10ms (void (*irq)(void))
{
	for (u8 i=0;i<10;i++)
	{
		if (soft_timer_10ms[i]==irq)
		{
			soft_timer_10ms[i]=0;
			return 0;
		}
	}
	return 1;
}







