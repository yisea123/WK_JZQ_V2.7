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
#include "file.h"

#include "my_topmsg.h"
#include "my_lcd.h"
#include "my_rf.h"
#include "my_autocontrol.h"
#include "my_light.h"
#include "my_cfg.h"
#include "my_idle.h"
#include "my_messeg.h"









const fun_list FUN_LIST[]={
	{strlen,"strlen"}, //int strlen ( const char *str) 计算字符串长度

	{delay_ms,"delay_ms"},//u32 delay_ms(u16 nms);毫秒级延时
	{sleep_ms,"sleep_ms"},//
	{delay_us,"delay_us"},//
	{getSysRunTime,"getSysRunTime"},//u32 getSysRunTime(void); 获取系统运行时间，单位秒
	
	{RTC_SetTimeBySec,"RTC_SetTimeBySec"},//u8 RTC_SetTimeBySec (u32 sec);	//用总秒数来设置系统时间
	{RTC_GetTimeBySec,"RTC_GetTimeBySec"},//u32 RTC_GetTimeBySec (void);		//获取系统时间
	{RTC_Get_Week,"RTC_Get_Week"},//
	{RTC_Set,"RTC_Set"},//
	
	{STMFLASH_Write,"STMFLASH_Write"},//
	{STMFLASH_Read,"STMFLASH_Read"},//
	
	{SPI_Flash_Read,"SPI_Flash_Read"},//
	{SPI_Flash_Write,"SPI_Flash_Write"},//
	
	{getMemValue,"getMemValue"},//
	{setMemValue,"setMemValue"},//
	{setMemU32,"setMemU32"},//
	{getMemU32,"getMemU32"},//
	{samestr,"samestr"},//
	
	{send_messeg,"send_messeg"},//
	{get_messeg,"get_messeg"},//
	
	{mem_perused,"mem_perused"},//
	{mymalloc,"mymalloc"},//
	{myfree,"myfree"},//
	{myrealloc,"myrealloc"},//
	
	{CheckTaskUsege,"CheckTaskUsege"},//void CheckTaskUsege ( void ); 统计任务栈使用情况
	{GetTaskSize,"GetTaskSize"},//u16 GetTaskSize (u8 prio); 获取指定优先级任务的栈空间大小
	{GetTaskUsed,"GetTaskUsed"},//u16 GetTaskUsed (u8 prio); 获取指定优先级任务的栈空间使用量
	{TaskPend,"TaskPend"},//INT8U TaskPend (INT32U prio); 任务挂起
	{TaskRepend,"TaskRepend"},//INT8U TaskRepend (INT32U prio); 任务恢复
	{TaskSendMsg,"TaskSendMsg"},//
	{TaskGetMsg,"TaskGetMsg"},//
	{DeleteTask,"DeleteTask"},//
	
	{Save_Config,"Save_Config"},//
	{Load_Config,"Load_Config"},//
	{Save_LCD,"Save_LCD"},//
	{Load_LCD,"Load_LCD"},//
	{GetDeviceType,"GetDeviceType"},//
	{GetDeviceState,"GetDeviceState"},//
	{GetDeviceNum,"GetDeviceNum"},//
	{GetCfgData,"GetCfgData"},//
	{GetCJDataAddr,"GetCJDataAddr"},//
	{Get_MyAddr,"Get_MyAddr"},//
	{Set_MyAddr,"Set_MyAddr"},//
	{Set_Device,"Set_Device"},//
	{Set_MyIP,"Set_MyIP"},//
	{Get_MyIP,"Get_MyIP"},//
	{Set_MyChanel,"Set_MyChanel"},//
	{Get_MyChanel,"Get_MyChanel"},//
	{setNativeIP,"setNativeIP"},//
	{setNativePort,"setNativePort"},//
	{setServerIP,"setServerIP"},//
	{setServerPort,"setServerPort"},//
	{setGatewayIP,"setGatewayIP"},//
	{setSubMask,"setSubMask"},//
	{addDevAddr,"addDevAddr"},//
	{delDevAddr,"delDevAddr"},//
	{clearDev,"clearDev"},//
	{getAutoCtrlFrequency,"getAutoCtrlFrequency"},//
	{setAutoCtrlFrequency,"setAutoCtrlFrequency"},//
	{getAutoCtrlAmount,"getAutoCtrlAmount"},//
	{setAutoCtrlAmount,"setAutoCtrlAmount"},//
	{setMyName,"setMyName"},//
	{getMyName,"getMyName"},//
	{getDhcpState,"getDhcpState"},//
	{setDhcpState,"setDhcpState"},//
	{setWarnTolerance,"setWarnTolerance"},//
	{getWarnTolerance,"getWarnTolerance"},//
	{getWkServerConnect,"getWkServerConnect"},//
	{setWkServerConnect,"setWkServerConnect"},//
	{Lcd_GetHandstate,"Lcd_GetHandstate"},//
	{Lcd_GetLimitData,"Lcd_GetLimitData"},//
	{Lcd_SetLimitData,"Lcd_SetLimitData"},//
	{Lcd_SetHandstate,"Lcd_SetHandstate"},//
	{getLcdCfgData,"getLcdCfgData"},//
	
	{Beep_On,"Beep_On"},//void Beep_On (u16 _10ms); 开启蜂鸣器
	{Beep_Off,"Beep_Off"},//
	{Beep_End,"Beep_End"},//
	{Beep_Set_Multiple,"Beep_Set_Multiple"},//
	{Load_song,"Load_song"},//
	{Load_song1,"Load_song1"},//
	{Load_song2,"Load_song2"},//
	{Load_song3,"Load_song3"},//
	{Load_up,"Load_up"},//
	{Load_down,"Load_down"},//
	{beep_playBy,"beep_playBy"},//u8 beep_playBy (char *buff); 加载缓存里的旋律并播放
	
	{Get_Key,"Get_Key"},//
	{Set_Key,"Set_Key"},//u8 Set_Key(u8 keynum,u8 action); 设置按键状态
	
	{key_setcolor,"key_setcolor"},//
	{key_senddata,"key_senddata"},//
	{light_setcolor,"light_setcolor"},//
	//{Run_Light,"Run_Light"},
	
	{Relay_On,"Relay_On"},//void Relay_On (void); 继电器开
	{Relay_Off,"Relay_Off"},//void Relay_Off (void); 继电器关
	
	{RF1_tx_bytes,"RF1_tx_bytes"},//
	{RF_GetCmd,"RF_GetCmd"},//
	{RF_Get,"RF_Get"},//
	{Get_Mycmd,"Get_Mycmd"},//
	{RF_SetChannel,"RF_SetChannel"},//u8 RF_SetChannel(u8 channel); 设置无线模块信道

	{addTimerIrq10ms,"addTimerIrq10ms"},//
	{delTimerIrq10ms,"delTimerIrq10ms"},//
	{addTimerIrq10us,"addTimerIrq10us"},//
	{delTimerIrq10us,"delTimerIrq10us"},//

	{USART1_SetFocus,"USART1_SetFocus"},//
	{USART2_SetFocus,"USART2_SetFocus"},//
	{USART1_GetFocus,"USART1_GetFocus"},//
	{USART2_GetFocus,"USART2_GetFocus"},//
	{SOCKET0_SetFocus,"SOCKET0_SetFocus"},//
	{SOCKET1_SetFocus,"SOCKET1_SetFocus"},//
	{SOCKET2_SetFocus,"SOCKET2_SetFocus"},//
	{SOCKET3_SetFocus,"SOCKET3_SetFocus"},//
	{SOCKET4_SetFocus,"SOCKET4_SetFocus"},//
	{SOCKET5_SetFocus,"SOCKET5_SetFocus"},//
	{SOCKET6_SetFocus,"SOCKET6_SetFocus"},//
	{SOCKET7_SetFocus,"SOCKET7_SetFocus"},//
	{SOCKET0_GetFocus,"SOCKET0_GetFocus"},//
	{SOCKET1_GetFocus,"SOCKET1_GetFocus"},//
	{SOCKET2_GetFocus,"SOCKET2_GetFocus"},//
	{SOCKET3_GetFocus,"SOCKET3_GetFocus"},//
	{SOCKET4_GetFocus,"SOCKET4_GetFocus"},//
	{SOCKET5_GetFocus,"SOCKET5_GetFocus"},//
	{SOCKET6_GetFocus,"SOCKET6_GetFocus"},//
	{SOCKET7_GetFocus,"SOCKET7_GetFocus"},//

	{dhcp_retry,"dhcp_retry"},//u8 dhcp_retry(void); 自动获取IP地址

	{dns_query,"dns_query"},//uint8 dns_query(uint8 s, uint8 * name,u8 *ip); 获取域名的IP地址

	{udp_send,"udp_send"},//
	{udp_init,"udp_init"},//u8 udp_init(SOCKET s,u16 mycom);  udp初始化
	{tcp_connect,"tcp_connect"},//
	{tcp_send,"tcp_send"},//
	{socket_recv,"socket_recv"},//
	{tcp_close,"tcp_close"},//
	{udp_close,"udp_close"},//u8 udp_close(SOCKET s);
	{socket_close,"socket_close"},//u8 socket_close (SOCKET s); 判断端口状态并关闭
	{net_set_getway,"net_set_getway"},//
	{net_set_sub_mask,"net_set_sub_mask"},//
	{net_set_myip,"net_set_myip"},//
	{net_set_disip,"net_set_disip"},//
	{net_get_phycstate,"net_get_phycstate"},//
	{net_get_comstate,"net_get_comstate"},//
	{net_check_gateway,"net_check_gateway"},//
	{net_check_parameters,"net_check_parameters"},//
	{set_ntp_serverip,"set_ntp_serverip"},//
	{set_dns_serverip,"set_dns_serverip"},//

	{ping_auto,"ping_auto"},//u16 ping_auto(uint8 s, uint8 *addr); 用指定端口ping 指定IP地址

	{ntp_gettime,"ntp_gettime"},//
	
	{SysPowerOff,"SysPowerOff"},//
	
	{dbg_Interpreter,"dbg_Interpreter"},//
	{dbg_info,"dbg_info"},//
	{dbg_err,"dbg_err"},//
	{dbg_devconfig,"dbg_devconfig"},//
	{dbg_help,"dbg_help"},//
	{dbg_reboot,"dbg_reboot"},//
	{dbg_oche,"dbg_oche"},//
	{dbg_booting,"dbg_booting"},//
	{dbg_copydata,"dbg_copydata"},//
	{dbg_getip,"dbg_getip"},//
	{dbg_set,"dbg_set"},//
	{dbg_mqtt,"dbg_mqtt"},//
	{dbg_task,"dbg_task"},//
	{dbg_ping,"dbg_ping"},//
	{dbg_ntp,"dbg_ntp"},//
	{dbg_whos,"dbg_whos"},//
	{dbg_find,"dbg_find"},//
	{dbg_key,"dbg_key"},//
	{dbg_mem,"dbg_mem"},//
	{dbg_print,"dbg_print"},//

	{dbg_swdinit,"dbg_swdinit"},//
	{dbg_swdidcode,"dbg_swdidcode"},//
	{dbg_swdloader,"dbg_swdloader"},//
	{dbg_swdprogram,"dbg_swdprogram"},//
	
	{addSoftTimerIrq10ms,"addSoftTimerIrq10ms"},//
	{delSoftTimerIrq10ms,"delSoftTimerIrq10ms"},//
	{load_test_cfg,"load_test_cfg"},//
	
	{lcd_beep,"lcd_beep"},//
	{lcd_dataUp,"lcd_dataUp"},//
	{LCD_tosleep,"LCD_tosleep"},//
	{LCD_tolight,"LCD_tolight"},//
	{lcd_gettime,"lcd_gettime"},//
	
	{Cmd_0x01,"Cmd_0x01"},//
	{Cmd_0x03,"Cmd_0x03"},//
	{loushui_warn,"loushui_warn"},//
	
	{send_warn,"send_warn"},//
	
	{key_light,"key_light"},//
	{key_around,"key_around"},//
	{led_light,"led_light"},//
	{light_setcolor,"light_setcolor"},//
	{light_runto,"light_runto"},//
	{led_lightall,"led_lightall"},//

	{into_cfg,"into_cfg"},//
	{exit_cfg,"exit_cfg"},//

	{err_to_str,"err_to_str"},//

	{run_beep,"run_beep"},  //u8 run_beep (const char *name_str); 播放文件里的beep音乐
//	{,""},
//	{,""},
//	{,""},
//	{,""},
//	{,""},
//	{,""},
//	{,""},
//	{,""},
//	{,""},
//	{,""},
//	{,""},
//	{,""},
//	{,""},
	{0,0}
	
};






