#include "includes.h"
#include "my_fun.h"


//比较两字符串值，相同返回1，不同返回0
u8 samestr(u8 *str1,u8 *str2)
{
	u16 i=strlen((const char *)str1);
	while(i--)
	{
		if ((*str1)!=(*str2))
			return 0;
		str1++;str2++;
	}
	return 1;
}


//字符串转换为整形
u32 str2num(u8 *str)
{
	u32 ret=0;
	while(*str)
	{
		if((*str>='0')&&(*str<='9'))
		{
			ret*=10;
			ret+=*str-'0';
		}
		str++;
	}
	return ret;
}



//提取字符串中的数字，用分隔符point分开,有length个数字
u8 getnumfstr(u8 *out,u8 *strin,u8 point,u8 length)
{
	u8 buff[10]={0};
	u8 i=0;
	while (length--)
	{
		while ((*strin!=point)&&(*strin!=0))
		{
			buff[i++]=*strin++;
		}
		buff[i]=0;
		*out++=str2num(buff);
		i=0;strin++;			
	}
	return 0;
}

//提取字符串中的数字，用分隔符point分开,直到结尾
u8 str2nums (u8 *out,char *strin,char point)
{
	u8 buff[10]={0};
	u8 i=0;
	u8 length=1;
	u8 ret=length;
	char *in=strin;
	while(*in)
	{
		if ((*in==point)&&(*(in+1))) length++;
		in++;
	}
	ret=length;
	while (length--)
	{
		while ((*strin!=point)&&(*strin!=0))
		{
			buff[i++]=*strin++;
		}
		buff[i]=0;
		*out++=str2num(buff);
		i=0;strin++;			
	}
	return ret;

}



//hex格式的字符串转为整形 ,小写
u32 str2hex (char *hexstr)
{
	u32 value=0;
	char *base=hexstr;
	while (*base)
	{
		if (*base>='A'&&*base<'a')
		{
			*base+='a'-'A';
		}
		base++;
	}
	if (samestr((u8*)"0x",(u8*)hexstr))
	{
		hexstr+=2;
		if (strlen(hexstr)<=8)
		{
			while(*hexstr)
			{
				value<<=4;
				if (*hexstr<'a')
				{
					value|=*hexstr-'0';
				}
				else
				{
					value|=*hexstr-'a'+10;
				}
				hexstr++;
			}
			return value;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}



//去掉字符串的双引号，不支持转义字符
char *getStr (char *str)
{
	str[strlen(str)-1]=0;
	return str+1;
}

//提取单引号包含的字符
char getChar (char *str)
{
	return *(str+1);
}


//找到以a结尾的字符串长度
u16 strlenByChar (char a,char *inbuff)
{
	u16 strlength=0;
	while (*(inbuff+strlength))
	{
		if (*(inbuff+strlength)!=a)
		{
			strlength++;
		}
		else
		{
			return strlength;
		}
	}
	return 0;
}





//复制数据
void copy_data (u8 *from,u8 *to,u16 length)
{
	while(length--)
	{
		*to++=*from++;
	}
}


//获取指定内存地址的值0,成功，1，失败
u32 getMemValue (u8 *buff,u32 addr,u16 len)
{
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	u8 *base_addr=(u8 *)addr;
	if (base_addr==0) return 1;
	OS_ENTER_CRITICAL();
	while(len--)
	{
		*buff++=*base_addr++;
	}
	OS_EXIT_CRITICAL();
	return 0;
}



u32 setMemValue (u8 *buff,u32 addr,u16 len)
{
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	u8 *base_addr=(u8 *)addr;
	if (base_addr==0) return 1;
	OS_ENTER_CRITICAL();
	while(len--)
	{
		*base_addr++=*buff++;
	}
	OS_EXIT_CRITICAL();
	return 0;
}


u32 setMemU32 (u32 value,u32 addr)
{
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	u8 *base_addr=(u8 *)addr;
	if (base_addr==0) return 1;
	if ((u32)base_addr&0x3) return 1;
	OS_ENTER_CRITICAL();
	*base_addr=value;
	OS_EXIT_CRITICAL();
	return 0;
}


u32 getMemU32 (u32 addr)
{
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	u32 value=0;
	u32 *base_addr=(u32 *)addr;
	if (base_addr==0) return 1;
	if ((u32)base_addr&0x3) return 1;
	OS_ENTER_CRITICAL();
	value=*base_addr;
	OS_EXIT_CRITICAL();
	return value;
}












