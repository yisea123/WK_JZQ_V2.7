#include "includes.h"
#include "power.h"
#include "cmd.h"



static u8 	*data_buff=0;
static u32 	flash_addr=0;//要操作的flash地址
static u16	data_offset=0;		//缓冲池偏移量
static u8 	in_iap=0;			//是否是iap模式


//这些是flash操作时的变量
static u16 data_num=0;//当前数据包
static u16 data_all=0;//总数据包


//字节数一定要是偶数
void cmd_flash_read(u8 *buff,u32 addr,u16 length)
{
	STMFLASH_Read(0x08000000+addr+1024*120,(u16 *)buff,length/2);
}
void cmd_flash_write(u8 *data,u32 addr,u16 length)
{
	STMFLASH_Write(0x08000000+addr+1024*120,(u16 *)data,length/2);
}



void (*p_Flash_Write)(u8 *data,u32 addr,u16 length);
void (*p_Flash_Read)(u8 *buff,u32 addr,u16 length);




static void (*cmd_dataSend) (u8 *data,u16 datalen);

//获取命令
void Get_cmd (void (*dataSend) (u8 *data,u16 datalen),u8 *buff)
{
	u16 length=0;
	u8 crc[2]={0};
	u8 is_cmd=0;
	cmd_dataSend=dataSend;
	if (buff[0]==0xff&&buff[1]==0xff)
	{
		length=((buff[5]<<8)|buff[6])+7;
		if (length>56) return;
		Get_Crc16(buff,length,crc);
		if (buff[length]==crc[0]&&buff[length+1]==crc[1])
		{
			is_cmd=1;
		}
	}
	
	if (is_cmd)
	{
		switch(buff[4])
		{
			case 0x05:
				cmd_0x05 (buff);
				break;
			default:
				cmd_return(buff,ERR_NULLCMD);
				break; 
		}
	}

}





void cmd_0x05 (u8 *buff)
{
	switch (buff[7])
	{
		case 0x00://读
			read_flash(buff);
			break;
		case 0x01://写
			write_flash(buff);
			break;
		case 0x02://写进行中
			break;
		case 0x03:
			writting_flash(buff);
			break;
		default:
			cmd_return(buff,ERR_DATAVALE);
			break;
	}
}






void write_flash (u8 *buff)
{
	u32 size=0;
	u32 stm32addr=0;
	size=(buff[8]<<24)|(buff[9]<<16)|(buff[10]<<8)|(buff[11]);
	stm32addr=(buff[12]<<24)|(buff[13]<<16)|(buff[14]<<8)|buff[15];
	
	if ((SPI_FLASH_TYPE&0xff00)!=0xef00)//本机没有检测到flash，不能iap升级
	{
		//cmd_return(buff,ERR_FLASHADDR);
		//return ;
		p_Flash_Read=cmd_flash_read;
		p_Flash_Write=cmd_flash_write;
	}
	else 
	{
		p_Flash_Write=SPI_Flash_Write;
		p_Flash_Read=SPI_Flash_Read;
	}
	
	if (stm32addr<0x08002800)//写程序空间不能重写IAP所在的程序段
	{
		cmd_return(buff,ERR_FLASHADDR);
		return ;
	}
	else
	{
		flash_addr=4096;
	}
	if (size>0x3d800)//确保数据都在在可操作地址范围内2018.10.8
	{
		cmd_return(buff,ERR_DATALENGTH);
		return ;
	}
	else
	{
		IapInfoDef iapinfo={0};							//更新iap信息
		p_Flash_Read ((u8*)&iapinfo,0,sizeof (iapinfo));
		iapinfo.appFlashAddr=flash_addr;
		iapinfo.appStm32Addr=stm32addr;
		iapinfo.appSize=size;
		p_Flash_Write ((u8*)&iapinfo,0,sizeof (iapinfo));
	}
	data_num=0;
	data_all=0;
	data_offset=0;
	in_iap=1;//进入iap模式
	if (data_buff==0) data_buff=mymalloc(2048);//申请内存
	for (u16 i=0;i<2048;i++){data_buff[i]=0;}
	cmd_return(buff,ERR_SUCCESS);
}





void read_flash (u8 *buff)
{
}


			//返回是否有数据，1，有数据
u8 data_real(void)
{
	u16 i=0;
	for (i=0;i<2048;i++)
	{
		if (data_buff[i]!=0)
		{
			return 1;
		}
	}
	return 0;
}


u8 cheeckflash (u8 *data1,u8 *data2,u16 size)
{
	while (size)
	{
		if (*data1!=*data2)
		{
			break;
		}
		data1++;
		data2++;
		size--;
	}
	if (size==0) return 0;//通过
	else return 1;//校验没通过
}



void writting_flash (u8 *buff)
{
	u16 i_num=0;
	u16 datalength=0;
	if (in_iap==1)
	{
		data_num++;
		i_num=(buff[8]<<8)|buff[9];
		data_all=(buff[10]<<8)|buff[11];
		if (i_num>data_num)
		{
			cmd_return(buff,ERR_FLASHWRITE);
			return;
		}
		if (i_num<data_num)
		{
			cmd_return(buff,ERR_SUCCESS);//主机没有收到上次的成功返回，补发一次
			data_num--;
			return;
		}
		datalength=((buff[5]<<8)|buff[6])-5;
		for (u16 i=0;i<datalength;i++)
		{
			data_buff[data_offset]=buff[12+i];
			data_offset++;
			if (data_offset>=2048)
			{
				data_offset=0;
				u8 checktry=10;
				u8 *readbuff=mymalloc (2048);
				do 
				{
					p_Flash_Write(data_buff,flash_addr,2048);
					p_Flash_Read(readbuff,flash_addr,2048);
					checktry--;
					if (checktry==0) SysPowerOff();
				}while (cheeckflash (readbuff,data_buff,2048));
				myfree(readbuff);
				flash_addr+=2048;
				for (u16 j=0;j<2048;j++){data_buff[j]=0;}
			}
		}
		if (data_num==data_all)//最后一个数据包
		{
			if (data_real()) 
			{
				p_Flash_Write(data_buff,flash_addr,data_offset);
				data_offset=0;
				flash_addr=0;
				for (u16 j=0;j<2048;j++){data_buff[j]=0;}
			}

			in_iap=0;//退出iap
			myfree(data_buff);
		}
		cmd_return(buff,ERR_SUCCESS);
		if (in_iap==0)
		{
			delay_ms(50);//防止数据发送出去之前就跳转到APP
			IAP_CMD=IAP_CMD_FLASH;
			SysPowerOff();
		}
	}
	else
	{
		cmd_return(buff,ERR_NOCONNECT);
	}
}


void cmd_return (u8 * buff,u16 err)
{
	u8 data[20]={0};
	u8 crc[2]={0};
	data[0]=0xff;
	data[1]=0xff;
	data[2]=buff[2];//地址和主机发送来的地址一致
	data[3]=buff[3];
	data[4]=0x80|buff[4];
	data[5]=0x00;
	data[6]=0x02;
	data[7]=err>>8;
	data[8]=err;
	Get_Crc16(data,(data[6])+7,crc);
	data[(data[6])+7]=crc[0];
	data[(data[6])+7+1]=crc[1];
	cmd_dataSend(data,(data[6])+7+2);
}








