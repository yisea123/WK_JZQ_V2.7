#ifndef MY_FUN_H_
#define MY_FUN_H_

#ifdef  __cplusplus
	extern "C" {
#endif
	


#define htons(s) __REV16(s)					//16λ��С��ת��
#define ntohs(s) __REV16(s)					//16λ��С��ת��


//��Ƭ����SN��
#define MCU_SN ((u8*)0x1FFFF7E8)
		
		

//�������ָ�벻Ϊ0�����øú���
#define BX_VOID_FUN(fun) do{\
															if (fun){\
																fun();}\
														}while(0);
																
		
		
		
		
		
//�Ƚ��ַ�������ͬΪ1����ͬΪ0
u8 samestr(u8 *str1,u8 *str2);
		
//�ַ���תΪ��������
u16 str2num(u8 *str);
		
//��ȡ�ַ����е����֣��÷ָ���point�ֿ�,��length������
u8 getnumfstr(u8 *out,u8 *strin,u8 point,u8 length);

u8 str2nums (u8 *out,char *strin,char point);

//hex��ʽ���ַ���תΪ���� ,Сд
u32 str2hex (char *hexstr);
														
														
//��������
void copy_data (u8 *from,u8 *to,u16 length);


enum
{
	strTypeErr=0,
	strTypeChar=1,
	strTypeStr=2,
	strTypeNum=3,
	strTypeHex=4,
};


//У��str������
u8 checkStrType (char *str);




u32 getMemValue (u8 *buff,u32 addr,u16 len);
														
u32 setMemValue (u8 *buff,u32 addr,u16 len);

u32 setMemU32 (u32 value,u32 addr);
	
u32 getMemU32 (u32 addr);

//����ָ������,����������4������
u32 runFunction (u32 fun_addr,char *Parameters);


#ifdef  __cplusplus
	}
#endif


#endif


