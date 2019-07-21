#ifndef __FILE_H
#define __FILE_H


#ifdef  __cplusplus
	extern "C" {
#endif




//文件支持的最大大小，比这个还大小还大的可能出现内存不够
#define FILE_MAX_SIZE  4096


u8 read_json(TCHAR *file_name,u8 *buf,u16 buff_size);//读取json文件，buf是缓存区地址，buff_size是缓存区大小

u8 write_config (void);
u8 read_config (void);

//运行系统初始化脚本
u8 run_sysinit (void);

//播放文件里的beep音乐
u8 run_beep (const char *name_str);

#ifdef  __cplusplus
	}
#endif



#endif








