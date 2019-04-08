/*
************************************************************************************************
��Ҫ�İ����ļ�
			
				�������һЩ���׸ı��ͷ�ļ�

�� ��: INCLUDES.C ucos�����ļ�
�� ��: Jean J. Labrosse
************************************************************************************************
*/

#ifndef __INCLUDES_H__
#define __INCLUDES_H__
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
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































