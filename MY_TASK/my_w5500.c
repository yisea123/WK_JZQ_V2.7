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

			msgλ���壺
			msg[0],1,���жϣ�2��������״̬��Ϣ����.3�������ڣ�4�����豸������Ϣ��5��ִ�н�����أ�6�����ͻ��������ޣ�7�����ͱ���
			msg[1],������Ϣʱ�����͵��豸����	��
													����ִ�н��ʱ����8λ
													
			msg[2���豸״̬��0�����ߣ�1�����ߣ�
													����ִ�н��ʱ����8λ
													
			�����豸����״̬��Ϣʱ
			������ʱ��
			msg3,id,��8λ
			msg4,id,��8λ
			msg5���豸״̬������״̬��1����
			msg6������״̬��1������2����
			�ɼ���ʱ��
			msg3~6���ɼ������ݻ�������ַ���ɸߵ���,
			msg7-8,����ʱ�ĵ�ַ
			

*********************************************/




//��������״̬�������ã�0��δ���ӣ�1�������������أ�2���������Ϸ�����
u8 DBG_INTER_STATE=0;



//������������
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
	
	//��Ӷ�ʱ��
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
		
		//30����û���յ���������Ϣ������
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
			continue;//����û�����ӣ�ִ����һ��ѭ��
		}
		
		if ((net_check_parameters()==0)||//����ip��ַ���Ϸ�
			(checkNetstate(CONFLICT)))//ip��ͻ
		{
			//if (getDhcpState())		//�Զ�dhcp��
			{
				if (dhcp_retry()==TRUE)//�Զ���ȡIP��ַ
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



//�����߳�
void my_debug_task ( void *t)
{
	SOCKET1_SetFocus(OS_GET_PRIOHIGH());
	while (1)
	{
		TaskGetMsg();
		my_debug ( );		//������Ϣ���
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



//������������
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


//����Ͽ���ʱ
void w5500_rest_timer_irq (void)
{
	W5500_TIMER++;
}




static u32 AddDev_Delay=0;

void send_adddev_timer_irq (void)
{
	AddDev_Delay++;
}





		//�¿ؿͻ����¼�����
void wk_client(void)
{
	u8 m_send[MESSEG_DATA]={0};
	u8 m_recv[MESSEG_DATA]={0};
	static u8 check_gateway_fail_time=0;
	/*�ж���Ϣ����*/
	if (checkSocketState(0,IR_CON))//���ӳɹ�����������Ϣ
	{
		AddDev_Delay=0;
		addSoftTimerIrq10ms (send_adddev_timer_irq);
		dbg_print ("�˿�0 TCP���ӳɹ�");
	}
	if (checkSocketState(0,IR_RECV))//��������ݵȴ�����
	{
		server_data ();
	}
	if (checkSocketState(0,IR_DISCON))//���ӱ��Ͽ�
	{
		socket_close(0);//�رն˿�
		delSoftTimerIrq10ms (send_adddev_timer_irq);
		AddDev_Delay=0;
		dbg_print ("�˿�0 TCP���ӱ��Ͽ�");
	}
	if (checkSocketState(0,IR_TIMEOUT))//��ʱ
	{
		socket_close(0);//�رն˿�
		delSoftTimerIrq10ms (send_adddev_timer_irq);
		AddDev_Delay=0;
		dbg_print ("�˿�0 ͨ�ų�ʱ");
	}
	
	//�����Ϸ�����10��֮��ʼ����������Ϣ
	if (AddDev_Delay>10*100)
	{
		m_send[0]=4;//ģ���յ��˷���������Ϣ
		send_json_adddevice(m_send);
		delSoftTimerIrq10ms (send_adddev_timer_irq);
		AddDev_Delay=0;
	}
	
	
	/*�˿�0������ϴ���*/
	if ((net_get_comstate(0)!=SOCK_ESTABLISHED))//������ӶϿ�
	{		
		u8 ip[4]={0};u16 port=0;
		net_get_unsenddir (ip,&port); 
		if ((socket_close (0)==TRUE)&&((!SAME_IP(ip,SERVER_IP))||(SERVER_PORT!=port)))//���Ŀ��IP�ǿ����ʹ��
		{
			if (tcp_connect(0,Get_MyIP()->nativePort,SERVER_IP,SERVER_PORT)==FALSE)
			{
				if (net_check_gateway()==TRUE)
				{
					DBG_INTER_STATE=1;//����������
					//serch_server();//���ҷ�����
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
							if (dhcp_retry()==TRUE)//�Զ���ȡIP��ַ
							{
								check_gateway_fail_time=0;
							}
						}
					}
				}
			}
			else
			{
				DBG_INTER_STATE=2;//�����Ϸ�����
			}
		}
		else
		{
			char *str=mymalloc (256);
			sprintf (str,"�˿�0 �ر�ʧ�ܻ�Ŀ���ַ���ɴ%d,%d,%d,%d:%d",ip[0],ip[1],ip[2],ip[3],port);
			dbg_print (str);
			myfree(str);
		}
	}



	//ϵͳ����Ϣ����
	if (get_messeg(WK_MESSEG,m_recv)==0)//�������Ϣ
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






















