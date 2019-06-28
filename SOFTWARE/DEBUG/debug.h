

#ifndef __DEBUG_H
#define __DEBUG_H


#ifdef __cplusplus
	extern "C" {
#endif



#define dbg_printf(fmat,...) do{char *str=mymalloc(128);fprintf (str,fmat,__VA_ARGS__);myfree(str);}while(0);




/***************************************************

					调试时使用网口输出调试信息

***************************************************/


extern u8 DBG_IP[4];//调试用的目标ip地址
extern u16 DBG_PORT;//调试用的端口号




/***************************************************

					调试时文件系统输出调试信息

***************************************************/


extern u8 DBG_FATS;


/***************************************************

					输出内存占用信息信息

***************************************************/

extern u32 memsize    ;							//内存总大小


/***************************************************

					输出网络连接状态信息

***************************************************/

extern u8 DBG_INTER_STATE;



extern u16 Get_MyAddr(void);

//复制与上位机的通信
extern u8 DBG_COPY_TO_S1CK;




//调试函数调用入口
void my_debug (void);

void dbg_Interpreter(u8 *recvbuff,void (*sendstr) (char *str));

	//板子信息
void dbg_info (void);

void dbg_err (u8 errtype);

void dbg_devconfig(void);

void dbg_help(void);

void dbg_reboot (void);

void dbg_oche (u8 *);

void dbg_booting(void);

	//复制数据到调试端口
void dbg_copydata (u8 *);


				//使用DNS获取域名的IP地址
void dbg_getip(u8 *buff);

void dbg_set (u8 *);
				//测试mqtt连接
void dbg_mqtt(u8 *buff);


void dbg_task (u8 *buff);

void dbg_ping (u8 *buff);

void dbg_ntp (u8 *buff);

void dbg_whos(u8 *buff); 

void dbg_find(u8 *buff);

void dbg_key(u8 *buff);

//内存相关的debug命令
void dbg_mem (u8 *buff);

//函数调用相关的debug命令
void dbg_fun (u8 *buff);

//执行脚本
void dbg_run (u8 *buff);

//dbg打印函数
void dbg_print (char *str);

//udp字符串发送接口
void dbg_send_udp (char *str);

//执行swd
void dbg_swd (u8 *buff);

//swd初始化
void dbg_swdinit (u8 *buff);

//swd连接目标设备
void dbg_swdidcode (u8 *buff);

//swd加载loader文件
void dbg_swdloader (u8 *buff);

//swd下载数据
void dbg_swdprogram (u8 *buff);

/**********执行指令****************/


void	cmd_byudp (u8 *);





#ifdef __cplusplus
	}
#endif




#endif



















