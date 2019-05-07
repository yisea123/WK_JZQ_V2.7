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
																
		
		
/**************************************************************/		
									/*�ַ���������غ���*/
		
//�Ƚ��ַ�������ͬΪ1����ͬΪ0
u8 samestr(u8 *str1,u8 *str2);
		
//�ַ���תΪ��������
u32 str2num(u8 *str);
		
//hex��ʽ���ַ���תΪ���� ,Сд
u32 str2hex (char *hexstr);
														
//��ȡ�ַ����е����֣��÷ָ���point�ֿ�,��length������
u8 getnumfstr(u8 *out,u8 *strin,u8 point,u8 length);

//��ȡ�ַ����е����֣�														
u8 str2nums (u8 *out,char *strin,char point);
														
//ȥ���ַ�����˫���ţ���֧��ת���ַ�
char *getStr (char *str);
	
//��ȡ�����Ű������ַ�
char getChar (char *str);
	
//ͳ����a��β���ַ�������
u16 strlenByChar (char a,char *inbuff);
	
//�ҵ���һ�γ���ָ���ַ�����λ��
u16 findStr (char *str,char *tofind);

//����һ�Ապ��ַ����ĳ���
u16 findPair (char left,char right,char *str);

									/*�ַ���������End*/
/**************************************************************/		





/********************�ڴ��������************************/

//ָ�����ȵ��ڴ渴��
void copy_data (u8 *from,u8 *to,u16 length);

//��ȡָ���ڴ��ַ������
u32 getMemValue (u8 *buff,u32 addr,u16 len);
		
//��ָ���ڴ��ַ��������Ϊָ��ֵ		
u32 setMemValue (u8 *buff,u32 addr,u16 len);

//����ָ��32λ��ַ��ֵΪvalue
u32 setMemU32 (u32 value,u32 addr);

//����ָ��32λ��ַ��ֵ
u32 getMemU32 (u32 addr);

/*****************�ڴ��������End*******************/







#ifdef  __cplusplus
	}
#endif


#endif


