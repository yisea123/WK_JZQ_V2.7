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
u16 str2num(u8 *str)
{
	u16 ret=0;
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





//复制数据
void copy_data (u8 *from,u8 *to,u16 length)
{
	while(length--)
	{
		*to++=*from++;
	}
}







