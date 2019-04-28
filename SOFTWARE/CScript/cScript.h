

#ifndef CScript_H__
#define CScript_H__


#ifdef  __cplusplus
	extern "C" {
#endif

		
		
//脚本解析器帮助文本		
#define cScriptHelpTxt "声明字符串变量支持的长度最大为31字节\r\n"\
				"声明数组变量支持最大长度为31字节\r\n"\
				"函数可以传递临时数组，最多支持一个\r\n"\
				"运算式不支持括号重设优先级\r\n"\
				"不支持i++，i--运算\r\n"\
				"脚本解析器同时只能由一个线程运行，\r\n"\
				"关键字：\r\n"\
				"	local 定义变量\r\n"\
				"	while while循环\r\n"\
				"	if else 条件分支\r\n"\
				"语法：\r\n"\
				"每个语句以分号;结尾\r\n"\
				"while 循环 if 分支 都要用大括号{}包括并在右括号后加分号\r\n"\
				"有else的陈if语句只在else分支的右括号加分号\r\n"\
				"变量声明必须有初始值，根据初始值来确定数据类型\r\n"\
				"声明字符串 local a=\"abcd\";\r\n"\
				"声明数组 local b=(1,2,3,4,5);\r\n"\
				"声明整数 local c=520;\r\n"\
				"声明用函数返回值作为初始值的整数 local d=fun();\r\n"\
				"while 循环\r\n"\
				"while (...)\r\n"\
				"{\r\n"\
				"	...;\r\n"\
				"	...;\r\n"\
				"	...;\r\n"\
				"};\r\n"\
				"if 条件语句\r\n"\
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
				"或\r\n"\
				"if (...)\r\n"\
				"{\r\n"\
				"	...;\r\n"\
				"	...;\r\n"\
				"	...;\r\n"\
				"};\r\n"
		
		
		
		
		
		
		
//变量类型结构体
typedef struct
{
	u32 varType;
	void *varValue;
	char varName[16];
	u8 arry[32];
	
}	cscript_variable;


//变量栈结构体
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





//运行脚本
u32 runCScript (char *par);

//查找函数名的地址
u32 findFunAddr (char *fun_name);

//运行指定函数,最多可以设置4个参数
u32 runFunction (char *Parameters);

//通过索引得到函数名
char *getFunNameByIndex (u16 index);


#ifdef  __cplusplus
	}
#endif



#endif


