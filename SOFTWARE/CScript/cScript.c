
#include "includes.h"
#include "cScript.h"


//�ű����еı���ջ
cscript_stack cScriptStack={0};

//�ҵ�һ���ֺŽ�β���ַ���������
char* findSemicolon (char *outbuff,char *inbuff);

//�ж������Ŀ������
u32 checkCategory (char *str);

//��ӱ���
int addVariable (char *name,void *value,u32 type);

//�޸ı���
int cheVariable (char *name,void *value);

//��ȡ����
u32 getVariable (char *name);

//����
u32 alculation (char *buf);




//�ҵ�һ���ֺŽ�β���ַ���������
char* findSemicolon (char *outbuff,char *inbuff)
{
	int strlength=0;
	
	while(*inbuff)		//�ֺ�֮��ķ���ĸ�ַ�ȫ������
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
	inbuff=inbuff+strlength+1;//�����ֺ�
	return inbuff;
}


//��ӱ���
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


//�ҵ���Ӧ�ı������޸ģ��ɹ�����1��ʧ�ܷ���0
int cheVariable (char *name,void *value)
{
	cscript_variable a={0};
	u16 offset=cScriptStack.offset;
	while(offset--)
	{
		mymemcpy(&a,&((cscript_variable*)cScriptStack.stack_base)[offset],sizeof(a));//ȡֵ
		if (samestr((u8*)name,(u8*)a.varName))
		{
			a.varValue=value;					//�޸�
			mymemcpy(&((cscript_variable*)cScriptStack.stack_base)[offset],&a,sizeof(a));//д��
			return 1;
		}
	}
	return 0;
}


//��ָ���������з���1���޷���0
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






//��ȡ����
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

//����
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
		//return checkCategory(buf);//û��������ţ�ֱ��ִ��
	}
	return 0;
}




//�ж������Ŀ�����Ͳ�ִ��
u32 checkCategory (char *str)
{
	char *str_par=str;
	char name[16]={0};
	int len=0;
	u32 ret=0;
	u8 str_type=0;
	if (samestr((u8*)"local ",(u8*)str))							//�������
	{
		str_par=str+6;
		len=strlenByChar('=',str_par);
		mymemcpy(name,str_par,len);
		str_par+=len+1;//�����Ⱥ�
		if (checkStrType(str_par)==strTypeStr)
		{
			addVariable(name,str_par,varTypeStr);//�������strָ����ڴ汻�ͷţ������bug
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
																										//�ַ�����������
	else if (str_type=checkStrType(str_par),str_type!=strTypeErr)							
	{
		if (str_type==strTypeFun)																//���к���
		{
			ret=runFunction(str_par);
		}
		else if (str_type==strTypeNum)							//�Ǹ�����
		{
			ret=str2num((u8*)str_par);
		}
		else if (str_type==strTypeHex)							//hex����
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
	else if (len=strlenByChar('=',str_par),len)				//�Ǹ���ֵ���
	{
		mymemcpy(name,str_par,len);
		cheVariable(name,(void *)checkCategory(str_par+len+1));
		ret=getVariable(name);
	}
	else if (findVariable(str_par))													//��ʾ�������
	{
		ret=getVariable(str_par);
	}
	else																										//ʲô�����ǣ�ִ�м���
	{
		ret=alculation(str_par);
	}
	return ret;
}








/***************************************************

			���нű���

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
		next_par=findSemicolon(n_par,next_par);//��ȡһ��ִ����Ŀ
		ret=checkCategory(n_par);//ִ��
		
	}while(*next_par);
	
	myfree(n_par);
	myfree(cScriptStack.stack_base);
	cScriptStack.stack_base=0;
	cScriptStack.offset=0;
	return ret;
}





//����ָ������,����������4������
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
					par+=fun_addr_str_ptr-fun_addr_str+1;//ʹָ��ָ����������
					break;
				}
			}
			if (*fun_addr_str=='0')//�����ǵ�ַ
			{
				fun_addr=str2hex(fun_addr_str);
			}
			else			//���ĺ�����
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
			par[strlen(par)-1]=0;//ȥ��������
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
		
		//���ֺ�������
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
		
		//��ÿ��������ֵ
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
			par_value[i]=checkCategory(par_str[i]);//У����������
		}
	}
	
	ret=BlxExternFun(par_value[0],par_value[1],par_value[2],par_value[3],fun_addr|1);
	
	myfree(my_par);
	return ret;
}




//���Һ������ĵ�ַ
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


//У��str������
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





