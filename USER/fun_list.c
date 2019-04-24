#include "includes.h"
#include "cscript.h"
#include "beep.h"
#include "key.h"
#include "lcd.h"
#include "light.h"
#include "relay.h"
#include "rf.h"
#include "timer.h"
#include "hard_irq.h"
#include "w5500.h"
#include "dhcp.h"
#include "dns.h"
#include "enternet.h"
#include "ping.h"
#include "power.h"
#include "ntp.h"
#include "debug.h"


const fun_list FUN_LIST[]={
	{(u32)strlen,"strlen"},

	{(u32)delay_ms,"delay_ms"},
	{(u32)sleep_ms,"sleep_ms"},
	{(u32)delay_us,"delay_us"},
	{(u32)getSysRunTime,"getSysRunTime"},
	
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
	
	{(u32)send_messeg,"send_messeg"},
	{(u32)get_messeg,"get_messeg"},
	
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
	{(u32)GetDeviceState,"GetDeviceState"},
	{(u32)GetDeviceNum,"GetDeviceNum"},
	{(u32)GetCfgData,"GetCfgData"},
	{(u32)GetCJDataAddr,"GetCJDataAddr"},
	{(u32)Get_MyAddr,"Get_MyAddr"},
	{(u32)Set_MyAddr,"Set_MyAddr"},
	{(u32)Set_Device,"Set_Device"},
	{(u32)Set_MyIP,"Set_MyIP"},
	{(u32)Get_MyIP,"Get_MyIP"},
	{(u32)Set_MyChanel,"Set_MyChanel"},
	{(u32)Get_MyChanel,"Get_MyChanel"},
	{(u32)setNativeIP,"setNativeIP"},
	{(u32)setNativePort,"setNativePort"},
	{(u32)setServerIP,"setServerIP"},
	{(u32)setServerPort,"setServerPort"},
	{(u32)setGatewayIP,"setGatewayIP"},
	{(u32)addDevAddr,"addDevAddr"},
	{(u32)delDevAddr,"delDevAddr"},
	{(u32)clearDev,"clearDev"},
	{(u32)getAutoCtrlFrequency,"getAutoCtrlFrequency"},
	{(u32)setAutoCtrlFrequency,"setAutoCtrlFrequency"},
	{(u32)getAutoCtrlAmount,"getAutoCtrlAmount"},
	{(u32)setAutoCtrlAmount,"setAutoCtrlAmount"},
	{(u32)setMyName,"setMyName"},
	{(u32)getMyName,"getMyName"},
	{(u32)getDhcpState,"getDhcpState"},
	{(u32)setDhcpState,"setDhcpState"},
	{(u32)setWarnTolerance,"setWarnTolerance"},
	{(u32)getWarnTolerance,"getWarnTolerance"},
	{(u32)getWkServerConnect,"getWkServerConnect"},
	{(u32)setWkServerConnect,"setWkServerConnect"},
	{(u32)Lcd_GetHandstate,"Lcd_GetHandstate"},
	{(u32)Lcd_GetLimitData,"Lcd_GetLimitData"},
	{(u32)Lcd_SetLimitData,"Lcd_SetLimitData"},
	{(u32)Lcd_SetHandstate,"Lcd_SetHandstate"},
	{(u32)getLcdCfgData,"getLcdCfgData"},
	
	{(u32)Beep_End,"Beep_End"},
	{(u32)Beep_Set_Multiple,"Beep_Set_Multiple"},
	{(u32)Load_song,"Load_song"},
	{(u32)Load_song1,"Load_song1"},
	{(u32)Load_song2,"Load_song2"},
	{(u32)Load_song3,"Load_song3"},
	{(u32)Load_up,"Load_up"},
	{(u32)Load_down,"Load_down"},
	
	{(u32)Get_Key,"Get_Key"},
	{(u32)Set_Key,"Set_Key"},
	
	{(u32)key_setcolor,"key_setcolor"},
	{(u32)key_senddata,"key_senddata"},
	{(u32)light_setcolor,"light_setcolor"},
	
	{(u32)Relay_On,"Relay_On"},
	{(u32)Relay_Off,"Relay_Off"},
	
	{(u32)RF1_tx_bytes,"RF1_tx_bytes"},
	{(u32)RF_GetCmd,"RF_GetCmd"},
	{(u32)RF_Get,"RF_Get"},
	{(u32)Get_Mycmd,"Get_Mycmd"},
	{(u32)RF_SetChannel,"RF_SetChannel"},

	{(u32)addTimerIrq10ms,"addTimerIrq10ms"},
	{(u32)delTimerIrq10ms,"delTimerIrq10ms"},
	{(u32)addTimerIrq10us,"addTimerIrq10us"},
	{(u32)delTimerIrq10us,"delTimerIrq10us"},

	{(u32)USART1_SetFocus,"USART1_SetFocus"},
	{(u32)USART2_SetFocus,"USART2_SetFocus"},
	{(u32)USART1_GetFocus,"USART1_GetFocus"},
	{(u32)USART2_GetFocus,"USART2_GetFocus"},
	{(u32)SOCKET0_SetFocus,"SOCKET0_SetFocus"},
	{(u32)SOCKET1_SetFocus,"SOCKET1_SetFocus"},
	{(u32)SOCKET2_SetFocus,"SOCKET2_SetFocus"},
	{(u32)SOCKET3_SetFocus,"SOCKET3_SetFocus"},
	{(u32)SOCKET4_SetFocus,"SOCKET4_SetFocus"},
	{(u32)SOCKET5_SetFocus,"SOCKET5_SetFocus"},
	{(u32)SOCKET6_SetFocus,"SOCKET6_SetFocus"},
	{(u32)SOCKET7_SetFocus,"SOCKET7_SetFocus"},
	{(u32)SOCKET0_GetFocus,"SOCKET0_GetFocus"},
	{(u32)SOCKET1_GetFocus,"SOCKET1_GetFocus"},
	{(u32)SOCKET2_GetFocus,"SOCKET2_GetFocus"},
	{(u32)SOCKET3_GetFocus,"SOCKET3_GetFocus"},
	{(u32)SOCKET4_GetFocus,"SOCKET4_GetFocus"},
	{(u32)SOCKET5_GetFocus,"SOCKET5_GetFocus"},
	{(u32)SOCKET6_GetFocus,"SOCKET6_GetFocus"},
	{(u32)SOCKET7_GetFocus,"SOCKET7_GetFocus"},

	{(u32)dhcp_retry,"dhcp_retry"},

	{(u32)dns_query,"dns_query"},

	{(u32)udp_send,"udp_send"},
	{(u32)udp_init,"udp_init"},
	{(u32)tcp_connect,"tcp_connect"},
	{(u32)tcp_send,"tcp_send"},
	{(u32)socket_recv,"socket_recv"},
	{(u32)tcp_close,"tcp_close"},
	{(u32)udp_close,"udp_close"},
	{(u32)net_set_getway,"net_set_getway"},
	{(u32)net_set_sub_mask,"net_set_sub_mask"},
	{(u32)net_set_myip,"net_set_myip"},
	{(u32)net_set_disip,"net_set_disip"},
	{(u32)net_get_phycstate,"net_get_phycstate"},
	{(u32)net_get_comstate,"net_get_comstate"},
	{(u32)net_check_gateway,"net_check_gateway"},
	{(u32)net_check_parameters,"net_check_parameters"},
	{(u32)set_ntp_serverip,"set_ntp_serverip"},
	{(u32)set_dns_serverip,"set_dns_serverip"},

	{(u32)ping_auto,"ping_auto"},

	{(u32)ntp_gettime,"ntp_gettime"},
	
	{(u32)SysPowerOff,"SysPowerOff"},
	
	{(u32)dbg_Interpreter,"dbg_Interpreter"},
	{(u32)dbg_info,"dbg_info"},
	{(u32)dbg_err,"dbg_err"},
	{(u32)dbg_devconfig,"dbg_devconfig"},
	{(u32)dbg_help,"dbg_help"},
	{(u32)dbg_reboot,"dbg_reboot"},
	{(u32)dbg_oche,"dbg_oche"},
	{(u32)dbg_booting,"dbg_booting"},
	{(u32)dbg_copydata,"dbg_copydata"},
	{(u32)dbg_getip,"dbg_getip"},
	{(u32)dbg_set,"dbg_set"},
	{(u32)dbg_mqtt,"dbg_mqtt"},
	{(u32)dbg_task,"dbg_task"},
	{(u32)dbg_ping,"dbg_ping"},
	{(u32)dbg_ntp,"dbg_ntp"},
	{(u32)dbg_whos,"dbg_whos"},
	{(u32)dbg_find,"dbg_find"},
	{(u32)dbg_key,"dbg_key"},
	{(u32)dbg_mem,"dbg_mem"},
	{(u32)dbg_print,"dbg_print"},
//	{(u32),""},
//	{(u32),""},
//	{(u32),""},
//	{(u32),""},
//	{(u32),""},
//	{(u32),""},
	{0,0}
	
};






