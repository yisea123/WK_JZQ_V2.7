#ifndef __FILE_H
#define __FILE_H


#ifdef  __cplusplus
	extern "C" {
#endif




//�ļ�֧�ֵ�����С�����������С����Ŀ��ܳ����ڴ治��
#define FILE_MAX_SIZE  4096


u8 read_json(TCHAR *file_name,u8 *buf,u16 buff_size);//��ȡjson�ļ���buf�ǻ�������ַ��buff_size�ǻ�������С

u8 write_config (void);
u8 read_config (void);

//����ϵͳ��ʼ���ű�
u8 run_sysinit (void);

//�����ļ����beep����
u8 run_beep (const char *name_str);

#ifdef  __cplusplus
	}
#endif



#endif








