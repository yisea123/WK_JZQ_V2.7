
#ifndef __DATA_MAP_H
#define __DATA_MAP_H



#ifdef __cplusplus
	extern "C" {
#endif





//������Ļ���õ����ݳ���
#define LCD_DATA_NUM 20

//�����������ݳ���
#define CONFIG_DATA_NUM 200

//���ò����ܳ���
#define ALL_PARA_LENGTH (LCD_DATA_NUM+CONFIG_DATA_NUM)

//������Ļ������
#define LCD_DATA getLcdCfgData()

//������������
#define EN_CONFIG  GetCfgData()



//����ת��float�ͱ���
typedef union
{
	float f;
	u8 u[4];
} f_to_u;

//�ɼ�����������
typedef struct
{
	u16 cjqId;
	float temp;		//��λ��
	float humi;		//��λ��
	float tvoc;		//��λppm
	float co2;		//��λppm
	float pm2_5;	//��λmg/m3
} cjq_data;










//�Ҽӵ����ڴ洢������Ϣ�ĺ���
void Save_Config (void);
void Load_Config(void);

//������Ļ������
void Save_LCD (void);
void Load_LCD(void);



enum {
	devTypeJzq=0,
	devTypeCjq=1,
	devTypeKt=2,
	devTypeCsj=3,
	devTypeJhq=4,
	devTypeJsj=5,
	devTypeYtj=6,
};





/****************************************************************
*******************�����������
*****************************************************************/

#define DEVICE_DATA_NUM 20

#define EN_DATA GetCJDataAddr()

//�ҳ��豸����
void Updata_DeviceNum (void);

//����ָ����ַ���豸����
u8 GetDeviceType (u16 addr);

//����ָ����ַ���豸״̬
u16 GetDeviceState(u16 addr);

//����ָ�����͵��豸����
u16 GetDeviceNum (u8 type);

//���ص�i��type���͵ĵ�ַ
u16 GetNextDeviceaddr (u8 type,u16 i);

//��ȡ������Ϣ���׵�ַ
u16 *GetCfgData (void);


//------------------------���ⲿ���õĺ���---------------------------


//��ȡ�ɼ��������׵�ַ
cjq_data *GetCJDataAddr (void);

//��ȡ������ַ
u16 Get_MyAddr(void);

//���ñ�����ַ
void Set_MyAddr(u16 addr);

//�����豸��������ʽ�ǰ����豸��ַ��8λ����8λ������,len,�豸����
void Set_Device(u8 * data,u16 len);

//����ip��ַ
void Set_MyIP (u8 *data);

//��ȡip��ַ
u16 * Get_MyIP (void);

//���ñ��������ŵ�
u8 Set_MyChanel (u8 chanel);

//��ȡ���������ŵ�
u8 Get_MyChanel (void);


//���ñ���IP��ַ
u8 setNativeIP ( u8 * ip);

//���ñ����˿�
u8 setNativePort (u16 port );

//���÷�����ip��ַ
u8 setServerIP (u8 * ip);

//���÷������˿�
u8 setServerPort (u16 port );

//��������IP��ַ
u8 setGatewayIP (u8 *ip);

//����豸0,�ɹ���1��ʧ��
u8 addDevAddr ( u8 dev,u16 addr );

//�Ƴ��豸��0�ɹ���1ʧ��
u8 delDevAddr (u16 addr);

//����������õ��豸
void clearDev (void);



//��ȡ�Զ�����Ƶ�ʣ���λ��
u16 getAutoCtrlFrequency (void);

//�����Զ�����Ƶ�ʣ���λ��,0,�ɹ���1��ʧ��
u16 setAutoCtrlFrequency (u16 fre);

//ȡ�ó�����
u16 getAutoCtrlAmount (void);

//���ó�����,0,�ɹ���1��ʧ��
u16 setAutoCtrlAmount (u16 amount );

//���ñ�������
u16 setMyName (char *name);

//��ȡ��������
char *getMyName (void);

//��ȡDHCP״̬
u16 getDhcpState (void);

//����DHCP״̬,0,�ɹ�
u16 setDhcpState (u16 new_state);

//�����ݲ�ֵ��0�ɹ���1ʧ��
u16 setWarnTolerance (u16 t);

u16 getWarnTolerance (void);



//------------------------���ⲿ���õĺ���End---------------------------

//�������ͻ�ȡ����
typedef struct 
{
	u8 objtype;
	u8 *name;
}objname;



//�����豸���ͻ�ȡ����
u8 *dbg_getdevname(u8 devtype);


//----------------------���ⲿ���õĺ���-------------------

//��ȡ�Ƿ��ֶ���״̬
u16 Lcd_GetHandstate (void);

//��ȡ����ֵ���޶ȣ�0���¶����ޣ�1���¶����ޣ�2��ʪ�����ޣ�3��ʪ�����ޣ�4��tvoc����
u16 Lcd_GetLimitData (u16 type);

//���û���ֵ�޶�
u16 Lcd_SetLimitData (u16 type,u16 newlimit);

//�ı��ֶ���״̬
void Lcd_SetHandstate (u16 handstate);

//��ȡlcd��Ļ��������Ϣ
u16 *getLcdCfgData (void);

//---------------------���ⲿ���õĺ���End-------------------






#ifdef __cplusplus
	}
#endif



#endif






