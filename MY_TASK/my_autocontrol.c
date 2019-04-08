
#include "includes.h"
#include "my_messeg.h"
#include "my_autocontrol.h"
//�Զ�����ʵ���ļ�

/*************************************************************


2018.8.2 �����Զ����ƣ�
2018.8.3 ���뻷��ֵ�쳣�ƹ�Ч��
					����ʱ�ƴ���ʾ��ɫ���쳣ʱ�ƴ���ʾ��ɫ��ͬʱ�쳣�Ļ���ֵ��Ӧ�İ�����Χ�ĵ�Ȧ�ᾯʾ
					�����ֶ�ʱ�����������Զ�ʱ��˸��ֱ���ָ�����ֵ��Ϩ��
2019.4.3 �Զ�������Ϊ˵��
					�Զ����������������Ϊ�ǣ������豸���������Ժ챨�����ƴ��Ժ챨�����ϱ�������������
					-------------------------------------------------------------------|
					���غ���Ϊ|����������|�����ݲ�|������|������|�ƴ�|�ϱ�������|�豸  |
					----------+----------+--------+------+------+----+----------+------|
					һ������	|��				 |��			|��		 |��		|��ɫ|���ϱ�		|��    |
					----------+----------+--------+------+------+----+----------+------|
					��������	|��				 |��			|��		 |��		|��ɫ|���ϱ�		|������|
					----------+----------+--------+------+------+----+----------+------|
					�޶ȳ���	|��				 |��			|��		 |������|��ɫ|���ϱ�		|��    |
					----------+----------+--------+------+------+----+----------+------|
					�ݲ��	|��				 |��			|��		 |��ɫ	|��ɫ|�ϱ�	  	|��    |
					-------------------------------------------------------------------|

************************************************************/

#define WARN_TOLERANCE getWarnTolerance()
//����ֵ
#define OVERSHOOT_RANGE getAutoCtrlAmount()

//1ʹ�þɰ棬0ʹ���°棬�ɰ洮������������1mg/m3Ϊ��λ���°���0.1mg/m3Ϊ��λ
#define __USE_OLD   0



u8 ctrl_temp (float now_temp);
u8 ctrl_humi (float now_humi);
u8 ctrl_air (float now_tvoc);



//�Զ�����ѭ��
void my_autocontrol (void * t)
{
	u8 m_send[MESSEG_DATA]={0};
	cjq_data *cj_data;
	float now_temp;//����ֵ������Ϊ�����ͣ��Ƚ�ʱ�Ƚ�С����2018.11.19
	float now_humi;
	float now_tvoc;
	u8 en_warning=0;
	cj_data=GetCJDataAddr ( );
	delay_ms(3000);//ϵͳ����3����֮��ʼִ��
	while (1)
	{
		delay_ms(getAutoCtrlFrequency()*1000+1);//ÿ30��ִ��һ���Զ��ж�
		if (cj_data->cjqId==0) continue;
		now_temp=cj_data->temp;//����С����2018.11.19
		now_humi=cj_data->humi;
		now_tvoc=cj_data->tvoc;

		#if __USE_OLD==0
				now_tvoc=now_tvoc*10;//TVOC�����ǳ���10֮������ݣ��������tvoc����*10��Ƚ�,2018.11.19
		#endif

		
		en_warning|=ctrl_temp(now_temp);
		en_warning|=ctrl_humi(now_humi);
		en_warning|=ctrl_air(now_tvoc);

		if (en_warning)		//�ƴ��쳣ָʾ
		{
			m_send[0]=3;
			m_send[1]=5;
			m_send[3]=200;m_send[4]=0;m_send[5]=0;
			send_messeg (LIT_MESSEG,m_send);
			en_warning=0;
		}
		else
		{
			m_send[0]=3;
			m_send[1]=5;
			m_send[3]=0;m_send[4]=200;m_send[5]=0;
			send_messeg (LIT_MESSEG,m_send);
		}	

	}
}









//�����豸��ַ���豸���ͣ��������ͣ�����ʱ��ֵ
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










//�����¶�,����1���б�����0��û��
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
		if (Lcd_GetHandstate()==0)//�Զ�ģʽ
		{
			send_messeg (RF_MESSEG,m_send);
		}
		
		
		if (now_temp>=Lcd_GetLimitData (0)+WARN_TOLERANCE)	//�������޼��ݲ�豨��
		{
			m_send[2]=1;//��˸��λ��
			m_send[0]=1;m_send[1]=1;
			send_messeg (LIT_MESSEG,m_send);
			warning=1;
			send_warn(Get_MyAddr(),0,1,(u8) now_temp);//���ͱ���������
		}
	}
	else if (now_temp<=Lcd_GetLimitData (1))
	{
		m_send[0]=2;
		m_send[1]=2;m_send[2]=0;m_send[3]=0;
		m_send[4]=1;
		m_send[5]=1;
		if (Lcd_GetHandstate()==0)//�Զ�ģʽ
		{
			send_messeg (RF_MESSEG,m_send);
		}
			
		if (now_temp+WARN_TOLERANCE<=Lcd_GetLimitData (1))//��������-�ݲ�豨��
		{
			m_send[2]=2;//��˸��λ��
			m_send[0]=1;m_send[1]=1;
			send_messeg (LIT_MESSEG,m_send);
			warning=2;
			send_warn(Get_MyAddr(),0,2,(u8) now_temp);//���ͱ���������
		}
	}
	else if ((now_temp+OVERSHOOT_RANGE<Lcd_GetLimitData (0))&&(now_temp>Lcd_GetLimitData (1)+OVERSHOOT_RANGE))
	{
		m_send[0]=2;
		m_send[1]=2;m_send[2]=0;m_send[3]=0;
		m_send[4]=0;
		m_send[5]=0;
		if (Lcd_GetHandstate()==0)//�Զ�ģʽ
		{
			send_messeg (RF_MESSEG,m_send);
		}
		m_send[2]=1;//��˸��λ��
		m_send[0]=1;m_send[1]=0;
		send_messeg (LIT_MESSEG,m_send);
		m_send[2]=2;//��˸��λ��
		m_send[0]=1;m_send[1]=0;
		send_messeg (LIT_MESSEG,m_send);
	}
	else					//�ڳ������䣬�ı���Ϊ�������������豸��������
	{
		m_send[2]=1;//��˸��λ��
		m_send[0]=1;m_send[1]=0;
		send_messeg (LIT_MESSEG,m_send);
		m_send[2]=2;//��˸��λ��
		m_send[0]=1;m_send[1]=0;
		send_messeg (LIT_MESSEG,m_send);
	}
	return warning;
}










//����ʪ�ȣ�1���б�����0��û�б���
u8 ctrl_humi (float now_humi)
{
	u8 m_send[MESSEG_DATA]={0};
	u8 warning=0;						
							/*��ʪ��ʪ���Զ�����*/
	if (now_humi>=Lcd_GetLimitData (2))
	{
		m_send[0]=3;
		m_send[1]=2;m_send[2]=0;m_send[3]=0;
		m_send[4]=1;
		m_send[5]=2;
		if (Lcd_GetHandstate()==0)//�Զ�ģʽ
		{
			send_messeg (RF_MESSEG,m_send);
		}
		if (now_humi>=Lcd_GetLimitData (2)+WARN_TOLERANCE)	//�������޼��ݲ�豨��
		{
			m_send[2]=3;//��˸��λ��
			m_send[0]=1;m_send[1]=1;
			send_messeg (LIT_MESSEG,m_send);			
			warning=3;
			send_warn(Get_MyAddr(),0,3,(u8)now_humi);//���ͱ���������
		}
	}
	else if (now_humi<=Lcd_GetLimitData (3))
	{
		m_send[0]=5;
		m_send[1]=2;m_send[2]=0;m_send[3]=0;
		m_send[4]=1;
		m_send[5]=1;
		if (Lcd_GetHandstate()==0)//�Զ�ģʽ
		{
			send_messeg (RF_MESSEG,m_send);
		}
		if (now_humi+WARN_TOLERANCE<=Lcd_GetLimitData (3))//��������-�ݲ�豨��
		{
			m_send[2]=4;//��˸��λ��
			m_send[0]=1;m_send[1]=1;
			send_messeg (LIT_MESSEG,m_send);
			warning=4;
			send_warn(Get_MyAddr(),0,4,(u8)now_humi);//���ͱ���������
		}
	}
	else if ((now_humi+OVERSHOOT_RANGE<Lcd_GetLimitData (2))&&(now_humi>Lcd_GetLimitData (3)+OVERSHOOT_RANGE))
	{
		m_send[0]=3;
		m_send[1]=2;m_send[2]=0;m_send[3]=0;
		m_send[4]=0;
		m_send[5]=0;
		if (Lcd_GetHandstate()==0)//�Զ�ģʽ
		{
			send_messeg (RF_MESSEG,m_send);
			m_send[0]=5;
			send_messeg (RF_MESSEG,m_send);
		}
		m_send[2]=3;//��˸��λ��
		m_send[0]=1;m_send[1]=0;
		send_messeg (LIT_MESSEG,m_send);
		m_send[2]=4;//��˸��λ��
		m_send[0]=1;m_send[1]=0;
		send_messeg (LIT_MESSEG,m_send);				
	}
	else
	{
		m_send[2]=3;//��˸��λ��
		m_send[0]=1;m_send[1]=0;
		send_messeg (LIT_MESSEG,m_send);
		m_send[2]=4;//��˸��λ��
		m_send[0]=1;m_send[1]=0;
		send_messeg (LIT_MESSEG,m_send);
	}
	return warning;
}



//������������
u8 ctrl_air (float now_tvoc)
{
	u8 m_send[MESSEG_DATA]={0};
	u8 warning=0;		
	
							/*�������������Զ�����*/
	if (now_tvoc>=Lcd_GetLimitData (4))
	{
		m_send[0]=4;
		m_send[1]=2;m_send[2]=0;m_send[3]=0;
		m_send[4]=1;
		m_send[5]=2;
		if (Lcd_GetHandstate()==0)//�Զ�ģʽ
		{
			send_messeg (RF_MESSEG,m_send);
		}
		if (now_tvoc>=Lcd_GetLimitData (4)+WARN_TOLERANCE)	//�������޼��ݲ�豨��
		{
			m_send[2]=6;//��˸��λ��
			m_send[0]=1;m_send[1]=1;
			send_messeg (LIT_MESSEG,m_send);
			warning=6;
			send_warn(Get_MyAddr(),0,5,(u8)now_tvoc);//���ͱ���������
		}
	}
	else
	{
		m_send[0]=4;
		m_send[1]=2;m_send[2]=0;m_send[3]=0;
		m_send[4]=0;
		m_send[5]=0;
		if (Lcd_GetHandstate()==0)//�Զ�ģʽ
		{
			send_messeg (RF_MESSEG,m_send);
		}
		m_send[2]=6;//��˸��λ��
		m_send[0]=1;m_send[1]=0;
		send_messeg (LIT_MESSEG,m_send);
	}
	return warning;
}



