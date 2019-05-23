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
 *  ��Ի:
 *          д��¥��д�ּ䣬д�ּ������Ա��
 *          ������Աд�������ó��򻻾�Ǯ��
 *          ����ֻ���������������������ߣ�
 *          ��������ո��գ����������긴�ꡣ
 *          ��Ը�������Լ䣬��Ը�Ϲ��ϰ�ǰ��
 *          ���۱������Ȥ���������г���Ա��
 *          ����Ц��߯��񲣬��Ц�Լ���̫����
 *          ��������Ư���ã��ĸ���ó���Ա��
 */









//�����ջ��ʼ�����������Ӳ���жϣ����Խ���Щֵ�Ĵ�
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







 
 
