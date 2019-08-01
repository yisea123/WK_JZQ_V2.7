/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name		: os_cpu_c.c 
* Author		: Librae
* Date			: 06/10/2010
* Description	: μCOS-II在STM32上的移植代码C语言部分，
*				  包括任务堆栈初始化代码和钩子函数等

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
*                    定义与编译器无关的数据类型
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

//STM32是32位位宽的,这里OS_STK和OS_CPU_SR都应该为32位数据类型
typedef unsigned int   OS_STK;			/* Each stack entry is 32-bit wide*/
typedef unsigned int   OS_CPU_SR;		/* Define size of CPU status register*/




/***********************任务上下文结构体****************************/


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



/*********************任务上下文结构体End****************************/



/*********************内核相关宏定义*****************************/

//定义栈的增长方向.
//CM3中,栈是由高地址向低地址增长的,所以OS_STK_GROWTH设置为1
#define  OS_STK_GROWTH        1      

//任务切换宏,由汇编实现.
#define  OS_TASK_SW()         OSCtxSw()

//OS_CRITICAL_METHOD = 1 :直接使用处理器的开关中断指令来实现宏 
//OS_CRITICAL_METHOD = 2 :利用堆栈保存和恢复CPU的状态 
//OS_CRITICAL_METHOD = 3 :利用编译器扩展功能获得程序状态字，保存在局部变量cpu_sr
#define  OS_CRITICAL_METHOD   3	 	//进入临界段的方法

//进入中断屏蔽临界区
#if OS_CRITICAL_METHOD == 3
#define  OS_ENTER_CRITICAL()  {cpu_sr = OS_CPU_SR_Save();}
#define  OS_EXIT_CRITICAL()   {OS_CPU_SR_Restore(cpu_sr);}
#endif


/********************内核相关宏定义End**************************/









/******************汇编函数定义***************************/

//任务中产生任务切换
void OSCtxSw(void);

//中断中产生任务切换
void OSIntCtxSw(void);

//从最高优先级开始任务调度
void OSStartHighRdy(void);

//软件中断服务函数
void OSPendSV(void);

//计算前导零个数
OS_CPU_SR GetZeroNum(OS_CPU_SR t);

//16位的按位反转
OS_CPU_SR GetRBIT16(OS_CPU_SR t);

//跳转至指定地址
OS_CPU_SR BlxExternFun (OS_CPU_SR a,OS_CPU_SR b,OS_CPU_SR c,OS_CPU_SR d,
					OS_CPU_SR e,OS_CPU_SR f,OS_CPU_SR g,OS_CPU_SR h,OS_CPU_SR addr);

#if OS_CRITICAL_METHOD == 3u

	//保存中断屏蔽状态
	OS_CPU_SR OS_CPU_SR_Save(void);
	
	//恢复中断屏蔽状态
	void OS_CPU_SR_Restore(OS_CPU_SR cpu_sr);
#endif


/******************汇编函数定义End***************************/







/*****************C函数定义***************************/

//任务堆栈初始化
OS_STK *OSTaskStkInit (void (*task)(void *p_arg), void *p_arg, OS_STK *ptos, INT16U opt);

//获取指定优先级的任务上下文
Task_Context *OS_TaskGetContext (u8 pro);

/******************C函数定义End************************/






#ifdef __cplusplus
	}
#endif



#endif

/************************ (C) COPYLEFT 2010 Leafgrass ************************/
