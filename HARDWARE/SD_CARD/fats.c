#include "includes.h"
#include "mmc_sd.h"
#include "fats.h"


/***********************************************

			1.�����������¿ؼ�������֧���ļ�ϵͳ����FLASH�ռ���ڵ���256KB��оƬ�Ͽ��������ļ�ϵͳ
			2.Ҫ֧���ļ�ϵͳ����Ҫ�����ļ�ϵͳ��صĴ��룬���ļ�ֻ���ṩ����صĺ������ýӿ�
			3.���ڲ��ܱ�֤оƬ��һ�����ļ�ϵͳ��ش��룬ʹ�ñ��ļ��ṩ�ĺ����ӿ�֮ǰһ��Ҫ�ж��ļ�ϵͳ���ڣ�

**********************************************/



FATFS *fatfs[2];  // Work area (file system object) for logical drive	 
FIL *file;
FIL *ftemp;	 

UINT file_br,file_bw;
FILINFO fileinfo;
DIR dir;


u8 *file_buf;//SD�����ݻ�����



u8 FATS_STATE=0;//0����֧���ļ�ϵͳ��1��֧���ļ�ϵͳ

		//�ļ�ϵͳ״̬�����ڵ��������0����֧���ļ�ϵͳ��1֧���ļ�ϵͳ����û��SD����2��SD�����ǹ���ʧ�ܣ�3֧���ļ�ϵͳ,,
u8 DBG_FATS=0;



				//�Ƿ�֧���ļ�ϵͳ��0����֧�֣�1��֧��
u8 fats_state (void)
{
	return FATS_STATE;
}



//fats��ʼ��
u8 fats_init (void)
{
	u32 a=0;
	function f;
	f.flash_read=0;
	f.flash_write=0;
	f.myfree=myfree;
	f.mymalloc=mymalloc;
	f.sd_GetSectorCount=SD_GetSectorCount;
	f.sd_Initialize=SD_Initialize;
	f.sd_read=SD_ReadDisk;
	f.sd_Reset=SD_Reset;
	f.sd_WaitReady=SD_WaitReady;
	f.sd_write=SD_WriteDisk;
	
	
	if (((*(u32*)(FATS))<0x08000000u)||((*(u32*)(FATS))>(0x08000000u+256*1024)))
	{
		DBG_FATS=0;
		return FR_NOT_READY;//��ַ�����������ش���
	}
	
	a=SD_Initialize();
	if (a!=0)//SD����ʼ��ʧ��
	{
		DBG_FATS=1;
		return FR_NOT_READY;
	}
	
	a=FATS->fun_init(&f);

	
	
	u8 ret=0;
	if (a==0xffffffff)//FATFS��ʼ��ʧ��
	{	
		DBG_FATS=2;
		return FR_NOT_READY;
	}
		
	
	
	
	
	fatfs[0]=(FATFS*)mymalloc(sizeof(FATFS));	//Ϊ����0�����������ڴ�	
	fatfs[1]=(FATFS*)mymalloc(sizeof(FATFS));	//Ϊ����1�����������ڴ�
	file=(FIL*)mymalloc(sizeof(FIL));		//Ϊfile�����ڴ�
	ftemp=(FIL*)mymalloc(sizeof(FIL));		//Ϊftemp�����ڴ�
	file_buf=(u8*)mymalloc(512);				//Ϊfatbuf�����ڴ�
	
	
	if (!(fatfs[0]&&fatfs[1]&&file&&ftemp&&file_buf))//ʧ��
	{
		myfree(fatfs[0]);
		myfree(fatfs[1]);
		myfree(file);
		myfree(ftemp);
		myfree(file_buf);
		return FR_NOT_READY;
		
	}

	ret=FATS->f_mount(0,fatfs[0]);
	
	if (ret)//�д������
	{
		DBG_FATS=2;
		return ret;	
	}
	DBG_FATS=3;
	FATS_STATE=1;
	return ret;	

}





FRESULT my_f_mount (BYTE a, FATFS*b)						/* Mount/Unmount a logical drive */
{
	if (fats_state())
	{
		return FATS->f_mount (a,b);
	}
	else
	{
		return FR_DISK_ERR;
	}
}


FRESULT my_f_open (FIL* a, const TCHAR* b, BYTE c)			/* Open or create a file */
{
	if (fats_state())
	{
		return FATS->f_open (a,b,c);
	}
	else
	{
		return FR_DISK_ERR;
	}
}
FRESULT my_f_read (FIL* a, void* b, UINT c, UINT* d)			/* Read data from a file */
{
	if (fats_state())
	{
		return FATS->f_read (a,b,c,d);
	}
	else
	{
		return FR_DISK_ERR;
	}
}
FRESULT my_f_lseek (FIL* a, DWORD b)						/* Move file pointer of a file object */
{
	if (fats_state())
	{
		return FATS->f_lseek (a,b);
	}
	else
	{
		return FR_DISK_ERR;
	}
}
FRESULT my_f_close (FIL* a)								/* Close an open file object */
{
	if (fats_state())
	{
		return FATS->f_close (a);
	}
	else
	{
		return FR_DISK_ERR;
	}
}
FRESULT my_f_opendir (DIR* a, const TCHAR* b)				/* Open an existing directory */
{
	if (fats_state())
	{
		return FATS->f_opendir (a,b);
	}
	else
	{
		return FR_DISK_ERR;
	}
}
FRESULT my_f_readdir (DIR* a, FILINFO* b)					/* Read a directory item */
{
	if (fats_state())
	{
		return FATS->f_readdir (a,b);
	}
	else
	{
		return FR_DISK_ERR;
	}
}
FRESULT my_f_stat (const TCHAR* a, FILINFO* b)			/* Get file status */
{
	if (fats_state())
	{
		return FATS->f_stat (a,b);
	}
	else
	{
		return FR_DISK_ERR;
	}
}
FRESULT my_f_write (FIL* a, const void* b, UINT c, UINT* d)	/* Write data to a file */
{
	if (fats_state())
	{
		return FATS->f_write (a,b,c,d);
	}
	else
	{
		return FR_DISK_ERR;
	}
}
FRESULT my_f_getfree (const TCHAR* a, DWORD* b, FATFS** c)	/* Get number of free clusters on the drive */
{
	if (fats_state())
	{
		return FATS->f_getfree (a,b,c);
	}
	else
	{
		return FR_DISK_ERR;
	}
}
FRESULT my_f_truncate (FIL* a)							/* Truncate file */
{
	if (fats_state())
	{
		return FATS->f_truncate (a);
	}
	else
	{
		return FR_DISK_ERR;
	}
}
FRESULT my_f_sync (FIL* a)								/* Flush cached data of a writing file */
{
	if (fats_state())
	{
		return FATS->f_sync (a);
	}
	else
	{
		return FR_DISK_ERR;
	}
}
FRESULT my_f_unlink (const TCHAR* a)					/* Delete an existing file or directory */
{
	if (fats_state())
	{
		return FATS->f_unlink (a);
	}
	else
	{
		return FR_DISK_ERR;
	}
}
FRESULT	my_f_mkdir (const TCHAR* a)						/* Create a new directory */
{
	if (fats_state())
	{
		return FATS->f_mkdir (a);
	}
	else
	{
		return FR_DISK_ERR;
	}
}

FRESULT my_f_chmod (const TCHAR* a, BYTE b, BYTE c)			/* Change attriburte of the file/dir */
{
	if (fats_state())
	{
		return FATS->f_chmod (a,b,c);
	}
	else
	{
		return FR_DISK_ERR;
	}
}
FRESULT my_f_utime (const TCHAR* a, const FILINFO* b)		/* Change timestamp of the file/dir */
{
	if (fats_state())
	{
		return FATS->f_utime (a,b);
	}
	else
	{
		return FR_DISK_ERR;
	}
}
FRESULT my_f_rename (const TCHAR* a, const TCHAR* b)		/* Rename/Move a file or directory */
{
	if (fats_state())
	{
		return FATS->f_rename (a,b);
	}
	else
	{
		return FR_DISK_ERR;
	}
}
FRESULT my_f_chdrive (BYTE a)							/* Change current drive */
{
	if (fats_state())
	{
		return FATS->f_chdrive (a);
	}
	else
	{
		return FR_DISK_ERR;
	}
}
FRESULT my_f_chdir (const TCHAR* a)						/* Change current directory */
{
	if (fats_state())
	{
		return FATS->f_chdir (a);
	}
	else
	{
		return FR_DISK_ERR;
	}
}

FRESULT my_f_getcwd (TCHAR* a, UINT b)					/* Get current directory */
{
	if (fats_state())
	{
		return FATS->f_getcwd (a,b);
	}
	else
	{
		return FR_DISK_ERR;
	}
}
FRESULT my_f_forward (FIL* a, UINT(* b)(const BYTE*,UINT), UINT c, UINT* d)	/* Forward data to the stream */
{
	if (fats_state())
	{
		return FATS->f_forward (a,b,c,d);
	}
	else
	{
		return FR_DISK_ERR;
	}
}
FRESULT my_f_mkfs (BYTE a, BYTE b, UINT c)					/* Create a file system on the drive */
{
	if (fats_state())
	{
		return FATS->f_mkfs (a,b,c);
	}
	else
	{
		return FR_DISK_ERR;
	}
}
FRESULT	my_f_fdisk (BYTE a, const DWORD b[] , void* c)		/* Divide a physical drive into some partitions */
{
	if (fats_state())
	{
		return FATS->f_fdisk (a,b,c);
	}
	else
	{
		return FR_DISK_ERR;
	}
}
int my_f_putc (TCHAR a, FIL* b)							/* Put a character to the file */
{
	if (fats_state())
	{
		return FATS->f_putc (a,b);
	}
	else
	{
		return -1;
	}
}

int my_f_puts (const TCHAR* a, FIL* b)					/* Put a string to the file */
{
	if (fats_state())
	{
		return FATS->f_puts (a,b);
	}
	else
	{
		return -1;
	}
}

/*
int my_f_printf (FIL* a, const TCHAR* b, ...)				// Put a formatted string to the file 
{
	if (fats_state())
	{
		return FATS->f_printf (a,b, ...);
	}
	else
	{
		return -1;
	}
}*/


#define my_f_printf(fil,fmt,...) FATS->f_printf( fil,fmt, ##__VA_ARGS__)


TCHAR* my_f_gets (TCHAR* a, int b, FIL* c)					/* Get a string from the file */
{
	if (fats_state())
	{
		return FATS->f_gets (a,b,c);
	}
	else
	{
		return 0;
	}
}














