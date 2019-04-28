
#include "includes.h"
#include "my_messeg.h"
#include "my_autocontrol.h"
//自动控制实现文件

/*************************************************************


2018.8.2 加入自动控制，
2018.8.3 加入环境值异常灯光效果
					正常时灯带显示绿色，异常时灯带显示红色，同时异常的环境值对应的按键周围的灯圈会警示
					处于手动时常亮，处于自动时闪烁，直至恢复正常值后熄灭
2019.4.3 自动控制行为说明
					自动控制任务包含的行为是：控制设备，按键灯显红报警，灯带显红报警，上报服务器报警，
					-------------------------------------------------------------------|
					因素和行为|环境上下限|报警容差|超调量|按键灯|灯带|上报服务器|设备  |
					----------+----------+--------+------+------+----+----------+------|
					一切正常	|√				 |√			|√		 |关		|绿色|不上报		|关    |
					----------+----------+--------+------+------+----+----------+------|
					超调不足	|√				 |√			|×		 |关		|绿色|不上报		|不更改|
					----------+----------+--------+------+------+----+----------+------|
					限度超标	|×				 |√			|〇		 |不更改|绿色|不上报		|开    |
					----------+----------+--------+------+------+----+----------+------|
					容差超标	|×				 |×			|〇		 |红色	|红色|上报	  	|开    |
					-------------------------------------------------------------------|

************************************************************/

#define WARN_TOLERANCE getWarnTolerance()
//超调值
#define OVERSHOOT_RANGE getAutoCtrlAmount()

#ifdef JZQ_V2_6
	//1使用旧版，0使用新版，旧版串口屏设置是以1mg/m3为单位，新版以0.1mg/m3为单位
	#define __USE_OLD   0
#else
	#define __USE_OLD   1
#endif



u8 ctrl_temp (float now_temp);
u8 ctrl_humi (float now_humi);
u8 ctrl_air (float now_tvoc);



//自动控制循环
void my_autocontrol (void * t)
{
	u8 m_send[MESSEG_DATA]={0};
	cjq_data *cj_data;
	float now_temp;//环境值变量改为浮点型，比较时比较小数，2018.11.19
	float now_humi;
	float now_tvoc;
	u8 en_warning=0;
	cj_data=GetCJDataAddr ( );
	delay_ms(3000);//系统运行3秒钟之后开始执行
	while (1)
	{
		delay_ms(getAutoCtrlFrequency()*1000+1);//每30秒执行一次自动判断
		if (cj_data->cjqId==0) continue;
		now_temp=cj_data->temp;//传入小数，2018.11.19
		now_humi=cj_data->humi;
		now_tvoc=cj_data->tvoc;

		#if __USE_OLD==0
				now_tvoc=now_tvoc*10;//TVOC上限是乘以10之后的数据，在这里把tvoc数据*10后比较,2018.11.19
		#endif

		
		en_warning|=ctrl_temp(now_temp);
		en_warning|=ctrl_humi(now_humi);
		en_warning|=ctrl_air(now_tvoc);

		if (en_warning)		//灯带异常指示
		{
			m_send[0]=3;
			m_send[1]=5;
			m_send[3]=100;m_send[4]=0;m_send[5]=0;
			send_messeg (LIT_MESSEG,m_send);
			en_warning=0;
		}
		else
		{
			m_send[0]=3;
			m_send[1]=5;
			m_send[3]=0;m_send[4]=100;m_send[5]=0;
			send_messeg (LIT_MESSEG,m_send);
		}	

	}
}









//报警设备地址，设备类型，报警类型，报警时的值
void send_warn(u16 addr,u8 devtype,u8 warntype,u8 ddd)
{
	u8 send[MESSEG_DATA]={0};
	send[0]=7;
	send[1]=warntype;
	send[2]=devtype;
	send[3]=addr>>8;
	send[4]=addr;
	send[5]=ddd;
		if (find_messeg(WK_MESSEG)==0)
	send_messeg(WK_MESSEG,send);
}










//控制温度,返回1，有报警，0，没有
u8 ctrl_temp (float now_temp)
{
	u8 m_send[MESSEG_DATA]={0};
	u8 warning=0;						
	if (now_temp>=Lcd_GetLimitData (0)) 
	{
		m_send[0]=2;
		m_send[1]=2;m_send[2]=0;m_send[3]=0;
		m_send[4]=1;
		m_send[5]=2;
		if (Lcd_GetHandstate()==0)//自动模式
		{
			send_messeg (RF_MESSEG,m_send);
		}
		
		
		if (now_temp>=Lcd_GetLimitData (0)+WARN_TOLERANCE)	//超过上限加容差，需报警
		{
			m_send[2]=1;//闪烁的位置
			m_send[0]=1;m_send[1]=1;
			send_messeg (LIT_MESSEG,m_send);
			warning=1;
			send_warn(Get_MyAddr(),0,1,(u8) now_temp);//发送报警到网口
		}
	}
	else if (now_temp<=Lcd_GetLimitData (1))
	{
		m_send[0]=2;
		m_send[1]=2;m_send[2]=0;m_send[3]=0;
		m_send[4]=1;
		m_send[5]=1;
		if (Lcd_GetHandstate()==0)//自动模式
		{
			send_messeg (RF_MESSEG,m_send);
		}
			
		if (now_temp+WARN_TOLERANCE<=Lcd_GetLimitData (1))//超过下限-容差，需报警
		{
			m_send[2]=2;//闪烁的位置
			m_send[0]=1;m_send[1]=1;
			send_messeg (LIT_MESSEG,m_send);
			warning=2;
			send_warn(Get_MyAddr(),0,2,(u8) now_temp);//发送报警到网口
		}
	}
	else if ((now_temp+OVERSHOOT_RANGE<Lcd_GetLimitData (0))&&(now_temp>Lcd_GetLimitData (1)+OVERSHOOT_RANGE))
	{
		m_send[0]=2;
		m_send[1]=2;m_send[2]=0;m_send[3]=0;
		m_send[4]=0;
		m_send[5]=0;
		if (Lcd_GetHandstate()==0)//自动模式
		{
			send_messeg (RF_MESSEG,m_send);
		}
		m_send[2]=1;//闪烁的位置
		m_send[0]=1;m_send[1]=0;
		send_messeg (LIT_MESSEG,m_send);
		m_send[2]=2;//闪烁的位置
		m_send[0]=1;m_send[1]=0;
		send_messeg (LIT_MESSEG,m_send);
	}
	else					//在超调区间，改报警为不报警，但是设备继续运行
	{
		m_send[2]=1;//闪烁的位置
		m_send[0]=1;m_send[1]=0;
		send_messeg (LIT_MESSEG,m_send);
		m_send[2]=2;//闪烁的位置
		m_send[0]=1;m_send[1]=0;
		send_messeg (LIT_MESSEG,m_send);
	}
	return warning;
}










//控制湿度，1，有报警，0，没有报警
u8 ctrl_humi (float now_humi)
{
	u8 m_send[MESSEG_DATA]={0};
	u8 warning=0;						
							/*除湿加湿的自动控制*/
	if (now_humi>=Lcd_GetLimitData (2))
	{
		m_send[0]=3;
		m_send[1]=2;m_send[2]=0;m_send[3]=0;
		m_send[4]=1;
		m_send[5]=2;
		if (Lcd_GetHandstate()==0)//自动模式
		{
			send_messeg (RF_MESSEG,m_send);
		}
		if (now_humi>=Lcd_GetLimitData (2)+WARN_TOLERANCE)	//超过上限加容差，需报警
		{
			m_send[2]=3;//闪烁的位置
			m_send[0]=1;m_send[1]=1;
			send_messeg (LIT_MESSEG,m_send);			
			warning=3;
			send_warn(Get_MyAddr(),0,3,(u8)now_humi);//发送报警到网口
		}
	}
	else if (now_humi<=Lcd_GetLimitData (3))
	{
		m_send[0]=5;
		m_send[1]=2;m_send[2]=0;m_send[3]=0;
		m_send[4]=1;
		m_send[5]=1;
		if (Lcd_GetHandstate()==0)//自动模式
		{
			send_messeg (RF_MESSEG,m_send);
		}
		if (now_humi+WARN_TOLERANCE<=Lcd_GetLimitData (3))//超过下限-容差，需报警
		{
			m_send[2]=4;//闪烁的位置
			m_send[0]=1;m_send[1]=1;
			send_messeg (LIT_MESSEG,m_send);
			warning=4;
			send_warn(Get_MyAddr(),0,4,(u8)now_humi);//发送报警到网口
		}
	}
	else if ((now_humi+OVERSHOOT_RANGE<Lcd_GetLimitData (2))&&(now_humi>Lcd_GetLimitData (3)+OVERSHOOT_RANGE))
	{
		m_send[0]=3;
		m_send[1]=2;m_send[2]=0;m_send[3]=0;
		m_send[4]=0;
		m_send[5]=0;
		if (Lcd_GetHandstate()==0)//自动模式
		{
			send_messeg (RF_MESSEG,m_send);
			m_send[0]=5;
			send_messeg (RF_MESSEG,m_send);
		}
		m_send[2]=3;//闪烁的位置
		m_send[0]=1;m_send[1]=0;
		send_messeg (LIT_MESSEG,m_send);
		m_send[2]=4;//闪烁的位置
		m_send[0]=1;m_send[1]=0;
		send_messeg (LIT_MESSEG,m_send);				
	}
	else
	{
		m_send[2]=3;//闪烁的位置
		m_send[0]=1;m_send[1]=0;
		send_messeg (LIT_MESSEG,m_send);
		m_send[2]=4;//闪烁的位置
		m_send[0]=1;m_send[1]=0;
		send_messeg (LIT_MESSEG,m_send);
	}
	return warning;
}



//空气质量控制
u8 ctrl_air (float now_tvoc)
{
	u8 m_send[MESSEG_DATA]={0};
	u8 warning=0;		
	
							/*空气净化机的自动控制*/
	if (now_tvoc>=Lcd_GetLimitData (4))
	{
		m_send[0]=4;
		m_send[1]=2;m_send[2]=0;m_send[3]=0;
		m_send[4]=1;
		m_send[5]=2;
		if (Lcd_GetHandstate()==0)//自动模式
		{
			send_messeg (RF_MESSEG,m_send);
		}
		if (now_tvoc>=Lcd_GetLimitData (4)+WARN_TOLERANCE)	//超过上限加容差，需报警
		{
			m_send[2]=6;//闪烁的位置
			m_send[0]=1;m_send[1]=1;
			send_messeg (LIT_MESSEG,m_send);
			warning=6;
			send_warn(Get_MyAddr(),0,5,(u8)now_tvoc);//发送报警到网口
		}
	}
	else
	{
		m_send[0]=4;
		m_send[1]=2;m_send[2]=0;m_send[3]=0;
		m_send[4]=0;
		m_send[5]=0;
		if (Lcd_GetHandstate()==0)//自动模式
		{
			send_messeg (RF_MESSEG,m_send);
		}
		m_send[2]=6;//闪烁的位置
		m_send[0]=1;m_send[1]=0;
		send_messeg (LIT_MESSEG,m_send);
	}
	return warning;
}



