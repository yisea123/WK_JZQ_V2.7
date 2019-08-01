
#ifndef __OS_TASK_H
#define __OS_TASK_H

#ifdef __cplusplus
	extern "C" {
#endif



/************************************************

				����TCB����

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






/***************ϵͳ�ļ��궨��******************/

//���������
#define TASK_MAX_NUM 32u

//����δʹ�õ�ջĬ��ֵ��������ʹ��ͳ��
#define IDLE_TACK_VALUE 0xffffffff

//���벻�ɵ����ٽ�����һ��Ҫ�ɶ�ʹ��
#define OS_ENTER_ONLYME() OS_Enter_Onlyme( )

//�˳����ɵ����ٽ�����һ��Ҫ�ɶ�ʹ��
#define OS_EXIT_ONLYME() OS_Exit_Onlyme()

//��ȡ��ǰ�ٽ�������
#define OS_GET_ONLYME() OS_Get_Onlyme()

//��ȡ��ǰ���е��������ȼ�
#define OS_GET_PRIOHIGH() OS_Get_PrioHighRdy()


/***************ϵͳ�ļ��궨��End******************/



/****************ϵͳ�ļ�ȫ�ֱ�������***************/

//��������ṹ��
extern OS_TCB TCB_Table[TASK_MAX_NUM];
  
//��������ע���,
#if TASK_MAX_NUM<=32u
extern INT32U   TASK_Free;
#else
extern INT32U  TASK_Free[TASK_MAX_NUM/32+1];
#endif

extern				INT32U  OSRunning    ;           // External referencesϵͳ�������б�־λ
extern        INT32U  OSPrioCur;
extern        INT32U  OSPrioHighRdy;
extern       pOS_TCB  OSTCBCur;							//����ṹ�壬��ջ��ַ����λ
extern       pOS_TCB  OSTCBHighRdy;
extern        INT32U  OSIntNesting;
extern        INT32U  OSIntExit;


/**************ϵͳ�ļ�ȫ�ֱ�������End***************/




/**************ϵͳȫ�ֺ�������**********************/


//������ȳ�ʼ��
//�������������ʼ���δ�ʱ��
//Ϊϵͳ�ṩ����
void os_task_init (void);

//��ʼ�������
void OSStart (void);

//�������񣬲�֧��ջռ����ͳ��
//������task��������
//������p_arg���������Ĳ���
//������ptos������ջջ��
//������prio���������ȼ�
//����ֵ��0�����񴴽��ɹ�����0������ʧ��
INT8U CreateTask (void   (*task)(void *p_arg),void *p_arg,OS_STK  *ptos,INT8U prio)	;

//��������֧��ջռ����ͳ��
//������task��������
//������p_arg���������Ĳ���
//������ptos_������ջջ��
//������tacksize������ջ��С
//������prio���������ȼ�
//����ֵ��0�����񴴽��ɹ�����0������ʧ��
INT8U CreateTaskN (void (*task)(void *p_arg),void *p_arg,OS_STK *ptos_,u32 tacksize,INT8U prio);

//ɾ������
//��ɾ��������ֻ������ȡ��ϵͳ�Ը�����ĵ��ȣ�
//������ռ�õ���Դ������Ϊ����ɾ��������
void DeleteTask (u8 pro);

//ͳ�����������ջʹ����
//�����������ʱ���жϷ������е��øú���
//ͳ��ʱ��Խ��ͳ�ƽ��Խ�ӽ���������ջʹ����
void CheckTaskUsege ( void );

//ͳ�Ƶ�ǰ�����ջʹ����
//���԰�����������ص������л��ص�ָ���ϣ�
//�ڷ��������л���ʱ��ͳ�Ƶ�ǰ�����ջʹ����
//���������ϴ��Ӱ��ϵͳЧ��
void CheckHighRdyTaskUsege ( void );

//��ȡָ�����ȼ������ջ�ռ�ʹ����
u16 GetTaskUsed (u8 prio);

//��ȡָ�����ȼ������ջ�ռ��С
u16 GetTaskSize (u8 prio);

//�������
//ÿ�ε��ö�����������Ĺ������
//������Ĺ��������Ϊ0ʱ���񲻱�ϵͳ����
//ʹ�ö�Ӧ������ָ�����ֱ���������Ϊ0ʱ�����ָ�����
INT8U TaskPend (INT32U prio);

//����ָ�
//ÿ�ε��ö����������Ĺ������
//������������Ϊ0ʱ�����������û���κ�����
INT8U TaskRepend (INT32U prio);

//�жϺ�����������ͨ�����ȼ�ȷ��
//�������ڵȴ��¼���ʱ����Ի�������
//���ܻ��ѱ����������
void TaskIntSendMsg(u8 pro,INT32U msg);

//�����л�������ͨ�����ȼ�ȷ��
//�������ڵȴ��¼���ʱ����Ի�������
//���ܻ��ѱ����������
u8 TaskSendMsg(u8 pro,INT32U msg);

//�����¼���־����
void TaskMsgZero(void);

//�ȴ��¼�
//�����������֮�������������ֱ�����¼�����
INT32U TaskGetMsg(void);

/**************ϵͳȫ�ֺ�������End**********************/




/*************ϵͳ�ڲ����ú�������*************************/


void OS_Enter_Onlyme(void);

void OS_Exit_Onlyme(void);

//��ȡ���ɵ��Ⱥ�
extern __INLINE INT8U OS_Get_Onlyme ( void );

//��ȡ��ǰ�������ȼ�
extern __INLINE INT32U OS_Get_PrioHighRdy ( void );

//�����ṹ���ʼ��
INT8U OS_TCBInit (void (*task)(void *p_arg),OS_STK *ptos,INT8U prio);

/*************ϵͳ�ڲ����ú�������End***********************/




#ifdef __cplusplus
	}
#endif




#endif


