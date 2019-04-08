#ifndef __LCD_H
#define __LCD_H



#ifdef  __cplusplus
	extern "C" {
#endif





				//保存设置页面的数据位
#define WENDU_UPLIMIT  		LCD_DATA[0]
#define WENDU_DOWNLIMIT  	LCD_DATA[1]
#define SHIDU_UPLIMIT  		LCD_DATA[2]
#define SHIDU_DOWNLIMIT  	LCD_DATA[3]
#define JINHUA_UPLIMIT  	LCD_DATA[4]
#define QUHAO  						LCD_DATA[5]
#define LCD_POWER_HS    	LCD_DATA[6]
#define LCD_POWER_SS    	LCD_DATA[7]
#define LCD_POWER_HE    	LCD_DATA[8]
#define LCD_POWER_SE    	LCD_DATA[9]
#define LCD_AUTOSLEEP   	LCD_DATA[10]

				//保存高级设置的数据位
#define CHUSHI_ADDR   		LCD_DATA[10]
#define KONGTIAO_ADDR 		LCD_DATA[11]
#define JINGHUA_ADDR  		LCD_DATA[12]
#define MY_ADDR						LCD_DATA[13]


				//保存手动设置数据位
#define CHUSHI_STATE 			LCD_DATA[14]
#define KONGTIAO_STATE 		LCD_DATA[15]
#define JINHUA_STATE  		LCD_DATA[16]
#define HAND_STATE   			LCD_DATA[17]









void LCD_Init(u32 bound);

void LCD_Send_Data(u8 *buf,u8 len);

void LCD_Receive_Data(u8 *buf,u16 *len);


void LCD_SetFocus(u8 focus);



#ifdef  __cplusplus
	}
#endif





#endif









