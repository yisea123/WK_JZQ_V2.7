#include "includes.h"
#include "power.h"
#include "my_rf.h"
#include "my_light.h"
#include "my_key.h"
#include "my_lcd.h"
#include "my_w5500.h"
#include "my_autocontrol.h"
#include "my_cfg.h"
#include "my_iot.h"
#include "my_idle.h"
#include "my_topmsg.h"


//任务堆栈初始化，如果进入硬件中断，尝试将这些值改大
__align(8) u32 my_keytack[128];
__align(8) u32 my_lcdtack[128];
__align(8) u32 rf_looptack[256];
__align(8) u32 my_lighttack[128];
//u32 my_rf_handtack[256];
//u32 my_rf_dealtack[64];
__align(8) u32 my_w5500tack[256];
__align(8) u32 my_autotack[128];
//u32 my_cfgtack[128];
__align(8) u32 my_iottack[256];
__align(8) u32 my_idletack[64];





u32 bx_test (u32 a,u32 b,u32 c,u32 d,u32 e,u32 f,u32 g,u32 h)
{
	u32 i=0;
	i=a+b+c+d+e+f+g+h;
	return i;
}






int main(void)
{	
	
	SysPowerOn();
	
	
	
	bx_test(1,2,3,4,5,6,7,8);
	
	
	
	
	
	
	CreateTaskN (my_topmsg, 			0, my_keytack,		128,				0)	;					
	CreateTaskN (my_lcd, 					0, my_lcdtack,		128,				2)	;	 				
	CreateTaskN (my_rf_loop, 			0, rf_looptack,		256,				3)	;	 				
	CreateTaskN (my_light, 				0, my_lighttack,	128,				4)	;		
	CreateTaskN (my_w5500, 				0, my_w5500tack,	256,				7)	;		
	CreateTaskN (my_autocontrol, 	0, my_autotack,		128,				8)	;		
	CreateTaskN (my_iot, 					0, my_iottack,		256,		 	 10)	;		
	CreateTaskN (idle_task, 			0, my_idletack,		 64,			 31)	;		
	OSStart ( ); 
	SysPowerOff();
  
}







 
 
