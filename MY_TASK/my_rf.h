#ifndef __MY_RF_H
#define __MY_RF_H


/****************************************************************

		EN_CONFIG���20λ�洢IP��ַ�������



*****************************************************************/







#ifdef __cplusplus
	extern "C" {
#endif








//������Ϣѭ��
void my_rf_loop (void * t);

//���ߵ����ݴ���
void my_rf_deal (void * t);

//�����ֶ���Ϣ
void my_rf_hand (void * t);



//��ȡ�豸��Ϣ��recv�ǽ�������
u16 Cmd_0x01 (u16 addr ,u8 *recv);

//�����豸
u16 Cmd_0x03 (u16 addr ,u8 type,u8 power,u8 state);

//�����˳�����ģʽ
u16 Cmd_0x06 (u16 addr ,u8 state);

//���û�������
u16 Cmd_0x07 (u16 addr ,u8 type);








void loushui_warn(u16 addr,u8 devtype);

void rf_cjq_deal(u8 *data);


u16 CheckId (u16 addr);//�ж��Ƿ��д��豸id


					//�豸����״ָ̬ʾ�����ͣ���������״̬��������
void Device_state_see(u8 type,u8 power,u8 state);

						//��ȡ�µĿ����豸��״̬
u16 get_newstate(u8 nowpower,u8 key,u16 addr);
							//��ȡһ�������״̬
u16 yt_newstate(u8 nowpower,u8 key,u16 addr);

									//���¼������м�¼���豸״̬
u16 Updata_devicestate(u16 addr ,u8 power,u8 state);





#ifdef __cplusplus
	}
#endif





#endif


