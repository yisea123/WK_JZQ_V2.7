#ifndef __KEY_H
#define __KEY_H


#ifdef  __cplusplus
	extern "C" {
#endif





//���������̰�����
#define PRESS_SHORT 1//�̰�����
#define PRESS_LONG 2	//�ﵽ����
#define PRESS_DOWN 3//����
#define PRESS_NONE 0




void Key_Init (void);


u8 Get_Key(u8 keynum);
		
//���ð���״̬���������ⰴ��,0,�ɹ���1,ʧ��
u8 Set_Key(u8 keynum,u8 action);

void KEY_IRQHandler(void) ;





#ifdef  __cplusplus
	}
#endif







#endif




