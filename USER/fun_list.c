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
	{strlen,"strlen"}, //int strlen ( const char *str) �����ַ�������

	{delay_ms,"delay_ms"},//u32 delay_ms(u16 nms);���뼶��ʱ
	{sleep_ms,"sleep_ms"},//
	{delay_us,"delay_us"},//
	{getSysRunTime,"getSysRunTime"},//u32 getSysRunTime(void); ��ȡϵͳ����ʱ�䣬��λ��
	
	{RTC_SetTimeBySec,"RTC_SetTimeBySec"},//u8 RTC_SetTimeBySec (u32 sec);	//��������������ϵͳʱ��
	{RTC_GetTimeBySec,"RTC_GetTimeBySec"},//u32 RTC_GetTimeBySec (void);		//��ȡϵͳʱ��
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
	
	{CheckTaskUsege,"CheckTaskUsege"},//void CheckTaskUsege ( void ); ͳ������ջʹ�����
	{GetTaskSize,"GetTaskSize"},//u16 GetTaskSize (u8 prio); ��ȡָ�����ȼ������ջ�ռ��С
	{GetTaskUsed,"GetTaskUsed"},//u16 GetTaskUsed (u8 prio); ��ȡָ�����ȼ������ջ�ռ�ʹ����
	{TaskPend,"TaskPend"},//INT8U TaskPend (INT32U prio); �������
	{TaskRepend,"TaskRepend"},//INT8U TaskRepend (INT32U prio); ����ָ�
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
	
	{Beep_On,"Beep_On"},//void Beep_On (u16 _10ms); ����������
	{Beep_Off,"Beep_Off"},//
	{Beep_End,"Beep_End"},//
	{Beep_Set_Multiple,"Beep_Set_Multiple"},//
	{Load_song,"Load_song"},//
	{Load_song1,"Load_song1"},//
	{Load_song2,"Load_song2"},//
	{Load_song3,"Load_song3"},//
	{Load_up,"Load_up"},//
	{Load_down,"Load_down"},//
	{beep_playBy,"beep_playBy"},//u8 beep_playBy (char *buff); ���ػ���������ɲ�����
	
	{Get_Key,"Get_Key"},//
	{Set_Key,"Set_Key"},//u8 Set_Key(u8 keynum,u8 action); ���ð���״̬
	
	{key_setcolor,"key_setcolor"},//
	{key_senddata,"key_senddata"},//
	{light_setcolor,"light_setcolor"},//
	//{Run_Light,"Run_Light"},
	
	{Relay_On,"Relay_On"},//void Relay_On (void); �̵�����
	{Relay_Off,"Relay_Off"},//void Relay_Off (void); �̵�����
	
	{RF1_tx_bytes,"RF1_tx_bytes"},//
	{RF_GetCmd,"RF_GetCmd"},//
	{RF_Get,"RF_Get"},//
	{Get_Mycmd,"Get_Mycmd"},//
	{RF_SetChannel,"RF_SetChannel"},//u8 RF_SetChannel(u8 channel); ��������ģ���ŵ�

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

	{dhcp_retry,"dhcp_retry"},//u8 dhcp_retry(void); �Զ���ȡIP��ַ

	{dns_query,"dns_query"},//uint8 dns_query(uint8 s, uint8 * name,u8 *ip); ��ȡ������IP��ַ

	{udp_send,"udp_send"},//
	{udp_init,"udp_init"},//u8 udp_init(SOCKET s,u16 mycom);  udp��ʼ��
	{tcp_connect,"tcp_connect"},//
	{tcp_send,"tcp_send"},//
	{socket_recv,"socket_recv"},//
	{tcp_close,"tcp_close"},//
	{udp_close,"udp_close"},//u8 udp_close(SOCKET s);
	{socket_close,"socket_close"},//u8 socket_close (SOCKET s); �ж϶˿�״̬���ر�
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

	{ping_auto,"ping_auto"},//u16 ping_auto(uint8 s, uint8 *addr); ��ָ���˿�ping ָ��IP��ַ

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

	{run_beep,"run_beep"},  //u8 run_beep (const char *name_str); �����ļ����beep����
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






