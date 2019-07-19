#include "includes.h"
#include "w5500.h"
#include "enternet.h"
#include "baidu_iot.h"
#include "my_topmsg.h"
#include "hard_irq.h"
#include "my_iot.h"

//IOT定时器计时中断
void IOT_Hander(void);

u8 MQTT_STATE=0;

void my_iot (void *t)
{
	u32 msg=0;
//	u16 len=0;
//	u16 temper=0;
	u8 ip[4]={255,255,255,255};
	char *buff=mymalloc(2048);
	char *ptxt=0;
	addSoftTimerIrq10ms(IOT_Hander);
	SOCKET3_SetFocus(OS_GET_PRIOHIGH());
	SOCKET4_SetFocus(OS_GET_PRIOHIGH());
	while(1)
	{
		if (net_get_comstate(3)!=SOCK_UDP)
		{
			udp_init(3,24);
		}
		if (net_get_comstate(4)!=SOCK_UDP)
		{
			udp_init(4,48);
		}
		
		sprintf (buff,"||  温度：%.3f ℃\t||  湿度：%.3f ％\t||  TVOC：%.3f ppm\t||  CO2：%.1f ppm\t||  PM2.5：%.3f mg/m3\t||\r\n",
			GetCJDataAddr()->temp,GetCJDataAddr()->humi,GetCJDataAddr()->tvoc,
			GetCJDataAddr()->co2,GetCJDataAddr()->pm2_5);
		udp_send (3,ip,7020,(u8*)buff,strlen(buff));
		
		sprintf (buff,"Temperature:%.3f '\r\nHumidity:%.3f %%\r\nTVOC:%.3f ppm\r\nCO2:%.1f ppm\r\nPM2.5:%.3f mg/m3\r\ntime:%d\r\n",
			GetCJDataAddr()->temp,GetCJDataAddr()->humi,GetCJDataAddr()->tvoc,
			GetCJDataAddr()->co2,GetCJDataAddr()->pm2_5,getSysRunTime());
		udp_send (4,ip,7030,(u8*)buff,strlen(buff));
		
		ptxt="||----------------------||----------------------||----------------------||----------------------||----------------------||\r\n";
		udp_send (3,ip,7020,(u8*)ptxt,strlen(ptxt));
		msg=sleep_ms(5000);
		if (msg==0)
		{
			msg=0;
		}
		else
		{
			msg=msg;
		}
	}
	
	
}







void IOT_Hander(void)
{
	static u8 i=0;
	static u16 sec=0;
	if (i<100)
	{
		i++;
	}
	else 
	{
		i=0;
		if (sec<60)
		{
			sec++;
		}
		else
		{
			sec=0;
			//这里唤醒任务发送心跳包
			//TaskIntSendMsg(10,2);
		}		
	}
	
}



