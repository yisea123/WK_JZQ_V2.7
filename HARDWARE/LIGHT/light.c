#include "includes.h"
#include "light.h"


#ifndef JZQ_V2_3


/**************************************************
		文件说明：灯光驱动，
		引脚使用：PWM：PB0，PB1；按键灯：PA1；环绕灯带：
		外部引用：PWM调光使用定时器硬件PWM，因此需在外部定时器初始化连接到PWM调光的引脚
		实现方法：可编程灯珠使用阻塞式软件延时发送彩色数据，PWM通过调整占空比来调光
		使用方法：本驱动的可编程灯珠调光是阻塞的，PWM是非阻塞的
		中断优先级：未使用中断
****************************************************/






#define LED_R PCout(7)
#define LED_B PCout(8)
//#define LIGHT_1 PBout(4)
#define LIGHT_2 PBout(5)
#define LIGHT_3 PBout(6)
#define LIGHT_4 PBout(7)
#define LIGHT_1 PBout(3)

//-------------------------------

#define KEY_LIGHT PAout(1)


#define DELAY_300NS  {__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
											__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
											__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
											__NOP();__NOP();__NOP();}
#define DELAY_900NS  {__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
											__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
											__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
											__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
											__NOP();__NOP();__NOP();__NOP();__NOP();}

											//按键旁边的灯缓冲池
static u8 data[7]={0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static u8 KEY_COLOR[3][6]={{0},{0},{20,20,20,20,20,20}};//新版集中器按键灯颜色2018.11.5

static u8 LED_COLOR[3][LED_NUM]={0};


void (*LIGHT_RUN_PTR)(void);//灯光亮度中断2019.1.18


void Light_init (void)
{

	GPIO_InitTypeDef GPIO_InitStructure;

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	
	GPIO_Init(GPIOB, &GPIO_InitStructure); //新板子不使用以前的按键灯光2018.11.5
	LED_R=0;
	LED_B=0;
	
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);



	key_senddata();
}



/************************************************************

		新增的按键彩灯的控制函数2018.11.5

			包括key_reset()
					key_send()
					key_senddata()
					key_setcolor()
					led_resetall()
					key_around()
*************************************************************/


void key_reset (void)
{
	KEY_LIGHT=0;
	delay_us(1000);
	//delay_us(1000);
	//sleep_ms(10);
}


void key_send (u8 red ,u8 green,u8 blue)
{
	u8 i=0;
	
	
//	__disable_irq() ;   //关闭总中断
	for (i=0;i<8;i++)
	{
		if (green&0x80)//发送1
		{
			__disable_irq() ;   //关闭总中断
			KEY_LIGHT=1;
			DELAY_900NS;
			KEY_LIGHT=0;
			__enable_irq() ;   //关闭总中断
			DELAY_300NS;
		}
		else//发送0
		{
			__disable_irq() ;   //关闭总中断
			KEY_LIGHT=1;
			DELAY_300NS;
			KEY_LIGHT=0;
			__enable_irq() ;   //关闭总中断
			DELAY_900NS;
		}
		green<<=1;
	}

	
	for (i=0;i<8;i++)
	{
		if (red&0x80)//发送1
		{
			__disable_irq() ;   //关闭总中断
			KEY_LIGHT=1;
			DELAY_900NS;
			KEY_LIGHT=0;
			__enable_irq() ;   //关闭总中断
			DELAY_300NS;
		}
		else//发送0
		{
			__disable_irq() ;   //关闭总中断
			KEY_LIGHT=1;
			DELAY_300NS;
			KEY_LIGHT=0;
			__enable_irq() ;   //关闭总中断
			DELAY_900NS;
		}
		red<<=1;
	}
	
	
	
	for (i=0;i<8;i++)
	{
		if (blue&0x80)//发送1
		{
			__disable_irq() ;   //关闭总中断
			KEY_LIGHT=1;
			DELAY_900NS;
			KEY_LIGHT=0;
			__enable_irq() ;   //关闭总中断
			DELAY_300NS;
		}
		else//发送0
		{
			__disable_irq() ;   //关闭总中断
			KEY_LIGHT=1;
			DELAY_300NS;
			KEY_LIGHT=0;
			__enable_irq() ;   //关闭总中断
			DELAY_900NS;
		}
		blue<<=1;
	}
//	__enable_irq() ;   //关闭总中断
	KEY_LIGHT=0;
}

			//刷新默认颜色
void key_senddata (void)
{
	u16 i=0;
	OS_ENTER_ONLYME();
	key_reset();
	for (i=0;i<6;i++)
	{
		key_send (KEY_COLOR[0][i] ,KEY_COLOR[1][i] ,KEY_COLOR[2][i] );
	}
	KEY_LIGHT=0;
	OS_EXIT_ONLYME();
}

			//刷新刷新指定缓存区颜色
void key_senddataby (u8 **colors)
{
	u16 i=0;
	key_reset();
	for (i=0;i<6;i++)
	{
		key_send (colors[0][i] ,colors[1][i] ,colors[2][i] );
	}
	KEY_LIGHT=0;
}


//取得按键的色彩
u8 key_getcolor (u16 led_num,u8 *red,u8 *green,u8 *blue)
{
	if (6<=led_num) return 1;
	*red=KEY_COLOR[0][led_num];
	*green=KEY_COLOR[1][led_num];
	*blue=KEY_COLOR[2][led_num];
	return 0;

}



u8 key_setcolor (u16 led_num,u8 red ,u8 green,u8 blue)
{
	if (6<=led_num) return 1;
	KEY_COLOR[0][led_num]=red;
	KEY_COLOR[1][led_num]=green;
	KEY_COLOR[2][led_num]=blue;
	return 0;
}

void key_resetall (void)
{
	u16 i=0;
	for (i=0;i<LED_NUM;i++)
	{
		KEY_COLOR[0][i]=0;
		KEY_COLOR[1][i]=0;
		KEY_COLOR[2][i]=0;
	}
}
			//按键指示灯，指示设备状态，绿色
void key_light (u8 key_num,u8 state)
{
	if (key_num>6) return;
	if(state)
	{
		KEY_COLOR[1][key_num-1]=200;
	}
	else
	{
		KEY_COLOR[1][key_num-1]=0;
	}
	//key_senddata();
}

			//按键报警灯，红色
void key_around (u8 key_num,u8 state)
{
	if (key_num>6) return;
	if(state)
	{
		KEY_COLOR[0][key_num-1]=200;
	}
	else
	{
		KEY_COLOR[0][key_num-1]=0;
	}
	//key_senddata();
	
}

/***************************************************
↑↑↑↑↑添加的键盘灯的控制函数到此为止↑↑↑↑↑
***************************************************/


void sm16703_reset (void)
{
	LIGHT_1=0;
	delay_us(800);
//	LIGHT_1=1;
}



//彩灯数据发送，对电平时长要求严格，要关闭所有中断，否则会有异常点亮。
//发送一个灯所需时间约28.8us
void sm16703_send (u8 red ,u8 green,u8 blue)
{
	u8 i=0;
	
	
//	__disable_irq() ;   //关闭总中断
	for (i=0;i<8;i++)
	{
		if (green&0x80)//发送1
		{
			__disable_irq() ;   //关闭总中断
			LIGHT_1=1;
			DELAY_900NS;
			LIGHT_1=0;
			__enable_irq() ;   //关闭总中断
			DELAY_300NS;
		}
		else//发送0
		{
			__disable_irq() ;   //关闭总中断
			LIGHT_1=1;
			DELAY_300NS;
			LIGHT_1=0;
			__enable_irq() ;   //关闭总中断
			DELAY_900NS;
		}
		green<<=1;
	}

	
	for (i=0;i<8;i++)
	{
		if (red&0x80)//发送1
		{
			__disable_irq() ;   //关闭总中断
			LIGHT_1=1;
			DELAY_900NS;
			LIGHT_1=0;
			__enable_irq() ;   //关闭总中断
			DELAY_300NS;
		}
		else//发送0
		{
			__disable_irq() ;   //关闭总中断
			LIGHT_1=1;
			DELAY_300NS;
			LIGHT_1=0;
			__enable_irq() ;   //关闭总中断
			DELAY_900NS;
		}
		red<<=1;
	}
	
	
	
	for (i=0;i<8;i++)
	{
		if (blue&0x80)//发送1
		{
			__disable_irq() ;   //关闭总中断
			LIGHT_1=1;
			DELAY_900NS;
			LIGHT_1=0;
			__enable_irq() ;   //关闭总中断
			DELAY_300NS;
		}
		else//发送0
		{
			__disable_irq() ;   //关闭总中断
			LIGHT_1=1;
			DELAY_300NS;
			LIGHT_1=0;
			__enable_irq() ;   //关闭总中断
			DELAY_900NS;
		}
		blue<<=1;
	}
//	__enable_irq() ;   //关闭总中断
	LIGHT_1=0;
}


//设置灯光的颜色,0,成功，1，失败
u8 led_setcolor (u16 led_num,u8 red ,u8 green,u8 blue)
{
	if (LED_NUM<=led_num) return 1;
	LED_COLOR[0][led_num]=red;
	LED_COLOR[1][led_num]=green;
	LED_COLOR[2][led_num]=blue;
	return 0;
}

//获取灯光的颜色,0,成功，1，失败
u8 led_getcolor (u16 led_num,u8 *red ,u8 *green,u8 *blue)
{
	if (LED_NUM<=led_num) return 1;
	*red=LED_COLOR[0][led_num];
	*green=LED_COLOR[1][led_num];
	*blue=LED_COLOR[2][led_num];
	return 0;
}

void led_resetall (void)
{
	u16 i=0;
	for (i=0;i<LED_NUM;i++)
	{
		LED_COLOR[0][i]=0;
		LED_COLOR[1][i]=0;
		LED_COLOR[2][i]=0;
	}
}

void led_setall (u8 red,u8 green,u8 blue)
{
	u16 i=0;
	for (i=0;i<LED_NUM;i++)
	{
		LED_COLOR[0][i]=red;
		LED_COLOR[1][i]=green;
		LED_COLOR[2][i]=blue;
	}
}


void led_senddata (void)
{
//	u16 i=0;
//	sm16703_reset();
//	for (i=0;i<LED_NUM;i++)
//	{
//		sm16703_send (LED_COLOR[0][i] ,LED_COLOR[1][i] ,LED_COLOR[2][i] );
//	}
//	LIGHT_1=0;
}



//新板子不使用以前的按键灯光2018.11.5 包装为空函数
void HC595_SendData(void)
{}

/*
//发送数据
void HC595_SendData(void)
{
	//所有的灯都亮  1是亮
//	u8 data=0xff;
	int i,j;
	u8 temp;
	LIGHT_OUT=0;
	for(j=0;j<7;j++)
	{
		if (j==0)
			temp=~data[6-j];//倒序
		else
			temp=data[6-j];//倒序
		for(i=0;i<8;i++)
		{
			LIGHT_SCK=0;
			LIGHT_DATA=temp>>(7-i);
			LIGHT_SCK=1;
		}
	}
	LIGHT_OUT=1;

}
*/

//a表示哪个灯亮b表示是哪个芯片
void on_light(u8 a,u8 b)
{


		data[b-1]|=a;//开大灯
		HC595_SendData();

}

//选择灯怎么亮a是按键状态b是哪个芯片
void off_light(u8 a,u8 b)
{


		data[b-1]&=(~a);//开大灯
		HC595_SendData();


}
//返回0，关，非0，开
u8 GetLight_State(u8 lednum,u8 ic)
{
	u8 ret;
	ret=data[ic-1]&lednum;
	return ret;
}

		//设置亮灯位
void Set_lightdata (u8 *data1)
{
	u16 i=0;
	for (i=0;i<6;i++)
	{
		data[i]=data1[i];
	}
}

		//hoqu亮灯位
void Get_lightdata (u8 *data1)
{
	u16 i=0;
	for (i=0;i<6;i++)
	{
		data1[i]=data[i];
	}
}



void Run_Light (void)
{
	static u8 t=0;
	static u16 len=0;
	static u8 red;
	static u8 green;
	static u8 blue ;
	static u8 led_t=0;
	u8 temp=0xff;
	u8 e=data[6];
	u8 i=0;
	for (i=0;i<7;i++)
	{
		data[i]=0;
	}
	
	if (t>=8) data[(t-8)/8]=(temp<<(7-t%8)); 
		data[t/8]=(temp>>(t%8));
	data[t/8+1]=temp<<(7-t%8);
	data[t/8+2]=temp>>(t%8);
	data[t/8+3]=temp<<(7-t%8);
	data[t/8+4]=temp>>(t%8);
	data[t/8+5]=temp<<(7-t%8);
	data[6]=e;
	
	t++;
	if (t>15) t=0;
	HC595_SendData();
	
	
		led_resetall ();
		led_setcolor (len-1,red*1/10,green*2/10,blue*2/10);
		led_setcolor (len-2,red*2/10,green*5/10,blue*5/10);
		led_setcolor (len-3,red*10/10,green*10/10,blue*10/10);
		led_setcolor (len-4,red*2/10,green*5/10,blue*5/10);
		led_setcolor (len-5,red*1/10,green*2/10,blue*2/10);
		led_senddata();
		len++;
		if (len>30)
		{
			len%=30;
			led_t++;
			if (led_t>=10) led_t=0;
		}
		switch (led_t)
		{
			case 0:
				red=255;
				green =255;
				blue =255;
				break;
			case 1:
				red=255;
				green =255;
				blue =0;
				break;
			case 2:
				red=0;
				green =255;
				blue =255;
				break;
			case 3:
				red=255;
				green =0;
				blue =255;
				break;
			case 4:
				red=255;
				green =0;
				blue =0;
				break;
			case 5:
				red=0;
				green =255;
				blue =0;
				break;
			case 6:
				red=0;
				green =0;
				blue =255;
				break;
			case 7:
				red=128;
				green =30;
				blue =255;
				break;
			case 8:
				red=255;
				green =30;
				blue =128;
				break;
			case 9:
				red=128;
				green =255;
				blue =30;
				break;
			default:
				break;
		}
	
}




#define LED111 PBout(0)
#define LED222 PBout(1)




#define LED_TEST1 PAout(15)
#define LED_TEST2 PBout(3)
#define LED_TEST3 PDout(2)
#define LED_TEST4 PCout(10)

u8 RED_LIGHT=0;
u8 GREEN_LIGHT=0;
u8 BLUE_LIGHT=0;



//设置灯光带的颜色
void light_setcolor(u8 red,u8 green,u8 blue)
{
	RED_LIGHT=red;
	GREEN_LIGHT=green;
	BLUE_LIGHT=blue;
	
	TIM3->CCR4=red;
	TIM3->CCR3=green;
}


#else /*ifdef JZQ_V2_3*/															/*V2.3板子的灯光驱动*/




#define LIGHT_DATA  PCout(0)
#define LINGT_EN    PCout(1)
#define LIGHT_OUT   PCout(2)
#define LIGHT_SCK   PCout(3)



#define LED_R PCout(7)
#define LED_B PCout(8)
//#define LIGHT_1 PBout(4)
#define LIGHT_2 PBout(5)
#define LIGHT_3 PBout(6)
#define LIGHT_4 PBout(7)
#define LIGHT_1 PBout(0)

//-------------------------------

#define KEY_LIGHT PAout(1)


#define DELAY_300NS  {__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
											__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
											__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
											__NOP();__NOP();__NOP();}
#define DELAY_900NS  {__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
											__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
											__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
											__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
											__NOP();__NOP();__NOP();__NOP();__NOP();}

											//按键旁边的灯缓冲池
static u8 data[7]={0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static u8 KEY_COLOR[3][6]={{0},{0},{20,20,20,20,20,20}};//新版集中器按键灯颜色2018.11.5

static u8 LED_COLOR[3][LED_NUM]={0};

void Light_init (void)
{

	GPIO_InitTypeDef GPIO_InitStructure;

	
				//板子上的灯
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	
		//临时使用这两个脚调光2018.11.23
	GPIO_Init(GPIOC, &GPIO_InitStructure); //新板子不使用以前的按键灯光2018.11.5
	LED_R=0;
	LED_B=0;

				//四周的灯带
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);


//	LIGHT_4=0;
//	LIGHT_3=0;
//	LIGHT_2=1;
	LIGHT_1=0;

	HC595_SendData( );//新板子不使用以前的按键灯光2018.11.5
	LINGT_EN =0;
	led_senddata();
//	key_senddata();
}



/************************************************************

		新增的按键彩灯的控制函数2018.11.5

			包括key_reset()
					key_send()
					key_senddata()
					key_setcolor()
					led_resetall()
					key_around()
*************************************************************/


void key_reset (void)
{
	KEY_LIGHT=0;
	delay_us(800);
//	LIGHT_1=1;
}


void key_send (u8 red ,u8 green,u8 blue)
{
	u8 i=0;
	
	
//	__disable_irq() ;   //关闭总中断
	for (i=0;i<8;i++)
	{
		if (green&0x80)//发送1
		{
			__disable_irq() ;   //关闭总中断
			KEY_LIGHT=1;
			DELAY_900NS;
			KEY_LIGHT=0;
			__enable_irq() ;   //关闭总中断
			DELAY_300NS;
		}
		else//发送0
		{
			__disable_irq() ;   //关闭总中断
			KEY_LIGHT=1;
			DELAY_300NS;
			KEY_LIGHT=0;
			__enable_irq() ;   //关闭总中断
			DELAY_900NS;
		}
		green<<=1;
	}

	
	for (i=0;i<8;i++)
	{
		if (red&0x80)//发送1
		{
			__disable_irq() ;   //关闭总中断
			KEY_LIGHT=1;
			DELAY_900NS;
			KEY_LIGHT=0;
			__enable_irq() ;   //关闭总中断
			DELAY_300NS;
		}
		else//发送0
		{
			__disable_irq() ;   //关闭总中断
			KEY_LIGHT=1;
			DELAY_300NS;
			KEY_LIGHT=0;
			__enable_irq() ;   //关闭总中断
			DELAY_900NS;
		}
		red<<=1;
	}
	
	
	
	for (i=0;i<8;i++)
	{
		if (blue&0x80)//发送1
		{
			__disable_irq() ;   //关闭总中断
			KEY_LIGHT=1;
			DELAY_900NS;
			KEY_LIGHT=0;
			__enable_irq() ;   //关闭总中断
			DELAY_300NS;
		}
		else//发送0
		{
			__disable_irq() ;   //关闭总中断
			KEY_LIGHT=1;
			DELAY_300NS;
			KEY_LIGHT=0;
			__enable_irq() ;   //关闭总中断
			DELAY_900NS;
		}
		blue<<=1;
	}
//	__enable_irq() ;   //关闭总中断
	KEY_LIGHT=0;
}

			//刷新默认颜色
void key_senddata (void)
{
	u16 i=0;
	key_reset();
	for (i=0;i<6;i++)
	{
		key_send (KEY_COLOR[0][i] ,KEY_COLOR[1][i] ,KEY_COLOR[2][i] );
	}
	KEY_LIGHT=0;
}

			//刷新刷新指定缓存区颜色
void key_senddataby (u8 **colors)
{
	u16 i=0;
	key_reset();
	for (i=0;i<6;i++)
	{
		key_send (colors[0][i] ,colors[1][i] ,colors[2][i] );
	}
	KEY_LIGHT=0;
}


//取得按键的色彩
u8 key_getcolor (u16 led_num,u8 *red,u8 *green,u8 *blue)
{
	if (6<=led_num) return 1;
	*red=KEY_COLOR[0][led_num];
	*green=KEY_COLOR[1][led_num];
	*blue=KEY_COLOR[2][led_num];
	return 0;

}




u8 key_setcolor (u16 led_num,u8 red ,u8 green,u8 blue)
{
	if (6<=led_num) return 1;
	KEY_COLOR[0][led_num]=red;
	KEY_COLOR[1][led_num]=green;
	KEY_COLOR[2][led_num]=blue;
	return 0;
}

void key_resetall (void)
{
	u16 i=0;
	for (i=0;i<LED_NUM;i++)
	{
		KEY_COLOR[0][i]=0;
		KEY_COLOR[1][i]=0;
		KEY_COLOR[2][i]=0;
	}
}


/*
			//按键指示灯，指示设备状态，绿色
void key_light (u8 key_num,u8 state)
{
	if (key_num>6) return;
	if(state)
	{
		KEY_COLOR[1][key_num-1]=200;
	}
	else
	{
		KEY_COLOR[1][key_num-1]=0;
	}
	key_senddata();
}

			//按键报警灯，红色
void key_around (u8 key_num,u8 state)
{
	if (key_num>6) return;
	if(state)
	{
		KEY_COLOR[0][key_num-1]=200;
	}
	else
	{
		KEY_COLOR[0][key_num-1]=0;
	}
	key_senddata();
	
}*/

/***************************************************
↑↑↑↑↑添加的键盘灯的控制函数到此为止↑↑↑↑↑
***************************************************/


void sm16703_reset (void)
{
	LIGHT_1=0;
	delay_us(800);
//	LIGHT_1=1;
}



//彩灯数据发送，对电平时长要求严格，要关闭所有中断，否则会有异常点亮。
//发送一个灯所需时间约28.8us
void sm16703_send (u8 red ,u8 green,u8 blue)
{
	u8 i=0;
	
	
//	__disable_irq() ;   //关闭总中断
	for (i=0;i<8;i++)
	{
		if (green&0x80)//发送1
		{
			__disable_irq() ;   //关闭总中断
			LIGHT_1=1;
			DELAY_900NS;
			LIGHT_1=0;
			__enable_irq() ;   //关闭总中断
			DELAY_300NS;
		}
		else//发送0
		{
			__disable_irq() ;   //关闭总中断
			LIGHT_1=1;
			DELAY_300NS;
			LIGHT_1=0;
			__enable_irq() ;   //关闭总中断
			DELAY_900NS;
		}
		green<<=1;
	}

	
	for (i=0;i<8;i++)
	{
		if (red&0x80)//发送1
		{
			__disable_irq() ;   //关闭总中断
			LIGHT_1=1;
			DELAY_900NS;
			LIGHT_1=0;
			__enable_irq() ;   //关闭总中断
			DELAY_300NS;
		}
		else//发送0
		{
			__disable_irq() ;   //关闭总中断
			LIGHT_1=1;
			DELAY_300NS;
			LIGHT_1=0;
			__enable_irq() ;   //关闭总中断
			DELAY_900NS;
		}
		red<<=1;
	}
	
	
	
	for (i=0;i<8;i++)
	{
		if (blue&0x80)//发送1
		{
			__disable_irq() ;   //关闭总中断
			LIGHT_1=1;
			DELAY_900NS;
			LIGHT_1=0;
			__enable_irq() ;   //关闭总中断
			DELAY_300NS;
		}
		else//发送0
		{
			__disable_irq() ;   //关闭总中断
			LIGHT_1=1;
			DELAY_300NS;
			LIGHT_1=0;
			__enable_irq() ;   //关闭总中断
			DELAY_900NS;
		}
		blue<<=1;
	}
//	__enable_irq() ;   //关闭总中断
	LIGHT_1=0;
}


//设置灯光的颜色,0,成功，1，失败
u8 led_setcolor (u16 led_num,u8 red ,u8 green,u8 blue)
{
	if (LED_NUM<=led_num) return 1;
	LED_COLOR[0][led_num]=red;
	LED_COLOR[1][led_num]=green;
	LED_COLOR[2][led_num]=blue;
	return 0;
}

//获取灯光的颜色,0,成功，1，失败
u8 led_getcolor (u16 led_num,u8 *red ,u8 *green,u8 *blue)
{
	if (LED_NUM<=led_num) return 1;
	*red=LED_COLOR[0][led_num];
	*green=LED_COLOR[1][led_num];
	*blue=LED_COLOR[2][led_num];
	return 0;
}

void led_resetall (void)
{
	u16 i=0;
	for (i=0;i<LED_NUM;i++)
	{
		LED_COLOR[0][i]=0;
		LED_COLOR[1][i]=0;
		LED_COLOR[2][i]=0;
	}
}

void led_setall (u8 red,u8 green,u8 blue)
{
	u16 i=0;
	for (i=0;i<LED_NUM;i++)
	{
		LED_COLOR[0][i]=red;
		LED_COLOR[1][i]=green;
		LED_COLOR[2][i]=blue;
	}
}


void led_senddata (void)
{
	u16 i=0;
	sm16703_reset();
	for (i=0;i<LED_NUM;i++)
	{
		sm16703_send (LED_COLOR[0][i] ,LED_COLOR[1][i] ,LED_COLOR[2][i] );
	}
	LIGHT_1=0;
}



//新板子不使用以前的按键灯光2018.11.5 包装为空函数
/*
void HC595_SendData(void)
{}
*/

//发送数据
void HC595_SendData(void)
{
	//所有的灯都亮  1是亮
//	u8 data=0xff;
	int i,j;
	u8 temp;
	LIGHT_OUT=0;
	for(j=0;j<7;j++)
	{
		if (j==0)
			temp=~data[6-j];//倒序
		else
			temp=data[6-j];//倒序
		for(i=0;i<8;i++)
		{
			LIGHT_SCK=0;
			LIGHT_DATA=temp>>(7-i);
			LIGHT_SCK=1;
		}
	}
	LIGHT_OUT=1;

}

//a表示哪个灯亮b表示是哪个芯片
void on_light(u8 a,u8 b)
{


		data[b-1]|=a;//开大灯
		HC595_SendData();

}

//选择灯怎么亮a是按键状态b是哪个芯片
void off_light(u8 a,u8 b)
{


		data[b-1]&=(~a);//开大灯
		HC595_SendData();


}
//返回0，关，非0，开
u8 GetLight_State(u8 lednum,u8 ic)
{
	u8 ret;
	ret=data[ic-1]&lednum;
	return ret;
}

		//设置亮灯位
void Set_lightdata (u8 *data1)
{
	u16 i=0;
	for (i=0;i<6;i++)
	{
		data[i]=data1[i];
	}
}

		//hoqu亮灯位
void Get_lightdata (u8 *data1)
{
	u16 i=0;
	for (i=0;i<6;i++)
	{
		data1[i]=data[i];
	}
}



void Run_Light (void)
{
	static u8 t=0;
	static u16 len=0;
	static u8 red;
	static u8 green;
	static u8 blue ;
	static u8 led_t=0;
	u8 temp=0xff;
	u8 e=data[6];
	u8 i=0;
	for (i=0;i<7;i++)
	{
		data[i]=0;
	}
	
	if (t>=8) data[(t-8)/8]=(temp<<(7-t%8)); 
		data[t/8]=(temp>>(t%8));
	data[t/8+1]=temp<<(7-t%8);
	data[t/8+2]=temp>>(t%8);
	data[t/8+3]=temp<<(7-t%8);
	data[t/8+4]=temp>>(t%8);
	data[t/8+5]=temp<<(7-t%8);
	data[6]=e;
	
	t++;
	if (t>15) t=0;
	HC595_SendData();
	
	
		led_resetall ();
		led_setcolor (len-1,red*1/10,green*2/10,blue*2/10);
		led_setcolor (len-2,red*2/10,green*5/10,blue*5/10);
		led_setcolor (len-3,red*10/10,green*10/10,blue*10/10);
		led_setcolor (len-4,red*2/10,green*5/10,blue*5/10);
		led_setcolor (len-5,red*1/10,green*2/10,blue*2/10);
		led_senddata();
		len++;
		if (len>30)
		{
			len%=30;
			led_t++;
			if (led_t>=10) led_t=0;
		}
		switch (led_t)
		{
			case 0:
				red=255;
				green =255;
				blue =255;
				break;
			case 1:
				red=255;
				green =255;
				blue =0;
				break;
			case 2:
				red=0;
				green =255;
				blue =255;
				break;
			case 3:
				red=255;
				green =0;
				blue =255;
				break;
			case 4:
				red=255;
				green =0;
				blue =0;
				break;
			case 5:
				red=0;
				green =255;
				blue =0;
				break;
			case 6:
				red=0;
				green =0;
				blue =255;
				break;
			case 7:
				red=128;
				green =30;
				blue =255;
				break;
			case 8:
				red=255;
				green =30;
				blue =128;
				break;
			case 9:
				red=128;
				green =255;
				blue =30;
				break;
			default:
				break;
		}
	
}



					//亮按键灯
void key_light (u8 key_num,u8 state)
{
	u8 t;
	switch (key_num)
	{
		case 1:
			t=LED1;break;
		case 2:
			t=LED2;break;
		case 3:
			t=LED3;break;
		case 4:
			t=LED4;break;
		case 5:
			t=LED5;break;
		case 6:
			t=LED6;break;
		default :
			break;
	}
	if (state)
	{
		on_light(t,LIGHT_LED);
	}
	else
	{
		off_light(t,LIGHT_LED);
	}
}
			//按键周围的灯
void key_around (u8 key_num,u8 state)
{
	u8 data[6]={0};
	u8 tt=0;
			//hoqu亮灯位
	Get_lightdata (data);

	if (state==1)
	{
		tt=0xff;
	}
	else if (state==2)
	{
		tt=0x55;
	}
	else if (state==3)
	{
		tt=0xaa;
	}
	else if (state==4)
	{
		tt=0x3c;
	}
	switch (key_num)
	{
		case 1:
			data[0]&=~0x7f;
			data[1]&=~0x80;
			data[0]|=tt>>1;
			data[1]|=tt<<7;
			break;
		case 2:
			data[0]&=~0x80;
			data[1]&=~0x7f;
			data[0]|=tt<<7;
			data[1]|=tt>>1;
			break;
		case 3:
			data[2]&=~0x3f;
			data[3]&=~0xc0;
			data[2]|=tt>>2;
			data[3]|=tt<<6;
			break;
		case 4:
			data[2]&=~0xc0;
			data[3]&=~0x3f;
			data[2]|=tt<<6;
			data[3]|=tt>>2;
			break;
		case 5:
			data[4]=0;
			data[4]=tt;
			break;
		case 6:
			data[5]=0;
			data[5]=tt;
			break;
		default:
			break;
			
	}
	
	Set_lightdata(data);
	HC595_SendData( );
}


//设置灯光带的颜色
void light_setcolor(u8 red,u8 green,u8 blue)
{
//	led_setall (red,green,blue);
//	led_senddata( );
}



#endif


