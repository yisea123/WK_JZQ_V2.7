#include "includes.h"
#include "swd.h"


#define SWD_CLK 			PAout(14)
//#define SWD_CLK 			PDout(2)
#define SWD_DIO_OUT 	PAout(13)
#define SWD_DIO_IN		PAin(13)

//用寄存器方式设置PA2为上拉输入模式
#define SWD_DIN() 	{u32 temp=GPIOA->CRH&0xff0fffff;\
											GPIOA->CRH=temp|(0x8<<(4*(13-8)));\
											GPIOA->BSRR=1<<13;}

//用寄存器方式设置PA2为推挽输出模式
#define SWD_DOUT() 	{u32 temp=GPIOA->CRH&0xff0fffff;\
											GPIOA->CRH=temp|(0x3<<(4*(13-8)));}

				
											

void SWD_Init (void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_AFIO, ENABLE );//PORTB时钟使能 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	
	//使用GPIO来模拟SWD接口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14; //PA2，PA3
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA2，PA3
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
	SWD_CLK=0;
	SWD_DIO_OUT=1;
}




//低位在前，高位在后
//上升沿刷新，下降沿锁存
void SWD_SendByte (u8 byte)
{
	SWD_DOUT();
	for (u8 i=0;i<8;i++)
	{
		SWD_DIO_OUT=byte>>i;
		SWD_CLK=1;
		SWD_CLK=0;
	}
}


u8 SWD_RecvByte (void)
{
	u8 byte=0;
	SWD_DIN();
	for (u8 i=0;i<8;i++)
	{
		byte>>=1;
		byte|=(SWD_DIO_IN<<7);
		SWD_CLK=1;
		SWD_CLK=0;
	}
	return byte;
}


void SWD_Turn (void)
{
//	SWD_DOUT();
//	SWD_DIO_OUT=0;
	SWD_CLK=1;
	SWD_CLK=0;
}


u8 SWD_Ask (void)
{
	u8 ask=0;
	SWD_DIN();
	for (u8 i=0;i<3;i++)
	{
		ask>>=1;
		ask|=(SWD_DIO_IN<<2);
		SWD_CLK=1;
		SWD_CLK=0;
	}
	return ask;
}


u8 SWD_Praity (void)
{
	u8 par=0;
	SWD_DIN();
	par=SWD_DIO_IN;
	SWD_CLK=1;
	SWD_CLK=0;
	return par;
}

void SWD_SendPraity (u8 pra)
{
	SWD_DOUT();
	SWD_DIO_OUT=pra;
	SWD_CLK=1;
	SWD_CLK=0;

}


void SWD_LineReset (void)
{
	for (u8 i=0;i<7;i++)
	{
		SWD_SendByte(0xff);
	}
	SWD_SendByte(0x9e);//低字节在前
	SWD_SendByte(0xe7);
	for (u8 i=0;i<7;i++)
	{
		SWD_SendByte(0xff);
	}
	SWD_SendByte(0xb6);//低字节在前
	SWD_SendByte(0xed);
	for (u8 i=0;i<7;i++)
	{
		SWD_SendByte(0xff);
	}
	SWD_SendByte(0x00);//低字节在前
	SWD_SendByte(0x00);

}


u32 SWD_ReadReg (u8 cmd,u8 *err)
{
	cmd|=0x81|SWD_READ; 
	u8 _1num=0;
	for (u8 i=0;i<4;i++)
	{
		if (cmd&(1<<(i+1)))
		{
			_1num++;
		}
	}
	if (_1num%2) cmd|=1<<5;
	
	
	u8 ask=0;
	do 
	{
		SWD_SendByte(cmd);
		SWD_Turn();
		ask=SWD_Ask();
	}while (ask==SWD_ERR_WAIT);
	*err=ask;
	
	u32 data=0;
	u8 praity=0;
	data|=SWD_RecvByte();
	data|=SWD_RecvByte()<<8;
	data|=SWD_RecvByte()<<16;
	data|=SWD_RecvByte()<<24;
	praity=SWD_Praity();
	SWD_SendByte(0);
	//SWD_Turn();
	_1num=0;
	for (u8 i=0;i<32;i++)
	{
		if (data&(1<<i))
		{
			_1num++;
		}
	}
	//delay_us(170);
	if ((_1num%2)==(praity&1))//校验成功
	{
		return data;
	}
	else 
	{
		*err=0x07;
		return 0;
	}	

}


void SWD_WriteReg (u8 cmd,u8 *err,u32 data)
{
	cmd|=0x81|SWD_WRITE;
	u8 _1num=0;
	for (u8 i=0;i<4;i++)
	{
		if (cmd&(1<<(i+1)))
		{
			_1num++;
		}
	}
	if (_1num%2) cmd|=1<<5;
	
	u8 ask=0;
	do 
	{
		SWD_SendByte(cmd);		
		SWD_Turn();
		ask=SWD_Ask();
		SWD_Turn();
	}while (ask==SWD_ERR_WAIT);
	*err=ask;
	
	
	_1num=0;
	for (u8 i=0;i<32;i++)
	{
		if (data&(1<<i))
		{
			_1num++;
		}
	}
	SWD_SendByte(data&0xff);
	SWD_SendByte(data>>8);
	SWD_SendByte(data>>16);
	SWD_SendByte(data>>24);
	//SWD_SendPraity(_1num%2);
	SWD_SendByte(_1num%2);
	//delay_us(170);

}



//向内存写入数据,成功返回设备id
u32 SWD_WriteSram (u32 addr,u32 *data,u32 len)
{
	u8 err=0;
	u32 id=0;
	//SWD_LineReset();
	id=SWD_ReadReg(SWD_REG_DP|SWD_REG_IDCODE,&err);
	if (err!=SWD_ERR_OK) return 0;
	SWD_WriteReg(SWD_REG_DP|SWD_REG_ABORT,&err,0x1e);
	if (err!=SWD_ERR_OK) return 0;
	SWD_WriteReg(SWD_REG_DP|SWD_REG_CTRL_STAT,&err,0x50000000);
	if (err!=SWD_ERR_OK) return 0;
	SWD_WriteReg(SWD_REG_DP|SWD_REG_SELECT,&err,0x00);
	if (err!=SWD_ERR_OK) return 0;
	SWD_WriteReg(SWD_REG_AP|SWD_REG_CSW,&err,0x23000012);//地址自动递增
	if (err!=SWD_ERR_OK) return 0;
	SWD_WriteReg(SWD_REG_AP|SWD_REG_TAR,&err,addr);//在这个地址的内存空间写数据
	if (err!=SWD_ERR_OK) return 0;
	while (len--)
	{
		SWD_WriteReg(SWD_REG_AP|SWD_REG_DRW,&err,*data);
		if (err!=SWD_ERR_OK) return 0;
		data++;
	}
	return id;
}



//向内存读取数据,成功返回设备id
u32 SWD_ReadSram (u32 addr,u32 *data,u32 len)
{
	u8 err=0;
	u32 id=0;
	//SWD_LineReset();
	id=SWD_ReadReg(SWD_REG_DP|SWD_REG_IDCODE,&err);
	if (err!=SWD_ERR_OK) return 0;
	SWD_WriteReg(SWD_REG_DP|SWD_REG_ABORT,&err,0x1e);
	if (err!=SWD_ERR_OK) return 0;
	SWD_WriteReg(SWD_REG_DP|SWD_REG_CTRL_STAT,&err,0x50000000);
	if (err!=SWD_ERR_OK) return 0;
	SWD_WriteReg(SWD_REG_DP|SWD_REG_SELECT,&err,0x00);
	if (err!=SWD_ERR_OK) return 0;
	SWD_WriteReg(SWD_REG_AP|SWD_REG_CSW,&err,0x23000012);//地址自动递增
	if (err!=SWD_ERR_OK) return 0;
	SWD_WriteReg(SWD_REG_AP|SWD_REG_TAR,&err,addr);//在这个地址的内存空间数据
	if (err!=SWD_ERR_OK) return 0;
	*data=SWD_ReadReg(SWD_REG_AP|SWD_REG_DRW,&err);//读取一次无效数据
	if (err!=SWD_ERR_OK) return 0;

	while (len--)
	{
		*data=SWD_ReadReg(SWD_REG_AP|SWD_REG_DRW,&err);
		if (err!=SWD_ERR_OK) return 0;
		data++;
	}
	return id;
}





//暂停或者启动内核1，暂停，0，启动
u32 SWD_Cm3Halt (u8 value)
{
	u8 err=0;
	u32 id=0;
	//SWD_LineReset();
	id=SWD_ReadReg(SWD_REG_DP|SWD_REG_IDCODE,&err);
	if (err!=SWD_ERR_OK) return 0;
	SWD_WriteReg(SWD_REG_DP|SWD_REG_ABORT,&err,0x1e);
	if (err!=SWD_ERR_OK) return 0;
	SWD_WriteReg(SWD_REG_DP|SWD_REG_CTRL_STAT,&err,0x50000000);
	if (err!=SWD_ERR_OK) return 0;
	SWD_WriteReg(SWD_REG_DP|SWD_REG_SELECT,&err,0x00);
	if (err!=SWD_ERR_OK) return 0;
	SWD_WriteReg(SWD_REG_AP|SWD_REG_CSW,&err,0x23000002);
	if (err!=SWD_ERR_OK) return 0;
	SWD_WriteReg(SWD_REG_AP|SWD_REG_TAR,&err,CM3_DHCSR);//暂停CM3内核
	if (err!=SWD_ERR_OK) return 0;
	if (value)
	{
		SWD_WriteReg(SWD_REG_AP|SWD_REG_DRW,&err,0xa05f0001);//写入钥匙,使能调试
		if (err!=SWD_ERR_OK) return 0;
		SWD_WriteReg(SWD_REG_AP|SWD_REG_TAR,&err,CM3_DEMCR);//使能复位后停止
		if (err!=SWD_ERR_OK) return 0;
		SWD_WriteReg(SWD_REG_AP|SWD_REG_DRW,&err,0x1);//
		if (err!=SWD_ERR_OK) return 0;
		SWD_WriteReg(SWD_REG_AP|SWD_REG_TAR,&err,CM3_AIRCR);//复位
		if (err!=SWD_ERR_OK) return 0;
		SWD_WriteReg(SWD_REG_AP|SWD_REG_DRW,&err,0x05fa0004);//
		if (err!=SWD_ERR_OK) return 0;
	}
	else
	{
		SWD_WriteReg(SWD_REG_AP|SWD_REG_DRW,&err,0xa05f0000);//写入钥匙,使能调试
		if (err!=SWD_ERR_OK) return 0;
//		SWD_WriteReg(SWD_REG_AP|SWD_REG_TAR,&err,CM3_DEMCR);//使能复位后停止
//		if (err!=SWD_ERR_OK) return 0;
//		SWD_WriteReg(SWD_REG_AP|SWD_REG_DRW,&err,0x0);//
//		if (err!=SWD_ERR_OK) return 0;
//		SWD_WriteReg(SWD_REG_AP|SWD_REG_TAR,&err,CM3_AIRCR);//复位
//		if (err!=SWD_ERR_OK) return 0;
//		SWD_WriteReg(SWD_REG_AP|SWD_REG_DRW,&err,0x05fa0004);//
//		if (err!=SWD_ERR_OK) return 0;
	}
	return id;
}





//向内核寄存器写入数据
u32 SWD_WriteCm3Reg (u16 reg_select,u32 data)
{
	u8 err=0;
	u32 id=0;
	//SWD_LineReset();
	id=SWD_ReadReg(SWD_REG_DP|SWD_REG_IDCODE,&err);
	if (err!=SWD_ERR_OK) return 0;
	SWD_WriteReg(SWD_REG_DP|SWD_REG_ABORT,&err,0x1e);
	if (err!=SWD_ERR_OK) return 0;
	SWD_WriteReg(SWD_REG_DP|SWD_REG_CTRL_STAT,&err,0x50000000);
	if (err!=SWD_ERR_OK) return 0;
	SWD_WriteReg(SWD_REG_DP|SWD_REG_SELECT,&err,0x00);
	if (err!=SWD_ERR_OK) return 0;
	SWD_WriteReg(SWD_REG_AP|SWD_REG_CSW,&err,0x23000002);
	if (err!=SWD_ERR_OK) return 0;
	SWD_WriteReg(SWD_REG_AP|SWD_REG_TAR,&err,CM3_DCRDR);//内核数据寄存器
	if (err!=SWD_ERR_OK) return 0;
	SWD_WriteReg(SWD_REG_AP|SWD_REG_DRW,&err,data);
	if (err!=SWD_ERR_OK) return 0;
	SWD_WriteReg(SWD_REG_AP|SWD_REG_TAR,&err,CM3_DCRSR);//选择内核寄存器
	if (err!=SWD_ERR_OK) return 0;
	SWD_WriteReg(SWD_REG_AP|SWD_REG_DRW,&err,reg_select|CM3_REG_WRITE);
	if (err!=SWD_ERR_OK) return 0;
	
	u32 dhcsr_data=0;
	SWD_WriteReg(SWD_REG_AP|SWD_REG_TAR,&err,CM3_DHCSR);//
	if (err!=SWD_ERR_OK) return 0;
	dhcsr_data=SWD_ReadReg(SWD_REG_AP|SWD_REG_DRW,&err);
	if (err!=SWD_ERR_OK) return 0;
	do
	{
		dhcsr_data=SWD_ReadReg(SWD_REG_AP|SWD_REG_DRW,&err);
		if (err!=SWD_ERR_OK) return 0;
	}while ((dhcsr_data&0x00010000)==0);
	return id;
}






