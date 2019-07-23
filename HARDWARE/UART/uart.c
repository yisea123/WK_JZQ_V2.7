
#include "includes.h"
#include "uart.h"



static u8 *rx_buff_ptr=0;

void uart_init (u32 bound,u8 *rx_buff)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	rx_buff_ptr=rx_buff;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
	
 	USART_DeInit(UART4);
 	USART_DeInit(UART5);

	//PD2,串口5的RX引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	//PC10,串口4的TX引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//UART4 初始化设置
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode =USART_Mode_Tx;
	USART_Cmd(UART4, DISABLE );
	USART_Init(UART4, &USART_InitStructure);
	USART_Cmd(UART4, ENABLE);

	//UART5 初始化设置
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode =USART_Mode_Rx;
	USART_Cmd(UART5, DISABLE );
	USART_Init(UART5, &USART_InitStructure);
	USART_Cmd(UART5, ENABLE);

	//Uart5 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启接收中断
	USART_ITConfig(UART5, USART_IT_IDLE, ENABLE);//开启空闲中断
}




u16 uart_sendBytes (u8 *data,u16 len)
{
	while( len-- )
	{
		while( RESET == USART_GetFlagStatus( UART4, USART_FLAG_TXE ));
		UART4->DR = *data;
		data++;
	}
	return 0;
}

void uart_print (char *str)
{
	uart_sendBytes ((u8 *)str,strlen(str));
}


void UART5_IRQHandler ( void )
{
	static u16 rx_num=0;
	u8 t=0;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	{
		t=UART5->DR;
		if (rx_buff_ptr)
		{
			rx_buff_ptr[rx_num++]=t;
		}
	}
	if(USART_GetITStatus(UART5, USART_IT_IDLE) != RESET)
	{
		if (rx_buff_ptr)
			rx_buff_ptr[rx_num++]=0;
		rx_num=0;
		t=UART5->SR;
		t=UART5->DR;
		TaskSendMsg(10,2);
	}
}



