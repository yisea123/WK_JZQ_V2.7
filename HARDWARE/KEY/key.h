#ifndef __KEY_H
#define __KEY_H


#ifdef  __cplusplus
	extern "C" {
#endif





//按键长按短按区分
#define PRESS_SHORT 1//短按弹起
#define PRESS_LONG 2	//达到长按
#define PRESS_DOWN 3//按下
#define PRESS_NONE 0




void Key_Init (void);


u8 Get_Key(u8 keynum);
		
//设置按键状态，用于虚拟按键,0,成功，1,失败
u8 Set_Key(u8 keynum,u8 action);

void KEY_IRQHandler(void) ;





#ifdef  __cplusplus
	}
#endif







#endif




