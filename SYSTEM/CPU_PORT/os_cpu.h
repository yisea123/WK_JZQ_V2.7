/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name		: os_cpu_c.c 
* Author		: Librae
* Date			: 06/10/2010
* Description	: ��COS-II��STM32�ϵ���ֲ����C���Բ��֣�
*				  ���������ջ��ʼ������͹��Ӻ�����

******************************************************************************/

#ifndef	__OS_CPU_H__
#define	__OS_CPU_H__



#ifdef __cplusplus
	extern "C" {
#endif









#ifdef  OS_CPU_GLOBALS
#define OS_CPU_EXT
#else
#define OS_CPU_EXT  extern
#endif

/******************************************************************************
*                    ������������޹ص���������
******************************************************************************/

typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;			/* Unsigned  8 bit quantity       */
typedef signed   char  INT8S;			/* Signed    8 bit quantity       */
typedef unsigned short INT16U;			/* Unsigned 16 bit quantity       */
typedef signed   short INT16S;			/* Signed   16 bit quantity       */
typedef unsigned int   INT32U;			/* Unsigned 32 bit quantity       */
typedef signed   int   INT32S;			/* Signed   32 bit quantity       */
typedef float          FP32;			/* Single precision floating point*/
typedef double         FP64;			/* Double precision floating point*/

//STM32��32λλ���,����OS_STK��OS_CPU_SR��Ӧ��Ϊ32λ��������
typedef unsigned int   OS_STK;			/* Each stack entry is 32-bit wide*/
typedef unsigned int   OS_CPU_SR;		/* Define size of CPU status register*/




/***********************���������Ľṹ��****************************/


typedef struct 
{
	OS_CPU_SR R4;
	OS_CPU_SR R5;
	OS_CPU_SR R6;
	OS_CPU_SR R7;
	OS_CPU_SR R8;
	OS_CPU_SR R9;
	OS_CPU_SR R10;
	OS_CPU_SR R11;
	OS_CPU_SR R0;
	OS_CPU_SR R1;
	OS_CPU_SR R2;
	OS_CPU_SR R3;
	OS_CPU_SR R12;
	OS_CPU_SR LR;
	OS_CPU_SR PC;

	//OS_CPU_SR SP;
} Task_Context;



/*********************���������Ľṹ��End****************************/



/*********************�ں���غ궨��*****************************/

//����ջ����������.
//CM3��,ջ���ɸߵ�ַ��͵�ַ������,����OS_STK_GROWTH����Ϊ1
#define  OS_STK_GROWTH        1      

//�����л���,�ɻ��ʵ��.
#define  OS_TASK_SW()         OSCtxSw()

//OS_CRITICAL_METHOD = 1 :ֱ��ʹ�ô������Ŀ����ж�ָ����ʵ�ֺ� 
//OS_CRITICAL_METHOD = 2 :���ö�ջ����ͻָ�CPU��״̬ 
//OS_CRITICAL_METHOD = 3 :���ñ�������չ���ܻ�ó���״̬�֣������ھֲ�����cpu_sr
#define  OS_CRITICAL_METHOD   3	 	//�����ٽ�εķ���

//�����ж������ٽ���
#if OS_CRITICAL_METHOD == 3
#define  OS_ENTER_CRITICAL()  {cpu_sr = OS_CPU_SR_Save();}
#define  OS_EXIT_CRITICAL()   {OS_CPU_SR_Restore(cpu_sr);}
#endif


/********************�ں���غ궨��End**************************/









/******************��ຯ������***************************/

//�����в��������л�
void OSCtxSw(void);

//�ж��в��������л�
void OSIntCtxSw(void);

//��������ȼ���ʼ�������
void OSStartHighRdy(void);

//����жϷ�����
void OSPendSV(void);

//����ǰ�������
OS_CPU_SR GetZeroNum(OS_CPU_SR t);

//16λ�İ�λ��ת
OS_CPU_SR GetRBIT16(OS_CPU_SR t);

//��ת��ָ����ַ
OS_CPU_SR BlxExternFun (OS_CPU_SR a,OS_CPU_SR b,OS_CPU_SR c,OS_CPU_SR d,
					OS_CPU_SR e,OS_CPU_SR f,OS_CPU_SR g,OS_CPU_SR h,OS_CPU_SR addr);

#if OS_CRITICAL_METHOD == 3u

	//�����ж�����״̬
	OS_CPU_SR OS_CPU_SR_Save(void);
	
	//�ָ��ж�����״̬
	void OS_CPU_SR_Restore(OS_CPU_SR cpu_sr);
#endif


/******************��ຯ������End***************************/







/*****************C��������***************************/

//�����ջ��ʼ��
OS_STK *OSTaskStkInit (void (*task)(void *p_arg), void *p_arg, OS_STK *ptos, INT16U opt);

//��ȡָ�����ȼ�������������
Task_Context *OS_TaskGetContext (u8 pro);

/******************C��������End************************/






#ifdef __cplusplus
	}
#endif



#endif

/************************ (C) COPYLEFT 2010 Leafgrass ************************/
