#include "sys.h"
#include "my_messeg.h"



	//�Ѵ���ת��Ϊ�ַ���
char *err_to_str(u16 err)
{
	switch (err)
	{
		case ERR_SUCCESS:
			return "�ɹ�";
		case ERR_FAIL:
			return "δ֪����";
		case ERR_DATAVALE:
			return "���ݴ���";
		case ERR_DATALENGTH:
			return "���ݳ��ȴ���";
		case ERR_WANTLENGTH:
			return "��Ҫ��ȡ�����ݳ��ȴ���";
		case ERR_NULLCMD:
			return "��֧�ֵ���������";
		case ERR_NOCONNECT:
			return "û������";
		case ERR_NOCJADDR:
			return "�ɼ����ĵ�ַ���Ǳ����󶨵Ĳɼ�����ַ";
		case ERR_CANNOTCFG:
			return "�ڲ��ǿ�����״̬���յ�����������";
		case ERR_NOTMYTYPE:
			return "��Ҫ���Ƶ��豸�뱾������";
		case ERR_TIMEOUT:
			return "��ʱ";
		case ERR_OFFLINE:
			return "�豸������";
		case ERR_NONEADDR:
			return "�豸��ַ����";
		case ERR_ADDRCROSS:
			return "����豸ʱ��ַԽ��";
		default:
			return "��������";
	}
}




