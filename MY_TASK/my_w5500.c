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
#include "timer.h"
#include "my_topmsg.h"
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



//检查服务器数据
void check_server_data (void);
static u32 W5500_TIMER=0;
void w5500_rest_timer_irq (void);



void my_w5500 (void * t)
{
	cJSON_Hooks a;
	W5500_Initialization();	
	net_test();
	net_sysboot_init();
	a.malloc_fn=mymalloc;
	a.free_fn=myfree;
	cJSON_InitHooks(&a);
	SOCKET0_SetFocus(OS_GET_PRIOHIGH());
	SOCKET2_SetFocus(OS_GET_PRIOHIGH());
	
	//添加定时器
	addTimerIrq10ms(w5500_rest_timer_irq);
	
	void sys_test (void);
	sys_test();
	
	
	u16 rest_time_w5500=0;
	u16 rest_time_sys=0;
	
	u32 *debug_task=mymalloc (4*256);
	CreateTaskN (my_debug_task, 	0, debug_task,		256,		 	 10)	;		

	
	while(1)
	{
		delay_ms(100);
		
		//30分钟没有收到服务器消息则重启
		check_server_data();		
		
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
			//if (getDhcpState())		//自动dhcp开
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
			//else
			{
			}
		}
		wk_client();
	}
}



//调试线程
void my_debug_task ( void *t)
{
	SOCKET1_SetFocus(OS_GET_PRIOHIGH());
	while (1)
	{
		TaskGetMsg();
		my_debug ( );		//调试信息输出
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



//检查服务器数据
void check_server_data (void)
{
	if (checkSocketStateN(0,IR_RECV))
	{
		W5500_TIMER=0;
	}
	else
	{
		if (W5500_TIMER>100*60*30)
		{
			SysPowerOff();
		}
	}
}


//网络断开计时
void w5500_rest_timer_irq (void)
{
	W5500_TIMER++;
}




static u32 AddDev_Delay=0;

void send_adddev_timer_irq (void)
{
	AddDev_Delay++;
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
		AddDev_Delay=0;
		addSoftTimerIrq10ms (send_adddev_timer_irq);
		dbg_print ("端口0 TCP连接成功");
	}
	if (checkSocketState(0,IR_RECV))//如果有数据等待接收
	{
		server_data ();
	}
	if (checkSocketState(0,IR_DISCON))//连接被断开
	{
		socket_close(0);//关闭端口
		delSoftTimerIrq10ms (send_adddev_timer_irq);
		AddDev_Delay=0;
		dbg_print ("端口0 TCP连接被断开");
	}
	if (checkSocketState(0,IR_TIMEOUT))//超时
	{
		socket_close(0);//关闭端口
		delSoftTimerIrq10ms (send_adddev_timer_irq);
		AddDev_Delay=0;
		dbg_print ("端口0 通信超时");
	}
	
	//连接上服务器10秒之后开始发送上线消息
	if (AddDev_Delay>10*100)
	{
		m_send[0]=4;//模拟收到了发送上线消息
		send_json_adddevice(m_send);
		delSoftTimerIrq10ms (send_adddev_timer_irq);
		AddDev_Delay=0;
	}
	
	
	/*端口0网络故障处理*/
	if ((net_get_comstate(0)!=SOCK_ESTABLISHED))//如果连接断开
	{		
		u8 ip[4]={0};u16 port=0;
		net_get_unsenddir (ip,&port); 
		if ((socket_close (0)==TRUE)&&((!SAME_IP(ip,SERVER_IP))||(SERVER_PORT!=port)))//如果目标IP是可以送达的
		{
			if (tcp_connect(0,Get_MyIP()->nativePort,SERVER_IP,SERVER_PORT)==FALSE)
			{
				if (net_check_gateway()==TRUE)
				{
					DBG_INTER_STATE=1;//连接上网关
					//serch_server();//查找服务器
					check_gateway_fail_time=0;
				}
				else
				{
					check_gateway_fail_time++;
					if (check_gateway_fail_time>5)
					{
						DBG_INTER_STATE=0;
						
						//if (getDhcpState())
						{
							if (dhcp_retry()==TRUE)//自动获取IP地址
							{
								check_gateway_fail_time=0;
							}
						}
					}
				}
			}
			else
			{
				DBG_INTER_STATE=2;//连接上服务器
			}
		}
		else
		{
			char *str=mymalloc (256);
			sprintf (str,"端口0 关闭失败或目标地址不可达：%d,%d,%d,%d:%d",ip[0],ip[1],ip[2],ip[3],port);
			dbg_print (str);
			myfree(str);
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






















