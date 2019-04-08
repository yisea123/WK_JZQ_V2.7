#ifndef __LED_H
#define __LED_H	 

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 




#ifdef  __cplusplus
	extern "C" {
#endif






#define RF_AUX PAin(8)
#define RF_M0 PAout(12)
#define RF_M1 PAout(11)
#define USART_REC_LEN  			50  	//定义最大接收字节数 200






//超时时间
#define TIME_OUT  10
		
//临时数组长度
#define BUFF_LENGTH 40



//设备状态
#define DEVICEON 				0x0100
#define DEVICEUP 				0x0200
#define DEVICEDOWN 			0x0400
#define DEVICEOFFLINE 	0x0800















void RF_Init(u32 bound);//初始化
u16 RF1_tx_bytes( uint8_t* TxBuffer, uint8_t Length );
u16 RF_GetCmd (uint8_t *buff,u16 len);


				//设置无线配置的时候用
u16 RF_Get (u8 *buff,u16 len);

u16  Get_Mycmd(u8 *data);
void Clear_Mycmd(void);

	//设置信道
u8 RF_SetChannel(u8 channel);







#ifdef  __cplusplus
	}
#endif




#endif
