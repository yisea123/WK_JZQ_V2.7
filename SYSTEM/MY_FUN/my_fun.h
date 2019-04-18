#ifndef MY_FUN_H_
#define MY_FUN_H_

#ifdef  __cplusplus
	extern "C" {
#endif
	


#define htons(s) __REV16(s)					//16位大小端转换
#define ntohs(s) __REV16(s)					//16位大小端转换


//单片机的SN码
#define MCU_SN ((u8*)0x1FFFF7E8)
		
		

//如果函数指针不为0，调用该函数
#define BX_VOID_FUN(fun) do{\
															if (fun){\
																fun();}\
														}while(0);
																
		
		
		
		
		
//比较字符串，相同为1，不同为0
u8 samestr(u8 *str1,u8 *str2);
		
//字符串转为整形数字
u16 str2num(u8 *str);
		
//提取字符串中的数字，用分隔符point分开,有length个数字
u8 getnumfstr(u8 *out,u8 *strin,u8 point,u8 length);

u8 str2nums (u8 *out,char *strin,char point);

//hex格式的字符串转为整形 ,小写
u32 str2hex (char *hexstr);
														
														
//复制数据
void copy_data (u8 *from,u8 *to,u16 length);


enum
{
	strTypeErr=0,
	strTypeChar=1,
	strTypeStr=2,
	strTypeNum=3,
	strTypeHex=4,
};


//校验str的类型
u8 checkStrType (char *str);




u32 getMemValue (u8 *buff,u32 addr,u16 len);
														
u32 setMemValue (u8 *buff,u32 addr,u16 len);

u32 setMemU32 (u32 value,u32 addr);
	
u32 getMemU32 (u32 addr);

//运行指定函数,最多可以设置4个参数
u32 runFunction (u32 fun_addr,char *Parameters);


#ifdef  __cplusplus
	}
#endif


#endif


