
#ifndef __OS_TASK_H
#define __OS_TASK_H

#ifdef __cplusplus
	extern "C" {
#endif



/************************************************

				任务TCB定义

************************************************/


typedef struct
{
	OS_STK *pMYStack;//栈顶指针
	void (*pTask)(void *p_arg);//任务的地址
	INT32U MYDelay_ms;//本次想延时的时间，单位，ms，
	INT32U MYWork;//我的工作，不为0时系统调度这个任务
	INT32U Pend;	//知否挂起
	INT32U TackUsed;//栈使用率
	OS_STK *TackInitial;//初始栈地址
	INT32U LastTime;		//最后一次调度的时间
}OS_TCB,*pOS_TCB;






/***************系统文件宏定义******************/

//最大任务数
#define TASK_MAX_NUM 32u

//任务未使用的栈默认值，用于做使用统计
#define IDLE_TACK_VALUE 0xffffffff

//进入不可调度临界区，一定要成对使用
#define OS_ENTER_ONLYME() OS_Enter_Onlyme( )

//退出不可调度临界区，一定要成对使用
#define OS_EXIT_ONLYME() OS_Exit_Onlyme()

//获取当前临界区层数
#define OS_GET_ONLYME() OS_Get_Onlyme()

//获取当前运行的任务优先级
#define OS_GET_PRIOHIGH() OS_Get_PrioHighRdy()


/***************系统文件宏定义End******************/



/****************系统文件全局变量定义***************/

//定义任务结构体
extern OS_TCB TCB_Table[TASK_MAX_NUM];
  
//定义任务注册表,
#if TASK_MAX_NUM<=32u
extern INT32U   TASK_Free;
#else
extern INT32U  TASK_Free[TASK_MAX_NUM/32+1];
#endif

extern				INT32U  OSRunning    ;           // External references系统正在运行标志位
extern        INT32U  OSPrioCur;
extern        INT32U  OSPrioHighRdy;
extern       pOS_TCB  OSTCBCur;							//任务结构体，堆栈地址在首位
extern       pOS_TCB  OSTCBHighRdy;
extern        INT32U  OSIntNesting;
extern        INT32U  OSIntExit;


/**************系统文件全局变量定义End***************/




/**************系统全局函数定义**********************/


//任务调度初始化
//调用这个函数初始化滴答定时器
//为系统提供节拍
void os_task_init (void);

//开始任务调度
void OSStart (void);

//创建任务，不支持栈占用率统计
//参数：task，任务函数
//参数：p_arg，任务函数的参数
//参数：ptos，任务栈栈顶
//参数：prio，任务优先级
//返回值：0，任务创建成功；非0，创建失败
INT8U CreateTask (void   (*task)(void *p_arg),void *p_arg,OS_STK  *ptos,INT8U prio)	;

//创建任务，支持栈占用率统计
//参数：task，任务函数
//参数：p_arg，任务函数的参数
//参数：ptos_，任务栈栈底
//参数：tacksize，任务栈大小
//参数：prio，任务优先级
//返回值：0，任务创建成功；非0，创建失败
INT8U CreateTaskN (void (*task)(void *p_arg),void *p_arg,OS_STK *ptos_,u32 tacksize,INT8U prio);

//删除任务
//该删除任务函数只是永久取消系统对该任务的调度，
//任务本身占用的资源不会因为任务被删除而回收
void DeleteTask (u8 pro);

//统计所有任务堆栈使用率
//可以在软件定时器中断服务函数中调用该函数
//统计时间越久统计结果越接近任务的最大栈使用率
void CheckTaskUsege ( void );

//统计当前任务的栈使用率
//可以把这个函数挂载到任务切换回调指针上，
//在发生任务切换的时候即统计当前任务的栈使用率
//这个函数会较大地影响系统效率
void CheckHighRdyTaskUsege ( void );

//获取指定优先级任务的栈空间使用量
u16 GetTaskUsed (u8 prio);

//获取指定优先级任务的栈空间大小
u16 GetTaskSize (u8 prio);

//任务挂起
//每次调用都会增加任务的挂起层数
//在任务的挂起层数不为0时任务不被系统调度
//使用对应次任务恢复函数直至挂起层数为0时任务会恢复调度
INT8U TaskPend (INT32U prio);

//任务恢复
//每次调用都会减少任务的挂起层数
//在任务挂起层数为0时调用这个函数没有任何作用
INT8U TaskRepend (INT32U prio);

//中断函数唤醒任务，通过优先级确定
//在任务处于等待事件的时候可以唤醒任务
//不能唤醒被挂起的任务
void TaskIntSendMsg(u8 pro,INT32U msg);

//任务中唤醒任务，通过优先级确定
//在任务处于等待事件的时候可以唤醒任务
//不能唤醒被挂起的任务
u8 TaskSendMsg(u8 pro,INT32U msg);

//任务事件标志清零
void TaskMsgZero(void);

//等待事件
//调用这个函数之后任务会阻塞，直到有事件发生
INT32U TaskGetMsg(void);

/**************系统全局函数定义End**********************/




/*************系统内部调用函数定义*************************/


void OS_Enter_Onlyme(void);

void OS_Exit_Onlyme(void);

//获取不可调度宏
extern __INLINE INT8U OS_Get_Onlyme ( void );

//获取当前任务优先级
extern __INLINE INT32U OS_Get_PrioHighRdy ( void );

//任务快结构体初始化
INT8U OS_TCBInit (void (*task)(void *p_arg),OS_STK *ptos,INT8U prio);

/*************系统内部调用函数定义End***********************/




#ifdef __cplusplus
	}
#endif




#endif


