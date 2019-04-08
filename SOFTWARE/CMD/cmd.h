
#ifndef CMD_H_
#define CMD_H_

#ifdef __cplusplus
	extern "C" {
#endif



		
		
		
//ģʽ2�еĴ������Ͷ���

#define ERR_SUCCESS         0x0000			//�ɹ�
#define ERR_FAIL						0x0001			//δ֪����
#define ERR_DATAVALE				0x0002			//���ݴ���
#define ERR_DATALENGTH			0x0003			//���ݳ��ȴ���
#define ERR_WANTLENGTH			0x0004			//��Ҫ��ȡ�����ݳ��ȴ���
#define ERR_NULLCMD					0x0005			//��֧�ֵ��������ͣ�ֻ֧��06,03
#define ERR_NOCONNECT				0x0006			//û�����ӣ�һ�в�����Ҫ��ͨ�ſ�ʼ֮��
#define ERR_NOCJADDR				0x0007			//�ɼ����ĵ�ַ���Ǳ����󶨵Ĳɼ�����ַ
#define ERR_CANNOTCFG				0x0008			//�ڲ��ǿ�����״̬���յ�����������
#define ERR_NOTMYTYPE				0x0009			//��Ҫ���Ƶ��豸�뱾������
#define ERR_TIMEOUT					0x000a  		//��ʱ
#define ERR_FLASHADDR				0x000b			//FLASH����ʱ��ַ����
#define ERR_FLASHWRITE			0x000c			//FLASHд��ʱ������

		
		
	//����IAP����
#define IAP_CMD (*(u32 *)0x20000000)

#define IAP_CMD_REBOOT			0xf0000000
#define IAP_CMD_RF 					0xf0000001
#define IAP_CMD_SDCARD			0xf0000002
#define IAP_CMD_INTERNET		0xf0000003
#define IAP_CMD_FLASH				0xf0000004

	

//����IAP���������Ϣ�ĺ�

#define IAP_Support 			*(u32 *)(0x08000000+28)
#define IAP_Version 			(char *)*(u32 *)(0x08000000+32)
#define IAP_CompileTime 	(char *)*(u32 *)(0x08000000+36)



		
//flash�洢ӳ����Ϣ		
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
		
		
		
		
		
		
		
		


//��ȡ����
void Get_cmd (void (*dataSend) (u8 *data,u16 datalen),u8 *buff);

void cmd_0x05 (u8 *buff);

void write_flash (u8 *buff);

void cmd_return (u8 * buff,u16 err);
		
void writting_flash (u8 *buff);
		
//�����Ƿ������ݣ�1��������
u8 data_real(void);
		
void read_flash (u8 *buff);
		
void write_flash (u8 *buff);











#ifdef __cplusplus
	}
#endif



#endif 







