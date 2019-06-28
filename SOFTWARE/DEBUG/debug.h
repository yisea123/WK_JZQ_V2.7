

#ifndef __DEBUG_H
#define __DEBUG_H


#ifdef __cplusplus
	extern "C" {
#endif



#define dbg_printf(fmat,...) do{char *str=mymalloc(128);fprintf (str,fmat,__VA_ARGS__);myfree(str);}while(0);




/***************************************************

					����ʱʹ���������������Ϣ

***************************************************/


extern u8 DBG_IP[4];//�����õ�Ŀ��ip��ַ
extern u16 DBG_PORT;//�����õĶ˿ں�




/***************************************************

					����ʱ�ļ�ϵͳ���������Ϣ

***************************************************/


extern u8 DBG_FATS;


/***************************************************

					����ڴ�ռ����Ϣ��Ϣ

***************************************************/

extern u32 memsize    ;							//�ڴ��ܴ�С


/***************************************************

					�����������״̬��Ϣ

***************************************************/

extern u8 DBG_INTER_STATE;



extern u16 Get_MyAddr(void);

//��������λ����ͨ��
extern u8 DBG_COPY_TO_S1CK;




//���Ժ����������
void my_debug (void);

void dbg_Interpreter(u8 *recvbuff,void (*sendstr) (char *str));

	//������Ϣ
void dbg_info (void);

void dbg_err (u8 errtype);

void dbg_devconfig(void);

void dbg_help(void);

void dbg_reboot (void);

void dbg_oche (u8 *);

void dbg_booting(void);

	//�������ݵ����Զ˿�
void dbg_copydata (u8 *);


				//ʹ��DNS��ȡ������IP��ַ
void dbg_getip(u8 *buff);

void dbg_set (u8 *);
				//����mqtt����
void dbg_mqtt(u8 *buff);


void dbg_task (u8 *buff);

void dbg_ping (u8 *buff);

void dbg_ntp (u8 *buff);

void dbg_whos(u8 *buff); 

void dbg_find(u8 *buff);

void dbg_key(u8 *buff);

//�ڴ���ص�debug����
void dbg_mem (u8 *buff);

//����������ص�debug����
void dbg_fun (u8 *buff);

//ִ�нű�
void dbg_run (u8 *buff);

//dbg��ӡ����
void dbg_print (char *str);

//udp�ַ������ͽӿ�
void dbg_send_udp (char *str);

//ִ��swd
void dbg_swd (u8 *buff);

//swd��ʼ��
void dbg_swdinit (u8 *buff);

//swd����Ŀ���豸
void dbg_swdidcode (u8 *buff);

//swd����loader�ļ�
void dbg_swdloader (u8 *buff);

//swd��������
void dbg_swdprogram (u8 *buff);

/**********ִ��ָ��****************/


void	cmd_byudp (u8 *);





#ifdef __cplusplus
	}
#endif




#endif



















