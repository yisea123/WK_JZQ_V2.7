#ifndef MY_TOPMSG_H__
#define MY_TOPMSG_H__


#ifdef __cplusplus
	extern "C" {
#endif



		
		
		
		
		

//������Ϣ�ַ�����
void my_topmsg (void *);


/*****************���ⲿ������õ�API********************/
		
//��������ʱ��
u8 addSoftTimerIrq10ms (void (*irq)(void));
		
//�Ƴ�ָ�������ʱ��
u8 delSoftTimerIrq10ms (void (*irq)(void));
		
/***************���ⲿ������õ�API End******************/
		
		
		
		
#ifdef __cplusplus
	}
#endif


#endif











