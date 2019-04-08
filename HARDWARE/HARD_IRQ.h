#ifndef HARD_IRQ_H__
#define HARD_IRQ_H__



#ifdef __cplusplus
	extern "C" {
#endif

		
		
		
		


//��������жϻ��ѵ�ֵ		
#define SYS_MSG_KEY					(0x1)		//���������ж�
#define SYS_MSG_USART2   		(0x2)		//����2�жϣ���LCD��
#define SYS_MSG_USART1			(0x4)		//����1�жϣ���RF
#define SYS_MSG_TIMER				(0x8)		//�����ʱ����10ms
#define SYS_MSG_W5500				(0x10)		//�����ж�

#define TASK_MSG_KEY				(0x1)		//���������ж�
#define TASK_MSG_USART2   	(0x2)		//����2�жϣ���LCD��
#define TASK_MSG_USART1			(0x4)		//����1�жϣ���RF
#define TASK_MSG_TIMER			(0x8)		//�����ʱ����10ms
#define TASK_MSG_W5500			(0x10)		//�����ж�

//����˿���Ϣ����
#define W5500_SOCKET        (0)
#define W5500_SOCKET0				(W5500_SOCKET|0x20)
#define W5500_SOCKET1				(W5500_SOCKET|0x40)
#define W5500_SOCKET2				(W5500_SOCKET|0x80)
#define W5500_SOCKET3				(W5500_SOCKET|0x100)
#define W5500_SOCKET4				(W5500_SOCKET|0x200)
#define W5500_SOCKET5				(W5500_SOCKET|0x400)
#define W5500_SOCKET6				(W5500_SOCKET|0x800)
#define W5500_SOCKET7				(W5500_SOCKET|0x1000)


void USART1_SetFocus(u8 focus);
void USART2_SetFocus(u8 focus);

u8 USART1_GetFocus(void);
u8 USART2_GetFocus(void);

void SOCKET0_SetFocus (u8 focus);
void SOCKET1_SetFocus (u8 focus);
void SOCKET2_SetFocus (u8 focus);
void SOCKET3_SetFocus (u8 focus);
void SOCKET4_SetFocus (u8 focus);
void SOCKET5_SetFocus (u8 focus);
void SOCKET6_SetFocus (u8 focus);
void SOCKET7_SetFocus (u8 focus);


u8 SOCKET0_GetFocus (void);
u8 SOCKET1_GetFocus (void);
u8 SOCKET2_GetFocus (void);
u8 SOCKET3_GetFocus (void);
u8 SOCKET4_GetFocus (void);
u8 SOCKET5_GetFocus (void);
u8 SOCKET6_GetFocus (void);
u8 SOCKET7_GetFocus (void);





		
#ifdef __cplusplus
	}
#endif


#endif







