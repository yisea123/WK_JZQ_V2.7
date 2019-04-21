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


//校验str的类型
u8 checkStrType (char *str)
{
	if (str==0) return 0;
	if (*str=='\"')
	{
		if (*(str+strlen(str)-1)=='\"')
			return strTypeStr;
		else
			return strTypeErr;
	}
	else if (*str=='\'')
	{
		if (*(str+2)=='\'')
		{
			return strTypeChar;
		}
		else
		{
			return strTypeErr;
		}
	}
	else if (samestr((u8 *)"0x",(u8*)str))
	{
		for (u8 i=0;*(str+i);i++)
		{
			if ((*(str+i)<'0')||((*(str+i)>'9')&&(*(str+i)<'A'))||((*(str+i)>'F')&&(*(str+i)<'a'))
				||(*(str+i)>'f'))
			{
				if (str[strlen(str)-1]==')')
					return strTypeFun;
				else
					return strTypeErr;
			}
		}
		return strTypeHex;
	}
	else if (*str>='0'&&*str<='9')
	{
		for (u8 i=0;*(str+i);i++)
		{
			if ((*(str+i)<'0')||(*(str+i)>'9'))
			{
				return strTypeErr;
			}
		}
		return strTypeNum;
	}
	else if ((*str>='a'&&*str<='z')||(*str>='A'&&*str<='Z')||*str=='_')
	{
		for (u8 i=0;*(str+i);i++)
		{
			if (!((*str>='a'&&*str<='z')||(*str>='A'&&*str<='Z')||*str=='_'))
			{
				if (*str=='(')
					break;
				else
					return strTypeErr;
			}
		}
		if (str[strlen(str)-1]!=')')
			return strTypeErr;
		else
			return strTypeFun; 
	}
}

//查找函数名的地址
u32 findFunAddr (char *fun_name)
{
	extern fun_list FUN_LIST[];
	for (u16 i=0;FUN_LIST[i].fun_addr;i++)
	{
		if (samestr((u8*)FUN_LIST[i].fun_name,(u8*)fun_name))
		{
			return FUN_LIST[i].fun_addr;
		}
	}
	return 0;
}








//运行指定函数,最多可以设置4个参数
u32 runFunction (char *Parameters)
{
	u32 fun_addr=0;
	
	
	
	
	
	u32 par_value[4]={0};
	char *(par_str[4])={0};
	u8 par_num=0;
	char *my_par=mymalloc(256);
	
	if (my_par==0) return 0;
	char *par=my_par;
	u32 ret=0;
	if (Parameters) 
	{
		mymemcpy(my_par,Parameters,strlen(Parameters)+1);
		
		char *fun_addr_str=mymalloc(32);
		char *fun_addr_str_ptr=fun_addr_str;
		if (fun_addr_str!=0) 
		{
			mymemcpy(fun_addr_str,Parameters,32);
			while (*fun_addr_str_ptr++) 
			{
				if (*fun_addr_str_ptr=='(')
				{
					*fun_addr_str_ptr=0;
					par+=fun_addr_str_ptr-fun_addr_str+1;//使指针指向函数参数处
					break;
				}
			}
			if (*par=='0')//传的是地址
			{
				fun_addr=str2hex(fun_addr_str);
			}
			else			//传的函数名
			{
				fun_addr=findFunAddr(fun_addr_str);
			}
		}
		myfree(fun_addr_str);
		if (fun_addr==0) 
		{	
			myfree(my_par);
			return 0;
		}
		
		if (par[strlen(par)-1]==')')
		{
			par[strlen(par)-1]=0;//去掉反括号
		}
		else
		{
			myfree(my_par);
			return 0;
		}
		
		if (*par)
			par_num++;
		par_str[par_num-1]=par;
		u8 strtype=0;
		
		//区分函数参数
		while (*par)
		{
			if (*par==',')
			{
				*par=0;
				par_num++;
				par_str[par_num-1]=par+1;
			}
			par++;
		}
		if (par_num>4) 
		{
			myfree(my_par);
			return 0;
		}
		
		//给每个参数赋值
		for (u8 i=0;i<par_num;i++)
		{
			strtype=checkStrType(par_str[i]);
			if (strtype==strTypeChar)
			{
				par_value[i]=(u32)getChar( par_str[i]);
			}
			else if (strtype==strTypeHex)
			{
				par_value[i]=str2hex(par_str[i]);
			}
			else if (strtype==strTypeNum)
			{
				par_value[i]=str2num((u8*)par_str[i]);
			}
			else if (strtype==strTypeStr)
			{
				par_value[i]=(u32)getStr( par_str[i]);
			}
			else if (strtype==strTypeFun)	
			{
				par_value[i]=(u32)runFunction( par_str[i]);
			}
			else
			{
				myfree(my_par);
				return 0;				
			}
		}
	}
	
	ret=BlxExternFun(par_value[0],par_value[1],par_value[2],par_value[3],fun_addr|1);
	
	myfree(my_par);
	return ret;
}





