#include "includes.h"
#include "w5500.h"
#include "uart.h"
#include "enternet.h"
#include "dhcp.h"
#include "my_messeg.h"
#include "wk_json.h"
#include "debug.h"
#include "ntp.h"
#include "power.h"
#include "hard_irq.h"
#include "timer.h"
#include "my_topmsg.h"
#include "my_w5500.h"







//调试线程
void my_debug_task ( void *t)
{
	u8 *rx_buff=0;
	u32 msg=0;
	uart_init(115200,rx_buff); 
	SOCKET1_SetFocus(OS_GET_PRIOHIGH());
	sleep_ms(5000);
	while (1)
	{
		msg=TaskGetMsg();
		if (msg!=2)
		{
			if (rx_buff)
			{
				myfree(rx_buff);
				rx_buff=0;
				uart_init(115200,rx_buff);
			}
			my_debug ( );		//调试信息输出
		}
		else
		{
			if (rx_buff==0)
			{
				rx_buff=mymalloc (1024);
				uart_init(115200,rx_buff); 
			}
			else
			{
				dbg_Interpreter(rx_buff,uart_print);
			}
		}
	}
	
}










