/*
************************************************************************************************
主要的包含文件
			
				这里包含一些不易改变的头文件

文 件: INCLUDES.C ucos包含文件
作 者: Jean J. Labrosse
************************************************************************************************
*/

#ifndef __INCLUDES_H__
#define __INCLUDES_H__
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>
#include <stm32f10x.h>	    

#include "os_cpu.h"
#include "sys.h"
#include "delay.h"
#include "rtc.h"

#include "crc8_16.h"
#include "fats.h"
#include "stmflash.h"
#include "flash.h"
#include "my_fun.h"


#include "os_mem.h"
#include "os_msg.h"
#include "os_task.h"
#include "os_fats.h"
#include "os_sys.h"

#include "MQTTPacket.h"
#include "cjson.h"

#include "data_map.h"


#ifndef malloc 
	#define malloc mymalloc
#endif
#ifndef free 
	#define free myfree
#endif
#ifndef memcpy
	#define memcpy mymemcpy
#endif

#endif































