
#ifndef __MY_AUTOCONTROL_H
#define __MY_AUTOCONTROL_H


#ifdef __cplusplus
	extern "C" {
#endif



//自动控制循环
void my_autocontrol (void * t);

//发送报警消息
void send_warn(u16 addr,u8 devtype,u8 warntype,u8 ddd);


#ifdef __cplusplus
	}
#endif



#endif



