
#ifndef __TIMER_H
#define __TIMER_H



#ifdef  __cplusplus
	extern "C" {
#endif




//��ʼ����ʱ��
void TimersInit (void);

//���10ms�Ķ�ʱ���жϷ�����
u8 addTimerIrq10ms (void (*irq)(void));

//���10ms��ʱ���ն˷�����
u8 delTimerIrq10ms (void (*irq)(void));

//���10us��ʱ���жϷ�����
u8 addTimerIrq10us (void (*irq)(void));

//���10us��ʱ���ն˷�����
u8 delTimerIrq10us (void (*irq)(void));



#ifdef  __cplusplus
	}
#endif


#endif



