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






/***
 *  佛曰:
 *          写字楼里写字间，写字间里程序员；
 *          程序人员写程序，又拿程序换酒钱。
 *          酒醒只在网上坐，酒醉还来网下眠；
 *          酒醉酒醒日复日，网上网下年复年。
 *          但愿老死电脑间，不愿鞠躬老板前；
 *          奔驰宝马贵者趣，公交自行程序员。
 *          别人笑我忒疯癫，我笑自己命太贱；
 *          不见满街漂亮妹，哪个归得程序员？
 */









//任务堆栈初始化，如果进入硬件中断，尝试将这些值改大
__align(8) u32 my_keytack[128];
__align(8) u32 my_lcdtack[128];
__align(8) u32 rf_looptack[256];
__align(8) u32 my_lighttack[128];
//u32 my_rf_handtack[256];
//u32 my_rf_dealtack[64];
__align(8) u32 my_w5500tack[256+128];
__align(8) u32 my_autotack[128];
//u32 my_cfgtack[128];
//__align(8) u32 my_iottack[256];
__align(8) u32 my_idletack[256];








int main(void)
{	
	
	SysPowerOn();
	
	
	
	
	
	
	CreateTaskN (my_topmsg, 			0, my_keytack,		128,				0)	;					
	CreateTaskN (my_lcd, 					0, my_lcdtack,		128,				2)	;	 				
	CreateTaskN (my_rf_loop, 			0, rf_looptack,		256,				3)	;	 				
	CreateTaskN (my_light, 				0, my_lighttack,	128,				4)	;		
	CreateTaskN (my_w5500, 				0, my_w5500tack,	256+128,				7)	;		
	CreateTaskN (my_autocontrol, 	0, my_autotack,		128,				8)	;		
//	CreateTaskN (my_iot, 					0, my_iottack,		256,		 	 10)	;		
	CreateTaskN (idle_task, 			0, my_idletack,		 256,			 31)	;		
	OSStart ( ); 
	SysPowerOff();
  
}







 
 
