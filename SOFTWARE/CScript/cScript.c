
#include "includes.h"
#include "cScript.h"


//脚本运行的变量栈
cscript_stack cScriptStack={0};

//找到一个分号结尾的字符串并复制
char* findSemicolon (char *outbuff,char *inbuff);

//判定这个条目的类型
u32 checkCategory (char *str);

//添加变量
int addVariable (char *name,void *value,u32 type);

//修改变量
int cheVariable (char *name,void *value);

//获取变量
u32 getVariable (char *name);

//计算
u32 alculation (char *buf);




//找到一个分号结尾的字符串并复制
char* findSemicolon (char *outbuff,char *inbuff)
{
	int strlength=0;
	
	while(*inbuff)		//分号之后的非字母字符全部忽略
	{
		if ((*inbuff>='a'&&*inbuff<='z')||(*inbuff>='A'&&*inbuff<='Z')||*inbuff=='_')
		{
			break;
		}
		else if (*inbuff>='0'&&*inbuff<='9')
		{
			break;
		}
		else
		{
			inbuff++;
		}
	}
	
	strlength=strlenByChar(';',inbuff);
	mymemcpy(outbuff,inbuff,strlength);
	outbuff[strlength]=0;
	inbuff=inbuff+strlength+1;//跳过分号
	return inbuff;
}


//添加变量
int addVariable (char *name,void *value,u32 type)
{
	cscript_variable a={0};
	mymemcpy( a.varName,name,strlen(name));
	a.varValue=value;
	a.varType=type;
	mymemcpy(&((cscript_variable*)cScriptStack.stack_base)[cScriptStack.offset],&a,sizeof(a));
	cScriptStack.offset++;
	return 0;
}


//找到对应的变量并修改，成功返回1，失败返回0
int cheVariable (char *name,void *value)
{
	cscript_variable a={0};
	u16 offset=cScriptStack.offset;
	while(offset--)
	{
		mymemcpy(&a,&((cscript_variable*)cScriptStack.stack_base)[offset],sizeof(a));//取值
		if (samestr((u8*)name,(u8*)a.varName))
		{
			a.varValue=value;					//修改
			mymemcpy(&((cscript_variable*)cScriptStack.stack_base)[offset],&a,sizeof(a));//写入
			return 1;
		}
	}
	return 0;
}


//找指定变量，有返回1，无返回0
u32 findVariable (char *name)
{
	cscript_variable a={0};
	u16 offset=cScriptStack.offset;
	u32 ret=0;
	while(offset--)
	{
		mymemcpy(&a,&((cscript_variable*)cScriptStack.stack_base)[offset],sizeof(a));
		if (samestr((u8*)name,(u8*)a.varName))
		{
			ret=1;
			break;
		}
	}
	return ret;

}






//获取变量
u32 getVariable (char *name)
{
	cscript_variable a={0};
	u16 offset=cScriptStack.offset;
	u32 ret=0;
	while(offset--)
	{
		mymemcpy(&a,&((cscript_variable*)cScriptStack.stack_base)[offset],sizeof(a));
		if (samestr((u8*)name,(u8*)a.varName))
		{
			ret=(u32)a.varValue;
			break;
		}
	}
	return ret;
	
}

//计算
u32 alculation (char *buf)
{
	u16 len=0;
	u32 ret1=0;
	u32 ret2=0;
	char par1[16]={0};
	char par2[16]={0};
	if (len=strlenByChar('+',buf),len)
	{
		mymemcpy(par1,buf,len);
		mymemcpy(par2,buf+len+1,strlen(buf+len+1));
		ret1=checkCategory(par1);
		ret2=checkCategory(par2);
		return ret1+ret2;
	}
	else if (len=strlenByChar('-',buf),len)
	{
		mymemcpy(par1,buf,len);
		mymemcpy(par2,buf+len+1,strlen(buf+len+1));
		ret1=checkCategory(par1);
		ret2=checkCategory(par2);
		return ret1-ret2;
	}
	else if (len=strlenByChar('*',buf),len)
	{
		mymemcpy(par1,buf,len);
		mymemcpy(par2,buf+len+1,strlen(buf+len+1));
		ret1=checkCategory(par1);
		ret2=checkCategory(par2);
		return ret1*ret2;
	}
	else if (len=strlenByChar('/',buf),len)
	{
		mymemcpy(par1,buf,len);
		mymemcpy(par2,buf+len+1,strlen(buf+len+1));
		ret1=checkCategory(par1);
		ret2=checkCategory(par2);
		return ret1/ret2;
	}
	else if (len=strlenByChar('%',buf),len)
	{
		mymemcpy(par1,buf,len);
		mymemcpy(par2,buf+len+1,strlen(buf+len+1));
		ret1=checkCategory(par1);
		ret2=checkCategory(par2);
		return ret1%ret2;
	}
	else if (len=strlenByChar('|',buf),len)
	{
		mymemcpy(par1,buf,len);
		mymemcpy(par2,buf+len+1,strlen(buf+len+1));
		ret1=checkCategory(par1);
		ret2=checkCategory(par2);
		return ret1|ret2;
	}
	else if (len=strlenByChar('&',buf),len)
	{
		mymemcpy(par1,buf,len);
		mymemcpy(par2,buf+len+1,strlen(buf+len+1));
		ret1=checkCategory(par1);
		ret2=checkCategory(par2);
		return ret1&ret2;
	}
	else if (len=strlenByChar('^',buf),len)
	{
		mymemcpy(par1,buf,len);
		mymemcpy(par2,buf+len+1,strlen(buf+len+1));
		ret1=checkCategory(par1);
		ret2=checkCategory(par2);
		return ret1^ret2;
	}
	else
	{
		//return checkCategory(buf);//没有运算符号，直接执行
	}
	return 0;
}




//判定这个条目的类型并执行
u32 checkCategory (char *str)
{
	char *str_par=str;
	char name[16]={0};
	int len=0;
	u32 ret=0;
	u8 str_type=0;
	if (samestr((u8*)"local ",(u8*)str))							//定义变量
	{
		str_par=str+6;
		len=strlenByChar('=',str_par);
		mymemcpy(name,str_par,len);
		str_par+=len+1;//跳过等号
		if (checkStrType(str_par)==strTypeStr)
		{
			addVariable(name,str_par,varTypeStr);//这里如果str指向的内存被释放，会出现bug
		}
		else if (checkStrType(str_par)==strTypeNum)
		{
			addVariable(name,(void *)(u32)str2num((u8 *)str_par),varTypeNum);
		}
		else if ((checkStrType(str_par)==strTypeHex))
		{
			addVariable(name,(void *)str2hex(str_par),varTypeNum);
		}
		ret=getVariable(name);
	}																								
																										//字符串数据类型
	else if (str_type=checkStrType(str_par),str_type!=strTypeErr)							
	{
		if (str_type==strTypeFun)																//运行函数
		{
			ret=runFunction(str_par);
		}
		else if (str_type==strTypeNum)							//是个数字
		{
			ret=str2num((u8*)str_par);
		}
		else if (str_type==strTypeHex)							//hex数字
		{
			ret=str2hex(str_par);
		}
		else if (str_type==strTypeStr)
		{
		}
		else if (str_type==strTypeVar)
		{
			ret=getVariable(str_par);
		}
		else if (str_type==strTypeChar)
		{
			ret=(u32)getChar(str_par);				
		}
	}
	else if (len=strlenByChar('=',str_par),len)				//是个赋值语句
	{
		mymemcpy(name,str_par,len);
		cheVariable(name,(void *)checkCategory(str_par+len+1));
		ret=getVariable(name);
	}
	else if (findVariable(str_par))													//显示这个变量
	{
		ret=getVariable(str_par);
	}
	else																										//什么都不是，执行计算
	{
		ret=alculation(str_par);
	}
	return ret;
}








/***************************************************

			运行脚本，

***************************************************/

u32 runCScript (char *par)
{
	u32 ret=0;
	char *n_par=mymalloc(1024);
		if (n_par==0) return (u32)-1;
	cScriptStack.stack_base=mymalloc(1024);
	if (cScriptStack.stack_base==0)
	{
		myfree(n_par);
		return (u32)-2;
	}
	char *next_par=par;
	do
	{
		next_par=findSemicolon(n_par,next_par);//提取一个执行条目
		ret=checkCategory(n_par);//执行
		
	}while(*next_par);
	
	myfree(n_par);
	myfree(cScriptStack.stack_base);
	cScriptStack.stack_base=0;
	cScriptStack.offset=0;
	return ret;
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
			if (*fun_addr_str=='0')//传的是地址
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
			/*
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
			else if (strtype==strTypeVar)	
			{
				par_value[i]=(u32)getVariable(par_str[i]);
			}
			else
			{
				myfree(my_par);
				return 0;				
			}
			*/
			par_value[i]=checkCategory(par_str[i]);//校验数据类型
		}
	}
	
	ret=BlxExternFun(par_value[0],par_value[1],par_value[2],par_value[3],fun_addr|1);
	
	myfree(my_par);
	return ret;
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
		u8 i=0;
		for (i=0;*(str+i);i++)
		{
			if (!((*(str+i)>='a'&&*(str+i)<='z')||(*(str+i)>='A'&&*(str+i)<='Z')||*(str+i)=='_'))
			{
				if (*(str+i)=='(')
					break;
				else if (*(str+i)=='=')
					return strTypeErr;
				else
					return strTypeErr;
			}
		}
		if (str[strlen(str)-1]!=')')
		{
			if (i==strlen(str))
				return strTypeVar;
			else
				return strTypeErr;
		}
		else
			return strTypeFun; 
	}
}





