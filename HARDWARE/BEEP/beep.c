#include "includes.h"
#include "timer.h"
#include "beep.h"


/**************************************************
		文件说明：蜂鸣器驱动，
		引脚使用：PC13
		外部引用：10us的定时器中断
		实现方法：使用软件的脉宽调制，使用定时器中断实现，
		使用方法：本驱动需要外部引用一个10us的定时器中断服务，本驱动提供的蜂鸣器音乐播放是非阻塞的。
		中断优先级：未使用中断
****************************************************/




static u8 BEEP_BUSY=0;//蜂鸣器正在播放，

//播放蜂鸣器音乐中断
void Beep_Run (void);

void BEEP_Init(void)
{ 
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能GPIOB端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //BEEP-->PB.8 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	 //pc13只能是2M
	GPIO_Init(GPIOC, &GPIO_InitStructure);	 //根据参数初始化GPIOB.8
	RCC_LSEConfig(RCC_LSE_OFF);

	GPIO_ResetBits(GPIOC,GPIO_Pin_13);//输出0，关闭蜂鸣器输出

	//addTimerIrq10us(Beep_Run);
}








//	装载一个歌曲
void Load_song(void)
{
	jianpu *j=mymalloc(sizeof(jianpu)*200);
	jianpu *jj=j;
	set_jianpu(jj,"6","1/1");
	jj++;
	set_jianpu(jj,"2","3/2");
	jj++;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"3","2/1");
	jj++;
	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"1","3/2");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"3","1/1");
	jj++;
	set_jianpu(jj,"6_","5/2");
	


	jj++;
	set_jianpu(jj,"6","1/1");
	jj++;
	set_jianpu(jj,"2","3/2");
	jj++;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"3","2/1");
	jj++;
	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"2","1/1");
	jj++;
	set_jianpu(jj,"6","2/1");
	jj++;
	set_jianpu(jj,"2","1/1");
	jj++;
	set_jianpu(jj,"3","2/1");
	
	
	jj++;
	set_jianpu(jj,"6","1/1");
	jj++;
	set_jianpu(jj,"3","1/1");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"2","2/1");
	jj++;
	set_jianpu(jj,"0","1/1");
	jj++;
	set_jianpu(jj,"1","1/1");
	jj++;
	set_jianpu(jj,"2","3/2");
	jj++;
	set_jianpu(jj,"3","1/1");
	jj++;
	set_jianpu(jj,"6_","5/2");


	jj++;
	set_jianpu(jj,"6","1/1");
	jj++;
	set_jianpu(jj,"3","1/1");
	jj++;
	set_jianpu(jj,"6","1/1");
	jj++;
	set_jianpu(jj,"7","1/1");
	jj++;
	set_jianpu(jj,"3","1/1");
	jj++;
	set_jianpu(jj,"7","1/1");
	jj++;
	set_jianpu(jj,"1~","1/1");
	jj++;
	set_jianpu(jj,"2~","2/1");
	jj++;
	set_jianpu(jj,"7","6/1");


	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"6","2/1");
	jj++;
	set_jianpu(jj,"3","1/1");
	jj++;
	set_jianpu(jj,"7","3/2");
	jj++;
	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"3","1/3");
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"7","1/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","1/1");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"3","2/1");
	
	
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"6","2/1");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"7","1/1");
	jj++;
	set_jianpu(jj,"6","1/1");
	jj++;
	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"2","1/1");
	jj++;
	set_jianpu(jj,"1","1/1");
	jj++;
	set_jianpu(jj,"2","1/1");
	jj++;
	set_jianpu(jj,"3","3/1");
	
	
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"6","2/1");
	jj++;
	set_jianpu(jj,"3","1/1");
	jj++;
	set_jianpu(jj,"7","3/2");
	jj++;
	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"7","1/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","1/1");
	jj++;
	set_jianpu(jj,"3","5/2");


	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"6","2/1");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"3~","1/1");
	jj++;
	set_jianpu(jj,"2~","3/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"1~","2/1");
	jj++;
	set_jianpu(jj,"7","1/1");
	jj++;
	set_jianpu(jj,"6","6/1");

	jj++;
	jianpu_end(jj);

	
	Beep_Play(j);
	myfree(j);
}





void Load_song1(void)
{
	jianpu *j=mymalloc(sizeof(jianpu)*200);
	jianpu *jj=j;
	set_jianpu(jj,"6_","1/2");
	jj++;
	set_jianpu(jj,"1","1/1");
	jj++;
	set_jianpu(jj,"6_","1/2");
	jj++;
	set_jianpu(jj,"3","3/2");
	jj++;
	set_jianpu(jj,"2","1/1");
	jj++;
	set_jianpu(jj,"1","7/2");


	jj++;
	set_jianpu(jj,"7_","1/2");
	jj++;
	set_jianpu(jj,"6_","1/1");
	jj++;
	set_jianpu(jj,"7_","1/2");
	jj++;
	set_jianpu(jj,"2","3/2");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"6_","7/2");


	jj++;
	set_jianpu(jj,"3","2/1");
	jj++;
	set_jianpu(jj,"6","3/2");
	jj++;
	set_jianpu(jj,"3","2/1");
	jj++;
	set_jianpu(jj,"6","1/1");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"4","1/2");
	jj++;
	set_jianpu(jj,"3","2/1");


	jj++;
	set_jianpu(jj,"1","3/2");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"3","1/1");
	jj++;
	set_jianpu(jj,"4","1/2");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"2","4/1");


	jj++;
	set_jianpu(jj,"6_","1/1");
	jj++;
	set_jianpu(jj,"3","1/1");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"6_","1/1");
	jj++;
	set_jianpu(jj,"1","3/1");
	jj++;
	set_jianpu(jj,"3","1/1");


	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"7_","1/2");
	jj++;
	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"6_","1/2");
	jj++;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"3","4/1");


	jj++;
	set_jianpu(jj,"3","2/1");
	jj++;
	set_jianpu(jj,"6","3/2");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"6","1/1");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"4","1/2");
	jj++;
	set_jianpu(jj,"3","2/1");


	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"4","1/2");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"1","1/1");
	jj++;
	set_jianpu(jj,"2","3/1");
	jj++;
	set_jianpu(jj,"3","1/1");


	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"7_","1/2");
	jj++;
	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"7_","1/2");
	jj++;
	set_jianpu(jj,"6_","1/2");
	jj++;
	set_jianpu(jj,"5_","1/1");
	jj++;
	set_jianpu(jj,"6_","3/1");
	jj++;
	set_jianpu(jj,"3","1/1");


	jj++;
	set_jianpu(jj,"5","3/1");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","3/2");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"7","1/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"5","1/1");
	jj++;
	set_jianpu(jj,"6","4/1");


	jj++;
	set_jianpu(jj,"1~","2/1");
	jj++;
	set_jianpu(jj,"7","3/2");
	jj++;
	set_jianpu(jj,"6","3/2");
	jj++;
	set_jianpu(jj,"5","5/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"3","4/1");


	jj++;
	set_jianpu(jj,"1~","2/1");
	jj++;
	set_jianpu(jj,"7","3/2");
	jj++;
	set_jianpu(jj,"6","3/2");
	jj++;
	set_jianpu(jj,"5","5/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"3","4/1");


	jj++;
	set_jianpu(jj,"6_","1/1");
	jj++;
	set_jianpu(jj,"1","1/1");
	jj++;
	set_jianpu(jj,"2","1/1");
	jj++;
	set_jianpu(jj,"3","3/2");
	jj++;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"3","1/1");
	jj++;
	set_jianpu(jj,"2","3/1");


	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"7_","1/2");
	jj++;
	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"7_","1/2");
	jj++;
	set_jianpu(jj,"6_","1/2");
	jj++;
	set_jianpu(jj,"5_","1/1");
	jj++;
	set_jianpu(jj,"6_","4/1");


	jj++;
	set_jianpu(jj,"6_","1/2");
	jj++;
	set_jianpu(jj,"1","1/1");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"3","3/2");
	jj++;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"3","1/1");
	jj++;
	set_jianpu(jj,"2","3/1");
	

	jj++;
	set_jianpu(jj,"3","1/1");
	jj++;
	set_jianpu(jj,"5","2/1");
	jj++;
	set_jianpu(jj,"3","1/1");
	jj++;
	set_jianpu(jj,"7","3/2");
	jj++;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"7","1/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"5","1/1");
	jj++;
	set_jianpu(jj,"6","7/1");
	jj++;
	jianpu_end(jj);
	
	Beep_Play(j);
	myfree(j);
}


void Load_song2(void)
{
	jianpu *j=mymalloc(sizeof(jianpu)*200);
	jianpu *jj=j;
	
	/*
	set_jianpu(jj,"6_","2/2");
	jj++;
	set_jianpu(jj,"7_","1/4");
	jj++;
	set_jianpu(jj,"1","9/4");
	jj++;
	set_jianpu(jj,"2","3/4");
	jj++;
	set_jianpu(jj,"5_","3/4");
	jj++;
	set_jianpu(jj,"6_","1/4");
	jj++;
	set_jianpu(jj,"7_","1/4");
	jj++;
	set_jianpu(jj,"6_","4/2");
	jj++;

	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"6_","2/2");
	jj++;
	set_jianpu(jj,"7_","1/4");
	jj++;
	set_jianpu(jj,"1","9/4");
	jj++;
	set_jianpu(jj,"2","3/2");
	jj++;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"3","2/2");
	jj++;

	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"6_","1/2");
	jj++;
	set_jianpu(jj,"4","3/2");
	jj++;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"2","2/2");
	jj++;
	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"5_","1/2");
	jj++;
	set_jianpu(jj,"3","2/2");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"1","3/2");
	jj++;


	set_jianpu(jj,"0","1/4");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"1","1/4");
	jj++;
	set_jianpu(jj,"6_","1/4");
	jj++;
	set_jianpu(jj,"2","3/4");
	jj++;
	set_jianpu(jj,"0","1/4");
	jj++;
	set_jianpu(jj,"6_","1/4");
	jj++;
	set_jianpu(jj,"1","1/4");
	jj++;
	set_jianpu(jj,"6_","1/4");
	jj++;
	set_jianpu(jj,"7_","5/4");
	jj++;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"6_","9/4");
	jj++;
*/

//	set_jianpu(jj,"0","5/2");
//	jj++;
	set_jianpu(jj,"6_","2/2");
	jj++;
	set_jianpu(jj,"7_","1/4");
	jj++;
	set_jianpu(jj,"1","5/4");
	jj++;
	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"6_","1/4");
	jj++;
	set_jianpu(jj,"1","1/4");
	jj++;
	set_jianpu(jj,"7_","2/2");
	jj++;
	set_jianpu(jj,"5_","1/4");
	jj++;
	set_jianpu(jj,"7_","3/4");
	jj++;
	set_jianpu(jj,"6_","4/2");
	jj++;


	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"6_","2/2");
	jj++;
	set_jianpu(jj,"7_","1/4");
	jj++;
	set_jianpu(jj,"1","5/4");
	jj++;
	set_jianpu(jj,"0","1/2");
	jj++;


	set_jianpu(jj,"6_","1/4");
	jj++;
	set_jianpu(jj,"1","1/4");
	jj++;
	set_jianpu(jj,"2","2/2");
	jj++;
	set_jianpu(jj,"1","1/4");
	jj++;
	set_jianpu(jj,"2","3/4");
	jj++;
	set_jianpu(jj,"3","2/2");
	jj++;


	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"6_","1/2");
	jj++;
	set_jianpu(jj,"4","3/2");
	jj++;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"2","1/4");
	jj++;
	set_jianpu(jj,"1","1/4");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;


	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"5_","1/2");
	jj++;
	set_jianpu(jj,"3","2/2");
	jj++;
	set_jianpu(jj,"2","3/4");
	jj++;
	set_jianpu(jj,"1","5/4");
	jj++;
	set_jianpu(jj,"0","1/4");
	jj++;


	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"1","1/4");
	jj++;
	set_jianpu(jj,"6_","1/4");
	jj++;
	set_jianpu(jj,"2","2/2");
	jj++;
	set_jianpu(jj,"6_","1/4");
	jj++;
	set_jianpu(jj,"1","1/4");
	jj++;
	set_jianpu(jj,"6_","1/4");
	jj++;
	set_jianpu(jj,"1","2/2");
	jj++;
	set_jianpu(jj,"2","1/4");
	jj++;
	set_jianpu(jj,"4","1/2");
	jj++;
	set_jianpu(jj,"3","8/4");
	jj++;
	set_jianpu(jj,"3","5/4");
	jj++;


	set_jianpu(jj,"0","1/2");//绫罗飘起遮住日落西
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"3~","2/2");
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;
	set_jianpu(jj,"6","1/4");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;


	set_jianpu(jj,"0","1/4");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;
	set_jianpu(jj,"7","2/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"5","1/4");
	jj++;
	set_jianpu(jj,"6","5/4");
	jj++;
	
	
	set_jianpu(jj,"0","1/2");//抬起画面如此的魅力
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"3~","2/2");
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;
	set_jianpu(jj,"6","1/4");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;
	

	set_jianpu(jj,"0","1/4");//
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;
	set_jianpu(jj,"7","2/2");
	jj++;
	set_jianpu(jj,"5","1/4");
	jj++;
	set_jianpu(jj,"7","1/2");
	jj++;
	set_jianpu(jj,"6","5/4");
	jj++;


	set_jianpu(jj,"0","1/2");//淡淡胭脂遮住了思绪
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"3~","2/2");
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;
	set_jianpu(jj,"6","1/4");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;
	
	set_jianpu(jj,"0","1/4");//
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;
	set_jianpu(jj,"7","2/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"5","1/4");
	jj++;
	set_jianpu(jj,"3","5/4");
	jj++;


	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"3~","3/4");
	jj++;
	set_jianpu(jj,"5~","1/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"2~","1/4");
	jj++;
	set_jianpu(jj,"6","3/4");
	jj++;
	set_jianpu(jj,"2~","2/2");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"7","3/4");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"6","3/2");
	jj++;
	



	jianpu_end(jj);
	
	Beep_Play(j);
	myfree(j);
}





void Load_song3(void)
{
	jianpu *j=mymalloc(sizeof(jianpu)*300);
	jianpu *jj=j;
	
	set_jianpu(jj,"6","3/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"7","2/2");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"7","1/2");
	jj++;
	set_jianpu(jj,"6","2/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"3","2/2");
	jj++;


	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","3/2");
	jj++;
	set_jianpu(jj,"7","1/2");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"2~","2/2");
	jj++;
	set_jianpu(jj,"3~","1/4");
	jj++;
	set_jianpu(jj,"5~","1/4");
	jj++;
	set_jianpu(jj,"2~","4/2");
	jj++;

	set_jianpu(jj,"0","4/2");
	jj++;
	set_jianpu(jj,"6","3/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"7","2/2");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"7","1/2");
	jj++;
	set_jianpu(jj,"6","2/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"3","2/2");
	jj++;

	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"6","2/2");
	jj++;
	set_jianpu(jj,"5","2/2");
	jj++;
	set_jianpu(jj,"6","4/2");
	jj++;

	set_jianpu(jj,"0","2/2");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"5~","3/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"1~","2/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"3","2/2");
	jj++;


	set_jianpu(jj,"1~","1/2");//哼着陌生乡音走在宫闱里
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"5~","1/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;
	set_jianpu(jj,"6","3/4");
	jj++;
	set_jianpu(jj,"3~","4/2");
	jj++;


	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"5~","3/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"2~","1/4");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"3","2/2");
	jj++;


	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","4/2");
	jj++;


	set_jianpu(jj,"0","2/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"5~","1/2");
	jj++;
	set_jianpu(jj,"6~","1/2");
	jj++;
	set_jianpu(jj,"5~","1/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"2~","2/2");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"2~","2/2");
	jj++;
	set_jianpu(jj,"5~","1/2");
	jj++;
	set_jianpu(jj,"3~","3/2");
	jj++;


	set_jianpu(jj,"0","1/2");//送回多少离人唏嘘
	jj++;
	set_jianpu(jj,"3~","1/4");
	jj++;
	set_jianpu(jj,"5~","1/4");
	jj++;
	set_jianpu(jj,"6~","2/2");
	jj++;
	set_jianpu(jj,"5~","1/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"2~","2/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"5~","1/2");
	jj++;
	set_jianpu(jj,"3~","4/2");
	jj++;


	set_jianpu(jj,"0","2/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"5~","1/2");
	jj++;
	set_jianpu(jj,"6~","2/2");
	jj++;
	set_jianpu(jj,"5~","1/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"2~","2/2");
	jj++;
	set_jianpu(jj,"1~","2/2");
	jj++;
	set_jianpu(jj,"7","2/2");
	jj++;
	set_jianpu(jj,"5","2/2");
	jj++;
	set_jianpu(jj,"1~","2/2");
	jj++;


	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","2/2");
	jj++;
	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"7","1/2");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;
	set_jianpu(jj,"7","1/4");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","6/2");
	jj++;


	jianpu_end(jj);
	Beep_Play(j);
	myfree(j);
}






void Load_up(void)
{
	jianpu *j=mymalloc(sizeof(jianpu)*200);
	jianpu *jj=j;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"4","1/2");
	jj++;
	set_jianpu(jj,"5","1/1");
	jj++;
	jianpu_end(jj);
	Beep_Play(j);
	myfree(j);
}

void Load_down(void)
{
	jianpu *j=mymalloc(sizeof(jianpu)*200);
	jianpu *jj=j;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"4","1/2");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"1","1/1");
	jj++;
	jianpu_end(jj);
	Beep_Play(j);
	myfree(j);
}


//最高8分之一音，音调的频率
static u32 *sond=0;

//time,8是一个全音，16是两个全音
void set_jianpu(jianpu *j,char *tone_,char *time_)
{
	u8 i=0;
	while(tone_[i])
	{
		j->tone[i]=tone_[i];
		i++;
	}
	j->tone[i]=0;//加上数据结尾
	i=0;
	while(time_[i])
	{
		j->time[i]=time_[i];
		i++;
	}
	j->time[i]=0;//加上数据结尾
}

//简谱终了
void jianpu_end(jianpu *j)
{
	j->time[0]=0;
	j->tone[0]=0;
}



static float Multiple=1;


void Beep_Set_Multiple (u32 multiple)
{
	Multiple=multiple/10.0;
}


void Beep_Play (jianpu *jianpu_)
{
	u16 i=0;
	u16 frequency[8]={0,2615/4,2935/4,3295/4,3490/4,3920/4,4400/4,4940/4};
	
	
	for (u8 m=0;m<8;m++)
	{
		frequency[m]=frequency[m]*Multiple;
	}
	
	
	
	if (BEEP_BUSY==1) return;//正在播放，返回
	BEEP_BUSY=1;//设置为忙
	sond=mymalloc(300*4);
	if (sond==0) return;//没有分配到内存，退出
	while(jianpu_->tone[0])
	{
		sond[i*2]=frequency[jianpu_->tone[0]-'0'];
		if (jianpu_->tone[1]=='_')
		{
			sond[i*2]=sond[i*2]/2;
		}
		else if (jianpu_->tone[1]=='~')
		{
			sond[i*2]=sond[i*2]*2;
		}
		if (sond[i*2])
			sond[i*2]=100000/sond[i*2];//设置音调周期
		sond[i*2+1]=8/(jianpu_->time[2]-'0');//以八分之一节拍为最小单位，所有节拍都要是这个时长的倍数
		sond[i*2+1]=sond[i*2+1]*(jianpu_->time[0]-'0');
		sond[i*2+1]*=10000;//设置时长，八分之一音的时长是1/10秒
		jianpu_++;
		i++;
	}
	sond[i*2+1]=0;//标记时长为0，这时播放停止
	//TIM_Cmd(TIM2,ENABLE); 
	addTimerIrq10us(Beep_Run);
}



//播放缓存里的旋律
u8 beep_playBy (char *buff)
{
	u8 tone_len=0;
	char *buff_ptr=buff;
	char *next_ptr=buff_ptr;
	if (buff==0) return 1;
	jianpu *j=mymalloc(sizeof(jianpu)*300);
	jianpu *jj=j;
	while(*next_ptr)
	{
		next_ptr=next_ptr+strlenByChar('{',next_ptr);//重新定位初始指针
		tone_len=strlenByChar('}',next_ptr);
		next_ptr[tone_len]=0;
		next_ptr[strlenByChar(',',next_ptr)]=0;
		set_jianpu(jj,next_ptr+1,next_ptr+tone_len-3);
		jj++;
		next_ptr+=tone_len+1;
	}
	jianpu_end(jj);
	Beep_Play(j);
	myfree(j);
	return 0;
}






//结束蜂鸣器音乐
void Beep_End (void)
{
	myfree(sond);
	sond=0;
	BEEP=0;
	BEEP_BUSY=0;
	delTimerIrq10us(Beep_Run);
}




void Beep_Run(void)
{
	static u16 i=0;
	static u32 time =0;//音调时长计时
	static u32 sondtime=0;//音调一个周期时长计时
	if (sond)
	{
		time++;
		sondtime++;
		if (sondtime>5)//超过周期的一半，
		{
			BEEP=0;
			if (sondtime>=sond[i*2])
			{
				sondtime=0;
				if (sond[i*2]) BEEP=1;
			}
		}
		
		if (time>=sond[i*2+1])//这个音的时长结束
		{
			i++;
			time=0;
			if (sond[i*2+1]==0)
			{
				i=0;
				time=0;
				sondtime=0;
				Beep_End();
			}
		}
	}
	else //没有音频信息，关闭定时器
	{
		i=0;
		time=0;
		sondtime=0;
		Beep_End();
	}
}





static u16 BeepOffTime=0;


void Beep_AutoOff_IRQ (void)
{
	if (BeepOffTime==0)
	{
		BEEP=0;
		delTimerIrq10ms(Beep_AutoOff_IRQ);
	}
	BeepOffTime--;
}








void Beep_On (u16 _10ms)
{
	BEEP=1;
	BeepOffTime=_10ms;
	addTimerIrq10ms(Beep_AutoOff_IRQ);
}


void Beep_Off (void)
{
	BEEP=0;
	delTimerIrq10ms(Beep_AutoOff_IRQ);
}





