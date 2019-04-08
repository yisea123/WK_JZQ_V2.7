
#ifndef __OS_TASK_H
#define __OS_TASK_H

#ifdef __cplusplus
	extern "C" {
#endif



/************************************************

				����������֧��32������
				

************************************************/


typedef struct
{
	OS_STK *pMYStack;//ջ��ָ��
	void (*pTask)(void *p_arg);//����ĵ�ַ
	INT32U MYDelay_ms;//��������ʱ��ʱ�䣬��λ��ms��
	INT32U MYWork;//�ҵĹ�������Ϊ0ʱϵͳ�����������
	INT32U Pend;	//֪�����
	INT32U TackUsed;//ջʹ����
	OS_STK *TackInitial;//��ʼջ��ַ
	INT32U LastTime;		//���һ�ε��ȵ�ʱ��
}OS_TCB,*pOS_TCB;

 


extern				INT32U  OSRunning    ;           // External referencesϵͳ�������б�־λ
extern        INT32U  OSPrioCur;
extern        INT32U  OSPrioHighRdy;
extern       pOS_TCB  OSTCBCur;							//����ṹ�壬��ջ��ַ����λ
extern       pOS_TCB OSTCBHighRdy;
extern        INT32U  OSIntNesting;
extern        INT32U  OSIntExit;




#define TASK_MAX_NUM 32u

//����δʹ�õ�ջĬ��ֵ��������ʹ��ͳ��
#define IDLE_TACK_VALUE 0xffffffff


//��������ṹ��
extern OS_TCB TCB_Table[TASK_MAX_NUM];
  
								//��������ע���,
#if TASK_MAX_NUM<=32u
extern INT32U   TASK_Free;
#else
extern INT32U  TASK_Free[TASK_MAX_NUM/32+1];
#endif






INT8U CreateTask (void   (*task)(void *p_arg),void *p_arg,OS_STK  *ptos,INT8U prio)	;		

INT8U CreateTaskN (void   (*task)(void *p_arg),//�����׵�ַ
                     void    *p_arg,					//�������
                     OS_STK  *ptos_,						//�����ջ��ַ������������
										 u32 tacksize,
                     INT8U    prio);						//�������ȼ�


//У�������ջʹ����
void CheckTaskUsege ( void );
										 
//�������л�ʱͳ������ʹ����
void CheckHighRdyTaskUsege ( void );

u16 GetTaskUsed (u8 prio);
u16 GetTaskSize (u8 prio);



void  OSStart (void);
					//�������
INT8U TaskPend (INT32U prio);
INT8U TaskRepend (INT32U prio);




						//�����ջ��ʼ��
OS_STK *OSTaskStkInit (void (*task)(void *p_arg), void *p_arg, OS_STK *ptos, INT16U opt);




					//�����ṹ���ʼ��
INT8U OS_TCBInit (void (*task)(void *p_arg),OS_STK *ptos,INT8U prio);





				//������ȳ�ʼ��
void os_task_init (void);



//��������ͨ�����ȼ�ȷ��,��������жϵ��ÿ��Ի�������
void TaskIntSendMsg(u8 pro,INT32U msg);

//Ч��ͬ�ϣ��������
u8 TaskSendMsg(u8 pro,INT32U msg);

void TaskMsgZero(void);

//�ȴ���Ϣ����
INT32U TaskGetMsg(void);


void OS_Enter_Onlyme(void);

void OS_Exit_Onlyme(void);


			//һ��Ҫ�ɶ�ʹ��
#define OS_ENTER_ONLYME() OS_Enter_Onlyme( )

#define OS_EXIT_ONLYME() OS_Exit_Onlyme()



#ifdef __cplusplus
	}
#endif




#endif


