#include "includes.h"
#include "mmc_sd.h"
#include "fats.h"


/***********************************************

			1.并不是所有温控集中器都支持文件系统，在FLASH空间大于等于256KB的芯片上可以下载文件系统
			2.要支持文件系统还需要下载文件系统相关的代码，本文件只是提供了相关的函数调用接口
			3.由于不能保证芯片中一定有文件系统相关代码，使用本文件提供的函数接口之前一定要判断文件系统存在！

**********************************************/



FATFS *fatfs[2];  // Work area (file system object) for logical drive	 
FIL *file;
FIL *ftemp;	 

UINT file_br,file_bw;
FILINFO fileinfo;
DIR dir;


u8 *file_buf;//SD卡数据缓存区



u8 FATS_STATE=0;//0，不支持文件系统，1，支持文件系统

		//文件系统状态，用于调试输出，0，不支持文件系统，1支持文件系统但是没有SD卡，2有SD卡但是挂载失败，3支持文件系统,,
u8 DBG_FATS=0;



				//是否支持文件系统，0，不支持，1，支持
u8 fats_state (void)
{
	return FATS_STATE;
}



//fats初始化
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
		return FR_NOT_READY;//地址不正常，返回错误
	}
	
	a=SD_Initialize();
	if (a!=0)//SD卡初始化失败
	{
		DBG_FATS=1;
		return FR_NOT_READY;
	}
	
	a=FATS->fun_init(&f);

	
	
	u8 ret=0;
	if (a==0xffffffff)//FATFS初始化失败
	{	
		DBG_FATS=2;
		return FR_NOT_READY;
	}
		
	
	
	
	
	fatfs[0]=(FATFS*)mymalloc(sizeof(FATFS));	//为磁盘0工作区申请内存	
	fatfs[1]=(FATFS*)mymalloc(sizeof(FATFS));	//为磁盘1工作区申请内存
	file=(FIL*)mymalloc(sizeof(FIL));		//为file申请内存
	ftemp=(FIL*)mymalloc(sizeof(FIL));		//为ftemp申请内存
	file_buf=(u8*)mymalloc(512);				//为fatbuf申请内存
	
	
	if (!(fatfs[0]&&fatfs[1]&&file&&ftemp&&file_buf))//失败
	{
		myfree(fatfs[0]);
		myfree(fatfs[1]);
		myfree(file);
		myfree(ftemp);
		myfree(file_buf);
		return FR_NOT_READY;
		
	}

	ret=FATS->f_mount(0,fatfs[0]);
	
	if (ret)//有错误产生
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














