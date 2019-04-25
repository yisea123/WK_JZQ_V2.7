#include "includes.h"
#include "data_map.h"

/************************************

这个文件描述系统参数的存储空间分配
所有需要用到FLASH存储的参数
内存变量在这个文件定义
包括：
		屏幕设置参数
		无线配置参数
		网络配置参数

**************************************/

//定义一个总的存储配置信息的变量数组
static u16 ALL_PARAMETERS[ALL_PARA_LENGTH];


//u16 LCD_DATA[LCD_DATA_NUM]={0};


			//环境变量配置项,集中器地址，采集器地址，控制器地址
			//按照[0]表示设备地址，[1]低8位表示设备类型格式，高8位是设备状态
			//设备状态位【0】表示开关状态，【1,2】位表示升降状态，【3】位表示离线状态,
			//设备地址按照集中器，采集器，控制器的顺序
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
		if (i&1) {EN_CONFIG[i]&=0x00ff;//初始化之后，把设备状态为0，2018.8.18
			if (EN_CONFIG[i]&0x00ff) EN_CONFIG[i]|=DEVICEOFFLINE;//初始化为离线状态，2018.8.22
		}
	}
	EN_CONFIG[1]|=0x0100;//集中器初始化为开启
}
 
 
				//保存手动设置数据位
#define CHUSHI_STATE 			LCD_DATA[14]
#define KONGTIAO_STATE 		LCD_DATA[15]
#define JINHUA_STATE  		LCD_DATA[16]
#define HAND_STATE   			LCD_DATA[17]

						//保存屏幕的数据
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
//	HAND_STATE  =tt[3];注释这一行手动自动模式掉电保存
	
}
 
 


/****************************************************************
*******************具体操作函数
*****************************************************************/

						//设备数量，按照数量，类型的格式
						//这个数组通过初始化函数填充
static u16 DEVICE_NUM[DEVICE_DATA_NUM]={1,1,1,3,1,2,1,4,1,5};
					//采集器的数据
//static u8 EN_DATA_[30]={0};
static cjq_data CJQ_DATA={0};


/***************************************************************

		config配置信息存储结构：
			EN_CONFIG[0]~EN_CONFIG[99] 设备配置信息
			EN_CONFIG[CONFIG_DATA_NUM-36]	是否连接温控服务器
			EN_CONFIG[CONFIG_DATA_NUM-35]	自动控制报警容差值
			EN_CONFIG[CONFIG_DATA_NUM-34]	自动获取IP地址开关
			EN_CONFIG[CONFIG_DATA_NUM-33]~EN_CONFIG[CONFIG_DATA_NUM-24]	本机名称，18个英语字符
			EN_CONFIG[CONFIG_DATA_NUM-23]	自动控制超调量
			EN_CONFIG[CONFIG_DATA_NUM-22]	自动控制频率
			EN_CONFIG[CONFIG_DATA_NUM-21]	无线信道
			EN_CONFIG[CONFIG_DATA_NUM-20]~EN_CONFIG[CONFIG_DATA_NUM-1]	IP地址相关


***************************************************************/

//找出设备数量
void Updata_DeviceNum (void)
{
	u16 i=0;u16 j=0;
	
	for (i=0;i<DEVICE_DATA_NUM;i++)
	{
		DEVICE_NUM[i]=0;
	}
	DEVICE_NUM[1]=1;//采集器
	DEVICE_NUM[3]=2;//空调
	DEVICE_NUM[5]=3;//除湿机
	DEVICE_NUM[7]=4;//净化器
	DEVICE_NUM[9]=5;//加湿机
	DEVICE_NUM[11]=6;//恒湿机
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



//返回指定地址的设备类型
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



//返回指定地址的设备状态
u16 GetDeviceState(u16 addr)
{
	u16 i=0;
	for (i=1;EN_CONFIG[i*2];i++)//修复集中器地址为0时，控制器只能开不能关的问题，2018.8.17
	{
		if (EN_CONFIG[i*2]==addr)
		{
			return EN_CONFIG[i*2+1]&0xff00;
		}
	}
	return 0xff;
	
}




//返回指定类型的设备数量
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




//返回第i个type类型的地址
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



//设置本地IP地址
u8 setNativeIP ( u8 * ip)
{
	//本机地址
	Get_MyIP()[0]=ip[0];
	Get_MyIP()[1]=ip[1];
	Get_MyIP()[2]=ip[2];
	Get_MyIP()[3]=ip[3];
	return 0;
}

//设置本机端口
u8 setNativePort (u16 port )
{
	Get_MyIP()[4]=port>>8;
	Get_MyIP()[5]=port&0x00ff;
	return 0;
}

//设置服务器ip地址
u8 setServerIP (u8 * ip)
{
	//服务器地址
	Get_MyIP()[6]=ip[0];
	Get_MyIP()[7]=ip[1];
	Get_MyIP()[8]=ip[2];
	Get_MyIP()[9]=ip[3];
	return 0;
}


//设置服务器端口
u8 setServerPort (u16 port )
{
	Get_MyIP()[10]=port>>8;
	Get_MyIP()[11]=port&0x00ff;
	return 0;
}


//设置网关IP地址
u8 setGatewayIP (u8 *ip)
{
	//网关IP
	Get_MyIP()[12]=ip[0];
	Get_MyIP()[13]=ip[1];
	Get_MyIP()[14]=ip[2];
	Get_MyIP()[15]=ip[3];
	return 0;
}


//设置子网掩码
u8 setSubMask (u8 *mask)
{
	//网关IP
	Get_MyIP()[16]=mask[0];
	Get_MyIP()[17]=mask[1];
	Get_MyIP()[18]=mask[2];
	Get_MyIP()[19]=mask[3];
	return 0;
}




//获取配置组地址
u16 *GetCfgData (void)
{
	return (&ALL_PARAMETERS[LCD_DATA_NUM]);
}




//获取本机无线信道
u8 Get_MyChanel (void)
{
	return (EN_CONFIG[CONFIG_DATA_NUM-20-1]&0x00ff);
}


//设置本机信道，0成功，1失败
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

//获取ip地址等网络参数
u16 * Get_MyIP (void)
{
		return &EN_CONFIG[CONFIG_DATA_NUM-20];
}


//设置ip地址
void Set_MyIP (u8 *data)
{
	u16 i=0;
	for (i=0;i<16;i++)
	{
		EN_CONFIG[CONFIG_DATA_NUM-20+i]=data[i];
	}
} 

//设置设备参数,
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



//获取采集器数据首地址
cjq_data *GetCJDataAddr (void)
{ 
	return &CJQ_DATA;
}


//获取本机地址
u16 Get_MyAddr(void)
{
	return EN_CONFIG[0];
}

//设置本机地址
void Set_MyAddr(u16 addr)
{
	EN_CONFIG[0]=addr;
}


//添加设备
u8 addDevAddr ( u8 dev,u16 addr )
{
	//0，表地址，1，高八位表状态，第八位表类型
	for (u16 i=1;i<50;i++)
	{
		if (EN_CONFIG[i*2]==addr)
		{
			return 1;//已存在这个地址的设备
		}
		if (EN_CONFIG[i*2]==0)
		{
			EN_CONFIG[i*2]=addr;
			EN_CONFIG[i*2+1]=0;
			EN_CONFIG[i*2+1]|=0x8000|dev;
			return 0;
		}
	}
	return 1;//超出了设备数限制
}


//移除设备，0成功，1失败
u8 delDevAddr (u16 addr)
{
	//0，表地址，1，高八位表状态，第八位表类型
	for (u16 i=1;i<50;i++)
	{
		if (EN_CONFIG[i*2]==addr)
		{
			EN_CONFIG[i*2]=0;						//移除当前设备
			EN_CONFIG[i*2+1]=0;						//移除当前设备
			EN_CONFIG[i*2+1]|=0x8000;
			for (u16 j=i;j<50-i;j++)			//把后面的设备移到前面来
			{
				if (EN_CONFIG[j*2]==0)
				{
					if (EN_CONFIG[j*2+2])		//后面有设备
					{
						EN_CONFIG[j*2]=EN_CONFIG[j*2+2];
						EN_CONFIG[j*2+1]=EN_CONFIG[j*2+2+1];
						EN_CONFIG[j*2+2]=0;
						EN_CONFIG[j*2+2+1]=0;
					}
					else		//当前是最后一个，
					{
						break;
					}
				}
			}
			return 0;
		}
	}
	return 1;//没找到这个地址的设备

}

//清除所有配置的设备
void clearDev (void)
{
	for (u16 i=1;i<100;i++)
	{
		EN_CONFIG[i]=0;
	}
}



//设备类型定义
const objname devicename[]=
{
	[0].objtype=0,[0].name=(u8*)"集中器",
	[1].objtype=1,[1].name=(u8*)"采集器",
	[2].objtype=2,[2].name=(u8*)"空调",
	[3].objtype=3,[3].name=(u8*)"除湿机",
	[4].objtype=4,[4].name=(u8*)"净化机",
	[5].objtype=5,[5].name=(u8*)"加湿机",
	[6].objtype=6,[6].name=(u8*)"一体机",
	[7].objtype=7,[7].name=(u8*)"不支持的类型",
};




//根据设备类型获取名称
u8 *dbg_getdevname(u8 devtype)
{
	if (devtype<7)
		return devicename[devtype].name;
	else
		return devicename[7].name;
}


//获取自动控制频率，单位秒
u16 getAutoCtrlFrequency (void)
{
	return EN_CONFIG[CONFIG_DATA_NUM-22];
}


//设置自动控制频率，单位秒,0,成功，1，失败
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
		return 1;//字符太长，设置失败
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
	return EN_CONFIG[CONFIG_DATA_NUM-35];//	自动控制报警容差值
}

//设置容差值，0成功，1失败
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

u16 getWkServerConnect (void)
{
	return EN_CONFIG[CONFIG_DATA_NUM-36];//	自动控制报警容差值
}

//设置值，0成功，1失败
u16 setWkServerConnect (u16 t)
{
	if (t)
	{
		EN_CONFIG[CONFIG_DATA_NUM-36]=1;
	}
	else
	{
		EN_CONFIG[CONFIG_DATA_NUM-36]=0;
	}
	return 0;
}



//------------------------供外部调用的函数--------------------------------


//获取是否手动的状态
u16 Lcd_GetHandstate (void)
{
	return HAND_STATE;
}


//改变手动的状态
void Lcd_SetHandstate (u16 handstate)
{
	HAND_STATE=handstate;
}



//获取环境限度值
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


//获取lcd屏幕的配置信息
u16 *getLcdCfgData (void)
{
	return (&ALL_PARAMETERS[0]);
}




