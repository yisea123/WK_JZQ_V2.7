

#ifndef CScript_H__
#define CScript_H__


#ifdef  __cplusplus
	extern "C" {
#endif

		
		
//�ű������������ı�		
#define cScriptHelpTxt "�����ַ�������֧�ֵĳ������Ϊ31�ֽ�\r\n"\
				"�����������֧����󳤶�Ϊ31�ֽ�\r\n"\
				"�������Դ�����ʱ���飬���֧��һ��\r\n"\
				"����ʽ��֧�������������ȼ�\r\n"\
				"��֧��i++��i--����\r\n"\
				"�ű�������ͬʱֻ����һ���߳����У�\r\n"\
				"�ؼ��֣�\r\n"\
				"	local �������\r\n"\
				"	while whileѭ��\r\n"\
				"	if else ������֧\r\n"\
				"�﷨��\r\n"\
				"ÿ������Էֺ�;��β\r\n"\
				"while ѭ�� if ��֧ ��Ҫ�ô�����{}�������������ź�ӷֺ�\r\n"\
				"��else�ĳ�if���ֻ��else��֧�������żӷֺ�\r\n"\
				"�������������г�ʼֵ�����ݳ�ʼֵ��ȷ����������\r\n"\
				"�����ַ��� local a=\"abcd\";\r\n"\
				"�������� local b=(1,2,3,4,5);\r\n"\
				"�������� local c=520;\r\n"\
				"�����ú�������ֵ��Ϊ��ʼֵ������ local d=fun();\r\n"\
				"while ѭ��\r\n"\
				"while (...)\r\n"\
				"{\r\n"\
				"	...;\r\n"\
				"	...;\r\n"\
				"	...;\r\n"\
				"};\r\n"\
				"if �������\r\n"\
				"if (...)\r\n"\
				"{\r\n"\
				"	...;\r\n"\
				"	...;\r\n"\
				"	...;\r\n"\
				"}\r\n"\
				"else\r\n"\
				"{\r\n"\
				"	...;\r\n"\
				"	...;\r\n"\
				"	...;\r\n"\
				"};\r\n"\
				"��\r\n"\
				"if (...)\r\n"\
				"{\r\n"\
				"	...;\r\n"\
				"	...;\r\n"\
				"	...;\r\n"\
				"};\r\n"
		
		
		
		
		
		
		
//�������ͽṹ��
typedef struct
{
	u32 varType;
	void *varValue;
	char varName[16];
	u8 arry[32];
	
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
	varTypeArry=2,
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
	strTypeArry=7,
};



typedef struct 
{
	void * fun_addr;
	char *fun_name;
}fun_list;





//���нű�
u32 runCScript (char *par);

//���Һ������ĵ�ַ
u32 findFunAddr (char *fun_name);

//����ָ������,����������4������
u32 runFunction (char *Parameters);

//ͨ�������õ�������
char *getFunNameByIndex (u16 index);


#ifdef  __cplusplus
	}
#endif



#endif


