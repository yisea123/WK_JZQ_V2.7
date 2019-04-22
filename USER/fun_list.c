#include "includes.h"
#include "cscript.h"
#include "beep.h"




fun_list FUN_LIST[]={
	{(u32)delay_ms,"delay_ms"},
	{(u32)sleep_ms,"sleep_ms"},
	{(u32)delay_us,"delay_us"},
	{(u32)getSysRunTime,"getSysRunTime"},
	
	{(u32)Load_song,"Load_song"},
	{(u32)Beep_End,"Beep_End"},
	
	{(u32)RTC_SetTimeBySec,"RTC_SetTimeBySec"},
	{(u32)RTC_GetTimeBySec,"RTC_GetTimeBySec"},
	
	{(u32)STMFLASH_Write,"STMFLASH_Write"},
	{(u32)STMFLASH_Read,"STMFLASH_Read"},
	
	{(u32)SPI_Flash_Read,"SPI_Flash_Read"},
	{(u32)SPI_Flash_Write,"SPI_Flash_Write"},
	
	{(u32)getMemValue,"getMemValue"},
	{(u32)setMemValue,"setMemValue"},
	{(u32)setMemU32,"setMemU32"},
	{(u32)getMemU32,"getMemU32"},
	{(u32)samestr,"samestr"},
	
	{(u32)mem_perused,"mem_perused"},
	{(u32)mymalloc,"mymalloc"},
	{(u32)myfree,"myfree"},
	{(u32)myrealloc,"myrealloc"},
	
	{(u32)CheckTaskUsege,"CheckTaskUsege"},
	{(u32)GetTaskSize,"GetTaskSize"},
	{(u32)GetTaskUsed,"GetTaskUsed"},
	{(u32)TaskPend,"TaskPend"},
	{(u32)TaskRepend,"TaskRepend"},
	{(u32)TaskSendMsg,"TaskSendMsg"},
	{(u32)TaskGetMsg,"TaskGetMsg"},
	
	{(u32)Save_Config,"Save_Config"},
	{(u32)Load_Config,"Load_Config"},
	{(u32)Save_LCD,"Save_LCD"},
	{(u32)Load_LCD,"Load_LCD"},
	{(u32)GetDeviceType,"GetDeviceType"},
//	{(u32),""},
//	{(u32),""},
//	{(u32),""},
//	{(u32),""},
//	{(u32),""},
//	{(u32),""},
//	{(u32),""},
//	{(u32),""},
//	{(u32),""},
	{0,0}
	
};






