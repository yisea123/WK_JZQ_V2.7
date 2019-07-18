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
#include "sysinfo.h"
#include "cScript.h"
#include "swd.h"
#include "debug.h"



#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
int _sys_exit(int x) 
{ 
	x = x; 
	return 0;
} 

void _ttywrch(int ch)
{
}


//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0){}//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 






//dbg�����ַ�������ָ��
void (*dbg_sendstr) (char *str);

static u16 NativeDbgPort=12;
static u8 DBG_IAP=0;
void my_debug (void)
{
	
	if (net_get_comstate(1)==SOCK_CLOSED)
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
				dbg_Interpreter(recvbuff+8,dbg_send_udp);//���������
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
void dbg_Interpreter(u8 *recvbuff,void (*sendstr) (char *str))
{

	dbg_sendstr=sendstr;
	if (DBG_OCHE)
	{
		dbg_sendstr((char *)recvbuff);
		dbg_sendstr("\r\n");
	}
	
	
	if (samestr((u8*)"sysinfo",recvbuff))
	{
		dbg_info();
	}
	else if (samestr((u8*)"help",recvbuff))
	{
		dbg_help();
	}
	else if (samestr((u8*)"devconfig",recvbuff))
	{
		dbg_devconfig();
	}
	else if (samestr((u8*)"reboot",recvbuff))
	{
		dbg_reboot(); 
	}
	else if (samestr((u8*)"oche ",recvbuff))
	{
		dbg_oche(recvbuff+5); 
	}
	else if (samestr((u8*)"copy ",recvbuff))
	{
		dbg_copydata(recvbuff+5); 
	}
	else if (samestr((u8*)"getip ",recvbuff))
	{
		dbg_getip(recvbuff+6); 
	}
	else if (samestr((u8*)"set",recvbuff))
	{
		dbg_set(recvbuff+3); 
	}
	else if (samestr((u8*)"mqtt",recvbuff))
	{
		dbg_mqtt(recvbuff); 
	}
	else if (samestr((u8*)"task ",recvbuff))
	{
		dbg_task(recvbuff+5); 
	}
	else if (samestr((u8*)"ping ",recvbuff))
	{
		dbg_ping(recvbuff+5); 
	}
	else if (samestr((u8*)"ntp",recvbuff))
	{
		dbg_ntp(recvbuff+3); 
	}
	else if (samestr((u8*)"whos",recvbuff))
	{
		dbg_whos(recvbuff+3); 
	}
	else if (samestr((u8*)"find ",recvbuff))
	{
		dbg_find(recvbuff+5); 
	}
	else if (samestr((u8*)"key",recvbuff))
	{
		dbg_key(recvbuff+3); 
	}
	else if (samestr((u8*)"mem",recvbuff))
	{
		dbg_mem(recvbuff+3); 
	}
	else if (samestr((u8*)"fun",recvbuff))
	{
		dbg_fun(recvbuff+3); 
	}
	else if (samestr((u8*)"run",recvbuff))
	{
		dbg_run(recvbuff+3); 
	}
	else if (samestr((u8*)"swd",recvbuff))
	{
		dbg_swd(recvbuff+3); 
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
		dbg_sendstr((char*)ptxt);
		
	}
	else if (samestr((u8*)"off",databuff))
	{
		DBG_OCHE=0;
		ptxt="�ѹرջ���\r\n";
		dbg_sendstr((char*)ptxt);
	}
}




	//������Ϣ
void dbg_info (void)
{
	char * ptxt=0;
	char *txtbuff=mymalloc(512);
	ptxt="�¿ؼ�������";
	dbg_sendstr((char*)ptxt);
  sprintf(txtbuff,"%.18s\r\n",getMyName());
	dbg_sendstr((char*)txtbuff);
	
  sprintf(txtbuff,"����汾��%.18s\r\n",JZQ_SoftVersion);
	dbg_sendstr((char*)txtbuff);

  sprintf(txtbuff,"����Ӳ���汾��%.18s\r\n",JZQ_Version); 
	dbg_sendstr((char*)txtbuff);

	sprintf(txtbuff,"����IP��ַ��%d.%d.%d.%d\r\n",IP_Addr[0],IP_Addr[1],IP_Addr[2],IP_Addr[3]);
	dbg_sendstr((char*)txtbuff);

	sprintf(txtbuff,"����MAC��ַ��%02X.%02X.%02X.%02X.%02X.%02X\r\n",Phy_Addr[0],Phy_Addr[1],Phy_Addr[2],Phy_Addr[3],Phy_Addr[4],Phy_Addr[5]);
	dbg_sendstr((char*)txtbuff);

	sprintf(txtbuff,"������IP��ַ��%d.%d.%d.%d:%d\r\n",SERVER_IP[0],SERVER_IP[1],SERVER_IP[2],SERVER_IP[3],SERVER_PORT);
	dbg_sendstr((char*)txtbuff);

	sprintf(txtbuff,"����IP��ַ��%d.%d.%d.%d\r\n",Gateway_IP[0],Gateway_IP[1],Gateway_IP[2],Gateway_IP[3]);
	dbg_sendstr((char*)txtbuff);

	sprintf(txtbuff,"DNS������IP��ַ��%d.%d.%d.%d\r\n",DNS_SERVER[0],DNS_SERVER[1],DNS_SERVER[2],DNS_SERVER[3]);
	dbg_sendstr((char*)txtbuff);

	sprintf(txtbuff,"NTP������IP��ַ��%d.%d.%d.%d\r\n",NTP_SERVER[0],NTP_SERVER[1],NTP_SERVER[2],NTP_SERVER[3]);
	dbg_sendstr((char*)txtbuff);

	ptxt="��������״̬��";
	dbg_sendstr((char*)ptxt);
	if (DBG_INTER_STATE==0) ptxt="û����������\r\n"; else if (DBG_INTER_STATE==1) ptxt="������������\r\n";
	else if (DBG_INTER_STATE==2) ptxt="�������Ϸ�����\r\n"; else ptxt="δ֪������״̬\r\n";
	dbg_sendstr((char*)ptxt);
	
	sprintf (txtbuff,"�����ŵ���%d\r\n",Get_MyChanel());
	dbg_sendstr((char*)txtbuff);

	sprintf (txtbuff,"�Զ�����Ƶ�ʣ�%d ��\r\n",getAutoCtrlFrequency());
	dbg_sendstr((char*)txtbuff);

	sprintf (txtbuff,"�Զ����Ƴ�������%d\r\n",getAutoCtrlAmount());
	dbg_sendstr((char*)txtbuff);

	sprintf (txtbuff,"�¿ر����ݲ�ֵ��%d\r\n",getWarnTolerance());
	dbg_sendstr((char*)txtbuff);

	sprintf(txtbuff,"������������ %d ��\r\n",getSysRunTime());
	dbg_sendstr((char*)txtbuff);
	
	sprintf(txtbuff,"ϵͳʱ�䣺-- %d�� - %d�� - %d�� -- %d : %d : %d -- \r\n",
		RTC_GetTimeObj()->w_year,RTC_GetTimeObj()->w_month,RTC_GetTimeObj()->w_date,
		RTC_GetTimeObj()->hour,RTC_GetTimeObj()->min,RTC_GetTimeObj()->sec);
	dbg_sendstr((char*)txtbuff);

	ptxt="�ⲿFLASH��";
	dbg_sendstr((char*)ptxt);
	if (SPI_FLASH_TYPE==0XEF13) ptxt="1MB\r\n"; else if (SPI_FLASH_TYPE==0XEF14) ptxt="2MB\r\n"; 
	else if (SPI_FLASH_TYPE==0XEF15) ptxt="4MB\r\n"; else if (SPI_FLASH_TYPE==0XEF16) ptxt="8MB\r\n";
	else if (SPI_FLASH_TYPE==0XEF17) ptxt="16MB\r\n"; else ptxt="δ֪������\r\n";
	dbg_sendstr((char*)ptxt);
	
	ptxt="�ļ�ϵͳ״̬��";
	dbg_sendstr((char*)ptxt);
	if (DBG_FATS==0) ptxt="��֧���ļ�ϵͳ\r\n"; else if (DBG_FATS==1) ptxt="û��SD��\r\n"; 
	else if (DBG_FATS==2) ptxt="SD������ʧ��\r\n"; else if (DBG_FATS==3) ptxt="֧���ļ�ϵͳ\r\n";
	else ptxt="δ֪״̬\r\n";
	dbg_sendstr((char*)ptxt);

	sprintf (txtbuff,"ϵͳ�ڴ�ʹ�������%dKB�ܹ���%dKB��ʹ�á�%dKBʣ�ࡢʹ����%d%%\r\n",
		memsize/1024,memsize*mem_perused()/100/1024,memsize*(100-mem_perused())/100/1024,mem_perused());
	dbg_sendstr((char*)txtbuff);
	
	sprintf(txtbuff,"����λ�ã�-- %#X -- \r\n",SCB->VTOR);
	dbg_sendstr((char*)txtbuff);

	sprintf(txtbuff,"����ʱ�䣺%s ---- %s\r\n",__DATE__,__TIME__);
	dbg_sendstr((char*)txtbuff);

	if (IAP_Support)
	{
		sprintf(txtbuff,"IAP����汾��-- %s --\r\n",IAP_Version);
		dbg_sendstr((char*)txtbuff);
			
		sprintf(txtbuff,"IAP�������ʱ�䣺---- %s ----\r\n",IAP_CompileTime);
		dbg_sendstr((char*)txtbuff);
	}
	else
	{
		
		sprintf(txtbuff,"��֧��IAP����");
		dbg_sendstr((char*)ptxt);
		
	}
	myfree(txtbuff);
}


void dbg_err (u8 errtype)
{
	char * ptxt=0;
	ptxt="��֧�ֵ��������\"help\"��ȡ������\r\n";
	dbg_sendstr((char*)ptxt);
	
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
	dbg_sendstr((char*)ptxt);
	
	for (i=0;EN_CONFIG[i*2];i++)
	{
		dbg_getdevstate(EN_CONFIG[i*2+1]>>8,txttemp);
		sprintf(txtbuff,"\t�豸��%s\t��ַ��%-8d\t�豸״̬��%s\r\n",dbg_getdevname(EN_CONFIG[i*2+1]&0x00ff),EN_CONFIG[i*2],txttemp);
		dbg_sendstr((char*)txtbuff);
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
	dbg_sendstr((char*)ptxt);
	
	ptxt="\t����\"copy host on\"��ʼ��������λ�������ݽ��������˿�\
			\r\n\t����\"copy host on [�˿ں�]\"��ʼ��������λ�������ݽ�����ָ���Ķ˿�\r\n\t����\"copy host off\"ֹͣ\r\n";
	dbg_sendstr((char*)ptxt);
	
	ptxt="\t����\"devconfig\"��ȡ�豸������Ϣ\r\n";
	dbg_sendstr((char*)ptxt);
	
	ptxt="\t����\"fun [������ַ/������](��������)\"ִ��ָ����ַ�ĺ���\r\n";
	dbg_sendstr((char*)ptxt);
	
	ptxt="\t����\"getip [����]\"��ȡ������Ӧ��IP��ַ\r\n";
	dbg_sendstr((char*)ptxt);
	
	ptxt="\t����\"key [��ֵ] [����]\"ģ�ⰴ���¼�\r\n";
	dbg_sendstr((char*)ptxt);

	ptxt="\t����\"reboot\"�豸����\r\n";
	dbg_sendstr((char*)ptxt);
	
	ptxt="\t����\"mqtt\"���ӵ��ٶ���\r\n";
	dbg_sendstr((char*)ptxt);
	
	ptxt="\t����\"mem [����] [����]\"���û��ȡ�ڴ�ֵ\r\n";
	dbg_sendstr((char*)ptxt);

	ptxt="\t����\"ntp\"��ȡ����ʱ��\r\n";
	dbg_sendstr((char*)ptxt);
	
	ptxt="\t����\"oche on\"��������\r\n\t����\"oche off\"�رջ���\r\n";
	dbg_sendstr((char*)ptxt);
	
	ptxt="\t����\"ping [IP]\"ʹ������ Ping ��[IP]Ϊ��ַ������\r\n";
	dbg_sendstr((char*)ptxt);
	
	ptxt="\t����\"run [�ű�]\"���нű��ļ�\r\n";
	dbg_sendstr((char*)ptxt);

	ptxt="\t����\"sysinfo\"��ȡ������Ϣ\r\n";
	dbg_sendstr((char*)ptxt);

	ptxt="\t����\"swd\"����swd�ӿ�\r\n";
	dbg_sendstr((char*)ptxt);

	ptxt="\t����\"set [������] [����]\"�޸ļ�����������\r\n";
	dbg_sendstr((char*)ptxt);

	ptxt="\t����\"task getidle\"��ѯ�����쳣������\r\n";
	dbg_sendstr((char*)ptxt);

	ptxt="\t����\"task getusege\"��ѯ����ջʹ�����\r\n";
	dbg_sendstr((char*)ptxt);

	ptxt="\t��㲥��ַ����\"whos\"��ѯ��������ļ�����\r\n";
	dbg_sendstr((char*)ptxt);

	ptxt="\t��㲥��ַ����\"find [����\\���]\"����ָ�����ƻ�ָ����ŵļ�����\r\n";
	dbg_sendstr((char*)ptxt);


	
	myfree(txtbuff);
}


void dbg_reboot (void)
{
	char * ptxt=0;
	ptxt="�豸������������\r\n";
	dbg_sendstr((char*)ptxt);

	SysPowerOff();
}


void dbg_booting(void)
{
	char *txtbuff=mymalloc(512);
	sprintf (txtbuff,"���Ϊ %d ���¿ؼ���������������\r\n",Get_MyAddr());
	dbg_sendstr((char*)txtbuff);
	
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
	dbg_sendstr((char*)txtbuff);
	myfree(txtbuff);
}















void dbg_getip(u8 *buff)
{
	u8 getip[4]={0};
	if (DBG_COPY_TO_S1CK) 
	{
		dbg_sendstr("���ȹر�\"copy\"�����\r\n");
		return;
	}
	if (dns_query(2,buff,getip))
	{
		char *txt=mymalloc(200);
		sprintf (txt,"������%s ��IP��ַ�ǣ�%d.%d.%d.%d\r\n",buff,getip[0],getip[1],getip[2],getip[3]);
		dbg_sendstr(txt);
		myfree(txt);
	}
	else
	{
		dbg_sendstr("��ȡIP��ַʧ��T_T\r\n");
	}
	
}



void dbg_mqtt(u8 *buff)
{
	u8 ret=0;
	ret=mqtt_connect("rahher9.mqtt.iot.gz.baidubce.com",1883,"two","rahher9/two","KGa5JL87iGCheRFF");
	char *txt=mymalloc(512);
	sprintf(txt,"������Ϊ��%d\r\n",ret);
	dbg_sendstr(txt);
	myfree(txt);
}





void dbg_task (u8 *buff)
{
	u32 lasttime=0;u32 dietimes=0;
	char *txtbuff=mymalloc(512);
	if ( samestr((u8*)"getidle",buff))
	{
		sprintf(txtbuff,"�����쳣������%08X\r\n",getIdleTask());
		dbg_sendstr(txtbuff);
		for (u8 i=0;i<TASK_MAX_NUM;i++)
		{
			if (GetTaskUsed(i))
			{
				getKilledTask(&lasttime,&dietimes,i);
				sprintf(txtbuff,"���ȼ�Ϊ %2d ������������ %2d �Σ����һ������ʱ���ǣ�%d\r\n",i,dietimes,lasttime);
				dbg_sendstr(txtbuff);
			}
		}
	}
	else if ( samestr((u8*)"getusege",buff))
	{
		sprintf(txtbuff,"����ջʹ�������\r\n");
		dbg_sendstr(txtbuff);
		for (u8 i=0;i<TASK_MAX_NUM;i++)
		{
			if (GetTaskUsed(i))
			{
				sprintf(txtbuff,"���ȼ�Ϊ %2d ջʹ�����Ϊ %2d /%d\r\n",i,GetTaskUsed(i),GetTaskSize(i)  );
				dbg_sendstr(txtbuff);
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
		dbg_sendstr("���ȹر�\"copy\"�����\r\n");
		return;
	}
	pingtime=ping_auto(2,getip);
	if (pingtime!=0xffff)
	{
		char *txt=mymalloc(200);
		sprintf (txt,"IP��ַ %d.%d.%d.%d �������ӳ��� %d ms\r\n",getip[0],getip[1],getip[2],getip[3],pingtime);
		dbg_sendstr(txt);
		myfree(txt);
	}
	else
	{
		dbg_sendstr("Ping ����ʧ��T_T\r\n");
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
			dbg_sendstr(txtbuff);
		}
		else if (samestr((u8*)"nativeip ",chars))
		{
			u8 getip[4]={0};
			getnumfstr(getip,chars+9,'.',4);
			setNativeIP (getip);
			Save_Config();
			sprintf (txtbuff,"�����ñ���IP��ַΪ��%d.%d.%d.%d\r\n",getip[0],getip[1],getip[2],getip[3]);
			dbg_sendstr(txtbuff);
		}
		else if (samestr((u8*)"nativeport ",chars))
		{
			u16 port=0;
			port=str2num(chars+11);
			setNativePort (port);
			Save_Config();
			sprintf (txtbuff,"�����ñ����˿�Ϊ��%d\r\n",port);
			dbg_sendstr(txtbuff);
		}
		else if (samestr((u8*)"serverip ",chars))
		{
			u8 getip[4]={0};
			getnumfstr(getip,chars+9,'.',4);
			setServerIP (getip);
			Save_Config();
			sprintf (txtbuff,"�����÷�����IP��ַΪ��%d.%d.%d.%d\r\n",getip[0],getip[1],getip[2],getip[3]);
			dbg_sendstr(txtbuff);
		}
		else if (samestr((u8*)"serverport ",chars))
		{
			u16 port=0;
			port=str2num(chars+11);
			setServerPort (port);
			Save_Config();
			sprintf (txtbuff,"�����÷������˿�Ϊ��%d\r\n",port);
			dbg_sendstr(txtbuff);
		}
		else if (samestr((u8*)"gatewayip ",chars))
		{
			u8 getip[4]={0};
			getnumfstr(getip,chars+10,'.',4);
			setGatewayIP (getip);
			Save_Config();
			sprintf (txtbuff,"����������IP��ַΪ��%d.%d.%d.%d\r\n",getip[0],getip[1],getip[2],getip[3]);
			dbg_sendstr(txtbuff);
		}
		else if (samestr((u8*)"submask ",chars))
		{
			u8 getip[4]={0};
			getnumfstr(getip,chars+8,'.',4);
			setSubMask (getip);
			Save_Config();
			sprintf (txtbuff,"��������������Ϊ��%d.%d.%d.%d\r\n",getip[0],getip[1],getip[2],getip[3]);
			dbg_sendstr(txtbuff);
		}
		else if (samestr((u8*)"dbgport ",chars))
		{
			u16 port=0;
			port=str2num(chars+8);
			NativeDbgPort=port;
			udp_init(1,NativeDbgPort);
			sprintf (txtbuff,"�����ü������ĵ��Զ˿�Ϊ��%d �����ӵ��¶˿�ͨ��\r\n",port);
			dbg_sendstr(txtbuff);
		}
		else if (samestr((u8*)"nativeid ",chars))
		{
			u16 id=0;
			id=str2num(chars+9);
			Set_MyAddr (id);
			Save_Config();
			sprintf (txtbuff,"�����ü��������豸��ַΪ��%d\r\n",id);
			dbg_sendstr(txtbuff);
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
			dbg_sendstr(txtbuff);
		}
		else if (samestr((u8*)"cleardev",chars))
		{
			clearDev();
			Save_Config();
			sprintf (txtbuff,"���Ƴ��������õ��豸\r\n");
			dbg_sendstr(txtbuff);
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
			dbg_sendstr(txtbuff);
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
			dbg_sendstr(txtbuff);
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
			dbg_sendstr(txtbuff);
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
			dbg_sendstr(txtbuff);
		}
		else
		{
			ptxt="�ݲ�֧�ֵ���������������� \"set\" �鿴֧�ֵĲ���\r\n";
			dbg_sendstr(ptxt);
		}
	}
	else
	{
		ptxt="���������˵����\r\n";
		dbg_sendstr(ptxt);
		
		ptxt="\t����\"set channel [�ŵ�]\"�޸ļ������������ŵ�\r\n";
		dbg_sendstr(ptxt);

		ptxt="\t����\"set nativeip [IP]\"�޸ļ�������IP��ַ\r\n";
		dbg_sendstr(ptxt);

		ptxt="\t����\"set nativeport [�˿�]\"�޸ļ�����������˿�\r\n";
		dbg_sendstr(ptxt);

		ptxt="\t����\"set serverip [IP]\"�޸ķ�������IP��ַ\r\n";
		dbg_sendstr(ptxt);

		ptxt="\t����\"set serverport [�˿�]\"�޸ķ������Ķ˿ں�\r\n";
		dbg_sendstr(ptxt);

		ptxt="\t����\"set gatewayip [IP]\"�޸ļ�����������IP��ַ\r\n";
		dbg_sendstr(ptxt);
		
		ptxt="\t����\"set submask [mask]\"�޸ļ���������������\r\n";
		dbg_sendstr(ptxt);
		
		ptxt="\t����\"set dbgport [�˿�]\"�޸ļ������ĵ��Զ˿�\r\n";
		dbg_sendstr(ptxt);
		
		ptxt="\t����\"set nativeid [��������ַ]\"���ü������ĵ�ַ\r\n";
		dbg_sendstr(ptxt);

		ptxt="\t����\"set adddev [�豸����] [�豸��ַ]\"��Ӽ������豸\r\n";
		dbg_sendstr(ptxt);
		
		ptxt="\t����\"set deldev [�豸��ַ]\"�Ƴ��������豸\r\n";
		dbg_sendstr(ptxt);

		ptxt="\t����\"set cleardev\"�Ƴ������������õ������豸\r\n";
		dbg_sendstr(ptxt);

		ptxt="\t����\"set ctrlfrequency [Ƶ��]\"���ü������Զ����Ƶ�Ƶ�ʣ���λ��\r\n";
		dbg_sendstr(ptxt);
		
		ptxt="\t����\"set ctrlamount [������]\"���ü������Զ����Ƶĳ�����\r\n";
		dbg_sendstr(ptxt);

		ptxt="\t����\"set warntolerance [�ݲ�ֵ]\"���ü������¿ر����ݲ�ֵ\r\n";
		dbg_sendstr(ptxt);

		ptxt="\t����\"set name [����]\"���ü���������\r\n";
		dbg_sendstr(ptxt);
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
		dbg_sendstr(txtbuff);
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
		dbg_sendstr(txtbuff);
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
		dbg_sendstr(txtbuff);
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
		dbg_sendstr(txtbuff);
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
		dbg_sendstr(txtbuff);
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
		dbg_sendstr(txtbuff);
	}
	else
	{
		ptxt="��֧�ֵ��豸���ͣ�����֧�ֵ��豸�����ǣ�\r\n";
		dbg_sendstr(ptxt);
		
		ptxt="\t�ɼ�����\tcjq\r\n";
		dbg_sendstr(ptxt);
		ptxt="\t�յ���������\tkt\r\n";
		dbg_sendstr(ptxt);
		ptxt="\t��ʪ����\tcsj\r\n";
		dbg_sendstr(ptxt);
		ptxt="\t��������\tjhq\r\n";
		dbg_sendstr(ptxt);
		ptxt="\t��ʪ����\tjsj\r\n";
		dbg_sendstr(ptxt);
		ptxt="\tһ�����\tytj\r\n";
		dbg_sendstr(ptxt);
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
		dbg_sendstr(chars);
	}
	else
	{
		ptxt="��ȡʱ��ʧ�ܣ�\r\n";
		dbg_sendstr(ptxt);
	}
	myfree(chars);
}



void dbg_whos(u8 *buff)
{
	char *chars=mymalloc(128);
	sprintf (chars,"��� %d �ļ�����IP��ַ�� %d.%d.%d.%d\r\n",Get_MyAddr(),IP_Addr[0],IP_Addr[1],IP_Addr[2],IP_Addr[3]);
	dbg_sendstr(chars);
	myfree(chars);
}


void dbg_find(u8 *buff)
{
	char *chars=mymalloc(128);
	if (samestr((u8*)getMyName(),buff))
	{
		sprintf (chars,"%s �ı���� %d ��IP��ַ�� %d.%d.%d.%d\r\n",buff,Get_MyAddr(),IP_Addr[0],IP_Addr[1],IP_Addr[2],IP_Addr[3]);
		dbg_sendstr(chars);
	}
	else if (str2num(buff)==Get_MyAddr())
	{
		sprintf (chars,"��� %d �������� %s��IP��ַ�� %d.%d.%d.%d\r\n",Get_MyAddr(),getMyName(),IP_Addr[0],IP_Addr[1],IP_Addr[2],IP_Addr[3]);
		dbg_sendstr(chars);
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
			dbg_sendstr(ptxt);
		}
		else
		{
			ptxt="���ü�ֵʧ��\r\n";
			dbg_sendstr(ptxt);
		}
	}
	else
	{
		ptxt="��ֵȡֵ��Χ�ǣ�1~6\r\n";
		dbg_sendstr(ptxt);

		ptxt="�����ǣ��̰���short \\������long\r\n";
		dbg_sendstr(ptxt);
	}
	myfree(txtbuff);
}




//�ڴ���ص�debug����
void dbg_mem (u8 *buff)
{
	char *txtbuff=mymalloc(512);
	char *ptxt=0;
	if (*buff++==' ')
	{
		u32 addr=0;
		u32 value=0;
		if (samestr("sets ",buff))
		{
			u8 *ints=mymalloc(64*2);
			mymemset(ints,0,64*2);
			buff[15]=0;
			addr=str2hex((char*)buff+5);
			value=str2nums (ints,(char*)&buff[16],','); 
			setMemValue(ints,addr,value);
			sprintf (txtbuff,"����ַ %#x ��ֵ���޸� \r\n",addr);
			dbg_sendstr(txtbuff);
			myfree(ints);
		}
		else if (samestr("gets ",buff))
		{
			u8 *ints=0;
			buff[15]=0;
			addr=str2hex((char*)buff+5);
			value=str2num (&buff[16]); 
			ints=mymalloc(value);
			mymemset(ints,0,value);
			getMemValue(ints,addr,value);
			char *inttxtbuff=mymalloc(value*4);
			char *inttxts=inttxtbuff;
			for (u8 i=0;i<value;i++)
			{
				sprintf (inttxts,"%3d,",ints[i]);
				inttxts+=strlen(inttxts);
			}
			sprintf (txtbuff,"����ַ %#x �������� %s ...\r\n",addr,inttxtbuff);
			dbg_sendstr(txtbuff);
			myfree(inttxtbuff);
		}
		else if (samestr("set ",buff))
		{
			buff[14]=0;
			addr=str2hex((char*)buff+4);
			value=str2hex((char*)&buff[15]);
			setMemU32 (value,addr);
			sprintf (txtbuff,"������ %#x ��ַ��ֵΪ %#x \r\n",addr,value);
			dbg_sendstr(txtbuff);
		}
		else if (samestr("get ",buff))
		{
			addr=str2hex((char*)buff+4);
			value=getMemU32 (addr);
			sprintf (txtbuff,"��ַ %#x ��ֵΪ %#x \r\n",addr,value);
			dbg_sendstr(txtbuff);
		}
	}
	else
	{
		ptxt="����\"mem set [addr] [value]\"����ָ����ַ��u32ֵ\r\n";
		dbg_sendstr(ptxt);

		ptxt="����\"mem get [addr]\"��ȡָ����ַ��32λֵ\r\n";
		dbg_sendstr(ptxt);
		
		ptxt="����\"mem sets [addr] [value1,value2...]\"����ָ����ַ��u8ֵ\r\n";
		dbg_sendstr(ptxt);

		ptxt="����\"mem gets [addr] [length]\"��ȡָ����ַָ�����ȵ�u8ֵ\r\n";
		dbg_sendstr(ptxt);
		
		ptxt="����ϳ����Ӧ��map�ļ����ԣ�\r\n";
		dbg_sendstr(ptxt);
		
		ptxt="�ڴ��޸��������������غ������Ǳ�Ҫ�벻Ҫ������\r\n";
		dbg_sendstr(ptxt);
	}	
	myfree(txtbuff);
}


//����������ص�debug����
void dbg_fun (u8 *buff)
{
	char *txtbuff=mymalloc(512);
	char *ptxt=0;
	if (*buff++==' ')
	{
		u32 value=0;
		
		sprintf (txtbuff,"���� %s ����ִ��...\r\n",buff);
		dbg_sendstr(txtbuff);
		
		value=runFunction((char *)buff); 	

		sprintf (txtbuff,"���� %s ִ�н���������ֵ�� %#x ...\r\n",buff,value);
		dbg_sendstr(txtbuff);
	}
	else
	{
		ptxt="���в����ĺ��������зǺ�����ַ��������غ���������������\r\n";
		dbg_sendstr(ptxt);
		
		ptxt="���ñ���δ�г��ĺ�������ϳ����Ӧ��map�ļ���\r\n";
		dbg_sendstr(ptxt);
				
		ptxt="����֧�ֵĺ������б�\r\n";
		dbg_sendstr(ptxt);
		
		for (u16 i=0;ptxt=getFunNameByIndex(i),ptxt;i++)
		{
			sprintf (txtbuff,"\t %s \r\n",ptxt);
			dbg_sendstr(txtbuff);
			
		}
		
	}
	myfree(txtbuff);
}





//ִ�нű�
void dbg_run (u8 *buff)
{
	char *txtbuff=mymalloc(512);
	char *ptxt=0;
	if (*buff++==' ')
	{
		u32 value=0;
		
		sprintf (txtbuff,"�ű�����ִ�� ...\r\n");
		dbg_sendstr(txtbuff);
		
		value=runCScript((char *)buff); 	

		sprintf (txtbuff,"�ű�ִ�н��� ����ֵ�� %#x ...\r\n",value);
		dbg_sendstr(txtbuff);
	}
	else
	{
		ptxt="�ű�֧�ֵĺ������� fun ����һ�£�֧�ֺ�����ַ����\r\n";
		dbg_sendstr(ptxt);
		
		ptxt="���в����ĺ��������зǺ�����ַ��������غ���������������\r\n";
		dbg_sendstr(ptxt);

		ptxt=cScriptHelpTxt ;
		dbg_sendstr(ptxt);

	}
	myfree(txtbuff);
}


//dbg��ӡ����,ִ�нű�ʱ����
void dbg_print (char *str)
{
	if (dbg_sendstr==0) return ;
	dbg_sendstr(str);	
	dbg_sendstr("\r\n");	
}




//udp�ַ������ͽӿ�
void dbg_send_udp (char *str)
{
	udp_send(1,DBG_IP,DBG_PORT,(u8*)str,strlen(str));	
}








//����ICP����
#define ICP_CMD_ADDR 				(0x20001000)//ICP����
#define ICP_ADDR_ADDR				(0x20001004)//ICP��flash�д洢��λ��
#define ICP_DATASIZE_ADDR 	(0x20001008)//����Ҫ���������ݴ�С

#define ICP_RUN   (0x00000001)	//�����Ѿ�ͨ��swdд���ڴ棬���Կ�ʼ����
#define ICP_RUNED	(0x00000000)	//��һ�ο�����ɣ����Կ�ʼ��һ�ο���
#define ICP_DOWN  (0x00000002)	//����д����ɣ���ʼ����








//����swd��ع���
void dbg_swd (u8 *buff)
{
	char *txtbuff=mymalloc(512);
	char *ptxt=0;
	if (*buff==' ')
	{
		buff++;
		if (samestr("init",buff))
		{
			dbg_swdinit (buff);
		}
		else if (samestr("idcode",buff))
		{
			dbg_swdidcode (buff);
		}
		else if (samestr("loader",buff))
		{
			if (*(buff+6)==' ')
			{
				dbg_swdloader (buff+7);
			}
			else
			{
				dbg_swdloader (0);
			}
		}
		else if (samestr("program",buff))//���
		{
			if (*(buff+7)==' ')
			{
				dbg_swdprogram (buff+8);
			}
			else
			{
				dbg_swdprogram (0);
			}
		}
	}
	else
	{
		ptxt="����\"swd init\"��ʼ���ӿ�\r\n";
		dbg_sendstr(ptxt);
		
		ptxt="����\"swd idcode\"�õ�Ŀ���豸��idcode\r\n";
		dbg_sendstr(ptxt);

		ptxt="����\"swd loader\"����BootLoader��Ŀ���\r\n";
		dbg_sendstr(ptxt);

		ptxt="����\"swd loader [�ļ�·��]\"����ָ��BootLoader��Ŀ���\r\n";
		dbg_sendstr(ptxt);

		ptxt="����\"swd program\"���س���Ŀ���\r\n";
		dbg_sendstr(ptxt);
		
		ptxt="����\"swd program [�ļ�·��]\"����ָ������Ŀ���\r\n";
		dbg_sendstr(ptxt);
	}
	myfree(txtbuff);
}


//swd��ʼ��
void dbg_swdinit (u8 *buff)
{
	char *txtbuff=mymalloc(512);
	char *ptxt=0;
	SWD_Init();
	ptxt="��ʼ��SWD�ӿ������\r\n";
	dbg_sendstr(ptxt);
	myfree(txtbuff);
	
}


//swd����Ŀ���豸
void dbg_swdidcode (u8 *buff)
{
	char *txtbuff=mymalloc(512);
	u32 id=0;
	u8 err;
	SWD_LineReset();
	id=SWD_ReadReg(SWD_REG_DP|SWD_REG_IDCODE,&err);
	sprintf (txtbuff,"�õ����豸id�ǣ�%#X\r\n",id);
	dbg_sendstr(txtbuff);
	myfree(txtbuff);
}



//swd����loader�ļ�,������Ҫ���صĳ�����ָ�룬0,ΪĬ���ļ�
void dbg_swdloader (u8 *buff)
{
	char *txtbuff=mymalloc(512);
	char *file_name=0;
	u32 id=0;
	u8 timeout=0;
	
			FRESULT ret;
			UINT real_length=0;
			u32 data=0;
			u32 icp_cmd=0;
			u8 *sram=mymalloc(4096);
			if (buff)
			{
				file_name=(char*)buff;
			}
			else
			{
				file_name="0:/loader.bin";
			}
			
			sprintf (txtbuff,"���ڼ���Loader��%s\r\n",file_name);
			dbg_sendstr(txtbuff);
			ret=FATS->f_open(file,_T(file_name),FA_OPEN_ALWAYS|FA_READ|FA_WRITE);
			if (ret==FR_OK)
			{
				if (file->fsize<=4096)
				{
					FATS->f_read(file,sram,file->fsize,&real_length);
					
					id=SWD_Cm3Halt(1);
					id=SWD_WriteSram(0x20000000,(u32*)sram,real_length/4);
					data=3;
					id=SWD_WriteSram(ICP_CMD_ADDR,&data,1);//�����������
					id=SWD_WriteCm3Reg(CM3_REG_SP,*(u32*)sram);
					id=SWD_WriteCm3Reg(CM3_REG_PC,((u32*)sram)[1]);
					id=SWD_Cm3Halt(0);
					timeout=0;
					do {
						delay_ms(50);
						id=SWD_ReadSram(ICP_CMD_ADDR,&icp_cmd,1);//�������Ƿ�д�뵽flash
						if (icp_cmd==ICP_RUNED)
						{
							sprintf (txtbuff,"Loader�ļ���д�룺%#X\r\n",id);
							dbg_sendstr(txtbuff);
							break;
						}
						else
						{
							timeout++;
							if (timeout>10)
							{
								sprintf (txtbuff,"Loader�ļ�д�볬ʱ��%#X\r\n",id);
								dbg_sendstr(txtbuff);
								break;
							}
						}
					}while (1);
				}
				else
				{
					sprintf (txtbuff,"Loader�ļ�̫��%u\r\n",(u32)file->fsize);
					dbg_sendstr(txtbuff);
				}
			}
			else
			{
				sprintf (txtbuff,"���ļ�ʧ�ܣ�%#X\r\n",ret);
				dbg_sendstr(txtbuff);
			}
			FATS->f_close(file);
			myfree(sram);
	myfree(txtbuff);
}



//swd��������,������Ҫ���صĳ�����ָ�룬0,ΪĬ���ļ�
void dbg_swdprogram (u8 *buff)
{
	char *txtbuff=mymalloc(512);
	char *file_name=0;
	u32 id=0;
	u8 timeout=0;
			u32 data=0;
			u32 flash_addr=0x08000000;
			u32 all_len=0;
			u32 icp_cmd=0;
			u16 packet_num=0;
			FRESULT ret;
			UINT real_length=0;
			u8 *sram=mymalloc(4096);
			if (buff)
			{
				file_name=(char*)buff;
			}
			else
			{
				file_name="0:/app.bin";
			}
			sprintf (txtbuff,"�������أ�%s\r\n",file_name);
			dbg_sendstr(txtbuff);
			ret=FATS->f_open(file,_T(file_name),FA_OPEN_ALWAYS|FA_READ|FA_WRITE);
			if (ret==FR_OK)
			{
				all_len=file->fsize;
				while(1)
				{
					packet_num++;
					if (all_len>1024)
					{
						FATS->f_read(file,sram,1024,&real_length);	
						//mymemset(sram,packet_num,2048);real_length=2048;
						if (real_length!=1024)
						{
							sprintf (txtbuff,"�ļ���ȡ����%#X\r\n",real_length);
							dbg_sendstr(txtbuff);
							break;
						}
					}
					else if (all_len)
					{
						FATS->f_read(file,sram,all_len,&real_length);	
						//mymemset(sram,packet_num,all_len);real_length=all_len;
						if (real_length!=all_len)
						{
							sprintf (txtbuff,"�ļ���ȡ����%#X\r\n",real_length);
							dbg_sendstr(txtbuff);
							break;
						}
						
					}
					else
					{
						icp_cmd=ICP_DOWN;
						id=SWD_WriteSram(ICP_CMD_ADDR,&icp_cmd,1);//�����������
						sprintf (txtbuff,"������ɣ�%#X\r\n",id);
						dbg_sendstr(txtbuff);
						break;
					}

					data=flash_addr;
					id=SWD_WriteSram(ICP_ADDR_ADDR,&data,1);//�������ص�flash�ĵ�ַ
					
//					id=SWD_ReadSram(ICP_ADDR_ADDR,&data,1);
//					sprintf (txtbuff,"�� %d ������ַ�ǣ�%#X:%#X\r\n",packet_num,flash_addr,data);
//					dbg_sendstr(txtbuff);		

					data=real_length;
					id=SWD_WriteSram(ICP_DATASIZE_ADDR,&data,1);//���ó���Ĵ�С
					
//					id=SWD_ReadSram(ICP_DATASIZE_ADDR,&data,1);
//					sprintf (txtbuff,"�� %d ������С�ǣ�%d\r\n",packet_num,data);
//					dbg_sendstr(txtbuff);		

					id=SWD_WriteSram(0x20003000,(u32*)sram,real_length/4);//���س��򵽻�����
					icp_cmd=ICP_RUN;
					id=SWD_WriteSram(ICP_CMD_ADDR,&icp_cmd,1);//���ô�����ɱ�־λ
					timeout=0;
					do {
						sleep_ms(50);
						id=SWD_ReadSram(ICP_CMD_ADDR,&icp_cmd,1);//�������Ƿ�д�뵽flash
						if (icp_cmd==ICP_RUNED)
						{
							sprintf (txtbuff,"�����˵� %d ������%#X:%d\r\n",packet_num,flash_addr,real_length);
							dbg_sendstr(txtbuff);		
							break;
						}
						else
						{
							timeout++;
							if (timeout>10)
							{
								sprintf (txtbuff,"�������س�ʱ��%#X\r\n",id);
								dbg_sendstr(txtbuff);
								break;
							}
						}
					} while (1);
					if (timeout>10)
					{
						break;
					}
					else
					{
						flash_addr+=real_length;
						all_len-=real_length;
					}
				}
			}
			else
			{
				sprintf (txtbuff,"���ļ�ʧ�ܣ�%#X\r\n",ret);
				dbg_sendstr(txtbuff);
			}
			FATS->f_close(file);
			myfree(sram);
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
		if (DBG_IAP==0)
			TaskPend(7);
		DBG_IAP=1;
	}
}







