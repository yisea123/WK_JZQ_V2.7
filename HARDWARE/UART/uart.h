

#ifndef UART_H__
#define UART_H__

#ifdef  __cplusplus
	extern "C" {
#endif


void uart_init (u32 bound,u8 *rx_buff);

u16 uart_sendBytes (u8 *data,u16 len);

void uart_print (char *str);





#ifdef  __cplusplus
	}
#endif





#endif






