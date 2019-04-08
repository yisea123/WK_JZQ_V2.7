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
		
//复制数据
void copy_data (u8 *from,u8 *to,u16 length);
		


#ifdef  __cplusplus
	}
#endif


#endif


