

#ifndef CScript_H__
#define CScript_H__


#ifdef  __cplusplus
	extern "C" {
#endif


		
//�������ͽṹ��
typedef struct
{
	u32 varType;
	void *varValue;
	char varName[16];
	
}	cscript_variable;


//����ջ�ṹ��
typedef struct 
{
	void *stack_base;
	u32 offset;
} cscript_stack;




enum
{
	varTypeNum=0,
	varTypeStr=1,
};
		


enum
{
	strTypeErr=0,
	strTypeChar=1,
	strTypeStr=2,
	strTypeNum=3,
	strTypeHex=4,
	strTypeFun=5,
	strTypeVar=6,
};



typedef struct 
{
	u32 fun_addr;
	char *fun_name;
}fun_list;




//���Һ������ĵ�ַ
u32 findFunAddr (char *fun_name);

//����ָ������,����������4������
u32 runFunction (char *Parameters);



#ifdef  __cplusplus
	}
#endif



#endif


