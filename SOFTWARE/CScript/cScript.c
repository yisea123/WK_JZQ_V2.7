
#include "includes.h"
#include "cScript.h"



/*****************************************************************

				自定义脚本解析器
				2019.4.23
				作者：冉川
				声明字符串变量支持的长度最大为31字节
				声明数组变量支持最大长度为31字节
				函数可以传递临时数组，最多支持一个
				运算式不支持括号重设优先级
				不支持i++，i--运算
				脚本解析器同时只能由一个线程运行，
				关键字：
					local 定义变量
					while while循环
					if else 条件分支
				语法：
				每个语句以分号;结尾
				while 循环 if 分支 都要用大括号{}包括并在右括号后加分号
				有else的陈if语句只在else分支的右括号加分号
				变量声明必须有初始值，根据初始值来确定数据类型
				声明字符串 local a="abcd";
				声明数组 local b=(1,2,3,4,5);
				声明整数 local c=520;
				声明用函数返回值作为初始值的整数 local d=fun();
				while 循环
				while (...)
				{
					...;
					...;
					...;
				};
				if 条件语句
				if (...)
				{
					...;
					...;
					...;
				}
				else
				{
					...;
					...;
					...;
				};
				或
				if (...)
				{
					...;
					...;
					...;
				};

********************************************************************/



//脚本运行的变量栈
cscript_stack cScriptStack={0};

static	u8 *Arry=0;//临时数组，一个函数调用只能使用一个临时数组


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
	
	char *n_str=inbuff;
	if ((samestr((u8*)"while ",(u8*)n_str))||
			(samestr((u8*)"for ",(u8*)n_str))
			)
	{
		n_str+=findPair('{','}',n_str);
		strlength=n_str-inbuff;
	}
	else if (samestr((u8*)"if ",(u8*)n_str))
	{
		n_str+=findPair('{','}',n_str);
		if (n_str[0]==';')//大括号后有分号，没有分支
		{
		}
		else				//有分支
		{
			n_str+=findPair('{','}',n_str);
		}
		strlength=n_str-inbuff;
		
	}
	else
	{
		strlength=strlenByChar(';',inbuff);
		if (strlength>100) strlength=0;
	}
	
	
	mymemcpy(outbuff,inbuff,strlength);
	outbuff[strlength]=0;
	inbuff=inbuff+strlength+1;//跳过分号
	return inbuff;
}


//添加变量
int addVariable (char *name,void *value,u32 type)
{
	cscript_variable a={0};
	mymemcpy( a.varName,name,strlen(name)+1);
	if (type==varTypeNum)
	{
		a.varValue=value;
	}
	else if (type==varTypeStr)
	{
		mymemcpy( a.arry,value,strlen((char*)value)+1);
		//a.varValue=a.arry;
	}
	else if (type==varTypeArry)
	{
		mymemcpy( a.arry,value,32);
		//a.varValue=a.arry;
	}
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
			if (a.varType==varTypeNum)//修改
			{
				a.varValue=value;
			}
			else if (a.varType==varTypeStr)
			{
				mymemcpy( a.arry,value,strlen((char*)value)+1);
				//a.varValue=a.arry;
			}
			else if (a.varType==varTypeArry)
			{
				mymemcpy( a.arry,value,32);
				//a.varValue=a.arry;
			}
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
	cscript_variable *a=0;
	u16 offset=cScriptStack.offset;
	u32 ret=0;
	while(offset--)
	{
		a=&((cscript_variable*)cScriptStack.stack_base)[offset];
		if (samestr((u8*)name,(u8*)a->varName))
		{
			if (a->varType!=varTypeNum)
			{
				ret=(u32)a->arry;
			}
			else
			{
				ret=(u32)a->varValue;
			}
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
	if (len=strlenByChar('|',buf),len<2000)
	{
		mymemcpy(par1,buf,len);
		mymemcpy(par2,buf+len+1,strlen(buf+len+1));
		ret1=checkCategory(par1);
		ret2=checkCategory(par2);
		return ret1|ret2;
	}
	else if (len=strlenByChar('&',buf),len<2000)
	{
		mymemcpy(par1,buf,len);
		mymemcpy(par2,buf+len+1,strlen(buf+len+1));
		ret1=checkCategory(par1);
		ret2=checkCategory(par2);
		return ret1&ret2;
	}
	else if (len=strlenByChar('^',buf),len<2000)
	{
		mymemcpy(par1,buf,len);
		mymemcpy(par2,buf+len+1,strlen(buf+len+1));
		ret1=checkCategory(par1);
		ret2=checkCategory(par2);
		return ret1^ret2;
	}
	else if (len=strlenByChar('+',buf),len<2000)
	{
		mymemcpy(par1,buf,len);
		mymemcpy(par2,buf+len+1,strlen(buf+len+1));
		ret1=checkCategory(par1);
		ret2=checkCategory(par2);
		return ret1+ret2;
	}
	else if (len=strlenByChar('-',buf),len<2000)
	{
		mymemcpy(par1,buf,len);
		mymemcpy(par2,buf+len+1,strlen(buf+len+1));
		ret1=checkCategory(par1);
		ret2=checkCategory(par2);
		return ret1-ret2;
	}
	else if (len=strlenByChar('*',buf),len<2000)
	{
		mymemcpy(par1,buf,len);
		mymemcpy(par2,buf+len+1,strlen(buf+len+1));
		ret1=checkCategory(par1);
		ret2=checkCategory(par2);
		return ret1*ret2;
	}
	else if (len=strlenByChar('/',buf),len<2000)
	{
		mymemcpy(par1,buf,len);
		mymemcpy(par2,buf+len+1,strlen(buf+len+1));
		ret1=checkCategory(par1);
		ret2=checkCategory(par2);
		return ret1/ret2;
	}
	else if (len=strlenByChar('%',buf),len<2000)
	{
		mymemcpy(par1,buf,len);
		mymemcpy(par2,buf+len+1,strlen(buf+len+1));
		ret1=checkCategory(par1);
		ret2=checkCategory(par2);
		return ret1%ret2;
	}
	else
	{
		//return checkCategory(buf);//没有运算符号，直接执行
	}
	return 0;
}






//while循环
u32 loopWhile (char *str)
{
	char *n_str=str+1;//去掉左括号
	u32 ret=0;
	char loop_par[32]={0};//循环参数
	mymemcpy (loop_par,n_str,strlenByChar(')',n_str));//复制循环参数并去掉右括号
	
	n_str+=strlenByChar('{',n_str);//定位到左括号处
	n_str[findPair('{','}',n_str)-1]=0;//去掉右大括号
	n_str+=1;//去掉左大括号
	while (checkCategory(loop_par))
	{
		ret=runCScript(n_str);//重复调用
	}
	return ret;
}


//if判断,不支持else if分支
u32 judgeIf (char *str)
{
	char *n_str1=str+1;//去掉左括号
	char *n_str2=str+1;
	u16 str2_offset=0;
	u32 ret=0;
	char judge_par[32]={0};//判断参数
	mymemcpy (judge_par,n_str1,strlenByChar(')',n_str1));//复制判断参数并去掉右括号
	
	n_str1+=strlenByChar('{',n_str1);//定位到左括号处
	str2_offset=findPair('{','}',n_str1);
	n_str1[str2_offset-1]=0;//去掉右大括号
	n_str2=&n_str1[str2_offset];
	n_str1+=1;//去掉左大括号
	
	if (*n_str2)
	{
		n_str2+=strlenByChar('{',n_str2);//定位到左括号处
		n_str2[findPair('{','}',n_str2)-1]=0;//去掉右大括号
		n_str2+=1;//去掉左大括号
	}
	
	if (checkCategory(judge_par))
	{
		ret=runCScript(n_str1);//重复调用
	}
	else if (*n_str2)
	{
		ret=runCScript(n_str2);//重复调用
	}
	return ret;
	
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
		if (len>2000) len=0;
		mymemcpy(name,str_par,len);
		str_par+=len+1;//跳过等号
		u8 var_type=0;
		if (str_type=checkStrType(str_par),str_type!=strTypeErr)				
		{
			if (str_type==strTypeStr)
			{
				var_type=varTypeStr;
			}
			else if (str_type==strTypeNum)
			{
				var_type=varTypeNum;
			}
			else if (str_type==strTypeHex)
			{
				var_type=varTypeNum;
			}
			else if (str_type==strTypeFun)
			{
				var_type=varTypeNum;
			}
			else if (str_type==strTypeArry)
			{
				var_type=varTypeArry;
			}
		}
		addVariable(name,(void *)checkCategory(str_par),var_type);//这里如果str指向的内存被释放，会出现bug
		
		ret=getVariable(name);
	}					
	else if (samestr((u8*)"while ",(u8*)str))	
	{
		ret=loopWhile(str+6);
	}
	else if (samestr((u8*)"for ",(u8*)str))	
	{
	}
	else if (samestr((u8*)"if ",(u8*)str))	
	{
		ret=judgeIf (str+3);
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
			str_par=getStr(str_par);
			u8 lenstr=strlen(str_par);
			if (lenstr>32) lenstr=32;
			mymemcpy(Arry,str_par,lenstr);
			Arry[lenstr]=0;//截断字符串
			ret=(u32)Arry;
		}
		else if (str_type==strTypeVar)
		{
			ret=getVariable(str_par);
		}
		else if (str_type==strTypeChar)
		{
			ret=(u32)getChar(str_par);				
		}
		else if (str_type==strTypeArry)
		{
			str_par++;
			str_par[strlen(str_par)-1]=0;
			str2nums(Arry,str_par,',');
			ret=(u32)Arry;
		}
	}
	else if (len=strlenByChar('=',str_par),len<2000)				//是个赋值语句
	{
		if (str_par[len+1]=='=')			//下一个也是等号,判断相等
		{
			str_par[len]=0;
			ret=(checkCategory(str_par)==checkCategory(&str_par[len+2]));
		}
		else
		{
			mymemcpy(name,str_par,len);
			cheVariable(name,(void *)checkCategory(str_par+len+1));
			ret=getVariable(name);
		}
	}
	else if (len=strlenByChar('!',str_par),len<2000)				//是个赋值语句
	{
		if (str_par[len+1]=='=')			//下一个是等号,判断不相等
		{
			str_par[len]=0;
			ret=(checkCategory(str_par)!=checkCategory(&str_par[len+2]));
		}
		else
		{
			mymemcpy(name,str_par,len);
			cheVariable(name,(void *)checkCategory(str_par+len+1));
			ret=getVariable(name);
		}
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
			此为不可重入的函数，不能在多个线程中同时运行脚本

***************************************************/
static u8 NumberOfFloors=0;
 

u32 runCScript (char *par)
{
	u32 ret=0;
	char *n_par=mymalloc(1024);
		if (n_par==0) return (u32)-1;
	if (NumberOfFloors==0)//第一次进入这里时申请内存
	{
		cScriptStack.stack_base=mymalloc(1024);
		if (cScriptStack.stack_base==0)
		{
			myfree(n_par);
			return (u32)-2;
		}
		Arry=mymalloc (32);
		{
			if (Arry==0)
			{
				myfree(n_par);
				myfree(cScriptStack.stack_base);
				return (u32)-3;
			}
		}
	}
	NumberOfFloors++;//增加一层调用
	char *next_par=par;
	do
	{
		next_par=findSemicolon(n_par,next_par);//提取一个执行条目
		ret=checkCategory(n_par);//执行
		
	}while(*next_par);
	myfree(n_par);
	NumberOfFloors--;//减少一层调用
	if (NumberOfFloors==0)		//最后一层调用时返回
	{
		myfree(cScriptStack.stack_base);
		myfree(Arry);
		Arry=0;
		cScriptStack.stack_base=0;
		cScriptStack.offset=0;
	}
	return ret;
}




//运行指定函数,最多可以设置8个参数
u32 runFunction (char *Parameters)
{
	u32 fun_addr=0;
	
	
	
	
	
	u32 par_value[8]={0};
	char *(par_str[8])={0};
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
		
		//区分函数参数
		u8 brackets=0;
		while (*par)
		{
			if (*par=='(')
			{
				brackets++;
			}
			else if (*par==')')
			{
				brackets--;
			}
			if (brackets==0)//
			{
				if (*par==',')
				{
					*par=0;
					par_num++;
					par_str[par_num-1]=par+1;
				}
			}
			par++;
		}
		if (par_num>8) 
		{
			myfree(my_par);
			return 0;
		}
		
		//给每个参数赋值
		for (u8 i=0;i<par_num;i++)
		{
			par_value[i]=checkCategory(par_str[i]);//校验数据类型
		}
	}
	
	//ret=BlxExternFun(par_value[0],par_value[1],par_value[2],par_value[3],fun_addr|1);
	ret=BlxExternFun(par_value[0],par_value[1],par_value[2],par_value[3],
		par_value[4],par_value[5],par_value[6],par_value[7],
		fun_addr|1);
	
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
			return (u32)FUN_LIST[i].fun_addr;
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
		for (u8 i=2;*(str+i);i++)
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
	else if (*str=='(')
	{
		if (str[strlen(str)-1]==')')
		{
			return strTypeArry;
		}
		else
		{
			return strTypeErr;
		}
	}
	return strTypeErr;
}





//通过索引得到函数名
char *getFunNameByIndex (u16 index)
{
	extern fun_list FUN_LIST[]; 
	return FUN_LIST[index].fun_name;
}



