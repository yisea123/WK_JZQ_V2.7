#include "includes.h"
#include "rf.h"
#include "hard_irq.h"
#include "my_messeg.h"
#include "my_cfg.h"
#include "my_rf.h"






/***********************************************************

		meg每个位的含义
		meg[0],设备类型,0切换为自动控制，0xff,控制指定地址的设备
		meg[1],1获取设备状态，2，手动控制设备,
		meg[2],设备地址高把位
		meg[3],设备地址第八位
		meg[4],如果是控制，开关状态，0，关，1，开，2，反向
		meg[5],如果是控制，升降状态，反向时的按键，1，升，2，降

		详细的消息定义参见my_messeg.h文件

***********************************************************/


//1使用旧版，0使用新版，旧版串口屏设置是以1mg/m3为单位，新版以0.1mg/m3为单位
#define __USE_OLD   0

u8 HANDING=0;//手动操作是否在进行


void rf_loop (u16 i);
void rf_send (u16 i);
void rf_hand (u8 *msg);
//发送设备故障报警，地址，设备类型，故障值
void rf_deverr (u16 addr,u8 devtype,u8 d);



//处理消息循环
void my_rf_loop (void * t)
{
	u16 i;//循环变量
	
	RF_M0=0;
	RF_M1=0;
	RF_Init(115200);
	
	USART1_SetFocus(OSPrioHighRdy);
	
	
	RF_SetChannel(Get_MyChanel()); 
	Updata_DeviceNum();
	while (1)
	{
		delay_ms(30);//防止没有设备的时候死机
		for (i=1;EN_CONFIG[i*2+1];i++)
		{
			if (delay_ms(1000)==DELAY_END)//没有收到其他消息
			{
				if (cfg_getstate()==0)
				{
					rf_loop(i);		//消息循环
				}
				rf_send(i);		//发送状态消息
			}
			
			u8 msg[MESSEG_DATA]={0};
			u8 rf_recv[50]={0};
			if (get_messeg(RF_MESSEG,msg)==0)
			{
				if (cfg_getstate()==0)
				{
					rf_hand(msg);
				}
				cfg_msg(msg);
			}
			Get_Mycmd(rf_recv);
			rf_cfg(rf_recv);
		}
	}
}
 


void rf_cjq_to_data (u8 *rf_data);

void rf_loop (u16 i)
{
	u8 rf_recv[55]={0};
	u16 ret;//返回值
	ret=Cmd_0x01 (EN_CONFIG[i*2] ,rf_recv);
	if (ret==0)//成功
	{
		if ((EN_CONFIG[i*2+1]&0x00ff)==1)
		{
			EN_CONFIG[i*2+1]|=DEVICEON;

			
			if (rf_recv[7+9])			//传感器故障
			{
				rf_deverr ((rf_recv[2]<<8)|rf_recv[3],devTypeCjq,rf_recv[7+9]);
				delay_ms(1000);
			}
			rf_cjq_to_data(rf_recv);
			rf_cjq_deal(rf_recv);
		}
		else 
		{
				//设备开关状态
			if (rf_recv[7+9]==1) 
			{
				EN_CONFIG[i*2+1]|=DEVICEON;
			}
			else 
			{
				EN_CONFIG[i*2+1]&=~DEVICEON;
			}

			if (GetDeviceType(EN_CONFIG[i*2])!=6)//不是恒湿机
			{
						//设备升降状态
				if (rf_recv[8+9]==1) 
				{
					EN_CONFIG[i*2+1]|=DEVICEUP;
					EN_CONFIG[i*2+1]&=~DEVICEDOWN;
				}
				else if (rf_recv[8+9]==2)
				{
					EN_CONFIG[i*2+1]|=DEVICEDOWN;
					EN_CONFIG[i*2+1]&=~DEVICEUP;
				}
			}else
			{
			}
			if(rf_recv[9+11]) //发送漏水报警
			loushui_warn(EN_CONFIG[i*2],GetDeviceType(EN_CONFIG[i*2]));
		}
		EN_CONFIG[i*2+1]&=~0xf800;//在线
	}
	else if (ret==ERR_TIMEOUT)
	{
		EN_CONFIG[i*2+1]+=0x1000;//离线计数器
		if ((EN_CONFIG[i*2+1]>>12)>5)//连续5次超时，确认离线
		{
		EN_CONFIG[i*2+1]&=0x00ff;//离线了
		EN_CONFIG[i*2+1]|=DEVICEOFFLINE;//离线了
		}
	}
	else
	{
		EN_CONFIG[i*2+1]&=0x00ff;//离线了
		EN_CONFIG[i*2+1]|=DEVICEOFFLINE;//离线了
	}


}



//发送设备故障报警，地址，设备类型，故障值
void rf_deverr (u16 addr,u8 devtype,u8 d)
{
	u8 send[MESSEG_DATA]={0};
	send[0]=7;
	send[1]=7;//错误类型，设备故障
	send[2]=devtype;
	send[3]=addr>>8;
	send[4]=addr;
	send[5]=d;
		if (find_messeg(WK_MESSEG)==0)
	send_messeg(WK_MESSEG,send);

}



//把无线的数据填充到采集器结构体中
void rf_cjq_to_data (u8 *rf_data)
{
	cjq_data *cjqdata=GetCJDataAddr();
	f_to_u t={0};
	
	//采集器地址
	cjqdata->cjqId=(rf_data[2]<<8)|rf_data[3];
	
	//温度
	t.u[0]=rf_data[7+10];
	t.u[1]=rf_data[7+11];
	t.u[2]=rf_data[7+12];
	t.u[3]=rf_data[7+13];
	cjqdata->temp=t.f;
	
	//湿度
	t.u[0]=rf_data[7+14];
	t.u[1]=rf_data[7+15];
	t.u[2]=rf_data[7+16];
	t.u[3]=rf_data[7+17];
	cjqdata->humi=t.f;

	//TVOC
	t.u[0]=rf_data[7+18];
	t.u[1]=rf_data[7+19];
	t.u[2]=rf_data[7+20];
	t.u[3]=rf_data[7+21];
	cjqdata->tvoc=t.f;
	
	t.u[0]=rf_data[7+22];
	t.u[1]=rf_data[7+23];
	t.u[2]=rf_data[7+24];
	t.u[3]=rf_data[7+25];
	cjqdata->co2=t.f;
	
	t.u[0]=rf_data[7+26];
	t.u[1]=rf_data[7+27];
	t.u[2]=rf_data[7+28];
	t.u[3]=rf_data[7+29];
	cjqdata->pm2_5=t.f;

}





void loushui_warn(u16 addr,u8 devtype)
{
	u8 send[MESSEG_DATA]={0};
	send[0]=7;
	send[1]=6;
	send[2]=devtype;
	send[3]=addr>>8;
	send[4]=addr;
	send[5]=1;
	if (find_messeg(WK_MESSEG)==0)
		send_messeg(WK_MESSEG,send);//发送给网口
	
}


			//发送采集器数据
void rf_cjq_deal(u8 *data) 
{
	u16 temp=0;
	cjq_data *t=GetCJDataAddr();
	u8 send[MESSEG_DATA]={0};
	
	
		//temp=data[17]*10+data[18];//原数据*10,温度
		temp=t->temp*10;
		send[0]=4;send[3]=temp>>8;send[4]=temp;
	
#if __USE_OLD == 0
		//temp=data[19]*10+data[20];//原数据，湿度
		temp=t->humi*10;
#else
		//temp=data[19];//原数据，湿度
		temp=t->humi;
#endif
		send[5]=temp>>8;send[6]=temp;
	
		//temp=data[21]*10+data[22];
		temp=t->tvoc*10;
		send[7]=temp>>8;send[8]=temp;
		send_messeg(LCD_MESSEG,send);//发送给屏幕显示
		
		send[0]=2;send[1]=1;send[2]=0;//发送采集器的数据
		send[3]=((u32 )EN_DATA>>24);	//设置地址
		send[4]=((u32 )EN_DATA>>16);
		send[5]=((u32 )EN_DATA>>8);
		send[6]=((u32 )EN_DATA);
		if (find_messeg(WK_MESSEG)==0)
			send_messeg(WK_MESSEG,send);//发送给网口
		data[0]=0;

}









void rf_hand (u8 *msg)
{
	u8 m_send[MESSEG_DATA]={0};//用来给网口返回控制信息
	u16 my_i=0;
	u16 m=0;//操作失败次数；
	u16 ret;
	u16 id=0;
	u16 newret=0;
	u8 newpower;
	u8 newstate_;//newstate这个变量名可能是系统关键字，会自动改变
	if ((msg[0]!=0)&&(msg[1]==2)) 
	{
		m=0;
		id=(msg[2]<<8)|msg[3];
		if (id==0)				//地址为0，控制全部同类设备
		{
			my_i=0;
			while(my_i<GetDeviceNum(msg[0]))
			{
				if (GetDeviceState(GetNextDeviceaddr(msg[0],my_i))&DEVICEOFFLINE)
				{
					my_i++;//设备不在线
					continue;
				}
				delay_ms(200);			
										//获取新的控制设备的状态,高8位开关，低8位状态
				newret=get_newstate(msg[4],msg[5],GetNextDeviceaddr(msg[0],my_i));newstate_=newret;newpower=(newret>>8);
														//设备地址，设备类型，开关状态，附加参数
				ret=Cmd_0x03 (GetNextDeviceaddr(msg[0],my_i),msg[0],newpower,newstate_);
				if (ret==0)
				{
					Updata_devicestate(GetNextDeviceaddr(msg[0],my_i) ,newpower,newstate_);
					Device_state_see(msg[0],newpower,newstate_);
					my_i++;//控制成功，控制下一个
				}
				else
				{
					m++;if (m>10){m=0;my_i++;}//重试10次
				}
			}
			//加一个控制恒湿机的循环
			if (msg[0]==3||msg[0]==5)
			{
				my_i=0;
				msg[0]=6;
				while(my_i<GetDeviceNum(6))//一体机的个数
				{
					if (GetDeviceState(GetNextDeviceaddr(msg[0],my_i))&DEVICEOFFLINE)
					{
						my_i++;//设备不在线
						continue;
					}
					delay_ms(200);	
					newret=yt_newstate(msg[4],msg[5],GetNextDeviceaddr(msg[0],my_i));newpower=(newret>>8);
					newstate_=newret;
															//设备地址，设备类型，开关状态，附加参数
					ret=Cmd_0x03 (GetNextDeviceaddr(msg[0],my_i),msg[0],newpower,newstate_);
					if (ret==0)
					{
						if (newstate_==99)
						{
							Updata_devicestate(GetNextDeviceaddr(msg[0],my_i) ,newpower,1);
							Device_state_see(5,newpower,1);
						}
						else if (newstate_==1)
						{
							Updata_devicestate(GetNextDeviceaddr(msg[0],my_i) ,newpower,2);
							Device_state_see(3,newpower,2);
						}
						else
						{
							Updata_devicestate(GetNextDeviceaddr(msg[0],my_i) ,newpower,0);
							Device_state_see(3,newpower,newstate_);
							Device_state_see(5,newpower,newstate_);
						}
						my_i++;//控制成功，控制下一个
					}
					else
					{
						m++;if (m>10){m=0;my_i++;}//重试10次
					}
				}
			}
		}
		else //控制指定地址的设备
		{
			while (1)
			{
				delay_ms(200);			
				if (CheckId (id)==0)
				{
					//给网口返回执行结果，不存在指定的ip地址
					m_send[0]=5;m_send[1]=ERR_NONEADDR>>8;m_send[2]=ERR_NONEADDR;
					send_messeg(WK_MESSEG,m_send);
					break;
				}
												//设备地址，设备类型，开关状态，附加参数
				ret=Cmd_0x03 (id,GetDeviceType(id),msg[4],msg[5]);
				if (ret==0)
				{
					Updata_devicestate(id ,msg[4],msg[5]);
					Device_state_see(GetDeviceType(id),msg[4],msg[5]);
					//给网口返回执行结果，成功
					m_send[0]=5;m_send[1]=ret>>8;m_send[2]=ret;
					send_messeg(WK_MESSEG,m_send);
					
					break;//控制成功，退出
				}
				else if (GetDeviceState(id)&DEVICEOFFLINE)
				{
					//给网口返回执行结果，设备不在线
					m_send[0]=5;m_send[1]=ERR_OFFLINE>>8;m_send[2]=ERR_OFFLINE;
					send_messeg(WK_MESSEG,m_send);
					break;//设备不在线
				}
				else
				{
					m++;if (m>10){
						//给网口返回执行结果，执行结果
					m_send[0]=5;m_send[1]=ret>>8;m_send[2]=ret;
					send_messeg(WK_MESSEG,m_send);
						
					break;}//重试10次
				}
			}
		}
	}
}






					//处理手动消息
void my_rf_hand (void * t)
{
	u8 msg[MESSEG_DATA]={0};
	u8 m_send[MESSEG_DATA]={0};//用来给网口返回控制信息
	u16 my_i=0;
	u16 m=0;//操作失败次数；
	u16 ret;
	u16 id=0;
	u16 newret=0;
	u8 newpower;
	u8 newstate_;//newstate这个变量名可能是系统关键字，会自动改变
	while(1)
	{
		delay_ms(200);
		if (get_messeg(RF_MESSEG,msg))//没有消息
		{
		}
		else
		{
			USART1_SetFocus(OSPrioHighRdy);
			TaskPend(3);
			if ((msg[0]!=0)&&(msg[1]==2)) 
			{
				m=0;
				id=(msg[2]<<8)|msg[3];
				if (id==0)				//地址为0，控制全部同类设备
				{
					my_i=0;
					while(my_i<GetDeviceNum(msg[0]))
					{
						if (GetDeviceState(GetNextDeviceaddr(msg[0],my_i))&DEVICEOFFLINE)
						{
							my_i++;//设备不在线
							continue;
						}
						delay_ms(200);			
												//获取新的控制设备的状态,高8位开关，低8位状态
						newret=get_newstate(msg[4],msg[5],GetNextDeviceaddr(msg[0],my_i));newstate_=newret;newpower=(newret>>8);
																//设备地址，设备类型，开关状态，附加参数
						ret=Cmd_0x03 (GetNextDeviceaddr(msg[0],my_i),msg[0],newpower,newstate_);
						if (ret==0)
						{
							Updata_devicestate(GetNextDeviceaddr(msg[0],my_i) ,newpower,newstate_);
							Device_state_see(msg[0],newpower,newstate_);
							my_i++;//控制成功，控制下一个
						}
						else
						{
							m++;if (m>10){m=0;my_i++;}//重试10次
						}
					}
					//加一个控制恒湿机的循环
					if (msg[0]==3||msg[0]==5)
					{
						my_i=0;
						msg[0]=6;
						while(my_i<GetDeviceNum(6))//一体机的个数
						{
							if (GetDeviceState(GetNextDeviceaddr(msg[0],my_i))&DEVICEOFFLINE)
							{
								my_i++;//设备不在线
								continue;
							}
							delay_ms(200);	
							newret=yt_newstate(msg[4],msg[5],GetNextDeviceaddr(msg[0],my_i));newpower=(newret>>8);
							newstate_=newret;
																	//设备地址，设备类型，开关状态，附加参数
							ret=Cmd_0x03 (GetNextDeviceaddr(msg[0],my_i),msg[0],newpower,newstate_);
							if (ret==0)
							{
								if (newstate_==99)
								{
									Updata_devicestate(GetNextDeviceaddr(msg[0],my_i) ,newpower,1);
									Device_state_see(5,newpower,1);
								}
								else if (newstate_==1)
								{
									Updata_devicestate(GetNextDeviceaddr(msg[0],my_i) ,newpower,2);
									Device_state_see(3,newpower,2);
								}
								else
								{
									Updata_devicestate(GetNextDeviceaddr(msg[0],my_i) ,newpower,0);
									Device_state_see(3,newpower,newstate_);
									Device_state_see(5,newpower,newstate_);
								}
								my_i++;//控制成功，控制下一个
							}
							else
							{
								m++;if (m>10){m=0;my_i++;}//重试10次
							}
						}
					}
				}
				else //控制指定地址的设备
				{
					while (1)
					{
						delay_ms(200);			
						if (CheckId (id)==0)
						{
							//给网口返回执行结果，不存在指定的ip地址
							m_send[0]=5;m_send[1]=ERR_NONEADDR>>8;m_send[2]=ERR_NONEADDR;
							send_messeg(WK_MESSEG,m_send);
							break;
						}
														//设备地址，设备类型，开关状态，附加参数
						ret=Cmd_0x03 (id,GetDeviceType(id),msg[4],msg[5]);
						if (ret==0)
						{
							Updata_devicestate(id ,msg[4],msg[5]);
							Device_state_see(GetDeviceType(id),msg[4],msg[5]);
							//给网口返回执行结果，成功
							m_send[0]=5;m_send[1]=ret>>8;m_send[2]=ret;
							send_messeg(WK_MESSEG,m_send);
							
							break;//控制成功，退出
						}
						else if (GetDeviceState(id)&DEVICEOFFLINE)
						{
							//给网口返回执行结果，设备不在线
							m_send[0]=5;m_send[1]=ERR_OFFLINE>>8;m_send[2]=ERR_OFFLINE;
							send_messeg(WK_MESSEG,m_send);
							break;//设备不在线
						}
						else
						{
							m++;if (m>10){
								//给网口返回执行结果，执行结果
							m_send[0]=5;m_send[1]=ret>>8;m_send[2]=ret;
							send_messeg(WK_MESSEG,m_send);
								
							break;}//重试10次
						}
					}
				}
			}
			USART1_SetFocus(3);
			TaskRepend(3);
		}
	}
}


 
u16 CheckId (u16 addr)
{
	u16 i=0;
	for (i=1;EN_CONFIG[i*2];i++)//修复集中器地址为0时，控制器只能开不能关的问题，2018.8.17
	{
		if (EN_CONFIG[i*2]==addr)
		{
			return 1;
		}
	}
	return 0;//没有找到
}









									//更新集中器中记录的设备状态
u16 Updata_devicestate(u16 addr ,u8 power,u8 state)
{
	u16 i=0;
	u16 state_=0;
	for (i=0;EN_CONFIG[i*2];i++)
	{
		if (EN_CONFIG[i*2]==addr)
		{
			state_= EN_CONFIG[i*2+1]&0x00ff;
			if (power==1)
			{
				state_|=DEVICEON;
			}
			if (state==1)
			{
				state_|=DEVICEUP;
			}
			else if (state==2)
			{
				state_|=DEVICEDOWN;
			}
			EN_CONFIG[i*2+1]=state_;
		}
	}
	return 0;
	
}














						//获取新的控制设备的状态,高8位开关，低8位状态
u16 get_newstate(u8 nowpower,u8 key,u16 addr)
{
	u16 state;
	u8 newpower;
	u8 newstate_;
							//根据协议，当控制状态为2时反向，不为2时输出原状态
	if (nowpower==2)
	{
		state=GetDeviceState(addr);
		if (state&DEVICEON)
		{
			if (state&DEVICEUP)
			{
				if (key==1)//key==1，升按键
				{						//改为关
					newpower=0;newstate_=0;
				} 
				else if (key==2)  //key==2,降按键
				{						//改为降
					newpower=1;newstate_=2;
				}
			}
			else if (state&DEVICEDOWN)
			{
				if (key==1)//key==1，升按键
				{						//改为升
					newpower=1;newstate_=1;
				} 
				else if (key==2)  //key==2,降按键
				{						//改为关
					newpower=0;newstate_=0;
				}				
			}
		}
		else //关着的，改为开
		{
			newpower=1;newstate_=key;
		}
		
	}
	else 
	{
		return (nowpower<<8)|key;
	}
	return (newpower<<8)|newstate_ ;
}


u16 yt_newstate(u8 nowpower,u8 key,u16 addr)
{
	u16 state;
	u8 newpower;
	u8 newstate_;
							//根据协议，当控制状态为2时反向，不为2时输出原状态
	if (nowpower==2)
	{
		state=GetDeviceState(addr);
		if (state&DEVICEON)
		{
			if (state&DEVICEUP)
			{
				if (key==1)//key==1，升按键
				{						//改为关
					newpower=0;newstate_=0;
				} 
				else if (key==2)  //key==2,降按键
				{						//改为降
					newpower=1;newstate_=1;
				}
			}
			else if (state&DEVICEDOWN)
			{
				if (key==1)//key==1，升按键
				{						//改为升
					newpower=1;newstate_=99;
				} 
				else if (key==2)  //key==2,降按键
				{						//改为关
					newpower=0;newstate_=0;
				}				
			}
			else//上位机设置了指定湿度之后是没有升温降温标识的
			{
				if (key==1)
				{
					newstate_=99;
				}
				else if (key==2)
				{
					newstate_=1;
				}
			}
		}
		else //关着的，改为开
		{
			newpower=1;
			if (key==1)
			{
				newstate_=99;
			}
			else if (key==2)
			{
				newstate_=1;
			}
		}
		
	}
	else //不是反向（即是自动控制或者上位机传来的控制），把1改为99,2改为1，实现除湿加湿功能2018.11.17
	{
		if (key==1) newstate_=99;
		else if (key==2) newstate_=1;
		return (nowpower<<8)|newstate_;
	}
	return (newpower<<8)|newstate_ ;
}







					//设备运行状态指示，类型，开关量，状态（升降）
void Device_state_see(u8 type,u8 power,u8 state)
{
	u8 m_send[MESSEG_DATA]={0};//发送的消息
	
						//在这里执行设备状态指示
	m_send[0]=2;//设备状态指示
	if (type==2)//空调
	{
		if (power)
		{
			if (state==1)
			{
				m_send[1]=1;
				m_send[2]=1;
				send_messeg(LIT_MESSEG,m_send);
				send_messeg(LCD_MESSEG,m_send);
				m_send[1]=0;
				m_send[2]=2;
				send_messeg(LIT_MESSEG,m_send);
			}
			else if (state==2)
			{
				m_send[1]=1;
				m_send[2]=2;
				send_messeg(LIT_MESSEG,m_send);
				send_messeg(LCD_MESSEG,m_send);
				m_send[1]=0;
				m_send[2]=1;
				send_messeg(LIT_MESSEG,m_send);
			}
		}
		else
		{
				m_send[1]=0;
				m_send[2]=1;
				send_messeg(LIT_MESSEG,m_send);
				send_messeg(LCD_MESSEG,m_send);
				m_send[1]=0;
				m_send[2]=2;
				send_messeg(LCD_MESSEG,m_send);//改正屏幕的显示不正常bug，2018.10.11
				send_messeg(LIT_MESSEG,m_send);
		}
	}
	else if (type==3||type==5)//除湿机或加湿机
	{
		if (power)
		{
			if (state==1)
			{
				m_send[1]=1;
				m_send[2]=3;
				send_messeg(LIT_MESSEG,m_send);
				send_messeg(LCD_MESSEG,m_send);
				m_send[1]=0;
				m_send[2]=4;
				send_messeg(LIT_MESSEG,m_send);
			}
			else if (state==2)
			{
				m_send[1]=1;
				m_send[2]=4;
				send_messeg(LIT_MESSEG,m_send);
				send_messeg(LCD_MESSEG,m_send);
				m_send[1]=0;
				m_send[2]=3;
				send_messeg(LIT_MESSEG,m_send);
			}
		}
		else
		{
				m_send[1]=0;
				m_send[2]=4;
				send_messeg(LIT_MESSEG,m_send);
				send_messeg(LCD_MESSEG,m_send);
				m_send[1]=0;
				m_send[2]=3;
				send_messeg(LIT_MESSEG,m_send);
				send_messeg(LCD_MESSEG,m_send);//改正屏幕的显示不正常bug，2018.10.11
		}
	}
	else if (type==4)
	{
		if (power)
		{
				m_send[1]=1;
				m_send[2]=6;
				send_messeg(LIT_MESSEG,m_send);
				send_messeg(LCD_MESSEG,m_send);
		}
		else
		{
				m_send[1]=0;
				m_send[2]=6;
				send_messeg(LIT_MESSEG,m_send);
				send_messeg(LCD_MESSEG,m_send);
		}
		
	}

}











void rf_send (u16 i)
{
	u8 send[MESSEG_DATA]={0};
	if ((EN_CONFIG[i*2+1]&0x00ff)==1)//是采集器
	{
		//发送采集器离线消息，在线时的消息在消息循环里发送
		if ((EN_CONFIG[i*2+1]&DEVICEOFFLINE))
		{
			send[0]=2;send[1]=1;send[2]=1;//发送采集器的数据
			send[3]=((u32 )EN_DATA>>24);	//设置地址
			send[4]=((u32 )EN_DATA>>16);
			send[5]=((u32 )EN_DATA>>8);
			send[6]=((u32 )EN_DATA);
			send[7]=EN_CONFIG[i*2]>>8;//地址
			send[8]=EN_CONFIG[i*2];
			if (find_messeg(WK_MESSEG)==0)
				send_messeg(WK_MESSEG,send);//发送给网口
		}
	}
	else
	{
		{
			send[0]=2;//发送消息
			send[1]=EN_CONFIG[i*2+1];//类型
			if ((EN_CONFIG[i*2+1]&DEVICEOFFLINE)==0)//设备是在线的
				send[2]=0;//设备在线
			else 
				send[2]=1;
			send[3]=EN_CONFIG[i*2]>>8;//地址
			send[4]=EN_CONFIG[i*2];
			if ((EN_CONFIG[i*2+1]&DEVICEON)==DEVICEON)
			{
				send[5]=1;//设备开
				if ((EN_CONFIG[i*2+1]&DEVICEUP)==DEVICEUP)
				{
					send[6]=1;//升
				}
				else if ((EN_CONFIG[i*2+1]&DEVICEDOWN)==DEVICEDOWN)//2019.2.28 给上位机返回没有降状态的bug 
				{
					send[6]=2;//降
				}
				else
				{
					send[6]=0;
				}
			}
			else
			{
				send[5]=0;		//设备是关的
				send[6]=0;
			}
			if (find_messeg(WK_MESSEG)==0)
				send_messeg(WK_MESSEG,send);//发送给网口
		}
	}
	
}





				//无线的数据处理
void my_rf_deal (void * t)
{
	u8 send[MESSEG_DATA]={0};
	u16 i=0;
	while (1)
	{
		delay_ms(300);//防止程序死在这里								
									//控制器数据
		for (i=1;EN_CONFIG[i*2+1];i++)
		{
			if ((EN_CONFIG[i*2+1]&0x00ff)==1)//是采集器
			{
				//发送采集器离线消息，在线时的消息在消息循环里发送
				if ((EN_CONFIG[i*2+1]&DEVICEOFFLINE))
				{
					send[0]=2;send[1]=1;send[2]=1;//发送采集器的数据
					send[3]=((u32 )EN_DATA>>24);	//设置地址
					send[4]=((u32 )EN_DATA>>16);
					send[5]=((u32 )EN_DATA>>8);
					send[6]=((u32 )EN_DATA);
					send[7]=EN_CONFIG[i*2]>>8;//地址
					send[8]=EN_CONFIG[i*2];
					if (find_messeg(WK_MESSEG)==0)
						send_messeg(WK_MESSEG,send);//发送给网口
				}
			}
			else
			{
				{
					send[0]=2;//发送消息
					send[1]=EN_CONFIG[i*2+1];//类型
					if ((EN_CONFIG[i*2+1]&DEVICEOFFLINE)==0)//设备是在线的
						send[2]=0;//设备在线
					else 
						send[2]=1;
					send[3]=EN_CONFIG[i*2]>>8;//地址
					send[4]=EN_CONFIG[i*2];
					if ((EN_CONFIG[i*2+1]&DEVICEON)==DEVICEON)
					{
						send[5]=1;//设备开
						if ((EN_CONFIG[i*2+1]&DEVICEUP)==DEVICEUP)
						{
							send[6]=1;//升
						}
						else if ((EN_CONFIG[i*2+1]&DEVICEDOWN)==DEVICEDOWN)//2019.2.28 给上位机返回没有降状态的bug 
						{
							send[6]=2;//降
						}
						else
						{
							send[6]=0;
						}
					}
					else
					{
						send[5]=0;		//设备是关的
						send[6]=0;
					}
					if (find_messeg(WK_MESSEG)==0)
						send_messeg(WK_MESSEG,send);//发送给网口
				}
			}
			delay_ms(5000);
		}
	}
	
}



				//获取设备信息，recv是接收数组
u16 Cmd_0x01 (u16 addr ,u8 *recv)
{
	u8 i=0;
	u8 data[12]={0};
	u8 crc[2]={0};
	data[0]=0xff;
	data[1]=0xff;
	data[2]=addr>>8;
	data[3]=addr;
	data[4]=0x01;
	data[5]=0x00;
	data[6]=0x02;
	data[7]=0x00;
	data[8]=30;//想要获取20位数据，包括PM2.5，CO2
	Get_Crc16(data,9,crc);
	data[9]=crc[0];
	data[10]=crc[1];
	RF1_tx_bytes(data,11);
	
	for(i=0;i<5;i++)
	{
		delay_ms(200);
		if (RF_GetCmd (recv,data[8]+9))
		{
			Get_Crc16(recv,recv[6]+7,crc);
			if (crc[0]==recv[recv[6]+7]&&crc[1]==recv[recv[6]+8])//crc校验通过
			{
				if ((((recv[2]<<8)|recv[3])==addr)&&(recv[4]==(data[4]|0x80)))//地址和命令符合
				{
					return (recv[7]<<8)|recv[8];
				}
			}
		}
	}
	return ERR_TIMEOUT;
}

					//控制设备
u16 Cmd_0x03 (u16 addr ,u8 type,u8 power,u8 state)
{
	u16 i=0;
	u8 recv[30];
	u8 data[13]={0};
	u8 crc[2]={0};
	data[0]=0xff;
	data[1]=0xff;
	data[2]=addr>>8;
	data[3]=addr;
	data[4]=0x03;
	data[5]=0x00;
	data[6]=0x03;
	data[7]=type;
	data[8]=power;
	data[9]=state;
	Get_Crc16(data,10,crc);
	data[10]=crc[0];
	data[11]=crc[1];
	RF1_tx_bytes(data,12);
	for(i=0;i<5;i++)
	{
		delay_ms(200);
		if (RF_GetCmd (recv,15+9))
		{
			Get_Crc16(recv,recv[6]+7,crc);
			if (crc[0]==recv[recv[6]+7]&&crc[1]==recv[recv[6]+8])//crc校验通过
			{
				if ((((recv[2]<<8)|recv[3])==addr)&&(recv[4]==(data[4]|0x80)))//地址和命令符合
				{
					return (recv[7]<<8)|recv[8];
				}
			}
		}
		
	}
	return ERR_TIMEOUT;

}



					//进入配置模式
u16 Cmd_0x06 (u16 addr ,u8 state)
{
	u16 i=0;
	u8 recv[30];
	u8 data[13]={0};
	u8 crc[2]={0};
	data[0]=0xff;
	data[1]=0xff;
	data[2]=addr>>8;
	data[3]=addr;
	data[4]=0x06;
	data[5]=0x00;
	data[6]=0x01;
	data[7]=state;
	Get_Crc16(data,data[6]+7,crc);
	data[8]=crc[0];
	data[9]=crc[1];
	RF1_tx_bytes(data,data[6]+7+2);
	for(i=0;i<5;i++)
	{
		delay_ms(200);
		if (RF_GetCmd (recv,15+9))
		{
			Get_Crc16(recv,recv[6]+7,crc);
			if (crc[0]==recv[recv[6]+7]&&crc[1]==recv[recv[6]+8])//crc校验通过
			{
				if ((recv[4]==(data[4]|0x80)))//命令符合
				{
					return (recv[7]<<8)|recv[8];
				}
			}
		}
		
	}
	return ERR_TIMEOUT;

}

			//配置基本参数，类型，地址等
u16 Cmd_0x07 (u16 addr ,u8 type)
{
	u16 i=0;
	u8 recv[30];
	u8 data[13]={0};
	u8 crc[2]={0};
	data[0]=0xff;
	data[1]=0xff;
	data[2]=0;
	data[3]=0;
	data[4]=0x07;
	data[5]=0x00;
	data[6]=0x04;
	data[7]=addr>>8;
	data[8]=addr;
	data[9]=type;
	data[10]=0x03;//控制器空调的类型为自定义
	Get_Crc16(data,data[6]+7,crc);
	data[data[6] + 7]=crc[0];
	data[data[6] + 8]=crc[1];
	RF1_tx_bytes(data,data[6]+7+2);
	for(i=0;i<5;i++)
	{
		delay_ms(200);
		if (RF_GetCmd (recv,15+9))
		{
			Get_Crc16(recv,recv[6]+7,crc);
			if (crc[0]==recv[recv[6]+7]&&crc[1]==recv[recv[6]+8])//crc校验通过
			{
				if (recv[4]==(data[4]|0x80))//命令符合
				{
					return (recv[7]<<8)|recv[8];
				}
			}
		}
		
	}
	return ERR_TIMEOUT;

}















/////////////////////文件操作相关代码/////////////////////


			//见"file.c"文件



