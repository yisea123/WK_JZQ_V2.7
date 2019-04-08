#include "includes.h"
#include "data_map.h"

/************************************

����ļ�����ϵͳ�����Ĵ洢�ռ����
������Ҫ�õ�FLASH�洢�Ĳ���
�ڴ����������ļ�����
������
		��Ļ���ò���
		�������ò���
		�������ò���

**************************************/

//����һ���ܵĴ洢������Ϣ�ı�������
static u16 ALL_PARAMETERS[ALL_PARA_LENGTH];


//u16 LCD_DATA[LCD_DATA_NUM]={0};


			//��������������,��������ַ���ɼ�����ַ����������ַ
			//����[0]��ʾ�豸��ַ��[1]��8λ��ʾ�豸���͸�ʽ����8λ���豸״̬
			//�豸״̬λ��0����ʾ����״̬����1,2��λ��ʾ����״̬����3��λ��ʾ����״̬,
			//�豸��ַ���ռ��������ɼ�������������˳��
//u16 EN_CONFIG[CONFIG_DATA_NUM]={1,0,2,1,3,3,4,2,5,4,6,5};

 
 
 
 
#define FLASH_SAVE_ADDR  (0x8040000-1024)
#define FLASH_LCD_ADDR FLASH_SAVE_ADDR
#define FLASH_CFG_ADDR (FLASH_SAVE_ADDR+LCD_DATA_NUM*2)
#define DEVICEOFFLINE 	0x0800


void Save_Config (void)
{
	STMFLASH_Write(FLASH_CFG_ADDR,EN_CONFIG,CONFIG_DATA_NUM);
}

void Load_Config(void)
{
	u16 i=0;
	STMFLASH_Read(FLASH_CFG_ADDR,EN_CONFIG,CONFIG_DATA_NUM);
	for (i=0;i<CONFIG_DATA_NUM;i++)
	{
		if (EN_CONFIG[i]==0xffff) EN_CONFIG[i]=0;
	}
	for (i=0;i<100;i++)
	{
		if (i&1) {EN_CONFIG[i]&=0x00ff;//��ʼ��֮�󣬰��豸״̬Ϊ0��2018.8.18
			if (EN_CONFIG[i]&0x00ff) EN_CONFIG[i]|=DEVICEOFFLINE;//��ʼ��Ϊ����״̬��2018.8.22
		}
	}
	EN_CONFIG[1]|=0x0100;//��������ʼ��Ϊ����
}
 
 
				//�����ֶ���������λ
#define CHUSHI_STATE 			LCD_DATA[14]
#define KONGTIAO_STATE 		LCD_DATA[15]
#define JINHUA_STATE  		LCD_DATA[16]
#define HAND_STATE   			LCD_DATA[17]

						//������Ļ������
void Save_LCD (void)
{
	STMFLASH_Write(FLASH_LCD_ADDR,LCD_DATA,LCD_DATA_NUM);
}


void Load_LCD(void)
{
	u16 i=0;
	u16 tt[4]={0};
	tt[0]=CHUSHI_STATE;
	tt[1]=KONGTIAO_STATE;
	tt[2]=JINHUA_STATE;
	tt[3]=HAND_STATE;
	STMFLASH_Read(FLASH_LCD_ADDR,LCD_DATA,LCD_DATA_NUM);
	for (i=0;i<LCD_DATA_NUM;i++)
	{
		if (LCD_DATA[i]==0xffff)
			LCD_DATA[i]=0;
	}
	CHUSHI_STATE=tt[0];
	KONGTIAO_STATE=tt[1];
	JINHUA_STATE =tt[2];
//	HAND_STATE  =tt[3];ע����һ���ֶ��Զ�ģʽ���籣��
	
}
 
 


/****************************************************************
*******************�����������
*****************************************************************/

						//�豸�������������������͵ĸ�ʽ
						//�������ͨ����ʼ���������
static u16 DEVICE_NUM[DEVICE_DATA_NUM]={1,1,1,3,1,2,1,4,1,5};
					//�ɼ���������
//static u8 EN_DATA_[30]={0};
static cjq_data CJQ_DATA={0};


/***************************************************************

		config������Ϣ�洢�ṹ��
			EN_CONFIG[0]~EN_CONFIG[99] �豸������Ϣ
			EN_CONFIG[CONFIG_DATA_NUM-35]	�Զ����Ʊ����ݲ�ֵ
			EN_CONFIG[CONFIG_DATA_NUM-34]	�Զ���ȡIP��ַ����
			EN_CONFIG[CONFIG_DATA_NUM-33]~EN_CONFIG[CONFIG_DATA_NUM-24]	�������ƣ�18��Ӣ���ַ�
			EN_CONFIG[CONFIG_DATA_NUM-23]	�Զ����Ƴ�����
			EN_CONFIG[CONFIG_DATA_NUM-22]	�Զ�����Ƶ��
			EN_CONFIG[CONFIG_DATA_NUM-21]	�����ŵ�
			EN_CONFIG[CONFIG_DATA_NUM-20]~EN_CONFIG[CONFIG_DATA_NUM-1]	IP��ַ���


***************************************************************/

//�ҳ��豸����
void Updata_DeviceNum (void)
{
	u16 i=0;u16 j=0;
	
	for (i=0;i<DEVICE_DATA_NUM;i++)
	{
		DEVICE_NUM[i]=0;
	}
	DEVICE_NUM[1]=1;//�ɼ���
	DEVICE_NUM[3]=2;//�յ�
	DEVICE_NUM[5]=3;//��ʪ��
	DEVICE_NUM[7]=4;//������
	DEVICE_NUM[9]=5;//��ʪ��
	DEVICE_NUM[11]=6;//��ʪ��
	for (i=0;i<DEVICE_DATA_NUM;i+=2)
	{
		for (j=2;(EN_CONFIG[j+1]&0x00ff);j+=2)
		{
			if ((DEVICE_NUM[i+1]&0x00ff)==(0x00ff&EN_CONFIG[j+1]))
			{
				DEVICE_NUM[i]++;
			}
		}
	}
}



//����ָ����ַ���豸����
u8 GetDeviceType (u16 addr)
{
	u16 i=0;
	for (i=0;EN_CONFIG[i*2];i++)
	{
		if (EN_CONFIG[i*2]==addr)
		{
			return EN_CONFIG[i*2+1]&0x00ff;
		}
	}
	return 0xff;
}



//����ָ����ַ���豸״̬
u16 GetDeviceState(u16 addr)
{
	u16 i=0;
	for (i=1;EN_CONFIG[i*2];i++)//�޸���������ַΪ0ʱ��������ֻ�ܿ����ܹص����⣬2018.8.17
	{
		if (EN_CONFIG[i*2]==addr)
		{
			return EN_CONFIG[i*2+1]&0xff00;
		}
	}
	return 0xff;
	
}




//����ָ�����͵��豸����
u16 GetDeviceNum (u8 type)
{
	u16 i=0;
	for (i=0;DEVICE_NUM[i*2+1];i++)
	{
		if (type==(DEVICE_NUM[i*2+1]&0x00ff))
		{
			return DEVICE_NUM[i*2];
		}
	}
	return  0;
}




//���ص�i��type���͵ĵ�ַ
u16 GetNextDeviceaddr (u8 type,u16 i)
{
	u16 t=0;
	u16 a=0;
	for(t=1;EN_CONFIG[t*2+1];t++)
	{
		if ((EN_CONFIG[t*2+1]&0x00ff)==type)
		{
			if (a==i)
			{
				return EN_CONFIG[t*2];
			}a++;
		}
	}
	return 0;
}



//���ñ���IP��ַ
u8 setNativeIP ( u8 * ip)
{
	//������ַ
	Get_MyIP()[0]=ip[0];
	Get_MyIP()[1]=ip[1];
	Get_MyIP()[2]=ip[2];
	Get_MyIP()[3]=ip[3];
	return 0;
}

//���ñ����˿�
u8 setNativePort (u16 port )
{
	Get_MyIP()[4]=port>>8;
	Get_MyIP()[5]=port&0x00ff;
	return 0;
}

//���÷�����ip��ַ
u8 setServerIP (u8 * ip)
{
	//��������ַ
	Get_MyIP()[6]=ip[0];
	Get_MyIP()[7]=ip[1];
	Get_MyIP()[8]=ip[2];
	Get_MyIP()[9]=ip[3];
	return 0;
}


//���÷������˿�
u8 setServerPort (u16 port )
{
	Get_MyIP()[10]=port>>8;
	Get_MyIP()[11]=port&0x00ff;
	return 0;
}


//��������IP��ַ
u8 setGatewayIP (u8 *ip)
{
	//����IP
	Get_MyIP()[12]=192;
	Get_MyIP()[13]=168;
	Get_MyIP()[14]=2;
	Get_MyIP()[15]=1;
	return 0;
}



//��ȡ�������ַ
u16 *GetCfgData (void)
{
	return (&ALL_PARAMETERS[LCD_DATA_NUM]);
}




//��ȡ���������ŵ�
u8 Get_MyChanel (void)
{
	return (EN_CONFIG[CONFIG_DATA_NUM-20-1]&0x00ff);
}


//���ñ����ŵ���0�ɹ���1ʧ��
u8 Set_MyChanel (u8 chanel)
{
	if ((chanel<=31))
	{
		EN_CONFIG[CONFIG_DATA_NUM-20-1]=chanel;
		return 0;
	}
	else
	{
		return 1;
	}
}

//��ȡip��ַ���������
u16 * Get_MyIP (void)
{
		return &EN_CONFIG[CONFIG_DATA_NUM-20];
}


//����ip��ַ
void Set_MyIP (u8 *data)
{
	u16 i=0;
	for (i=0;i<16;i++)
	{
		EN_CONFIG[CONFIG_DATA_NUM-20+i]=data[i];
	}
} 

//�����豸����,
void Set_Device(u8 * data,u16 len)
{
	u16 i=0;
	for (i=1;i<100;i++) {EN_CONFIG[i]=0;}
	for (i=0;i<len;i++)
	{
		EN_CONFIG[2+i*2]=(data[i*3]<<8)|data[i*3+1];
		EN_CONFIG[3+i*2]=data[i*3+2];
	}
}



//��ȡ�ɼ��������׵�ַ
cjq_data *GetCJDataAddr (void)
{ 
	return &CJQ_DATA;
}


//��ȡ������ַ
u16 Get_MyAddr(void)
{
	return EN_CONFIG[0];
}

//���ñ�����ַ
void Set_MyAddr(u16 addr)
{
	EN_CONFIG[0]=addr;
}


//����豸
u8 addDevAddr ( u8 dev,u16 addr )
{
	//0�����ַ��1���߰�λ��״̬���ڰ�λ������
	for (u16 i=1;i<50;i++)
	{
		if (EN_CONFIG[i*2]==addr)
		{
			return 1;//�Ѵ��������ַ���豸
		}
		if (EN_CONFIG[i*2]==0)
		{
			EN_CONFIG[i*2]=addr;
			EN_CONFIG[i*2+1]=0;
			EN_CONFIG[i*2+1]|=0x8000|dev;
			return 0;
		}
	}
	return 1;//�������豸������
}


//�Ƴ��豸��0�ɹ���1ʧ��
u8 delDevAddr (u16 addr)
{
	//0�����ַ��1���߰�λ��״̬���ڰ�λ������
	for (u16 i=1;i<50;i++)
	{
		if (EN_CONFIG[i*2]==addr)
		{
			EN_CONFIG[i*2]=0;						//�Ƴ���ǰ�豸
			EN_CONFIG[i*2+1]=0;						//�Ƴ���ǰ�豸
			EN_CONFIG[i*2+1]|=0x8000;
			for (u16 j=i;j<50-i;j++)			//�Ѻ�����豸�Ƶ�ǰ����
			{
				if (EN_CONFIG[j*2]==0)
				{
					if (EN_CONFIG[j*2+2])		//�������豸
					{
						EN_CONFIG[j*2]=EN_CONFIG[j*2+2];
						EN_CONFIG[j*2+1]=EN_CONFIG[j*2+2+1];
						EN_CONFIG[j*2+2]=0;
						EN_CONFIG[j*2+2+1]=0;
					}
					else		//��ǰ�����һ����
					{
						break;
					}
				}
			}
			return 0;
		}
	}
	return 1;//û�ҵ������ַ���豸

}

//����������õ��豸
void clearDev (void)
{
	for (u16 i=1;i<100;i++)
	{
		EN_CONFIG[i]=0;
	}
}



//�豸���Ͷ���
const objname devicename[]=
{
	[0].objtype=0,[0].name=(u8*)"������",
	[1].objtype=1,[1].name=(u8*)"�ɼ���",
	[2].objtype=2,[2].name=(u8*)"�յ�",
	[3].objtype=3,[3].name=(u8*)"��ʪ��",
	[4].objtype=4,[4].name=(u8*)"������",
	[5].objtype=5,[5].name=(u8*)"��ʪ��",
	[6].objtype=6,[6].name=(u8*)"һ���",
	[7].objtype=7,[7].name=(u8*)"��֧�ֵ�����",
};




//�����豸���ͻ�ȡ����
u8 *dbg_getdevname(u8 devtype)
{
	if (devtype<7)
		return devicename[devtype].name;
	else
		return devicename[7].name;
}


//��ȡ�Զ�����Ƶ�ʣ���λ��
u16 getAutoCtrlFrequency (void)
{
	return EN_CONFIG[CONFIG_DATA_NUM-22];
}


//�����Զ�����Ƶ�ʣ���λ��,0,�ɹ���1��ʧ��
u16 setAutoCtrlFrequency (u16 fre)
{
	EN_CONFIG[CONFIG_DATA_NUM-22]=fre;
	return 0;
}


u16 getAutoCtrlAmount (void)
{
	return EN_CONFIG[CONFIG_DATA_NUM-23];
}


u16 setAutoCtrlAmount (u16 amount )
{
	if (amount<5)
	{
		EN_CONFIG[CONFIG_DATA_NUM-23]=amount;
		return 0;
	}
	else
	{
		return 1;
	}

}

u16 setMyName (char *name)
{
	u8 strlenth=0;
	char *dec;
	strlenth=strlen (name);
	if (strlenth>18)
	{
		return 1;//�ַ�̫��������ʧ��
	}
	else
	{
		dec=(char *)&EN_CONFIG[CONFIG_DATA_NUM-33];
		mymemcpy (dec,name,strlenth+1);
		return 0;
	}
}

char *getMyName (void)
{
	return (char *)&EN_CONFIG[CONFIG_DATA_NUM-33];
}



u16 getDhcpState (void)
{
	return EN_CONFIG[CONFIG_DATA_NUM-34]	;
}

u16 setDhcpState (u16 new_state)
{
	if (new_state)
	{
		EN_CONFIG[CONFIG_DATA_NUM-34]=1;
	}
	else
	{
		EN_CONFIG[CONFIG_DATA_NUM-34]=0;
	}
	return 0;
}


u16 getWarnTolerance (void)
{
	return EN_CONFIG[CONFIG_DATA_NUM-35];//	�Զ����Ʊ����ݲ�ֵ
}

//�����ݲ�ֵ��0�ɹ���1ʧ��
u16 setWarnTolerance (u16 t)
{
	if (t>5)
	{
		return 1;
	}
	else
	{
		EN_CONFIG[CONFIG_DATA_NUM-35]=t;
		return 0;
	}
}




//------------------------���ⲿ���õĺ���--------------------------------


//��ȡ�Ƿ��ֶ���״̬
u16 Lcd_GetHandstate (void)
{
	return HAND_STATE;
}


//�ı��ֶ���״̬
void Lcd_SetHandstate (u16 handstate)
{
	HAND_STATE=handstate;
}



//��ȡ�����޶�ֵ
u16 Lcd_GetLimitData (u16 type)
{
	if (type<5)
	{
		return LCD_DATA[type];
	}
	return 0;
}



u16 Lcd_SetLimitData (u16 type,u16 newlimit)
{
	if (type<5)
	{
		LCD_DATA[type]=newlimit;
	}
	return 0;
}


//��ȡlcd��Ļ��������Ϣ
u16 *getLcdCfgData (void)
{
	return (&ALL_PARAMETERS[0]);
}




