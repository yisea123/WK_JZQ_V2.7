#include "includes.h"
#include "timer.h"
#include "beep.h"


/**************************************************
		�ļ�˵����������������
		����ʹ�ã�PC13
		�ⲿ���ã�10us�Ķ�ʱ���ж�
		ʵ�ַ�����ʹ�������������ƣ�ʹ�ö�ʱ���ж�ʵ�֣�
		ʹ�÷�������������Ҫ�ⲿ����һ��10us�Ķ�ʱ���жϷ��񣬱������ṩ�ķ��������ֲ����Ƿ������ġ�
		�ж����ȼ���δʹ���ж�
****************************************************/




static u8 BEEP_BUSY=0;//���������ڲ��ţ�

//���ŷ����������ж�
void beep_run_irq (void);

//�����������жϣ�ÿ���жϷ�ת������״̬
void beep_tone_irq ( void);

//���������������жϣ�ÿ���жϲ�����һ������
void beep_10ms_irq (void);


void BEEP_Init(void)
{ 
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��GPIOB�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //BEEP-->PB.8 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	 //pc13ֻ����2M
	GPIO_Init(GPIOC, &GPIO_InitStructure);	 //���ݲ�����ʼ��GPIOB.8
	RCC_LSEConfig(RCC_LSE_OFF);

	GPIO_ResetBits(GPIOC,GPIO_Pin_13);//���0���رշ��������

	//addTimerIrq10us(Beep_Run);
}








//	װ��һ������
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


	set_jianpu(jj,"0","1/2");//���Ʈ����ס������
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
	
	
	set_jianpu(jj,"0","1/2");//̧������˵�����
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


	set_jianpu(jj,"0","1/2");//������֬��ס��˼��
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


	set_jianpu(jj,"1~","1/2");//����İ���������ڹ�����
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


	set_jianpu(jj,"0","1/2");//�ͻض�����������
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


//���8��֮һ����������Ƶ��
static u32 *sond=0;

//time,8��һ��ȫ����16������ȫ��
void set_jianpu(jianpu *j,char *tone_,char *time_)
{
	u8 i=0;
	while(tone_[i])
	{
		j->tone[i]=tone_[i];
		i++;
	}
	j->tone[i]=0;//�������ݽ�β
	i=0;
	while(time_[i])
	{
		j->time[i]=time_[i];
		i++;
	}
	j->time[i]=0;//�������ݽ�β
}

//��������
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





//���������ļ�ΪƵ�ʲ�����
void Beep_Play (jianpu *jianpu_)
{
	u16 i=0;
	u16 frequency[8]={0,2615,2935,3295,3490,3920,4400,4940};
	
	
	for (u8 m=0;m<8;m++)
	{
		frequency[m]=frequency[m]*Multiple;
	}
	
	
	
	if (BEEP_BUSY==1) return;//���ڲ��ţ�����
	BEEP_BUSY=1;//����Ϊæ
	sond=mymalloc(300*4);
	if (sond==0) return;//û�з��䵽�ڴ棬�˳�
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
			sond[i*2]=1000000/sond[i*2];//������������
		sond[i*2+1]=8/(jianpu_->time[2]-'0');//�԰˷�֮һ����Ϊ��С��λ�����н��Ķ�Ҫ�����ʱ���ı���
		sond[i*2+1]=sond[i*2+1]*(jianpu_->time[0]-'0');
		sond[i*2+1]*=10;//����ʱ�����˷�֮һ����ʱ����1/10��
		jianpu_++;
		i++;
	}
	sond[i*2+1]=0;//���ʱ��Ϊ0����ʱ����ֹͣ
	timer2_SetPeriod(sond[0*2]);
	addTimerIrq10us(beep_tone_irq);
	addTimerIrq10ms(beep_10ms_irq);
}



//���Ż����������
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
		next_ptr=next_ptr+strlenByChar('{',next_ptr);//���¶�λ��ʼָ��
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






//��������������
void Beep_End (void)
{
	myfree(sond);
	sond=0;
	delTimerIrq10us(beep_tone_irq);
	delTimerIrq10ms(beep_10ms_irq);
	BEEP=0;
	BEEP_BUSY=0;
}



//�����������жϣ�����ʱ������������
void beep_run_irq(void)
{
	static u16 i=0;
	static u32 time =0;//����ʱ����ʱ
	static u32 sondtime=0;//����һ������ʱ����ʱ
	if (sond)
	{
		time++;
		sondtime++;
		if (sondtime>sond[i*2]/2)//�������ڵ�һ�룬
		{
			BEEP=0;
			if (sondtime>=sond[i*2])
			{
				sondtime=0;
				if (sond[i*2]) BEEP=1;
			}
		}
		
		if (time>=sond[i*2+1])//�������ʱ������
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
	else //û����Ƶ��Ϣ���رն�ʱ��
	{
		i=0;
		time=0;
		sondtime=0;
		Beep_End();
	}
}


//�����������жϣ�ÿ���жϷ�ת������״̬
void beep_tone_irq ( void)
{
	BEEP=!BEEP;
}

//���������������жϣ�ÿ���жϲ�����һ������
void beep_10ms_irq (void)
{
	static u16 i=0;
	static u32 time =0;//����ʱ����ʱ
	time++;
	if (time>=sond[i*2+1])//�������ʱ������
	{
		i++;
		time=0;
		BEEP=0;
		timer2_SetPeriod(sond[i*2]);
		if (sond[i*2+1]==0)
		{
			i=0;
			time=0;
			Beep_End();
		}
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





