
#include "includes.h"
#include "cScript.h"



/*****************************************************************

				�Զ���ű�������
				2019.4.23
				���ߣ�Ƚ��
				�����ַ�������֧�ֵĳ������Ϊ31�ֽ�
				�����������֧����󳤶�Ϊ31�ֽ�
				�������Դ�����ʱ���飬���֧��һ��
				����ʽ��֧�������������ȼ�
				��֧��i++��i--����
				�ű�������ͬʱֻ����һ���߳����У�
				�ؼ��֣�
					local �������
					while whileѭ��
					if else ������֧
				�﷨��
				ÿ������Էֺ�;��β
				while ѭ�� if ��֧ ��Ҫ�ô�����{}�������������ź�ӷֺ�
				��else�ĳ�if���ֻ��else��֧�������żӷֺ�
				�������������г�ʼֵ�����ݳ�ʼֵ��ȷ����������
				�����ַ��� local a="abcd";
				�������� local b=(1,2,3,4,5);
				�������� local c=520;
				�����ú�������ֵ��Ϊ��ʼֵ������ local d=fun();
				while ѭ��
				while (...)
				{
					...;
					...;
					...;
				};
				if �������
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
				��
				if (...)
				{
					...;
					...;
					...;
				};

********************************************************************/



//�ű����еı���ջ
cscript_stack cScriptStack={0};

static	u8 *Arry=0;//��ʱ���飬һ����������ֻ��ʹ��һ����ʱ����


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
		if (n_str[0]==';')//�����ź��зֺţ�û�з�֧
		{
		}
		else				//�з�֧
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
	inbuff=inbuff+strlength+1;//�����ֺ�
	return inbuff;
}


//��ӱ���
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
			if (a.varType==varTypeNum)//�޸�
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

//����
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
		//return checkCategory(buf);//û��������ţ�ֱ��ִ��
	}
	return 0;
}






//whileѭ��
u32 loopWhile (char *str)
{
	char *n_str=str+1;//ȥ��������
	u32 ret=0;
	char loop_par[32]={0};//ѭ������
	mymemcpy (loop_par,n_str,strlenByChar(')',n_str));//����ѭ��������ȥ��������
	
	n_str+=strlenByChar('{',n_str);//��λ�������Ŵ�
	n_str[findPair('{','}',n_str)-1]=0;//ȥ���Ҵ�����
	n_str+=1;//ȥ���������
	while (checkCategory(loop_par))
	{
		ret=runCScript(n_str);//�ظ�����
	}
	return ret;
}


//if�ж�,��֧��else if��֧
u32 judgeIf (char *str)
{
	char *n_str1=str+1;//ȥ��������
	char *n_str2=str+1;
	u16 str2_offset=0;
	u32 ret=0;
	char judge_par[32]={0};//�жϲ���
	mymemcpy (judge_par,n_str1,strlenByChar(')',n_str1));//�����жϲ�����ȥ��������
	
	n_str1+=strlenByChar('{',n_str1);//��λ�������Ŵ�
	str2_offset=findPair('{','}',n_str1);
	n_str1[str2_offset-1]=0;//ȥ���Ҵ�����
	n_str2=&n_str1[str2_offset];
	n_str1+=1;//ȥ���������
	
	if (*n_str2)
	{
		n_str2+=strlenByChar('{',n_str2);//��λ�������Ŵ�
		n_str2[findPair('{','}',n_str2)-1]=0;//ȥ���Ҵ�����
		n_str2+=1;//ȥ���������
	}
	
	if (checkCategory(judge_par))
	{
		ret=runCScript(n_str1);//�ظ�����
	}
	else if (*n_str2)
	{
		ret=runCScript(n_str2);//�ظ�����
	}
	return ret;
	
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
		if (len>2000) len=0;
		mymemcpy(name,str_par,len);
		str_par+=len+1;//�����Ⱥ�
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
		addVariable(name,(void *)checkCategory(str_par),var_type);//�������strָ����ڴ汻�ͷţ������bug
		
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
			str_par=getStr(str_par);
			u8 lenstr=strlen(str_par);
			if (lenstr>32) lenstr=32;
			mymemcpy(Arry,str_par,lenstr);
			Arry[lenstr]=0;//�ض��ַ���
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
	else if (len=strlenByChar('=',str_par),len<2000)				//�Ǹ���ֵ���
	{
		if (str_par[len+1]=='=')			//��һ��Ҳ�ǵȺ�,�ж����
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
	else if (len=strlenByChar('!',str_par),len<2000)				//�Ǹ���ֵ���
	{
		if (str_par[len+1]=='=')			//��һ���ǵȺ�,�жϲ����
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
			��Ϊ��������ĺ����������ڶ���߳���ͬʱ���нű�

***************************************************/
static u8 NumberOfFloors=0;
 

u32 runCScript (char *par)
{
	u32 ret=0;
	char *n_par=mymalloc(1024);
		if (n_par==0) return (u32)-1;
	if (NumberOfFloors==0)//��һ�ν�������ʱ�����ڴ�
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
	NumberOfFloors++;//����һ�����
	char *next_par=par;
	do
	{
		next_par=findSemicolon(n_par,next_par);//��ȡһ��ִ����Ŀ
		ret=checkCategory(n_par);//ִ��
		
	}while(*next_par);
	myfree(n_par);
	NumberOfFloors--;//����һ�����
	if (NumberOfFloors==0)		//���һ�����ʱ����
	{
		myfree(cScriptStack.stack_base);
		myfree(Arry);
		Arry=0;
		cScriptStack.stack_base=0;
		cScriptStack.offset=0;
	}
	return ret;
}




//����ָ������,����������8������
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
		
		//���ֺ�������
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
		
		//��ÿ��������ֵ
		for (u8 i=0;i<par_num;i++)
		{
			par_value[i]=checkCategory(par_str[i]);//У����������
		}
	}
	
	//ret=BlxExternFun(par_value[0],par_value[1],par_value[2],par_value[3],fun_addr|1);
	ret=BlxExternFun(par_value[0],par_value[1],par_value[2],par_value[3],
		par_value[4],par_value[5],par_value[6],par_value[7],
		fun_addr|1);
	
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
			return (u32)FUN_LIST[i].fun_addr;
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





//ͨ�������õ�������
char *getFunNameByIndex (u16 index)
{
	extern fun_list FUN_LIST[]; 
	return FUN_LIST[index].fun_name;
}



