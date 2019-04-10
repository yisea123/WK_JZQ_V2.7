
#ifndef SYSINFO_H__
#define SYSINFO_H__

/***********************************************

版本说明：
	V2.3  
			使用实体按键，彩色灯带，串口屏精度为1
	V2.5
			使用电容按键，单色灯带，串口屏精度为1
	V2.6
			使用电容按键，单色灯带，串口屏精度为0.1


***********************************************/

#if defined(JZQ_V2_3)

	#define JZQ_Version "KN_WKJZQ V2.3"

#elif defined(JZQ_V2_5) 

	#define JZQ_Version "KN_WKJZQ V2.5"

#elif defined(JZQ_V2_6) /*JZQ_V2_6*/

	#define JZQ_Version "KN_WKJZQ V2.6"

#endif


#define JZQ_SoftVersion "WK_JZQ V2.7"



#endif













