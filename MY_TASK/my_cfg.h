
#ifndef __MY_CFG_H
#define __MY_CFG_H


#ifdef __cplusplus
	extern "C" {
#endif


//���ù���������
void my_cfg (void * t);

//��������ģʽ
void into_cfg(void);
		
//�˳�����ģʽ
void exit_cfg(void);

//��������ģʽ����Ϣ
void cfg_msg (u8 *msg);
		
//��������
void rf_cfg (u8 *rf_recv);
		
u8 cfg_getstate (void);

		
#ifdef __cplusplus
	}
#endif
		
		

#endif









