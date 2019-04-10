#include "includes.h"
#include "rf.h"
#include "hard_irq.h"
#include "my_messeg.h"
#include "my_cfg.h"
#include "my_rf.h"






/***********************************************************

		megÿ��λ�ĺ���
		meg[0],�豸����,0�л�Ϊ�Զ����ƣ�0xff,����ָ����ַ���豸
		meg[1],1��ȡ�豸״̬��2���ֶ������豸,
		meg[2],�豸��ַ�߰�λ
		meg[3],�豸��ַ�ڰ�λ
		meg[4],����ǿ��ƣ�����״̬��0���أ�1������2������
		meg[5],����ǿ��ƣ�����״̬������ʱ�İ�����1������2����

		��ϸ����Ϣ����μ�my_messeg.h�ļ�

***********************************************************/


#ifdef JZQ_V2_6
	//1ʹ�þɰ棬0ʹ���°棬�ɰ洮������������1mg/m3Ϊ��λ���°���0.1mg/m3Ϊ��λ
	#define __USE_OLD   0
#else
	#define __USE_OLD   1
#endif

u8 HANDING=0;//�ֶ������Ƿ��ڽ���


void rf_loop (u16 i);
void rf_send (u16 i);
void rf_hand (u8 *msg);
//�����豸���ϱ�������ַ���豸���ͣ�����ֵ
void rf_deverr (u16 addr,u8 devtype,u8 d);



//������Ϣѭ��
void my_rf_loop (void * t)
{
	u16 i;//ѭ������
	
	RF_M0=0;
	RF_M1=0;
	RF_Init(115200);
	
	USART1_SetFocus(OSPrioHighRdy);
	
	
	RF_SetChannel(Get_MyChanel()); 
	Updata_DeviceNum();
	while (1)
	{
		delay_ms(30);//��ֹû���豸��ʱ������
		for (i=1;EN_CONFIG[i*2+1];i++)
		{
			if (delay_ms(1000)==DELAY_END)//û���յ�������Ϣ
			{
				if (cfg_getstate()==0)
				{
					rf_loop(i);		//��Ϣѭ��
				}
				rf_send(i);		//����״̬��Ϣ
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
	u16 ret;//����ֵ
	ret=Cmd_0x01 (EN_CONFIG[i*2] ,rf_recv);
	if (ret==0)//�ɹ�
	{
		if ((EN_CONFIG[i*2+1]&0x00ff)==1)
		{
			EN_CONFIG[i*2+1]|=DEVICEON;

			
			if (rf_recv[7+9])			//����������
			{
				rf_deverr ((rf_recv[2]<<8)|rf_recv[3],devTypeCjq,rf_recv[7+9]);
				delay_ms(1000);
			}
			rf_cjq_to_data(rf_recv);
			rf_cjq_deal(rf_recv);
		}
		else 
		{
				//�豸����״̬
			if (rf_recv[7+9]==1) 
			{
				EN_CONFIG[i*2+1]|=DEVICEON;
			}
			else 
			{
				EN_CONFIG[i*2+1]&=~DEVICEON;
			}

			if (GetDeviceType(EN_CONFIG[i*2])!=6)//���Ǻ�ʪ��
			{
						//�豸����״̬
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
			if(rf_recv[9+11]) //����©ˮ����
			loushui_warn(EN_CONFIG[i*2],GetDeviceType(EN_CONFIG[i*2]));
		}
		EN_CONFIG[i*2+1]&=~0xf800;//����
	}
	else if (ret==ERR_TIMEOUT)
	{
		EN_CONFIG[i*2+1]+=0x1000;//���߼�����
		if ((EN_CONFIG[i*2+1]>>12)>5)//����5�γ�ʱ��ȷ������
		{
		EN_CONFIG[i*2+1]&=0x00ff;//������
		EN_CONFIG[i*2+1]|=DEVICEOFFLINE;//������
		}
	}
	else
	{
		EN_CONFIG[i*2+1]&=0x00ff;//������
		EN_CONFIG[i*2+1]|=DEVICEOFFLINE;//������
	}


}



//�����豸���ϱ�������ַ���豸���ͣ�����ֵ
void rf_deverr (u16 addr,u8 devtype,u8 d)
{
	u8 send[MESSEG_DATA]={0};
	send[0]=7;
	send[1]=7;//�������ͣ��豸����
	send[2]=devtype;
	send[3]=addr>>8;
	send[4]=addr;
	send[5]=d;
		if (find_messeg(WK_MESSEG)==0)
	send_messeg(WK_MESSEG,send);

}



//�����ߵ�������䵽�ɼ����ṹ����
void rf_cjq_to_data (u8 *rf_data)
{
	cjq_data *cjqdata=GetCJDataAddr();
	f_to_u t={0};
	
	//�ɼ�����ַ
	cjqdata->cjqId=(rf_data[2]<<8)|rf_data[3];
	

	if (rf_data[6]>=30)
	{
	
		//�¶�
		t.u[0]=rf_data[7+10];
		t.u[1]=rf_data[7+11];
		t.u[2]=rf_data[7+12];
		t.u[3]=rf_data[7+13];
		cjqdata->temp=t.f;
		
		//ʪ��
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
	else 
	{
		//�¶�
		cjqdata->temp=rf_data[17]+rf_data[18]/10.0;
		
		//ʪ��
		cjqdata->humi=rf_data[19]+rf_data[20]/10.0;

		//TVOC
		cjqdata->tvoc=rf_data[21]+rf_data[22]/10.0;		
	}
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
		send_messeg(WK_MESSEG,send);//���͸�����
	
}


			//���Ͳɼ�������
void rf_cjq_deal(u8 *data) 
{
	u16 temp=0;
	cjq_data *t=GetCJDataAddr();
	u8 send[MESSEG_DATA]={0};
	
	
		//temp=data[17]*10+data[18];//ԭ����*10,�¶�
		temp=t->temp*10;
		send[0]=4;send[3]=temp>>8;send[4]=temp;
	
#if __USE_OLD == 0
		//temp=data[19]*10+data[20];//ԭ���ݣ�ʪ��
		temp=t->humi*10;
#else
		//temp=data[19];//ԭ���ݣ�ʪ��
		temp=t->humi;
#endif
		send[5]=temp>>8;send[6]=temp;
	
		//temp=data[21]*10+data[22];
		temp=t->tvoc*10;
		send[7]=temp>>8;send[8]=temp;
		send_messeg(LCD_MESSEG,send);//���͸���Ļ��ʾ
		
		send[0]=2;send[1]=1;send[2]=0;//���Ͳɼ���������
		send[3]=((u32 )EN_DATA>>24);	//���õ�ַ
		send[4]=((u32 )EN_DATA>>16);
		send[5]=((u32 )EN_DATA>>8);
		send[6]=((u32 )EN_DATA);
		if (find_messeg(WK_MESSEG)==0)
			send_messeg(WK_MESSEG,send);//���͸�����
		data[0]=0;

}









void rf_hand (u8 *msg)
{
	u8 m_send[MESSEG_DATA]={0};//���������ڷ��ؿ�����Ϣ
	u16 my_i=0;
	u16 m=0;//����ʧ�ܴ�����
	u16 ret;
	u16 id=0;
	u16 newret=0;
	u8 newpower;
	u8 newstate_;//newstate���������������ϵͳ�ؼ��֣����Զ��ı�
	if ((msg[0]!=0)&&(msg[1]==2)) 
	{
		m=0;
		id=(msg[2]<<8)|msg[3];
		if (id==0)				//��ַΪ0������ȫ��ͬ���豸
		{
			my_i=0;
			while(my_i<GetDeviceNum(msg[0]))
			{
				if (GetDeviceState(GetNextDeviceaddr(msg[0],my_i))&DEVICEOFFLINE)
				{
					my_i++;//�豸������
					continue;
				}
				delay_ms(200);			
										//��ȡ�µĿ����豸��״̬,��8λ���أ���8λ״̬
				newret=get_newstate(msg[4],msg[5],GetNextDeviceaddr(msg[0],my_i));newstate_=newret;newpower=(newret>>8);
														//�豸��ַ���豸���ͣ�����״̬�����Ӳ���
				ret=Cmd_0x03 (GetNextDeviceaddr(msg[0],my_i),msg[0],newpower,newstate_);
				if (ret==0)
				{
					Updata_devicestate(GetNextDeviceaddr(msg[0],my_i) ,newpower,newstate_);
					Device_state_see(msg[0],newpower,newstate_);
					my_i++;//���Ƴɹ���������һ��
				}
				else
				{
					m++;if (m>10){m=0;my_i++;}//����10��
				}
			}
			//��һ�����ƺ�ʪ����ѭ��
			if (msg[0]==3||msg[0]==5)
			{
				my_i=0;
				msg[0]=6;
				while(my_i<GetDeviceNum(6))//һ����ĸ���
				{
					if (GetDeviceState(GetNextDeviceaddr(msg[0],my_i))&DEVICEOFFLINE)
					{
						my_i++;//�豸������
						continue;
					}
					delay_ms(200);	
					newret=yt_newstate(msg[4],msg[5],GetNextDeviceaddr(msg[0],my_i));newpower=(newret>>8);
					newstate_=newret;
															//�豸��ַ���豸���ͣ�����״̬�����Ӳ���
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
						my_i++;//���Ƴɹ���������һ��
					}
					else
					{
						m++;if (m>10){m=0;my_i++;}//����10��
					}
				}
			}
		}
		else //����ָ����ַ���豸
		{
			while (1)
			{
				delay_ms(200);			
				if (CheckId (id)==0)
				{
					//�����ڷ���ִ�н����������ָ����ip��ַ
					m_send[0]=5;m_send[1]=ERR_NONEADDR>>8;m_send[2]=ERR_NONEADDR;
					send_messeg(WK_MESSEG,m_send);
					break;
				}
												//�豸��ַ���豸���ͣ�����״̬�����Ӳ���
				ret=Cmd_0x03 (id,GetDeviceType(id),msg[4],msg[5]);
				if (ret==0)
				{
					Updata_devicestate(id ,msg[4],msg[5]);
					Device_state_see(GetDeviceType(id),msg[4],msg[5]);
					//�����ڷ���ִ�н�����ɹ�
					m_send[0]=5;m_send[1]=ret>>8;m_send[2]=ret;
					send_messeg(WK_MESSEG,m_send);
					
					break;//���Ƴɹ����˳�
				}
				else if (GetDeviceState(id)&DEVICEOFFLINE)
				{
					//�����ڷ���ִ�н�����豸������
					m_send[0]=5;m_send[1]=ERR_OFFLINE>>8;m_send[2]=ERR_OFFLINE;
					send_messeg(WK_MESSEG,m_send);
					break;//�豸������
				}
				else
				{
					m++;if (m>10){
						//�����ڷ���ִ�н����ִ�н��
					m_send[0]=5;m_send[1]=ret>>8;m_send[2]=ret;
					send_messeg(WK_MESSEG,m_send);
						
					break;}//����10��
				}
			}
		}
	}
}






					//�����ֶ���Ϣ
void my_rf_hand (void * t)
{
	u8 msg[MESSEG_DATA]={0};
	u8 m_send[MESSEG_DATA]={0};//���������ڷ��ؿ�����Ϣ
	u16 my_i=0;
	u16 m=0;//����ʧ�ܴ�����
	u16 ret;
	u16 id=0;
	u16 newret=0;
	u8 newpower;
	u8 newstate_;//newstate���������������ϵͳ�ؼ��֣����Զ��ı�
	while(1)
	{
		delay_ms(200);
		if (get_messeg(RF_MESSEG,msg))//û����Ϣ
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
				if (id==0)				//��ַΪ0������ȫ��ͬ���豸
				{
					my_i=0;
					while(my_i<GetDeviceNum(msg[0]))
					{
						if (GetDeviceState(GetNextDeviceaddr(msg[0],my_i))&DEVICEOFFLINE)
						{
							my_i++;//�豸������
							continue;
						}
						delay_ms(200);			
												//��ȡ�µĿ����豸��״̬,��8λ���أ���8λ״̬
						newret=get_newstate(msg[4],msg[5],GetNextDeviceaddr(msg[0],my_i));newstate_=newret;newpower=(newret>>8);
																//�豸��ַ���豸���ͣ�����״̬�����Ӳ���
						ret=Cmd_0x03 (GetNextDeviceaddr(msg[0],my_i),msg[0],newpower,newstate_);
						if (ret==0)
						{
							Updata_devicestate(GetNextDeviceaddr(msg[0],my_i) ,newpower,newstate_);
							Device_state_see(msg[0],newpower,newstate_);
							my_i++;//���Ƴɹ���������һ��
						}
						else
						{
							m++;if (m>10){m=0;my_i++;}//����10��
						}
					}
					//��һ�����ƺ�ʪ����ѭ��
					if (msg[0]==3||msg[0]==5)
					{
						my_i=0;
						msg[0]=6;
						while(my_i<GetDeviceNum(6))//һ����ĸ���
						{
							if (GetDeviceState(GetNextDeviceaddr(msg[0],my_i))&DEVICEOFFLINE)
							{
								my_i++;//�豸������
								continue;
							}
							delay_ms(200);	
							newret=yt_newstate(msg[4],msg[5],GetNextDeviceaddr(msg[0],my_i));newpower=(newret>>8);
							newstate_=newret;
																	//�豸��ַ���豸���ͣ�����״̬�����Ӳ���
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
								my_i++;//���Ƴɹ���������һ��
							}
							else
							{
								m++;if (m>10){m=0;my_i++;}//����10��
							}
						}
					}
				}
				else //����ָ����ַ���豸
				{
					while (1)
					{
						delay_ms(200);			
						if (CheckId (id)==0)
						{
							//�����ڷ���ִ�н����������ָ����ip��ַ
							m_send[0]=5;m_send[1]=ERR_NONEADDR>>8;m_send[2]=ERR_NONEADDR;
							send_messeg(WK_MESSEG,m_send);
							break;
						}
														//�豸��ַ���豸���ͣ�����״̬�����Ӳ���
						ret=Cmd_0x03 (id,GetDeviceType(id),msg[4],msg[5]);
						if (ret==0)
						{
							Updata_devicestate(id ,msg[4],msg[5]);
							Device_state_see(GetDeviceType(id),msg[4],msg[5]);
							//�����ڷ���ִ�н�����ɹ�
							m_send[0]=5;m_send[1]=ret>>8;m_send[2]=ret;
							send_messeg(WK_MESSEG,m_send);
							
							break;//���Ƴɹ����˳�
						}
						else if (GetDeviceState(id)&DEVICEOFFLINE)
						{
							//�����ڷ���ִ�н�����豸������
							m_send[0]=5;m_send[1]=ERR_OFFLINE>>8;m_send[2]=ERR_OFFLINE;
							send_messeg(WK_MESSEG,m_send);
							break;//�豸������
						}
						else
						{
							m++;if (m>10){
								//�����ڷ���ִ�н����ִ�н��
							m_send[0]=5;m_send[1]=ret>>8;m_send[2]=ret;
							send_messeg(WK_MESSEG,m_send);
								
							break;}//����10��
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
	for (i=1;EN_CONFIG[i*2];i++)//�޸���������ַΪ0ʱ��������ֻ�ܿ����ܹص����⣬2018.8.17
	{
		if (EN_CONFIG[i*2]==addr)
		{
			return 1;
		}
	}
	return 0;//û���ҵ�
}









									//���¼������м�¼���豸״̬
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














						//��ȡ�µĿ����豸��״̬,��8λ���أ���8λ״̬
u16 get_newstate(u8 nowpower,u8 key,u16 addr)
{
	u16 state;
	u8 newpower;
	u8 newstate_;
							//����Э�飬������״̬Ϊ2ʱ���򣬲�Ϊ2ʱ���ԭ״̬
	if (nowpower==2)
	{
		state=GetDeviceState(addr);
		if (state&DEVICEON)
		{
			if (state&DEVICEUP)
			{
				if (key==1)//key==1��������
				{						//��Ϊ��
					newpower=0;newstate_=0;
				} 
				else if (key==2)  //key==2,������
				{						//��Ϊ��
					newpower=1;newstate_=2;
				}
			}
			else if (state&DEVICEDOWN)
			{
				if (key==1)//key==1��������
				{						//��Ϊ��
					newpower=1;newstate_=1;
				} 
				else if (key==2)  //key==2,������
				{						//��Ϊ��
					newpower=0;newstate_=0;
				}				
			}
		}
		else //���ŵģ���Ϊ��
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
							//����Э�飬������״̬Ϊ2ʱ���򣬲�Ϊ2ʱ���ԭ״̬
	if (nowpower==2)
	{
		state=GetDeviceState(addr);
		if (state&DEVICEON)
		{
			if (state&DEVICEUP)
			{
				if (key==1)//key==1��������
				{						//��Ϊ��
					newpower=0;newstate_=0;
				} 
				else if (key==2)  //key==2,������
				{						//��Ϊ��
					newpower=1;newstate_=1;
				}
			}
			else if (state&DEVICEDOWN)
			{
				if (key==1)//key==1��������
				{						//��Ϊ��
					newpower=1;newstate_=99;
				} 
				else if (key==2)  //key==2,������
				{						//��Ϊ��
					newpower=0;newstate_=0;
				}				
			}
			else//��λ��������ָ��ʪ��֮����û�����½��±�ʶ��
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
		else //���ŵģ���Ϊ��
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
	else //���Ƿ��򣨼����Զ����ƻ�����λ�������Ŀ��ƣ�����1��Ϊ99,2��Ϊ1��ʵ�ֳ�ʪ��ʪ����2018.11.17
	{
		if (key==1) newstate_=99;
		else if (key==2) newstate_=1;
		return (nowpower<<8)|newstate_;
	}
	return (newpower<<8)|newstate_ ;
}







					//�豸����״ָ̬ʾ�����ͣ���������״̬��������
void Device_state_see(u8 type,u8 power,u8 state)
{
	u8 m_send[MESSEG_DATA]={0};//���͵���Ϣ
	
						//������ִ���豸״ָ̬ʾ
	m_send[0]=2;//�豸״ָ̬ʾ
	if (type==2)//�յ�
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
				send_messeg(LCD_MESSEG,m_send);//������Ļ����ʾ������bug��2018.10.11
				send_messeg(LIT_MESSEG,m_send);
		}
	}
	else if (type==3||type==5)//��ʪ�����ʪ��
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
				send_messeg(LCD_MESSEG,m_send);//������Ļ����ʾ������bug��2018.10.11
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
	if ((EN_CONFIG[i*2+1]&0x00ff)==1)//�ǲɼ���
	{
		//���Ͳɼ���������Ϣ������ʱ����Ϣ����Ϣѭ���﷢��
		if ((EN_CONFIG[i*2+1]&DEVICEOFFLINE))
		{
			send[0]=2;send[1]=1;send[2]=1;//���Ͳɼ���������
			send[3]=((u32 )EN_DATA>>24);	//���õ�ַ
			send[4]=((u32 )EN_DATA>>16);
			send[5]=((u32 )EN_DATA>>8);
			send[6]=((u32 )EN_DATA);
			send[7]=EN_CONFIG[i*2]>>8;//��ַ
			send[8]=EN_CONFIG[i*2];
			if (find_messeg(WK_MESSEG)==0)
				send_messeg(WK_MESSEG,send);//���͸�����
		}
	}
	else
	{
		{
			send[0]=2;//������Ϣ
			send[1]=EN_CONFIG[i*2+1];//����
			if ((EN_CONFIG[i*2+1]&DEVICEOFFLINE)==0)//�豸�����ߵ�
				send[2]=0;//�豸����
			else 
				send[2]=1;
			send[3]=EN_CONFIG[i*2]>>8;//��ַ
			send[4]=EN_CONFIG[i*2];
			if ((EN_CONFIG[i*2+1]&DEVICEON)==DEVICEON)
			{
				send[5]=1;//�豸��
				if ((EN_CONFIG[i*2+1]&DEVICEUP)==DEVICEUP)
				{
					send[6]=1;//��
				}
				else if ((EN_CONFIG[i*2+1]&DEVICEDOWN)==DEVICEDOWN)//2019.2.28 ����λ������û�н�״̬��bug 
				{
					send[6]=2;//��
				}
				else
				{
					send[6]=0;
				}
			}
			else
			{
				send[5]=0;		//�豸�ǹص�
				send[6]=0;
			}
			if (find_messeg(WK_MESSEG)==0)
				send_messeg(WK_MESSEG,send);//���͸�����
		}
	}
	
}





				//���ߵ����ݴ���
void my_rf_deal (void * t)
{
	u8 send[MESSEG_DATA]={0};
	u16 i=0;
	while (1)
	{
		delay_ms(300);//��ֹ������������								
									//����������
		for (i=1;EN_CONFIG[i*2+1];i++)
		{
			if ((EN_CONFIG[i*2+1]&0x00ff)==1)//�ǲɼ���
			{
				//���Ͳɼ���������Ϣ������ʱ����Ϣ����Ϣѭ���﷢��
				if ((EN_CONFIG[i*2+1]&DEVICEOFFLINE))
				{
					send[0]=2;send[1]=1;send[2]=1;//���Ͳɼ���������
					send[3]=((u32 )EN_DATA>>24);	//���õ�ַ
					send[4]=((u32 )EN_DATA>>16);
					send[5]=((u32 )EN_DATA>>8);
					send[6]=((u32 )EN_DATA);
					send[7]=EN_CONFIG[i*2]>>8;//��ַ
					send[8]=EN_CONFIG[i*2];
					if (find_messeg(WK_MESSEG)==0)
						send_messeg(WK_MESSEG,send);//���͸�����
				}
			}
			else
			{
				{
					send[0]=2;//������Ϣ
					send[1]=EN_CONFIG[i*2+1];//����
					if ((EN_CONFIG[i*2+1]&DEVICEOFFLINE)==0)//�豸�����ߵ�
						send[2]=0;//�豸����
					else 
						send[2]=1;
					send[3]=EN_CONFIG[i*2]>>8;//��ַ
					send[4]=EN_CONFIG[i*2];
					if ((EN_CONFIG[i*2+1]&DEVICEON)==DEVICEON)
					{
						send[5]=1;//�豸��
						if ((EN_CONFIG[i*2+1]&DEVICEUP)==DEVICEUP)
						{
							send[6]=1;//��
						}
						else if ((EN_CONFIG[i*2+1]&DEVICEDOWN)==DEVICEDOWN)//2019.2.28 ����λ������û�н�״̬��bug 
						{
							send[6]=2;//��
						}
						else
						{
							send[6]=0;
						}
					}
					else
					{
						send[5]=0;		//�豸�ǹص�
						send[6]=0;
					}
					if (find_messeg(WK_MESSEG)==0)
						send_messeg(WK_MESSEG,send);//���͸�����
				}
			}
			delay_ms(5000);
		}
	}
	
}



				//��ȡ�豸��Ϣ��recv�ǽ�������
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
	data[8]=30;//��Ҫ��ȡ20λ���ݣ�����PM2.5��CO2
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
			if (crc[0]==recv[recv[6]+7]&&crc[1]==recv[recv[6]+8])//crcУ��ͨ��
			{
				if ((((recv[2]<<8)|recv[3])==addr)&&(recv[4]==(data[4]|0x80)))//��ַ���������
				{
					return (recv[7]<<8)|recv[8];
				}
			}
		}
	}
	return ERR_TIMEOUT;
}

					//�����豸
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
			if (crc[0]==recv[recv[6]+7]&&crc[1]==recv[recv[6]+8])//crcУ��ͨ��
			{
				if ((((recv[2]<<8)|recv[3])==addr)&&(recv[4]==(data[4]|0x80)))//��ַ���������
				{
					return (recv[7]<<8)|recv[8];
				}
			}
		}
		
	}
	return ERR_TIMEOUT;

}



					//��������ģʽ
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
			if (crc[0]==recv[recv[6]+7]&&crc[1]==recv[recv[6]+8])//crcУ��ͨ��
			{
				if ((recv[4]==(data[4]|0x80)))//�������
				{
					return (recv[7]<<8)|recv[8];
				}
			}
		}
		
	}
	return ERR_TIMEOUT;

}

			//���û������������ͣ���ַ��
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
	data[10]=0x03;//�������յ�������Ϊ�Զ���
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
			if (crc[0]==recv[recv[6]+7]&&crc[1]==recv[recv[6]+8])//crcУ��ͨ��
			{
				if (recv[4]==(data[4]|0x80))//�������
				{
					return (recv[7]<<8)|recv[8];
				}
			}
		}
		
	}
	return ERR_TIMEOUT;

}















/////////////////////�ļ�������ش���/////////////////////


			//��"file.c"�ļ�


