#ifndef __LED_H
#define __LED_H	 

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 




#ifdef  __cplusplus
	extern "C" {
#endif






#define RF_AUX PAin(8)
#define RF_M0 PAout(12)
#define RF_M1 PAout(11)
#define USART_REC_LEN  			50  	//�����������ֽ��� 200






//��ʱʱ��
#define TIME_OUT  10
		
//��ʱ���鳤��
#define BUFF_LENGTH 40



//�豸״̬
#define DEVICEON 				0x0100
#define DEVICEUP 				0x0200
#define DEVICEDOWN 			0x0400
#define DEVICEOFFLINE 	0x0800















void RF_Init(u32 bound);//��ʼ��
u16 RF1_tx_bytes( uint8_t* TxBuffer, uint8_t Length );
u16 RF_GetCmd (uint8_t *buff,u16 len);


				//�����������õ�ʱ����
u16 RF_Get (u8 *buff,u16 len);

u16  Get_Mycmd(u8 *data);
void Clear_Mycmd(void);

	//�����ŵ�
u8 RF_SetChannel(u8 channel);







#ifdef  __cplusplus
	}
#endif




#endif
