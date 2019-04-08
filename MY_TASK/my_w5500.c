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
			continue;//����û�����ӣ�ִ����һ��ѭ��
		}
		
		if ((net_check_parameters()==0)||//����ip��ַ���Ϸ�
			(checkNetstate(CONFLICT)))//ip��ͻ
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
		my_debug ( );		//������Ϣ���
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







		//�¿ؿͻ����¼�����
void wk_client(void)
{
	u8 m_send[MESSEG_DATA]={0};
	u8 m_recv[MESSEG_DATA]={0};
	static u8 check_gateway_fail_time=0;
	/*�ж���Ϣ����*/
	if (checkSocketState(0,IR_CON))//���ӳɹ�����������Ϣ
	{
			m_send[0]=4;//ģ���յ��˷���������Ϣ
			send_json_adddevice(m_send);
	}
	if (checkSocketState(0,IR_RECV))//��������ݵȴ�����
	{
		server_data ();
	}
	
	
			/*�˿�0������ϴ���*/
	if ((net_get_comstate(0)!=SOCK_ESTABLISHED))//������ӶϿ�
	{		
		if (tcp_connect(0,4545,SERVER_IP,SERVER_PORT)==FALSE)
		{
			if (net_check_gateway()==TRUE)
			{
				DBG_INTER_STATE=1;//����������
				serch_server();//���ҷ�����
				check_gateway_fail_time=0;
			}
			else
			{
				check_gateway_fail_time++;
				if (check_gateway_fail_time>5)
				{
					DBG_INTER_STATE=0;
					if (dhcp_retry()==TRUE)//�Զ���ȡIP��ַ
					{
						check_gateway_fail_time=0;
					}
				}
			}
		}
		else
		{
			DBG_INTER_STATE=2;//�����Ϸ�����
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






















