#include "includes.h"
#include "w5500.h"
#include "enternet.h"
#include "dhcp.h"
#include "my_messeg.h"
#include "wk_json.h"
#include "debug.h"
#include "ntp.h"
#include "power.h"
#include "hard_irq.h"
#include "my_w5500.h"


/*******************************************

			msg位定义：
			msg[0],1,有中断，2，有运行状态消息发送.3重启网口，4发送设备上线消息，5，执行结果返回，6，发送环境上下限，7，发送报警
			msg[1],发送消息时，发送的设备类型	，
													返回执行结果时，高8位
													
			msg[2，设备状态，0，在线，1，离线，
													返回执行结果时，低8位
													
			发送设备运行状态消息时
			控制器时：
			msg3,id,高8位
			msg4,id,低8位
			msg5，设备状态，控制状态，1，开
			msg6，控制状态，1，升，2，降
			采集器时：
			msg3~6，采集器数据缓存区地址，由高到低,
			msg7-8,离线时的地址
			

*********************************************/




//网络连接状态，调试用，0，未连接，1，已连接上网关，2，已连接上服务器
u8 DBG_INTER_STATE=0;








void my_w5500 (void * t)
{
	cJSON_Hooks a;
	W5500_Initialization();	
	net_test();
	net_sysboot_init();
	a.malloc_fn=mymalloc;
	a.free_fn=myfree;
	cJSON_InitHooks(&a);
	SOCKET0_SetFocus(OSPrioHighRdy);
	SOCKET1_SetFocus(OSPrioHighRdy);
	SOCKET2_SetFocus(OSPrioHighRdy);
	
	void sys_test (void);
	sys_test();
	
	
	u16 rest_time_w5500=0;
	u16 rest_time_sys=0;
	
	
	while(1)
	{
		delay_ms(100);
		if (net_get_phycstate()==0) 
		{
			rest_time_w5500++;
			if (rest_time_w5500>=10*30)
			{
				W5500_Initialization();
				net_sysboot_init();
				rest_time_w5500=0;
				rest_time_sys++;
				if (rest_time_sys>=2*10)
				{
					//SysPowerOff();
				}
			}
			continue;//网线没有连接，执行下一次循环
		}
		
		if ((net_check_parameters()==0)||//本机ip地址不合法
			(checkNetstate(CONFLICT)))//ip冲突
		{
			if (dhcp_retry()==TRUE)//自动获取IP地址
			{
				DBG_INTER_STATE=1;
			}
			else
			{
				DBG_INTER_STATE=0;
			}
		}
		my_debug ( );		//调试信息输出
		wk_client();
	}
}





void sys_test (void)
{
	int test=0;
	u32 flashaddr=0;
	u8 *buff;
	buff=mymalloc(2048);
	while (test)
	{
		SPI_Flash_Read (buff,flashaddr,2048);
		
	}
	myfree(buff);
}







		//温控客户端事件处理
void wk_client(void)
{
	u8 m_send[MESSEG_DATA]={0};
	u8 m_recv[MESSEG_DATA]={0};
	static u8 check_gateway_fail_time=0;
	/*中断消息处理*/
	if (checkSocketState(0,IR_CON))//连接成功发送上线消息
	{
			m_send[0]=4;//模拟收到了发送上线消息
			send_json_adddevice(m_send);
	}
	if (checkSocketState(0,IR_RECV))//如果有数据等待接收
	{
		server_data ();
	}
	
	
			/*端口0网络故障处理*/
	if ((net_get_comstate(0)!=SOCK_ESTABLISHED))//如果连接断开
	{		
		if (tcp_connect(0,4545,SERVER_IP,SERVER_PORT)==FALSE)
		{
			if (net_check_gateway()==TRUE)
			{
				DBG_INTER_STATE=1;//连接上网关
				serch_server();//查找服务器
				check_gateway_fail_time=0;
			}
			else
			{
				check_gateway_fail_time++;
				if (check_gateway_fail_time>5)
				{
					DBG_INTER_STATE=0;
					if (dhcp_retry()==TRUE)//自动获取IP地址
					{
						check_gateway_fail_time=0;
					}
				}
			}
		}
		else
		{
			DBG_INTER_STATE=2;//连接上服务器
		}
	}



	//系统内消息处理
	if (get_messeg(WK_MESSEG,m_recv)==0)//如果有消息
	{
		if (DBG_INTER_STATE==2) 
		{
			switch (m_recv[0])
			{
				case 2:
					send_json (m_recv);
					break;
				case 3:
					net_test();
					net_sysboot_init();
					break;
				case 4:
					send_json_adddevice(m_recv);
					break;
				case 6:
					send_json_limit (m_recv);
					break;
				case 7:
					send_json_warn(m_recv); 
					break;
				default:
					break;
			}
		}
	}

}






















