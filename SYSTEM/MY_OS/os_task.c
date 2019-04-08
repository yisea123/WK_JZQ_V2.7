#include "includes.h"
#include "os_task.h"


/****************************************************

		有关任务的操作函数
		任务切换有4种：
								1.高优先级主动释放CPU，如果有其他就绪任务会产生任务调度
								2.任务中唤醒其他任务，如果所唤醒的优先级比当前任务高，会产生任务调度
								3.中断唤醒其他任务，如果中断所唤醒的任务优先级比当前任务高，会产生任务调度
									中断导致的任务调度可能发生在任何地方，当CPU在执行不可打断的操作时要暂时屏蔽
									防止因任务调度产生的意外，在操作结束后马上进行一次调度
									使用OS_ENTER_ONLYME()宏来进入屏蔽区，使用OS_EXIT_ONLYME()来退出并且进行一次第4种调度
								4.任务的唤醒是在中断中发生的，但是在唤醒的时候CPU正在执行不可打断的操作导致无法调度
									在不可打断的操作执行完之后，如果有更高优先级的就绪任务，会产生任务调度

******************************************************/

INT32U  OSRunning=0  ;       //任务调度在运行吗，1，运行，0，没运行    // External references
INT32U  OSPrioCur=3;				
INT32U  OSPrioHighRdy=4;//最高的优先级
pOS_TCB  OSTCBCur;
pOS_TCB  OSTCBHighRdy;//优先级最高的TCB指针
INT32U  OSIntNesting=7;   //进入中断的层数
INT32U  OSIntExit=8;			//中断退出函数

			//在这里统计任务栈使用情况
void (*OSTaskSwHook)(void) =0;//CheckHighRdyTaskUsege;

INT8U  OS_ONLYME=0;//禁止调度标志

 
//定义任务结构体
OS_TCB TCB_Table[TASK_MAX_NUM]={0};
  
								//定义任务注册表,在注册表中的任务是可以调度的
#if TASK_MAX_NUM<=32u
INT32U   TASK_Free=0;
#else
INT32U  TASK_Free[TASK_MAX_NUM/32+1]={0};
#endif
 


/*************************************

		函数作用，初始化系统延时，初始化之后任务调度才会启用

**************************************/

void os_task_init (void)
{
	delay_init();	    	 //延时函数初始化
}



INT8U CreateTaskN (void   (*task)(void *p_arg),//任务首地址
                     void    *p_arg,					//任务参数
                     OS_STK  *ptos_,						//任务堆栈地址顶，向下生长
										 u32 tacksize,
                     INT8U    prio)						//任务优先级
{
#if TASK_MAX_NUM<=32u
	INT32U myprio=0x80000000>>prio;
#endif
	OS_STK *psp;
	
	
	OS_STK *ptos=ptos_+tacksize-1;
	
	
	
#if TASK_MAX_NUM<=32u
	if (TASK_Free&myprio)
	{
		return 1;//返回错误，该优先级已经注册过
	}
	else
	{
		TASK_Free|=myprio;
	}
#endif
	
	for (u16 i=0;i<tacksize;i++)
	{
		*(ptos-i)=IDLE_TACK_VALUE;
	}
	
	TCB_Table[prio].TackInitial=ptos;
	psp=OSTaskStkInit(task,p_arg,ptos,0);//初始化任务堆栈，返回新栈顶
	OS_TCBInit (task,psp,prio);
	TCB_Table[prio].TackUsed=tacksize&0x0000ffff;
	return 0;
}




INT8U CreateTask (void   (*task)(void *p_arg),//任务首地址
                     void    *p_arg,					//任务参数
                     OS_STK  *ptos,						//任务堆栈地址顶，向下生长
                     INT8U    prio)						//任务优先级
{
#if TASK_MAX_NUM<=32u
	INT32U myprio=0x80000000>>prio;
#endif
	OS_STK *psp;
	
	
	
	
	
#if TASK_MAX_NUM<=32u
	if (TASK_Free&myprio)
	{
		return 1;//返回错误，该优先级已经注册过
	}
	else
	{
		TASK_Free|=myprio;
	}
#endif
	
	psp=OSTaskStkInit(task,p_arg,ptos,0);//初始化任务堆栈，返回新栈顶
	OS_TCBInit (task,psp,prio);
	return 0;
}




u16 GetTaskSize (u8 prio)
{
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	u16 size=0;
	OS_ENTER_CRITICAL();
	size=TCB_Table[prio].TackUsed&0x0000ffff;
	OS_EXIT_CRITICAL();
	return size;
}


u16 GetTaskUsed (u8 prio)
{
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	u16 used=0;
	OS_ENTER_CRITICAL();
	used=TCB_Table[prio].TackUsed>>16;
	OS_EXIT_CRITICAL();
	return used;
}


//校验任务堆栈使用率
void CheckTaskUsege ( void )
{
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	u32 used=0;
	u32 used_old=0;
	u32 *tack_p=0;
	OS_ENTER_CRITICAL();
	for (u8 i=0;i<TASK_MAX_NUM;i++)
	{
		if (TCB_Table[i].pTask)
		{
			tack_p=TCB_Table[i].TackInitial-(TCB_Table[i].TackUsed&0x0000ffff)+1;
			while(*tack_p++==IDLE_TACK_VALUE);
			//used=TCB_Table[i].TackInitial- TCB_Table[i].pMYStack;
			used=(TCB_Table[i].TackInitial- tack_p)+1;
			used_old=TCB_Table[i].TackUsed>>16;
			if (used_old<used)		//记录最大使用量
			{
				TCB_Table[i].TackUsed&=0x0000ffff;
				TCB_Table[i].TackUsed|=used<<16;
			}
		}
	}
	OS_EXIT_CRITICAL();
	
}

//在任务切换时统计任务使用率
void CheckHighRdyTaskUsege ( void )
{
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	u32 used=0;
	u32 used_old=0;
	OS_ENTER_CRITICAL();
	if (TCB_Table[OSPrioHighRdy].pTask)
	{
		used=TCB_Table[OSPrioHighRdy].TackInitial- TCB_Table[OSPrioHighRdy].pMYStack;
		used_old=TCB_Table[OSPrioHighRdy].TackUsed>>16;
		if (used_old<used)		//记录最大使用量
		{
			TCB_Table[OSPrioHighRdy].TackUsed&=0x0000ffff;
			TCB_Table[OSPrioHighRdy].TackUsed|=used<<16;
		}
	}
	OS_EXIT_CRITICAL();
	
}



			//初始化任务堆栈结构体，
INT8U OS_TCBInit (void (*task)(void *p_arg),OS_STK *ptos,INT8U prio)
{
	TCB_Table[prio].pMYStack=ptos;
	TCB_Table[prio].pTask=task;
	TCB_Table[prio].MYWork=0x80000000;//在任务初始化的时候调用一遍
	return 0;
}



void  OSStart (void)
{
    if (OSRunning == 0) {
				OSPrioHighRdy=GetZeroNum(TASK_Free);
        OSPrioCur     = OSPrioHighRdy;
        OSTCBHighRdy  = &TCB_Table[OSPrioHighRdy]; /* Point to highest priority task ready to run    */
        OSTCBCur      = OSTCBHighRdy;
        OSStartHighRdy();                            /* Execute target specific code to start task     */
    }
}





u8 Task_AddFree (u8 pro)
{
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	if (pro>=TASK_MAX_NUM)
		return 0;
	OS_ENTER_CRITICAL(); 
	if (TCB_Table[pro].pTask)
	{
		TASK_Free|=0x80000000>>pro;//任务切换为就绪状态
		TCB_Table[pro].LastTime=getSysRunTime();
	}
	OS_EXIT_CRITICAL();
	return 1;
}


u8 Task_DelFree(u8 pro)
{
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	if (pro>=TASK_MAX_NUM)
		return 0;
	OS_ENTER_CRITICAL(); 
	if (TCB_Table[pro].pTask)
	{
		TASK_Free&=~(0x80000000>>pro);//清除任务就绪标志
//		TCB_Table[pro].LastTime=getSysRunTime();
	}
	OS_EXIT_CRITICAL();
	return 1;
}






					//任务挂起
INT8U TaskPend (INT32U prio)
{
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	if (prio>=TASK_MAX_NUM)
		return 0;
	OS_ENTER_CRITICAL(); 
	TCB_Table[prio].Pend++;
	Task_DelFree(prio);
	OS_EXIT_CRITICAL();
	return 1;
}
			//任务恢复
INT8U TaskRepend (INT32U prio)
{
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	if (prio>=TASK_MAX_NUM)
		return 0;
	OS_ENTER_CRITICAL(); 
	if (TCB_Table[prio].Pend)
		TCB_Table[prio].Pend--;
	if (TCB_Table[prio].Pend==0)
		Task_AddFree(prio);
	OS_EXIT_CRITICAL();
	return 1;
}

			//任务函数中切换任务
void ToggleTasks(void)
{
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	INT32U tt=TASK_MAX_NUM;
	OS_ENTER_CRITICAL(); 
	tt=GetZeroNum(TASK_Free);
	if (tt!=OSPrioHighRdy)//如果有其他就绪任务
	{
		if (tt<TASK_MAX_NUM)
		{
			OSPrioHighRdy=tt;
			OSTCBHighRdy=&TCB_Table[tt];
			OSCtxSw();
		}
	}
	OS_EXIT_CRITICAL();
}




//唤醒任务，通过优先级确定,这个函数中断调用可以唤醒任务
//在中断中调用，需要加不可调度保护，
void TaskIntSendMsg(u8 pro,INT32U msg)
{
	if (pro>=TASK_MAX_NUM) return;
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	INT32U tt=TASK_MAX_NUM;
	OS_ENTER_CRITICAL(); 
	if (TCB_Table[pro].pTask)
	{
		TCB_Table[pro].MYWork|=msg;
		if (!(TCB_Table[pro].Pend))
			Task_AddFree(pro);
	}
	OS_EXIT_CRITICAL();
	
	
	//发送消息之后进行任务调度
	if (OS_ONLYME) return;//此时不可进行调度
	OS_ENTER_CRITICAL();
	tt=GetZeroNum(TASK_Free);
	if (tt<OSPrioHighRdy)//如果当前任务优先级最高，强行跳转
	{
		if (tt<TASK_MAX_NUM)
		{
			OSPrioHighRdy=tt;
			OSTCBHighRdy=&TCB_Table[tt];
			OSIntCtxSw();
		}
	}
	OS_EXIT_CRITICAL();
}

			//给其他任务发送消息，任务调用这个函数可以唤醒其他任务
			//由于是在线程状态下调用的，需要加不可调度保护
			//0,失败，1，成功
u8 TaskSendMsg(u8 pro,INT32U msg)
{
	if (pro>=TASK_MAX_NUM) return 0;
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	OS_ENTER_CRITICAL(); 
	if (TCB_Table[pro].pTask)
	{
		TCB_Table[pro].MYWork|=msg;
		if (!(TCB_Table[pro].Pend))
			Task_AddFree(pro);
	}
	OS_EXIT_CRITICAL();
 
	
	//发送消息之后进行任务调度
	ToggleTasks();
	return 1;

}





//任务内部处理了消息之后调用这个函数开始休息
void TaskMsgZero(void)
{
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	OS_ENTER_CRITICAL(); 
	TCB_Table[OSPrioHighRdy].MYWork=0;
	OS_EXIT_CRITICAL();
}


//等待消息唤醒
INT32U TaskGetMsg(void)
{
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	u32 msg=0;
	OS_ENTER_CRITICAL(); 
	if (TCB_Table[OSPrioHighRdy].MYWork==0)//做此判断用于支持自己给自己发消息
		Task_DelFree(OSPrioHighRdy);
	OS_EXIT_CRITICAL();
	
					//高优先级任务主动释放CPU在这里进行任务跳转
	ToggleTasks();
	while(!TASK_Free){};
	OS_ENTER_CRITICAL(); 
	msg=TCB_Table[OSPrioHighRdy].MYWork;
	TCB_Table[OSPrioHighRdy].MYWork=0;//清除自身的消息
	OS_EXIT_CRITICAL();	
	return msg;
}

u8 ONLYME_PRO=TASK_MAX_NUM;//记录进入不可调度状态的任务

void OS_Enter_Onlyme(void)
{
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	OS_ENTER_CRITICAL(); 
	OS_ONLYME++;
	ONLYME_PRO=OSPrioHighRdy;					//找出造成CPU独占的任务
	OS_EXIT_CRITICAL();
	
}

void OS_Exit_Onlyme(void)
{
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
	 OS_CPU_SR  cpu_sr;
	#endif
	OS_ENTER_CRITICAL(); 
	if (OS_ONLYME) OS_ONLYME--;
	if (!OS_ONLYME)
		ONLYME_PRO=TASK_MAX_NUM;
	OS_EXIT_CRITICAL();
	
	
	ToggleTasks();
}


