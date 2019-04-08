#include "includes.h"
#include "hard_irq.h"


// ‰»ÎΩπµ„
u8 RF_FOCUS=TASK_MAX_NUM;
u8 LCD_FOCUS=TASK_MAX_NUM;
u8 SOCKET0_FOCUS=TASK_MAX_NUM;
u8 SOCKET1_FOCUS=TASK_MAX_NUM;
u8 SOCKET2_FOCUS=TASK_MAX_NUM;
u8 SOCKET3_FOCUS=TASK_MAX_NUM;
u8 SOCKET4_FOCUS=TASK_MAX_NUM;
u8 SOCKET5_FOCUS=TASK_MAX_NUM;
u8 SOCKET6_FOCUS=TASK_MAX_NUM;
u8 SOCKET7_FOCUS=TASK_MAX_NUM;






void USART1_SetFocus(u8 focus)
{
	if (focus<TASK_MAX_NUM)
		RF_FOCUS=focus;
}
u8 USART1_GetFocus (void)
{
	return RF_FOCUS;
}





void USART2_SetFocus(u8 focus)
{
	if (focus<TASK_MAX_NUM)
		LCD_FOCUS=focus;
}
u8 USART2_GetFocus (void)
{
	return LCD_FOCUS;
}




void SOCKET0_SetFocus (u8 focus)
{
	if (focus<TASK_MAX_NUM)
		SOCKET0_FOCUS=focus;
}
void SOCKET1_SetFocus (u8 focus)
{
	if (focus<TASK_MAX_NUM)
		SOCKET1_FOCUS=focus;
}
void SOCKET2_SetFocus (u8 focus)
{
	if (focus<TASK_MAX_NUM)
		SOCKET2_FOCUS=focus;
}
void SOCKET3_SetFocus (u8 focus)
{
	if (focus<TASK_MAX_NUM)
		SOCKET3_FOCUS=focus;
}
void SOCKET4_SetFocus (u8 focus)
{
	if (focus<TASK_MAX_NUM)
		SOCKET4_FOCUS=focus;
}
void SOCKET5_SetFocus (u8 focus)
{
	if (focus<TASK_MAX_NUM)
		SOCKET5_FOCUS=focus;
}
void SOCKET6_SetFocus (u8 focus)
{
	if (focus<TASK_MAX_NUM)
		SOCKET6_FOCUS=focus;
}
void SOCKET7_SetFocus (u8 focus)
{
	if (focus<TASK_MAX_NUM)
		SOCKET7_FOCUS=focus;
}




u8 SOCKET0_GetFocus (void)
{
	return SOCKET0_FOCUS;
}
u8 SOCKET1_GetFocus (void)
{
	return SOCKET1_FOCUS;
}
u8 SOCKET2_GetFocus (void)
{
	return SOCKET2_FOCUS;
}
u8 SOCKET3_GetFocus (void)
{
	return SOCKET3_FOCUS;
}
u8 SOCKET4_GetFocus (void)
{
	return SOCKET4_FOCUS;
}
u8 SOCKET5_GetFocus (void)
{
	return SOCKET5_FOCUS;
}
u8 SOCKET6_GetFocus (void)
{
	return SOCKET6_FOCUS;
}
u8 SOCKET7_GetFocus (void)
{
	return SOCKET7_FOCUS;
}


