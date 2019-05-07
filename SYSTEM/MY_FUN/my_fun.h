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
																
		
		
/**************************************************************/		
									/*字符串处理相关函数*/
		
//比较字符串，相同为1，不同为0
u8 samestr(u8 *str1,u8 *str2);
		
//字符串转为整形数字
u32 str2num(u8 *str);
		
//hex格式的字符串转为整形 ,小写
u32 str2hex (char *hexstr);
														
//提取字符串中的数字，用分隔符point分开,有length个数字
u8 getnumfstr(u8 *out,u8 *strin,u8 point,u8 length);

//提取字符串中的数字，														
u8 str2nums (u8 *out,char *strin,char point);
														
//去掉字符串的双引号，不支持转义字符
char *getStr (char *str);
	
//提取单引号包含的字符
char getChar (char *str);
	
//统计以a结尾的字符串长度
u16 strlenByChar (char a,char *inbuff);
	
//找到第一次出现指定字符串的位置
u16 findStr (char *str,char *tofind);

//返回一对闭合字符串的长度
u16 findPair (char left,char right,char *str);

									/*字符串处理函数End*/
/**************************************************************/		





/********************内存操作函数************************/

//指定长度的内存复制
void copy_data (u8 *from,u8 *to,u16 length);

//获取指定内存地址的数据
u32 getMemValue (u8 *buff,u32 addr,u16 len);
		
//把指定内存地址的数据设为指定值		
u32 setMemValue (u8 *buff,u32 addr,u16 len);

//设置指定32位地址的值为value
u32 setMemU32 (u32 value,u32 addr);

//返回指定32位地址的值
u32 getMemU32 (u32 addr);

/*****************内存操作函数End*******************/







#ifdef  __cplusplus
	}
#endif


#endif


