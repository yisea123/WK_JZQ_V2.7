#include "includes.h"
#include "power.h"
#include "w5500.h"
#include "enternet.h"
#include "dns.h"
#include "ping.h"
#include "ntp.h"
#include "baidu_iot.h"
#include "my_idle.h"
#include "wk_json.h"
#include "cmd.h"
#include "iwdg.h"
#include "key.h"
#include "debug.h"








static u16 NativeDbgPort=12;
static u8 DBG_IAP=0;
void my_debug (void)
{
	
	if (net_get_comstate(1)!=SOCK_UDP)
	{
		if (udp_init(1,NativeDbgPort))
		{
			//dbg_booting();
		}
	}
	
	do
	{
		if (checkSocketState(1,IR_RECV))//��������ݵȴ�����
		{
			u8 *recvbuff=mymalloc(2048);
			memset(recvbuff,0,2048);//����ڴ��е�����
			Read_SOCK_Data_Buffer(1, recvbuff);

			//�������õ�������Ŀ���ַ
			mymemcpy(DBG_IP,recvbuff,4);
			DBG_PORT=(recvbuff[4]<<8)|recvbuff[5];

			if (DBG_PORT==7010)
			{
				IWDG_Feed();
				cmd_byudp (recvbuff+8); 
			}
			else 
			{
				dbg_Interpreter(recvbuff);//���������
			}
			myfree(recvbuff);
		}
	}while (DBG_IAP);
}

/*****************************************************

						��������������ʹ����ڴ��������ִ��
						help����ʾ������Ϣ
						sysinfo����ʾϵͳ��Ϣ
						setip�������������


******************************************************/

static u8 DBG_OCHE =0;//����
u8 DBG_IP[4]={255,255,255,255};//�����õ�Ŀ��ip��ַ
u16 DBG_PORT=7000;//�����õĶ˿ں�
void dbg_Interpreter(u8 *recvbuff)
{

	if (DBG_OCHE)
	{
		udp_send(1,DBG_IP,DBG_PORT,(u8*)recvbuff+8,strlen((const char *)recvbuff+8));
		udp_send(1,DBG_IP,DBG_PORT,(u8*)"\r\n",2);
	}
	
	
	if (samestr((u8*)"sysinfo",recvbuff+8))
	{
		dbg_info();
	}
	else if (samestr((u8*)"help",recvbuff+8))
	{
		dbg_help();
	}
	else if (samestr((u8*)"devconfig",recvbuff+8))
	{
		dbg_devconfig();
	}
	else if (samestr((u8*)"reboot",recvbuff+8))
	{
		dbg_reboot(); 
	}
	else if (samestr((u8*)"oche ",recvbuff+8))
	{
		dbg_oche(recvbuff+8+5); 
	}
	else if (samestr((u8*)"copy ",recvbuff+8))
	{
		dbg_copydata(recvbuff+8+5); 
	}
	else if (samestr((u8*)"getip ",recvbuff+8))
	{
		dbg_getip(recvbuff+8+6); 
	}
	else if (samestr((u8*)"set",recvbuff+8))
	{
		dbg_set(recvbuff+8+3); 
	}
	else if (samestr((u8*)"mqtt",recvbuff+8))
	{
		dbg_mqtt(recvbuff+8); 
	}
	else if (samestr((u8*)"task ",recvbuff+8))
	{
		dbg_task(recvbuff+8+5); 
	}
	else if (samestr((u8*)"ping ",recvbuff+8))
	{
		dbg_ping(recvbuff+8+5); 
	}
	else if (samestr((u8*)"ntp",recvbuff+8))
	{
		dbg_ntp(recvbuff+8+3); 
	}
	else if (samestr((u8*)"whos",recvbuff+8))
	{
		dbg_whos(recvbuff+8+3); 
	}
	else if (samestr((u8*)"find ",recvbuff+8))
	{
		dbg_find(recvbuff+8+5); 
	}
	else if (samestr((u8*)"key",recvbuff+8))
	{
		dbg_key(recvbuff+8+3); 
	}
	else
	{
		dbg_err(1);
	}

	
}





void dbg_oche (u8 *databuff)
{
	char * ptxt=0;

	if (samestr((u8*)"on",databuff))
	{
		DBG_OCHE=1;
		ptxt="�ѿ�������\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		
	}
	else if (samestr((u8*)"off",databuff))
	{
		DBG_OCHE=0;
		ptxt="�ѹرջ���\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	}
}




	//������Ϣ
void dbg_info (void)
{
	char * ptxt=0;
	char *txtbuff=mymalloc(512);
	ptxt="�¿ؼ�������";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
  sprintf(txtbuff,"%.18s\r\n",getMyName());
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));
	
	ptxt="�����������·��汾��2018-10-22\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen(ptxt));
	
	sprintf(txtbuff,"����IP��ַ��%d.%d.%d.%d\r\n",IP_Addr[0],IP_Addr[1],IP_Addr[2],IP_Addr[3]);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf(txtbuff,"����MAC��ַ��%02X.%02X.%02X.%02X.%02X.%02X\r\n",Phy_Addr[0],Phy_Addr[1],Phy_Addr[2],Phy_Addr[3],Phy_Addr[4],Phy_Addr[5]);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf(txtbuff,"������IP��ַ��%d.%d.%d.%d:%d\r\n",SERVER_IP[0],SERVER_IP[1],SERVER_IP[2],SERVER_IP[3],SERVER_PORT);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf(txtbuff,"����IP��ַ��%d.%d.%d.%d\r\n",Gateway_IP[0],Gateway_IP[1],Gateway_IP[2],Gateway_IP[3]);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf(txtbuff,"DNS������IP��ַ��%d.%d.%d.%d\r\n",DNS_SERVER[0],DNS_SERVER[1],DNS_SERVER[2],DNS_SERVER[3]);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf(txtbuff,"NTP������IP��ַ��%d.%d.%d.%d\r\n",NTP_SERVER[0],NTP_SERVER[1],NTP_SERVER[2],NTP_SERVER[3]);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	ptxt="��������״̬��";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	if (DBG_INTER_STATE==0) ptxt="û����������\r\n"; else if (DBG_INTER_STATE==1) ptxt="������������\r\n";
	else if (DBG_INTER_STATE==2) ptxt="�������Ϸ�����\r\n"; else ptxt="δ֪������״̬\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	sprintf (txtbuff,"�����ŵ���%d\r\n",Get_MyChanel());
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf (txtbuff,"�Զ�����Ƶ�ʣ�%d ��\r\n",getAutoCtrlFrequency());
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf (txtbuff,"�Զ����Ƴ�������%d\r\n",getAutoCtrlAmount());
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf (txtbuff,"�¿ر����ݲ�ֵ��%d\r\n",getWarnTolerance());
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf(txtbuff,"������������ %d ��\r\n",getSysRunTime());
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));
	
	sprintf(txtbuff,"ϵͳʱ�䣺-- %d�� - %d�� - %d�� -- %d : %d : %d -- \r\n",
		RTC_GetTimeObj()->w_year,RTC_GetTimeObj()->w_month,RTC_GetTimeObj()->w_date,
		RTC_GetTimeObj()->hour,RTC_GetTimeObj()->min,RTC_GetTimeObj()->sec);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	ptxt="�ⲿFLASH��";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	if (SPI_FLASH_TYPE==0XEF13) ptxt="1MB\r\n"; else if (SPI_FLASH_TYPE==0XEF14) ptxt="2MB\r\n"; 
	else if (SPI_FLASH_TYPE==0XEF15) ptxt="4MB\r\n"; else if (SPI_FLASH_TYPE==0XEF16) ptxt="8MB\r\n";
	else if (SPI_FLASH_TYPE==0XEF17) ptxt="16MB\r\n"; else ptxt="δ֪������\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	ptxt="�ļ�ϵͳ״̬��";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	if (DBG_FATS==0) ptxt="��֧���ļ�ϵͳ\r\n"; else if (DBG_FATS==1) ptxt="û��SD��\r\n"; 
	else if (DBG_FATS==2) ptxt="SD������ʧ��\r\n"; else if (DBG_FATS==3) ptxt="֧���ļ�ϵͳ\r\n";
	else ptxt="δ֪״̬\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	sprintf (txtbuff,"ϵͳ�ڴ�ʹ�������%dKB�ܹ���%dKB��ʹ�á�%dKBʣ�ࡢʹ����%d%%\r\n",
		memsize/1024,memsize*mem_perused()/100/1024,memsize*(100-mem_perused())/100/1024,mem_perused());
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));
	
	sprintf(txtbuff,"����λ�ã�-- %#X -- \r\n",SCB->VTOR);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf(txtbuff,"����ʱ�䣺%s ---- %s\r\n",__DATE__,__TIME__);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	if (IAP_Support)
	{
		sprintf(txtbuff,"IAP����汾��-- %s --\r\n",IAP_Version);
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));
			
		sprintf(txtbuff,"IAP�������ʱ�䣺---- %s ----\r\n",IAP_CompileTime);
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));
	}
	else
	{
		
		sprintf(txtbuff,"��֧��IAP����");
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));		
		
	}
	myfree(txtbuff);
}


void dbg_err (u8 errtype)
{
	char * ptxt=0;
	ptxt="��֧�ֵ��������\"help\"��ȡ������\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
}



//��ȡ�豸״̬�ַ������λ��0����ʾ����״̬����1,2��λ��ʾ����״̬����3��λ��ʾ����״̬,
//��4��5,6���������Դ�������7��������
void dbg_getdevstate(u8 state,char *txtbuff)
{
	if (state&0x08)
	{
		memcpy(txtbuff,"���ߡ�",6);
		txtbuff+=6;
		txtbuff[0]=0;
		return ;//����״̬��������������
	}
	else
	{
		memcpy(txtbuff,"���ߡ�",6);
		txtbuff+=6;
	}
	if (state&0x01)
	{
		memcpy(txtbuff,"�豸������",10);
		txtbuff+=10;
	}
	else
	{
		memcpy(txtbuff,"�豸�رա�",10);
		txtbuff+=10;
		txtbuff[0]=0;
		return ;
	}
	
	if (state&0x02)
	{
		memcpy(txtbuff,"������",6);
		txtbuff+=6;
	}
	else if (state&0x04)
	{
		memcpy(txtbuff,"������",6);
		txtbuff+=6;
	}
	else if (state&(0x06))
	{
		memcpy(txtbuff,"�ǳ���״̬",10);
		txtbuff+=10;
	}
	else
	{
	}
	txtbuff[0]=0;
}

//��ȡ�豸����״̬
void dbg_devconfig(void)
{
	u16 i=0;
	char * ptxt=0;
	char *txtbuff=mymalloc(512);
	char *txttemp=mymalloc(512);
	ptxt="�豸������Ϣ���£�\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	for (i=0;EN_CONFIG[i*2];i++)
	{
		dbg_getdevstate(EN_CONFIG[i*2+1]>>8,txttemp);
		sprintf(txtbuff,"\t�豸��%s\t��ַ��%-8d\t�豸״̬��%s\r\n",dbg_getdevname(EN_CONFIG[i*2+1]&0x00ff),EN_CONFIG[i*2],txttemp);
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
	}
	myfree(txttemp);
	myfree(txtbuff);
}

//��ʾ������Ϣ
void dbg_help(void)
{
	char * ptxt=0;
	char *txtbuff=mymalloc(512);
	ptxt="������\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	ptxt="\t����\"copy host on\"��ʼ��������λ�������ݽ��������˿�\
			\r\n\t����\"copy host on [�˿ں�]\"��ʼ��������λ�������ݽ�����ָ���Ķ˿�\r\n\t����\"copy host off\"ֹͣ\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	ptxt="\t����\"devconfig\"��ȡ�豸������Ϣ\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	ptxt="\t����\"getip [����]\"��ȡ������Ӧ��IP��ַ\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	ptxt="\t����\"key [��ֵ] [����]\"ģ�ⰴ���¼�\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	ptxt="\t����\"reboot\"�豸����\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	ptxt="\t����\"mqtt\"���ӵ��ٶ���\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	ptxt="\t����\"ntp\"��ȡ����ʱ��\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	ptxt="\t����\"oche on\"��������\r\n\t����\"oche off\"�رջ���\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	ptxt="\t����\"ping [IP]\"ʹ������ Ping ��[IP]Ϊ��ַ������\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	ptxt="\t����\"sysinfo\"��ȡ������Ϣ\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	ptxt="\t����\"set [������] [����]\"�޸ļ�����������\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	ptxt="\t����\"task getidle\"��ѯ�����쳣������\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	ptxt="\t����\"task getusege\"��ѯ����ջʹ�����\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	ptxt="\t��㲥��ַ����\"whos\"��ѯ��������ļ�����\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	ptxt="\t��㲥��ַ����\"find [����\\���]\"����ָ�����ƻ�ָ����ŵļ�����\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));


	
	myfree(txtbuff);
}


void dbg_reboot (void)
{
	char * ptxt=0;
	ptxt="�豸������������\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	SysPowerOff();
}


void dbg_booting(void)
{
	char *txtbuff=mymalloc(512);
	sprintf (txtbuff,"���Ϊ %d ���¿ؼ���������������\r\n",Get_MyAddr());
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
	
	myfree(txtbuff);
	
}

//�������ݵ����Զ˿�
void dbg_copydata (u8 *buff)
{
	u16 port=0;
	char *txtbuff=mymalloc(512);
	if ( samestr((u8*)"host on",buff))
	{
		
		if (DBG_COPY_TO_S1CK)
		{
			sprintf (txtbuff,"�������ڽ����У���\r\n");
		}
		else
		{
			port=str2num(buff+8);
			if (port!=0)
			{
				if (tcp_connect(2,30,DBG_IP,port)==TRUE)
				{
					DBG_COPY_TO_S1CK=2;
					sprintf (txtbuff,"��ʼ�������������������ݵ��˿� %d����\r\n",port);
				}
				else
				{
					tcp_close(2);
					sprintf (txtbuff,"�뱣֤����Ķ˿ں��Ѵ�ΪTCP Server!!!\r\n");
				}
			}
			else
			{
				DBG_COPY_TO_S1CK=1;
				sprintf (txtbuff,"��ʼ�������������������ݵ����˿ڡ���\r\n");
			}
		}
	}
	else if ( samestr((u8*)"host off",buff))
	{
		if (DBG_COPY_TO_S1CK==2)
		{
			tcp_close(2);
		}
		DBG_COPY_TO_S1CK=0;
		sprintf (txtbuff,"ֹͣ�������������������ݡ���\r\n");
	}
	else
	{
	}
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
	myfree(txtbuff);
}















void dbg_getip(u8 *buff)
{
	u8 getip[4]={0};
	if (DBG_COPY_TO_S1CK) 
	{
		udp_send(1,DBG_IP,DBG_PORT,(u8*)"���ȹر�\"copy\"�����\r\n",22);
		return;
	}
	if (dns_query(2,buff,getip))
	{
		char *txt=mymalloc(200);
		sprintf (txt,"������%s ��IP��ַ�ǣ�%d.%d.%d.%d\r\n",buff,getip[0],getip[1],getip[2],getip[3]);
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txt,strlen((const char *)txt));
		myfree(txt);
	}
	else
	{
		udp_send(1,DBG_IP,DBG_PORT,(u8*)"��ȡIP��ַʧ��T_T\r\n",17);
	}
	
}



void dbg_mqtt(u8 *buff)
{
	u8 ret=0;
	ret=mqtt_connect("rahher9.mqtt.iot.gz.baidubce.com",1883,"two","rahher9/two","KGa5JL87iGCheRFF");
	char *txt=mymalloc(512);
	sprintf(txt,"������Ϊ��%d\r\n",ret);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txt,strlen(txt));
	myfree(txt);
}





void dbg_task (u8 *buff)
{
	u32 lasttime=0;u32 dietimes=0;
	char *txtbuff=mymalloc(512);
	if ( samestr((u8*)"getidle",buff))
	{
		sprintf(txtbuff,"�����쳣������%8X\r\n",getIdleTask());
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));
		for (u8 i=0;i<TASK_MAX_NUM;i++)
		{
			if (GetTaskUsed(i))
			{
				getKilledTask(&lasttime,&dietimes,i);
				sprintf(txtbuff,"���ȼ�Ϊ %2d ������������ %2d �Σ����һ������ʱ���ǣ�%d\r\n",i,dietimes,lasttime);
				udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));
			}
		}
	}
	else if ( samestr((u8*)"getusege",buff))
	{
		sprintf(txtbuff,"����ջʹ�������\r\n");
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));
		for (u8 i=0;i<TASK_MAX_NUM;i++)
		{
			if (GetTaskUsed(i))
			{
				sprintf(txtbuff,"���ȼ�Ϊ %2d ջʹ�����Ϊ %2d /%d\r\n",i,GetTaskUsed(i),GetTaskSize(i)  );
				udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));
			}
		}
	}
	myfree(txtbuff);
}



void dbg_ping (u8 *buff)
{
	u8 getip[4]={0};
	u16 pingtime=0;
	getnumfstr(getip,buff,'.',4);
	if (DBG_COPY_TO_S1CK) 
	{
		udp_send(1,DBG_IP,DBG_PORT,(u8*)"���ȹر�\"copy\"�����\r\n",22);
		return;
	}
	pingtime=ping_auto(2,getip);
	if (pingtime!=0xffff)
	{
		char *txt=mymalloc(200);
		sprintf (txt,"IP��ַ %d.%d.%d.%d �������ӳ��� %d ms\r\n",getip[0],getip[1],getip[2],getip[3],pingtime);
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txt,strlen((const char *)txt));
		myfree(txt);
	}
	else
	{
		udp_send(1,DBG_IP,DBG_PORT,(u8*)"Ping ����ʧ��T_T\r\n",18);
	}
	
}





void dbg_set (u8 *chars)
{ 
	char *txtbuff=mymalloc(512);
	char *ptxt=0;
	if (*chars++==' ')
	{
		if (samestr((u8*)"channel ",chars))
		{
			if (Set_MyChanel(str2num(chars+8)))
			{
				sprintf(txtbuff,"����ʧ�ܣ�����������ŵ�Ϊ %d������0~31��Χ�ڡ���\r\n",str2num(chars));
			}
			else
			{
				Save_Config();
				sprintf(txtbuff,"�����������ŵ�Ϊ %d����������Ч����\r\n",Get_MyChanel());
			}
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		}
		else if (samestr((u8*)"nativeip ",chars))
		{
			u8 getip[4]={0};
			getnumfstr(getip,chars+9,'.',4);
			setNativeIP (getip);
			Save_Config();
			sprintf (txtbuff,"�����ñ���IP��ַΪ��%d.%d.%d.%d\r\n",getip[0],getip[1],getip[2],getip[3]);
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		}
		else if (samestr((u8*)"nativeport ",chars))
		{
			u16 port=0;
			port=str2num(chars+11);
			setNativePort (port);
			Save_Config();
			sprintf (txtbuff,"�����ñ����˿�Ϊ��%d\r\n",port);
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		}
		else if (samestr((u8*)"serverip ",chars))
		{
			u8 getip[4]={0};
			getnumfstr(getip,chars+9,'.',4);
			setServerIP (getip);
			Save_Config();
			sprintf (txtbuff,"�����÷�����IP��ַΪ��%d.%d.%d.%d\r\n",getip[0],getip[1],getip[2],getip[3]);
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		}
		else if (samestr((u8*)"serverport ",chars))
		{
			u16 port=0;
			port=str2num(chars+11);
			setServerPort (port);
			Save_Config();
			sprintf (txtbuff,"�����÷������˿�Ϊ��%d\r\n",port);
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		}
		else if (samestr((u8*)"gatewayip ",chars))
		{
			u8 getip[4]={0};
			getnumfstr(getip,chars+10,'.',4);
			setGatewayIP (getip);
			Save_Config();
			sprintf (txtbuff,"����������IP��ַΪ��%d.%d.%d.%d\r\n",getip[0],getip[1],getip[2],getip[3]);
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		}
		else if (samestr((u8*)"dbgport ",chars))
		{
			u16 port=0;
			port=str2num(chars+8);
			NativeDbgPort=port;
			udp_init(1,NativeDbgPort);
			sprintf (txtbuff,"�����ü������ĵ��Զ˿�Ϊ��%d �����ӵ��¶˿�ͨ��\r\n",port);
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		}
		else if (samestr((u8*)"nativeid ",chars))
		{
			u16 id=0;
			id=str2num(chars+9);
			Set_MyAddr (id);
			Save_Config();
			sprintf (txtbuff,"�����ü��������豸��ַΪ��%d\r\n",id);
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		}
		else if (samestr((u8*)"adddev",chars))
		{
			void dbg_set_adddev (u8 *chars);
			dbg_set_adddev(chars+6);
		}
		else if (samestr((u8*)"deldev ",chars))
		{
			u16 id=0;
			id=str2num(chars+7);
			if (delDevAddr(id)==0)
			{
				Save_Config();
				sprintf (txtbuff,"���Ƴ���ַΪ %d ���豸\r\n",id);
			}
			else
			{
				sprintf (txtbuff,"�Ƴ���ַΪ %d ���豸ʧ�ܣ������ǲ������������豸\r\n",id);
			}
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		}
		else if (samestr((u8*)"cleardev",chars))
		{
			clearDev();
			Save_Config();
			sprintf (txtbuff,"���Ƴ��������õ��豸\r\n");
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		}
		else if (samestr((u8*)"ctrlfrequency ",chars))
		{
			u16 frequency=0;
			frequency=str2num(chars+14);
			if (setAutoCtrlFrequency(frequency)==0)
			{
				Save_Config();
				sprintf (txtbuff,"�������Զ����Ƶ�Ƶ��Ϊ %d ��\r\n",frequency);
			}
			else
			{
				sprintf (txtbuff,"�Զ����Ƶ�Ƶ������ʧ��\r\n");
			}
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		}
		else if (samestr((u8*)"ctrlamount ",chars))
		{
			u16 amount=0;
			amount=str2num(chars+11);
			if (setAutoCtrlAmount(amount)==0)
			{
				Save_Config();
				sprintf (txtbuff,"�������Զ����Ƶĳ�����Ϊ %d \r\n",amount);
			}
			else
			{
				sprintf (txtbuff,"�Զ����Ƴ���������ʧ�ܣ������ǳ�����ֵ̫��\r\n");
			}
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		}
		else if (samestr((u8*)"warntolerance ",chars))
		{
			u16 amount=0;
			amount=str2num(chars+14);
			if (setWarnTolerance(amount)==0)
			{
				Save_Config();
				sprintf (txtbuff,"�������¿ر����ݲ�ֵΪ %d \r\n",amount);
			}
			else
			{
				sprintf (txtbuff,"�¿ر����ݲ�ֵ����ʧ�ܣ�������ֵ̫��\r\n");
			}
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		}
		else if (samestr((u8*)"name ",chars))
		{
			if (setMyName((char *)chars+5)==0)
			{
				Save_Config();
				sprintf (txtbuff,"�������Զ����Ƶ�����Ϊ %s\r\n",chars+5);
			}
			else
			{
				sprintf (txtbuff,"���ü�����������ʧ�ܣ�����������̫��\r\n");
			}
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		}
		else
		{
			ptxt="�ݲ�֧�ֵ���������������� \"set\" �鿴֧�ֵĲ���\r\n";
			udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		}
	}
	else
	{
		ptxt="���������˵����\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		
		ptxt="\t����\"set channel [�ŵ�]\"�޸ļ������������ŵ�\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

		ptxt="\t����\"set nativeip [IP]\"�޸ļ�������IP��ַ\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

		ptxt="\t����\"set nativeport [�˿�]\"�޸ļ�����������˿�\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

		ptxt="\t����\"set serverip [IP]\"�޸ķ�������IP��ַ\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

		ptxt="\t����\"set serverport [�˿�]\"�޸ķ������Ķ˿ں�\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

		ptxt="\t����\"set gatewayip [IP]\"�޸ļ�����������IP��ַ\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		
		ptxt="\t����\"set dbgport [�˿�]\"�޸ļ������ĵ��Զ˿�\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		
		ptxt="\t����\"set nativeid [��������ַ]\"���ü������ĵ�ַ\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

		ptxt="\t����\"set adddev [�豸����] [�豸��ַ]\"��Ӽ������豸\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		
		ptxt="\t����\"set deldev [�豸��ַ]\"�Ƴ��������豸\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

		ptxt="\t����\"set cleardev\"�Ƴ������������õ������豸\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

		ptxt="\t����\"set ctrlfrequency [Ƶ��]\"���ü������Զ����Ƶ�Ƶ�ʣ���λ��\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		
		ptxt="\t����\"set ctrlamount [������]\"���ü������Զ����Ƶĳ�����\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

		ptxt="\t����\"set warntolerance [�ݲ�ֵ]\"���ü������¿ر����ݲ�ֵ\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

		ptxt="\t����\"set name [����]\"���ü���������\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	}
	myfree(txtbuff);
}



void dbg_set_adddev (u8 *chars)
{
	char *txtbuff=mymalloc(512);
	char *ptxt=0;
	if (*chars==' ')
		chars++;
	if (samestr((u8*)"cjq ",chars))
	{
		u16 id=0;
		id=str2num(chars+4);
		if (addDevAddr (devTypeCjq, id)==0)
		{
			Save_Config();
			sprintf (txtbuff,"����ӵ�ַΪ��%d �Ĳɼ���\r\n",id);
		}
		else
		{
			sprintf (txtbuff,"����豸ʧ�ܣ��������豸��ַ��ռ��\r\n");
		}
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
	}
	else if (samestr((u8*)"kt ",chars))
	{
		u16 id=0;
		id=str2num(chars+3);
		if (addDevAddr (devTypeKt, id)==0)
		{
			Save_Config();
			sprintf (txtbuff,"����ӵ�ַΪ��%d �Ŀյ�������\r\n",id);
		}
		else
		{
			sprintf (txtbuff,"����豸ʧ�ܣ��������豸��ַ��ռ��\r\n");
		}
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
	}
	else if (samestr((u8*)"csj ",chars))
	{
		u16 id=0;
		id=str2num(chars+4);
		if (addDevAddr (devTypeCsj, id)==0)
		{
			Save_Config();
			sprintf (txtbuff,"����ӵ�ַΪ��%d �ĳ�ʪ��������\r\n",id);
		}
		else
		{
			sprintf (txtbuff,"����豸ʧ�ܣ��������豸��ַ��ռ��\r\n");
		}
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
	}
	else if (samestr((u8*)"jhq ",chars))
	{
		u16 id=0;
		id=str2num(chars+4);
		if (addDevAddr (devTypeJhq, id)==0)
		{
			Save_Config();
			sprintf (txtbuff,"����ӵ�ַΪ��%d �ľ�����\r\n",id);
		}
		else
		{
			sprintf (txtbuff,"����豸ʧ�ܣ��������豸��ַ��ռ��\r\n");
		}
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
	}
	else if (samestr((u8*)"jsj ",chars))
	{
		u16 id=0;
		id=str2num(chars+4);
		if (addDevAddr (devTypeJsj, id)==0)
		{
			Save_Config();
			sprintf (txtbuff,"����ӵ�ַΪ��%d �ļ�ʪ��\r\n",id);
		}
		else
		{
			sprintf (txtbuff,"����豸ʧ�ܣ��������豸��ַ��ռ��\r\n");
		}
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
	}
	else if (samestr((u8*)"ytj ",chars))
	{
		u16 id=0;
		id=str2num(chars+4);
		if (addDevAddr (devTypeYtj, id)==0)
		{
			Save_Config();
			sprintf (txtbuff,"����ӵ�ַΪ��%d ��һ���������\r\n",id);
		}
		else
		{
			sprintf (txtbuff,"����豸ʧ�ܣ��������豸��ַ��ռ��\r\n");
		}
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
	}
	else
	{
		ptxt="��֧�ֵ��豸���ͣ�����֧�ֵ��豸�����ǣ�\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		
		ptxt="\t�ɼ�����\tcjq\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		ptxt="\t�յ���������\tkt\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		ptxt="\t��ʪ����\tcsj\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		ptxt="\t��������\tjhq\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		ptxt="\t��ʪ����\tjsj\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		ptxt="\tһ�����\tytj\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	}
	myfree(txtbuff);
}


//ͨ��ntp��ȡʱ��
void dbg_ntp (u8 *buff)
{
	u32 time=0;
	char *chars=mymalloc(128);
	char *ptxt=0;
	time=ntp_gettime(2,NTP_SERVER);
	if (time!=0)
	{
		RTC_SetTimeBySec(time+8*3600-0x83AA7E80);//�����й���ʱ������ȥ1900��1970��ʱ���
		sprintf (chars,"��ȡ����ʱ���ǣ�%u\r\n",time);
		udp_send(1,DBG_IP,DBG_PORT,(u8*)chars,strlen((const char *)chars));
	}
	else
	{
		ptxt="��ȡʱ��ʧ�ܣ�\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	}
	myfree(chars);
}



void dbg_whos(u8 *buff)
{
	char *chars=mymalloc(128);
	sprintf (chars,"��� %d �ļ�����IP��ַ�� %d.%d.%d.%d\r\n",Get_MyAddr(),IP_Addr[0],IP_Addr[1],IP_Addr[2],IP_Addr[3]);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)chars,strlen((const char *)chars));
	myfree(chars);
}


void dbg_find(u8 *buff)
{
	char *chars=mymalloc(128);
	if (samestr((u8*)getMyName(),buff))
	{
		sprintf (chars,"%s �ı���� %d ��IP��ַ�� %d.%d.%d.%d\r\n",buff,Get_MyAddr(),IP_Addr[0],IP_Addr[1],IP_Addr[2],IP_Addr[3]);
		udp_send(1,DBG_IP,DBG_PORT,(u8*)chars,strlen((const char *)chars));
	}
	else if (str2num(buff)==Get_MyAddr())
	{
		sprintf (chars,"��� %d �������� %s��IP��ַ�� %d.%d.%d.%d\r\n",Get_MyAddr(),getMyName(),IP_Addr[0],IP_Addr[1],IP_Addr[2],IP_Addr[3]);
		udp_send(1,DBG_IP,DBG_PORT,(u8*)chars,strlen((const char *)chars));
	}
	myfree(chars);
}




void dbg_key(u8 *buff)
{
	char *txtbuff=mymalloc(512);
	char *ptxt=0;
	if (*buff++==' ')
	{
		buff[1]=0;
		u8 key=str2num(buff);
		u8 action=PRESS_NONE;
		if (samestr("short",buff+2))
		{
			action=PRESS_SHORT;
		}
		else if (samestr("long",buff+2))
		{
			action=PRESS_LONG;
		}
		else
		{
			
		}
		if (Set_Key (key,action)==0)
		{
			ptxt="���ü�ֵ�ɹ�\r\n";
			udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		}
		else
		{
			ptxt="���ü�ֵʧ��\r\n";
			udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		}
	}
	else
	{
		ptxt="��ֵȡֵ��Χ�ǣ�1~6\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

		ptxt="�����ǣ��̰���short \\������long\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	}
	myfree(txtbuff);
}








//ר�Ÿ�cmd�������װ�ĺ���
void cmd_send (u8 *data,u16 datalen )
{
	udp_send(1,DBG_IP,DBG_PORT,data,datalen);
}


//ͨ�����紫���������������
void	cmd_byudp (u8 *cmd)
{
	if ((cmd[1]==0xff)&&(cmd[0]==0xff))
	{
		Get_cmd (cmd_send,cmd);
		DBG_IAP=1;
	}
}







