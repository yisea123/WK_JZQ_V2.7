#include "includes.h"
#include "hard_irq.h"
#include "lcd.h"

/**************************************************
		�ļ�˵����������������
		����ʹ�ã�PA2���������������PA3�������������룻PB12��485ʱ������������ƽ�
		�ⲿ���ã���
		ʵ�ַ������жϽ��յ����棬����ʽ����
		ʹ�÷�����ʹ�ö�ȡ������ȡ���յ������ݣ����ͺ�������

****************************************************/







#define MY_USART USART2
#define USART_485_TX_EN		PBout(12)	//485ģʽ����.0,����;1,����.
#define USART2_RX_BUFLEN  30

//���ջ����� 	
static u8 USART2_RX_BUF[USART2_RX_BUFLEN];  	//���ջ���,���64���ֽ�.

//���յ������ݳ���
static u8 USART2_RX_CNT=0;   		  

//���ڿ��б�־λ
static u8 USART2_IDLE=1;
  
	
	
	
	
void USART2_IRQHandler(void)
{
	u8 res;	    
 
 	if(USART_GetITStatus(MY_USART, USART_IT_RXNE) != RESET) //���յ�����
	{	  
		res =USART_ReceiveData(MY_USART); 	//��ȡ���յ�������
		if ((USART2_RX_CNT==0)&&(USART2_IDLE==1))
		{
			USART2_IDLE=0;
		}
		if (USART2_IDLE==0)
		{
			if(USART2_RX_CNT<USART2_RX_BUFLEN)
			{
				USART2_RX_BUF[USART2_RX_CNT]=res;			//��¼���յ���ֵ
				USART2_RX_CNT++;											//������������1 
			}
		}
	}  											 
	if(USART_GetITStatus(MY_USART, USART_IT_IDLE) != RESET)  //�����ж�
	{
		res=MY_USART->SR;
		res=MY_USART->DR;
		res=0;
		USART2_IDLE=1;
		TaskIntSendMsg(0,SYS_MSG_USART2);//���͸��������
	}
} 


void LCD_SetFocus(u8 focus)
{
//	if (focus<TASK_MAX_NUM)
//		LCD_FOCUS=focus;
}



void LCD_Init(u32 bound)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��GPIOA,Gʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��GPIOA,Gʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //PG9�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	//PA2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);  

	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE);//��λ����2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,DISABLE);//ֹͣ��λ


	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;///��żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ

	USART_Init(MY_USART, &USART_InitStructure);  //��ʼ������

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; //ʹ�ܴ���2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	USART_ITConfig(MY_USART, USART_IT_RXNE, ENABLE);//�����ж�
	USART_ITConfig(MY_USART, USART_IT_IDLE, ENABLE);//���������ж�

	USART_Cmd(MY_USART, ENABLE);                    //ʹ�ܴ��� 


	USART_485_TX_EN=0;			//Ĭ��Ϊ����ģʽ

}



//RS485����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
void LCD_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	USART_485_TX_EN=1;			//����Ϊ����ģʽ
	for(t=0;t<len;t++)		//ѭ����������
	{		   
		while(USART_GetFlagStatus(MY_USART, USART_FLAG_TC) == RESET);	  
		USART_SendData(MY_USART,buf[t]);
	}	 
	while(USART_GetFlagStatus(MY_USART, USART_FLAG_TC) == RESET);		
	USART_485_TX_EN=0;				//����Ϊ����ģʽ	
}



//RS485��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
void LCD_Receive_Data(u8 *buf,u16 *len)
{
	u8 rxlen=USART2_RX_CNT;
	*len=USART2_RX_CNT;				//Ĭ��Ϊ0
	if (USART2_IDLE==1)
	{
		mymemcpy (buf,USART2_RX_BUF,rxlen);
		mymemset (USART2_RX_BUF,0,rxlen);
		USART2_RX_CNT=0;
	}
}

















