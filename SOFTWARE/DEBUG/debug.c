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
#include "debug.h"








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
		if (checkSocketState(1,IR_RECV))//如果有数据等待接收
		{
			u8 *recvbuff=mymalloc(2048);
			memset(recvbuff,0,2048);//清空内存中的数据
			Read_SOCK_Data_Buffer(1, recvbuff);

			//重新设置调试主机目标地址
			mymemcpy(DBG_IP,recvbuff,4);
			DBG_PORT=(recvbuff[4]<<8)|recvbuff[5];

			if (DBG_PORT==7010)
			{
				IWDG_Feed();
				cmd_byudp (recvbuff+8); 
			}
			else 
			{
				dbg_Interpreter(recvbuff);//命令解释器
			}
			myfree(recvbuff);
		}
	}while (DBG_IAP);
}

/*****************************************************

						命令解释器：解释从网口传来的命令并执行
						help：显示帮助信息
						sysinfo：显示系统信息
						setip：设置网络参数


******************************************************/

static u8 DBG_OCHE =0;//回显
u8 DBG_IP[4]={255,255,255,255};//调试用的目标ip地址
u16 DBG_PORT=7000;//调试用的端口号
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
	else if (samestr((u8*)"mem",recvbuff+8))
	{
		dbg_mem(recvbuff+8+3); 
	}
	else if (samestr((u8*)"fun",recvbuff+8))
	{
		dbg_fun(recvbuff+8+3); 
	}
	else if (samestr((u8*)"run",recvbuff+8))
	{
		dbg_run(recvbuff+8+3); 
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
		ptxt="已开启回显\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		
	}
	else if (samestr((u8*)"off",databuff))
	{
		DBG_OCHE=0;
		ptxt="已关闭回显\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	}
}




	//板子信息
void dbg_info (void)
{
	char * ptxt=0;
	char *txtbuff=mymalloc(512);
	ptxt="温控集中器：";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
  sprintf(txtbuff,"%.18s\r\n",getMyName());
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));
	
  sprintf(txtbuff,"软件版本：%.18s\r\n",JZQ_SoftVersion);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

  sprintf(txtbuff,"适配硬件版本：%.18s\r\n",JZQ_Version); 
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf(txtbuff,"本机IP地址：%d.%d.%d.%d\r\n",IP_Addr[0],IP_Addr[1],IP_Addr[2],IP_Addr[3]);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf(txtbuff,"本机MAC地址：%02X.%02X.%02X.%02X.%02X.%02X\r\n",Phy_Addr[0],Phy_Addr[1],Phy_Addr[2],Phy_Addr[3],Phy_Addr[4],Phy_Addr[5]);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf(txtbuff,"服务器IP地址：%d.%d.%d.%d:%d\r\n",SERVER_IP[0],SERVER_IP[1],SERVER_IP[2],SERVER_IP[3],SERVER_PORT);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf(txtbuff,"网关IP地址：%d.%d.%d.%d\r\n",Gateway_IP[0],Gateway_IP[1],Gateway_IP[2],Gateway_IP[3]);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf(txtbuff,"DNS服务器IP地址：%d.%d.%d.%d\r\n",DNS_SERVER[0],DNS_SERVER[1],DNS_SERVER[2],DNS_SERVER[3]);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf(txtbuff,"NTP服务器IP地址：%d.%d.%d.%d\r\n",NTP_SERVER[0],NTP_SERVER[1],NTP_SERVER[2],NTP_SERVER[3]);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	ptxt="网络连接状态：";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	if (DBG_INTER_STATE==0) ptxt="没有网络连接\r\n"; else if (DBG_INTER_STATE==1) ptxt="已连接上网关\r\n";
	else if (DBG_INTER_STATE==2) ptxt="已连接上服务器\r\n"; else ptxt="未知的网络状态\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	sprintf (txtbuff,"无线信道：%d\r\n",Get_MyChanel());
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf (txtbuff,"自动控制频率：%d 秒\r\n",getAutoCtrlFrequency());
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf (txtbuff,"自动控制超调量：%d\r\n",getAutoCtrlAmount());
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf (txtbuff,"温控报警容差值：%d\r\n",getWarnTolerance());
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf(txtbuff,"集中器已运行 %d 秒\r\n",getSysRunTime());
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));
	
	sprintf(txtbuff,"系统时间：-- %d年 - %d月 - %d日 -- %d : %d : %d -- \r\n",
		RTC_GetTimeObj()->w_year,RTC_GetTimeObj()->w_month,RTC_GetTimeObj()->w_date,
		RTC_GetTimeObj()->hour,RTC_GetTimeObj()->min,RTC_GetTimeObj()->sec);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	ptxt="外部FLASH：";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	if (SPI_FLASH_TYPE==0XEF13) ptxt="1MB\r\n"; else if (SPI_FLASH_TYPE==0XEF14) ptxt="2MB\r\n"; 
	else if (SPI_FLASH_TYPE==0XEF15) ptxt="4MB\r\n"; else if (SPI_FLASH_TYPE==0XEF16) ptxt="8MB\r\n";
	else if (SPI_FLASH_TYPE==0XEF17) ptxt="16MB\r\n"; else ptxt="未知的类型\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	ptxt="文件系统状态：";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	if (DBG_FATS==0) ptxt="不支持文件系统\r\n"; else if (DBG_FATS==1) ptxt="没有SD卡\r\n"; 
	else if (DBG_FATS==2) ptxt="SD卡挂载失败\r\n"; else if (DBG_FATS==3) ptxt="支持文件系统\r\n";
	else ptxt="未知状态\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	sprintf (txtbuff,"系统内存使用情况：%dKB总共、%dKB已使用、%dKB剩余、使用了%d%%\r\n",
		memsize/1024,memsize*mem_perused()/100/1024,memsize*(100-mem_perused())/100/1024,mem_perused());
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));
	
	sprintf(txtbuff,"程序位置：-- %#X -- \r\n",SCB->VTOR);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf(txtbuff,"编译时间：%s ---- %s\r\n",__DATE__,__TIME__);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	if (IAP_Support)
	{
		sprintf(txtbuff,"IAP程序版本：-- %s --\r\n",IAP_Version);
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));
			
		sprintf(txtbuff,"IAP程序编译时间：---- %s ----\r\n",IAP_CompileTime);
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));
	}
	else
	{
		
		sprintf(txtbuff,"不支持IAP升级");
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));		
		
	}
	myfree(txtbuff);
}


void dbg_err (u8 errtype)
{
	char * ptxt=0;
	ptxt="不支持的命令，输入\"help\"获取帮助。\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
}



//获取设备状态字符串最高位【0】表示开关状态，【1,2】位表示升降状态，【3】位表示离线状态,
//【4，5,6】代表重试次数，【7】，保留
void dbg_getdevstate(u8 state,char *txtbuff)
{
	if (state&0x08)
	{
		memcpy(txtbuff,"离线、",6);
		txtbuff+=6;
		txtbuff[0]=0;
		return ;//离线状态其他参数无意义
	}
	else
	{
		memcpy(txtbuff,"在线、",6);
		txtbuff+=6;
	}
	if (state&0x01)
	{
		memcpy(txtbuff,"设备开启、",10);
		txtbuff+=10;
	}
	else
	{
		memcpy(txtbuff,"设备关闭、",10);
		txtbuff+=10;
		txtbuff[0]=0;
		return ;
	}
	
	if (state&0x02)
	{
		memcpy(txtbuff,"参数升",6);
		txtbuff+=6;
	}
	else if (state&0x04)
	{
		memcpy(txtbuff,"参数减",6);
		txtbuff+=6;
	}
	else if (state&(0x06))
	{
		memcpy(txtbuff,"非常规状态",10);
		txtbuff+=10;
	}
	else
	{
	}
	txtbuff[0]=0;
}

//获取设备配置状态
void dbg_devconfig(void)
{
	u16 i=0;
	char * ptxt=0;
	char *txtbuff=mymalloc(512);
	char *txttemp=mymalloc(512);
	ptxt="设备配置信息如下：\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	for (i=0;EN_CONFIG[i*2];i++)
	{
		dbg_getdevstate(EN_CONFIG[i*2+1]>>8,txttemp);
		sprintf(txtbuff,"\t设备：%s\t地址：%-8d\t设备状态：%s\r\n",dbg_getdevname(EN_CONFIG[i*2+1]&0x00ff),EN_CONFIG[i*2],txttemp);
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
	}
	myfree(txttemp);
	myfree(txtbuff);
}

//显示帮助信息
void dbg_help(void)
{
	char * ptxt=0;
	char *txtbuff=mymalloc(512);
	ptxt="帮助：\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	ptxt="\t输入\"copy host on\"开始复制与上位机的数据交换到本端口\
			\r\n\t输入\"copy host on [端口号]\"开始复制与上位机的数据交换到指定的端口\r\n\t输入\"copy host off\"停止\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	ptxt="\t输入\"devconfig\"获取设备配置信息\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	ptxt="\t输入\"fun [函数地址/函数名](函数参数)\"执行指定地址的函数\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	ptxt="\t输入\"getip [域名]\"获取域名对应的IP地址\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	ptxt="\t输入\"key [键值] [动作]\"模拟按键事件\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	ptxt="\t输入\"reboot\"设备重启\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	ptxt="\t输入\"mqtt\"连接到百度云\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	ptxt="\t输入\"mem [命令] [参数]\"设置或获取内存值\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	ptxt="\t输入\"ntp\"获取网络时间\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	ptxt="\t输入\"oche on\"开启回显\r\n\t输入\"oche off\"关闭回显\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	ptxt="\t输入\"ping [IP]\"使集中器 Ping 以[IP]为地址的主机\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	ptxt="\t输入\"run [脚本]\"运行脚本文件\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	ptxt="\t输入\"sysinfo\"获取板子信息\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	ptxt="\t输入\"set [设置项] [参数]\"修改集中器的配置\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	ptxt="\t输入\"task getidle\"查询运行异常的任务\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	ptxt="\t输入\"task getusege\"查询任务栈使用情况\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	ptxt="\t向广播地址发送\"whos\"查询接入网络的集中器\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	ptxt="\t向广播地址发送\"find [名称\\编号]\"查找指定名称或指定编号的集中器\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));


	
	myfree(txtbuff);
}


void dbg_reboot (void)
{
	char * ptxt=0;
	ptxt="设备即将重启……\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	SysPowerOff();
}


void dbg_booting(void)
{
	char *txtbuff=mymalloc(512);
	sprintf (txtbuff,"编号为 %d 的温控集中器已启动……\r\n",Get_MyAddr());
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
	
	myfree(txtbuff);
	
}

//复制数据到调试端口
void dbg_copydata (u8 *buff)
{
	u16 port=0;
	char *txtbuff=mymalloc(512);
	if ( samestr((u8*)"host on",buff))
	{
		
		if (DBG_COPY_TO_S1CK)
		{
			sprintf (txtbuff,"复制已在进行中！！\r\n");
		}
		else
		{
			port=str2num(buff+8);
			if (port!=0)
			{
				if (tcp_connect(2,30,DBG_IP,port)==TRUE)
				{
					DBG_COPY_TO_S1CK=2;
					sprintf (txtbuff,"开始复制与主机交换的数据到端口 %d……\r\n",port);
				}
				else
				{
					tcp_close(2);
					sprintf (txtbuff,"请保证输入的端口号已打开为TCP Server!!!\r\n");
				}
			}
			else
			{
				DBG_COPY_TO_S1CK=1;
				sprintf (txtbuff,"开始复制与主机交换的数据到本端口……\r\n");
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
		sprintf (txtbuff,"停止复制与主机交换的数据……\r\n");
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
		udp_send(1,DBG_IP,DBG_PORT,(u8*)"请先关闭\"copy\"命令！！\r\n",22);
		return;
	}
	if (dns_query(2,buff,getip))
	{
		char *txt=mymalloc(200);
		sprintf (txt,"域名：%s 的IP地址是：%d.%d.%d.%d\r\n",buff,getip[0],getip[1],getip[2],getip[3]);
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txt,strlen((const char *)txt));
		myfree(txt);
	}
	else
	{
		udp_send(1,DBG_IP,DBG_PORT,(u8*)"获取IP地址失败T_T\r\n",17);
	}
	
}



void dbg_mqtt(u8 *buff)
{
	u8 ret=0;
	ret=mqtt_connect("rahher9.mqtt.iot.gz.baidubce.com",1883,"two","rahher9/two","KGa5JL87iGCheRFF");
	char *txt=mymalloc(512);
	sprintf(txt,"错误码为：%d\r\n",ret);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txt,strlen(txt));
	myfree(txt);
}





void dbg_task (u8 *buff)
{
	u32 lasttime=0;u32 dietimes=0;
	char *txtbuff=mymalloc(512);
	if ( samestr((u8*)"getidle",buff))
	{
		sprintf(txtbuff,"运行异常的任务：%08X\r\n",getIdleTask());
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));
		for (u8 i=0;i<TASK_MAX_NUM;i++)
		{
			if (GetTaskUsed(i))
			{
				getKilledTask(&lasttime,&dietimes,i);
				sprintf(txtbuff,"优先级为 %2d 的任务死亡了 %2d 次，最后一次死亡时间是：%d\r\n",i,dietimes,lasttime);
				udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));
			}
		}
	}
	else if ( samestr((u8*)"getusege",buff))
	{
		sprintf(txtbuff,"任务栈使用情况：\r\n");
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));
		for (u8 i=0;i<TASK_MAX_NUM;i++)
		{
			if (GetTaskUsed(i))
			{
				sprintf(txtbuff,"优先级为 %2d 栈使用情况为 %2d /%d\r\n",i,GetTaskUsed(i),GetTaskSize(i)  );
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
		udp_send(1,DBG_IP,DBG_PORT,(u8*)"请先关闭\"copy\"命令！！\r\n",22);
		return;
	}
	pingtime=ping_auto(2,getip);
	if (pingtime!=0xffff)
	{
		char *txt=mymalloc(200);
		sprintf (txt,"IP地址 %d.%d.%d.%d 的网络延迟是 %d ms\r\n",getip[0],getip[1],getip[2],getip[3],pingtime);
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txt,strlen((const char *)txt));
		myfree(txt);
	}
	else
	{
		udp_send(1,DBG_IP,DBG_PORT,(u8*)"Ping 操作失败T_T\r\n",18);
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
				sprintf(txtbuff,"设置失败：输入的无线信道为 %d，不在0~31范围内……\r\n",str2num(chars));
			}
			else
			{
				Save_Config();
				sprintf(txtbuff,"已设置无线信道为 %d，重启后生效……\r\n",Get_MyChanel());
			}
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		}
		else if (samestr((u8*)"nativeip ",chars))
		{
			u8 getip[4]={0};
			getnumfstr(getip,chars+9,'.',4);
			setNativeIP (getip);
			Save_Config();
			sprintf (txtbuff,"已设置本机IP地址为：%d.%d.%d.%d\r\n",getip[0],getip[1],getip[2],getip[3]);
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		}
		else if (samestr((u8*)"nativeport ",chars))
		{
			u16 port=0;
			port=str2num(chars+11);
			setNativePort (port);
			Save_Config();
			sprintf (txtbuff,"已设置本机端口为：%d\r\n",port);
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		}
		else if (samestr((u8*)"serverip ",chars))
		{
			u8 getip[4]={0};
			getnumfstr(getip,chars+9,'.',4);
			setServerIP (getip);
			Save_Config();
			sprintf (txtbuff,"已设置服务器IP地址为：%d.%d.%d.%d\r\n",getip[0],getip[1],getip[2],getip[3]);
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		}
		else if (samestr((u8*)"serverport ",chars))
		{
			u16 port=0;
			port=str2num(chars+11);
			setServerPort (port);
			Save_Config();
			sprintf (txtbuff,"已设置服务器端口为：%d\r\n",port);
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		}
		else if (samestr((u8*)"gatewayip ",chars))
		{
			u8 getip[4]={0};
			getnumfstr(getip,chars+10,'.',4);
			setGatewayIP (getip);
			Save_Config();
			sprintf (txtbuff,"已设置网关IP地址为：%d.%d.%d.%d\r\n",getip[0],getip[1],getip[2],getip[3]);
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		}
		else if (samestr((u8*)"dbgport ",chars))
		{
			u16 port=0;
			port=str2num(chars+8);
			NativeDbgPort=port;
			udp_init(1,NativeDbgPort);
			sprintf (txtbuff,"已设置集中器的调试端口为：%d 请连接到新端口通信\r\n",port);
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		}
		else if (samestr((u8*)"nativeid ",chars))
		{
			u16 id=0;
			id=str2num(chars+9);
			Set_MyAddr (id);
			Save_Config();
			sprintf (txtbuff,"已设置集中器的设备地址为：%d\r\n",id);
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
				sprintf (txtbuff,"已移除地址为 %d 的设备\r\n",id);
			}
			else
			{
				sprintf (txtbuff,"移除地址为 %d 的设备失败，可能是不存在这样的设备\r\n",id);
			}
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		}
		else if (samestr((u8*)"cleardev",chars))
		{
			clearDev();
			Save_Config();
			sprintf (txtbuff,"已移除所有配置的设备\r\n");
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		}
		else if (samestr((u8*)"ctrlfrequency ",chars))
		{
			u16 frequency=0;
			frequency=str2num(chars+14);
			if (setAutoCtrlFrequency(frequency)==0)
			{
				Save_Config();
				sprintf (txtbuff,"已设置自动控制的频率为 %d 秒\r\n",frequency);
			}
			else
			{
				sprintf (txtbuff,"自动控制的频率设置失败\r\n");
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
				sprintf (txtbuff,"已设置自动控制的超调量为 %d \r\n",amount);
			}
			else
			{
				sprintf (txtbuff,"自动控制超调量设置失败，可能是超调量值太大\r\n");
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
				sprintf (txtbuff,"已设置温控报警容差值为 %d \r\n",amount);
			}
			else
			{
				sprintf (txtbuff,"温控报警容差值设置失败，可能是值太大\r\n");
			}
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		}
		else if (samestr((u8*)"name ",chars))
		{
			if (setMyName((char *)chars+5)==0)
			{
				Save_Config();
				sprintf (txtbuff,"已设置自动控制的名称为 %s\r\n",chars+5);
			}
			else
			{
				sprintf (txtbuff,"设置集中器的名称失败，可能是名称太长\r\n");
			}
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		}
		else
		{
			ptxt="暂不支持的设置项参数，输入 \"set\" 查看支持的参数\r\n";
			udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		}
	}
	else
	{
		ptxt="设置项参数说明：\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		
		ptxt="\t输入\"set channel [信道]\"修改集中器的无线信道\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

		ptxt="\t输入\"set nativeip [IP]\"修改集中器的IP地址\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

		ptxt="\t输入\"set nativeport [端口]\"修改集中器的网络端口\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

		ptxt="\t输入\"set serverip [IP]\"修改服务器的IP地址\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

		ptxt="\t输入\"set serverport [端口]\"修改服务器的端口号\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

		ptxt="\t输入\"set gatewayip [IP]\"修改集中器的网关IP地址\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		
		ptxt="\t输入\"set dbgport [端口]\"修改集中器的调试端口\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		
		ptxt="\t输入\"set nativeid [集中器地址]\"设置集中器的地址\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

		ptxt="\t输入\"set adddev [设备类型] [设备地址]\"添加集中器设备\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		
		ptxt="\t输入\"set deldev [设备地址]\"移除集中器设备\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

		ptxt="\t输入\"set cleardev\"移除集中器中配置的所有设备\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

		ptxt="\t输入\"set ctrlfrequency [频率]\"设置集中器自动控制的频率，单位秒\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		
		ptxt="\t输入\"set ctrlamount [超调量]\"设置集中器自动控制的超调量\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

		ptxt="\t输入\"set warntolerance [容差值]\"设置集中器温控报警容差值\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

		ptxt="\t输入\"set name [名称]\"设置集中器名称\r\n";
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
			sprintf (txtbuff,"已添加地址为：%d 的采集器\r\n",id);
		}
		else
		{
			sprintf (txtbuff,"添加设备失败，可能是设备地址被占用\r\n");
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
			sprintf (txtbuff,"已添加地址为：%d 的空调控制器\r\n",id);
		}
		else
		{
			sprintf (txtbuff,"添加设备失败，可能是设备地址被占用\r\n");
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
			sprintf (txtbuff,"已添加地址为：%d 的除湿机控制器\r\n",id);
		}
		else
		{
			sprintf (txtbuff,"添加设备失败，可能是设备地址被占用\r\n");
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
			sprintf (txtbuff,"已添加地址为：%d 的净化器\r\n",id);
		}
		else
		{
			sprintf (txtbuff,"添加设备失败，可能是设备地址被占用\r\n");
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
			sprintf (txtbuff,"已添加地址为：%d 的加湿机\r\n",id);
		}
		else
		{
			sprintf (txtbuff,"添加设备失败，可能是设备地址被占用\r\n");
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
			sprintf (txtbuff,"已添加地址为：%d 的一体机控制器\r\n",id);
		}
		else
		{
			sprintf (txtbuff,"添加设备失败，可能是设备地址被占用\r\n");
		}
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
	}
	else
	{
		ptxt="不支持的设备类型，本机支持的设备类型是：\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		
		ptxt="\t采集器：\tcjq\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		ptxt="\t空调控制器：\tkt\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		ptxt="\t除湿机：\tcsj\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		ptxt="\t净化器：\tjhq\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		ptxt="\t加湿机：\tjsj\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		ptxt="\t一体机：\tytj\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	}
	myfree(txtbuff);
}


//通过ntp获取时间
void dbg_ntp (u8 *buff)
{
	u32 time=0;
	char *chars=mymalloc(128);
	char *ptxt=0;
	time=ntp_gettime(2,NTP_SERVER);
	if (time!=0)
	{
		RTC_SetTimeBySec(time+8*3600-0x83AA7E80);//加上中国的时区，减去1900到1970的时间差
		sprintf (chars,"获取到的时间是：%u\r\n",time);
		udp_send(1,DBG_IP,DBG_PORT,(u8*)chars,strlen((const char *)chars));
	}
	else
	{
		ptxt="获取时间失败！\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	}
	myfree(chars);
}



void dbg_whos(u8 *buff)
{
	char *chars=mymalloc(128);
	sprintf (chars,"编号 %d 的集中器IP地址是 %d.%d.%d.%d\r\n",Get_MyAddr(),IP_Addr[0],IP_Addr[1],IP_Addr[2],IP_Addr[3]);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)chars,strlen((const char *)chars));
	myfree(chars);
}


void dbg_find(u8 *buff)
{
	char *chars=mymalloc(128);
	if (samestr((u8*)getMyName(),buff))
	{
		sprintf (chars,"%s 的编号是 %d ，IP地址是 %d.%d.%d.%d\r\n",buff,Get_MyAddr(),IP_Addr[0],IP_Addr[1],IP_Addr[2],IP_Addr[3]);
		udp_send(1,DBG_IP,DBG_PORT,(u8*)chars,strlen((const char *)chars));
	}
	else if (str2num(buff)==Get_MyAddr())
	{
		sprintf (chars,"编号 %d 的名称是 %s，IP地址是 %d.%d.%d.%d\r\n",Get_MyAddr(),getMyName(),IP_Addr[0],IP_Addr[1],IP_Addr[2],IP_Addr[3]);
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
			ptxt="设置键值成功\r\n";
			udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		}
		else
		{
			ptxt="设置键值失败\r\n";
			udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		}
	}
	else
	{
		ptxt="键值取值范围是：1~6\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

		ptxt="动作是：短按：short \\长按：long\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	}
	myfree(txtbuff);
}




//内存相关的debug命令
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
			sprintf (txtbuff,"基地址 %#x 的值已修改 \r\n",addr);
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
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
			sprintf (txtbuff,"基地址 %#x 的数据是 %s ...\r\n",addr,inttxtbuff);
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
			myfree(inttxtbuff);
		}
		else if (samestr("set ",buff))
		{
			buff[14]=0;
			addr=str2hex((char*)buff+4);
			value=str2hex((char*)&buff[15]);
			setMemU32 (value,addr);
			sprintf (txtbuff,"已设置 %#x 地址的值为 %#x \r\n",addr,value);
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		}
		else if (samestr("get ",buff))
		{
			addr=str2hex((char*)buff+4);
			value=getMemU32 (addr);
			sprintf (txtbuff,"地址 %#x 的值为 %#x \r\n",addr,value);
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		}
	}
	else
	{
		ptxt="输入\"mem set [addr] [value]\"设置指定地址的u32值\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

		ptxt="输入\"mem get [addr]\"获取指定地址的32位值\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		
		ptxt="输入\"mem sets [addr] [value1,value2...]\"设置指定地址的u8值\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

		ptxt="输入\"mem gets [addr] [length]\"获取指定地址指定长度的u8值\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		
		ptxt="请配合程序对应的map文件调试！\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		
		ptxt="内存修改命令可能造成严重后果，如非必要请不要操作！\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	}	
	myfree(txtbuff);
}


//函数调用相关的debug命令
void dbg_fun (u8 *buff)
{
	char *txtbuff=mymalloc(512);
	char *ptxt=0;
	if (*buff++==' ')
	{
		u32 value=0;
		
		sprintf (txtbuff,"函数 %s 即将执行...\r\n",buff);
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		
		value=runFunction((char *)buff); 	

		sprintf (txtbuff,"函数 %s 执行结束，返回值是 %#x ...\r\n",buff,value);
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
	}
	else
	{
		ptxt="运行不当的函数或运行非函数地址会造成严重后果，请谨慎操作！\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		
		ptxt="调用本机未列出的函数请配合程序对应的map文件！\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
				
		ptxt="本机支持的函数名列表：\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		
		for (u16 i=0;ptxt=getFunNameByIndex(i),ptxt;i++)
		{
			sprintf (txtbuff,"\t %s \r\n",ptxt);
			udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
			
		}
		
	}
	myfree(txtbuff);
}





//执行脚本
void dbg_run (u8 *buff)
{
	char *txtbuff=mymalloc(512);
	char *ptxt=0;
	if (*buff++==' ')
	{
		u32 value=0;
		
		sprintf (txtbuff,"脚本即将执行 ...\r\n");
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
		
		value=runCScript((char *)buff); 	

		sprintf (txtbuff,"脚本执行结束 返回值是 %#x ...\r\n",value);
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
	}
	else
	{
		ptxt="脚本支持的函数名与 fun 命令一致，支持函数地址调用\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		
		ptxt="运行不当的函数或运行非函数地址会造成严重后果，请谨慎操作！\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

		ptxt="声明字符串变量支持的长度最大为31字节\r\n"
				"声明数组变量支持最大长度为31字节\r\n"
				"函数可以传递临时数组，最多支持一个\r\n"
				"运算式不支持括号重设优先级\r\n"
				"不支持i++，i--运算\r\n"
				"脚本解析器同时只能由一个线程运行，\r\n"
				"关键字：\r\n"
				"	local 定义变量\r\n"
				"	while while循环\r\n"
				"	if else 条件分支\r\n"
				"语法：\r\n"
				"每个语句以分号;结尾\r\n"
				"while 循环 if 分支 都要用大括号{}包括并在右括号后加分号\r\n"
				"有else的陈if语句只在else分支的右括号加分号\r\n"
				"变量声明必须有初始值，根据初始值来确定数据类型\r\n"
				"声明字符串 local a=\"abcd\";\r\n"
				"声明数组 local b=(1,2,3,4,5);\r\n"
				"声明整数 local c=520;\r\n"
				"声明用函数返回值作为初始值的整数 local d=fun();\r\n"
				"while 循环\r\n"
				"while (...)\r\n"
				"{\r\n"
				"	...;\r\n"
				"	...;\r\n"
				"	...;\r\n"
				"};\r\n"
				"if 条件语句\r\n"
				"if (...)\r\n"
				"{\r\n"
				"	...;\r\n"
				"	...;\r\n"
				"	...;\r\n"
				"}\r\n"
				"else\r\n"
				"{\r\n"
				"	...;\r\n"
				"	...;\r\n"
				"	...;\r\n"
				"};\r\n"
				"或\r\n"
				"if (...)\r\n"
				"{\r\n"
				"	...;\r\n"
				"	...;\r\n"
				"	...;\r\n"
				"};\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	}
	myfree(txtbuff);
}


//dbg打印函数
void dbg_print (char *str)
{
	udp_send(1,DBG_IP,DBG_PORT,(u8*)str,strlen(str));	
	udp_send(1,DBG_IP,DBG_PORT,(u8*)"\r\n",2);	
}




//专门给cmd命令交互封装的函数
void cmd_send (u8 *data,u16 datalen )
{
	udp_send(1,DBG_IP,DBG_PORT,data,datalen);
}


//通过网络传输程序升级等数据
void	cmd_byudp (u8 *cmd)
{
	if ((cmd[1]==0xff)&&(cmd[0]==0xff))
	{
		Get_cmd (cmd_send,cmd);
		DBG_IAP=1;
	}
}







