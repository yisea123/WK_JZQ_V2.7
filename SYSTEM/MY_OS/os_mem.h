#ifndef OS_MEM_H_
#define OS_MEM_H_

#ifdef __cplusplus
	extern "C" {
#endif




	
#ifndef NULL
#define NULL 0
#endif

#define STM32_FLASH_SIZE ((*(u32 *)0x1FFFF7E0)&0x0000ffff)

#define SYS_MEM_SIZE (10*1024) //������ʹ�õ��ڴ��С

#define MEM_BLOCK_SIZE			  (32)  	  					          //�ڴ���СΪ32�ֽ�
																																//�����ڴ�����СΪ30k��ʣ��1k���ļ�ϵͳʹ��
#define MEM_MAX_SIZE			    ((STM32_FLASH_SIZE<256?(10*1024):(37*1024))*\
																MEM_BLOCK_SIZE/(MEM_BLOCK_SIZE+2))  						        //�������ڴ� 2K
#define MEM_ALLOC_TABLE_SIZE	((MEM_MAX_SIZE/MEM_BLOCK_SIZE)&(~3)) //�ڴ����С,����Ϊż����˫�ֶ���
		 
//�ڴ����������
struct _m_mallco_dev
{
	void (*init)(void);					//��ʼ��
	u8   (*perused)(void);		  	    	//�ڴ�ʹ����
	u8 	  *membase;					//�ڴ��
	u16   *memmap; 					//�ڴ����״̬��
	u8     memrdy; 						//�ڴ�����Ƿ����
};

extern struct _m_mallco_dev mallco_dev;	 //��mallco.c���涨��

void mymemset(void *s,u8 c,u32 count);	 //�����ڴ�
void mymemcpy(void *des,void *src,u32 n);//�����ڴ� 

void mem_init(void);					 //�ڴ������ʼ������
u32 mem_malloc(u32 size);		 //�ڴ����
u8 mem_free(u32 offset);		 //�ڴ��ͷ�
u8 mem_perused(void);				 //����ڴ�ʹ���� 
////////////////////////////////////////////////////////////////////////////////
//�û����ú���

//�ڴ������ʼ��
void os_mem_init (void);

void myfree(void *ptr);  			//�ڴ��ͷ�
void *mymalloc(u32 size);			//�ڴ����
void *myrealloc(void *ptr,u32 size);//���·����ڴ�



		
#ifdef __cplusplus
	}
#endif

	
		
#endif
