
#ifndef CMD_H_
#define CMD_H_

#ifdef __cplusplus
	extern "C" {
#endif



		
		
		
//模式2中的错误类型定义

#define ERR_SUCCESS         0x0000			//成功
#define ERR_FAIL						0x0001			//未知错误
#define ERR_DATAVALE				0x0002			//数据错误
#define ERR_DATALENGTH			0x0003			//数据长度错误
#define ERR_WANTLENGTH			0x0004			//想要读取的数据长度错误
#define ERR_NULLCMD					0x0005			//不支持的命令类型，只支持06,03
#define ERR_NOCONNECT				0x0006			//没有连接，一切操作都要在通信开始之后
#define ERR_NOCJADDR				0x0007			//采集器的地址不是本机绑定的采集器地址
#define ERR_CANNOTCFG				0x0008			//在不是可配置状态下收到了配置命令
#define ERR_NOTMYTYPE				0x0009			//所要控制的设备与本机不符
#define ERR_TIMEOUT					0x000a  		//超时
#define ERR_FLASHADDR				0x000b			//FLASH操作时地址出错
#define ERR_FLASHWRITE			0x000c			//FLASH写入时包出错

		
		
	//定义IAP操作
#define IAP_CMD (*(u32 *)0x20000000)

#define IAP_CMD_REBOOT			0xf0000000
#define IAP_CMD_RF 					0xf0000001
#define IAP_CMD_SDCARD			0xf0000002
#define IAP_CMD_INTERNET		0xf0000003
#define IAP_CMD_FLASH				0xf0000004

	

//定义IAP程序相关信息的宏

#define IAP_Support 			*(u32 *)(0x08000000+28)
#define IAP_Version 			(char *)*(u32 *)(0x08000000+32)
#define IAP_CompileTime 	(char *)*(u32 *)(0x08000000+36)



		
//flash存储映像信息		
typedef struct 
{
	u32 appFlashAddr;
	u32 appStm32Addr;
	u32 appSize;
	u32 fatAddr;
	u32 fatStm32Addr;
	u32 fatSize;
	u32 dataAddr;
	u32 dataStm32Addr;
	u32 dataSize;
} IapInfoDef;
		
		
		
		
		
		
		
		


//获取命令
void Get_cmd (void (*dataSend) (u8 *data,u16 datalen),u8 *buff);

void cmd_0x05 (u8 *buff);

void write_flash (u8 *buff);

void cmd_return (u8 * buff,u16 err);
		
void writting_flash (u8 *buff);
		
//返回是否有数据，1，有数据
u8 data_real(void);
		
void read_flash (u8 *buff);
		
void write_flash (u8 *buff);











#ifdef __cplusplus
	}
#endif



#endif 







